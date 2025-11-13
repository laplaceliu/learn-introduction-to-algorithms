#include "max_flow.h"
#include <iostream>
#include <vector>

using namespace algorithms;

void test_ford_fulkerson() {
  std::cout << "=== Ford-Fulkerson算法测试 ===" << std::endl;

  // 测试1：简单流网络
  std::cout << "测试1: 简单流网络" << std::endl;
  FlowNetwork network1(4, 0, 3); // 4个节点，源点0，汇点3

  network1.add_edge(0, 1, 10);
  network1.add_edge(0, 2, 10);
  network1.add_edge(1, 2, 2);
  network1.add_edge(1, 3, 15);
  network1.add_edge(2, 3, 10);

  network1.print_network();

  auto result1 = FordFulkerson::compute_max_flow(network1);
  print_max_flow_result(result1, 0, 3);

  // 测试2：算法导论图26.6示例
  std::cout << "\n测试2: 算法导论图26.6示例" << std::endl;
  FlowNetwork network2(6, 0, 5);

  network2.add_edge(0, 1, 16);
  network2.add_edge(0, 2, 13);
  network2.add_edge(1, 3, 12);
  network2.add_edge(2, 1, 4);
  network2.add_edge(2, 4, 14);
  network2.add_edge(3, 2, 9);
  network2.add_edge(3, 5, 20);
  network2.add_edge(4, 3, 7);
  network2.add_edge(4, 5, 4);

  network2.print_network();

  auto result2 = FordFulkerson::compute_max_flow(network2);
  print_max_flow_result(result2, 0, 5);

  std::cout << std::endl;
}

void test_bipartite_matching() {
  std::cout << "=== 二分图匹配测试 ===" << std::endl;

  // 测试1：简单二分图
  std::cout << "测试1: 简单二分图" << std::endl;
  std::vector<std::vector<int>> graph1 = {
      {1, 0, 1}, // 左部节点0
      {1, 1, 0}, // 左部节点1
      {0, 1, 1}  // 左部节点2
  };

  BipartiteMatching::print_matching(graph1, 3, 3);

  // 测试2：算法导论图26.8示例
  std::cout << "\n测试2: 算法导论图26.8示例" << std::endl;
  std::vector<std::vector<int>> graph2 = {
      {1, 1, 0, 0}, // L1
      {0, 0, 1, 0}, // L2
      {0, 1, 0, 0}, // L3
      {0, 0, 1, 1}, // L4
      {0, 0, 0, 1}  // L5
  };

  BipartiteMatching::print_matching(graph2, 5, 4);

  std::cout << std::endl;
}

void test_push_relabel() {
  std::cout << "=== 推送-重贴标签算法测试 ===" << std::endl;

  // 测试1：简单网络
  std::cout << "测试1: 简单网络" << std::endl;
  FlowNetwork network1(4, 0, 3);

  network1.add_edge(0, 1, 3);
  network1.add_edge(0, 2, 3);
  network1.add_edge(1, 2, 2);
  network1.add_edge(1, 3, 3);
  network1.add_edge(2, 3, 3);

  network1.print_network();

  auto result1 = PushRelabel::compute_max_flow(network1);
  print_max_flow_result(result1, 0, 3);

  // 测试2：复杂网络
  std::cout << "\n测试2: 复杂网络" << std::endl;
  FlowNetwork network2(5, 0, 4);

  network2.add_edge(0, 1, 10);
  network2.add_edge(0, 2, 5);
  network2.add_edge(1, 2, 15);
  network2.add_edge(1, 3, 10);
  network2.add_edge(2, 3, 10);
  network2.add_edge(2, 4, 10);
  network2.add_edge(3, 4, 10);

  network2.print_network();

  auto result2 = PushRelabel::compute_max_flow(network2);
  print_max_flow_result(result2, 0, 4);

  std::cout << std::endl;
}

void test_edge_cases() {
  std::cout << "=== 边界情况测试 ===" << std::endl;

  // 测试1：单边网络
  std::cout << "测试1: 单边网络" << std::endl;
  FlowNetwork network1(2, 0, 1);
  network1.add_edge(0, 1, 5);

  network1.print_network();

  auto result1 = FordFulkerson::compute_max_flow(network1);
  print_max_flow_result(result1, 0, 1);

  // 测试2：无路径网络
  std::cout << "\n测试2: 无路径网络" << std::endl;
  FlowNetwork network2(3, 0, 2);
  network2.add_edge(0, 1, 5);
  // 没有从节点1到节点2的边

  network2.print_network();

  auto result2 = FordFulkerson::compute_max_flow(network2);
  print_max_flow_result(result2, 0, 2);

  // 测试3：多路径网络
  std::cout << "\n测试3: 多路径网络" << std::endl;
  FlowNetwork network3(4, 0, 3);
  network3.add_edge(0, 1, 3);
  network3.add_edge(0, 2, 3);
  network3.add_edge(1, 3, 3);
  network3.add_edge(2, 3, 3);

  network3.print_network();

  auto result3 = FordFulkerson::compute_max_flow(network3);
  print_max_flow_result(result3, 0, 3);

  std::cout << std::endl;
}

void compare_algorithms() {
  std::cout << "=== 算法比较测试 ===" << std::endl;

  // 使用相同的网络比较不同算法
  FlowNetwork network(4, 0, 3);
  network.add_edge(0, 1, 10);
  network.add_edge(0, 2, 5);
  network.add_edge(1, 2, 15);
  network.add_edge(1, 3, 10);
  network.add_edge(2, 3, 10);

  network.print_network();

  std::cout << "Ford-Fulkerson算法结果:" << std::endl;
  auto result_ff = FordFulkerson::compute_max_flow(network);
  print_max_flow_result(result_ff, 0, 3);

  std::cout << "\n推送-重贴标签算法结果:" << std::endl;
  auto result_pr = PushRelabel::compute_max_flow(network);
  print_max_flow_result(result_pr, 0, 3);

  std::cout << "算法结果是否一致: "
            << (result_ff.max_flow_value == result_pr.max_flow_value ? "是"
                                                                     : "否")
            << std::endl;

  std::cout << std::endl;
}

int main() {
  std::cout << "第26章 最大流算法演示程序" << std::endl;
  std::cout << "==========================" << std::endl;

  try {
    test_ford_fulkerson();
    test_bipartite_matching();
    test_push_relabel();
    test_edge_cases();
    compare_algorithms();

    // 打印算法导论经典示例
    print_max_flow_examples();

    std::cout << "所有测试完成！" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "错误: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
