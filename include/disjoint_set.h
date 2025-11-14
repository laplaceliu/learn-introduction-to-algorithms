#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <iostream>
#include <stdexcept>
#include <vector>

namespace algorithms {

/**
 * @brief 并查集数据结构（Disjoint Set Union, DSU）
 *
 * 实现算法导论第21章"用于不相交集合的数据结构"
 * 支持路径压缩和按秩合并优化
 */
class DisjointSet {
private:
  std::vector<int> parent; // 父节点数组
  std::vector<int> rank;   // 秩（树的高度）
  int set_count;           // 集合数量

public:
  /**
   * @brief 构造函数，初始化n个单元素集合
   * @param n 元素数量
   */
  DisjointSet(int n) : set_count(n) {
    if (n <= 0) {
      throw std::invalid_argument("元素数量必须大于0");
    }

    parent.resize(n);
    rank.resize(n, 0);

    // 初始化每个元素为独立的集合
    for (int i = 0; i < n; i++) {
      parent[i] = i;
    }
  }

  /**
   * @brief 查找元素x所在集合的代表元素（带路径压缩）
   * @param x 元素索引
   * @return 集合的代表元素
   */
  int find(int x) {
    if (x < 0 || x >= static_cast<int>(parent.size())) {
      throw std::out_of_range("元素索引超出范围");
    }

    // 路径压缩：在查找过程中将路径上的节点直接连接到根节点
    if (parent[x] != x) {
      parent[x] = find(parent[x]);
    }
    return parent[x];
  }

  /**
   * @brief 合并元素x和y所在的集合（按秩合并）
   * @param x 第一个元素
   * @param y 第二个元素
   */
  void union_sets(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);

    if (rootX == rootY) {
      return; // 已经在同一个集合中
    }

    // 按秩合并：将秩较小的树合并到秩较大的树下
    if (rank[rootX] < rank[rootY]) {
      parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
      parent[rootY] = rootX;
    } else {
      // 秩相等时，任意选择，并增加新根的秩
      parent[rootY] = rootX;
      rank[rootX]++;
    }

    set_count--;
  }

  /**
   * @brief 检查两个元素是否在同一个集合中
   * @param x 第一个元素
   * @param y 第二个元素
   * @return true如果在同一个集合中，否则false
   */
  bool is_same_set(int x, int y) { return find(x) == find(y); }

  /**
   * @brief 获取集合数量
   * @return 当前集合的数量
   */
  int get_set_count() const { return set_count; }

  /**
   * @brief 获取元素数量
   * @return 元素的总数量
   */
  int get_element_count() const { return static_cast<int>(parent.size()); }

  /**
   * @brief 获取指定集合的大小
   * @param x 集合中的任意元素
   * @return 集合的大小
   */
  int get_set_size(int x) {
    int root = find(x);
    int size = 0;

    // 遍历所有元素，统计属于该集合的元素数量
    for (int i = 0; i < static_cast<int>(parent.size()); i++) {
      if (find(i) == root) {
        size++;
      }
    }

    return size;
  }

