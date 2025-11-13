#include "b_tree.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

// 测试B树的基本操作
void test_basic_operations() {
  std::cout << "=== 测试B树基本操作 ===" << std::endl;

  // 创建最小度数为3的B树
  BTree<int> btree(3);

  // 插入一些键值
  std::vector<int> keys = {10, 20, 5, 6, 12, 30, 7, 17};

  std::cout << "插入键值: ";
  for (int key : keys) {
    std::cout << key << " ";
    btree.insert(key);
  }
  std::cout << std::endl;

  // 遍历B树
  auto result = btree.traverse();
  std::cout << "中序遍历结果: ";
  for (int key : result) {
    std::cout << key << " ";
  }
  std::cout << std::endl;

  // 验证B树性质
  std::cout << "B树性质验证: " << (btree.validate() ? "通过" : "失败")
            << std::endl;
  std::cout << "B树高度: " << btree.height() << std::endl;

  // 搜索测试
  std::cout << "搜索测试:" << std::endl;
  for (int key : {5, 15, 20, 25}) {
    auto node = btree.search(key);
    std::cout << "搜索 " << key << ": " << (node != nullptr ? "找到" : "未找到")
              << std::endl;
  }

  std::cout << std::endl;
}

// 测试B树的删除操作
void test_delete_operations() {
  std::cout << "=== 测试B树删除操作 ===" << std::endl;

  BTree<int> btree(3);

  // 插入更多键值
  std::vector<int> keys = {1,  3,  7,  10, 11, 13, 14, 15, 18, 16, 19, 24,
                           25, 26, 21, 4,  5,  20, 22, 2,  17, 12, 6};

  std::cout << "插入键值: ";
  for (int key : keys) {
    btree.insert(key);
    std::cout << key << " ";
  }
  std::cout << std::endl;

  auto initial_traverse = btree.traverse();
  std::cout << "删除前中序遍历: ";
  for (int key : initial_traverse) {
    std::cout << key << " ";
  }
  std::cout << std::endl;

  std::cout << "删除前B树结构:" << std::endl;
  btree.print();

  // 删除一些键值
  std::vector<int> keys_to_delete = {6, 13, 7, 4, 2, 16};

  std::cout << "删除键值: ";
  for (int key : keys_to_delete) {
    bool success = btree.remove(key);
    std::cout << key << "(" << (success ? "成功" : "失败") << ") ";
  }
  std::cout << std::endl;

  auto final_traverse = btree.traverse();
  std::cout << "删除后中序遍历: ";
  for (int key : final_traverse) {
    std::cout << key << " ";
  }
  std::cout << std::endl;

  std::cout << "删除后B树结构:" << std::endl;
  btree.print();

  std::cout << "B树性质验证: " << (btree.validate() ? "通过" : "失败")
            << std::endl;
  std::cout << "B树高度: " << btree.height() << std::endl;

  std::cout << std::endl;
}

