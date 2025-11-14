#ifndef VAN_EMDE_BOAS_TREE_H
#define VAN_EMDE_BOAS_TREE_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace algorithms {

/**
 * @brief van Emde Boas树数据结构
 *
 * 实现算法导论第20章"van Emde Boas树"
 * 支持快速查找、插入、删除、前驱、后继等操作
 * 适用于处理0到u-1范围内的整数集合
 */
class VanEmdeBoasTree {
private:
  int universe_size;                                      // 全域大小u
  int min_val;                                            // 树中的最小值
  int max_val;                                            // 树中的最大值
  std::unique_ptr<VanEmdeBoasTree> summary;               // 摘要结构
  std::vector<std::unique_ptr<VanEmdeBoasTree>> clusters; // 簇数组

  /**
   * @brief 计算高函数：返回x的高位部分
   * @param x 输入值
   * @return 高位部分
   */
  int high(int x) const {
    // 计算cluster_size（与构造函数中相同）
    int k = 0;
    int temp = universe_size;
    while (temp > 1) {
      temp >>= 1;
      k++;
    }
    int cluster_size = 1 << ((k + 1) / 2);
    int cluster_universe_size = 1 << (k / 2);
    if (cluster_size * cluster_universe_size < universe_size) {
      cluster_universe_size = 1 << ((k + 1) / 2);
    }
    return x / cluster_universe_size;
  }

  /**
   * @brief 计算低函数：返回x的低位部分
   * @param x 输入值
   * @return 低位部分
   */
  int low(int x) const {
    // 计算cluster_size（与构造函数中相同）
    int k = 0;
    int temp = universe_size;
    while (temp > 1) {
      temp >>= 1;
      k++;
    }
    int cluster_size = 1 << ((k + 1) / 2);
    int cluster_universe_size = 1 << (k / 2);
    if (cluster_size * cluster_universe_size < universe_size) {
      cluster_universe_size = 1 << ((k + 1) / 2);
    }
    return x % cluster_universe_size;
  }

  /**
   * @brief 根据高位和低位计算索引
   * @param high 高位部分
   * @param low 低位部分
   * @return 完整索引
   */
  int index(int high, int low) const {
    // 计算cluster_size（与构造函数中相同）
    int k = 0;
    int temp = universe_size;
    while (temp > 1) {
      temp >>= 1;
      k++;
    }
    int cluster_size = 1 << ((k + 1) / 2);
    int cluster_universe_size = 1 << (k / 2);
    if (cluster_size * cluster_universe_size < universe_size) {
      cluster_universe_size = 1 << ((k + 1) / 2);
    }
    return high * cluster_universe_size + low;
  }

public:
  /**
   * @brief 构造函数
   * @param u 全域大小，必须是2的幂
   */
  VanEmdeBoasTree(int u) : universe_size(u), min_val(-1), max_val(-1) {
    if (u <= 0) {
      throw std::invalid_argument("全域大小必须大于0");
    }

    // 检查u是否是2的幂
    if ((u & (u - 1)) != 0) {
      throw std::invalid_argument("全域大小必须是2的幂");
    }

    // 基础情况：u=2
    if (u == 2) {
      summary = nullptr;
      clusters.clear();
      return;
    }

    // 递归构建vEB树结构
    // 对于vEB树，我们需要找到k使得2^k = u
    // 然后cluster_size = 2^(ceil(k/2))，每个簇的大小是2^(floor(k/2))
    int k = 0;
    int temp = u;
    while (temp > 1) {
      temp >>= 1;
      k++;
    }

    // cluster_size = 2^(ceil(k/2))
    int cluster_size = 1 << ((k + 1) / 2);

    // 每个簇的大小是2^(floor(k/2))
    int cluster_universe_size = 1 << (k / 2);

    // 验证：cluster_size * cluster_universe_size >= u
    if (cluster_size * cluster_universe_size < u) {
      // 如果不够大，调整cluster_universe_size
      cluster_universe_size = 1 << ((k + 1) / 2);
    }

    summary = std::make_unique<VanEmdeBoasTree>(cluster_size);
    clusters.resize(cluster_size);

    for (int i = 0; i < cluster_size; i++) {
      clusters[i] = std::make_unique<VanEmdeBoasTree>(cluster_universe_size);
    }
  }

