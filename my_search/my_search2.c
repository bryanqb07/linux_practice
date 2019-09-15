#include <stdbool.h> 
#include <stddef.h> //size_t
#include <stdio.h>  // fprintf
#include <stdlib.h> // exit
#include <stdarg.h> // va_list
#include <sys/stat.h> // stat
#include <string.h> // for strlen, strcpy, strcmp
#include <dirent.h> // for DIR, struct dirent
#include <assert.h>

static const size_t kMaxPath = 1024;

static void listMatches(char path[], size_t length, const char *name){
  DIR *dir = opendir(path); // open directory
  if(dir == NULL) return;
  strcpy(path + length++, "/"); // append / to dir name
  while(true){
    struct dirent *de = readdir(dir); // read names of files from dir
    if (de == NULL) return;
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue; // skip hidden
    if (length + strlen(de->d_name) > kMaxPath) continue; // no buffer overflow
    strcpy(path + length, de->d_name); // add file/dir name to path
    struct stat st;
    lstat(path, &st); // get file info on new path
    if (S_ISREG(st.st_mode)){ // if file
      if(strcmp(name, de->d_name) == 0) printf("%s\n", path);
    }else if (S_ISDIR(st.st_mode)){ // if dir
      listMatches(path, length + strlen(de->d_name), name);
    }
  }
  closedir(dir);
}

int main(int argc, char *argv[]){
  assert(argc == 3);
  const char *directory = argv[1];
  struct stat st; // allows system call for whether file is a directory
  lstat(directory, &st); // sets mode for st 
  assert(S_ISDIR(st.st_mode));
  size_t length = strlen(directory);
  if(length > kMaxPath) return 0;
  char path[kMaxPath + 1];
  strcpy(path, directory);
  const char *pattern = argv[2];
  listMatches(path, length, pattern);
  return 0;
}
