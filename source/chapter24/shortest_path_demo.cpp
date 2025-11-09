#include "shortest_path.h"
#include "graph_representation.h"
#include <iostream>
#include <vector>
#include <tuple>

using namespace algorithms;

// 24.1 Bellman-Ford算法测试
void test_bellman_ford() {
    std::cout << "=== 测试Bellman-Ford算法 (24.1) ===" << std::endl;

    // 创建有向图（算法导论图24.4示例）
    AdjacencyListGraph graph(true);

    // 添加节点
    for (int i = 0; i < 5; i++) {
        graph.add_node(i);
    }

    // 添加边（算法导论图24.4的权重）
    graph.add_edge(0, 1, 6);  // s->t: 6
    graph.add_edge(0, 2, 7);  // s->x: 7
    graph.add_edge(1, 2, 8);  // t->x: 8
    graph.add_edge(1, 3, 5);  // t->y: 5
    graph.add_edge(1, 4, -4); // t->z: -4
    graph.add_edge(2, 3, -3); // x->y: -3
    graph.add_edge(2, 4, 9);  // x->z: 9
    graph.add_edge(3, 1, -2); // y->t: -2
    graph.add_edge(4, 0, 2);  // z->s: 2
    graph.add_edge(4, 3, 7);  // z->y: 7

    std::cout << "图结构:" << std::endl;
    graph.print_adjacency_list();

    // 运行Bellman-Ford算法
    auto result = BellmanFord::find_shortest_path(graph, 0);

    // 打印结果
    print_shortest_path_result(result, 0);

    std::cout << std::endl;
}

// 24.2 有向无环图中的单源最短路径问题测试
void test_dag_shortest_path() {
    std::cout << "=== 测试有向无环图最短路径 (24.2) ===" << std::endl;

    // 创建有向无环图（算法导论图24.5示例）
    AdjacencyListGraph graph(true);

    // 添加节点
    for (int i = 0; i < 6; i++) {
        graph.add_node(i);
    }

    // 添加边
    graph.add_edge(0, 1, 5);  // r->s: 5
    graph.add_edge(0, 2, 3);  // r->t: 3
    graph.add_edge(1, 2, 2);  // s->t: 2
    graph.add_edge(1, 3, 6);  // s->x: 6
    graph.add_edge(2, 3, 7);  // t->x: 7
    graph.add_edge(2, 4, 4);  // t->y: 4
    graph.add_edge(2, 5, 2);  // t->z: 2
    graph.add_edge(3, 4, -1); // x->y: -1
    graph.add_edge(3, 5, 1);  // x->z: 1
    graph.add_edge(4, 5, -2); // y->z: -2

    std::cout << "有向无环图结构:" << std::endl;
    graph.print_adjacency_list();

    // 运行DAG最短路径算法
    auto result = DAGShortestPath::find_shortest_path(graph, 0);

    // 打印结果
    print_shortest_path_result(result, 0);

    std::cout << std::endl;
}

// 24.3 Dijkstra算法测试
void test_dijkstra() {
    std::cout << "=== 测试Dijkstra算法 (24.3) ===" << std::endl;

    // 创建有向图（算法导论图24.6示例）
    AdjacencyListGraph graph(true);

    // 添加节点
    for (int i = 0; i < 5; i++) {
        graph.add_node(i);
    }

    // 添加边（非负权重）
    graph.add_edge(0, 1, 10); // s->t: 10
    graph.add_edge(0, 2, 5);  // s->x: 5
    graph.add_edge(1, 2, 2);  // t->x: 2
    graph.add_edge(1, 3, 1);  // t->y: 1
    graph.add_edge(2, 1, 3);  // x->t: 3
    graph.add_edge(2, 3, 9);  // x->y: 9
    graph.add_edge(2, 4, 2);  // x->z: 2
    graph.add_edge(3, 4, 4);  // y->z: 4
    graph.add_edge(4, 0, 7);  // z->s: 7
    graph.add_edge(4, 3, 6);  // z->y: 6

    std::cout << "图结构（非负权重）:" << std::endl;
    graph.print_adjacency_list();

    // 运行Dijkstra算法
    auto result = Dijkstra::find_shortest_path(graph, 0);

    // 打印结果
    print_shortest_path_result(result, 0);

    std::cout << std::endl;
}

// 24.4 差分约束和最短路径测试
void test_difference_constraints() {
    std::cout << "=== 测试差分约束系统 (24.4) ===" << std::endl;

    // 差分约束系统示例
    // x1 - x2 <= 3
    // x2 - x3 <= -2
    // x1 - x3 <= 2
    // x3 - x4 <= 1
    // x4 - x1 <= -1

    std::vector<std::tuple<int, int, int>> constraints = {
        {1, 2, 3},   // x1 - x2 <= 3
        {2, 3, -2},  // x2 - x3 <= -2
        {1, 3, 2},   // x1 - x3 <= 2
        {3, 4, 1},   // x3 - x4 <= 1
        {4, 1, -1}   // x4 - x1 <= -1
    };

    std::cout << "差分约束系统:" << std::endl;
    for (const auto& constraint : constraints) {
        int u = std::get<0>(constraint);
        int v = std::get<1>(constraint);
        int w = std::get<2>(constraint);
        std::cout << "x" << u << " - x" << v << " <= " << w << std::endl;
    }

    // 求解差分约束系统
    auto solution = DifferenceConstraints::solve(constraints, 4);

    if (solution.empty()) {
        std::cout << "差分约束系统无解（存在负权环）" << std::endl;
    } else {
        std::cout << "差分约束系统的解:" << std::endl;
        for (int i = 0; i < solution.size(); i++) {
            std::cout << "x" << (i + 1) << " = " << solution[i] << std::endl;
        }
    }

    std::cout << std::endl;
}