  /**
   * @brief 检查树是否为空
   * @return true如果树为空
   */
  bool is_empty() const { return min_val == -1; }

  /**
   * @brief 检查元素x是否在树中
   * @param x 要查找的元素
   * @return true如果元素存在
   */
  bool contains(int x) const {
    if (x < 0 || x >= universe_size) {
      return false;
    }

    if (x == min_val || x == max_val) {
      return true;
    }

    if (universe_size == 2) {
      return false;
    }

    return clusters[high(x)]->contains(low(x));
  }

  /**
   * @brief 获取树中的最小值
   * @return 最小值，如果树为空返回-1
   */
  int get_min() const { return min_val; }

  /**
   * @brief 获取树中的最大值
   * @return 最大值，如果树为空返回-1
   */
  int get_max() const { return max_val; }

  /**
   * @brief 获取x的后继（大于x的最小元素）
   * @param x 当前元素
   * @return 后继元素，如果不存在返回-1
   */
  int successor(int x) {
    if (x < 0 || x >= universe_size) {
      return -1;
    }

    // 基础情况：u=2
    if (universe_size == 2) {
      if (x == 0 && max_val == 1) {
        return 1;
      }
      return -1;
    }

    // 如果x小于最小值，则后继是min_val
    if (min_val != -1 && x < min_val) {
      return min_val;
    }

    // 在x所在的簇中查找后继
    int h = high(x);
    int l = low(x);
    int max_low = clusters[h]->get_max();

    if (max_low != -1 && l < max_low) {
      int offset = clusters[h]->successor(l);
      return index(h, offset);
    }

    // 在后续簇中查找
    int succ_cluster = summary->successor(h);
    if (succ_cluster == -1) {
      return -1;
    }

    int offset = clusters[succ_cluster]->get_min();
    return index(succ_cluster, offset);
  }

  /**
   * @brief 获取x的前驱（小于x的最大元素）
   * @param x 当前元素
   * @return 前驱元素，如果不存在返回-1
   */
  int predecessor(int x) {
    if (x < 0 || x >= universe_size) {
      return -1;
    }

    // 基础情况：u=2
    if (universe_size == 2) {
      if (x == 1 && min_val == 0) {
        return 0;
      }
      return -1;
    }

    // 如果x大于最大值，则前驱是max_val
    if (max_val != -1 && x > max_val) {
      return max_val;
    }

    // 在x所在的簇中查找前驱
    int h = high(x);
    int l = low(x);
    int min_low = clusters[h]->get_min();

    if (min_low != -1 && l > min_low) {
      int offset = clusters[h]->predecessor(l);
      return index(h, offset);
    }

    // 在前序簇中查找
    int pred_cluster = summary->predecessor(h);
    if (pred_cluster == -1) {
      // 检查min_val
      if (min_val != -1 && x > min_val) {
        return min_val;
      }
      return -1;
    }

    int offset = clusters[pred_cluster]->get_max();
    return index(pred_cluster, offset);
  }

  /**
   * @brief 插入元素x
   * @param x 要插入的元素
   */
  void insert(int x) {
    if (x < 0 || x >= universe_size) {
      throw std::out_of_range("插入元素超出全域范围");
    }

    if (contains(x)) {
      return; // 元素已存在
    }

    // 如果树为空，直接设置min和max
    if (is_empty()) {
      min_val = x;
      max_val = x;
      return;
    }

    // 更新最小值
    if (x < min_val) {
      std::swap(x, min_val);
    }

    // 基础情况：u=2
    if (universe_size == 2) {
      if (x != min_val) {
        max_val = x;
      }
      return;
    }

    // 递归插入
    int h = high(x);
    int l = low(x);

    if (clusters[h]->is_empty()) {
      summary->insert(h);
      clusters[h]->min_val = l;
      clusters[h]->max_val = l;
    } else {
      clusters[h]->insert(l);
    }

    // 更新最大值
    if (x > max_val) {
      max_val = x;
    }
  }

