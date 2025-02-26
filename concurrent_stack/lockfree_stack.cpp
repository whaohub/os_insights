#include <atomic>
#include <iostream>
#include <thread>
class Stack_overflow {};
class Stack_underflow {};
template <typename T>
class LFStack {
 public:
  void pop() {
    Node* got = head_.load();
    Node* nextin = nullptr;

    do {
      if (got == nullptr) {
        throw Stack_underflow();
      }
      nextin = got->next;
    } while (!head_.compare_exchange_weak(got, nextin));
    std::cout << "Popped: " << got->data << std::endl;
    delete got;
  }
  void push(T& val) {
    Node* node = new Node(val);
    node->next = head_.load();
    while (!head_.compare_exchange_weak(node->next, node))
      ;

    std::cout << "Pushed: " << val << std::endl;
  }

 private:
  struct Node {
    T data;
    Node* next;
    Node(const T& data) : data(data), next(nullptr) {}
  };
  std::atomic<Node*> head_;
};

void thread_push(LFStack<int>& stack) {
  for (int i = 0; i < 10; ++i) {  // Use a small value for easier observation
    stack.push(i);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(10));  // Simulate delay
  }
}

void thread_pop(LFStack<int>& stack) {
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
  LFStack<int> stack;
  // Launch threads
  std::thread t1(thread_push, std::ref(stack));
  std::thread t2(thread_pop, std::ref(stack));

  // Wait for threads to complete
  t1.join();
  t2.join();
}