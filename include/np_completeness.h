#ifndef NP_COMPLETENESS_H
#define NP_COMPLETENESS_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace algorithms {

/**
 * @brief 图结构（用于NP问题演示）
 */
class Graph {
private:
  int vertex_count;
  std::vector<std::vector<int>> adjacency_list;

public:
  Graph(int n) : vertex_count(n), adjacency_list(n) {}

  void add_edge(int u, int v) {
    if (u < 0 || u >= vertex_count || v < 0 || v >= vertex_count) {
      throw std::out_of_range("顶点索引超出范围");
    }
    adjacency_list[u].push_back(v);
    adjacency_list[v].push_back(u);
  }

  const std::vector<int> &get_neighbors(int v) const {
    return adjacency_list[v];
  }

  int get_vertex_count() const { return vertex_count; }

  void print() const {
    std::cout << "图结构:" << std::endl;
    for (int i = 0; i < vertex_count; i++) {
      std::cout << "顶点 " << i << " 的邻居: ";
      for (int neighbor : adjacency_list[i]) {
        std::cout << neighbor << " ";
      }
      std::cout << std::endl;
    }
  }
};

/**
 * @brief NP完全性相关算法和问题演示
 */
class NPCompleteness {
public:
  /**
   * @brief 3-SAT问题验证器
   * @param formula 3-CNF公式，每个子句包含3个文字
   * @param assignment 变量赋值
   * @return 是否满足公式
   */
  static bool verify_3sat(const std::vector<std::vector<int>> &formula,
                          const std::vector<bool> &assignment) {
    for (const auto &clause : formula) {
      bool clause_satisfied = false;
      for (int literal : clause) {
        int var = std::abs(literal) - 1; // 变量索引从0开始
        bool value = (literal > 0) ? assignment[var] : !assignment[var];
        if (value) {
          clause_satisfied = true;
          break;
        }
      }
      if (!clause_satisfied) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief 顶点覆盖问题验证器
   * @param graph 图
   * @param cover 顶点覆盖集合
   * @param k 覆盖大小
   * @return 是否是大小为k的顶点覆盖
   */
  static bool verify_vertex_cover(const Graph &graph,
                                  const std::unordered_set<int> &cover, int k) {
    if (cover.size() != k) {
      return false;
    }

    // 检查每条边是否至少有一个端点在覆盖中
    for (int u = 0; u < graph.get_vertex_count(); u++) {
      for (int v : graph.get_neighbors(u)) {
        if (u < v) { // 避免重复检查
          if (cover.find(u) == cover.end() && cover.find(v) == cover.end()) {
            return false;
          }
        }
      }
    }
    return true;
  }

  /**
   * @brief 团问题验证器
   * @param graph 图
   * @param clique 团集合
   * @param k 团大小
   * @return 是否是大小为k的团
   */
  static bool verify_clique(const Graph &graph,
                            const std::unordered_set<int> &clique, int k) {
    if (clique.size() != k) {
      return false;
    }

    std::vector<int> clique_vec(clique.begin(), clique.end());

    // 检查团中每对顶点是否都有边连接
    for (int i = 0; i < clique_vec.size(); i++) {
      for (int j = i + 1; j < clique_vec.size(); j++) {
        int u = clique_vec[i];
        int v = clique_vec[j];

        const auto &neighbors = graph.get_neighbors(u);
        if (std::find(neighbors.begin(), neighbors.end(), v) ==
            neighbors.end()) {
          return false;
        }
      }
    }
    return true;
  }

  /**
   * @brief 哈密顿回路问题验证器
   * @param graph 图
   * @param path 路径
   * @return 是否是哈密顿回路
   */
  static bool verify_hamiltonian_cycle(const Graph &graph,
                                       const std::vector<int> &path) {
    int n = graph.get_vertex_count();

    // 检查路径长度
    if (path.size() != n + 1 || path[0] != path[n]) {
      return false;
    }

    // 检查是否包含所有顶点
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; i++) {
      if (path[i] < 0 || path[i] >= n) {
        return false;
      }
      visited[path[i]] = true;
    }

    for (bool v : visited) {
      if (!v) {
        return false;
      }
    }

    // 检查路径是否连续
    for (int i = 0; i < n; i++) {
      int u = path[i];
      int v = path[i + 1];

      const auto &neighbors = graph.get_neighbors(u);
      if (std::find(neighbors.begin(), neighbors.end(), v) == neighbors.end()) {
        return false;
      }
    }

    return true;
  }

  /**
   * @brief 旅行商问题验证器
   * @param graph 完全图（邻接矩阵表示权重）
   * @param tour 旅行路线
   * @param k 最大成本
   * @return 路线成本是否不超过k
   */
  static bool verify_tsp(const std::vector<std::vector<int>> &graph,
                         const std::vector<int> &tour, int k) {
    int n = graph.size();

    // 检查路径长度
    if (tour.size() != n + 1 || tour[0] != tour[n]) {
      return false;
    }

    // 检查是否包含所有顶点
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; i++) {
      if (tour[i] < 0 || tour[i] >= n) {
        return false;
      }
      visited[tour[i]] = true;
    }

    for (bool v : visited) {
      if (!v) {
        return false;
      }
    }

    // 计算总成本
    int total_cost = 0;
    for (int i = 0; i < n; i++) {
      int u = tour[i];
      int v = tour[i + 1];
      total_cost += graph[u][v];
    }

    return total_cost <= k;
  }

