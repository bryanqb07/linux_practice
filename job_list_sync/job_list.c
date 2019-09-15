#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

static const int kForkFailed = 1;
static const int kExecFailed = 2;
static const int kWaitFailed = 4;
static const int kSignalFailed = 8;


static void reapProcesses(int sig){
  while(true){
    pid_t pid = waitpid(-1, NULL, WNOHANG);
    if(pid <= 0) break;
    printf("Job %d removed from the job list.\n", pid);
  }
}

char * const kArguments[] = {"date", NULL};


int main(int argc, char *argv[]){
  signal(SIGCHLD, reapProcesses); // set up signal handler
  sigset_t set;
  sigemptyset(&set); //initialize empty set
  sigaddset(&set, SIGCHLD); // add child signals to set
  for(size_t i = 0; i < 3; i++){
    sigprocmask(SIG_BLOCK, &set, NULL); // block signals
    // any blocked signals in parent are also blocked in children
    pid_t pid = fork();
    if(pid == 0){
      sigprocmask(SIG_UNBLOCK, &set, NULL); // don't block child process of child
      // don't want date function's children to be blocked
      // only care about blocking parent's children's signals 
      execvp(kArguments[0], kArguments);
    }
    sleep(1); // forces the parent off the CPU
    printf("Job %d added to the job list.\n", pid);
    sigprocmask(SIG_UNBLOCK, &set, NULL); // now signal handler can be called
  }

  return 0;
}
