#include <stdbool.h> //for bool
#include <stddef.h> // for NULL
#include <stdio.h> // for printf
#include <sys/types.h> // for mode_t, uid_t
#include <sys/stat.h> //for lsat
#include <dirent.h> //for DIR, opendir
#include <string.h> // strlen, strcpy
#include <pwd.h> // getpwuid
#include <grp.h> //getgrgid
#include <time.h> // time, localtime
#include <unistd.h> // readlink


static void listEntries(const char *entries[], size_t numEntries){
  
}


static const char *kDefaultEntries[] = {".", NULL};
int main(int argc, char *argv[]){
  const char **entries = argc == 1 ? kDefaultEntries : (const char **) argv + 1; 
  size_t numEntries = argc == 1 ? 1 : argc - 1;
  listEntries(entries, numEntries);
  return 0;
}
