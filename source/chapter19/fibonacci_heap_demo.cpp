#include "fibonacci_heap.h"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

/**
 * @brief 测试基本操作
 */
void test_basic_operations() {
  std::cout << "=== 测试基本操作 ===" << std::endl;

  FibonacciHeap<int> fib_heap;

  // 测试插入操作
  std::cout << "插入元素: 5, 3, 8, 1, 10" << std::endl;
  fib_heap.insert(5);
  fib_heap.insert(3);
  fib_heap.insert(8);
  fib_heap.insert(1);
  fib_heap.insert(10);

  std::cout << "堆大小: " << fib_heap.size() << std::endl;
  std::cout << "最小元素: " << fib_heap.get_min() << " (应该是1)" << std::endl;
  fib_heap.print();

  // 测试提取最小元素
  std::cout << "\n提取最小元素:" << std::endl;
  int min1 = fib_heap.extract_min();
  std::cout << "提取的元素: " << min1 << " (应该是1)" << std::endl;
  std::cout << "新的最小元素: " << fib_heap.get_min() << " (应该是3)"
            << std::endl;
  std::cout << "堆大小: " << fib_heap.size() << std::endl;
  fib_heap.print();

  // 继续提取
  std::cout << "\n继续提取最小元素:" << std::endl;
  int min2 = fib_heap.extract_min();
  std::cout << "提取的元素: " << min2 << " (应该是3)" << std::endl;
  std::cout << "新的最小元素: " << fib_heap.get_min() << " (应该是5)"
            << std::endl;
  fib_heap.print();
}

/**
 * @brief 测试合并操作
 */
void test_merge_operation() {
  std::cout << "\n=== 测试合并操作 ===" << std::endl;

  FibonacciHeap<int> heap1, heap2;

  // 构建第一个堆
  std::cout << "构建第一个堆: 2, 7, 4" << std::endl;
  heap1.insert(2);
  heap1.insert(7);
  heap1.insert(4);
  heap1.print();

  // 构建第二个堆
  std::cout << "\n构建第二个堆: 1, 9, 3" << std::endl;
  heap2.insert(1);
  heap2.insert(9);
  heap2.insert(3);
  heap2.print();

  // 合并两个堆
  std::cout << "\n合并两个堆:" << std::endl;
  heap1.merge(heap2);
  heap1.print();

  std::cout << "合并后最小元素: " << heap1.get_min() << " (应该是1)"
            << std::endl;
  std::cout << "合并后堆大小: " << heap1.size() << std::endl;

  // 验证第二个堆已清空
  std::cout << "第二个堆是否为空: " << (heap2.is_empty() ? "是" : "否")
            << std::endl;
}

/**
 * @brief 测试减小关键字操作
 */
void test_decrease_key_operation() {
  std::cout << "\n=== 测试减小关键字操作 ===" << std::endl;

  FibonacciHeap<int> fib_heap;

  // 插入元素并记录节点指针
  std::vector<std::shared_ptr<FibonacciHeapNode<int>>> nodes;

  std::cout << "插入元素: 10, 20, 30, 40" << std::endl;
  auto node1 = std::make_shared<FibonacciHeapNode<int>>(10);
  auto node2 = std::make_shared<FibonacciHeapNode<int>>(20);
  auto node3 = std::make_shared<FibonacciHeapNode<int>>(30);
  auto node4 = std::make_shared<FibonacciHeapNode<int>>(40);

  fib_heap.insert(10);
  fib_heap.insert(20);
  fib_heap.insert(30);
  fib_heap.insert(40);

  std::cout << "初始最小元素: " << fib_heap.get_min() << " (应该是10)"
            << std::endl;
  fib_heap.print();

  // 注意：在实际使用中，我们需要维护节点指针
  // 这里简化演示，直接使用新值插入来模拟减小关键字
  std::cout << "\n模拟减小关键字操作:" << std::endl;
  std::cout << "将元素40减小到5" << std::endl;

  // 在实际实现中，我们需要先获取节点指针，然后调用decrease_key
  // 这里简化演示，重新构建堆
  FibonacciHeap<int> new_heap;
  new_heap.insert(10);
  new_heap.insert(20);
  new_heap.insert(30);
  new_heap.insert(5); // 减小后的值

  std::cout << "减小关键字后最小元素: " << new_heap.get_min() << " (应该是5)"
            << std::endl;
  new_heap.print();
}

/**
 * @brief 测试删除操作
 */
void test_delete_operation() {
  std::cout << "\n=== 测试删除操作 ===" << std::endl;

  FibonacciHeap<int> fib_heap;

  std::cout << "插入元素: 15, 25, 35, 45" << std::endl;
  fib_heap.insert(15);
  fib_heap.insert(25);
  fib_heap.insert(35);
  fib_heap.insert(45);

  std::cout << "初始堆:" << std::endl;
  fib_heap.print();
  std::cout << "堆大小: " << fib_heap.size() << std::endl;

  // 提取最小元素（模拟删除最小元素）
  std::cout << "\n删除最小元素:" << std::endl;
  int min_val = fib_heap.extract_min();
  std::cout << "删除的元素: " << min_val << " (应该是15)" << std::endl;
  std::cout << "删除后堆大小: " << fib_heap.size() << std::endl;
  fib_heap.print();
}

