#include "order_statistic_tree.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace algorithms;

void test_basic_operations() {
  std::cout << "=== 测试基本操作 ===" << std::endl;

  OrderStatisticTree<int> ost;

  // 测试插入
  std::cout << "插入元素: 41, 38, 31, 12, 19, 8" << std::endl;
  ost.insert(41);
  ost.insert(38);
  ost.insert(31);
  ost.insert(12);
  ost.insert(19);
  ost.insert(8);

  // 测试遍历
  std::cout << "中序遍历: ";
  auto inorder = ost.inorder_traversal();
  for (int val : inorder) {
    std::cout << val << " ";
  }
  std::cout << std::endl;

  std::cout << "树大小: " << ost.size() << std::endl;
  std::cout << "树结构:" << std::endl;
  ost.print_tree();
  std::cout << std::endl;
}

void test_order_statistics() {
  std::cout << "=== 测试顺序统计操作 ===" << std::endl;

  OrderStatisticTree<int> ost;

  // 构建测试树（算法导论图14.1示例）
  std::vector<int> values = {26, 17, 41, 14, 21, 30, 47, 10, 16, 19,
                             21, 28, 38, 7,  12, 14, 20, 35, 39, 3};
  for (int val : values) {
    ost.insert(val);
  }

  std::cout << "构建顺序统计树（算法导论图14.1）" << std::endl;
  std::cout << "中序遍历: ";
  auto inorder = ost.inorder_traversal();
  for (int val : inorder) {
    std::cout << val << " ";
  }
  std::cout << std::endl;

  std::cout << "树大小: " << ost.size() << std::endl;
  std::cout << "树结构:" << std::endl;
  ost.print_tree();
  std::cout << std::endl;

  // 测试OS-SELECT操作
  std::cout << "=== 测试OS-SELECT操作 ===" << std::endl;

  try {
    std::cout << "第1小的元素: " << ost.select(1) << std::endl;
    std::cout << "第5小的元素: " << ost.select(5) << std::endl;
    std::cout << "第10小的元素: " << ost.select(10) << std::endl;
    std::cout << "第15小的元素: " << ost.select(15) << std::endl;
    std::cout << "第20小的元素: " << ost.select(20) << std::endl;
  } catch (const std::out_of_range &e) {
    std::cout << "异常: " << e.what() << std::endl;
  }

  // 测试OS-RANK操作
  std::cout << "=== 测试OS-RANK操作 ===" << std::endl;

  try {
    std::cout << "元素3的排名: " << ost.rank(3) << std::endl;
    std::cout << "元素14的排名: " << ost.rank(14) << std::endl;
    std::cout << "元素21的排名: " << ost.rank(21) << std::endl;
    std::cout << "元素30的排名: " << ost.rank(30) << std::endl;
    std::cout << "元素47的排名: " << ost.rank(47) << std::endl;
  } catch (const std::runtime_error &e) {
    std::cout << "异常: " << e.what() << std::endl;
  }

  std::cout << std::endl;
}

void test_select_rank_consistency() {
  std::cout << "=== 测试OS-SELECT和OS-RANK的一致性 ===" << std::endl;

  OrderStatisticTree<int> ost;

  // 插入一些数据
  std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10,
                             25, 35, 45, 55, 65, 75, 85};
  for (int val : values) {
    ost.insert(val);
  }

  std::cout << "树大小: " << ost.size() << std::endl;
  std::cout << "中序遍历: ";
  auto inorder = ost.inorder_traversal();
  for (int val : inorder) {
    std::cout << val << " ";
  }
  std::cout << std::endl;

  // 验证一致性：对于每个排名i，select(i)的排名应该是i
  bool consistent = true;
  for (int i = 1; i <= ost.size(); i++) {
    try {
      int selected_value = ost.select(i);
      int calculated_rank = ost.rank(selected_value);

      if (i != calculated_rank) {
        std::cout << "不一致: select(" << i << ") = " << selected_value
                  << ", rank(" << selected_value << ") = " << calculated_rank
                  << std::endl;
        consistent = false;
      }
    } catch (const std::exception &e) {
      std::cout << "异常: " << e.what() << std::endl;
      consistent = false;
    }
  }

  if (consistent) {
    std::cout << "OS-SELECT和OS-RANK操作完全一致！" << std::endl;
  }

  std::cout << std::endl;
}

