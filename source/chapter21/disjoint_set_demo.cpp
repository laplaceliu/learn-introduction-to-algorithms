#include "disjoint_set.h"
#include <iostream>
#include <vector>

using namespace algorithms;

/**
 * @brief 测试基本的并查集操作
 */
void test_basic_operations() {
  std::cout << "=== 测试基本并查集操作 ===" << std::endl;

  DisjointSet ds(10);

  // 初始状态
  std::cout << "初始状态:" << std::endl;
  ds.print_state();

  // 合并操作
  std::cout << "\n执行合并操作:" << std::endl;
  ds.union_sets(0, 1);
  ds.union_sets(2, 3);
  ds.union_sets(4, 5);
  ds.union_sets(6, 7);
  ds.union_sets(8, 9);

  ds.print_state();

  // 进一步合并
  std::cout << "\n进一步合并:" << std::endl;
  ds.union_sets(1, 2);
  ds.union_sets(5, 6);

  ds.print_state();

  // 查询操作
  std::cout << "\n查询操作:" << std::endl;
  std::cout << "元素0和3是否在同一集合: "
            << (ds.is_same_set(0, 3) ? "是" : "否") << std::endl;
  std::cout << "元素4和8是否在同一集合: "
            << (ds.is_same_set(4, 8) ? "是" : "否") << std::endl;
  std::cout << "元素7和9是否在同一集合: "
            << (ds.is_same_set(7, 9) ? "是" : "否") << std::endl;

  // 集合信息
  std::cout << "\n集合信息:" << std::endl;
  auto reps = ds.get_representatives();
  for (int rep : reps) {
    int size = ds.get_set_size(rep);
    std::cout << "集合 " << rep << " 的大小: " << size << std::endl;

    auto elements = ds.get_set_elements(rep);
    std::cout << "  元素: ";
    for (int elem : elements) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "总集合数量: " << ds.get_set_count() << std::endl;
}

/**
 * @brief 测试路径压缩效果
 */
void test_path_compression() {
  std::cout << "\n=== 测试路径压缩效果 ===" << std::endl;

  DisjointSet ds(10);

  // 创建链式结构: 0->1->2->3->4
  for (int i = 0; i < 4; i++) {
    ds.union_sets(i, i + 1);
  }

  std::cout << "合并后状态（链式结构）:" << std::endl;
  ds.print_state();

  // 查找元素0，这会触发路径压缩
  std::cout << "\n查找元素0（触发路径压缩）:" << std::endl;
  int root = ds.find(0);
  std::cout << "根节点: " << root << std::endl;

  std::cout << "路径压缩后状态:" << std::endl;
  ds.print_state();

  // 验证路径压缩效果
  std::cout << "\n验证路径压缩效果:" << std::endl;
  for (int i = 0; i <= 4; i++) {
    std::cout << "元素 " << i << " 的父节点: " << ds.find(i) << std::endl;
  }
}

/**
 * @brief 测试按秩合并效果
 */
void test_union_by_rank() {
  std::cout << "\n=== 测试按秩合并效果 ===" << std::endl;

  DisjointSet ds(10);

  // 创建两个不同高度的树
  ds.union_sets(0, 1);
  ds.union_sets(0, 2); // 树高度为2

  ds.union_sets(3, 4);
  ds.union_sets(3, 5);
  ds.union_sets(3, 6); // 树高度为2

  std::cout << "合并前状态:" << std::endl;
  ds.print_state();

  // 合并两个树，演示按秩合并
  std::cout << "\n合并树0和树3:" << std::endl;
  ds.union_sets(0, 3);

  std::cout << "合并后状态:" << std::endl;
  ds.print_state();

  // 验证合并结果
  std::cout << "\n验证合并结果:" << std::endl;
  std::cout << "元素0和3的根节点: " << ds.find(0) << " (应该相同)" << std::endl;
  std::cout << "元素0和3是否在同一集合: "
            << (ds.is_same_set(0, 3) ? "是" : "否") << std::endl;
}

/**
 * @brief 测试算法导论第21章图21.1示例
 */
void test_textbook_example() {
  std::cout << "\n=== 测试算法导论图21.1示例 ===" << std::endl;

  DisjointSet ds(16); // 16个元素

  // 按照算法导论图21.1的合并顺序
  int unions[][2] = {{1, 2},   {3, 4},  {3, 5},  {1, 7},  {3, 6},  {8, 9},
                     {1, 8},   {3, 10}, {3, 11}, {3, 12}, {3, 13}, {14, 15},
                     {16, 14}, {3, 14}, {1, 3},  {1, 16}};

  std::cout << "执行算法导论图21.1的合并序列:" << std::endl;

  // 注意：我们的索引从0开始，算法导论从1开始
  for (size_t i = 0; i < sizeof(unions) / sizeof(unions[0]); i++) {
    std::cout << "合并 " << (unions[i][0] - 1) << " 和 " << (unions[i][1] - 1)
              << std::endl;
    ds.union_sets(unions[i][0] - 1, unions[i][1] - 1);
  }

  std::cout << "\n最终状态:" << std::endl;
  ds.print_state();

  // 验证最终结果
  std::cout << "\n验证结果:" << std::endl;
  std::cout << "所有元素是否在同一个集合中: "
            << (ds.get_set_count() == 1 ? "是" : "否") << std::endl;

  if (ds.get_set_count() == 1) {
    std::cout << "最终集合的大小: " << ds.get_set_size(0) << std::endl;

    // 验证所有元素确实在同一个集合中
    bool all_same = true;
    int root = ds.find(0);
    for (int i = 1; i < 16; i++) {
      if (ds.find(i) != root) {
        all_same = false;
        break;
      }
    }
    std::cout << "验证所有元素在同一集合: " << (all_same ? "通过" : "失败")
              << std::endl;
  }
}

/**
 * @brief 测试性能和大规模数据
 */
void test_performance() {
  std::cout << "\n=== 测试性能和大规模数据 ===" << std::endl;

  const int n = 10000;
  std::cout << "创建包含 " << n << " 个元素的并查集" << std::endl;

  DisjointSet ds(n);

  // 执行一系列合并操作
  int operations = 0;
  std::cout << "执行合并操作..." << std::endl;
  for (int i = 0; i < n - 1; i++) {
    ds.union_sets(i, i + 1);
    operations++;
  }

  // 执行一系列查找操作
  std::cout << "执行查找操作..." << std::endl;
  for (int i = 0; i < n; i++) {
    ds.find(i);
    operations++;
  }

  std::cout << "完成 " << operations << " 次操作" << std::endl;
  std::cout << "最终集合数量: " << ds.get_set_count() << std::endl;

  // 验证所有元素在同一个集合中
  if (ds.get_set_count() == 1) {
    std::cout << "验证: 所有元素在同一个集合中 - 通过" << std::endl;
  } else {
    std::cout << "验证: 所有元素在同一个集合中 - 失败" << std::endl;
  }
}

/**
 * @brief 测试边界情况和错误处理
 */
void test_edge_cases() {
  std::cout << "\n=== 测试边界情况和错误处理 ===" << std::endl;

  // 测试单元素集合
  std::cout << "测试单元素集合:" << std::endl;
  DisjointSet ds1(1);
  ds1.print_state();
  std::cout << "集合数量: " << ds1.get_set_count() << std::endl;

  // 测试空集合（应该抛出异常）
  std::cout << "\n测试空集合（应该抛出异常）:" << std::endl;
  try {
    DisjointSet ds0(0);
    std::cout << "错误: 应该抛出异常但没有" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "正确捕获异常: " << e.what() << std::endl;
  }

  // 测试越界访问
  std::cout << "\n测试越界访问:" << std::endl;
  DisjointSet ds5(5);
  try {
    ds5.find(10); // 越界访问
    std::cout << "错误: 应该抛出异常但没有" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "正确捕获异常: " << e.what() << std::endl;
  }

  // 测试自合并
  std::cout << "\n测试自合并:" << std::endl;
  DisjointSet ds3(3);
  ds3.union_sets(1, 1); // 合并同一个元素
  std::cout << "自合并后集合数量: " << ds3.get_set_count() << " (应该为3)"
            << std::endl;

  // 验证自合并不影响结果
  bool valid = (ds3.get_set_count() == 3);
  std::cout << "自合并验证: " << (valid ? "通过" : "失败") << std::endl;
}

/**
 * @brief 测试并查集在连通性问题中的应用
 */
void test_connectivity_application() {
  std::cout << "\n=== 测试并查集在连通性问题中的应用 ===" << std::endl;

  // 模拟图的连通性检测
  const int node_count = 8;
  DisjointSet ds(node_count);

  // 添加边（连接关系）
  int edges[][2] = {{0, 1}, {1, 2}, {2, 3}, {4, 5}, {5, 6}, {6, 7}};

  std::cout << "图结构: 两个连通分量 {0,1,2,3} 和 {4,5,6,7}" << std::endl;

  // 添加所有边
  for (size_t i = 0; i < sizeof(edges) / sizeof(edges[0]); i++) {
    ds.union_sets(edges[i][0], edges[i][1]);
  }

  std::cout << "添加边后的连通分量数量: " << ds.get_set_count() << std::endl;

  // 验证连通性
  std::cout << "\n验证连通性:" << std::endl;
  std::cout << "节点0和3是否连通: " << (ds.is_same_set(0, 3) ? "是" : "否")
            << std::endl;
  std::cout << "节点4和7是否连通: " << (ds.is_same_set(4, 7) ? "是" : "否")
            << std::endl;
  std::cout << "节点0和4是否连通: " << (ds.is_same_set(0, 4) ? "是" : "否")
            << std::endl;

  // 添加连接两个分量的边
  std::cout << "\n添加连接两个分量的边 (3,4):" << std::endl;
  ds.union_sets(3, 4);

  std::cout << "添加边后的连通分量数量: " << ds.get_set_count() << std::endl;
  std::cout << "节点0和4是否连通: " << (ds.is_same_set(0, 4) ? "是" : "否")
            << std::endl;

  // 验证所有节点现在都连通
  bool all_connected = true;
  int root = ds.find(0);
  for (int i = 1; i < node_count; i++) {
    if (ds.find(i) != root) {
      all_connected = false;
      break;
    }
  }

  std::cout << "所有节点是否连通: " << (all_connected ? "是" : "否")
            << std::endl;
}

/**
 * @brief 主函数
 */
int main() {
  std::cout << "========== 第21章 用于不相交集合的数据结构 演示程序 =========="
            << std::endl;
  std::cout << "算法导论第21章：并查集（Disjoint Set Union）实现" << std::endl;
  std::cout << "支持路径压缩和按秩合并优化" << std::endl;
  std::cout << "============================================================"
            << std::endl;

  // 运行所有测试
  test_basic_operations();
  test_path_compression();
  test_union_by_rank();
  test_textbook_example();
  test_performance();
  test_edge_cases();
  test_connectivity_application();

  std::cout << "\n========== 所有测试完成 ==========" << std::endl;

  // 使用类中的演示函数
  std::cout << "\n=== 使用DisjointSetDemo类运行演示 ===" << std::endl;
  DisjointSetDemo::run_all_demos();

  return 0;
}
