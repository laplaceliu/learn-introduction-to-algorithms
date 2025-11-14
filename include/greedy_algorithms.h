#ifndef GREEDY_ALGORITHMS_H
#define GREEDY_ALGORITHMS_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace algorithms {

/**
 * @brief 第16章 贪心算法实现
 *
 * 实现算法导论第16章的贪心算法：
 * - 16.1节 活动选择问题
 * - 16.2节 贪心算法原理
 * - 16.3节 赫夫曼编码
 * - 16.4节 拟阵和贪心算法
 * - 16.5节 用拟阵求解任务调度问题
 */

/**
 * @brief 活动结构体
 */
struct Activity {
  int id;     // 活动编号
  int start;  // 开始时间
  int finish; // 结束时间

  Activity(int i, int s, int f) : id(i), start(s), finish(f) {}

  // 用于排序的比较函数
  bool operator<(const Activity &other) const { return finish < other.finish; }
};

/**
 * @brief 活动选择问题 - 16.1节
 *
 * 实现活动选择问题的贪心算法，选择最大兼容活动子集
 */
class ActivitySelector {
private:
  std::vector<Activity> activities;

public:
  /**
   * @brief 添加活动
   * @param id 活动编号
   * @param start 开始时间
   * @param finish 结束时间
   */
  void add_activity(int id, int start, int finish) {
    if (start >= finish) {
      throw std::invalid_argument("开始时间必须小于结束时间");
    }
    activities.emplace_back(id, start, finish);
  }

  /**
   * @brief 贪心算法选择活动（递归版本）
   * @param k 当前考虑的活动索引
   * @param n 活动总数
   * @return 选择的活动ID列表
   */
  std::vector<int> recursive_activity_selector(int k, int n) {
    std::vector<int> selected;

    // 按结束时间排序
    std::sort(activities.begin(), activities.end());

    int m = k + 1;

    // 找到第一个开始时间大于等于当前活动结束时间的活动
    while (m <= n && activities[m - 1].start < activities[k - 1].finish) {
      m++;
    }

    if (m <= n) {
      selected.push_back(activities[m - 1].id);
      auto next = recursive_activity_selector(m, n);
      selected.insert(selected.end(), next.begin(), next.end());
    }

    return selected;
  }

  /**
   * @brief 贪心算法选择活动（迭代版本）
   * @return 选择的活动ID列表
   */
  std::vector<int> greedy_activity_selector() {
    if (activities.empty()) {
      return {};
    }

    // 按结束时间排序
    std::sort(activities.begin(), activities.end());

    std::vector<int> selected;
    selected.push_back(activities[0].id);

    int last_finish = activities[0].finish;

    for (size_t i = 1; i < activities.size(); ++i) {
      if (activities[i].start >= last_finish) {
        selected.push_back(activities[i].id);
        last_finish = activities[i].finish;
      }
    }

    return selected;
  }

  /**
   * @brief 获取所有活动
   * @return 活动列表
   */
  std::vector<Activity> get_activities() const { return activities; }

  /**
   * @brief 清空活动列表
   */
  void clear() { activities.clear(); }
};

/**
 * @brief 赫夫曼树节点
 */
struct HuffmanNode {
  char character;     // 字符（对于内部节点为'\0'）
  int frequency;      // 频率
  HuffmanNode *left;  // 左子节点
  HuffmanNode *right; // 右子节点

  HuffmanNode(char ch, int freq)
      : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

  HuffmanNode(int freq, HuffmanNode *l, HuffmanNode *r)
      : character('\0'), frequency(freq), left(l), right(r) {}

  // 用于优先队列的比较函数
  struct Compare {
    bool operator()(HuffmanNode *a, HuffmanNode *b) {
      return a->frequency > b->frequency;
    }
  };
};

/**
 * @brief 赫夫曼编码 - 16.3节
 *
 * 实现赫夫曼编码算法，构建最优前缀码
 */
class HuffmanCoding {
private:
  HuffmanNode *root;
  std::map<char, std::string> codes;

