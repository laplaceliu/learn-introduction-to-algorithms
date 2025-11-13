#ifndef ALL_PAIRS_SHORTEST_PATH_H
#define ALL_PAIRS_SHORTEST_PATH_H

#include "graph_representation.h"
#include "shortest_path.h"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

namespace algorithms {

// 所有节点对最短路径结果结构
struct AllPairsShortestPathResult {
  std::vector<std::vector<int>> distances;    // 距离矩阵
  std::vector<std::vector<int>> predecessors; // 前驱矩阵
  bool has_negative_cycle;                    // 是否存在负权环

  AllPairsShortestPathResult(int n)
      : distances(n, std::vector<int>(n, std::numeric_limits<int>::max())),
        predecessors(n, std::vector<int>(n, -1)), has_negative_cycle(false) {}

  // 打印距离矩阵
  void print_distances() const {
    std::cout << "距离矩阵:" << std::endl;
    int n = distances.size();

    // 打印列标题
    std::cout << "    ";
    for (int i = 0; i < n; i++) {
      std::cout << std::setw(4) << i;
    }
    std::cout << std::endl;

    // 打印矩阵
    for (int i = 0; i < n; i++) {
      std::cout << std::setw(3) << i << " ";
      for (int j = 0; j < n; j++) {
        if (distances[i][j] == std::numeric_limits<int>::max()) {
          std::cout << std::setw(4) << "INF";
        } else {
          std::cout << std::setw(4) << distances[i][j];
        }
      }
      std::cout << std::endl;
    }
  }

  // 打印从i到j的最短路径
  void print_path(int i, int j) const {
    if (distances[i][j] == std::numeric_limits<int>::max()) {
      std::cout << "节点 " << i << " 到节点 " << j << " 不可达" << std::endl;
      return;
    }

    std::cout << "节点 " << i << " 到节点 " << j << " 的最短路径: ";

    std::vector<int> path;
    int current = j;

    // 反向追踪路径
    while (current != -1 && current != i) {
      path.push_back(current);
      current = predecessors[i][current];
    }

    if (current == -1) {
      std::cout << "路径不存在" << std::endl;
      return;
    }

    path.push_back(i);
    std::reverse(path.begin(), path.end());

    for (size_t k = 0; k < path.size(); k++) {
      std::cout << path[k];
      if (k < path.size() - 1) {
        std::cout << " -> ";
      }
    }
    std::cout << " (距离: " << distances[i][j] << ")" << std::endl;
  }
};

// 25.1 最短路径和矩阵乘法（动态规划方法）
class MatrixMultiplicationShortestPath {
public:
  static AllPairsShortestPathResult
  find_all_pairs_shortest_path(const AdjacencyListGraph &graph) {
    int n = graph.get_node_count();
    AllPairsShortestPathResult result(n);

    if (n == 0) {
      return result;
    }

    // 初始化距离矩阵
    auto edges = graph.get_all_edges();

    // 设置直接边的距离
    for (int i = 0; i < n; i++) {
      result.distances[i][i] = 0;
    }

    for (const auto &edge : edges) {
      result.distances[edge.from][edge.to] = edge.weight;
      result.predecessors[edge.from][edge.to] = edge.from;
    }

    // 矩阵乘法方法（动态规划）
    for (int m = 2; m <= n; m++) {
      auto new_distances = result.distances;

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          for (int k = 0; k < n; k++) {
            if (result.distances[i][k] != std::numeric_limits<int>::max() &&
                result.distances[k][j] != std::numeric_limits<int>::max()) {
              int new_dist = result.distances[i][k] + result.distances[k][j];
              if (new_dist < new_distances[i][j]) {
                new_distances[i][j] = new_dist;
                result.predecessors[i][j] = result.predecessors[k][j];
              }
            }
          }
        }
      }

      result.distances = new_distances;
    }