// 算法导论经典示例测试
void test_clrs_examples() {
    std::cout << "=== 算法导论经典示例测试 ===" << std::endl;

    // 示例1：图24.1 - 最短路径树
    std::cout << "示例1 (图24.1 - 最短路径树):" << std::endl;
    AdjacencyListGraph example1(true);

    for (int i = 0; i < 5; i++) {
        example1.add_node(i);
    }

    example1.add_edge(0, 1, 10);
    example1.add_edge(0, 2, 5);
    example1.add_edge(1, 2, 2);
    example1.add_edge(1, 3, 1);
    example1.add_edge(2, 1, 3);
    example1.add_edge(2, 3, 9);
    example1.add_edge(2, 4, 2);
    example1.add_edge(3, 4, 4);
    example1.add_edge(4, 0, 7);
    example1.add_edge(4, 3, 6);

    auto result1 = Dijkstra::find_shortest_path(example1, 0);
    print_shortest_path_result(result1, 0);

    // 示例2：图24.4 - Bellman-Ford算法
    std::cout << "\n示例2 (图24.4 - Bellman-Ford算法):" << std::endl;
    AdjacencyListGraph example2(true);

    for (int i = 0; i < 5; i++) {
        example2.add_node(i);
    }

    example2.add_edge(0, 1, 6);
    example2.add_edge(0, 2, 7);
    example2.add_edge(1, 2, 8);
    example2.add_edge(1, 3, 5);
    example2.add_edge(1, 4, -4);
    example2.add_edge(2, 3, -3);
    example2.add_edge(2, 4, 9);
    example2.add_edge(3, 1, -2);
    example2.add_edge(4, 0, 2);
    example2.add_edge(4, 3, 7);

    auto result2 = BellmanFord::find_shortest_path(example2, 0);
    print_shortest_path_result(result2, 0);

    std::cout << std::endl;
}

// 边界情况测试
void test_edge_cases() {
    std::cout << "=== 边界情况测试 ===" << std::endl;

    // 测试空图
    std::cout << "测试空图:" << std::endl;
    AdjacencyListGraph empty_graph(true);
    auto empty_result = BellmanFord::find_shortest_path(empty_graph, 0);
    print_shortest_path_result(empty_result, 0);

    // 测试单节点图
    std::cout << "\n测试单节点图:" << std::endl;
    AdjacencyListGraph single_node_graph(true);
    single_node_graph.add_node(0);
    auto single_result = Dijkstra::find_shortest_path(single_node_graph, 0);
    print_shortest_path_result(single_result, 0);

    // 测试不可达节点
    std::cout << "\n测试不可达节点:" << std::endl;
    AdjacencyListGraph unreachable_graph(true);
    unreachable_graph.add_node(0);
    unreachable_graph.add_node(1);
    unreachable_graph.add_node(2);
    unreachable_graph.add_edge(0, 1, 5);
    // 节点2不可达
    auto unreachable_result = Dijkstra::find_shortest_path(unreachable_graph, 0);
    print_shortest_path_result(unreachable_result, 0);

    std::cout << std::endl;
}

// 性能比较测试
void test_performance_comparison() {
    std::cout << "=== 性能比较测试 ===" << std::endl;

    // 创建测试图
    AdjacencyListGraph test_graph(true);

    // 添加节点
    for (int i = 0; i < 10; i++) {
        test_graph.add_node(i);
    }

    // 添加随机边（非负权重）
    test_graph.add_edge(0, 1, 4);
    test_graph.add_edge(0, 2, 2);
    test_graph.add_edge(1, 3, 3);
    test_graph.add_edge(1, 4, 6);
    test_graph.add_edge(2, 3, 1);
    test_graph.add_edge(2, 4, 5);
    test_graph.add_edge(3, 5, 2);
    test_graph.add_edge(4, 5, 4);
    test_graph.add_edge(5, 6, 3);
    test_graph.add_edge(5, 7, 2);
    test_graph.add_edge(6, 8, 1);
    test_graph.add_edge(7, 8, 5);
    test_graph.add_edge(8, 9, 2);

    std::cout << "测试图结构:" << std::endl;
    test_graph.print_adjacency_list();

    // 比较不同算法的结果
    std::cout << "\nDijkstra算法结果:" << std::endl;
    auto dijkstra_result = Dijkstra::find_shortest_path(test_graph, 0);
    print_shortest_path_result(dijkstra_result, 0);

    std::cout << "\nBellman-Ford算法结果:" << std::endl;
    auto bellman_result = BellmanFord::find_shortest_path(test_graph, 0);
    print_shortest_path_result(bellman_result, 0);

    std::cout << "\nDAG最短路径算法结果:" << std::endl;
    auto dag_result = DAGShortestPath::find_shortest_path(test_graph, 0);
    print_shortest_path_result(dag_result, 0);

    std::cout << std::endl;
}

int main() {
    std::cout << "算法导论第24章 单源最短路径演示程序" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << std::endl;

    test_bellman_ford();
    test_dag_shortest_path();
    test_dijkstra();
    test_difference_constraints();
    test_clrs_examples();
    test_edge_cases();
    test_performance_comparison();

    std::cout << "所有测试完成!" << std::endl;

    return 0;
}
