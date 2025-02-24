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
    return data_[top_--];
  }
  void push(int val) {
    if (top_ == sz_ - 1) {
      throw Stack_overflow();
    }
    data_[++top_] = val;
  }

 private:
  int* data_;
  int top_;
  int sz_;
};

int main() {
  Stack s(10000);
  for (int i = 0; i < 10000; i++) {
    s.push(i);
  }

  std::vector<std::thread> threads;
  std::unordered_set<int> unordered_set;

  const int kThreadNum = 2;
  for (int i = 0; i < kThreadNum; ++i) {
    threads.emplace_back([&]() {
      auto v = s.pop();
      unordered_set.insert(v);
      if (unordered_set.count(v)) {
        std::cerr << "Error: " << v << " was poped before" << std::endl;
      }
    });
  }

  for (auto& t : threads) {
    t.join();
  }
}
