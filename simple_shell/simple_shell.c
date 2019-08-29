#include <stdlib.h> //exit
#include <stdio.h> // printf
#include <stdbool.h>
#include <string.h> // strchr, strcmp
#include <unistd.h> // fork, exeve
#include <sys/wait.h> // waitpid

int main(int argc, char *argv[]){
  while(true){
    char command[kMxCommandLength + 1];
    readCommand(commmand, kMaxCommandLength);
    char *arguments[kMaxArgumentCount + 1];
    int count = parseCommandLine(command, arguments, kMaxArgumentCount);
    if (count == 0) continue;
    if (strcmp(arguments[0], "quit") == ) break;
    bool isbg = strcmp(arguments[count - 1], "&") == 0; 
    // if last argument is &, let child run in background
    if (isbg) arguments[--count] == NULL; //overwrite &
    pid_t pid = fork();
    if (pid == 0) execvp(arguments[0], arguments);
    if (isbg){ // if background don't wait for child to finish
      printf("%d %s\n", pid, command);
    }else{ // don't do anything until child process ends
      waitpid(pid, NULL, 0);
    }
  }

}
