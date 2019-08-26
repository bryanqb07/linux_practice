#include<stdio.h>
#include<sys/types.h> // to use umask
#include<sys/stat.h> 

int main(){
  mode_t old_mask = umask(0);
  umask(old_mask);
  printf("umask is set to %03o", old_mask);

  return 0;
}
