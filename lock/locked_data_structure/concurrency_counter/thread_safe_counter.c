#include "common_threads.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct counter_t {
  int value;
  pthread_mutex_t lock;
} counter_t;

int loop;

void init(counter_t *counter) {
  counter->value = 0;
  pthread_mutex_init(&counter->lock, NULL);
}

void destory(counter_t *counter) {
  pthread_mutex_destroy(&counter->lock);
}

void increment(counter_t *counter) {
  Pthread_mutex_lock(&counter->lock);
  counter->value++;
  Pthread_mutex_unlock(&counter->lock)
}

void decrement(counter_t *counter) {
  Pthread_mutex_lock(&counter->lock);
  counter->value--;
  Pthread_mutex_unlock(&counter->lock)
}

int get(void *arg) {
  struct counter_t *counter = arg;

  Pthread_mutex_lock(&counter->lock);
  int rc = counter->value;
  Pthread_mutex_unlock(&counter->lock);

  return rc;
}

void *worker(void *arg) {
  struct counter_t *counter = arg;
  for (int i = 0; i < loop; ++i) {
    increment(counter);
  }

  return NULL;
}
int main(int argc, char **argv) {

  if (argc < 3) {
    perror("arg nums error\n");
    exit(-1);
  }

  int thread_num = atoi(argv[1]);
  loop = atoi(argv[2]);

  counter_t counter;
  pthread_t tid[thread_num];

  init(&counter);

  for (int i = 0; i < thread_num; ++i) {
    Pthread_create(&tid[i], NULL, worker, (void *)&counter);
  }

  for (int i = 0; i < thread_num; ++i) {
    Pthread_join(tid[i], NULL);
  }
		
	printf("thread exit\n");
  printf("counter value = %d\n", get(&counter)); //

  destory(&counter);
  return 0;
}