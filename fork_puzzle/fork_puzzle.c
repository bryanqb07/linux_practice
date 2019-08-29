#include <unistd.h> // fork
#include <stdio.h> // printf
#include <string.h> // for strlen
#include <assert.h>

static const char *kTrail = "abcd";

int main(int argc, char *argv[]){
  size_t trailLength = strlen(kTrail);
  for(size_t i = 0; i < trailLength; i++){
    printf("%c\n", kTrail[i]);
    pid_t pid = fork();
    assert(pid >= 0);
  }
  return 0;
}
