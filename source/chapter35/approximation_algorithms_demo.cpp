#include "approximation_algorithms.h"
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace algorithms;

/**
 * @brief 测试顶点覆盖的2-近似算法
 */
void test_vertex_cover_approximation() {
  std::cout << "=== 顶点覆盖2-近似算法测试 ===" << std::endl;

  // 构建测试图
  Graph graph(6);
  graph.add_edge(0, 1);
  graph.add_edge(0, 2);
  graph.add_edge(1, 2);
  graph.add_edge(1, 3);
  graph.add_edge(2, 4);
  graph.add_edge(3, 4);
  graph.add_edge(3, 5);
  graph.add_edge(4, 5);

  graph.print();

  // 应用2-近似算法
  auto cover = ApproximationAlgorithms::vertex_cover_2_approx(graph);

  std::cout << "近似顶点覆盖: ";
  for (int v : cover) {
    std::cout << v << " ";
  }
  std::cout << std::endl;
  std::cout << "覆盖大小: " << cover.size() << std::endl;

  // 验证覆盖有效性
  bool valid = true;
  for (int u = 0; u < graph.get_vertex_count(); u++) {
    for (int v : graph.get_neighbors(u)) {
      if (u < v) { // 避免重复检查
        if (cover.find(u) == cover.end() && cover.find(v) == cover.end()) {
          valid = false;
          break;
        }
      }
    }
  }

  std::cout << "覆盖是否有效: " << (valid ? "是" : "否") << std::endl;
  std::cout << "近似比: 2" << std::endl;
}

/**
 * @brief 测试旅行商问题的2-近似算法
 */
void test_tsp_approximation() {
  std::cout << "\n=== 旅行商问题2-近似算法测试 ===" << std::endl;

  // 构建完全图（满足三角不等式）
  std::vector<std::vector<int>> distance_matrix = {
      {0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}};

  std::cout << "距离矩阵:" << std::endl;
  for (const auto &row : distance_matrix) {
    for (int dist : row) {
      std::cout << dist << " ";
    }
    std::cout << std::endl;
  }

  // 应用2-近似算法
  auto tour = ApproximationAlgorithms::tsp_2_approx(distance_matrix);

  std::cout << "近似旅行路线: ";
  for (int city : tour) {
    std::cout << city << " ";
  }
  std::cout << std::endl;

  // 计算路线总成本
  int total_cost = 0;
  for (int i = 0; i < tour.size() - 1; i++) {
    total_cost += distance_matrix[tour[i]][tour[i + 1]];
  }

  std::cout << "路线总成本: " << total_cost << std::endl;
  std::cout << "近似比: 2" << std::endl;
}

/**
 * @brief 测试集合覆盖的近似算法
 */
