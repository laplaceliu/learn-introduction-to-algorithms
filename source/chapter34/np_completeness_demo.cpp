#include "np_completeness.h"
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace algorithms;

/**
 * @brief 测试P类问题
 */
void test_p_class_problems() {
  std::cout << "=== 测试P类问题 ===" << std::endl;

  NPCompleteness::demonstrate_p_class();

  // 演示更多P类问题
  std::cout << "\n其他P类问题示例:" << std::endl;
  std::cout << "1. 最短路径问题 (Dijkstra算法)" << std::endl;
  std::cout << "2. 最小生成树问题 (Prim/Kruskal算法)" << std::endl;
  std::cout << "3. 最大流问题 (Ford-Fulkerson算法)" << std::endl;
  std::cout << "4. 线性规划问题 (单纯形法)" << std::endl;
  std::cout << "5. 字符串匹配问题 (KMP算法)" << std::endl;
}

/**
 * @brief 测试NP类问题验证器
 */
void test_np_verifiers() {
  std::cout << "\n=== 测试NP类问题验证器 ===" << std::endl;

  NPCompleteness::demonstrate_np_class();

  // 测试顶点覆盖验证器
  std::cout << "\n--- 顶点覆盖验证器测试 ---" << std::endl;
  Graph graph1(4);
  graph1.add_edge(0, 1);
  graph1.add_edge(0, 2);
  graph1.add_edge(1, 2);
  graph1.add_edge(1, 3);
  graph1.add_edge(2, 3);

  graph1.print();

  std::unordered_set<int> cover1 = {1, 2};
  std::unordered_set<int> cover2 = {0, 3};

  std::cout << "覆盖{1, 2}是否有效: "
            << (NPCompleteness::verify_vertex_cover(graph1, cover1, 2) ? "是"
                                                                       : "否")
            << std::endl;
  std::cout << "覆盖{0, 3}是否有效: "
            << (NPCompleteness::verify_vertex_cover(graph1, cover2, 2) ? "是"
                                                                       : "否")
            << std::endl;

  // 测试团问题验证器
  std::cout << "\n--- 团问题验证器测试 ---" << std::endl;
  Graph graph2(5);
  graph2.add_edge(0, 1);
  graph2.add_edge(0, 2);
  graph2.add_edge(0, 3);
  graph2.add_edge(1, 2);
  graph2.add_edge(1, 3);
  graph2.add_edge(2, 3);
  graph2.add_edge(3, 4);

  graph2.print();

  std::unordered_set<int> clique1 = {0, 1, 2, 3};
  std::unordered_set<int> clique2 = {0, 1, 4};

  std::cout << "团{0, 1, 2, 3}是否有效: "
            << (NPCompleteness::verify_clique(graph2, clique1, 4) ? "是" : "否")
            << std::endl;
  std::cout << "团{0, 1, 4}是否有效: "
            << (NPCompleteness::verify_clique(graph2, clique2, 3) ? "是" : "否")
            << std::endl;

  // 测试子集和问题验证器
  std::cout << "\n--- 子集和问题验证器测试 ---" << std::endl;
  std::vector<int> set = {3, 34, 4, 12, 5, 2};
  int target = 9;

  std::cout << "集合: ";
  for (int num : set) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
  std::cout << "目标和: " << target << std::endl;

  std::unordered_set<int> subset1 = {4, 5};
  std::unordered_set<int> subset2 = {3, 4, 2};

  std::cout << "子集{4, 5}和是否为9: "
            << (NPCompleteness::verify_subset_sum(set, subset1, target) ? "是"
                                                                        : "否")
            << std::endl;
  std::cout << "子集{3, 4, 2}和是否为9: "
            << (NPCompleteness::verify_subset_sum(set, subset2, target) ? "是"
                                                                        : "否")
            << std::endl;
}

/**
 * @brief 测试NP完全问题
 */
