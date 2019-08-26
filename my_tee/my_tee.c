#include <fcntl.h> // for open 
#include <unistd.h> // for read, write, close
#include <stdbool.h> // for bool type

//O_TRUNC => if file exists, wipe it out before writing to it
#define DEFAULT_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define DEFAULT_PERMISSIONS 0644

static void writeall(int fd, const char buffer[], size_t len){
  size_t numWritten = 0;
  while (numWritten < len){
    // write(source, start point, num of chars remaining)
    numWritten += write(fd, buffer + numWritten, len - numWritten);
  }
}

int main(int argc, char *argv[]){
  int fds[argc]; // create argc number of files
  // STDOUT_FILENO, STDIN_FILENO, STDERR_FILENO all pre-exist
  fds[0] = STDOUT_FILENO; // we need to write to standard output and send back to user
  for (size_t i = 1; i < argc; i++){
    fds[i] = open(argv[i], DEFAULT_FLAGS, DEFAULT_PERMISSIONS);
  }
  char buffer[2048]; // read 2048 bytes at a time 
  while(true){
    ssize_t numRead = read(STDIN_FILENO, buffer, sizeof(buffer));
    if(numRead == 0) break; // if finished
    for(size_t i = 0; i < argc; i++) writeall(fds[i], buffer, numRead);
  }
  for (size_t i = 0; i < argc; i++) close(fds[i]); // close all the files

  return 0;
}