    // 检查负权环
    for (int i = 0; i < n; i++) {
      if (result.distances[i][i] < 0) {
        result.has_negative_cycle = true;
        break;
      }
    }

    return result;
  }
};

// 25.2 Floyd-Warshall算法
class FloydWarshall {
public:
  static AllPairsShortestPathResult
  find_all_pairs_shortest_path(const AdjacencyListGraph &graph) {
    int n = graph.get_node_count();
    AllPairsShortestPathResult result(n);

    if (n == 0) {
      return result;
    }

    // 初始化距离矩阵
    auto edges = graph.get_all_edges();

    // 设置直接边的距离
    for (int i = 0; i < n; i++) {
      result.distances[i][i] = 0;
    }

    for (const auto &edge : edges) {
      result.distances[edge.from][edge.to] = edge.weight;
      result.predecessors[edge.from][edge.to] = edge.from;
    }

    // Floyd-Warshall算法
    for (int k = 0; k < n; k++) {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (result.distances[i][k] != std::numeric_limits<int>::max() &&
              result.distances[k][j] != std::numeric_limits<int>::max()) {
            int new_dist = result.distances[i][k] + result.distances[k][j];
            if (new_dist < result.distances[i][j]) {
              result.distances[i][j] = new_dist;
              result.predecessors[i][j] = result.predecessors[k][j];
            }
          }
        }
      }
    }

    // 检查负权环
    for (int i = 0; i < n; i++) {
      if (result.distances[i][i] < 0) {
        result.has_negative_cycle = true;
        break;
      }
    }

    return result;
  }
};

// 25.3 Johnson算法（用于稀疏图）
class Johnson {
public:
  static AllPairsShortestPathResult
  find_all_pairs_shortest_path(const AdjacencyListGraph &graph) {
    int n = graph.get_node_count();
    AllPairsShortestPathResult result(n);

    if (n == 0) {
      return result;
    }

    // 步骤1：添加新节点s，连接到所有其他节点（权重为0）
    AdjacencyListGraph extended_graph = graph;
    int s = n; // 新节点编号
    extended_graph.add_node(s);

    for (int i = 0; i < n; i++) {
      try {
        extended_graph.add_edge(s, i, 0);
      } catch (const std::invalid_argument &e) {
        // 边已存在，忽略
      }
    }

    // 步骤2：使用Bellman-Ford算法计算从s到所有节点的最短路径
    auto bellman_result = BellmanFord::find_shortest_path(extended_graph, s);

    if (bellman_result.has_negative_cycle) {
      result.has_negative_cycle = true;
      return result;
    }

    // 步骤3：重新权重边，消除负权边
    std::vector<int> h(n + 1); // 势函数
    for (int i = 0; i <= n; i++) {
      h[i] = bellman_result.distances[i];
    }

    // 创建重新权重后的图
    AdjacencyListGraph reweighted_graph(true);
    for (int i = 0; i < n; i++) {
      reweighted_graph.add_node(i);
    }

    auto edges = graph.get_all_edges();
    for (const auto &edge : edges) {
      int new_weight = edge.weight + h[edge.from] - h[edge.to];
      try {
        reweighted_graph.add_edge(edge.from, edge.to, new_weight);
      } catch (const std::invalid_argument &e) {
        // 边已存在，忽略
      }
    }

    // 步骤4：对每个节点运行Dijkstra算法
    for (int i = 0; i < n; i++) {
      auto dijkstra_result = Dijkstra::find_shortest_path(reweighted_graph, i);

      // 恢复原始距离
      for (int j = 0; j < n; j++) {
        if (dijkstra_result.distances[j] != std::numeric_limits<int>::max()) {
          result.distances[i][j] = dijkstra_result.distances[j] + h[j] - h[i];
          result.predecessors[i][j] = dijkstra_result.predecessors[j];
        }
      }
    }

    return result;
  }
};

} // namespace algorithms

#endif // ALL_PAIRS_SHORTEST_PATH_H