  /**
   * @brief 删除元素x
   * @param x 要删除的元素
   */
  void remove(int x) {
    if (x < 0 || x >= universe_size) {
      throw std::out_of_range("删除元素超出全域范围");
    }

    if (!contains(x)) {
      return; // 元素不存在
    }

    // 如果树中只有一个元素
    if (min_val == max_val) {
      min_val = -1;
      max_val = -1;
      return;
    }

    // 基础情况：u=2
    if (universe_size == 2) {
      if (x == 0) {
        min_val = 1;
      } else {
        min_val = 0;
      }
      max_val = min_val;
      return;
    }

    // 如果x是min_val，需要找到新的min_val
    if (x == min_val) {
      int first_cluster = summary->get_min();
      if (first_cluster == -1) {
        // 只有min_val一个元素
        min_val = max_val;
        return;
      }

      int new_min_low = clusters[first_cluster]->get_min();
      min_val = index(first_cluster, new_min_low);
      x = min_val; // 现在删除新的min_val
    }

    // 递归删除
    int h = high(x);
    int l = low(x);
    clusters[h]->remove(l);

    if (clusters[h]->is_empty()) {
      summary->remove(h);

      // 如果删除的是max_val，需要更新max_val
      if (x == max_val) {
        int summary_max = summary->get_max();
        if (summary_max == -1) {
          max_val = min_val;
        } else {
          int cluster_max = clusters[summary_max]->get_max();
          max_val = index(summary_max, cluster_max);
        }
      }
    } else if (x == max_val) {
      int cluster_max = clusters[h]->get_max();
      max_val = index(h, cluster_max);
    }
  }

  /**
   * @brief 获取树中元素数量
   * @return 元素数量
   */
  int size() const {
    if (is_empty()) {
      return 0;
    }

    if (universe_size == 2) {
      return (min_val == max_val) ? 1 : 2;
    }

    int count = 1; // min_val

    // 递归计算所有簇的大小
    for (int i = 0; i < (int)sqrt(universe_size); i++) {
      if (!clusters[i]->is_empty()) {
        count += clusters[i]->size();
      }
    }

    return count;
  }

  /**
   * @brief 打印树的结构
   * @param level 当前层级（用于缩进）
   */
  void print_tree(int level = 0) const {
    std::string indent(level * 2, ' ');

    std::cout << indent << "vEB(" << universe_size << "): ";
    if (is_empty()) {
      std::cout << "empty" << std::endl;
      return;
    }

    std::cout << "min=" << min_val << ", max=" << max_val << std::endl;

    if (universe_size > 2) {
      std::cout << indent << "Summary: " << std::endl;
      summary->print_tree(level + 1);

      std::cout << indent << "Clusters: " << std::endl;
      for (int i = 0; i < (int)sqrt(universe_size); i++) {
        if (!clusters[i]->is_empty()) {
          std::cout << indent << "  Cluster " << i << ": " << std::endl;
          clusters[i]->print_tree(level + 2);
        }
      }
    }
  }

  /**
   * @brief 获取所有元素
   * @return 包含所有元素的向量
   */
  std::vector<int> get_elements() const {
    std::vector<int> elements;

    if (is_empty()) {
      return elements;
    }

    // 添加min_val
    elements.push_back(min_val);

    if (universe_size == 2) {
      if (min_val != max_val) {
        elements.push_back(max_val);
      }
      return elements;
    }

    // 递归获取所有簇的元素
    for (int i = 0; i < (int)sqrt(universe_size); i++) {
      if (!clusters[i]->is_empty()) {
        auto cluster_elements = clusters[i]->get_elements();
        for (int elem : cluster_elements) {
          elements.push_back(index(i, elem));
        }
      }
    }

    return elements;
  }
};

/**
 * @brief 算法导论第20章经典示例和测试
 */
class VanEmdeBoasTreeDemo {
public:
  /**
   * @brief 演示基本的vEB树操作
   */
  static void demo_basic_operations() {
    std::cout << "=== 基本vEB树操作演示 ===" << std::endl;

    // 创建全域大小为16的vEB树
    VanEmdeBoasTree veb(16);

    std::cout << "初始状态:" << std::endl;
    veb.print_tree();

    // 插入一些元素
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

    // 测试包含查询
    std::cout << "\n包含查询:" << std::endl;
    std::cout << "包含2: " << (veb.contains(2) ? "是" : "否") << std::endl;
    std::cout << "包含6: " << (veb.contains(6) ? "是" : "否") << std::endl;
    std::cout << "包含15: " << (veb.contains(15) ? "是" : "否") << std::endl;

    // 测试前驱和后继
    std::cout << "\n前驱和后继查询:" << std::endl;
    std::cout << "6的后继: " << veb.successor(6) << std::endl;
    std::cout << "8的前驱: " << veb.predecessor(8) << std::endl;
    std::cout << "14的后继: " << veb.successor(14) << std::endl;

    // 获取所有元素
    auto elements = veb.get_elements();
    std::cout << "\n所有元素: ";
    for (int elem : elements) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;

    std::cout << "元素数量: " << veb.size() << std::endl;
  }