// 测试B树的边界情况
void test_edge_cases() {
  std::cout << "=== 测试B树边界情况 ===" << std::endl;

  // 测试空树
  {
    BTree<int> btree(2);
    std::cout << "空树遍历: ";
    auto result = btree.traverse();
    for (int key : result) {
      std::cout << key << " ";
    }
    std::cout << "(空)" << std::endl;

    std::cout << "空树搜索 5: "
              << (btree.search(5) != nullptr ? "找到" : "未找到") << std::endl;
    std::cout << "空树删除 5: " << (btree.remove(5) ? "成功" : "失败")
              << std::endl;
    std::cout << "空树验证: " << (btree.validate() ? "通过" : "失败")
              << std::endl;
  }

  // 测试单节点树
  {
    BTree<int> btree(2);
    btree.insert(10);

    std::cout << "单节点树遍历: ";
    auto result = btree.traverse();
    for (int key : result) {
      std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "单节点树验证: " << (btree.validate() ? "通过" : "失败")
              << std::endl;

    // 删除唯一节点
    btree.remove(10);
    std::cout << "删除唯一节点后验证: " << (btree.validate() ? "通过" : "失败")
              << std::endl;
  }

  // 测试重复插入和删除
  {
    BTree<int> btree(3);

    // 重复插入相同键值
    btree.insert(5);
    btree.insert(5); // 重复插入

    auto result = btree.traverse();
    std::cout << "重复插入后遍历: ";
    for (int key : result) {
      std::cout << key << " ";
    }
    std::cout << std::endl;

    // 删除不存在的键值
    std::cout << "删除不存在的键值 100: "
              << (btree.remove(100) ? "成功" : "失败") << std::endl;

    std::cout << "重复操作验证: " << (btree.validate() ? "通过" : "失败")
              << std::endl;
  }

  std::cout << std::endl;
}

// 测试不同最小度数的B树
void test_different_degrees() {
  std::cout << "=== 测试不同最小度数的B树 ===" << std::endl;

  // 测试最小度数为2的B树
  {
    BTree<int> btree(2);
    std::vector<int> keys = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int key : keys) {
      btree.insert(key);
    }

    auto result = btree.traverse();
    std::cout << "最小度数2的B树遍历: ";
    for (int key : result) {
      std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "最小度数2验证: " << (btree.validate() ? "通过" : "失败")
              << std::endl;
    std::cout << "高度: " << btree.height() << std::endl;
  }

  // 测试最小度数为4的B树
  {
    BTree<int> btree(4);
    std::vector<int> keys = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    for (int key : keys) {
      btree.insert(key);
    }

    auto result = btree.traverse();
    std::cout << "最小度数4的B树遍历: ";
    for (int key : result) {
      std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "最小度数4验证: " << (btree.validate() ? "通过" : "失败")
              << std::endl;
    std::cout << "高度: " << btree.height() << std::endl;
  }

  std::cout << std::endl;
}

// 性能测试：插入大量数据
void test_performance() {
  std::cout << "=== 性能测试：插入大量数据 ===" << std::endl;

  BTree<int> btree(3);

  // 生成100个随机数
  std::vector<int> keys;
  for (int i = 0; i < 100; i++) {
    keys.push_back(i + 1);
  }

  // 打乱顺序
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(keys.begin(), keys.end(), g);

  // 插入所有键值
  for (int key : keys) {
    btree.insert(key);
  }

  // 验证所有键值都存在
  bool all_found = true;
  for (int i = 1; i <= 100; i++) {
    if (btree.search(i) == nullptr) {
      all_found = false;
      break;
    }
  }

  std::cout << "插入100个键值后验证: "
            << (all_found ? "所有键值都存在" : "有键值丢失") << std::endl;
  std::cout << "B树性质验证: " << (btree.validate() ? "通过" : "失败")
            << std::endl;
  std::cout << "B树高度: " << btree.height() << std::endl;

  // 遍历结果应该是排序的
  auto result = btree.traverse();
  bool is_sorted = std::is_sorted(result.begin(), result.end());
  std::cout << "遍历结果是否排序: " << (is_sorted ? "是" : "否") << std::endl;

  std::cout << std::endl;
}

// 与算法导论示例对比
void test_clrs_example() {
  std::cout << "=== 与算法导论示例对比 ===" << std::endl;

  // 创建最小度数为2的B树（对应算法导论中的t=2）
  BTree<int> btree(2);

  // 插入算法导论图18.7中的键值（使用字母对应的ASCII值）
  std::vector<int> numeric_keys = {'G', 'M', 'P', 'X', 'A', 'C', 'D',
                                   'E', 'J', 'K', 'N', 'O', 'R', 'S',
                                   'T', 'U', 'V', 'Y', 'Z'};

  for (int key : numeric_keys) {
    btree.insert(key);
  }

  std::cout << "算法导论示例键值插入完成" << std::endl;

  auto result = btree.traverse();
  std::cout << "中序遍历结果: ";
  for (int key : result) {
    std::cout << key << " ";
  }
  std::cout << std::endl;

  std::cout << "B树结构:" << std::endl;
  btree.print();

  std::cout << "B树性质验证: " << (btree.validate() ? "通过" : "失败")
            << std::endl;
  std::cout << "B树高度: " << btree.height() << std::endl;

  // 测试算法导论中的删除操作
  std::cout << "测试删除操作:" << std::endl;

  // 删除F（对应数字6）
  btree.remove(6); // F不存在，应该失败
  std::cout << "删除F(6): " << (btree.remove(6) ? "成功" : "失败") << std::endl;

  // 删除M（对应数字13）
  std::cout << "删除M(13): " << (btree.remove(13) ? "成功" : "失败")
            << std::endl;

  std::cout << "删除后B树结构:" << std::endl;
  btree.print();

  std::cout << std::endl;
}

int main() {
  std::cout << "算法导论第18章 B树演示程序" << std::endl;
  std::cout << "============================" << std::endl;
  std::cout << std::endl;

  test_basic_operations();
  test_delete_operations();
  test_edge_cases();
  test_different_degrees();
  test_performance();
  test_clrs_example();

  std::cout << "所有测试完成!" << std::endl;

  return 0;
}
