#include "van_emde_boas_tree.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace algorithms;

/**
 * @brief 测试基本的vEB树操作
 */
void test_basic_operations() {
  std::cout << "=== 测试基本vEB树操作 ===" << std::endl;

  // 创建全域大小为16的vEB树
  VanEmdeBoasTree veb(16);

  std::cout << "初始状态:" << std::endl;
  veb.print_tree();
  std::cout << "是否为空: " << (veb.is_empty() ? "是" : "否") << std::endl;

  // 插入元素
  std::cout << "\n插入元素: 2, 3, 4, 5, 7, 14, 15" << std::endl;
  veb.insert(2);
  veb.insert(3);
  veb.insert(4);
  veb.insert(5);
  veb.insert(7);
  veb.insert(14);
  veb.insert(15);

  std::cout << "插入后状态:" << std::endl;
  veb.print_tree();
  std::cout << "最小值: " << veb.get_min() << std::endl;
  std::cout << "最大值: " << veb.get_max() << std::endl;
  std::cout << "元素数量: " << veb.size() << std::endl;

  // 测试包含查询
  std::cout << "\n包含查询:" << std::endl;
  std::cout << "包含2: " << (veb.contains(2) ? "是" : "否") << std::endl;
  std::cout << "包含6: " << (veb.contains(6) ? "是" : "否") << std::endl;
  std::cout << "包含15: " << (veb.contains(15) ? "是" : "否") << std::endl;
  std::cout << "包含0: " << (veb.contains(0) ? "是" : "否") << std::endl;

  // 测试前驱和后继
  std::cout << "\n前驱和后继查询:" << std::endl;
  std::cout << "6的后继: " << veb.successor(6) << " (应该是7)" << std::endl;
  std::cout << "8的前驱: " << veb.predecessor(8) << " (应该是7)" << std::endl;
  std::cout << "14的后继: " << veb.successor(14) << " (应该是15)" << std::endl;
  std::cout << "15的后继: " << veb.successor(15) << " (应该不存在，返回-1)"
            << std::endl;
  std::cout << "0的前驱: " << veb.predecessor(0) << " (应该不存在，返回-1)"
            << std::endl;

  // 获取所有元素
  auto elements = veb.get_elements();
  std::sort(elements.begin(), elements.end());
  std::cout << "\n所有元素: ";
  for (int elem : elements) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
}

/**
 * @brief 测试算法导论图20.3示例
 */
void test_textbook_example() {
  std::cout << "\n=== 测试算法导论图20.3示例 ===" << std::endl;

  // 创建全域大小为16的vEB树，对应算法导论图20.3
  VanEmdeBoasTree veb(16);

  // 插入图20.3中的元素：2, 3, 4, 5, 7, 14, 15
  int elements[] = {2, 3, 4, 5, 7, 14, 15};

  std::cout << "插入元素: ";
  for (int elem : elements) {
    std::cout << elem << " ";
    veb.insert(elem);
  }
  std::cout << std::endl;

  std::cout << "vEB树结构:" << std::endl;
  veb.print_tree();

  // 验证最小值、最大值
  std::cout << "最小值: " << veb.get_min() << " (应该是2)" << std::endl;
  std::cout << "最大值: " << veb.get_max() << " (应该是15)" << std::endl;

  // 测试前驱和后继操作
  std::cout << "\n测试前驱和后继:" << std::endl;
  std::cout << "6的后继: " << veb.successor(6) << " (应该是7)" << std::endl;
  std::cout << "7的前驱: " << veb.predecessor(7) << " (应该是5)" << std::endl;
  std::cout << "13的后继: " << veb.successor(13) << " (应该是14)" << std::endl;
  std::cout << "15的后继: " << veb.successor(15) << " (应该不存在，返回-1)"
            << std::endl;

  // 验证所有元素
  auto all_elements = veb.get_elements();
  std::sort(all_elements.begin(), all_elements.end());

  std::cout << "\n验证所有元素: ";
  for (int elem : all_elements) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  bool correct = (all_elements.size() == 7) && (all_elements[0] == 2) &&
                 (all_elements[1] == 3) && (all_elements[2] == 4) &&
                 (all_elements[3] == 5) && (all_elements[4] == 7) &&
                 (all_elements[5] == 14) && (all_elements[6] == 15);

  std::cout << "元素验证: " << (correct ? "通过" : "失败") << std::endl;
}

/**
 * @brief 测试删除操作
 */
