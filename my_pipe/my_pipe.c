#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static const int kPipeFailed = 1;
static const int kForkFailed = 2;

int main(int argc, char *argv[]){
  int fds[2]; // set up file descriptor for pipe
  // fds[0] is the reader
  // fds[1] is the writer
  pipe(fds);
  pid_t pid = fork();
  if (pid == 0) { // child
    // read a pipe
    // child will read from fds[0[
    close(fds[1]); // no need to write
    char buffer[6];
    read(fds[0], buffer, sizeof(buffer)); // read will wait 
    printf("Read from pipe bridging the processes %s\n", buffer);
    close(fds[0]); // done reading
    return 0;
  }
  // parent
  // write into pipe
  close(fds[0]); // no reading necessary
  write(fds[1], "hello", 6);
  waitpid(pid, NULL, 0);
  close(fds[1]);
  
  return 0;
}
