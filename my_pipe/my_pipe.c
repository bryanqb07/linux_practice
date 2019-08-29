#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

static const int kPipeFailed = 1;
static const int kForkFailed = 2;

// create a pipe, 
// parent will write data that child will read

// fds[0] reader
// fds[1] writer

int main(int argc, char *argv[]){
  int fds[2];
  pipe(fds);
  pid_t pid = fork();
  if (pid == 0) { // child
    // child will read from pipe
    close(fds[1]); // no need to write, don't want parent/child 
    //writing simultaneously
    char buffer[6];
    read(fds[0], buffer, sizeof(buffer));
    printf("Read from pipe bridging the processes: %s.\n", buffer);
    close(fds[0]); // done reading
    return 0;
  }
  // parent
  // parent will write to fds[1]
  close(fds[0]); // no reaading necessary
  write(fds[1], "hello", 6);
  waitpid(pid, NULL, 0);
  close(fds[0]);

  return 0;
}
