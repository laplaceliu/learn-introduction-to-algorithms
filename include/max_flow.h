#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include "graph_representation.h"
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stack>
#include <functional>

namespace algorithms {

// 最大流结果结构
struct MaxFlowResult {
    int max_flow_value;                    // 最大流值
    std::vector<std::vector<int>> flow;    // 流矩阵
    std::vector<std::vector<int>> capacity; // 容量矩阵
    std::vector<std::vector<int>> residual; // 残量矩阵
    
    MaxFlowResult(int n) : max_flow_value(0), 
                          flow(n, std::vector<int>(n, 0)),
                          capacity(n, std::vector<int>(n, 0)),
                          residual(n, std::vector<int>(n, 0)) {}
};

// 26.1 流网络
class FlowNetwork {
private:
    AdjacencyMatrixGraph graph;
    std::vector<std::vector<int>> capacities;
    int source, sink;
    
public:
    FlowNetwork(int n, int s, int t) : graph(true), capacities(n, std::vector<int>(n, 0)), source(s), sink(t) {
        // 添加节点
        for (int i = 0; i < n; i++) {
            graph.add_node(i);
        }
    }
    
    // 添加边（容量）
    void add_edge(int u, int v, int capacity) {
        if (capacity < 0) {
            throw std::invalid_argument("Capacity cannot be negative");
        }
        
        try {
            graph.add_edge(u, v, capacity);
            capacities[u][v] = capacity;
        } catch (const std::invalid_argument& e) {
            // 边已存在，更新容量
            capacities[u][v] = capacity;
        }
    }
    
    // 获取容量
    int get_capacity(int u, int v) const {
        return capacities[u][v];
    }
    
    // 获取源点
    int get_source() const { return source; }
    
    // 获取汇点
    int get_sink() const { return sink; }
    
    // 获取节点数量
    int get_node_count() const { return graph.get_node_count(); }
    
    // 获取图的引用
    const AdjacencyMatrixGraph& get_graph() const { return graph; }
    
    // 打印流网络
    void print_network() const {
        std::cout << "流网络 (源点: " << source << ", 汇点: " << sink << "):" << std::endl;
        std::cout << "容量矩阵:" << std::endl;
        
        int n = get_node_count();
        std::cout << "    ";
        for (int i = 0; i < n; i++) {
            std::cout << std::setw(4) << i;
        }
        std::cout << std::endl;
        
        for (int i = 0; i < n; i++) {
            std::cout << std::setw(4) << i;
            for (int j = 0; j < n; j++) {
                std::cout << std::setw(4) << capacities[i][j];
            }
            std::cout << std::endl;
        }
    }
};

// 26.2 Ford-Fulkerson方法
class FordFulkerson {
private:
    // BFS查找增广路径
    static bool bfs(const std::vector<std::vector<int>>& residual, int source, int sink, std::vector<int>& parent) {
        int n = residual.size();
        std::vector<bool> visited(n, false);
        std::queue<int> q;
        
        q.push(source);
        visited[source] = true;
        parent[source] = -1;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v = 0; v < n; v++) {
                if (!visited[v] && residual[u][v] > 0) {
                    if (v == sink) {
                        parent[v] = u;
                        return true;
                    }
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }
        
        return false;
    }
    
public:
    static MaxFlowResult compute_max_flow(const FlowNetwork& network) {
        int n = network.get_node_count();
        int source = network.get_source();
        int sink = network.get_sink();
        
        MaxFlowResult result(n);
        
        // 初始化容量矩阵
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result.capacity[i][j] = network.get_capacity(i, j);
                result.residual[i][j] = network.get_capacity(i, j);
            }
        }
        
        std::vector<int> parent(n);
        
        // Ford-Fulkerson算法
        while (bfs(result.residual, source, sink, parent)) {
            // 找到增广路径上的最小残量
            int path_flow = std::numeric_limits<int>::max();
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                path_flow = std::min(path_flow, result.residual[u][v]);
            }
            
            // 更新残量网络
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                result.residual[u][v] -= path_flow;
                result.residual[v][u] += path_flow;
                result.flow[u][v] += path_flow;
            }
            
            result.max_flow_value += path_flow;
        }
        
        return result;
    }
};

