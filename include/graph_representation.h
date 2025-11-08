#ifndef GRAPH_REPRESENTATION_H
#define GRAPH_REPRESENTATION_H

#include <vector>
#include <list>
#include <memory>
#include <iostream>
#include <iomanip>
#include <queue>
#include <stack>
#include <algorithm>

namespace algorithms {

// 图节点
struct GraphNode {
    int id;
    std::string name;
    
    GraphNode(int node_id, const std::string& node_name = "") 
        : id(node_id), name(node_name) {}
    
    bool operator==(const GraphNode& other) const {
        return id == other.id;
    }
    
    bool operator<(const GraphNode& other) const {
        return id < other.id;
    }
};

// 图的边
struct GraphEdge {
    int from;
    int to;
    int weight;
    
    GraphEdge(int f, int t, int w = 1) : from(f), to(t), weight(w) {}
    
    bool operator==(const GraphEdge& other) const {
        return from == other.from && to == other.to;
    }
};

// 邻接表表示的图
class AdjacencyListGraph {
private:
    std::vector<GraphNode> nodes;
    std::vector<std::list<GraphEdge>> adjacency_list;
    bool directed;
    
public:
    AdjacencyListGraph(bool is_directed = false) : directed(is_directed) {}
    
    // 添加节点
    void add_node(int node_id, const std::string& name = "") {
        if (find_node_index(node_id) != -1) {
            throw std::invalid_argument("Node already exists");
        }
        
        nodes.emplace_back(node_id, name);
        adjacency_list.emplace_back();
    }
    
    // 添加边
    void add_edge(int from, int to, int weight = 1) {
        int from_index = find_node_index(from);
        int to_index = find_node_index(to);
        
        if (from_index == -1 || to_index == -1) {
            throw std::invalid_argument("Node not found");
        }
        
        // 检查边是否已存在
        for (const auto& edge : adjacency_list[from_index]) {
            if (edge.to == to) {
                throw std::invalid_argument("Edge already exists");
            }
        }
        
        adjacency_list[from_index].emplace_back(from, to, weight);
        
        // 如果是无向图，添加反向边
        if (!directed) {
            adjacency_list[to_index].emplace_back(to, from, weight);
        }
    }
    
    // 获取节点的邻居
    std::vector<int> get_neighbors(int node_id) const {
        int node_index = find_node_index(node_id);
        if (node_index == -1) {
            return {};
        }
        
        std::vector<int> neighbors;
        for (const auto& edge : adjacency_list[node_index]) {
            neighbors.push_back(edge.to);
        }
        return neighbors;
    }
    
    // 获取节点的度（出度）
    int get_degree(int node_id) const {
        int node_index = find_node_index(node_id);
        if (node_index == -1) {
            return -1;
        }
        return adjacency_list[node_index].size();
    }
    
    // 广度优先搜索
    std::vector<int> bfs(int start_node) const {
        int start_index = find_node_index(start_node);
        if (start_index == -1) {
            return {};
        }
        
        std::vector<int> result;
        std::vector<bool> visited(nodes.size(), false);
        std::queue<int> q;
        
        visited[start_index] = true;
        q.push(start_index);
        
        while (!q.empty()) {
            int current_index = q.front();
            q.pop();
            result.push_back(nodes[current_index].id);
            
            for (const auto& edge : adjacency_list[current_index]) {
                int neighbor_index = find_node_index(edge.to);
                if (!visited[neighbor_index]) {
                    visited[neighbor_index] = true;
                    q.push(neighbor_index);
                }
            }
        }
        
        return result;
    }
    
    // 深度优先搜索
    std::vector<int> dfs(int start_node) const {
        int start_index = find_node_index(start_node);
        if (start_index == -1) {
            return {};
        }
        
        std::vector<int> result;
        std::vector<bool> visited(nodes.size(), false);
        std::stack<int> s;
        
        s.push(start_index);
        
        while (!s.empty()) {
            int current_index = s.top();
            s.pop();
            
            if (!visited[current_index]) {
                visited[current_index] = true;
                result.push_back(nodes[current_index].id);
                
                // 逆序添加邻居，以保持与递归DFS相似的顺序
                std::vector<int> neighbors;
                for (const auto& edge : adjacency_list[current_index]) {
                    neighbors.push_back(find_node_index(edge.to));
                }
                std::reverse(neighbors.begin(), neighbors.end());
                
                for (int neighbor_index : neighbors) {
                    if (!visited[neighbor_index]) {
                        s.push(neighbor_index);
                    }
                }
            }
        }
        
        return result;
    }
    
