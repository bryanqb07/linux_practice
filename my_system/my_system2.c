#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h> 
#include <string.h>
#include <stdlib.h>


int runCommand(const char *command){
  pid_t pid = fork();
  if(pid == 0){
    char *arguments[] = {"/bin/sh", "-c", (char *) command, NULL };
    execvp(arguments[0], arguments);
    printf("File closed abnormally");
    exit(0);
  }
  int status;
  waitpid(pid, &status, 0);
  return WIFEXITED(status) ? WEXITSTATUS(status) : -WTERMSIG(status);
}


const int kMaxCommandLength = 1028;

int main(int argc, char *argv[]){
  char command[kMaxCommandLength];
  while(true){
    printf(">> ");
    fgets(command, kMaxCommandLength, stdin);
    if(feof(stdin)) break;
    command[strlen(command) - 1] = '\0'; // overwrite new line character
    runCommand(command);
  }
  return 0;
}
