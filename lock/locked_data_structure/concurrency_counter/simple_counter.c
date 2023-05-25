#include <stdio.h>
// simple counter data struct
typedef struct counter_t {
  int value;
} counter_t;

void init(counter_t *counter) { counter->value = 0; }

void increment(counter_t *counter) { counter->value++; }

void decrement(counter_t *counter) { counter->value--; }

int get(counter_t *counter) { return counter->value; }

int main() {
  counter_t counter;
  init(&counter); // value 0

  increment(&counter); // value 1
  increment(&counter);
  increment(&counter); // value 3

  decrement(&counter); // valude 2

  printf("counter value = %d\n", get(&counter)); // expect 2

  return 0;
}