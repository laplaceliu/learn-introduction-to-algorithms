#include "minimum_spanning_tree.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace algorithms;

void test_classic_examples() {
  std::cout << "=== 测试经典最小生成树示例 ===" << std::endl;

  print_mst_classic_examples();
  std::cout << std::endl;
}

void test_kruskal_algorithm() {
  std::cout << "=== 测试Kruskal算法 ===" << std::endl;

  // 创建无向图
  AdjacencyListGraph graph(false);

  // 添加节点
  for (int i = 1; i <= 6; i++) {
    graph.add_node(i, "Node" + std::to_string(i));
  }

  // 添加带权重的边
  graph.add_edge(1, 2, 3);
  graph.add_edge(1, 3, 1);
  graph.add_edge(2, 3, 7);
  graph.add_edge(2, 4, 5);
  graph.add_edge(3, 4, 2);
  graph.add_edge(3, 5, 4);
  graph.add_edge(4, 5, 6);
  graph.add_edge(4, 6, 8);
  graph.add_edge(5, 6, 9);

  graph.print_adjacency_list();

  MinimumSpanningTree mst(graph);

  // 测试Kruskal算法
  auto kruskal_edges = mst.kruskal();
  MinimumSpanningTree::print_mst(kruskal_edges, "Kruskal算法");

  // 验证MST
  bool valid = mst.validate_mst(kruskal_edges);
  std::cout << "MST验证: " << (valid ? "通过" : "失败") << std::endl;

  int total_weight = mst.calculate_total_weight(kruskal_edges);
  std::cout << "总权重: " << total_weight << std::endl;

  std::cout << std::endl;
}

void test_prim_algorithm() {
  std::cout << "=== 测试Prim算法 ===" << std::endl;

  // 创建无向图
  AdjacencyListGraph graph(false);

  // 添加节点
  for (int i = 1; i <= 7; i++) {
    graph.add_node(i, "Node" + std::to_string(i));
  }

  // 添加带权重的边
  graph.add_edge(1, 2, 2);
  graph.add_edge(1, 3, 4);
  graph.add_edge(1, 4, 1);
  graph.add_edge(2, 4, 3);
  graph.add_edge(2, 5, 10);
  graph.add_edge(3, 4, 2);
  graph.add_edge(3, 6, 5);
  graph.add_edge(4, 5, 7);
  graph.add_edge(4, 6, 8);
  graph.add_edge(4, 7, 4);
  graph.add_edge(5, 7, 6);
  graph.add_edge(6, 7, 1);

  graph.print_adjacency_list();

  MinimumSpanningTree mst(graph);

  // 测试Prim算法（从不同起始节点）
  auto prim_edges = mst.prim(1);
  MinimumSpanningTree::print_mst(prim_edges, "Prim算法（从节点1开始）");

  // 验证MST
  bool valid = mst.validate_mst(prim_edges);
  std::cout << "MST验证: " << (valid ? "通过" : "失败") << std::endl;

  int total_weight = mst.calculate_total_weight(prim_edges);
  std::cout << "总权重: " << total_weight << std::endl;

  std::cout << std::endl;
}

void test_comparison_between_algorithms() {
  std::cout << "=== 两种最小生成树算法对比 ===" << std::endl;

  // 创建相同的无向图
  AdjacencyListGraph graph(false);

  // 添加节点
  for (int i = 1; i <= 5; i++) {
    graph.add_node(i, "Node" + std::to_string(i));
  }

  // 添加带权重的边
  graph.add_edge(1, 2, 1);
  graph.add_edge(1, 3, 7);
  graph.add_edge(2, 3, 5);
  graph.add_edge(2, 4, 4);
  graph.add_edge(2, 5, 3);
  graph.add_edge(3, 4, 6);
  graph.add_edge(3, 5, 8);
  graph.add_edge(4, 5, 2);

  graph.print_adjacency_list();

  MinimumSpanningTree mst(graph);

  // 测试Kruskal算法
  auto kruskal_edges = mst.kruskal();
  MinimumSpanningTree::print_mst(kruskal_edges, "Kruskal算法");

  // 测试Prim算法
  auto prim_edges = mst.prim(1);
  MinimumSpanningTree::print_mst(prim_edges, "Prim算法");

  // 比较两种算法的结果
  int kruskal_weight = mst.calculate_total_weight(kruskal_edges);
  int prim_weight = mst.calculate_total_weight(prim_edges);

  std::cout << "\n算法对比:" << std::endl;
  std::cout << "  Kruskal算法总权重: " << kruskal_weight << std::endl;
  std::cout << "  Prim算法总权重: " << prim_weight << std::endl;

  if (kruskal_weight == prim_weight) {
    std::cout << "  两种算法得到相同的最小生成树总权重" << std::endl;
  } else {
    std::cout << "  错误: 两种算法得到不同的总权重" << std::endl;
  }

  std::cout << "\n算法特点:" << std::endl;
  std::cout << "  Kruskal算法:" << std::endl;
  std::cout << "    - 基于边排序和并查集" << std::endl;
  std::cout << "    - 时间复杂度: O(E log E)" << std::endl;
  std::cout << "    - 适合稀疏图" << std::endl;

  std::cout << "  Prim算法:" << std::endl;
  std::cout << "    - 基于节点扩展和优先队列" << std::endl;
  std::cout << "    - 时间复杂度: O(E log V)" << std::endl;
  std::cout << "    - 适合稠密图" << std::endl;

  std::cout << std::endl;
}

