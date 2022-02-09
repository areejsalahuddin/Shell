#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
void execute_inbackground(char *cmdline[]){
    pid_t pid = fork();
    if (pid == -1) {
        printf("Failed!");
        return;
    } else if (pid == 0) {
        if (execvp(cmdline[0], cmdline) < 0) {
            printf("Invalid Command\n");
        }
    }

}
void execute_Command(char *cmdline[] , int is_background){
    if (strcmp(cmdline[0], "exit") == 0) {
        printf("\n========Terminal Closed=========\n");
        exit(0);

    }
    if(strcmp(cmdline[0],"cd")==0)
    {   //check if directory exists and if yes it will change it
        if(chdir(cmdline[1]) < 0)
        {
            printf("Directory doesn't exist!\n");
        }
        return;
    }
    else if(is_background == 1){
        execute_inbackground(cmdline);
    }
    else {
        pid_t pid = fork();
        if (pid == -1) {
            printf("Failed!");
            return;
        } else if (pid == 0) {
            //check if command exists and execute it
            if (execvp(cmdline[0], cmdline) < 0) {
                printf("Invalid Command\n");
                exit(0);
            }
        } else {
            // blocking until child terminates
            wait(NULL);
            return;
        }
    }
}



void parse_execute(char *user_input){
    char *token;
    char *cmdline[50];
    int i=0;
    int is_background=0;
    token = strtok(user_input, " ");
    while( token != NULL )
    {
        if(token!=NULL && strcmp(token,"&")==0)
        {
            is_background = 1;
            break;
        }
        cmdline[i++] = token;
        token = strtok(NULL, " ");
    }
    cmdline[i] = NULL;

    execute_Command(cmdline,is_background);

}


void log_handler()
{
    FILE *ptr;
    ptr = fopen("logfile.txt","a");
    fprintf(ptr,"Child Process Terminated!\n");
    fclose(ptr);
}
int main(){
    char user_input[20];
    while (1){
        signal(SIGCHLD,log_handler);
        printf("myShell@areej> ");
        if((fgets(user_input,20,stdin)==NULL) && ferror(stdin)){
            perror("Can't read line");
        }
        if(feof(stdin)){
            printf("\n");
            exit(0);
        }
        parse_execute(user_input);
    }
    return (0);
}