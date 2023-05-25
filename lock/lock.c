include "common_threads.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __lock_t {
  int flag;
} lock_t;

int CompareAndSwap(int *ptr, int expected, int new) {

  int original = *ptr;
  if (original == expected)
    *ptr = new;

  return original;
}

int TestAndSet(int *old_ptr, int new) {
  int old = *old_ptr; // fetch old value at old_ptr
  *old_ptr = new;     // store ’new’ into old_ptr
  return old;         // return the old value
}

void init(lock_t *mutex) {
  // 0 -> lock is available, 1 -> held
  mutex->flag = 0;
}

void lock(lock_t *mutex) {
  //while (TestAndSet(&mutex->flag, 1) == 1) //test and set
  while (CompareAndSwap(&mutex->flag, 0, 1) == 1)
    ; // TEST the flag  spin-wait (do nothing)
  // mutex->flag = 1; // now SET it!
}

void unlock(lock_t *mutex) { mutex->flag = 0; }

int loops = 0;

lock_t *mutex;
int sum = 0;
void *worker(void *thread) {
  printf("thread %lu is lock\n", pthread_self());
  lock(mutex);
  for (int i = 0; i < loops; ++i)
    sum++;
  unlock(mutex);

  return NULL;
}

int main(int argc, char **argv) {
  if (argc < 2)
    exit(1);

  loops = atoi(argv[1]);

  mutex = (lock_t *)malloc(sizeof(lock_t));
  init(mutex);

  pthread_t tid[loops];

  for (int i = 0; i < loops; ++i)
    Pthread_create(&tid[i], NULL, &worker, "A");

  for (int i = 0; i < loops; ++i)
    Pthread_join(tid[i], NULL);

  printf("final sum = %d\n", sum);

  free(mutex);

  return 0;
}
