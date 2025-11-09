#include "all_pairs_shortest_path.h"
#include "graph_representation.h"
#include <iostream>
#include <vector>
#include <tuple>

using namespace algorithms;

// 25.1 矩阵乘法方法测试
void test_matrix_multiplication() {
    std::cout << "=== 测试矩阵乘法方法 (25.1) ===" << std::endl;

    // 创建有向图（算法导论图25.1示例）
    AdjacencyListGraph graph(true);

    // 添加节点
    for (int i = 0; i < 5; i++) {
        graph.add_node(i);
    }

    // 添加边（算法导论图25.1的权重）
    graph.add_edge(0, 1, 3);
    graph.add_edge(0, 2, 8);
    graph.add_edge(0, 4, -4);
    graph.add_edge(1, 3, 1);
    graph.add_edge(1, 4, 7);
    graph.add_edge(2, 1, 4);
    graph.add_edge(3, 0, 2);
    graph.add_edge(3, 2, -5);
    graph.add_edge(4, 3, 6);

    std::cout << "图结构:" << std::endl;
    graph.print_adjacency_list();

    // 运行矩阵乘法算法
    auto result = MatrixMultiplicationShortestPath::find_all_pairs_shortest_path(graph);

    if (result.has_negative_cycle) {
        std::cout << "图中存在负权环" << std::endl;
    } else {
        result.print_distances();

        // 打印一些示例路径
        std::cout << "\n示例路径:" << std::endl;
        result.print_path(0, 3);
        result.print_path(1, 2);
        result.print_path(4, 0);
    }

    std::cout << std::endl;
}

// 25.2 Floyd-Warshall算法测试
void test_floyd_warshall() {
    std::cout << "=== 测试Floyd-Warshall算法 (25.2) ===" << std::endl;

    // 创建有向图（算法导论图25.1示例）
    AdjacencyListGraph graph(true);

    // 添加节点
    for (int i = 0; i < 5; i++) {
        graph.add_node(i);
    }

    // 添加边（算法导论图25.1的权重）
    graph.add_edge(0, 1, 3);
    graph.add_edge(0, 2, 8);
    graph.add_edge(0, 4, -4);
    graph.add_edge(1, 3, 1);
    graph.add_edge(1, 4, 7);
    graph.add_edge(2, 1, 4);
    graph.add_edge(3, 0, 2);
    graph.add_edge(3, 2, -5);
    graph.add_edge(4, 3, 6);

    std::cout << "图结构:" << std::endl;
    graph.print_adjacency_list();

    // 运行Floyd-Warshall算法
    auto result = FloydWarshall::find_all_pairs_shortest_path(graph);

    if (result.has_negative_cycle) {
        std::cout << "图中存在负权环" << std::endl;
    } else {
        result.print_distances();

        // 打印一些示例路径
        std::cout << "\n示例路径:" << std::endl;
        result.print_path(0, 3);
        result.print_path(1, 2);
        result.print_path(4, 0);
    }

    std::cout << std::endl;
}

// 25.3 Johnson算法测试
void test_johnson() {
    std::cout << "=== 测试Johnson算法 (25.3) ===" << std::endl;

    // 创建有向图（算法导论图25.1示例）
    AdjacencyListGraph graph(true);

    // 添加节点
    for (int i = 0; i < 5; i++) {
        graph.add_node(i);
    }

    // 添加边（算法导论图25.1的权重）
    graph.add_edge(0, 1, 3);
    graph.add_edge(0, 2, 8);
    graph.add_edge(0, 4, -4);
    graph.add_edge(1, 3, 1);
    graph.add_edge(1, 4, 7);
    graph.add_edge(2, 1, 4);
    graph.add_edge(3, 0, 2);
    graph.add_edge(3, 2, -5);
    graph.add_edge(4, 3, 6);

    std::cout << "图结构:" << std::endl;
    graph.print_adjacency_list();

    // 运行Johnson算法
    auto result = Johnson::find_all_pairs_shortest_path(graph);

    if (result.has_negative_cycle) {
        std::cout << "图中存在负权环" << std::endl;
    } else {
        result.print_distances();

        // 打印一些示例路径
        std::cout << "\n示例路径:" << std::endl;
        result.print_path(0, 3);
        result.print_path(1, 2);
        result.print_path(4, 0);
    }

    std::cout << std::endl;
}

