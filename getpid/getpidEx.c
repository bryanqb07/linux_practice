#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getpid

int main(int argc, char ** argv){
  pid_t pid = getpid(); // pid_t is just an integer
  printf("My process id: %d\n", pid);
  return 0;
}