// 26.3 最大二分匹配
class BipartiteMatching {
public:
    // 使用Ford-Fulkerson解决最大二分匹配问题
    static int find_max_matching(const std::vector<std::vector<int>>& bipartite_graph, 
                                 int left_size, int right_size) {
        // 构建流网络：源点 -> 左部节点 -> 右部节点 -> 汇点
        int n = left_size + right_size + 2; // +2 for source and sink
        int source = 0;
        int sink = n - 1;
        
        FlowNetwork network(n, source, sink);
        
        // 连接源点到所有左部节点
        for (int i = 1; i <= left_size; i++) {
            network.add_edge(source, i, 1);
        }
        
        // 连接左部节点到右部节点
        for (int i = 0; i < left_size; i++) {
            for (int j = 0; j < right_size; j++) {
                if (bipartite_graph[i][j] == 1) {
                    network.add_edge(i + 1, left_size + j + 1, 1);
                }
            }
        }
        
        // 连接所有右部节点到汇点
        for (int j = 1; j <= right_size; j++) {
            network.add_edge(left_size + j, sink, 1);
        }
        
        // 计算最大流
        auto result = FordFulkerson::compute_max_flow(network);
        return result.max_flow_value;
    }
    
    // 打印二分图匹配结果
    static void print_matching(const std::vector<std::vector<int>>& bipartite_graph,
                              int left_size, int right_size) {
        std::cout << "二分图:" << std::endl;
        std::cout << "左部节点: 0-" << (left_size - 1) << std::endl;
        std::cout << "右部节点: 0-" << (right_size - 1) << std::endl;
        
        std::cout << "边:" << std::endl;
        for (int i = 0; i < left_size; i++) {
            for (int j = 0; j < right_size; j++) {
                if (bipartite_graph[i][j] == 1) {
                    std::cout << "(" << i << ", " << j << ") ";
                }
            }
        }
        std::cout << std::endl;
        
        int max_matching = find_max_matching(bipartite_graph, left_size, right_size);
        std::cout << "最大匹配数: " << max_matching << std::endl;
    }
};

// 26.4 推送-重贴标签算法（Push-Relabel）
class PushRelabel {
private:
    static void initialize_preflow(const std::vector<std::vector<int>>& capacity,
                                  std::vector<std::vector<int>>& flow,
                                  std::vector<int>& height,
                                  std::vector<int>& excess,
                                  int source, int n) {
        height.assign(n, 0);
        excess.assign(n, 0);
        
        // 初始化高度和超额流
        height[source] = n;
        
        // 从源点推送流到所有邻居
        for (int v = 0; v < n; v++) {
            if (capacity[source][v] > 0) {
                flow[source][v] = capacity[source][v];
                flow[v][source] = -flow[source][v];
                excess[v] = flow[source][v];
                excess[source] -= flow[source][v];
            }
        }
    }
    
    static void push(int u, int v, const std::vector<std::vector<int>>& capacity,
                     std::vector<std::vector<int>>& flow,
                     std::vector<int>& excess) {
        int residual_capacity = capacity[u][v] - flow[u][v];
        int push_amount = std::min(excess[u], residual_capacity);
        
        flow[u][v] += push_amount;
        flow[v][u] -= push_amount;
        excess[u] -= push_amount;
        excess[v] += push_amount;
    }
    
