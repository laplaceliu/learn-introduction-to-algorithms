#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include <deque>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace algorithms {

/**
 * @brief 栈数据结构实现（基于数组）
 *
 * 实现算法导论10.1节的栈操作：
 * - PUSH(S, x): 压入元素
 * - POP(S): 弹出元素
 * - STACK-EMPTY(S): 检查栈是否为空
 */
class Stack {
private:
  std::vector<int> data;
  int top_index; // 栈顶指针
  int capacity;  // 栈的容量

public:
  /**
   * @brief 构造函数
   * @param size 栈的容量（默认为100）
   */
  Stack(int size = 100) : top_index(-1), capacity(size) {
    data.resize(capacity);
  }

  /**
   * @brief 压入元素（PUSH操作）
   * @param x 要压入的元素
   * @throws std::overflow_error 如果栈已满
   */
  void push(int x) {
    if (top_index >= capacity - 1) {
      throw std::overflow_error("栈溢出");
    }
    data[++top_index] = x;
  }

  /**
   * @brief 弹出元素（POP操作）
   * @return 弹出的元素
   * @throws std::underflow_error 如果栈为空
   */
  int pop() {
    if (empty()) {
      throw std::underflow_error("栈下溢");
    }
    return data[top_index--];
  }

  /**
   * @brief 查看栈顶元素
   * @return 栈顶元素
   * @throws std::underflow_error 如果栈为空
   */
  int top() const {
    if (empty()) {
      throw std::underflow_error("栈为空");
    }
    return data[top_index];
  }

  /**
   * @brief 检查栈是否为空（STACK-EMPTY操作）
   * @return 如果栈为空返回true，否则返回false
   */
  bool empty() const { return top_index == -1; }

  /**
   * @brief 获取栈的大小
   * @return 栈中元素的数量
   */
  int size() const { return top_index + 1; }

  /**
   * @brief 获取栈的容量
   * @return 栈的总容量
   */
  int get_capacity() const { return capacity; }

  /**
   * @brief 打印栈的内容（从栈底到栈顶）
   */
  void print() const {
    std::cout << "栈 [底->顶]: ";
    for (int i = 0; i <= top_index; ++i) {
      std::cout << data[i] << " ";
    }
    std::cout << std::endl;
  }
};

/**
 * @brief 队列数据结构实现（基于数组的循环队列）
 *
 * 实现算法导论10.1节的队列操作：
 * - ENQUEUE(Q, x): 入队
 * - DEQUEUE(Q): 出队
 * - QUEUE-EMPTY(Q): 检查队列是否为空
 */
class Queue {
private:
  std::vector<int> data;
  int head;     // 队头指针
  int tail;     // 队尾指针
  int capacity; // 队列容量
  int count;    // 当前元素数量

public:
  /**
   * @brief 构造函数
   * @param size 队列的容量（默认为100）
   */
  Queue(int size = 100) : head(0), tail(0), capacity(size), count(0) {
    data.resize(capacity);
  }

  /**
   * @brief 入队操作（ENQUEUE操作）
   * @param x 要入队的元素
   * @throws std::overflow_error 如果队列已满
   */
  void enqueue(int x) {
    if (count >= capacity) {
      throw std::overflow_error("队列已满");
    }
    data[tail] = x;
    tail = (tail + 1) % capacity;
    count++;
  }

  /**
   * @brief 出队操作（DEQUEUE操作）
   * @return 出队的元素
   * @throws std::underflow_error 如果队列为空
   */
  int dequeue() {
    if (empty()) {
      throw std::underflow_error("队列为空");
    }
    int x = data[head];
    head = (head + 1) % capacity;
    count--;
    return x;
  }

  /**
   * @brief 查看队头元素
   * @return 队头元素
   * @throws std::underflow_error 如果队列为空
   */
  int front() const {
    if (empty()) {
      throw std::underflow_error("队列为空");
    }
    return data[head];
  }

  /**
   * @brief 检查队列是否为空（QUEUE-EMPTY操作）
   * @return 如果队列为空返回true，否则返回false
   */
  bool empty() const { return count == 0; }

  /**
   * @brief 获取队列的大小
   * @return 队列中元素的数量
   */
  int size() const { return count; }

  /**
   * @brief 获取队列的容量
   * @return 队列的总容量
   */
  int get_capacity() const { return capacity; }

  /**
   * @brief 打印队列的内容（从队头到队尾）
   */
  void print() const {
    std::cout << "队列 [头->尾]: ";
    int current = head;
    for (int i = 0; i < count; ++i) {
      std::cout << data[current] << " ";
      current = (current + 1) % capacity;
    }
    std::cout << std::endl;
  }
};

/**
 * @brief 双端队列（Deque）实现
 *
 * 扩展的队列，支持在两端进行插入和删除操作
 */