    // 打印邻接表
    void print_adjacency_list() const {
        std::cout << "邻接表表示:" << std::endl;
        for (size_t i = 0; i < nodes.size(); i++) {
            std::cout << "节点 " << nodes[i].id;
            if (!nodes[i].name.empty()) {
                std::cout << " (" << nodes[i].name << ")";
            }
            std::cout << ": ";
            
            for (const auto& edge : adjacency_list[i]) {
                std::cout << "-> " << edge.to;
                if (edge.weight != 1) {
                    std::cout << " (w=" << edge.weight << ")";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
    
    // 获取节点数量
    size_t get_node_count() const {
        return nodes.size();
    }
    
    // 获取边数量
    size_t get_edge_count() const {
        size_t count = 0;
        for (const auto& list : adjacency_list) {
            count += list.size();
        }
        
        if (!directed) {
            count /= 2; // 无向图的边被计算了两次
        }
        
        return count;
    }
    
    // 检查图是否是有向图
    bool is_directed() const {
        return directed;
    }

private:
    // 查找节点的索引
    int find_node_index(int node_id) const {
        for (size_t i = 0; i < nodes.size(); i++) {
            if (nodes[i].id == node_id) {
                return i;
            }
        }
        return -1;
    }
};

// 邻接矩阵表示的图
class AdjacencyMatrixGraph {
private:
    std::vector<GraphNode> nodes;
    std::vector<std::vector<int>> adjacency_matrix;
    bool directed;
    
public:
    AdjacencyMatrixGraph(bool is_directed = false) : directed(is_directed) {}
    
    // 添加节点
    void add_node(int node_id, const std::string& name = "") {
        if (find_node_index(node_id) != -1) {
            throw std::invalid_argument("Node already exists");
        }
        
        nodes.emplace_back(node_id, name);
        
        // 扩展邻接矩阵
        size_t new_size = nodes.size();
        for (auto& row : adjacency_matrix) {
            row.resize(new_size, 0);
        }
        
        adjacency_matrix.resize(new_size, std::vector<int>(new_size, 0));
    }
    
    // 添加边
    void add_edge(int from, int to, int weight = 1) {
        int from_index = find_node_index(from);
        int to_index = find_node_index(to);
        
        if (from_index == -1 || to_index == -1) {
            throw std::invalid_argument("Node not found");
        }
        
        if (adjacency_matrix[from_index][to_index] != 0) {
            throw std::invalid_argument("Edge already exists");
        }
        
        adjacency_matrix[from_index][to_index] = weight;
        
        // 如果是无向图，设置对称位置
        if (!directed) {
            adjacency_matrix[to_index][from_index] = weight;
        }
    }
    
    // 获取节点的邻居
    std::vector<int> get_neighbors(int node_id) const {
        int node_index = find_node_index(node_id);
        if (node_index == -1) {
            return {};
        }
        
        std::vector<int> neighbors;
        for (size_t i = 0; i < nodes.size(); i++) {
            if (adjacency_matrix[node_index][i] != 0) {
                neighbors.push_back(nodes[i].id);
            }
        }
        return neighbors;
    }
    
    // 获取节点的度
    int get_degree(int node_id) const {
        int node_index = find_node_index(node_id);
        if (node_index == -1) {
            return -1;
        }
        
        int degree = 0;
        for (size_t i = 0; i < nodes.size(); i++) {
            if (adjacency_matrix[node_index][i] != 0) {
                degree++;
            }
        }
        return degree;
    }
    
    // 检查两个节点之间是否有边
    bool has_edge(int from, int to) const {
        int from_index = find_node_index(from);
        int to_index = find_node_index(to);
        
        if (from_index == -1 || to_index == -1) {
            return false;
        }
        
        return adjacency_matrix[from_index][to_index] != 0;
    }
    
    // 获取边的权重
    int get_edge_weight(int from, int to) const {
        int from_index = find_node_index(from);
        int to_index = find_node_index(to);
        
        if (from_index == -1 || to_index == -1) {
            return -1;
        }
        
        return adjacency_matrix[from_index][to_index];
    }
    
    // 打印邻接矩阵
    void print_adjacency_matrix() const {
        std::cout << "邻接矩阵表示:" << std::endl;
        
        // 打印列标题
        std::cout << "    ";
        for (const auto& node : nodes) {
            std::cout << std::setw(3) << node.id;
        }
        std::cout << std::endl;
        
        // 打印矩阵
        for (size_t i = 0; i < nodes.size(); i++) {
            std::cout << std::setw(3) << nodes[i].id;
            for (size_t j = 0; j < nodes.size(); j++) {
                std::cout << std::setw(3) << adjacency_matrix[i][j];
            }
            std::cout << std::endl;
        }
    }
    
    // 获取节点数量
    size_t get_node_count() const {
        return nodes.size();
    }
    
    // 获取边数量
    size_t get_edge_count() const {
        size_t count = 0;
        for (size_t i = 0; i < nodes.size(); i++) {
            for (size_t j = 0; j < nodes.size(); j++) {
                if (adjacency_matrix[i][j] != 0) {
                    count++;
                }
            }
        }
        
        if (!directed) {
            count /= 2; // 无向图的边被计算了两次
        }
        
        return count;
    }
    
    // 检查图是否是有向图
    bool is_directed() const {
        return directed;
    }

private:
    // 查找节点的索引
    int find_node_index(int node_id) const {
        for (size_t i = 0; i < nodes.size(); i++) {
            if (nodes[i].id == node_id) {
                return i;
            }
        }
        return -1;
    }
};

// 算法导论中的经典图示例
void print_classic_examples() {
    std::cout << "=== 算法导论经典图示例 ===" << std::endl;
    
    // 示例1：算法导论图22.1(a) - 无向图
    std::cout << "示例1: 无向图（算法导论图22.1(a)）" << std::endl;
    AdjacencyListGraph undirected_graph(false);
    
    // 添加节点
    for (int i = 1; i <= 5; i++) {
        undirected_graph.add_node(i);
    }
    
    // 添加边
    undirected_graph.add_edge(1, 2);
    undirected_graph.add_edge(1, 5);
    undirected_graph.add_edge(2, 5);
    undirected_graph.add_edge(2, 3);
    undirected_graph.add_edge(2, 4);
    undirected_graph.add_edge(3, 4);
    undirected_graph.add_edge(4, 5);
    
    undirected_graph.print_adjacency_list();
    std::cout << "节点数量: " << undirected_graph.get_node_count() << std::endl;
    std::cout << "边数量: " << undirected_graph.get_edge_count() << std::endl;
    std::cout << "是有向图: " << (undirected_graph.is_directed() ? "是" : "否") << std::endl;
    
    // 示例2：算法导论图22.2 - 有向图
    std::cout << "\n示例2: 有向图（算法导论图22.2）" << std::endl;
    AdjacencyMatrixGraph directed_graph(true);
    
    // 添加节点
    for (int i = 1; i <= 6; i++) {
        directed_graph.add_node(i);
    }
    
    // 添加边
    directed_graph.add_edge(1, 2);
    directed_graph.add_edge(1, 4);
    directed_graph.add_edge(2, 5);
    directed_graph.add_edge(3, 5);
    directed_graph.add_edge(3, 6);
    directed_graph.add_edge(4, 2);
    directed_graph.add_edge(5, 4);
    directed_graph.add_edge(6, 6);
    
    directed_graph.print_adjacency_matrix();
    std::cout << "节点数量: " << directed_graph.get_node_count() << std::endl;
    std::cout << "边数量: " << directed_graph.get_edge_count() << std::endl;
    std::cout << "是有向图: " << (directed_graph.is_directed() ? "是" : "否") << std::endl;
}

} // namespace algorithms

#endif // GRAPH_REPRESENTATION_H