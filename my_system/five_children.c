#include <stdio.h>
#include <sys/wait.h> // waitpid
#include <sys/types.h> // fork
#include <unistd.h> // sleep
#include <stdlib.h>

static const int kSignalFailed = 1;
static const int kForkFailed = 2;
static const int kWaitFailed = 3;

static const size_t kNumChildren = 5;
static size_t numChildrenDonePlaying = 0;

static void reapChild(int sig){
  waitpid(-1, NULL, 0); // clean up child after done 
  numChildrenDonePlaying++;
}

int main(int argc, char *argv[]){
  printf("Let my five children play while I take a nap.\n");
  signal(SIGCHLD, reapChild); // tell signal which handler to use
  for(size_t kid = 1; kid <= 5; kid++){
    if( fork() == 0){ // if child
      sleep(3 * kid); // sleep emulates playing
      printf("Child %zu tired. Returns to dad.\n", kid);
      return 0;
    }
  }
  // parent
  while (numChildrenDonePlaying < kNumChildren){
    printf("At least one child is still playing, so dad nods off.\n");
    sleep(5);
    printf("Dad wakes up!\n");
  }
  printf("All children accounted for. Good job dad!\n");
  return 0;
}
