#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

static const int kExecFailed = 1;
static int mysystem(const char *command){
  pid_t pid = fork();
  if (pid == 0) { // if child
    char *arguments[] = { "/bin/sh", "-c", (char *) command, NULL };
    execvp(arguments[0], arguments);
    // if failed => should return on its own if succeeded
    printf("Failed to invoke /bin/sh to execute supplied cmd\n"); 
    exit(0);
  }
  // if parent
  int status;
  waitpid(pid, &status, 0);
  // return neg of term status if failed
  return WIFEXITED(status) ? WEXITSTATUS(status) : -WTERMSIG(status);
  return -1;
}

static const size_t kMaxLines = 2048;
int main(int argc, char *argv[]){
  char command[kMaxLines];
  while(true){
    printf("> ");
    // read in from stdin
    fgets(command, kMaxLines, stdin);
    // feof -> reached end of file
    if(feof(stdin)) break;
    command[strlen(command) - 1] = '\0'; // replace \n with null term
    printf("Return code: %d\n", mysystem(command));
  }
  printf("\n");
  return 0;
}
