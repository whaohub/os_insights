#include "common_threads.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMCPUS 8
typedef struct  counter_t{
  int global;                      // global count
  pthread_mutex_t glock;           // global lock
  int local[NUMCPUS];              // local count
  pthread_mutex_t llock[NUMCPUS];  // local lock
  int threshold;                   // update frequency
}counter_t;

typedef struct args {
  counter_t *counter;
  int thread_id;
}thread_args;
// init: record threshold, init locks, init values
// of all local counts and global count
void init(counter_t *counter) {

  counter->global = 0;
  pthread_mutex_init(&counter->glock, NULL);

  for(int i = 0; i < NUMCPUS; ++i) {
    counter->local[i] = 0;
    pthread_mutex_init(&counter->llock[i], NULL);
  }

  counter->threshold = 0;
}

void destory(counter_t *counter) {
  pthread_mutex_destroy(&counter->glock);

  for (int i = 0; i < NUMCPUS; ++i) {
    pthread_mutex_destroy(&counter->llock[i]);
  }
}

// update: usually, just grab local lock and update local amount
// once local count has risen by 'threshold', grab global
// lock and transfer local values to it 
void update(counter_t *counter, int thread_id, int cmt) {
  Pthread_mutex_lock(&counter->llock[thread_id]);
  counter->local[thread_id]+=cmt;
  
  if(counter->local[thread_id] >= counter->threshold) {
    Pthread_mutex_lock(&counter->glock);
    counter->global += counter->local[thread_id];
    Pthread_mutex_unlock(&counter->glock);
    counter->local[thread_id] = 0;
  }

  Pthread_mutex_unlock(&counter->llock[thread_id]);

}

int get(counter_t *counter) {
  Pthread_mutex_lock(&counter->glock);
  printf("counter global = %d\n", counter->global);
  int rc = counter->global;
  Pthread_mutex_unlock(&counter->glock);
  return rc;
}

int loops;
void *worker(void *arg) {
  struct args *thread_arg = arg;
  const int count = 1;
  for (int i = 0; i < loops; ++i) {
    update(thread_arg->counter, thread_arg->thread_id, count);
  }
  pthread_exit(NULL);
  return NULL;
}

int main(int argc, char **argv) {
  if(argc < 2)
    perror("args error");

  loops = atoi(argv[1]);

  pthread_t tid[NUMCPUS];
  counter_t counter;

  init(&counter);
  counter.threshold = 10240;

  thread_args thread_arg[NUMCPUS];

  for(int thread_id = 0; thread_id < NUMCPUS; ++thread_id) {
    thread_arg[thread_id].counter = &counter;
    thread_arg[thread_id].thread_id = thread_id;
    Pthread_create(&tid[thread_id], NULL, worker, (void*)&thread_arg[thread_id]);
  }

  for(int thread_id = 0; thread_id < NUMCPUS; ++thread_id) {
    Pthread_join(tid[thread_id], NULL);
  }
  printf("thread exit\n");
  printf("global value = %d\n", get(&counter));
  
  destory(&counter);
}