#include <pthread.h>
#include <stdio.h>

static const char *kFriends[] = {
  "Jack", "Michaela", "Luis", "Richard", "Jordan", "Lisa", "Imaginary"
};

static const size_t kNumFriends = sizeof(kFriends) / sizeof(kFriends[0]) - 1;

static void *meetup(void *args){
  const char *name = args;
  printf("%s says hello\n", name);
  return NULL;
}


int main(){
  printf("Let's here from %zu friends\n", kNumFriends);
  pthread_t friends[kNumFriends];
  for(size_t i = 0; i < kNumFriends; i++){
    pthread_create(&friends[i], NULL, meetup, (void *) kFriends[i]);
  }
  for(size_t i = 0; i < kNumFriends; i++){
    pthread_join(friends[i], NULL);
  }

  return 0;
}
