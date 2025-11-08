#ifndef TOPOLOGICAL_SORT_H
#define TOPOLOGICAL_SORT_H

#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "graph_representation.h"

namespace algorithms {

// 拓扑排序类
class TopologicalSort {
private:
    const AdjacencyListGraph& graph;
    
public:
    TopologicalSort(const AdjacencyListGraph& g) : graph(g) {
        if (!graph.is_directed()) {
            throw std::invalid_argument("拓扑排序需要应用于有向图");
        }
    }
    
    // 使用DFS进行拓扑排序（算法导论22.4节方法）
    std::vector<int> topological_sort_dfs() {
        size_t node_count = graph.get_node_count();
        if (node_count == 0) {
            return {};
        }
        
        std::vector<bool> visited(node_count, false);
        std::vector<bool> on_stack(node_count, false); // 用于检测环
        std::stack<int> result_stack;
        
        // 对每个未访问的节点进行DFS
        for (size_t i = 0; i < node_count; i++) {
            if (!visited[i]) {
                if (!dfs_visit(i, visited, on_stack, result_stack)) {
                    throw std::runtime_error("图包含环，无法进行拓扑排序");
                }
            }
        }
        
        // 将栈中的结果转换为向量（逆序）
        std::vector<int> result;
        while (!result_stack.empty()) {
            result.push_back(result_stack.top());
            result_stack.pop();
        }
        
        return result;
    }
    
    // 使用Kahn算法进行拓扑排序（基于入度的算法）
    std::vector<int> topological_sort_kahn() {
        size_t node_count = graph.get_node_count();
        if (node_count == 0) {
            return {};
        }
        
        // 计算每个节点的入度
        std::vector<int> in_degree(node_count, 0);
        for (size_t i = 0; i < node_count; i++) {
            auto neighbors = graph.get_neighbors(i + 1); // 节点ID从1开始
            for (int neighbor : neighbors) {
                in_degree[neighbor - 1]++; // 节点ID从1开始
            }
        }
        
        // 初始化队列（入度为0的节点）
        std::queue<int> zero_in_degree;
        for (size_t i = 0; i < node_count; i++) {
            if (in_degree[i] == 0) {
                zero_in_degree.push(i);
            }
        }
        
        std::vector<int> result;
        size_t visited_count = 0;
        
        // 处理队列中的节点
        while (!zero_in_degree.empty()) {
            int current = zero_in_degree.front();
            zero_in_degree.pop();
            result.push_back(current + 1); // 节点ID从1开始
            visited_count++;
            
            // 减少邻居的入度
            auto neighbors = graph.get_neighbors(current + 1);
            for (int neighbor : neighbors) {
                in_degree[neighbor - 1]--;
                if (in_degree[neighbor - 1] == 0) {
                    zero_in_degree.push(neighbor - 1);
                }
            }
        }
        
        // 检查是否存在环
        if (visited_count != node_count) {
            throw std::runtime_error("图包含环，无法进行拓扑排序");
        }
        
        return result;
    }
    
    // 检查图是否为有向无环图（DAG）
    bool is_dag() {
        try {
            topological_sort_dfs();
            return true;
        } catch (const std::runtime_error&) {
            return false;
        }
    }
    
    // 打印拓扑排序结果
    static void print_topological_order(const std::vector<int>& order) {
        std::cout << "拓扑排序结果: ";
        for (size_t i = 0; i < order.size(); i++) {
            std::cout << order[i];
            if (i < order.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

private:
    // DFS辅助函数（返回false表示检测到环）
    bool dfs_visit(int node_index, std::vector<bool>& visited, 
                   std::vector<bool>& on_stack, std::stack<int>& result_stack) {
        visited[node_index] = true;
        on_stack[node_index] = true;
        
        // 递归访问所有邻居
        auto neighbors = graph.get_neighbors(node_index + 1); // 节点ID从1开始
        for (int neighbor : neighbors) {
            int neighbor_index = neighbor - 1; // 节点ID从1开始
            
            if (!visited[neighbor_index]) {
                if (!dfs_visit(neighbor_index, visited, on_stack, result_stack)) {
                    return false;
                }
            } else if (on_stack[neighbor_index]) {
                // 检测到后向边，存在环
                return false;
            }
        }
        
        on_stack[node_index] = false;
        result_stack.push(node_index + 1); // 节点ID从1开始
        return true;
    }
};

// 算法导论中的经典拓扑排序示例
void print_topological_sort_examples() {
    std::cout << "=== 算法导论拓扑排序经典示例 ===" << std::endl;
    
    // 示例1：算法导论图22.7 - 穿衣顺序
    std::cout << "示例1: 穿衣顺序（算法导论图22.7）" << std::endl;
    AdjacencyListGraph dressing_graph(true);
    
    // 添加衣物节点
    std::vector<std::string> clothing_items = {
        "内裤", "袜子", "裤子", "鞋子", "衬衫", "腰带", "领带", "外套", "手表"
    };
    
    for (int i = 1; i <= 9; i++) {
        dressing_graph.add_node(i, clothing_items[i-1]);
    }
    
    // 添加依赖关系（穿衣顺序）
    dressing_graph.add_edge(1, 3); // 内裤 -> 裤子
    dressing_graph.add_edge(1, 4); // 内裤 -> 鞋子
    dressing_graph.add_edge(2, 4); // 袜子 -> 鞋子
    dressing_graph.add_edge(3, 4); // 裤子 -> 鞋子
    dressing_graph.add_edge(3, 6); // 裤子 -> 腰带
    dressing_graph.add_edge(5, 6); // 衬衫 -> 腰带
    dressing_graph.add_edge(5, 7); // 衬衫 -> 领带
    dressing_graph.add_edge(6, 8);  // 腰带 -> 外套
    dressing_graph.add_edge(7, 8); // 领带 -> 外套
    
    dressing_graph.print_adjacency_list();
    
    TopologicalSort ts(dressing_graph);
    auto order = ts.topological_sort_dfs();
    ts.print_topological_order(order);
    
    // 示例2：课程依赖关系
    std::cout << "\n示例2: 课程依赖关系" << std::endl;
    AdjacencyListGraph course_graph(true);
    
    // 添加课程节点
    std::vector<std::string> courses = {
        "微积分", "线性代数", "离散数学", "数据结构", "算法导论",
        "操作系统", "编译原理", "计算机网络"
    };
    
    for (int i = 1; i <= 8; i++) {
        course_graph.add_node(i, courses[i-1]);
    }
    
    // 添加课程依赖关系
    course_graph.add_edge(1, 4); // 微积分 -> 数据结构
    course_graph.add_edge(2, 4); // 线性代数 -> 数据结构
    course_graph.add_edge(3, 4); // 离散数学 -> 数据结构
    course_graph.add_edge(4, 5); // 数据结构 -> 算法导论
    course_graph.add_edge(4, 6); // 数据结构 -> 操作系统
    course_graph.add_edge(4, 7); // 数据结构 -> 编译原理
    course_graph.add_edge(4, 8); // 数据结构 -> 计算机网络
    
    course_graph.print_adjacency_list();
    
    TopologicalSort ts2(course_graph);
    auto order2 = ts2.topological_sort_kahn();
    ts2.print_topological_order(order2);
}

} // namespace algorithms

#endif // TOPOLOGICAL_SORT_H