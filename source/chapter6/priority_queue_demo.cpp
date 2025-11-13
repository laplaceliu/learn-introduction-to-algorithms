#include "priority_queue.h"
#include <iostream>
#include <vector>

using namespace algorithms;

void test_basic_priority_queue() {
  std::cout << "=== 测试基本优先队列 ===" << std::endl;

  PriorityQueue pq;

  // 插入元素
  std::cout << "插入元素: 10, 5, 15, 3, 7" << std::endl;
  pq.insert(10);
  pq.insert(5);
  pq.insert(15);
  pq.insert(3);
  pq.insert(7);

  pq.print();
  std::cout << "当前最大值: " << pq.maximum() << std::endl;

  // 提取最大值
  std::cout << "提取最大值: " << pq.extract_max() << std::endl;
  pq.print();
  std::cout << "当前最大值: " << pq.maximum() << std::endl;

  // 继续提取
  std::cout << "提取最大值: " << pq.extract_max() << std::endl;
  pq.print();

  std::cout << std::endl;
}

void test_enhanced_priority_queue() {
  std::cout << "=== 测试增强版优先队列（支持INCREASE-KEY） ===" << std::endl;

  EnhancedPriorityQueue epq(10); // 最多支持10个元素

  // 插入元素，每个元素有唯一ID
  std::cout << "插入元素: " << std::endl;
  std::cout << "元素0: 关键字10" << std::endl;
  std::cout << "元素1: 关键字5" << std::endl;
  std::cout << "元素2: 关键字15" << std::endl;
  std::cout << "元素3: 关键字3" << std::endl;

  epq.insert(0, 10);
  epq.insert(1, 5);
  epq.insert(2, 15);
  epq.insert(3, 3);

  epq.print();
  std::cout << "当前最大值: " << epq.maximum() << std::endl;

  // 测试INCREASE-KEY操作
  std::cout << "将元素1的关键字从5增加到20" << std::endl;
  epq.increase_key(1, 20);
  epq.print();
  std::cout << "当前最大值: " << epq.maximum() << std::endl;

  // 提取最大值
  std::cout << "提取最大值: " << epq.extract_max() << std::endl;
  epq.print();

  // 再次测试INCREASE-KEY
  std::cout << "将元素3的关键字从3增加到25" << std::endl;
  epq.increase_key(3, 25);
  epq.print();

  std::cout << std::endl;
}

void test_priority_queue_operations() {
  std::cout << "=== 测试优先队列的标准操作 ===" << std::endl;

  // 算法导论6.4节的标准操作演示
  PriorityQueue pq;

  std::cout << "执行 INSERT(S, 10)" << std::endl;
  pq.insert(10);
  pq.print();

  std::cout << "执行 INSERT(S, 5)" << std::endl;
  pq.insert(5);
  pq.print();

  std::cout << "执行 INSERT(S, 15)" << std::endl;
  pq.insert(15);
  pq.print();

  std::cout << "执行 MAXIMUM(S): " << pq.maximum() << std::endl;

  std::cout << "执行 EXTRACT-MAX(S): " << pq.extract_max() << std::endl;
  pq.print();

  std::cout << "执行 INSERT(S, 8)" << std::endl;
  pq.insert(8);
  pq.print();

  std::cout << "执行 EXTRACT-MAX(S): " << pq.extract_max() << std::endl;
  pq.print();

  std::cout << "执行 EXTRACT-MAX(S): " << pq.extract_max() << std::endl;
  pq.print();

  std::cout << std::endl;
}

void test_edge_cases() {
  std::cout << "=== 测试边界情况 ===" << std::endl;

  // 测试空队列
  PriorityQueue empty_pq;
  std::cout << "空队列大小: " << empty_pq.size() << std::endl;
  std::cout << "空队列是否为空: " << (empty_pq.empty() ? "是" : "否")
            << std::endl;

  // 测试单个元素
  PriorityQueue single_pq;
  single_pq.insert(42);
  std::cout << "单元素队列的最大值: " << single_pq.maximum() << std::endl;
  std::cout << "提取单元素: " << single_pq.extract_max() << std::endl;
  std::cout << "提取后队列是否为空: " << (single_pq.empty() ? "是" : "否")
            << std::endl;

  std::cout << std::endl;
}

int main() {
  std::cout << "=== 算法导论 6.4节 - 优先队列演示 ===" << std::endl;
  std::cout << std::endl;

  test_basic_priority_queue();
  test_enhanced_priority_queue();
  test_priority_queue_operations();
  test_edge_cases();

  std::cout << "=== 演示结束 ===" << std::endl;

  return 0;
}