  /**
   * @brief 递归生成编码
   * @param node 当前节点
   * @param code 当前编码
   */
  void generate_codes(HuffmanNode *node, std::string code) {
    if (node == nullptr) {
      return;
    }

    // 如果是叶子节点，记录编码
    if (node->left == nullptr && node->right == nullptr) {
      codes[node->character] = code;
      return;
    }

    // 递归处理左右子树
    generate_codes(node->left, code + "0");
    generate_codes(node->right, code + "1");
  }

  /**
   * @brief 递归删除树节点
   * @param node 当前节点
   */
  void delete_tree(HuffmanNode *node) {
    if (node == nullptr) {
      return;
    }

    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
  }

public:
  HuffmanCoding() : root(nullptr) {}

  ~HuffmanCoding() { delete_tree(root); }

  /**
   * @brief 构建赫夫曼树
   * @param frequencies 字符频率映射
   */
  void build_huffman_tree(const std::map<char, int> &frequencies) {
    if (frequencies.empty()) {
      throw std::invalid_argument("频率映射不能为空");
    }

    // 使用优先队列（最小堆）
    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>,
                        HuffmanNode::Compare>
        pq;

    // 创建叶子节点并加入优先队列
    for (const auto &pair : frequencies) {
      pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // 构建赫夫曼树
    while (pq.size() > 1) {
      // 取出频率最小的两个节点
      HuffmanNode *left = pq.top();
      pq.pop();

      HuffmanNode *right = pq.top();
      pq.pop();

      // 创建新节点，频率为两个子节点频率之和
      HuffmanNode *parent =
          new HuffmanNode(left->frequency + right->frequency, left, right);
      pq.push(parent);
    }

    // 最后剩下的节点就是根节点
    root = pq.top();
    pq.pop();

    // 生成编码
    codes.clear();
    generate_codes(root, "");
  }

  /**
   * @brief 获取字符的赫夫曼编码
   * @param ch 字符
   * @return 对应的赫夫曼编码
   */
  std::string get_code(char ch) const {
    auto it = codes.find(ch);
    if (it != codes.end()) {
      return it->second;
    }
    throw std::invalid_argument("字符不在赫夫曼树中");
  }

  /**
   * @brief 获取所有编码
   * @return 字符到编码的映射
   */
  std::map<char, std::string> get_codes() const { return codes; }

  /**
   * @brief 编码字符串
   * @param text 原始文本
   * @return 编码后的二进制字符串
   */
  std::string encode(const std::string &text) const {
    std::string encoded;
    for (char ch : text) {
      encoded += get_code(ch);
    }
    return encoded;
  }

  /**
   * @brief 解码二进制字符串
   * @param encoded 编码后的二进制字符串
   * @return 解码后的原始文本
   */
  std::string decode(const std::string &encoded) const {
    if (root == nullptr) {
      throw std::runtime_error("赫夫曼树未构建");
    }

    std::string decoded;
    HuffmanNode *current = root;

    for (char bit : encoded) {
      if (bit == '0') {
        current = current->left;
      } else if (bit == '1') {
        current = current->right;
      } else {
        throw std::invalid_argument("编码字符串包含非法字符");
      }

      // 如果到达叶子节点
      if (current->left == nullptr && current->right == nullptr) {
        decoded += current->character;
        current = root; // 重置到根节点
      }
    }

    return decoded;
  }

  /**
   * @brief 计算压缩率
   * @param original 原始文本
   * @return 压缩率（编码后长度/原始长度）
   */
  double get_compression_ratio(const std::string &original) const {
    if (original.empty()) {
      return 0.0;
    }

    std::string encoded = encode(original);
    return static_cast<double>(encoded.length()) / (original.length() * 8);
  }

  /**
   * @brief 重置赫夫曼树
   */
  void reset() {
    delete_tree(root);
    root = nullptr;
    codes.clear();
  }
};

/**
 * @brief 拟阵定义 - 16.4节
 *
 * 拟阵是一个有序对 M = (S, I)，其中：
 * - S 是一个有限集
 * - I 是 S 的子集族，满足以下条件：
 *   1. 遗传性：如果 B ∈ I 且 A ⊆ B，则 A ∈ I
 *   2. 交换性：如果 A, B ∈ I 且 |A| < |B|，则存在 x ∈ B - A 使得 A ∪ {x} ∈ I
 */
template <typename T> class Matroid {
private:
  std::set<T> ground_set;
  std::function<bool(const std::set<T> &)> independence_oracle;

public:
  /**
   * @brief 构造函数
   * @param set 拟阵的基集
   * @param oracle 独立性判断函数
   */
  Matroid(const std::set<T> &set,
          std::function<bool(const std::set<T> &)> oracle)
      : ground_set(set), independence_oracle(oracle) {}

  /**
   * @brief 判断子集是否独立
   * @param subset 要判断的子集
   * @return 如果子集独立返回true，否则返回false
   */
  bool is_independent(const std::set<T> &subset) const {
    return independence_oracle(subset);
  }

  /**
   * @brief 获取基集
   * @return 拟阵的基集
   */
  std::set<T> get_ground_set() const { return ground_set; }

  /**
   * @brief 贪心算法求解拟阵上的最大权重独立集
   * @param weight_function 权重函数
   * @return 最大权重独立集
   */
  std::set<T> greedy_max_weight_independent_set(
      std::function<double(const T &)> weight_function) const {
    // 按权重降序排序基集中的元素
    std::vector<T> sorted_elements(ground_set.begin(), ground_set.end());
    std::sort(sorted_elements.begin(), sorted_elements.end(),
              [&](const T &a, const T &b) {
                return weight_function(a) > weight_function(b);
              });

    std::set<T> result;

    // 贪心选择：按权重从高到低，如果加入后仍然独立，则加入
    for (const T &element : sorted_elements) {
      std::set<T> candidate = result;
      candidate.insert(element);

      if (is_independent(candidate)) {
        result = candidate;
      }
    }

    return result;
  }
};

/**
 * @brief 图形拟阵（Graphic Matroid）
 *
 * 图形拟阵的基集是图的边集，独立集是无环边集（森林）
 */
class GraphicMatroid {
private:
  int num_vertices;
  std::set<std::pair<int, int>> edges;

