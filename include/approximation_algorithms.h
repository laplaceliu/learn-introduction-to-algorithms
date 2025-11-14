#ifndef APPROXIMATION_ALGORITHMS_H
#define APPROXIMATION_ALGORITHMS_H

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_set>
#include <vector>

namespace algorithms {

/**
 * @brief 图结构（用于近似算法演示）
 */
class Graph {
private:
  int vertex_count;
  std::vector<std::vector<int>> adjacency_list;
  std::vector<std::vector<int>> weight_matrix;

public:
  Graph(int n)
      : vertex_count(n), adjacency_list(n),
        weight_matrix(n, std::vector<int>(n, 0)) {}

  void add_edge(int u, int v, int weight = 1) {
    if (u < 0 || u >= vertex_count || v < 0 || v >= vertex_count) {
      throw std::out_of_range("顶点索引超出范围");
    }
    adjacency_list[u].push_back(v);
    adjacency_list[v].push_back(u);
    weight_matrix[u][v] = weight;
    weight_matrix[v][u] = weight;
  }

  const std::vector<int> &get_neighbors(int v) const {
    return adjacency_list[v];
  }

  int get_weight(int u, int v) const { return weight_matrix[u][v]; }

  int get_vertex_count() const { return vertex_count; }

  void print() const {
    std::cout << "图结构:" << std::endl;
    for (int i = 0; i < vertex_count; i++) {
      std::cout << "顶点 " << i << " 的邻居: ";
      for (int neighbor : adjacency_list[i]) {
        std::cout << neighbor << "(" << weight_matrix[i][neighbor] << ") ";
      }
      std::cout << std::endl;
    }
  }
};

/**
 * @brief 近似算法相关实现
 */
class ApproximationAlgorithms {
public:
  /**
   * @brief 顶点覆盖问题的2-近似算法
   * @param graph 图
   * @return 近似顶点覆盖
   */
  static std::unordered_set<int> vertex_cover_2_approx(const Graph &graph) {
    std::unordered_set<int> cover;
    std::vector<bool> visited(graph.get_vertex_count(), false);

    // 遍历所有边
    for (int u = 0; u < graph.get_vertex_count(); u++) {
      if (visited[u])
        continue;

      for (int v : graph.get_neighbors(u)) {
        if (!visited[v]) {
          // 将u和v加入覆盖
          cover.insert(u);
          cover.insert(v);
          visited[u] = true;
          visited[v] = true;
          break;
        }
      }
    }

    return cover;
  }

  /**
   * @brief 旅行商问题的2-近似算法（基于MST）
   * @param graph 完全图（邻接矩阵）
   * @return 近似旅行路线
   */
  static std::vector<int>
  tsp_2_approx(const std::vector<std::vector<int>> &graph) {
    int n = graph.size();

    // 1. 构建最小生成树（使用Prim算法）
    auto mst = prim_mst(graph);

    // 2. 对MST进行前序遍历得到哈密顿回路
    std::vector<int> tour;
    std::vector<bool> visited(n, false);
    preorder_traversal(mst, 0, visited, tour);

    // 3. 添加起点形成回路
    tour.push_back(0);

    return tour;
  }

  /**
   * @brief 集合覆盖问题的近似算法（贪心算法）
   * @param universe 全集
   * @param subsets 子集集合
   * @return 近似集合覆盖
   */
  static std::vector<std::unordered_set<int>>
  set_cover_greedy(const std::unordered_set<int> &universe,
                   const std::vector<std::unordered_set<int>> &subsets) {

    std::unordered_set<int> uncovered = universe;
    std::vector<std::unordered_set<int>> cover;
    std::vector<bool> used(subsets.size(), false);

    while (!uncovered.empty()) {
      // 选择覆盖最多未覆盖元素的子集
      int best_index = -1;
      int max_covered = 0;

      for (int i = 0; i < subsets.size(); i++) {
        if (used[i])
          continue;

        int covered_count = 0;
        for (int elem : subsets[i]) {
          if (uncovered.find(elem) != uncovered.end()) {
            covered_count++;
          }
        }

        if (covered_count > max_covered) {
          max_covered = covered_count;
          best_index = i;
        }
      }

      if (best_index == -1)
        break;

      // 添加最佳子集到覆盖
      cover.push_back(subsets[best_index]);
      used[best_index] = true;

      // 移除已覆盖的元素
      for (int elem : subsets[best_index]) {
        uncovered.erase(elem);
      }
    }

    return cover;
  }

