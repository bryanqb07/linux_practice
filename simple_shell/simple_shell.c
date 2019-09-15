#include <stdlib.h> //exit
#include <stdio.h> // printf
#include <stdbool.h>
#include <string.h> // strchr, strcmp
#include <unistd.h> // fork, exeve
#include <sys/wait.h> // waitpid
#include <signal.h>


static void toggleBLOCKCHLDBlock(int how){
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGCHLD);
  sigprocmask(how, &mask, NULL);
}

void blockSIGCHILD(){
  toggleSIGCHLDblock(SIG_BLOCK);
}

void unblockSIGCHILD(){
  toggleSIGCHLDblock(SIG_UNBLOCK);
}

static pid_t fgpid = 0; // gloabl

static void reapProcesses(int sig){
  while(true){
    pid_t pid = waitpid(-1, NULL, WNOHANG);
    if (pid <= 0) break;
    if (pid == fgpid) fgpid = 0; // clear foreground process
  }
}

static void waitForForegroundProcess(pid_t pid){
  fgpid = pid;
  sigset_t empty;
  sigemptyset(&empty);
  while (fgpid == pid) {
    sigsuspend(&empty);
  }
  unblockSIGCHLD();
}

int main(int argc, char *argv[]){
  signal(SIGCHLD, reapProcesses);
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
    blockSIGCHLD();
    pid_t pid = fork();
    if (pid == 0){
      unblockSIGCHLD();
      execvp(arguments[0], arguments);
      printf("%s: Command not found\n", argv[0]); //error checking
      exit(0);
    }
    if (isbg){ // if background don't wait for child to finish
      printf("%d %s\n", pid, command);
      unblockSIGCHLD();
    }else{ // don't do anything until child process ends
      waitForForegroundProcess(pid);
    }
  }

}