  /**
   * @brief 判断边集是否形成环
   * @param edge_set 边集
   * @return 如果无环返回true，否则返回false
   */
  bool is_acyclic(const std::set<std::pair<int, int>> &edge_set) const {
    // 使用并查集检测环
    std::vector<int> parent(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
      parent[i] = i;
    }

    auto find = [&](int x) {
      while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
      }
      return x;
    };

    for (const auto &edge : edge_set) {
      int u = edge.first, v = edge.second;
      int rootU = find(u), rootV = find(v);

      if (rootU == rootV) {
        return false; // 形成环
      }

      parent[rootU] = rootV; // 合并
    }

    return true;
  }

public:
  /**
   * @brief 构造函数
   * @param vertices 顶点数量
   * @param edge_set 边集
   */
  GraphicMatroid(int vertices, const std::set<std::pair<int, int>> &edge_set)
      : num_vertices(vertices), edges(edge_set) {}

  /**
   * @brief 判断边子集是否独立（无环）
   * @param subset 边子集
   * @return 如果无环返回true，否则返回false
   */
  bool is_independent(const std::set<std::pair<int, int>> &subset) const {
    return is_acyclic(subset);
  }

  /**
   * @brief 获取边集
   * @return 图的边集
   */
  std::set<std::pair<int, int>> get_edges() const { return edges; }

  /**
   * @brief 获取顶点数量
   * @return 顶点数量
   */
  int get_num_vertices() const { return num_vertices; }
};

/**
 * @brief 任务调度问题 - 16.5节
 *
 * 单机任务调度问题：每个任务有截止时间和惩罚，
 * 目标是安排任务顺序使得总惩罚最小
 */
class TaskScheduling {
public:
  /**
   * @brief 任务结构体
   */
  struct Task {
    int id;             // 任务编号
    int deadline;       // 截止时间
    int penalty;        // 惩罚值
    int processing_time; // 处理时间（默认为1）

    Task(int i, int d, int p, int pt = 1)
        : id(i), deadline(d), penalty(p), processing_time(pt) {}

    // 用于排序的比较函数（按惩罚降序）
    bool operator<(const Task &other) const { return penalty > other.penalty; }
  };

