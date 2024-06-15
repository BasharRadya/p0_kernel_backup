#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

char *get_current_dir_name(void);

void print_pwd()
{
    char *pwd=get_current_dir_name();
    printf("%s$ ",pwd);
    fflush(stdout);
    free(pwd);
}

int sub_split(char* s,int start) {
    int count=0;
    while(!isspace(s[start]) && s[start]!='\0'){
        count++;
        start++;

    }
    return count;
}
char* make_sub_spliot(char* s,int start,int end) {
    int sz=end-start+1;
    char* new_s=(char*)malloc(sizeof(char)*sz);
    for(int i=start;i<end;i++) {
        new_s[i-start]=s[i];
    }
    new_s[sz-1]='\0';
    return new_s;
}
char** split(char* s,int *num_of_words) {
    int length=strlen(s);
    char ** arr=(char**)malloc(sizeof(char*)*length);
    int num_words=0;
    int i=0;
    while(i+1<length) {
        int tmp=sub_split(s,i);
        if(tmp!=0) {
            char *c=make_sub_spliot(s,i,i+tmp);
            arr[num_words]=c;
            num_words++;
            i=i+tmp;
        }else {
            i++;
        }
    }
    *num_of_words=num_words;
    return arr;

}



char** get_input(int* x) {
    char* input=NULL; //force getline to allloc it
    size_t len=0;
    ssize_t byteread=getline(&input,&len,stdin);
    if(byteread==1 && input[0]=='\n') {
        free(input);
        return NULL;
    }else if(byteread!=-1){
        input[strcspn(input,"\n")]='\0'; //delete \n at the end and if tis not their then we are overriding null terminator with null terminator
        char** arr=split(input,x);
        free(input);
        return arr;
    }else {
        exit(0);
    }
}

void free_all(char **c,int num_of_words) {
    for(int i=0;i<num_of_words;i++) {
        free(c[i]);
    }
    free((char*)c);
}
void handle_command(char **arr,int num_words) {
    if(strcmp(arr[0],"exit")==0) {
        exit(0);
    }else if(strcmp(arr[0],"cd")==0) {
        if(num_words>2) {
            printf("Usage cd: cd [dir]\n");
            return;
        }
        if(chdir(arr[1])==-1) {
            printf("Error changing directory errno num %d :%s\n",errno,strerror(errno));
        };

    }else if(strcmp(arr[0],"exec")==0) {
        arr[num_words]=NULL;
        int x= execv(arr[1], &arr[1]);
        printf("error num %d : %s\n",errno,strerror(errno));
        // execv(const char *path, char *const argv[]);
    }else {
        printf("Unrecognized command: %s\n",arr[0]);
    }
    //this should be alwys be last
    free_all(arr,num_words);
}
int main(void) {
    while(1) {
        print_pwd();
        int num_words=0;
        char**arr=get_input(&num_words);
        if(arr==NULL) {
            continue;
        }
        handle_command(arr,num_words);

    }
    return 0;
}