void test_delete_operations() {
  std::cout << "\n=== 测试删除操作 ===" << std::endl;

  VanEmdeBoasTree veb(16);

  // 插入元素
  veb.insert(2);
  veb.insert(3);
  veb.insert(5);
  veb.insert(8);
  veb.insert(10);
  veb.insert(12);
  veb.insert(14);

  std::cout << "插入后状态:" << std::endl;
  veb.print_tree();
  std::cout << "元素数量: " << veb.size() << std::endl;

  // 删除一些元素
  std::cout << "\n删除元素: 5, 10" << std::endl;
  veb.remove(5);
  veb.remove(10);

  std::cout << "删除后状态:" << std::endl;
  veb.print_tree();
  std::cout << "元素数量: " << veb.size() << std::endl;

  // 验证删除结果
  std::cout << "\n验证删除结果:" << std::endl;
  std::cout << "包含5: " << (veb.contains(5) ? "是（错误）" : "否（正确）")
            << std::endl;
  std::cout << "包含10: " << (veb.contains(10) ? "是（错误）" : "否（正确）")
            << std::endl;
  std::cout << "包含8: " << (veb.contains(8) ? "是（正确）" : "否（错误）")
            << std::endl;

  // 测试前驱和后继
  std::cout << "\n删除后前驱和后继测试:" << std::endl;
  std::cout << "4的后继: " << veb.successor(4) << " (应该是8)" << std::endl;
  std::cout << "9的前驱: " << veb.predecessor(9) << " (应该是8)" << std::endl;
  std::cout << "11的后继: " << veb.successor(11) << " (应该是12)" << std::endl;

  // 删除更多元素
  std::cout << "\n删除元素: 2, 14" << std::endl;
  veb.remove(2);
  veb.remove(14);

  std::cout << "最终状态:" << std::endl;
  veb.print_tree();
  std::cout << "元素数量: " << veb.size() << std::endl;
  std::cout << "最小值: " << veb.get_min() << std::endl;
  std::cout << "最大值: " << veb.get_max() << std::endl;
}

/**
 * @brief 测试不同全域大小的vEB树
 */
void test_different_universe_sizes() {
  std::cout << "\n=== 测试不同全域大小 ===" << std::endl;

  // 测试全域大小为4
  std::cout << "全域大小4:" << std::endl;
  VanEmdeBoasTree veb4(4);
  veb4.insert(0);
  veb4.insert(3);
  veb4.print_tree();
  std::cout << "最小值: " << veb4.get_min() << ", 最大值: " << veb4.get_max()
            << std::endl;

  // 测试全域大小为8
  std::cout << "\n全域大小8:" << std::endl;
  VanEmdeBoasTree veb8(8);
  veb8.insert(1);
  veb8.insert(4);
  veb8.insert(6);
  veb8.print_tree();
  std::cout << "元素数量: " << veb8.size() << std::endl;

  // 测试全域大小为32
  std::cout << "\n全域大小32:" << std::endl;
  VanEmdeBoasTree veb32(32);
  veb32.insert(5);
  veb32.insert(10);
  veb32.insert(15);
  veb32.insert(20);
  veb32.insert(25);
  veb32.insert(30);
  veb32.print_tree();
  std::cout << "元素数量: " << veb32.size() << std::endl;

  // 测试前驱和后继
  std::cout << "\n全域大小32的前驱和后继测试:" << std::endl;
  std::cout << "12的后继: " << veb32.successor(12) << " (应该是15)"
            << std::endl;
  std::cout << "18的前驱: " << veb32.predecessor(18) << " (应该是15)"
            << std::endl;
  std::cout << "28的后继: " << veb32.successor(28) << " (应该是30)"
            << std::endl;
}

/**
 * @brief 测试边界情况
 */
void test_edge_cases() {
  std::cout << "\n=== 测试边界情况 ===" << std::endl;

  // 测试空树
  std::cout << "空树测试:" << std::endl;
  VanEmdeBoasTree veb(16);
  std::cout << "是否为空: " << (veb.is_empty() ? "是" : "否") << std::endl;
  std::cout << "最小值: " << veb.get_min() << " (应该是-1)" << std::endl;
  std::cout << "最大值: " << veb.get_max() << " (应该是-1)" << std::endl;

  // 测试单元素树
  std::cout << "\n单元素树测试:" << std::endl;
  veb.insert(8);
  std::cout << "是否为空: " << (veb.is_empty() ? "是" : "否") << std::endl;
  std::cout << "最小值: " << veb.get_min() << " (应该是8)" << std::endl;
  std::cout << "最大值: " << veb.get_max() << " (应该是8)" << std::endl;
  std::cout << "元素数量: " << veb.size() << " (应该是1)" << std::endl;

  // 测试边界值
  std::cout << "\n边界值测试:" << std::endl;
  veb.insert(0);  // 最小值
  veb.insert(15); // 最大值
  std::cout << "包含0: " << (veb.contains(0) ? "是" : "否") << std::endl;
  std::cout << "包含15: " << (veb.contains(15) ? "是" : "否") << std::endl;
  std::cout << "-1的后继: " << veb.successor(-1) << " (应该是0)" << std::endl;
  std::cout << "16的前驱: " << veb.predecessor(16) << " (应该是15)"
            << std::endl;

  // 测试重复插入
  std::cout << "\n重复插入测试:" << std::endl;
  veb.insert(8); // 重复插入
  std::cout << "重复插入8后元素数量: " << veb.size() << " (应该不变)"
            << std::endl;

  // 测试删除不存在的元素
  std::cout << "\n删除不存在的元素测试:" << std::endl;
  try {
    veb.remove(100); // 删除不存在的元素
    std::cout << "删除不存在的元素后元素数量: " << veb.size() << " (应该不变)"
              << std::endl;
  } catch (const std::out_of_range &e) {
    std::cout << "正确捕获异常: " << e.what() << std::endl;
    std::cout << "删除不存在的元素后元素数量: " << veb.size() << " (应该不变)"
              << std::endl;
  }
}

