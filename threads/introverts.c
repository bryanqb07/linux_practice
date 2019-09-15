#include <pthread.h> // provides pthread_t type, thread functions
#include <stdio.h> // printf, which is thread safe (atomic)

static void *recharge(void *args){
  printf("I recharge by spending time alone.\n");
  return NULL;
}

static const size_t kNumIntroverts = 6;
int main(int argc, char *argv[]){
  printf("Lets here from %zu introverts", kNumIntroverts);
  pthread_t introverts[kNumIntroverts];
  for(size_t i = 0; i < kNumIntroverts; i++){
    // params (thread_id, priority, function, args)
    pthread_create(&introverts[i], NULL, recharge, NULL);
  }
  for(size_t i = 0; i < kNumIntroverts; i++){
    // params (thread_id, return value)
    pthread_join(introverts[i], NULL);
  }

  printf("Everyone is recharged\n");
  return 0;
}
