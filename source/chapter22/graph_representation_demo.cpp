#include "graph_representation.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace algorithms;

void test_classic_examples() {
  std::cout << "=== 测试经典示例（算法导论图22.1和22.2） ===" << std::endl;

  print_classic_examples();
  std::cout << std::endl;
}

void test_adjacency_list_operations() {
  std::cout << "=== 测试邻接表操作 ===" << std::endl;

  // 创建无向图
  AdjacencyListGraph graph(false);

  // 添加节点
  for (int i = 1; i <= 6; i++) {
    graph.add_node(i, "Node" + std::to_string(i));
  }

  // 添加边
  graph.add_edge(1, 2);
  graph.add_edge(1, 3);
  graph.add_edge(2, 4);
  graph.add_edge(2, 5);
  graph.add_edge(3, 6);
  graph.add_edge(4, 5);
  graph.add_edge(5, 6);

  graph.print_adjacency_list();

  std::cout << "图统计信息:" << std::endl;
  std::cout << "  节点数量: " << graph.get_node_count() << std::endl;
  std::cout << "  边数量: " << graph.get_edge_count() << std::endl;
  std::cout << "  是有向图: " << (graph.is_directed() ? "是" : "否")
            << std::endl;

  // 测试邻居查询
  std::cout << "\n邻居查询测试:" << std::endl;
  for (int i = 1; i <= 6; i++) {
    auto neighbors = graph.get_neighbors(i);
    std::cout << "  节点 " << i << " 的邻居: ";
    for (int neighbor : neighbors) {
      std::cout << neighbor << " ";
    }
    std::cout << "(度=" << graph.get_degree(i) << ")" << std::endl;
  }

  // 测试BFS
  std::cout << "\n广度优先搜索 (BFS) 测试:" << std::endl;
  auto bfs_result = graph.bfs(1);
  std::cout << "  从节点1开始的BFS: ";
  for (int node : bfs_result) {
    std::cout << node << " ";
  }
  std::cout << std::endl;

  // 测试DFS
  std::cout << "深度优先搜索 (DFS) 测试:" << std::endl;
  auto dfs_result = graph.dfs(1);
  std::cout << "  从节点1开始的DFS: ";
  for (int node : dfs_result) {
    std::cout << node << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
}

void test_adjacency_matrix_operations() {
  std::cout << "=== 测试邻接矩阵操作 ===" << std::endl;

  // 创建有向图
  AdjacencyMatrixGraph graph(true);

  // 添加节点
  for (int i = 1; i <= 5; i++) {
    graph.add_node(i, "Node" + std::to_string(i));
  }

  // 添加带权重的边
  graph.add_edge(1, 2, 3);
  graph.add_edge(1, 3, 8);
  graph.add_edge(1, 5, -4);
  graph.add_edge(2, 4, 1);
  graph.add_edge(2, 5, 7);
  graph.add_edge(3, 2, 4);
  graph.add_edge(4, 1, 2);
  graph.add_edge(4, 3, -5);
  graph.add_edge(5, 4, 6);

  graph.print_adjacency_matrix();

  std::cout << "图统计信息:" << std::endl;
  std::cout << "  节点数量: " << graph.get_node_count() << std::endl;
  std::cout << "  边数量: " << graph.get_edge_count() << std::endl;
  std::cout << "  是有向图: " << (graph.is_directed() ? "是" : "否")
            << std::endl;

  // 测试邻居查询
  std::cout << "\n邻居查询测试:" << std::endl;
  for (int i = 1; i <= 5; i++) {
    auto neighbors = graph.get_neighbors(i);
    std::cout << "  节点 " << i << " 的邻居: ";
    for (int neighbor : neighbors) {
      std::cout << neighbor << " ";
    }
    std::cout << "(出度=" << graph.get_degree(i) << ")" << std::endl;
  }

  // 测试边查询
  std::cout << "\n边查询测试:" << std::endl;
  std::vector<std::pair<int, int>> test_edges = {
      {1, 2}, {1, 3}, {2, 1}, {3, 4}, {4, 5}};

  for (const auto &edge : test_edges) {
    int from = edge.first;
    int to = edge.second;
    bool exists = graph.has_edge(from, to);
    int weight = graph.get_edge_weight(from, to);

    std::cout << "  边 " << from << " -> " << to << ": ";
    if (exists) {
      std::cout << "存在 (权重=" << weight << ")" << std::endl;
    } else {
      std::cout << "不存在" << std::endl;
    }
  }

  std::cout << std::endl;
}

void test_edge_cases() {
  std::cout << "=== 测试边界情况 ===" << std::endl;

  // 测试空图
  AdjacencyListGraph empty_graph(false);
  std::cout << "空图测试:" << std::endl;
  std::cout << "  节点数量: " << empty_graph.get_node_count() << std::endl;
  std::cout << "  边数量: " << empty_graph.get_edge_count() << std::endl;

  // 测试单节点图
  AdjacencyListGraph single_node_graph(false);
  single_node_graph.add_node(1);

  std::cout << "\n单节点图测试:" << std::endl;
  std::cout << "  节点数量: " << single_node_graph.get_node_count()
            << std::endl;
  std::cout << "  边数量: " << single_node_graph.get_edge_count() << std::endl;
  std::cout << "  节点1的度: " << single_node_graph.get_degree(1) << std::endl;

  // 测试自环
  AdjacencyMatrixGraph self_loop_graph(true);
  self_loop_graph.add_node(1);
  self_loop_graph.add_edge(1, 1, 5);

  std::cout << "\n自环图测试:" << std::endl;
  std::cout << "  节点1的邻居: ";
  auto neighbors = self_loop_graph.get_neighbors(1);
  for (int neighbor : neighbors) {
    std::cout << neighbor << " ";
  }
  std::cout << std::endl;
  std::cout << "  边1->1的权重: " << self_loop_graph.get_edge_weight(1, 1)
            << std::endl;

  std::cout << std::endl;
}

void test_error_handling() {
  std::cout << "=== 测试错误处理 ===" << std::endl;

  AdjacencyListGraph graph(false);
  graph.add_node(1);
  graph.add_node(2);

  // 测试重复添加节点
  try {
    graph.add_node(1);
    std::cout << "  重复添加节点: 应该抛出异常" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "  重复添加节点: 正确抛出异常" << std::endl;
  }

  // 测试重复添加边
  graph.add_edge(1, 2);
  try {
    graph.add_edge(1, 2);
    std::cout << "  重复添加边: 应该抛出异常" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "  重复添加边: 正确抛出异常" << std::endl;
  }

  // 测试添加不存在的节点
  try {
    graph.add_edge(1, 3);
    std::cout << "  添加不存在的节点: 应该抛出异常" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "  添加不存在的节点: 正确抛出异常" << std::endl;
  }

  std::cout << std::endl;
}

void test_comparison_between_representations() {
  std::cout << "=== 两种表示方法的对比 ===" << std::endl;

  // 创建相同的图，使用两种不同的表示方法
  std::vector<std::pair<int, int>> edges = {{1, 2}, {1, 3}, {2, 4}, {2, 5},
                                            {3, 6}, {4, 5}, {5, 6}};

  // 邻接表表示
  AdjacencyListGraph list_graph(false);
  for (int i = 1; i <= 6; i++) {
    list_graph.add_node(i);
  }
  for (const auto &edge : edges) {
    list_graph.add_edge(edge.first, edge.second);
  }

  // 邻接矩阵表示
  AdjacencyMatrixGraph matrix_graph(false);
  for (int i = 1; i <= 6; i++) {
    matrix_graph.add_node(i);
  }
  for (const auto &edge : edges) {
    matrix_graph.add_edge(edge.first, edge.second);
  }

  std::cout << "邻接表表示:" << std::endl;
  std::cout << "  空间复杂度: O(V + E)" << std::endl;
  std::cout << "  添加边的时间: O(1)" << std::endl;
  std::cout << "  查询邻居的时间: O(degree)" << std::endl;

  std::cout << "\n邻接矩阵表示:" << std::endl;
  std::cout << "  空间复杂度: O(V^2)" << std::endl;
  std::cout << "  添加边的时间: O(1)" << std::endl;
  std::cout << "  查询边的时间: O(1)" << std::endl;

  // 验证两种表示方法的一致性
  bool consistent = true;
  for (int i = 1; i <= 6; i++) {
    auto list_neighbors = list_graph.get_neighbors(i);
    auto matrix_neighbors = matrix_graph.get_neighbors(i);

    if (list_neighbors != matrix_neighbors) {
      consistent = false;
      break;
    }
  }

  if (consistent) {
    std::cout << "\n验证: 两种表示方法结果一致" << std::endl;
  } else {
    std::cout << "\n验证: 两种表示方法结果不一致" << std::endl;
  }

  std::cout << std::endl;
}

int main() {
  std::cout << "第22.1章 图的表示演示程序" << std::endl;
  std::cout << "============================" << std::endl;

  test_classic_examples();
  test_adjacency_list_operations();
  test_adjacency_matrix_operations();
  test_edge_cases();
  test_error_handling();
  test_comparison_between_representations();

  std::cout << "所有测试完成！" << std::endl;

  return 0;
}
