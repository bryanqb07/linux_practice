#include <stdio.h>
#include <stdlib.h> // for exit
#include <stdarg.h> // for va_list, etc
#include <sys/stat.h> // for stat
#include <string.h> // for strlen, strcpy, strcmp
#include <dirent.h> //for DIR , struct dirent
#include <assert.h>

static const size_t kMaxPath = 1024;

static void listMatches(char path[], size_t length, const char *name){
  DIR *dir = opendir(path); // open directory
  if (dir == NULL) return; // if dir but no permission to open
  strcpy(path + length++, "/"); // cpy a / onto path name
  while (true){
    struct dirent *de = readdir(dir); // read in a directory
    if (de == NULL) break; // iterated over all directories
    if (strcmp(de->d_name, ".") == 0 ) || strcmp(de->d_name, ".." == 0) continue; // skip directories named . (current) .. (previous)
    if (length + strlen(de->d_name) > kMaxPath) continue;
    strcpy(path + length, de->d_name); // copies dir name onto path
    struct stat st;
    lstat(path, &st); // call st function to show file info
    if(S_ISREG(st.st_mode)){ // if it's a file
      // print it to STDOUT if file matches search param
      if(strcmp(de->d_name, name) == 0) printf("%s\n", path);
    }else if(S_ISDIR(st.st_mode)){ // if it's a directory
      // recursively search through directory
      listMatches(path, length + strlen(de->d_name), name);
    }
  }
  closedir(dir);
}

int main(int argc, char *argv[]){
  assert(argc == 3);
  const char *directory = argv[1];
  struct stat st;
  lstat(directory, &st); // populate struct stat with file name
  assert(S_ISDIR(st.st_mode)); // check to see if it's directory
  size_t length = strlen(directory);
  if (length > kMaxPath) return 0; 
  const char *pattern = argv[2];
  char path[kMaxPath + 1]; 
  strcpy(path, directory); // prevent buffer overflow
  listMatches(path, length, pattern);
  return 0;
}