// 负权环测试
void test_negative_cycle() {
    std::cout << "=== 测试负权环检测 ===" << std::endl;

    // 创建包含负权环的图
    AdjacencyListGraph graph(true);

    // 添加节点
    for (int i = 0; i < 4; i++) {
        graph.add_node(i);
    }

    // 添加边（包含负权环）
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 2);
    graph.add_edge(2, 3, -4);
    graph.add_edge(3, 1, 1); // 形成负权环：1->2->3->1
    graph.add_edge(0, 3, 5);

    std::cout << "图结构（包含负权环）:" << std::endl;
    graph.print_adjacency_list();

    // 测试Floyd-Warshall算法
    std::cout << "Floyd-Warshall算法结果:" << std::endl;
    auto result1 = FloydWarshall::find_all_pairs_shortest_path(graph);
    if (result1.has_negative_cycle) {
        std::cout << "检测到负权环" << std::endl;
    } else {
        result1.print_distances();
    }

    // 测试矩阵乘法方法
    std::cout << "\n矩阵乘法方法结果:" << std::endl;
    auto result2 = MatrixMultiplicationShortestPath::find_all_pairs_shortest_path(graph);
    if (result2.has_negative_cycle) {
        std::cout << "检测到负权环" << std::endl;
    } else {
        result2.print_distances();
    }

    // 测试Johnson算法
    std::cout << "\nJohnson算法结果:" << std::endl;
    auto result3 = Johnson::find_all_pairs_shortest_path(graph);
    if (result3.has_negative_cycle) {
        std::cout << "检测到负权环" << std::endl;
    } else {
        result3.print_distances();
    }

    std::cout << std::endl;
}

// 稀疏图测试（Johnson算法优势场景）
void test_sparse_graph() {
    std::cout << "=== 测试稀疏图（Johnson算法优势场景） ===" << std::endl;

    // 创建稀疏图
    AdjacencyListGraph graph(true);

    // 添加节点
    for (int i = 0; i < 6; i++) {
        graph.add_node(i);
    }

    // 添加边（稀疏连接）
    graph.add_edge(0, 1, 2);
    graph.add_edge(0, 2, 5);
    graph.add_edge(1, 3, 3);
    graph.add_edge(2, 4, 1);
    graph.add_edge(3, 5, 4);
    graph.add_edge(4, 5, 2);

    std::cout << "稀疏图结构:" << std::endl;
    graph.print_adjacency_list();

    // 比较不同算法的结果
    std::cout << "Floyd-Warshall算法结果:" << std::endl;
    auto result1 = FloydWarshall::find_all_pairs_shortest_path(graph);
    result1.print_distances();

    std::cout << "\nJohnson算法结果:" << std::endl;
    auto result2 = Johnson::find_all_pairs_shortest_path(graph);
    result2.print_distances();

    std::cout << "\n矩阵乘法方法结果:" << std::endl;
    auto result3 = MatrixMultiplicationShortestPath::find_all_pairs_shortest_path(graph);
    result3.print_distances();

    std::cout << std::endl;
}

// 边界情况测试
void test_edge_cases() {
    std::cout << "=== 边界情况测试 ===" << std::endl;

    // 测试空图
    std::cout << "测试空图:" << std::endl;
    AdjacencyListGraph empty_graph(true);
    auto empty_result = FloydWarshall::find_all_pairs_shortest_path(empty_graph);
    empty_result.print_distances();

    // 测试单节点图
    std::cout << "\n测试单节点图:" << std::endl;
    AdjacencyListGraph single_node_graph(true);
    single_node_graph.add_node(0);
    auto single_result = FloydWarshall::find_all_pairs_shortest_path(single_node_graph);
    single_result.print_distances();

    // 测试不连通图
    std::cout << "\n测试不连通图:" << std::endl;
    AdjacencyListGraph disconnected_graph(true);
    disconnected_graph.add_node(0);
    disconnected_graph.add_node(1);
    disconnected_graph.add_node(2);
    disconnected_graph.add_edge(0, 1, 3);
    // 节点2不连通
    auto disconnected_result = FloydWarshall::find_all_pairs_shortest_path(disconnected_graph);
    disconnected_result.print_distances();

    std::cout << std::endl;
}

int main() {
    std::cout << "算法导论第25章 所有节点对最短路径演示程序" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;

    test_matrix_multiplication();
    test_floyd_warshall();
    test_johnson();
    test_negative_cycle();
    test_sparse_graph();
    test_edge_cases();

    std::cout << "所有测试完成!" << std::endl;

    return 0;
}