void test_edge_cases() {
  std::cout << "=== 测试边界情况 ===" << std::endl;

  // 测试单节点图
  std::cout << "测试单节点图:" << std::endl;
  AdjacencyListGraph single_node_graph(false);
  single_node_graph.add_node(1);

  try {
    MinimumSpanningTree mst(single_node_graph);
    auto edges = mst.kruskal();
    std::cout << "  单节点图的MST边数: " << edges.size() << " (应为0)"
              << std::endl;
  } catch (const std::exception &e) {
    std::cout << "  单节点图处理: " << e.what() << std::endl;
  }

  // 测试完全图
  std::cout << "\n测试完全图:" << std::endl;
  AdjacencyListGraph complete_graph(false);

  for (int i = 1; i <= 4; i++) {
    complete_graph.add_node(i);
  }

  // 添加所有可能的边
  for (int i = 1; i <= 4; i++) {
    for (int j = i + 1; j <= 4; j++) {
      complete_graph.add_edge(i, j, i + j); // 权重为节点编号之和
    }
  }

  complete_graph.print_adjacency_list();

  MinimumSpanningTree mst(complete_graph);
  auto edges = mst.kruskal();
  MinimumSpanningTree::print_mst(edges, "完全图的MST");

  bool valid = mst.validate_mst(edges);
  std::cout << "MST验证: " << (valid ? "通过" : "失败") << std::endl;

  std::cout << std::endl;
}

void test_error_handling() {
  std::cout << "=== 测试错误处理 ===" << std::endl;

  // 测试有向图
  std::cout << "测试有向图:" << std::endl;
  AdjacencyListGraph directed_graph(true);
  directed_graph.add_node(1);
  directed_graph.add_node(2);
  directed_graph.add_edge(1, 2);

  try {
    MinimumSpanningTree mst(directed_graph);
    std::cout << "  错误: 应该检测到有向图并抛出异常" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "  正确检测到有向图: " << e.what() << std::endl;
  }

  // 测试空图
  std::cout << "\n测试空图:" << std::endl;
  AdjacencyListGraph empty_graph(false);

  try {
    MinimumSpanningTree mst(empty_graph);
    std::cout << "  错误: 应该检测到空图并抛出异常" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "  正确检测到空图: " << e.what() << std::endl;
  }

  // 测试不连通图
  std::cout << "\n测试不连通图:" << std::endl;
  AdjacencyListGraph disconnected_graph(false);

  for (int i = 1; i <= 4; i++) {
    disconnected_graph.add_node(i);
  }

  // 只添加部分边，使图不连通
  disconnected_graph.add_edge(1, 2, 1);
  disconnected_graph.add_edge(3, 4, 1);

  disconnected_graph.print_adjacency_list();

  MinimumSpanningTree mst(disconnected_graph);
  auto edges = mst.kruskal();

  std::cout << "不连通图的MST边数: " << edges.size()
            << " (应为3，但实际可能更少)" << std::endl;
  bool valid = mst.validate_mst(edges);
  std::cout << "MST验证: " << (valid ? "通过" : "失败") << std::endl;

  std::cout << std::endl;
}

void test_algorithm_correctness() {
  std::cout << "=== 测试算法正确性 ===" << std::endl;

  // 创建复杂的无向图
  AdjacencyListGraph complex_graph(false);

  // 添加节点
  for (int i = 1; i <= 8; i++) {
    complex_graph.add_node(i);
  }

  // 添加复杂的边关系
  complex_graph.add_edge(1, 2, 4);
  complex_graph.add_edge(1, 3, 3);
  complex_graph.add_edge(1, 4, 2);
  complex_graph.add_edge(2, 5, 5);
  complex_graph.add_edge(2, 6, 6);
  complex_graph.add_edge(3, 4, 1);
  complex_graph.add_edge(3, 7, 7);
  complex_graph.add_edge(4, 8, 8);
  complex_graph.add_edge(5, 6, 2);
  complex_graph.add_edge(5, 7, 3);
  complex_graph.add_edge(6, 8, 4);
  complex_graph.add_edge(7, 8, 5);

  complex_graph.print_adjacency_list();

  MinimumSpanningTree mst(complex_graph);

  // 测试两种算法的一致性
  auto kruskal_edges = mst.kruskal();
  auto prim_edges = mst.prim(1);

  MinimumSpanningTree::print_mst(kruskal_edges, "Kruskal算法");
  MinimumSpanningTree::print_mst(prim_edges, "Prim算法");

  // 验证两种算法的结果
  bool kruskal_valid = mst.validate_mst(kruskal_edges);
  bool prim_valid = mst.validate_mst(prim_edges);

  std::cout << "\n正确性验证:" << std::endl;
  std::cout << "  Kruskal算法MST验证: " << (kruskal_valid ? "通过" : "失败")
            << std::endl;
  std::cout << "  Prim算法MST验证: " << (prim_valid ? "通过" : "失败")
            << std::endl;

  int kruskal_weight = mst.calculate_total_weight(kruskal_edges);
  int prim_weight = mst.calculate_total_weight(prim_edges);

  std::cout << "  权重比较 - Kruskal: " << kruskal_weight
            << ", Prim: " << prim_weight << std::endl;

  if (kruskal_valid && prim_valid && kruskal_weight == prim_weight) {
    std::cout << "  两种算法都产生了正确的最小生成树" << std::endl;
  }

  std::cout << std::endl;
}

int main() {
  std::cout << "第23章 最小生成树演示程序" << std::endl;
  std::cout << "============================" << std::endl;

  test_classic_examples();
  test_kruskal_algorithm();
  test_prim_algorithm();
  test_comparison_between_algorithms();
  test_edge_cases();
  test_error_handling();
  test_algorithm_correctness();

  std::cout << "所有测试完成！" << std::endl;

  return 0;
}