void test_np_complete_problems() {
  std::cout << "\n=== 测试NP完全问题 ===" << std::endl;

  NPCompleteness::demonstrate_np_complete();

  // 测试哈密顿回路问题
  std::cout << "\n--- 哈密顿回路问题测试 ---" << std::endl;
  Graph graph(5);
  graph.add_edge(0, 1);
  graph.add_edge(0, 3);
  graph.add_edge(1, 2);
  graph.add_edge(1, 4);
  graph.add_edge(2, 3);
  graph.add_edge(2, 4);
  graph.add_edge(3, 4);

  graph.print();

  std::vector<int> path1 = {0, 1, 2, 3, 4, 0}; // 有效哈密顿回路
  std::vector<int> path2 = {0, 1, 3, 4, 0};    // 无效路径（缺少顶点2）

  std::cout << "路径1: ";
  for (int v : path1) {
    std::cout << v << " ";
  }
  std::cout << "是否是哈密顿回路: "
            << (NPCompleteness::verify_hamiltonian_cycle(graph, path1) ? "是"
                                                                       : "否")
            << std::endl;

  std::cout << "路径2: ";
  for (int v : path2) {
    std::cout << v << " ";
  }
  std::cout << "是否是哈密顿回路: "
            << (NPCompleteness::verify_hamiltonian_cycle(graph, path2) ? "是"
                                                                       : "否")
            << std::endl;

  // 测试旅行商问题
  std::cout << "\n--- 旅行商问题测试 ---" << std::endl;
  std::vector<std::vector<int>> distance_matrix = {
      {0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}};

  std::vector<int> tour1 = {0, 1, 3, 2, 0}; // 有效旅行路线
  std::vector<int> tour2 = {0, 2, 1, 0};    // 无效路线（缺少顶点3）

  int max_cost = 80;

  std::cout << "距离矩阵:" << std::endl;
  for (const auto &row : distance_matrix) {
    for (int dist : row) {
      std::cout << dist << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "最大允许成本: " << max_cost << std::endl;

  std::cout << "路线1: ";
  for (int v : tour1) {
    std::cout << v << " ";
  }
  std::cout << "成本是否不超过" << max_cost << ": "
            << (NPCompleteness::verify_tsp(distance_matrix, tour1, max_cost)
                    ? "是"
                    : "否")
            << std::endl;

  std::cout << "路线2: ";
  for (int v : tour2) {
    std::cout << v << " ";
  }
  std::cout << "是否是有效路线: "
            << (NPCompleteness::verify_tsp(distance_matrix, tour2, max_cost)
                    ? "是"
                    : "否")
            << std::endl;
}

/**
 * @brief 测试问题归约
 */
void test_problem_reductions() {
  std::cout << "\n=== 测试问题归约 ===" << std::endl;

  NPCompleteness::demonstrate_reduction();

  std::cout << "\n具体归约示例:" << std::endl;
  std::cout << "1. 3-SAT ≤p 顶点覆盖" << std::endl;
  std::cout << "   - 将3-CNF公式转换为图" << std::endl;
  std::cout << "   - 每个子句对应一个三角形" << std::endl;
  std::cout << "   - 变量对应顶点，文字对应边" << std::endl;

  std::cout << "\n2. 顶点覆盖 ≤p 团问题" << std::endl;
  std::cout << "   - 图G有大小为k的顶点覆盖" << std::endl;
  std::cout << "   - 当且仅当G的补图有大小为n-k的团" << std::endl;

  std::cout << "\n3. 团问题 ≤p 哈密顿回路" << std::endl;
  std::cout << "   - 将团问题实例转换为哈密顿回路实例" << std::endl;
  std::cout << "   - 通过构造特殊图结构实现" << std::endl;

  std::cout << "\n归约的重要性:" << std::endl;
  std::cout << "- 建立了NP完全问题之间的等价关系" << std::endl;
  std::cout << "- 为证明新问题的NP完全性提供了工具" << std::endl;
  std::cout << "- 揭示了计算复杂性的内在结构" << std::endl;
}

/**
 * @brief 测试P vs NP问题
 */
void test_p_vs_np() {
  std::cout << "\n=== 测试P vs NP问题 ===" << std::endl;

  NPCompleteness::demonstrate_p_vs_np();

  std::cout << "\n当前研究状态:" << std::endl;
  std::cout << "1. 已经证明数千个问题是NP完全的" << std::endl;
  std::cout << "2. 没有找到任何NP完全问题的多项式时间算法" << std::endl;
  std::cout << "3. 大多数专家相信P ≠ NP" << std::endl;

  std::cout << "\n实际应用:" << std::endl;
  std::cout << "1. 密码学: 基于NP问题的困难性" << std::endl;
  std::cout << "2. 近似算法: 为NP完全问题寻找近似解" << std::endl;
  std::cout << "3. 启发式算法: 在实际应用中寻找可行解" << std::endl;
  std::cout << "4. 参数化复杂性: 研究问题参数对复杂性的影响" << std::endl;
}

/**
 * @brief 演示NP完全性在实际中的应用
 */
void demonstrate_real_world_applications() {
  std::cout << "\n=== NP完全性在实际中的应用 ===" << std::endl;

  std::cout << "1. 调度问题" << std::endl;
  std::cout << "   - 作业车间调度 (NP完全)" << std::endl;
  std::cout << "   - 航班调度" << std::endl;
  std::cout << "   - 课程表安排" << std::endl;

  std::cout << "\n2. 网络设计" << std::endl;
  std::cout << "   - 网络布线优化" << std::endl;
  std::cout << "   - 服务器布局" << std::endl;
  std::cout << "   - 通信网络规划" << std::endl;

  std::cout << "\n3. 生物信息学" << std::endl;
  std::cout << "   - 蛋白质结构预测" << std::endl;
  std::cout << "   - DNA序列比对" << std::endl;
  std::cout << "   - 基因调控网络分析" << std::endl;

  std::cout << "\n4. 人工智能" << std::endl;
  std::cout << "   - 知识表示与推理" << std::endl;
  std::cout << "   - 规划问题" << std::endl;
  std::cout << "   - 机器学习中的优化问题" << std::endl;

  std::cout << "\n应对策略:" << std::endl;
  std::cout << "- 近似算法: 牺牲精度换取效率" << std::endl;
  std::cout << "- 启发式搜索: 利用问题特性" << std::endl;
  std::cout << "- 并行计算: 利用多核/分布式计算" << std::endl;
  std::cout << "- 问题简化: 针对特定实例优化" << std::endl;
}

int main() {
  std::cout << "NP完全性演示程序" << std::endl;
  std::cout << "==================" << std::endl;

  try {
    test_p_class_problems();
    test_np_verifiers();
    test_np_complete_problems();
    test_problem_reductions();
    test_p_vs_np();
    demonstrate_real_world_applications();

    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "NP完全性理论是计算复杂性理论的核心内容" << std::endl;
    std::cout << "它帮助我们理解计算问题的内在难度" << std::endl;
    std::cout << "并为算法设计和优化提供了理论指导" << std::endl;

    std::cout << "\n所有测试完成!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "错误: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