  /**
   * @brief 背包问题的近似算法（贪心算法）
   * @param weights 物品重量
   * @param values 物品价值
   * @param capacity 背包容量
   * @return 选择的物品索引
   */
  static std::vector<int> knapsack_greedy(const std::vector<int> &weights,
                                          const std::vector<int> &values,
                                          int capacity) {
    int n = weights.size();

    // 计算价值密度
    std::vector<std::pair<double, int>> density;
    for (int i = 0; i < n; i++) {
      density.push_back({static_cast<double>(values[i]) / weights[i], i});
    }

    // 按价值密度降序排序
    std::sort(density.begin(), density.end(),
              [](const auto &a, const auto &b) { return a.first > b.first; });

    std::vector<int> selected;
    int current_weight = 0;

    // 贪心选择
    for (const auto &[d, index] : density) {
      if (current_weight + weights[index] <= capacity) {
        selected.push_back(index);
        current_weight += weights[index];
      }
    }

    return selected;
  }

  /**
   * @brief 最大割问题的2-近似算法
   * @param graph 图
   * @return 割的两个集合
   */
  static std::pair<std::unordered_set<int>, std::unordered_set<int>>
  max_cut_2_approx(const Graph &graph) {
    std::unordered_set<int> set_a, set_b;

    // 随机初始化
    for (int i = 0; i < graph.get_vertex_count(); i++) {
      if (i % 2 == 0) {
        set_a.insert(i);
      } else {
        set_b.insert(i);
      }
    }

    // 局部改进
    bool improved = true;
    while (improved) {
      improved = false;

      for (int v = 0; v < graph.get_vertex_count(); v++) {
        int cut_gain = calculate_cut_gain(graph, v, set_a, set_b);

        if (cut_gain > 0) {
          if (set_a.find(v) != set_a.end()) {
            set_a.erase(v);
            set_b.insert(v);
          } else {
            set_b.erase(v);
            set_a.insert(v);
          }
          improved = true;
        }
      }
    }

    return {set_a, set_b};
  }

  /**
   * @brief 设施选址问题的近似算法
   * @param facilities 设施位置和开设成本
   * @param clients 客户位置
   * @param distances 距离矩阵
   * @return 选择的设施和客户分配
   */
  static std::pair<std::vector<int>, std::vector<int>>
  facility_location_approx(const std::vector<int> &facility_costs,
                           const std::vector<std::vector<int>> &distances) {
    int n_facilities = facility_costs.size();
    int n_clients = distances[0].size();

    std::vector<int> selected_facilities;
    std::vector<int> assignments(n_clients, -1);

    // 贪心算法：每次选择性价比最高的设施
    std::vector<bool> facility_used(n_facilities, false);
    std::vector<bool> client_covered(n_clients, false);

    while (std::find(client_covered.begin(), client_covered.end(), false) !=
           client_covered.end()) {
      double best_ratio = std::numeric_limits<double>::max();
      int best_facility = -1;

      for (int f = 0; f < n_facilities; f++) {
        if (facility_used[f])
          continue;

        // 计算该设施能覆盖的未覆盖客户的总距离节省
        int total_saving = 0;
        int covered_count = 0;

        for (int c = 0; c < n_clients; c++) {
          if (!client_covered[c]) {
            total_saving += distances[f][c];
            covered_count++;
          }
        }

        if (covered_count > 0) {
          double ratio = static_cast<double>(facility_costs[f]) / total_saving;
          if (ratio < best_ratio) {
            best_ratio = ratio;
            best_facility = f;
          }
        }
      }

      if (best_facility == -1)
        break;

      // 选择最佳设施
      selected_facilities.push_back(best_facility);
      facility_used[best_facility] = true;

      // 分配客户
      for (int c = 0; c < n_clients; c++) {
        if (!client_covered[c]) {
          assignments[c] = best_facility;
          client_covered[c] = true;
        }
      }
    }

    return {selected_facilities, assignments};
  }