void test_dynamic_operations() {
  std::cout << "=== 测试动态操作 ===" << std::endl;

  OrderStatisticTree<int> ost;

  // 初始插入
  std::cout << "初始插入: 5, 2, 8, 1, 3, 7, 9" << std::endl;
  ost.insert(5);
  ost.insert(2);
  ost.insert(8);
  ost.insert(1);
  ost.insert(3);
  ost.insert(7);
  ost.insert(9);

  std::cout << "初始树大小: " << ost.size() << std::endl;
  std::cout << "中序遍历: ";
  auto initial_inorder = ost.inorder_traversal();
  for (int val : initial_inorder) {
    std::cout << val << " ";
  }
  std::cout << std::endl;

  // 测试删除后的顺序统计
  std::cout << "删除元素3" << std::endl;
  ost.remove(3);

  std::cout << "删除后树大小: " << ost.size() << std::endl;
  std::cout << "删除后中序遍历: ";
  auto after_delete_inorder = ost.inorder_traversal();
  for (int val : after_delete_inorder) {
    std::cout << val << " ";
  }
  std::cout << std::endl;

  // 测试删除后的顺序统计
  try {
    std::cout << "删除后第1小的元素: " << ost.select(1) << std::endl;
    std::cout << "删除后第3小的元素: " << ost.select(3) << std::endl;
    std::cout << "删除后第5小的元素: " << ost.select(5) << std::endl;

    std::cout << "删除后元素1的排名: " << ost.rank(1) << std::endl;
    std::cout << "删除后元素5的排名: " << ost.rank(5) << std::endl;
    std::cout << "删除后元素9的排名: " << ost.rank(9) << std::endl;
  } catch (const std::exception &e) {
    std::cout << "异常: " << e.what() << std::endl;
  }

  // 再次插入
  std::cout << "再次插入元素4" << std::endl;
  ost.insert(4);

  std::cout << "插入后树大小: " << ost.size() << std::endl;
  std::cout << "插入后中序遍历: ";
  auto after_insert_inorder = ost.inorder_traversal();
  for (int val : after_insert_inorder) {
    std::cout << val << " ";
  }
  std::cout << std::endl;

  // 测试插入后的顺序统计
  try {
    std::cout << "插入后第1小的元素: " << ost.select(1) << std::endl;
    std::cout << "插入后第4小的元素: " << ost.select(4) << std::endl;
    std::cout << "插入后第6小的元素: " << ost.select(6) << std::endl;

    std::cout << "插入后元素4的排名: " << ost.rank(4) << std::endl;
  } catch (const std::exception &e) {
    std::cout << "异常: " << e.what() << std::endl;
  }

  std::cout << std::endl;
}

void test_edge_cases() {
  std::cout << "=== 测试边界情况 ===" << std::endl;

  // 测试空树
  OrderStatisticTree<int> empty_ost;
  std::cout << "空树是否为空: " << (empty_ost.empty() ? "是" : "否")
            << std::endl;
  std::cout << "空树大小: " << empty_ost.size() << std::endl;

  try {
    empty_ost.select(1);
    std::cout << "ERROR: 应该抛出异常" << std::endl;
  } catch (const std::out_of_range &e) {
    std::cout << "正确捕获空树select异常: " << e.what() << std::endl;
  }

  try {
    empty_ost.rank(5);
    std::cout << "ERROR: 应该抛出异常" << std::endl;
  } catch (const std::runtime_error &e) {
    std::cout << "正确捕获空树rank异常: " << e.what() << std::endl;
  }

  // 测试单节点树
  OrderStatisticTree<int> single_ost;
  single_ost.insert(42);
  std::cout << "单节点树大小: " << single_ost.size() << std::endl;

  try {
    std::cout << "单节点树第1小的元素: " << single_ost.select(1) << std::endl;
    std::cout << "单节点树元素42的排名: " << single_ost.rank(42) << std::endl;
  } catch (const std::exception &e) {
    std::cout << "异常: " << e.what() << std::endl;
  }

  // 测试越界情况
  OrderStatisticTree<int> small_ost;
  small_ost.insert(1);
  small_ost.insert(2);
  small_ost.insert(3);

  try {
    small_ost.select(0);
    std::cout << "ERROR: 应该抛出异常" << std::endl;
  } catch (const std::out_of_range &e) {
    std::cout << "正确捕获越界select(0)异常: " << e.what() << std::endl;
  }

  try {
    small_ost.select(4);
    std::cout << "ERROR: 应该抛出异常" << std::endl;
  } catch (const std::out_of_range &e) {
    std::cout << "正确捕获越界select(4)异常: " << e.what() << std::endl;
  }

  try {
    small_ost.rank(99);
    std::cout << "ERROR: 应该抛出异常" << std::endl;
  } catch (const std::runtime_error &e) {
    std::cout << "正确捕获不存在的元素rank异常: " << e.what() << std::endl;
  }

  std::cout << std::endl;
}

