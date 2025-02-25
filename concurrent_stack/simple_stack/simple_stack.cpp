// Description: A simple stack implementation with push and pop functions.
// reference :
// https://www.geeksforgeeks.org/introduction-to-stack-data-structure-and-algorithm-tutorials/
// https://gist.github.com/manonthemat/49c6a2dadd57b0ae75b2

#include <iostream>
#include <thread>
#include <unordered_set>
#include <vector>
class Stack_overflow {};
class Stack_underflow {};

class Stack {
 public:
  Stack(int sz) : sz_(sz) {
    data_ = new int[sz];
    top_ = -1;
  };
  int pop() {
    if (top_ == -1) {
      throw Stack_underflow();
    }
    auto value = data_[top_--];
    std::cout << "Popped: " << value << std::endl;
    return value;
  }
  void push(int val) {
    if (top_ == sz_ - 1) {
      throw Stack_overflow();
    }
    std::cout << "Pushed: " << val << std::endl;
    data_[++top_] = val;
  }

 private:
  int* data_;
  int top_;
  int sz_;
};

void thread_push(Stack& stack) {
  for (int i = 0; i < 10; ++i) {  // Use a small value for easier observation
    stack.push(i);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(10));  // Simulate delay
  }
}

void thread_pop(Stack& stack) {
  for (int i = 0; i < 10; ++i) {  // Use a small value for easier observation
    try {
      stack.pop();
    } catch (const std::out_of_range& e) {
      // Handle empty stack scenario
    }
    std::this_thread::sleep_for(
        std::chrono::milliseconds(15));  // Simulate delay
  }
}
int main() {
  Stack stack(10000);

  // Launch threads
  std::thread t1(thread_push, std::ref(stack));
  std::thread t2(thread_pop, std::ref(stack));

  // Wait for threads to complete
  t1.join();
  t2.join();
}
