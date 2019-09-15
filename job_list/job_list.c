#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

static const int kForkFailed = 1;
static const int kExecFailed = 2;
static const int kWaitFailed = 4;
static const int kSignalFailed = 8;

static void reapProcesses(int sig){
  while(true){
    pid_t pid = waitpid(-1, NULL, WNOHANG);
    if(pid <= 0) break;
    printf("Job %d removed from job list\n", pid);
  }
}

char * const kArguments[] = {"date", NULL};

int main(int argc, char *argv[]){
  signal(SIGCHLD, reapProcesses);
  sigset_t set;
  sigemptyset(&set); // intialize set
  sigaddset(&set, SIGCHLD); // wait to block signal

  for(size_t i = 0; i < 3; i++){
    sigprocmask(SIG_BLOCK, &set, NULL); // blocks signals but also passes block to children
    pid_t pid = fork();
    if(pid == 0){ // unblock for childre nol
      sigprocmask(SIG_UNBLOCK, &set, NULL);
      execvp(kArguments[0], kArguments);
    }
    sleep(1); // forces parent off CPU
    printf("Job %d added to job list.\n", pid); 
    sigprocmask(SIG_UNBLOCK, &set, NULL);
  }

  return 0;
}
