#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "topological_sort.h"

using namespace algorithms;

void test_classic_examples() {
    std::cout << "=== 测试经典拓扑排序示例 ===" << std::endl;

    print_topological_sort_examples();
    std::cout << std::endl;
}

void test_dfs_topological_sort() {
    std::cout << "=== 测试DFS拓扑排序算法 ===" << std::endl;

    // 创建有向无环图（DAG）
    AdjacencyListGraph dag(true);

    // 添加节点
    for (int i = 1; i <= 7; i++) {
        dag.add_node(i, "Node" + std::to_string(i));
    }

    // 添加边（确保无环）
    dag.add_edge(1, 2);
    dag.add_edge(1, 3);
    dag.add_edge(2, 4);
    dag.add_edge(2, 5);
    dag.add_edge(3, 6);
    dag.add_edge(4, 7);
    dag.add_edge(5, 7);
    dag.add_edge(6, 7);

    dag.print_adjacency_list();

    TopologicalSort ts(dag);

    // 测试DFS拓扑排序
    auto dfs_order = ts.topological_sort_dfs();
    std::cout << "DFS拓扑排序结果: ";
    TopologicalSort::print_topological_order(dfs_order);

    // 验证拓扑排序的正确性
    std::cout << "\n验证拓扑排序正确性:" << std::endl;
    bool valid = true;
    for (size_t i = 0; i < dfs_order.size(); i++) {
        for (size_t j = i + 1; j < dfs_order.size(); j++) {
            // 检查是否所有边都从排序靠前的节点指向排序靠后的节点
            auto neighbors = dag.get_neighbors(dfs_order[j]);
            if (std::find(neighbors.begin(), neighbors.end(), dfs_order[i]) != neighbors.end()) {
                std::cout << "  错误: 边 " << dfs_order[j] << " -> " << dfs_order[i]
                         << " 违反了拓扑顺序" << std::endl;
                valid = false;
            }
        }
    }

    if (valid) {
        std::cout << "  拓扑排序验证通过" << std::endl;
    }

    std::cout << std::endl;
}

void test_kahn_topological_sort() {
    std::cout << "=== 测试Kahn拓扑排序算法 ===" << std::endl;

    // 创建有向无环图（DAG）
    AdjacencyListGraph dag(true);

    // 添加节点
    for (int i = 1; i <= 6; i++) {
        dag.add_node(i, "Node" + std::to_string(i));
    }

    // 添加边（确保无环）
    dag.add_edge(1, 2);
    dag.add_edge(1, 3);
    dag.add_edge(2, 4);
    dag.add_edge(3, 4);
    dag.add_edge(3, 5);
    dag.add_edge(4, 6);
    dag.add_edge(5, 6);

    dag.print_adjacency_list();

    TopologicalSort ts(dag);

    // 测试Kahn拓扑排序
    auto kahn_order = ts.topological_sort_kahn();
    std::cout << "Kahn拓扑排序结果: ";
    TopologicalSort::print_topological_order(kahn_order);

    // 验证拓扑排序的正确性
    std::cout << "\n验证拓扑排序正确性:" << std::endl;
    bool valid = true;
    for (size_t i = 0; i < kahn_order.size(); i++) {
        for (size_t j = i + 1; j < kahn_order.size(); j++) {
            // 检查是否所有边都从排序靠前的节点指向排序靠后的节点
            auto neighbors = dag.get_neighbors(kahn_order[j]);
            if (std::find(neighbors.begin(), neighbors.end(), kahn_order[i]) != neighbors.end()) {
                std::cout << "  错误: 边 " << kahn_order[j] << " -> " << kahn_order[i]
                         << " 违反了拓扑顺序" << std::endl;
                valid = false;
            }
        }
    }

    if (valid) {
        std::cout << "  拓扑排序验证通过" << std::endl;
    }

    std::cout << std::endl;
}

void test_cycle_detection() {
    std::cout << "=== 测试环检测 ===" << std::endl;

    // 创建包含环的有向图
    AdjacencyListGraph cyclic_graph(true);

    // 添加节点
    for (int i = 1; i <= 4; i++) {
        cyclic_graph.add_node(i, "Node" + std::to_string(i));
    }

    // 添加边（创建环：1->2->3->4->1）
    cyclic_graph.add_edge(1, 2);
    cyclic_graph.add_edge(2, 3);
    cyclic_graph.add_edge(3, 4);
    cyclic_graph.add_edge(4, 1);

    cyclic_graph.print_adjacency_list();

    TopologicalSort ts(cyclic_graph);

    // 测试是否为DAG
    bool is_dag = ts.is_dag();
    std::cout << "图是否为有向无环图(DAG): " << (is_dag ? "是" : "否") << std::endl;

    // 测试拓扑排序（应该抛出异常）
    try {
        auto order = ts.topological_sort_dfs();
        std::cout << "错误: 应该检测到环并抛出异常" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "正确检测到环: " << e.what() << std::endl;
    }

    std::cout << std::endl;
}

