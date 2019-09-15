#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

int main(){
  printf("I am about to terminate my own process\n");

  raise(SIGSTOP);

  return 0;

}
