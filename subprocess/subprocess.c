
/* File: subprocess.c */

/* Implements the subprocess routines, which is similar to popen */
/*   and allows the parent process to spawn child process, print */
/*   its standard output, and then suspend until child process */
/* has finished */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct sub_process_t{
  pid_t pid;
  int supplyfd;
} sub_process_t;

sub_process_t subprocess(const char *command){
  int fds[2];
  pipe(fds); 
  sub_process_t process = { fork(), fds[1] };
  if(process.pid == 0){ //child
    close(fds[1]); // no writing necessary
    dup2(fds[0], STDIN_FILENO); // std input replaced by read end of pipe
    close(fds[0]); // already duplicated
    char *argv[] = {"/bin/sh", "-c", (char *) command, NULL};
    execvp(argv[0], argv); // sort where std input = read end of pipe
    // child will never return
  }
  close(fds[0]); // not used in parent
  return process;
  // returns subprocess_t
  //    pid -> child process id
  //    supplyfd -> file reader fds[0]
 
}

int main(int argc, char *argv[]){
  sub_process_t sp = subprocess("/usr/bin/sort"); // sort process
  // waiting for text to be typed in
  const char *words[] = {
    "felicity", "umbrage", "halcyon", "ablution", "somnolent",
    "indefatigable"
  };
  // simulate typing in words to sort
  // words go to supplyfd
  // then get read by stdin
  // sort waits for file to end before doing sorting
  for(size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++){
    dprintf(sp.supplyfd, "%s\n", words[i]); // print to file descriptor
    // print words into file descriptor of struct subprocess
  }
  close(sp.supplyfd); // tell sort to finish
  // also close last instance of fd[0]
  int status;
  pid_t pid = waitpid(sp.pid, &status, 0); // wait for child sort process
  return pid == sp.pid && WIFEXITED(status) ? WEXITSTATUS(status) : -127;
}