class Deque {
private:
  std::deque<int> data;

public:
  /**
   * @brief 在队头插入元素
   * @param x 要插入的元素
   */
  void push_front(int x) { data.push_front(x); }

  /**
   * @brief 在队尾插入元素
   * @param x 要插入的元素
   */
  void push_back(int x) { data.push_back(x); }

  /**
   * @brief 从队头删除元素
   * @return 被删除的元素
   * @throws std::underflow_error 如果队列为空
   */
  int pop_front() {
    if (empty()) {
      throw std::underflow_error("双端队列为空");
    }
    int x = data.front();
    data.pop_front();
    return x;
  }

  /**
   * @brief 从队尾删除元素
   * @return 被删除的元素
   * @throws std::underflow_error 如果队列为空
   */
  int pop_back() {
    if (empty()) {
      throw std::underflow_error("双端队列为空");
    }
    int x = data.back();
    data.pop_back();
    return x;
  }

  /**
   * @brief 查看队头元素
   * @return 队头元素
   * @throws std::underflow_error 如果队列为空
   */
  int front() const {
    if (empty()) {
      throw std::underflow_error("双端队列为空");
    }
    return data.front();
  }

  /**
   * @brief 查看队尾元素
   * @return 队尾元素
   * @throws std::underflow_error 如果队列为空
   */
  int back() const {
    if (empty()) {
      throw std::underflow_error("双端队列为空");
    }
    return data.back();
  }

  /**
   * @brief 检查队列是否为空
   * @return 如果队列为空返回true，否则返回false
   */
  bool empty() const { return data.empty(); }

  /**
   * @brief 获取队列的大小
   * @return 队列中元素的数量
   */
  int size() const { return data.size(); }

  /**
   * @brief 打印双端队列的内容
   */
  void print() const {
    std::cout << "双端队列 [头->尾]: ";
    for (int val : data) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }
};

/**
 * @brief 使用两个栈实现队列
 *
 * 算法导论练习10.1-6：用两个栈实现队列
 */
class QueueWithTwoStacks {
private:
  Stack input_stack;  // 输入栈
  Stack output_stack; // 输出栈

  /**
   * @brief 将输入栈的所有元素转移到输出栈
   */
  void transfer_elements() {
    while (!input_stack.empty()) {
      output_stack.push(input_stack.pop());
    }
  }

public:
  /**
   * @brief 构造函数
   * @param size 队列容量
   */
  QueueWithTwoStacks(int size = 100) : input_stack(size), output_stack(size) {}

  /**
   * @brief 入队操作
   * @param x 要入队的元素
   */
  void enqueue(int x) { input_stack.push(x); }

  /**
   * @brief 出队操作
   * @return 出队的元素
   * @throws std::underflow_error 如果队列为空
   */
  int dequeue() {
    if (empty()) {
      throw std::underflow_error("队列为空");
    }

    if (output_stack.empty()) {
      transfer_elements();
    }

    return output_stack.pop();
  }

  /**
   * @brief 查看队头元素
   * @return 队头元素
   * @throws std::underflow_error 如果队列为空
   */
  int front() {
    if (empty()) {
      throw std::underflow_error("队列为空");
    }

    if (output_stack.empty()) {
      transfer_elements();
    }

    return output_stack.top();
  }

  /**
   * @brief 检查队列是否为空
   * @return 如果队列为空返回true，否则返回false
   */
  bool empty() const { return input_stack.empty() && output_stack.empty(); }

  /**
   * @brief 获取队列的大小
   * @return 队列中元素的数量
   */
  int size() const { return input_stack.size() + output_stack.size(); }

  /**
   * @brief 打印队列的内容
   */
  void print() {
    // 由于使用两个栈实现，打印需要临时操作
    std::cout << "队列 [头->尾]: ";

    // 先打印输出栈（从栈底到栈顶）
    Stack temp_output = output_stack;
    std::vector<int> output_elements;
    while (!temp_output.empty()) {
      output_elements.push_back(temp_output.pop());
    }

    // 逆序打印输出栈元素（使其按队列顺序）
    for (auto it = output_elements.rbegin(); it != output_elements.rend();
         ++it) {
      std::cout << *it << " ";
    }

    // 再打印输入栈（按入栈顺序）
    Stack temp_input = input_stack;
    std::vector<int> input_elements;
    while (!temp_input.empty()) {
      input_elements.push_back(temp_input.pop());
    }

    // 逆序打印输入栈元素（使其按队列顺序）
    for (auto it = input_elements.rbegin(); it != input_elements.rend(); ++it) {
      std::cout << *it << " ";
    }

    std::cout << std::endl;
  }
};

} // namespace algorithms

#endif // STACK_QUEUE_H
