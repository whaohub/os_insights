#include <iostream>
#include <mutex>
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
    std::lock_guard<std::mutex> lock(mutex_);
    if (top_ == -1) {
      throw Stack_underflow();
    }
    return data_[top_--];
  }
  void push(int val) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (top_ == sz_ - 1) {
      throw Stack_overflow();
    }
    data_[++top_] = val;
  }

 private:
  int* data_;
  int top_;
  int sz_;
  std::mutex mutex_;
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