/**
 * @brief 测试vEB树的时间复杂度优势
 */
void test_performance_characteristics() {
  std::cout << "\n=== 测试时间复杂度特性 ===" << std::endl;

  // 创建较大的vEB树
  VanEmdeBoasTree veb(1024); // 全域大小1024

  // 插入一些元素
  for (int i = 0; i < 100; i += 3) {
    veb.insert(i);
  }

  std::cout << "插入100个元素后的元素数量: " << veb.size() << std::endl;

  // 测试查找操作
  int search_operations = 0;
  for (int i = 0; i < 1000; i++) {
    if (veb.contains(i % 1024)) {
      search_operations++;
    }
  }
  std::cout << "完成1000次查找操作，成功查找次数: " << search_operations
            << std::endl;

  // 测试前驱操作
  int predecessor_operations = 0;
  for (int i = 0; i < 500; i++) {
    veb.predecessor(i % 1024);
    predecessor_operations++;
  }
  std::cout << "完成500次前驱操作" << std::endl;

  // 测试后继操作
  int successor_operations = 0;
  for (int i = 0; i < 500; i++) {
    veb.successor(i % 1024);
    successor_operations++;
  }
  std::cout << "完成500次后继操作" << std::endl;

  std::cout << "所有操作完成，vEB树支持O(log log u)时间复杂度的操作"
            << std::endl;
  std::cout << "对于全域大小1024，log log 1024 ≈ 3.32，操作非常高效"
            << std::endl;
}

/**
 * @brief 测试vEB树在算法中的应用
 */
void test_algorithm_applications() {
  std::cout << "\n=== 测试vEB树在算法中的应用 ===" << std::endl;

  // 应用1: 快速查找缺失的最小正整数
  std::cout << "应用1: 查找缺失的最小正整数" << std::endl;
  VanEmdeBoasTree veb(32);

  // 插入一些正整数
  int present[] = {1, 2, 4, 5, 7, 8, 10, 12, 15, 18, 20};
  for (int num : present) {
    veb.insert(num);
  }

  // 查找缺失的最小正整数
  int missing = 0;
  for (int i = 1; i < 32; i++) {
    if (!veb.contains(i)) {
      missing = i;
      break;
    }
  }

  std::cout << "缺失的最小正整数: " << missing << " (应该是3)" << std::endl;

  // 应用2: 维护动态集合的前驱后继
  std::cout << "\n应用2: 动态集合的前驱后继维护" << std::endl;
  VanEmdeBoasTree dynamic_set(64);

  // 动态插入和删除
  dynamic_set.insert(10);
  dynamic_set.insert(20);
  dynamic_set.insert(30);
  dynamic_set.insert(40);

  std::cout << "插入10,20,30,40后的前驱后继:" << std::endl;
  std::cout << "15的后继: " << dynamic_set.successor(15) << " (应该是20)"
            << std::endl;
  std::cout << "25的前驱: " << dynamic_set.predecessor(25) << " (应该是20)"
            << std::endl;

  // 动态删除
  dynamic_set.remove(20);
  std::cout << "\n删除20后的前驱后继:" << std::endl;
  std::cout << "15的后继: " << dynamic_set.successor(15) << " (应该是30)"
            << std::endl;
  std::cout << "25的前驱: " << dynamic_set.predecessor(25) << " (应该是10)"
            << std::endl;
}

/**
 * @brief 主函数
 */
int main() {
  std::cout << "========== 第20章 van Emde Boas树 演示程序 =========="
            << std::endl;
  std::cout << "算法导论第20章：van Emde Boas树实现" << std::endl;
  std::cout << "支持O(log log u)时间复杂度的查找、插入、删除、前驱、后继操作"
            << std::endl;
  std::cout << "======================================================"
            << std::endl;

  // 运行所有测试
  test_basic_operations();
  test_textbook_example();
  test_delete_operations();
  test_different_universe_sizes();
  test_edge_cases();
  test_performance_characteristics();
  test_algorithm_applications();

  std::cout << "\n========== 所有测试完成 ==========" << std::endl;

  // 使用类中的演示函数
  std::cout << "\n=== 使用VanEmdeBoasTreeDemo类运行演示 ===" << std::endl;
  VanEmdeBoasTreeDemo::run_all_demos();

  return 0;
}
