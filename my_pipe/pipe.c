#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static const int kPipeFailed = 1;
static const int kForkFailed = 2;

typedef struct {
  pid_t pid; // pid for program 
  int supplyfd; //file descriptor that becomes input for program running
} subprocess_t;

subprocess_t subprocess(const char *command){
  int fds[2];
  pipe(fds);
  subprocess_t process = { fork(), fds[1] };
  if (process.pid == 0){ // child
    // setup to read from file descriptor as stdin
    close(fds[1]); // no writing necessary
    dup2(fds[0], STDIN_FILENO); // stdin points to our new pipe
    close(fds[0]); // already duplicated
    char *argv[] = { "/bin/sh", "-c", (char *) command, NULL };
    // need to cast const, otherwise type incompatibility
    // const char * != char *
    execvp(argv[0], argv);
  }
  // child will never get to here
  close(fds[0]);
  return process;
}

int main(int argc, char *argv[]){
  
  subprocess_t sp = subprocess("/usr/bin/sort"); // starts sort subprocess
  const char *words[] = {
    "felicity", "umbrage", "susurration", "halcyon",
    "pulchritude", "ablution", "somnolent", "indefatigable"
  };

  for(size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++){
    dprintf(sp.supplyfd, "%s\n", words[i]); // print to file descriptor)
  }
  close(sp.supplyfd);
  int status;
  pid_t pid = waitpid(sp.pid, &status, 0);
  return pid == sp.pid && WIFEXITED(status) ? WEXITSTATUS(status) : -127;
  
  return 0;
}
