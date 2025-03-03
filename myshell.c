// General Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>





#ifdef _WIN32 // Windows Headers
#include <windows.h>
#include <direct.h>
#include <limits.h>

// char* getDir(){
//     char *cwd[MAX_PATH];

//     if(_getcwd(cwd, MAX_PATH) == NULL){
        
//     }



// }

void executeCommand(char *command){
    printf("Running on Windows\n");
}


#else // masOS/Linux Headers
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PATH 256

// Function to Runs macOS and Linux Commands
void executeCommand(char *args[]){
    pid_t pid = fork();
    if ( pid == -1){
        perror("Fork Failed- something went wrong");
        return;
    }
    if( pid == 0 ){
        if(execvp(args[0], args) == -1){
            perror("Oops! Command execution failed.");
        }
        exit(EXIT_FAILURE);
    }else{
        wait(NULL);
    }
}
// Function to get PATH to current directory
char* getDir(){
    char *cwd = malloc(MAX_PATH);
    if(cwd == NULL){
        perror("Ran out of memory");
        return NULL;
    }
    if (getcwd(cwd, MAX_PATH) != NULL){
        return cwd;
    }else{
        perror("Uh-oh! Unable to get current directory");
        free(cwd);
        return NULL;
    }
}
#endif


#define MAX_INPUT_SIZE 256
#define MAX_ARGS 10

// Function to get the current folder
char* getCurrentFolder(){

    char* cwd = getDir();
    if(cwd == NULL) return NULL;

    char* ptr = strrchr(cwd, '/');
    char *folder = NULL;

    if(ptr != NULL && *ptr+1 != '\0'){
        folder = strdup(ptr+1);
        
    }
    
    free(cwd);
    return folder;
}

void changeDirectory(char *path){
    if(path == NULL){
        printf("No PATH given, Taking you to Home\n");
        path = getenv("HOME");
    }
    if(chdir(path) != 0){
        perror("error changing directory");
 
    }
}
void printDirectory(){
    char *cwd = getDir();
    if(cwd){
        printf("You are here: %s\n",cwd);
        free(cwd);
    }
}
void echoCommand(char *args[]){

    int i = 0;
    while(args[i] != NULL){
        printf("%s ", args[i]);
        i++;
    }
    printf("\n"); 

}
void helpCommand(){
    printf("cd <dir> - Change directory\n");
    printf("help - Show all built in commands\n");
    printf("exit - Exit Shell\n");
    printf("cd <dir> - Change directory\n");
    printf("pwd - Print current directory\n");
}
void exitCommand(){
    printf("Exiting...\n");
    exit(0);

}


void builtinCommand(char *args[]){
    if(strcmp(args[0],"cd") == 0){
        changeDirectory(args[1]);
    }
    else if (strcmp(args[0],"help") == 0){
        helpCommand();
    }
    else if (strcmp(args[0],"exit") == 0){
        exitCommand();
    }
    else if (strcmp(args[0],"pwd") == 0){
        printDirectory();
    }
    else if (strcmp(args[0],"echo") == 0){
        echoCommand(args + 1);
    }
    else{
        executeCommand(args); 
    }


}



int main(int argc, char const *argv[])
{

    char commandString[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    printf("Shell is running\n");

    while (1)
    {
        char *cwd = getCurrentFolder();
        
        printf("%s - mysh>>> ", cwd);
        fgets(commandString, MAX_INPUT_SIZE, stdin);
        commandString[strcspn(commandString,"\n")]= 0;

        // if (strcmp(commandString, "exit")==0) break;
#ifdef _WIN32
        break;
#else 
        char *token = strtok(commandString," ");
        int i = 0;
        while (token != NULL){
            args[i++] = token;
            token = strtok(NULL," ");
        }
        args[i] = NULL;
        builtinCommand(args);
#endif
    
    }
        return 0;
}
