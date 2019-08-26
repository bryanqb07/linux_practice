#include <fcntl.h> // for open
#include <unistd.h> // read, write, close
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

const char *kFileName = "my_file.txt";
const int kFileExistsErr = 17;

int main(){
  umask(0); // set permissions to anything, block nothing
  int file_descriptor = open(kFileName, O_WRONLY | O_CREAT | O_EXCL, 0777);
  // file perm w---rw-rw
  if(file_descriptor == -1){
    printf("There was a problem creating '%s'!\n", kFileName);
    if (errno == kFileExistsErr){
      printf("The file already exists!\n");
    }else{
      printf("Unknown errno: %d\n", errno);
    }
    return -1;
  }
  close(file_descriptor);
  return 0;
}