  /**
   * @brief 贪心算法求解任务调度问题
   * @param tasks 任务列表
   * @return 调度顺序和总惩罚
   */
  static std::pair<std::vector<Task>, int>
  greedy_task_scheduling(std::vector<Task> tasks) {
    // 按惩罚值降序排序
    std::sort(tasks.begin(), tasks.end());

    int n = tasks.size();
    std::vector<bool> time_slot(n + 1, false); // 时间槽占用情况
    std::vector<Task> schedule;
    int total_penalty = 0;

    // 为每个任务分配时间槽
    for (const Task &task : tasks) {
      // 找到最晚的可用时间槽（不超过截止时间）
      int slot = std::min(task.deadline, n);
      while (slot > 0 && time_slot[slot]) {
        slot--;
      }

      if (slot > 0) {
        // 成功安排任务
        time_slot[slot] = true;
        schedule.push_back(task);
      } else {
        // 无法安排，计入惩罚
        total_penalty += task.penalty;
      }
    }

    // 按时间槽顺序重新排列已安排的任务
    std::vector<Task> final_schedule;
    for (int i = 1; i <= n; ++i) {
      if (time_slot[i]) {
        for (const Task &task : schedule) {
          if (task.deadline >= i) {
            final_schedule.push_back(task);
            break;
          }
        }
      }
    }

    return {final_schedule, total_penalty};
  }

  /**
   * @brief 验证调度是否可行
   * @param schedule 调度顺序
   * @return 如果可行返回true，否则返回false
   */
  static bool validate_schedule(const std::vector<Task> &schedule) {
    int current_time = 0;
    for (const Task &task : schedule) {
      current_time += task.processing_time;
      if (current_time > task.deadline) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief 计算拟阵在任务调度问题中的应用
   * @return 拟阵理论的说明
   */
  static std::string explain_matroid_application() {
    return "任务调度问题可以建模为拟阵上的优化问题："
           "- 基集：所有任务"
           "- 独立集：可以在截止时间前完成的任务子集"
           "- 权重：任务的惩罚值（负权重）"
           "- 贪心算法：按惩罚值降序选择任务，保证最优解";
  }
};

/**
 * @brief 贪心算法原理演示 - 16.2节
 *
 * 演示贪心选择性质和最优子结构性质
 */
class GreedyPrinciples {
public:
  /**
   * @brief 分数背包问题（贪心算法）
   * @param values 物品价值数组
   * @param weights 物品重量数组
   * @param capacity 背包容量
   * @return 最大总价值
   */
  static double fractional_knapsack(const std::vector<double> &values,
                                   const std::vector<double> &weights,
                                   double capacity) {
    if (values.size() != weights.size()) {
      throw std::invalid_argument("价值和重量数组大小必须相同");
    }

    int n = values.size();
    std::vector<std::pair<double, int>> value_per_weight;

    // 计算单位价值
    for (int i = 0; i < n; ++i) {
      if (weights[i] <= 0) {
        throw std::invalid_argument("物品重量必须大于0");
      }
      value_per_weight.emplace_back(values[i] / weights[i], i);
    }

    // 按单位价值降序排序
    std::sort(value_per_weight.begin(), value_per_weight.end(),
              [](const auto &a, const auto &b) { return a.first > b.first; });

    double total_value = 0.0;
    double remaining_capacity = capacity;

    // 贪心选择：优先选择单位价值高的物品
    for (const auto &item : value_per_weight) {
      int idx = item.second;

      if (remaining_capacity >= weights[idx]) {
        // 可以完整放入
        total_value += values[idx];
        remaining_capacity -= weights[idx];
      } else {
        // 放入部分物品
        total_value += values[idx] * (remaining_capacity / weights[idx]);
        break;
      }
    }

    return total_value;
  }

  /**
   * @brief 验证贪心选择性质
   * @return 贪心选择性质的说明
   */
  static std::string explain_greedy_choice_property() {
    return "贪心选择性质：我们可以通过做出局部最优选择（贪心选择）来构造全局最"
           "优解。"
           "这意味着我们不需要考虑子问题的解，可以直接做出当前最优选择。";
  }

  /**
   * @brief 验证最优子结构性质
   * @return 最优子结构性质的说明
   */
  static std::string explain_optimal_substructure() {
    return "最优子结构性质：问题的最优解包含其子问题的最优解。"
           "这意味着我们可以通过组合子问题的最优解来构造原问题的最优解。";
  }
};

} // namespace algorithms

#endif // GREEDY_ALGORITHMS_H