  /**
   * @brief 打印并查集的当前状态
   */
  void print_state() const {
    std::cout << "并查集状态:" << std::endl;
    std::cout << "元素数量: " << parent.size() << std::endl;
    std::cout << "集合数量: " << set_count << std::endl;

    std::cout << "父节点数组: ";
    for (size_t i = 0; i < parent.size(); i++) {
      std::cout << parent[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "秩数组: ";
    for (size_t i = 0; i < rank.size(); i++) {
      std::cout << rank[i] << " ";
    }
    std::cout << std::endl;
  }

  /**
   * @brief 获取所有集合的代表元素
   * @return 代表元素的向量
   */
  std::vector<int> get_representatives() {
    std::vector<int> reps;
    for (int i = 0; i < static_cast<int>(parent.size()); i++) {
      if (parent[i] == i) {
        reps.push_back(i);
      }
    }
    return reps;
  }

  /**
   * @brief 获取指定集合的所有元素
   * @param representative 集合的代表元素
   * @return 该集合的所有元素
   */
  std::vector<int> get_set_elements(int representative) {
    int root = find(representative);
    std::vector<int> elements;

    for (int i = 0; i < static_cast<int>(parent.size()); i++) {
      if (find(i) == root) {
        elements.push_back(i);
      }
    }

    return elements;
  }
};

/**
 * @brief 算法导论第21章经典示例和测试
 */
class DisjointSetDemo {
public:
  /**
   * @brief 演示基本的并查集操作
   */
  static void demo_basic_operations() {
    std::cout << "=== 基本并查集操作演示 ===" << std::endl;

    DisjointSet ds(10);
    ds.print_state();

    // 合并一些集合
    std::cout << "\n合并操作:" << std::endl;
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

    // 测试查询操作
    std::cout << "\n查询操作:" << std::endl;
    std::cout << "元素0和3是否在同一集合: "
              << (ds.is_same_set(0, 3) ? "是" : "否") << std::endl;
    std::cout << "元素4和8是否在同一集合: "
              << (ds.is_same_set(4, 8) ? "是" : "否") << std::endl;

    // 获取集合信息
    std::cout << "\n集合信息:" << std::endl;
    auto reps = ds.get_representatives();
    for (int rep : reps) {
      std::cout << "集合 " << rep << " 的大小: " << ds.get_set_size(rep)
                << std::endl;
    }
  }

  /**
   * @brief 演示路径压缩的效果
   */
  static void demo_path_compression() {
    std::cout << "\n=== 路径压缩效果演示 ===" << std::endl;

    // 创建一个链式结构来演示路径压缩
    DisjointSet ds(10);

    // 创建链式结构: 0->1->2->3->4
    for (int i = 0; i < 4; i++) {
      ds.union_sets(i, i + 1);
    }

    std::cout << "合并前状态:" << std::endl;
    ds.print_state();

    // 查找元素0，这会触发路径压缩
    std::cout << "\n查找元素0（触发路径压缩）:" << std::endl;
    int root = ds.find(0);
    std::cout << "根节点: " << root << std::endl;

    std::cout << "路径压缩后状态:" << std::endl;
    ds.print_state();
  }

  /**
   * @brief 演示按秩合并的效果
   */
  static void demo_union_by_rank() {
    std::cout << "\n=== 按秩合并效果演示 ===" << std::endl;

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
  }

  /**
   * @brief 算法导论第21章图21.1示例
   */
  static void demo_textbook_example() {
    std::cout << "\n=== 算法导论图21.1示例 ===" << std::endl;

    DisjointSet ds(16); // 16个元素

    // 按照算法导论图21.1的合并顺序
    int unions[][2] = {{1, 2},   {3, 4},  {3, 5},  {1, 7},  {3, 6},  {8, 9},
                       {1, 8},   {3, 10}, {3, 11}, {3, 12}, {3, 13}, {14, 15},
                       {16, 14}, {3, 14}, {1, 3},  {1, 16}};

    // 注意：我们的索引从0开始，算法导论从1开始
    for (size_t i = 0; i < sizeof(unions) / sizeof(unions[0]); i++) {
      ds.union_sets(unions[i][0] - 1, unions[i][1] - 1);
    }

    ds.print_state();

    // 验证最终结果
    std::cout << "\n验证结果:" << std::endl;
    std::cout << "所有元素是否在同一个集合中: "
              << (ds.get_set_count() == 1 ? "是" : "否") << std::endl;

    if (ds.get_set_count() == 1) {
      std::cout << "最终集合的大小: " << ds.get_set_size(0) << std::endl;
    }
  }

  /**
   * @brief 性能测试：比较有优化和无优化的并查集
   */
  static void demo_performance_comparison() {
    std::cout << "\n=== 性能对比演示 ===" << std::endl;

    // 测试大规模数据下的性能
    const int n = 10000;
    DisjointSet ds(n);

    // 执行一系列合并操作
    int operations = 0;
    for (int i = 0; i < n - 1; i++) {
      ds.union_sets(i, i + 1);
      operations++;
    }

    // 执行一系列查找操作
    for (int i = 0; i < n; i++) {
      ds.find(i);
      operations++;
    }

    std::cout << "完成 " << operations << " 次操作" << std::endl;
    std::cout << "最终集合数量: " << ds.get_set_count() << std::endl;
    std::cout << "性能测试完成" << std::endl;
  }

  /**
   * @brief 运行所有演示
   */
  static void run_all_demos() {
    std::cout << "========== 第21章 用于不相交集合的数据结构 =========="
              << std::endl;

    demo_basic_operations();
    demo_path_compression();
    demo_union_by_rank();
    demo_textbook_example();
    demo_performance_comparison();

    std::cout << "\n========== 所有演示完成 ==========" << std::endl;
  }
};

} // namespace algorithms

#endif // DISJOINT_SET_H