  /**
   * @brief 子集和问题验证器
   * @param set 整数集合
   * @param subset 子集
   * @param target 目标和
   * @return 子集和是否等于目标
   */
  static bool verify_subset_sum(const std::vector<int> &set,
                                const std::unordered_set<int> &subset,
                                int target) {
    int sum = 0;

    // 检查子集是否来自原集合
    for (int elem : subset) {
      if (std::find(set.begin(), set.end(), elem) == set.end()) {
        return false;
      }
      sum += elem;
    }

    return sum == target;
  }

  /**
   * @brief 演示P类问题（多项式时间可解）
   */
  static void demonstrate_p_class() {
    std::cout << "=== P类问题演示（多项式时间可解） ===" << std::endl;

    // 排序问题（P类问题）
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3};
    std::cout << "排序前: ";
    for (int num : numbers) {
      std::cout << num << " ";
    }
    std::cout << std::endl;

    std::sort(numbers.begin(), numbers.end());

    std::cout << "排序后: ";
    for (int num : numbers) {
      std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "排序问题属于P类，时间复杂度O(n log n)" << std::endl;
  }

  /**
   * @brief 演示NP类问题（多项式时间可验证）
   */
  static void demonstrate_np_class() {
    std::cout << "\n=== NP类问题演示（多项式时间可验证） ===" << std::endl;

    // 3-SAT问题
    std::vector<std::vector<int>> formula = {
        {1, 2, -3}, // (x1 ∨ x2 ∨ ¬x3)
        {-1, 2, 3}, // (¬x1 ∨ x2 ∨ x3)
        {1, -2, 3}  // (x1 ∨ ¬x2 ∨ x3)
    };

    std::vector<bool> assignment = {true, true,
                                    true}; // x1=true, x2=true, x3=true

    std::cout << "3-SAT公式:" << std::endl;
    for (const auto &clause : formula) {
      std::cout << "(";
      for (int literal : clause) {
        if (literal > 0) {
          std::cout << "x" << literal;
        } else {
          std::cout << "¬x" << -literal;
        }
        if (&literal != &clause.back()) {
          std::cout << " ∨ ";
        }
      }
      std::cout << ")" << std::endl;
    }

    std::cout << "赋值: x1=" << assignment[0] << ", x2=" << assignment[1]
              << ", x3=" << assignment[2] << std::endl;

    bool satisfied = verify_3sat(formula, assignment);
    std::cout << "公式是否满足: " << (satisfied ? "是" : "否") << std::endl;

    std::cout << "3-SAT属于NP类，验证解只需多项式时间" << std::endl;
  }

  /**
   * @brief 演示NP完全问题
   */
  static void demonstrate_np_complete() {
    std::cout << "\n=== NP完全问题演示 ===" << std::endl;

    // 顶点覆盖问题
    Graph graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 4);
    graph.add_edge(3, 4);

    graph.print();

    std::unordered_set<int> cover = {1, 2}; // 顶点1和2构成覆盖
    int k = 2;

    std::cout << "顶点覆盖候选: {1, 2}" << std::endl;
    bool is_cover = verify_vertex_cover(graph, cover, k);
    std::cout << "是否是大小为" << k
              << "的顶点覆盖: " << (is_cover ? "是" : "否") << std::endl;

    std::cout << "顶点覆盖问题是NP完全的" << std::endl;
  }

  /**
   * @brief 演示问题归约
   */
  static void demonstrate_reduction() {
    std::cout << "\n=== 问题归约演示 ===" << std::endl;

    std::cout << "NP完全问题之间的关系:" << std::endl;
    std::cout << "3-SAT ≤p 顶点覆盖 ≤p 团问题 ≤p 哈密顿回路 ≤p 旅行商问题"
              << std::endl;

    std::cout << "\n归约的意义:" << std::endl;
    std::cout
        << "如果问题A可以多项式时间归约到问题B，且A是NP完全的，则B也是NP完全的"
        << std::endl;
    std::cout << "这意味着如果我们能在多项式时间内解决B，就能在多项式时间内解决"
                 "所有NP问题"
              << std::endl;
  }

  /**
   * @brief 演示P vs NP问题
   */
  static void demonstrate_p_vs_np() {
    std::cout << "\n=== P vs NP 问题演示 ===" << std::endl;

    std::cout << "P类问题: 多项式时间可解的问题" << std::endl;
    std::cout << "NP类问题: 多项式时间可验证的问题" << std::endl;
    std::cout << "NP完全问题: NP中最难的问题" << std::endl;

    std::cout << "\n重要结论:" << std::endl;
    std::cout << "1. P ⊆ NP (所有P类问题都是NP类问题)" << std::endl;
    std::cout << "2. 如果任何一个NP完全问题属于P，则P = NP" << std::endl;
    std::cout << "3. P = NP 是计算机科学中最重要的未解决问题之一" << std::endl;

    std::cout << "\n现实意义:" << std::endl;
    std::cout
        << "如果P = "
           "NP，许多现在认为困难的问题（如密码破解、蛋白质折叠）将变得容易解决"
        << std::endl;
    std::cout << "但大多数专家认为P ≠ NP" << std::endl;
  }
};

} // namespace algorithms

#endif // NP_COMPLETENESS_H
