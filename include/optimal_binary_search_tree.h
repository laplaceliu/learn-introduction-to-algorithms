#ifndef OPTIMAL_BINARY_SEARCH_TREE_H
#define OPTIMAL_BINARY_SEARCH_TREE_H

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "binary_search_tree.h"

namespace algorithms {

// 最优二叉搜索树节点（扩展BST节点，包含概率信息）
template<typename T>
class OBSTNode : public BSTNode<T> {
public:
    double probability; // 搜索概率
    
    OBSTNode(const T& k, double p = 0.0) 
        : BSTNode<T>(k), probability(p) {}
        
    // 添加虚析构函数以支持多态
    virtual ~OBSTNode() = default;
};

// 最优二叉搜索树
template<typename T>
class OptimalBinarySearchTree {
private:
    std::shared_ptr<OBSTNode<T>> root;
    std::vector<T> keys;
    std::vector<double> probabilities;
    
public:
    OptimalBinarySearchTree(const std::vector<T>& k, const std::vector<double>& p) 
        : keys(k), probabilities(p) {
        if (keys.size() != probabilities.size()) {
            throw std::invalid_argument("Keys and probabilities must have the same size");
        }
        build_optimal_bst();
    }
    
    // 构建最优二叉搜索树（动态规划算法）
    void build_optimal_bst() {
        int n = keys.size();
        if (n == 0) {
            root = nullptr;
            return;
        }
        
        // 创建DP表：e[i][j]表示从i到j的最优子树的期望搜索成本
        std::vector<std::vector<double>> e(n + 2, std::vector<double>(n + 1, 0.0));
        
        // 创建根表：root[i][j]表示从i到j的最优子树的根索引
        std::vector<std::vector<int>> root_table(n + 1, std::vector<int>(n + 1, 0));
        
        // 创建概率和表：w[i][j]表示从i到j的概率和
        std::vector<std::vector<double>> w(n + 2, std::vector<double>(n + 1, 0.0));
        
        // 初始化单个键的情况
        for (int i = 1; i <= n; i++) {
            e[i][i - 1] = 0.0;
            w[i][i - 1] = 0.0;
        }
        
        // 填充DP表
        for (int l = 1; l <= n; l++) {
            for (int i = 1; i <= n - l + 1; i++) {
                int j = i + l - 1;
                e[i][j] = std::numeric_limits<double>::max();
                w[i][j] = w[i][j - 1] + probabilities[j - 1];
                
                // 尝试所有可能的根
                for (int r = i; r <= j; r++) {
                    double t = e[i][r - 1] + e[r + 1][j] + w[i][j];
                    if (t < e[i][j]) {
                        e[i][j] = t;
                        root_table[i][j] = r;
                    }
                }
            }
        }
        
        // 构建最优BST
        root = build_optimal_bst_helper(1, n, root_table);
    }
    
    // 获取期望搜索成本
    double get_expected_cost() const {
        int n = keys.size();
        if (n == 0) return 0.0;
        
        std::vector<std::vector<double>> e(n + 2, std::vector<double>(n + 1, 0.0));
        std::vector<std::vector<double>> w(n + 2, std::vector<double>(n + 1, 0.0));
        
        for (int i = 1; i <= n; i++) {
            e[i][i - 1] = 0.0;
            w[i][i - 1] = 0.0;
        }
        
        for (int l = 1; l <= n; l++) {
            for (int i = 1; i <= n - l + 1; i++) {
                int j = i + l - 1;
                e[i][j] = std::numeric_limits<double>::max();
                w[i][j] = w[i][j - 1] + probabilities[j - 1];
                
                for (int r = i; r <= j; r++) {
                    double t = e[i][r - 1] + e[r + 1][j] + w[i][j];
                    if (t < e[i][j]) {
                        e[i][j] = t;
                    }
                }
            }
        }
        
        return e[1][n];
    }
    