  /**
   * @brief 演示算法导论图20.3示例
   */
  static void demo_textbook_example() {
    std::cout << "\n=== 算法导论图20.3示例 ===" << std::endl;

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
    std::cout << "13的后继: " << veb.successor(13) << " (应该是14)"
              << std::endl;
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
   * @brief 演示删除操作
   */
  static void demo_delete_operations() {
    std::cout << "\n=== 删除操作演示 ===" << std::endl;

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

    // 删除一些元素
    std::cout << "\n删除元素: 5, 10" << std::endl;
    veb.remove(5);
    veb.remove(10);

    std::cout << "删除后状态:" << std::endl;
    veb.print_tree();

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
    std::cout << "11的后继: " << veb.successor(11) << " (应该是12)"
              << std::endl;
  }

  /**
   * @brief 演示不同全域大小的vEB树
   */
  static void demo_different_universe_sizes() {
    std::cout << "\n=== 不同全域大小演示 ===" << std::endl;

    // 测试全域大小为4
    std::cout << "全域大小4:" << std::endl;
    VanEmdeBoasTree veb4(4);
    veb4.insert(0);
    veb4.insert(3);
    veb4.print_tree();

    // 测试全域大小为8
    std::cout << "\n全域大小8:" << std::endl;
    VanEmdeBoasTree veb8(8);
    veb8.insert(1);
    veb8.insert(4);
    veb8.insert(6);
    veb8.print_tree();

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
  }

  /**
   * @brief 演示边界情况
   */
  static void demo_edge_cases() {
    std::cout << "\n=== 边界情况演示 ===" << std::endl;

    // 测试空树
    std::cout << "空树测试:" << std::endl;
    VanEmdeBoasTree veb(16);
    std::cout << "是否为空: " << (veb.is_empty() ? "是" : "否") << std::endl;
    std::cout << "最小值: " << veb.get_min() << std::endl;
    std::cout << "最大值: " << veb.get_max() << std::endl;

    // 测试单元素树
    std::cout << "\n单元素树测试:" << std::endl;
    veb.insert(8);
    std::cout << "是否为空: " << (veb.is_empty() ? "是" : "否") << std::endl;
    std::cout << "最小值: " << veb.get_min() << std::endl;
    std::cout << "最大值: " << veb.get_max() << std::endl;
    std::cout << "元素数量: " << veb.size() << std::endl;

    // 测试边界值
    std::cout << "\n边界值测试:" << std::endl;
    veb.insert(0);  // 最小值
    veb.insert(15); // 最大值
    std::cout << "包含0: " << (veb.contains(0) ? "是" : "否") << std::endl;
    std::cout << "包含15: " << (veb.contains(15) ? "是" : "否") << std::endl;
    std::cout << "-1的后继: " << veb.successor(-1) << " (应该是0)" << std::endl;
    std::cout << "16的前驱: " << veb.predecessor(16) << " (应该是15)"
              << std::endl;
  }

  /**
   * @brief 演示vEB树的时间复杂度优势
   */
  static void demo_performance_characteristics() {
    std::cout << "\n=== 时间复杂度特性演示 ===" << std::endl;

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
    std::cout << "完成1000次查找操作" << std::endl;

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
  }

  /**
   * @brief 运行所有演示
   */
  static void run_all_demos() {
    std::cout << "========== 第20章 van Emde Boas树 ==========" << std::endl;

    demo_basic_operations();
    demo_textbook_example();
    demo_delete_operations();
    demo_different_universe_sizes();
    demo_edge_cases();
    demo_performance_characteristics();

    std::cout << "\n========== 所有演示完成 ==========" << std::endl;
  }
};

} // namespace algorithms

#endif // VAN_EMDE_BOAS_TREE_H