    static void relabel(int u, const std::vector<std::vector<int>>& capacity,
                       const std::vector<std::vector<int>>& flow,
                       std::vector<int>& height) {
        int min_height = std::numeric_limits<int>::max();
        
        for (int v = 0; v < capacity.size(); v++) {
            if (capacity[u][v] - flow[u][v] > 0) {
                min_height = std::min(min_height, height[v]);
            }
        }
        
        if (min_height != std::numeric_limits<int>::max()) {
            height[u] = min_height + 1;
        }
    }
    
public:
    static MaxFlowResult compute_max_flow(const FlowNetwork& network) {
        int n = network.get_node_count();
        int source = network.get_source();
        int sink = network.get_sink();
        
        MaxFlowResult result(n);
        
        // 初始化容量矩阵
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result.capacity[i][j] = network.get_capacity(i, j);
            }
        }
        
        std::vector<int> height(n, 0);
        std::vector<int> excess(n, 0);
        
        initialize_preflow(result.capacity, result.flow, height, excess, source, n);
        
        bool active = true;
        while (active) {
            active = false;
            
            for (int u = 0; u < n; u++) {
                if (u == sink) continue;
                
                while (excess[u] > 0) {
                    bool pushed = false;
                    
                    // 尝试推送
                    for (int v = 0; v < n && excess[u] > 0; v++) {
                        if (result.capacity[u][v] - result.flow[u][v] > 0 && height[u] == height[v] + 1) {
                            push(u, v, result.capacity, result.flow, excess);
                            pushed = true;
                            active = true;
                        }
                    }
                    
                    // 如果不能推送，则重贴标签
                    if (!pushed && excess[u] > 0) {
                        relabel(u, result.capacity, result.flow, height);
                        active = true;
                        break;
                    }
                }
            }
        }
        
        // 计算最大流值
        result.max_flow_value = 0;
        for (int v = 0; v < n; v++) {
            result.max_flow_value += result.flow[source][v];
        }
        
        // 计算残量矩阵
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result.residual[i][j] = result.capacity[i][j] - result.flow[i][j];
            }
        }
        
        return result;
    }
};

// 工具函数：打印最大流结果
void print_max_flow_result(const MaxFlowResult& result, int source, int sink) {
    std::cout << "最大流结果:" << std::endl;
    std::cout << "最大流值: " << result.max_flow_value << std::endl;
    
    int n = result.flow.size();
    
    std::cout << "流矩阵:" << std::endl;
    std::cout << "    ";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(4) << i;
    }
    std::cout << std::endl;
    
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(4) << i;
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(4) << result.flow[i][j];
        }
        std::cout << std::endl;
    }
    
    std::cout << "残量矩阵:" << std::endl;
    std::cout << "    ";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(4) << i;
    }
    std::cout << std::endl;
    
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(4) << i;
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(4) << result.residual[i][j];
        }
        std::cout << std::endl;
    }
}

// 算法导论中的经典最大流示例
void print_max_flow_examples() {
    std::cout << "=== 算法导论经典最大流示例 ===" << std::endl;
    
    // 示例1：算法导论图26.1 - 简单流网络
    std::cout << "示例1: 简单流网络（算法导论图26.1）" << std::endl;
    FlowNetwork network1(6, 0, 5); // 6个节点，源点0，汇点5
    
    network1.add_edge(0, 1, 16);
    network1.add_edge(0, 2, 13);
    network1.add_edge(1, 2, 10);
    network1.add_edge(1, 3, 12);
    network1.add_edge(2, 1, 4);
    network1.add_edge(2, 4, 14);
    network1.add_edge(3, 2, 9);
    network1.add_edge(3, 5, 20);
    network1.add_edge(4, 3, 7);
    network1.add_edge(4, 5, 4);
    
    network1.print_network();
    
    auto result1 = FordFulkerson::compute_max_flow(network1);
    print_max_flow_result(result1, 0, 5);
    
    // 示例2：二分图匹配
    std::cout << "\n示例2: 二分图匹配" << std::endl;
    std::vector<std::vector<int>> bipartite_graph = {
        {1, 1, 0, 0},  // 左部节点0
        {0, 1, 1, 0},  // 左部节点1
        {1, 0, 0, 1},  // 左部节点2
        {0, 0, 1, 0}   // 左部节点3
    };
    
    BipartiteMatching::print_matching(bipartite_graph, 4, 4);
    
    // 示例3：推送-重贴标签算法
    std::cout << "\n示例3: 推送-重贴标签算法" << std::endl;
    FlowNetwork network3(4, 0, 3); // 4个节点，源点0，汇点3
    
    network3.add_edge(0, 1, 3);
    network3.add_edge(0, 2, 3);
    network3.add_edge(1, 2, 2);
    network3.add_edge(1, 3, 3);
    network3.add_edge(2, 3, 3);
    
    network3.print_network();
    
    auto result3 = PushRelabel::compute_max_flow(network3);
    print_max_flow_result(result3, 0, 3);
}

} // namespace algorithms

#endif // MAX_FLOW_H