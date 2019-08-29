#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

static const int kForkFailed = 1;
static const int kWaitFailed = 2;

int main(int argc, char *argv[]){
  printf("Before.\n");
  pid_t pid = fork();
  printf("After.\n");
  if (pid == 0) { //child
    printf("I am the child, and the parent will wait up for me.\n");
    int *a = NULL;
    *a = 5;
    return 110; //random num
  }
  else{ // parent 
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)){
      printf("Child exited with status %d.\n", WEXITSTATUS(status));
    }else{
      printf("Child terminated abnormally");
    }
    return 0; 
  }  
}