void test_comparison_between_algorithms() {
    std::cout << "=== 两种拓扑排序算法对比 ===" << std::endl;

    // 创建相同的DAG
    AdjacencyListGraph dag(true);

    // 添加节点
    for (int i = 1; i <= 8; i++) {
        dag.add_node(i, "Node" + std::to_string(i));
    }

    // 添加边
    dag.add_edge(1, 2);
    dag.add_edge(1, 3);
    dag.add_edge(2, 4);
    dag.add_edge(2, 5);
    dag.add_edge(3, 6);
    dag.add_edge(4, 7);
    dag.add_edge(5, 7);
    dag.add_edge(6, 7);
    dag.add_edge(7, 8);

    dag.print_adjacency_list();

    TopologicalSort ts(dag);

    // 测试DFS算法
    auto dfs_order = ts.topological_sort_dfs();
    std::cout << "DFS算法结果: ";
    TopologicalSort::print_topological_order(dfs_order);

    // 测试Kahn算法
    auto kahn_order = ts.topological_sort_kahn();
    std::cout << "Kahn算法结果: ";
    TopologicalSort::print_topological_order(kahn_order);

    // 比较两种算法的结果
    std::cout << "\n算法对比:" << std::endl;
    std::cout << "  DFS算法特点:" << std::endl;
    std::cout << "    - 基于深度优先搜索" << std::endl;
    std::cout << "    - 时间复杂度: O(V + E)" << std::endl;
    std::cout << "    - 空间复杂度: O(V)" << std::endl;
    std::cout << "    - 自动检测环" << std::endl;

    std::cout << "  Kahn算法特点:" << std::endl;
    std::cout << "    - 基于入度计算" << std::endl;
    std::cout << "    - 时间复杂度: O(V + E)" << std::endl;
    std::cout << "    - 空间复杂度: O(V)" << std::endl;
    std::cout << "    - 更容易理解" << std::endl;

    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;

    // 测试空图
    AdjacencyListGraph empty_graph(true);
    TopologicalSort ts_empty(empty_graph);

    auto empty_order = ts_empty.topological_sort_dfs();
    std::cout << "空图的拓扑排序: ";
    if (empty_order.empty()) {
        std::cout << "空序列" << std::endl;
    } else {
        std::cout << "错误: 应该为空序列" << std::endl;
    }

    // 测试单节点图
    AdjacencyListGraph single_node_graph(true);
    single_node_graph.add_node(1);

    TopologicalSort ts_single(single_node_graph);
    auto single_order = ts_single.topological_sort_dfs();
    std::cout << "单节点图的拓扑排序: ";
    TopologicalSort::print_topological_order(single_order);

    // 测试线性链
    AdjacencyListGraph linear_graph(true);
    for (int i = 1; i <= 5; i++) {
        linear_graph.add_node(i);
    }
    for (int i = 1; i <= 4; i++) {
        linear_graph.add_edge(i, i + 1);
    }

    TopologicalSort ts_linear(linear_graph);
    auto linear_order = ts_linear.topological_sort_dfs();
    std::cout << "线性链的拓扑排序: ";
    TopologicalSort::print_topological_order(linear_order);

    std::cout << std::endl;
}

void test_algorithm_correctness() {
    std::cout << "=== 测试算法正确性 ===" << std::endl;

    // 创建复杂的DAG
    AdjacencyListGraph complex_dag(true);

    // 添加节点
    for (int i = 1; i <= 10; i++) {
        complex_dag.add_node(i);
    }

    // 添加复杂的依赖关系
    complex_dag.add_edge(1, 2);
    complex_dag.add_edge(1, 3);
    complex_dag.add_edge(2, 4);
    complex_dag.add_edge(2, 5);
    complex_dag.add_edge(3, 5);
    complex_dag.add_edge(3, 6);
    complex_dag.add_edge(4, 7);
    complex_dag.add_edge(5, 7);
    complex_dag.add_edge(5, 8);
    complex_dag.add_edge(6, 8);
    complex_dag.add_edge(7, 9);
    complex_dag.add_edge(8, 9);
    complex_dag.add_edge(9, 10);

    complex_dag.print_adjacency_list();

    TopologicalSort ts(complex_dag);

    // 测试两种算法的一致性
    auto dfs_order = ts.topological_sort_dfs();
    auto kahn_order = ts.topological_sort_kahn();

    std::cout << "DFS算法结果: ";
    TopologicalSort::print_topological_order(dfs_order);

    std::cout << "Kahn算法结果: ";
    TopologicalSort::print_topological_order(kahn_order);

    // 验证两种算法结果是否都是有效的拓扑排序
    bool dfs_valid = true;
    bool kahn_valid = true;

    for (size_t i = 0; i < dfs_order.size(); i++) {
        for (size_t j = i + 1; j < dfs_order.size(); j++) {
            auto neighbors = complex_dag.get_neighbors(dfs_order[j]);
            if (std::find(neighbors.begin(), neighbors.end(), dfs_order[i]) != neighbors.end()) {
                dfs_valid = false;
            }
        }
    }

    for (size_t i = 0; i < kahn_order.size(); i++) {
        for (size_t j = i + 1; j < kahn_order.size(); j++) {
            auto neighbors = complex_dag.get_neighbors(kahn_order[j]);
            if (std::find(neighbors.begin(), neighbors.end(), kahn_order[i]) != neighbors.end()) {
                kahn_valid = false;
            }
        }
    }

    std::cout << "\n正确性验证:" << std::endl;
    std::cout << "  DFS算法结果是否有效: " << (dfs_valid ? "是" : "否") << std::endl;
    std::cout << "  Kahn算法结果是否有效: " << (kahn_valid ? "是" : "否") << std::endl;

    if (dfs_valid && kahn_valid) {
        std::cout << "  两种算法都产生了有效的拓扑排序" << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    std::cout << "第22.4章 拓扑排序演示程序" << std::endl;
    std::cout << "============================" << std::endl;

    test_classic_examples();
    test_dfs_topological_sort();
    test_kahn_topological_sort();
    test_cycle_detection();
    test_comparison_between_algorithms();
    test_edge_cases();
    test_algorithm_correctness();

    std::cout << "所有测试完成！" << std::endl;

    return 0;
}
