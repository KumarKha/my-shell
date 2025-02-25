
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#ifdef _WIN32 // Windows
#include <windows.h>
void execute_command(char *command){
    printf("Running on Windows\n");
}


#else // masOS/Linux
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

    
void execute_command(char *args[]){
    pid_t pid = fork();
    if ( pid == -1){
        perror("fork failed ");
        return;
    }
    if( pid == 0 ){
        if(execvp(args[0], args) == -1){
            perror("exec failed");
        }
        exit(EXIT_FAILURE);
    }else{
        wait(NULL);
    }
}
#endif

#define MAX_INPUT_SIZE 256
#define MAX_ARGS 10

int main(int argc, char const *argv[])
{

    char commandString[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    printf("Shell is running\n");

    while (1)
    {
        printf("mysh>>> ");
        fgets(commandString, MAX_INPUT_SIZE, stdin);
        commandString[strcspn(commandString,"\n")]= 0;

        if (strcmp(commandString, "exit")==0) break;
#ifdef _WIN32
        break;
#else 
        char *token = strtok(commandString," ");
        int i = 0;
        while (token != NULL){
            args[i++] = token;
            token =strtok(NULL," ");
        }
        args[i] = NULL;
        execute_command(args);
#endif
    
    }
        return 0;
}