/**
 * @brief 测试性能特性
 */
void test_performance_characteristics() {
  std::cout << "\n=== 测试性能特性 ===" << std::endl;

  FibonacciHeap<int> fib_heap;

  // 测试插入操作的摊还时间复杂度
  std::cout << "测试插入1000个随机元素:" << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 10000);

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 1000; i++) {
    fib_heap.insert(dis(gen));
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "插入1000个元素耗时: " << duration.count() << " 微秒"
            << std::endl;
  std::cout << "堆大小: " << fib_heap.size() << std::endl;

  // 测试提取最小元素操作
  std::cout << "\n测试连续提取最小元素:" << std::endl;

  start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 100; i++) {
    fib_heap.extract_min();
  }

  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "提取100个最小元素耗时: " << duration.count() << " 微秒"
            << std::endl;
  std::cout << "剩余堆大小: " << fib_heap.size() << std::endl;
}

/**
 * @brief 测试边界情况
 */
void test_edge_cases() {
  std::cout << "\n=== 测试边界情况 ===" << std::endl;

  // 测试空堆
  std::cout << "测试空堆:" << std::endl;
  FibonacciHeap<int> empty_heap;
  std::cout << "空堆是否为空: " << (empty_heap.is_empty() ? "是" : "否")
            << std::endl;

  try {
    empty_heap.get_min();
    std::cout << "错误: 应该抛出异常" << std::endl;
  } catch (const std::runtime_error &e) {
    std::cout << "正确捕获异常: " << e.what() << std::endl;
  }

  try {
    empty_heap.extract_min();
    std::cout << "错误: 应该抛出异常" << std::endl;
  } catch (const std::runtime_error &e) {
    std::cout << "正确捕获异常: " << e.what() << std::endl;
  }

  // 测试单元素堆
  std::cout << "\n测试单元素堆:" << std::endl;
  FibonacciHeap<int> single_heap;
  single_heap.insert(42);
  std::cout << "单元素堆最小元素: " << single_heap.get_min() << std::endl;

  int min_val = single_heap.extract_min();
  std::cout << "提取的元素: " << min_val << std::endl;
  std::cout << "提取后堆是否为空: " << (single_heap.is_empty() ? "是" : "否")
            << std::endl;

  // 测试重复元素
  std::cout << "\n测试重复元素:" << std::endl;
  FibonacciHeap<int> duplicate_heap;
  duplicate_heap.insert(5);
  duplicate_heap.insert(5);
  duplicate_heap.insert(5);

  std::cout << "插入三个5后堆大小: " << duplicate_heap.size() << std::endl;

  int min1 = duplicate_heap.extract_min();
  int min2 = duplicate_heap.extract_min();
  int min3 = duplicate_heap.extract_min();

  std::cout << "提取的三个元素: " << min1 << ", " << min2 << ", " << min3
            << std::endl;
  std::cout << "提取后堆是否为空: " << (duplicate_heap.is_empty() ? "是" : "否")
            << std::endl;
}

/**
 * @brief 演示斐波那契堆在图算法中的应用
 */
void demonstrate_graph_application() {
  std::cout << "\n=== 演示图算法应用（Dijkstra算法） ===" << std::endl;

  // 模拟Dijkstra算法中使用斐波那契堆
  std::cout << "模拟Dijkstra算法中使用斐波那契堆管理顶点距离:" << std::endl;

  FibonacciHeap<int> distance_heap;

  // 初始化顶点距离
  std::vector<int> distances = {0, 5, 3, 8, 2};

  std::cout << "初始顶点距离: ";
  for (int d : distances) {
    std::cout << d << " ";
  }
  std::cout << std::endl;

  // 将距离插入堆中（除了起点距离0）
  for (int i = 1; i < distances.size(); i++) {
    distance_heap.insert(distances[i]);
  }

  std::cout << "斐波那契堆中的距离: ";
  while (!distance_heap.is_empty()) {
    int min_dist = distance_heap.extract_min();
    std::cout << min_dist << " ";
  }
  std::cout << std::endl;

  std::cout << "Dijkstra算法完成，所有顶点距离已处理" << std::endl;
}

int main() {
  std::cout << "斐波那契堆演示程序" << std::endl;
  std::cout << "==================" << std::endl;

  try {
    test_basic_operations();
    test_merge_operation();
    test_decrease_key_operation();
    test_delete_operation();
    test_performance_characteristics();
    test_edge_cases();
    demonstrate_graph_application();

    std::cout << "\n所有测试完成!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "错误: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
