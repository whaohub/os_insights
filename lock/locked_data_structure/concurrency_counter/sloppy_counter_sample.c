/*
 *  Sample Sloppy Counter Implementation
 *  Based on Operating Systems: Three Easy Pieces by R. Arpaci-Dusseau and A. Arpaci-Dusseau
 * 
 *  This example assigns CPU affinity for the threads.. Threads are free to be scheduled by Linux...
 *
 *  Wes J. Lloyd
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sched.h>

#define NUMTHREADS 8
#define SLOPPY_THRESHOLD 100000
#define OUTPUT 0
#define COUNTTO 10000000

typedef struct __counter_t {
  int			global;
  pthread_mutex_t	glock;
  int 			local[NUMTHREADS];	
  pthread_mutex_t	llock[NUMTHREADS];
  int			threshold;
} counter_t;

typedef struct __cthread
{
  counter_t * counter;
  int threadID;
  int amt;
  int core;
}  cthread;


void init(counter_t *c, int threshold)  {
#if SLOPPY_THRESHOLD
  c->threshold = SLOPPY_THRESHOLD;
#else
  c->threshold = threshold;
#endif
  
  c->global = 0;
  pthread_mutex_init(&c->glock, NULL);

  int i;
  for (i=0; i < NUMTHREADS; i++) {
    c->local[i] = 0;
    pthread_mutex_init(&c->llock[i], NULL);
  }
}

void update(counter_t *c, int threadID, int amt) {
  pthread_mutex_lock(&c->llock[threadID]);
  c->local[threadID] += amt;
  if (c->local[threadID] >= c->threshold) {
    pthread_mutex_lock(&c->glock);
    c->global += c->local[threadID];
#if OUTPUT
      cpu_set_t set;
      sched_getaffinity(0,sizeof(set),&set);
      printf("cpu=%lu>", *set.__bits);
      printf(" threshold update thread=%d thead_local=%d global=%d\n", threadID, c->global, c->local[threadID]);
#endif
    pthread_mutex_unlock(&c->glock);
    c->local[threadID] = 0;
  }
  pthread_mutex_unlock(&c->llock[threadID]);
}

int get(counter_t *c) {
  pthread_mutex_lock(&c->glock);
  int val = c->global;
  pthread_mutex_unlock(&c->glock);
  return val;
}

void *worker(void *arg)
{
  cthread * ct = (cthread *) arg;
  cpu_set_t my_set;
  //CPU_ZERO(&my_set);

// Play with where to pin ---
//  CPU_SET(ct->threadID, &my_set);
//  CPU_SET(7, &my_set);
 // CPU_SET(ct->core, &my_set);

  //sched_setaffinity(0, sizeof(cpu_set_t), &my_set); 
  printf("THREAD #%d: begin affinity=%lu\n",  ct->threadID, *my_set.__bits);
  int i;
  for (i=0;i<COUNTTO;i++) 
    update(ct->counter, ct->threadID, ct->amt);
  printf("THREAD #%d: done\n", ct->threadID);
  return NULL;
}

void printCounter(counter_t * counter)
{
  int x=0;
  printf("GC=%d  ", get(counter));
  for (x=0; x<NUMTHREADS; x++)
    printf(" T%d=%d ",x,counter->local[x]);
  printf("\n");
}

int main (int argc, char * argv[])
{
  pthread_t p1, p2, p3, p4, p5, p6, p7, p8;
  printf("main: begin \n");
  counter_t counter;
  cthread cp1, cp2, cp3, cp4, cp5, cp6, cp7, cp8;

  cp1.counter = &counter;
  cp1.threadID = 0;
  cp1.amt = 1;
  cp1.core = 1;

  cp2.counter = &counter;
  cp2.threadID = 1;
  cp2.amt = 1;
  cp2.core = 2;
  
  cp3.counter = &counter;
  cp3.threadID = 2;
  cp3.amt = 1;
  cp3.core = 1;
 
  cp4.counter = &counter;
  cp4.threadID = 3;
  cp4.amt = 1;
  cp4.core = 2;

  cp5.counter = &counter;
  cp5.threadID = 4;
  cp5.amt = 1;
  cp5.core = 1;

  cp6.counter = &counter;
  cp6.threadID = 5;
  cp6.amt = 1;
  cp6.core = 2;

  cp7.counter = &counter;
  cp7.threadID = 6;
  cp7.amt = 1;
  cp7.core = 1;

  cp8.counter = &counter;
  cp8.threadID = 7;
  cp8.amt = 1;
  cp8.core = 2;

  init(&counter, 2048);
 
  printf("Sloppy update threshold=%d\n",counter.threshold);

  pthread_create(&p1, NULL, worker, &cp1);
  pthread_create(&p2, NULL, worker, &cp2);
  pthread_create(&p3, NULL, worker, &cp3);
  pthread_create(&p4, NULL, worker, &cp4);
  pthread_create(&p5, NULL, worker, &cp5);
  pthread_create(&p6, NULL, worker, &cp6);
  pthread_create(&p7, NULL, worker, &cp7);
  pthread_create(&p8, NULL, worker, &cp8);

//#if OUTPUT
//  while (get(&counter) < COUNTTO * NUMCPUS)
//  {
//    printCounter(&counter);
//    usleep(1000);
//  }
//#endif

  pthread_join(p1, NULL);
  printCounter(&counter);
  pthread_join(p2, NULL);
  printCounter(&counter);
  pthread_join(p3, NULL);
  printCounter(&counter);
  pthread_join(p4, NULL);
  printCounter(&counter);
  pthread_join(p5, NULL);
  printCounter(&counter);
  pthread_join(p6, NULL);
  printCounter(&counter);
  pthread_join(p7, NULL);
  printCounter(&counter);
  pthread_join(p8, NULL);
  printf("main done with counting.\n");
  printCounter(&counter);
  return 0;
}