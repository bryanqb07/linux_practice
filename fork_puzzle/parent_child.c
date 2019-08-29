#include <stdbool.h> // for bool
#include <stdlib.h> // exit, srandom, random
#include <unistd.h> // fork
#include <stdio.h>
#include <sys/wait.h> // waitpid
#include <time.h> // for time

static const int kForkFailure = 1;

int main(int argc, char *argv[]){
  srandom(time(NULL));
  printf("I am unique and just get printed once.\n");
  pid_t pid = fork();
  bool parent = pid != 0;
  if((random() % 2 == 0) == parent) sleep(1);
  if (parent) waitpid(pid, NULL, 0); // parent waits for child
  printf("I get printed twice (this one is from %s).\n", parent ? "parent" : "child");
  return 0;
}