    // 打印DP表（用于调试和可视化）
    void print_dp_tables() const {
        int n = keys.size();
        if (n == 0) {
            std::cout << "没有键值" << std::endl;
            return;
        }
        
        std::vector<std::vector<double>> e(n + 2, std::vector<double>(n + 1, 0.0));
        std::vector<std::vector<double>> w(n + 2, std::vector<double>(n + 1, 0.0));
        std::vector<std::vector<int>> root_table(n + 1, std::vector<int>(n + 1, 0));
        
        // 初始化
        for (int i = 1; i <= n; i++) {
            e[i][i - 1] = 0.0;
            w[i][i - 1] = 0.0;
        }
        
        // 填充表
        for (int l = 1; l <= n; l++) {
            for (int i = 1; i <= n - l + 1; i++) {
                int j = i + l - 1;
                e[i][j] = std::numeric_limits<double>::max();
                w[i][j] = w[i][j - 1] + probabilities[j - 1];
                
                for (int r = i; r <= j; r++) {
                    double t = e[i][r - 1] + e[r + 1][j] + w[i][j];
                    if (t < e[i][j]) {
                        e[i][j] = t;
                        root_table[i][j] = r;
                    }
                }
            }
        }
        
        // 打印期望成本表
        std::cout << "期望搜索成本表 e[i][j]:" << std::endl;
        std::cout << "    ";
        for (int j = 0; j <= n; j++) {
            std::cout << std::setw(8) << j;
        }
        std::cout << std::endl;
        
        for (int i = 1; i <= n; i++) {
            std::cout << std::setw(4) << i;
            for (int j = 0; j <= n; j++) {
                if (j < i - 1) {
                    std::cout << std::setw(8) << "-";
                } else {
                    std::cout << std::setw(8) << std::fixed << std::setprecision(3) << e[i][j];
                }
            }
            std::cout << std::endl;
        }
        
        // 打印概率和表
        std::cout << "\n概率和表 w[i][j]:" << std::endl;
        std::cout << "    ";
        for (int j = 0; j <= n; j++) {
            std::cout << std::setw(8) << j;
        }
        std::cout << std::endl;
        
        for (int i = 1; i <= n; i++) {
            std::cout << std::setw(4) << i;
            for (int j = 0; j <= n; j++) {
                if (j < i - 1) {
                    std::cout << std::setw(8) << "-";
                } else {
                    std::cout << std::setw(8) << std::fixed << std::setprecision(3) << w[i][j];
                }
            }
            std::cout << std::endl;
        }
        
        // 打印根表
        std::cout << "\n根表 root[i][j]:" << std::endl;
        std::cout << "    ";
        for (int j = 1; j <= n; j++) {
            std::cout << std::setw(4) << j;
        }
        std::cout << std::endl;
        
        for (int i = 1; i <= n; i++) {
            std::cout << std::setw(4) << i;
            for (int j = 1; j <= n; j++) {
                if (j < i) {
                    std::cout << std::setw(4) << "-";
                } else {
                    std::cout << std::setw(4) << root_table[i][j];
                }
            }
            std::cout << std::endl;
        }
    }
    
    // 中序遍历（按关键字升序）
    std::vector<T> inorder_traversal() const {
        std::vector<T> result;
        inorder_helper(root, result);
        return result;
    }
    
    // 前序遍历
    std::vector<T> preorder_traversal() const {
        std::vector<T> result;
        preorder_helper(root, result);
        return result;
    }
    
    // 获取树的高度
    int get_height() const {
        return get_height_helper(root);
    }
    
    // 搜索操作
    std::shared_ptr<OBSTNode<T>> search(const T& key) const {
        return std::dynamic_pointer_cast<OBSTNode<T>>(search_helper(root, key));
    }
    
    // 获取根节点
    std::shared_ptr<OBSTNode<T>> get_root() const {
        return root;
    }
    