void test_set_cover_approximation() {
  std::cout << "\n=== 集合覆盖近似算法测试 ===" << std::endl;

  // 定义全集
  std::unordered_set<int> universe = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // 定义子集
  std::vector<std::unordered_set<int>> subsets = {
      {1, 2, 3, 4}, {2, 5, 6}, {7, 8, 9, 10}, {3, 6, 9}, {1, 5, 7}, {4, 8, 10}};

  std::cout << "全集: ";
  for (int elem : universe) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  std::cout << "子集:" << std::endl;
  for (int i = 0; i < subsets.size(); i++) {
    std::cout << "子集" << i << ": ";
    for (int elem : subsets[i]) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;
  }

  // 应用贪心算法
  auto cover = ApproximationAlgorithms::set_cover_greedy(universe, subsets);

  std::cout << "近似集合覆盖:" << std::endl;
  for (int i = 0; i < cover.size(); i++) {
    std::cout << "覆盖子集" << i << ": ";
    for (int elem : cover[i]) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "覆盖大小: " << cover.size() << std::endl;
  std::cout << "近似比: O(log n)" << std::endl;
}

/**
 * @brief 测试背包问题的近似算法
 */
void test_knapsack_approximation() {
  std::cout << "\n=== 背包问题近似算法测试 ===" << std::endl;

  // 定义物品
  std::vector<int> weights = {2, 3, 4, 5};
  std::vector<int> values = {3, 4, 5, 6};
  int capacity = 8;

  std::cout << "背包容量: " << capacity << std::endl;
  std::cout << "物品信息:" << std::endl;
  for (int i = 0; i < weights.size(); i++) {
    std::cout << "物品" << i << ": 重量=" << weights[i]
              << ", 价值=" << values[i]
              << ", 价值密度=" << static_cast<double>(values[i]) / weights[i]
              << std::endl;
  }

  // 应用贪心算法
  auto selected =
      ApproximationAlgorithms::knapsack_greedy(weights, values, capacity);

  std::cout << "选择的物品: ";
  int total_weight = 0;
  int total_value = 0;
  for (int index : selected) {
    std::cout << index << " ";
    total_weight += weights[index];
    total_value += values[index];
  }
  std::cout << std::endl;

  std::cout << "总重量: " << total_weight << ", 总价值: " << total_value
            << std::endl;
  std::cout << "近似比: 任意接近最优解（通过近似方案）" << std::endl;
}

/**
 * @brief 测试最大割问题的2-近似算法
 */
void test_max_cut_approximation() {
  std::cout << "\n=== 最大割问题2-近似算法测试 ===" << std::endl;

  // 构建加权图
  Graph graph(5);
  graph.add_edge(0, 1, 2);
  graph.add_edge(0, 2, 3);
  graph.add_edge(0, 3, 1);
  graph.add_edge(1, 2, 4);
  graph.add_edge(1, 4, 2);
  graph.add_edge(2, 3, 3);
  graph.add_edge(3, 4, 5);

  graph.print();

  // 应用2-近似算法
  auto [set_a, set_b] = ApproximationAlgorithms::max_cut_2_approx(graph);

  std::cout << "集合A: ";
  for (int v : set_a) {
    std::cout << v << " ";
  }
  std::cout << std::endl;

  std::cout << "集合B: ";
  for (int v : set_b) {
    std::cout << v << " ";
  }
  std::cout << std::endl;

  // 计算割值
  int cut_value = 0;
  for (int u = 0; u < graph.get_vertex_count(); u++) {
    for (int v : graph.get_neighbors(u)) {
      if (u < v) { // 避免重复计算
        if ((set_a.find(u) != set_a.end() && set_b.find(v) != set_b.end()) ||
            (set_b.find(u) != set_b.end() && set_a.find(v) != set_a.end())) {
          cut_value += graph.get_weight(u, v);
        }
      }
    }
  }

  std::cout << "割值: " << cut_value << std::endl;
  std::cout << "近似比: 2" << std::endl;
}

/**
 * @brief 测试设施选址问题的近似算法
 */
void test_facility_location_approximation() {
  std::cout << "\n=== 设施选址问题近似算法测试 ===" << std::endl;

  // 定义设施开设成本
  std::vector<int> facility_costs = {10, 15, 20, 12};

  // 定义距离矩阵（设施到客户的距离）
  std::vector<std::vector<int>> distances = {
      {2, 5, 8, 3}, // 设施0到各客户的距离
      {6, 3, 7, 4}, // 设施1到各客户的距离
      {4, 6, 2, 5}, // 设施2到各客户的距离
      {7, 4, 3, 6}  // 设施3到各客户的距离
  };

  std::cout << "设施开设成本: ";
  for (int cost : facility_costs) {
    std::cout << cost << " ";
  }
  std::cout << std::endl;

  std::cout << "距离矩阵:" << std::endl;
  for (const auto &row : distances) {
    for (int dist : row) {
      std::cout << dist << " ";
    }
    std::cout << std::endl;
  }

  // 应用近似算法
  auto [selected_facilities, assignments] =
      ApproximationAlgorithms::facility_location_approx(facility_costs,
                                                        distances);

  std::cout << "选择的设施: ";
  for (int f : selected_facilities) {
    std::cout << f << " ";
  }
  std::cout << std::endl;

  std::cout << "客户分配:" << std::endl;
  for (int c = 0; c < assignments.size(); c++) {
    std::cout << "客户" << c << " -> 设施" << assignments[c] << std::endl;
  }

  // 计算总成本
  int total_cost = 0;
  for (int f : selected_facilities) {
    total_cost += facility_costs[f];
  }
  for (int c = 0; c < assignments.size(); c++) {
    total_cost += distances[assignments[c]][c];
  }

  std::cout << "总成本: " << total_cost << std::endl;
  std::cout << "近似比: O(log n)" << std::endl;
}

/**
 * @brief 演示近似算法的性能分析
 */
void demonstrate_performance_analysis() {
  std::cout << "\n=== 近似算法性能分析 ===" << std::endl;

  ApproximationAlgorithms::demonstrate_approximation_ratios();

  std::cout << "\n近似算法的优势:" << std::endl;
  std::cout << "1. 多项式时间复杂性" << std::endl;
  std::cout << "2. 可证明的近似保证" << std::endl;
  std::cout << "3. 实际应用中的良好表现" << std::endl;
  std::cout << "4. 对大规模问题的可扩展性" << std::endl;

  std::cout << "\n近似算法的局限性:" << std::endl;
  std::cout << "1. 解的质量可能不如精确算法" << std::endl;
  std::cout << "2. 某些问题难以获得好的近似比" << std::endl;
  std::cout << "3. 近似比分析可能比较复杂" << std::endl;
}

/**
 * @brief 演示近似算法在实际中的应用
 */
void demonstrate_real_world_applications() {
  std::cout << "\n=== 近似算法在实际中的应用 ===" << std::endl;

  std::cout << "1. 网络设计" << std::endl;
  std::cout << "   - 使用TSP近似算法规划网络布线" << std::endl;
  std::cout << "   - 使用顶点覆盖优化网络监控点布局" << std::endl;

  std::cout << "\n2. 资源分配" << std::endl;
  std::cout << "   - 使用背包问题近似算法进行资源分配" << std::endl;
  std::cout << "   - 使用集合覆盖优化服务覆盖范围" << std::endl;

  std::cout << "\n3. 生产调度" << std::endl;
  std::cout << "   - 使用设施选址算法优化工厂布局" << std::endl;
  std::cout << "   - 使用最大割算法进行任务分配" << std::endl;

  std::cout << "\n4. 数据挖掘" << std::endl;
  std::cout << "   - 使用聚类近似算法进行数据分组" << std::endl;
  std::cout << "   - 使用覆盖算法进行特征选择" << std::endl;

  std::cout << "\n实际应用中的考虑:" << std::endl;
  std::cout << "- 问题规模通常很大，精确算法不可行" << std::endl;
  std::cout << "- 近似解的质量通常足够满足实际需求" << std::endl;
  std::cout << "- 计算效率是首要考虑因素" << std::endl;
}

int main() {
  std::cout << "近似算法演示程序" << std::endl;
  std::cout << "==================" << std::endl;

  try {
    test_vertex_cover_approximation();
    test_tsp_approximation();
    test_set_cover_approximation();
    test_knapsack_approximation();
    test_max_cut_approximation();
    test_facility_location_approximation();
    demonstrate_performance_analysis();
    demonstrate_real_world_applications();

    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "近似算法是解决NP完全问题的重要工具" << std::endl;
    std::cout << "通过在多项式时间内提供质量有保证的近似解" << std::endl;
    std::cout << "在实际应用中发挥了重要作用" << std::endl;

    std::cout << "\n所有测试完成!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "错误: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
