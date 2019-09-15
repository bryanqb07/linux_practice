#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define DEFAULT_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define DEFAULT_PERMISSIONS 0644

static const int kWrongArgumentCount = 1;

int main(int argc, char *argv[]){
  if(argc < 2){
    fprintf(stderr, "%s <destination file>. \n", argv[0]);
    return kWrongArgumentCount;
  }

  int fds[argc];
  fds[0] = STDOUT_FILENO;
  for(int i = 1; i < argc; i++) fds[i] = open(argv[i], DEFAULT_FLAGS, DEFAULT_PERMISSIONS);
  

  while(true){
    char buffer[1024];
    ssize_t bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (bytesRead == 0) break;
    for(int i = 0; i < argc ; i++){
      size_t bytesWritten = 0;
      while(bytesWritten < bytesRead){
	bytesWritten += write(fds[i], buffer + bytesWritten, bytesRead - bytesWritten);
      }
    }
  }

  for(int i = 0; i < argc; i++) close(fds[i]);
  
  return 0;
}