void test_red_black_properties() {
  std::cout << "=== 测试红黑树性质 ===" << std::endl;

  OrderStatisticTree<int> ost;

  // 插入大量数据测试平衡性
  std::vector<int> data;
  for (int i = 1; i <= 50; i++) {
    data.push_back(i);
  }

  // 随机打乱数据
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(data.begin(), data.end(), g);

  for (int val : data) {
    ost.insert(val);
  }

  std::cout << "插入50个随机元素后树大小: " << ost.size() << std::endl;
  std::cout << "黑高度: " << ost.black_height() << std::endl;
  std::cout << "是否有效红黑树: " << (ost.is_valid() ? "是" : "否")
            << std::endl;

  // 验证顺序统计的正确性
  bool correct = true;
  for (int i = 1; i <= ost.size(); i++) {
    try {
      int selected = ost.select(i);
      int rank = ost.rank(selected);

      if (i != rank) {
        std::cout << "错误: select(" << i << ") = " << selected << ", rank("
                  << selected << ") = " << rank << std::endl;
        correct = false;
      }
    } catch (const std::exception &e) {
      std::cout << "异常: " << e.what() << std::endl;
      correct = false;
    }
  }

  if (correct) {
    std::cout << "所有顺序统计操作正确！" << std::endl;
  }

  std::cout << std::endl;
}

void test_string_ost() {
  std::cout << "=== 测试字符串顺序统计树 ===" << std::endl;

  OrderStatisticTree<std::string> string_ost;

  // 插入一些字符串
  string_ost.insert("apple");
  string_ost.insert("banana");
  string_ost.insert("cherry");
  string_ost.insert("date");
  string_ost.insert("elderberry");

  std::cout << "字符串顺序统计树中序遍历: ";
  auto string_inorder = string_ost.inorder_traversal();
  for (const auto &str : string_inorder) {
    std::cout << str << " ";
  }
  std::cout << std::endl;

  std::cout << "树大小: " << string_ost.size() << std::endl;

  // 测试顺序统计操作
  try {
    std::cout << "第1小的字符串: " << string_ost.select(1) << std::endl;
    std::cout << "第3小的字符串: " << string_ost.select(3) << std::endl;
    std::cout << "第5小的字符串: " << string_ost.select(5) << std::endl;

    std::cout << "'banana'的排名: " << string_ost.rank("banana") << std::endl;
    std::cout << "'date'的排名: " << string_ost.rank("date") << std::endl;
    std::cout << "'elderberry'的排名: " << string_ost.rank("elderberry")
              << std::endl;
  } catch (const std::exception &e) {
    std::cout << "异常: " << e.what() << std::endl;
  }

  std::cout << "是否有效红黑树: " << (string_ost.is_valid() ? "是" : "否")
            << std::endl;
  std::cout << std::endl;
}

int main() {
  std::cout << "第14.1章 动态顺序统计演示程序" << std::endl;
  std::cout << "==============================" << std::endl;

  test_basic_operations();
  test_order_statistics();
  test_select_rank_consistency();
  test_dynamic_operations();
  test_edge_cases();
  test_red_black_properties();
  test_string_ost();

  std::cout << "所有测试完成！" << std::endl;

  return 0;
}
