#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static const int kWrongArgumentCount = 1;
static const int kSourceFileNonExistent = 2;
static const int kDestinationFileOpenFailure = 3;
static const int kreadFailure = 8;
static const int kDefaultPermissions = 0644; // "rw-r--r--"

int main(int argc, char *argv[]){
  if(argc != 3){
    fprintf(stderr, "%s <source-file> <destination-file>.\n", argv[0]);
    return kWrongArgumentCount;
  }
  int fdin = open(argv[1], O_RDONLY);
  int fout = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644);
  while (true) {
      char buffer[1024]; // max # of bytes to be read at one time
      ssize_t bytesRead = read(fdin, buffer, sizeof(buffer));// signed size-t, allows for neg val esp. -1 
      if (bytesRead == 0) break;
      size_t bytesWritten = 0;
      while(bytesWritten < bytesRead){
	bytesWritten += write(fout, buffer + bytesWritten, bytesRead - bytesWritten);
      }
    }
  close(fdin);
  close(fout);
  return 0;
}