  /**
   * @brief 计算近似比
   */
  static void demonstrate_approximation_ratios() {
    std::cout << "=== 近似比演示 ===" << std::endl;

    std::cout << "常见近似算法的近似比:" << std::endl;
    std::cout << "1. 顶点覆盖问题: 2-近似算法" << std::endl;
    std::cout << "2. 旅行商问题（满足三角不等式）: 2-近似算法" << std::endl;
    std::cout << "3. 集合覆盖问题: O(log n)-近似算法" << std::endl;
    std::cout << "4. 最大割问题: 2-近似算法" << std::endl;
    std::cout << "5. 背包问题: 任意接近最优解的近似方案" << std::endl;

    std::cout << "\n近似比的意义:" << std::endl;
    std::cout << "- 近似比C表示算法解的值不超过最优解的C倍" << std::endl;
    std::cout << "- 对于最小化问题，C ≥ 1" << std::endl;
    std::cout << "- 对于最大化问题，C ≤ 1" << std::endl;
  }

private:
  /**
   * @brief Prim算法构建最小生成树
   */
  static std::vector<std::vector<int>>
  prim_mst(const std::vector<std::vector<int>> &graph) {
    int n = graph.size();
    std::vector<std::vector<int>> mst(n, std::vector<int>(n, 0));
    std::vector<int> key(n, std::numeric_limits<int>::max());
    std::vector<bool> in_mst(n, false);
    std::vector<int> parent(n, -1);

    key[0] = 0;

    for (int count = 0; count < n - 1; count++) {
      int u = -1;
      for (int v = 0; v < n; v++) {
        if (!in_mst[v] && (u == -1 || key[v] < key[u])) {
          u = v;
        }
      }

      in_mst[u] = true;

      if (parent[u] != -1) {
        mst[parent[u]][u] = graph[parent[u]][u];
        mst[u][parent[u]] = graph[u][parent[u]];
      }

      for (int v = 0; v < n; v++) {
        if (graph[u][v] > 0 && !in_mst[v] && graph[u][v] < key[v]) {
          parent[v] = u;
          key[v] = graph[u][v];
        }
      }
    }

    return mst;
  }

  /**
   * @brief 前序遍历
   */
  static void preorder_traversal(const std::vector<std::vector<int>> &mst,
                                 int node, std::vector<bool> &visited,
                                 std::vector<int> &tour) {
    visited[node] = true;
    tour.push_back(node);

    for (int i = 0; i < mst.size(); i++) {
      if (mst[node][i] > 0 && !visited[i]) {
        preorder_traversal(mst, i, visited, tour);
      }
    }
  }

  /**
   * @brief 计算割的增益
   */
  static int calculate_cut_gain(const Graph &graph, int v,
                                const std::unordered_set<int> &set_a,
                                const std::unordered_set<int> &set_b) {
    int gain = 0;

    // 计算移动到另一个集合的增益
    if (set_a.find(v) != set_a.end()) {
      // 当前在A集合，计算移动到B的增益
      for (int neighbor : graph.get_neighbors(v)) {
        if (set_a.find(neighbor) != set_a.end()) {
          gain += graph.get_weight(v, neighbor); // 增加割值
        } else {
          gain -= graph.get_weight(v, neighbor); // 减少割值
        }
      }
    } else {
      // 当前在B集合，计算移动到A的增益
      for (int neighbor : graph.get_neighbors(v)) {
        if (set_b.find(neighbor) != set_b.end()) {
          gain += graph.get_weight(v, neighbor); // 增加割值
        } else {
          gain -= graph.get_weight(v, neighbor); // 减少割值
        }
      }
    }

    return gain;
  }
};

} // namespace algorithms

#endif // APPROXIMATION_ALGORITHMS_H