    // 打印树结构
    void print_tree() const {
        print_tree_helper(root, 0, "");
    }

private:
    // 构建最优BST的辅助函数
    std::shared_ptr<OBSTNode<T>> build_optimal_bst_helper(int i, int j, 
                                                         const std::vector<std::vector<int>>& root_table) {
        if (i > j) {
            return nullptr;
        }
        
        int r = root_table[i][j];
        std::shared_ptr<OBSTNode<T>> node = std::make_shared<OBSTNode<T>>(keys[r - 1], probabilities[r - 1]);
        
        node->left = build_optimal_bst_helper(i, r - 1, root_table);
        node->right = build_optimal_bst_helper(r + 1, j, root_table);
        
        // 设置父指针
        if (node->left != nullptr) {
            node->left->parent = node;
        }
        if (node->right != nullptr) {
            node->right->parent = node;
        }
        
        return node;
    }
    
    // 中序遍历辅助函数
    void inorder_helper(std::shared_ptr<OBSTNode<T>> node, std::vector<T>& result) const {
        if (node != nullptr) {
            inorder_helper(std::dynamic_pointer_cast<OBSTNode<T>>(node->left), result);
            result.push_back(node->key);
            inorder_helper(std::dynamic_pointer_cast<OBSTNode<T>>(node->right), result);
        }
    }
    
    // 前序遍历辅助函数
    void preorder_helper(std::shared_ptr<OBSTNode<T>> node, std::vector<T>& result) const {
        if (node != nullptr) {
            result.push_back(node->key);
            preorder_helper(std::dynamic_pointer_cast<OBSTNode<T>>(node->left), result);
            preorder_helper(std::dynamic_pointer_cast<OBSTNode<T>>(node->right), result);
        }
    }
    
    // 计算高度辅助函数
    int get_height_helper(std::shared_ptr<OBSTNode<T>> node) const {
        if (node == nullptr) {
            return -1;
        }
        return 1 + std::max(get_height_helper(std::dynamic_pointer_cast<OBSTNode<T>>(node->left)), 
                           get_height_helper(std::dynamic_pointer_cast<OBSTNode<T>>(node->right)));
    }
    
    // 搜索辅助函数
    std::shared_ptr<BSTNode<T>> search_helper(std::shared_ptr<BSTNode<T>> node, const T& key) const {
        if (node == nullptr || node->key == key) {
            return node;
        }
        
        if (key < node->key) {
            return search_helper(node->left, key);
        } else {
            return search_helper(node->right, key);
        }
    }
    
    // 打印树结构辅助函数
    void print_tree_helper(std::shared_ptr<OBSTNode<T>> node, int level, const std::string& prefix) const {
        if (node != nullptr) {
            print_tree_helper(std::dynamic_pointer_cast<OBSTNode<T>>(node->right), level + 1, "    |");
            
            std::cout << std::string(level * 4, ' ');
            if (level > 0) {
                std::cout << prefix.substr(0, prefix.length() - 1) + "└── ";
            }
            std::cout << node->key << " (p=" << std::fixed << std::setprecision(3) << node->probability << ")" << std::endl;
            
            print_tree_helper(std::dynamic_pointer_cast<OBSTNode<T>>(node->left), level + 1, "    |");
        }
    }
};

// 算法导论中的经典示例
void print_classic_example() {
    std::cout << "=== 算法导论经典示例（图15.9） ===" << std::endl;
    
    // 示例：算法导论图15.9
    std::vector<std::string> keys = {"k1", "k2", "k3", "k4", "k5"};
    std::vector<double> probabilities = {0.15, 0.10, 0.05, 0.10, 0.20};
    
    OptimalBinarySearchTree<std::string> obst(keys, probabilities);
    
    std::cout << "键值: ";
    for (const auto& key : keys) {
        std::cout << key << " ";
    }
    std::cout << std::endl;
    
    std::cout << "概率: ";
    for (double p : probabilities) {
        std::cout << std::fixed << std::setprecision(3) << p << " ";
    }
    std::cout << std::endl;
    
    std::cout << "期望搜索成本: " << std::fixed << std::setprecision(3) << obst.get_expected_cost() << std::endl;
    std::cout << "树高度: " << obst.get_height() << std::endl;
    
    auto inorder = obst.inorder_traversal();
    std::cout << "中序遍历: ";
    for (const auto& key : inorder) {
        std::cout << key << " ";
    }
    std::cout << std::endl;
}

} // namespace algorithms

#endif // OPTIMAL_BINARY_SEARCH_TREE_H