#ifndef B_TREE_H
#define B_TREE_H

#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>

namespace algorithms {

// B树节点
template<typename T>
class BTreeNode : public std::enable_shared_from_this<BTreeNode<T>> {
public:
    bool is_leaf;
    std::vector<T> keys;
    std::vector<std::shared_ptr<BTreeNode<T>>> children;
    int n;  // 当前键值数量
    
    BTreeNode(bool leaf = false) : is_leaf(leaf), n(0) {}
    
    // 判断节点是否已满
    bool is_full(int t) const {
        return n == 2 * t - 1;
    }
    
    // 判断节点是否过空
    bool is_minimum(int t) const {
        return n == t - 1;
    }
    
    // 在节点中查找键值的位置
    int find_key(const T& key) const {
        int i = 0;
        while (i < n && keys[i] < key) {
            i++;
        }
        return i;
    }
    
    // 插入键值到非满节点
    void insert_non_full(const T& key, int t) {
        int i = n - 1;
        
        if (is_leaf) {
            // 如果是叶子节点，直接插入
            keys.push_back(T()); // 增加一个位置
            while (i >= 0 && keys[i] > key) {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = key;
            n++;
        } else {
            // 找到要插入的子节点
            while (i >= 0 && keys[i] > key) {
                i--;
            }
            i++;
            
            if (children[i]->is_full(t)) {
                // 如果子节点已满，先分裂
                split_child(i, t);
                if (keys[i] < key) {
                    i++;
                }
            }
            children[i]->insert_non_full(key, t);
        }
    }
    
    // 分裂子节点
    void split_child(int i, int t) {
        auto y = children[i];
        auto z = std::make_shared<BTreeNode<T>>(y->is_leaf);
        
        // 将y的后t-1个键值移动到z
        z->keys.insert(z->keys.begin(), y->keys.begin() + t, y->keys.end());
        z->n = t - 1;
        
        // 如果不是叶子节点，移动子节点
        if (!y->is_leaf) {
            z->children.insert(z->children.begin(), y->children.begin() + t, y->children.end());
        }
        
        // 更新y的键值数量
        y->keys.resize(t - 1);
        y->n = t - 1;
        
        // 将中间键值提升到当前节点
        keys.insert(keys.begin() + i, y->keys[t - 1]);
        
        // 插入新的子节点
        children.insert(children.begin() + i + 1, z);
        
        n++;
    }
    
    // 删除键值
    bool remove(const T& key, int t) {
        int idx = find_key(key);
        
        if (idx < n && keys[idx] == key) {
            // 键值在当前节点中
            if (is_leaf) {
                // 情况1：叶子节点
                keys.erase(keys.begin() + idx);
                n--;
                return true;
            } else {
                // 情况2：内部节点
                return remove_from_internal_node(idx, t);
            }
        } else {
            // 键值不在当前节点中
            if (is_leaf) {
                return false; // 键值不存在
            }
            
            // 情况3：键值可能在子树中
            bool flag = (idx == n);
            
            if (children[idx]->n < t) {
                // 子节点键值数量不足，需要填充
                fill(idx, t);
            }
            
            if (flag && idx > n) {
                return children[idx - 1]->remove(key, t);
            } else {
                return children[idx]->remove(key, t);
            }
        }
    }
    
    // 从内部节点删除键值
    bool remove_from_internal_node(int idx, int t) {
        T key = keys[idx];
        
        if (children[idx]->n >= t) {
            // 情况2a：前驱子节点有足够键值
            T pred = get_predecessor(idx);
            keys[idx] = pred;
            return children[idx]->remove(pred, t);
        } else if (children[idx + 1]->n >= t) {
            // 情况2b：后继子节点有足够键值
            T succ = get_successor(idx);
            keys[idx] = succ;
            return children[idx + 1]->remove(succ, t);
        } else {
            // 情况2c：合并子节点
            merge(idx, t);
            return children[idx]->remove(key, t);
        }
    }
    
    // 获取前驱
    T get_predecessor(int idx) {
        auto current = children[idx];
        while (!current->is_leaf) {
            current = current->children[current->n];
        }
        return current->keys[current->n - 1];
    }
    
    // 获取后继
    T get_successor(int idx) {
        auto current = children[idx + 1];
        while (!current->is_leaf) {
            current = current->children[0];
        }
        return current->keys[0];
    }
    
    // 填充子节点
    void fill(int idx, int t) {
        if (idx != 0 && children[idx - 1]->n >= t) {
            // 从左兄弟借键值
            borrow_from_prev(idx);
        } else if (idx != n && children[idx + 1]->n >= t) {
            // 从右兄弟借键值
            borrow_from_next(idx);
        } else {
            // 合并子节点
            if (idx != n) {
                merge(idx, t);
            } else {
                merge(idx - 1, t);
            }
        }
    }
    
    // 从左兄弟借键值
    void borrow_from_prev(int idx) {
        auto child = children[idx];
        auto sibling = children[idx - 1];
        
        // 将当前节点的键值下移
        child->keys.insert(child->keys.begin(), keys[idx - 1]);
        
        if (!child->is_leaf) {
            child->children.insert(child->children.begin(), sibling->children[sibling->n]);
        }
        
        // 将兄弟节点的最大键值上移
        keys[idx - 1] = sibling->keys[sibling->n - 1];
        
        sibling->keys.pop_back();
        sibling->n--;
        child->n++;
    }
    
    // 从右兄弟借键值
    void borrow_from_next(int idx) {
        auto child = children[idx];
        auto sibling = children[idx + 1];
        
        // 将当前节点的键值下移
        child->keys.push_back(keys[idx]);
        
        if (!child->is_leaf) {
            child->children.push_back(sibling->children[0]);
        }
        
        // 将兄弟节点的最小键值上移
        keys[idx] = sibling->keys[0];
        
        sibling->keys.erase(sibling->keys.begin());
        if (!sibling->is_leaf) {
            sibling->children.erase(sibling->children.begin());
        }
        
        sibling->n--;
        child->n++;
    }
    
    // 合并子节点
    void merge(int idx, int t) {
        auto child = children[idx];
        auto sibling = children[idx + 1];
        
        // 将当前节点的键值下移
        child->keys.push_back(keys[idx]);
        
        // 合并兄弟节点的键值
        child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());
        
        // 合并子节点
        if (!child->is_leaf) {
            child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
        }
        
        // 删除当前节点的键值和子节点
        keys.erase(keys.begin() + idx);
        children.erase(children.begin() + idx + 1);
        
        child->n += sibling->n + 1;
        n--;
        
        // 如果根节点变空，更新根节点
        if (n == 0) {
            // 在实际的B树类中处理根节点更新
        }
    }
    
    // 搜索键值
    std::shared_ptr<BTreeNode<T>> search(const T& key) {
        int i = 0;
        while (i < n && key > keys[i]) {
            i++;
        }
        
        if (i < n && keys[i] == key) {
            return std::static_pointer_cast<BTreeNode<T>>(this->shared_from_this());
        }
        
        if (is_leaf) {
            return nullptr;
        }
        
        return children[i]->search(key);
    }
    
    // 遍历B树（中序遍历）
    void traverse(std::vector<T>& result) {
        int i;
        for (i = 0; i < n; i++) {
            if (!is_leaf) {
                children[i]->traverse(result);
            }
            result.push_back(keys[i]);
        }
        
        if (!is_leaf) {
            children[i]->traverse(result);
        }
    }
    
    // 打印B树（用于调试）
    void print(int level = 0) {
        std::cout << "Level " << level << " : ";
        for (int i = 0; i < n; i++) {
            std::cout << keys[i] << " ";
        }
        std::cout << std::endl;
        
        for (int i = 0; i <= n; i++) {
            if (!is_leaf) {
                children[i]->print(level + 1);
            }
        }
    }
};

// B树类
template<typename T>
class BTree {
private:
    std::shared_ptr<BTreeNode<T>> root;
    int t;  // 最小度数
    
public:
    BTree(int degree) : t(degree), root(nullptr) {}
    
    // 搜索键值
    std::shared_ptr<BTreeNode<T>> search(const T& key) {
        return (root == nullptr) ? nullptr : root->search(key);
    }
    
    // 插入键值
    void insert(const T& key) {
        if (root == nullptr) {
            root = std::make_shared<BTreeNode<T>>(true);
            root->keys.push_back(key);
            root->n = 1;
        } else {
            if (root->is_full(t)) {
                // 根节点已满，需要分裂
                auto s = std::make_shared<BTreeNode<T>>(false);
                s->children.push_back(root);
                s->split_child(0, t);
                
                int i = 0;
                if (s->keys[0] < key) {
                    i++;
                }
                s->children[i]->insert_non_full(key, t);
                root = s;
            } else {
                root->insert_non_full(key, t);
            }
        }
    }
    
    // 删除键值
    bool remove(const T& key) {
        if (root == nullptr) {
            return false;
        }
        
        bool result = root->remove(key, t);
        
        // 如果根节点变空，更新根节点
        if (root->n == 0) {
            if (root->is_leaf) {
                root = nullptr;
            } else {
                root = root->children[0];
            }
        }
        
        return result;
    }
    
    // 遍历B树（中序遍历）
    std::vector<T> traverse() {
        std::vector<T> result;
        if (root != nullptr) {
            root->traverse(result);
        }
        return result;
    }
    
    // 打印B树
    void print() {
        if (root != nullptr) {
            root->print();
        }
    }
    
    // 获取树的高度
    int height() {
        if (root == nullptr) {
            return 0;
        }
        
        auto node = root;
        int h = 0;
        while (!node->is_leaf) {
            node = node->children[0];
            h++;
        }
        return h;
    }
    
    // 验证B树性质
    bool validate() {
        if (root == nullptr) {
            return true;
        }
        return validate_node(root, true);
    }
    
private:
    // 验证节点性质
    bool validate_node(std::shared_ptr<BTreeNode<T>> node, bool is_root) {
        // 根节点可以有1到2t-1个键值，非根节点至少有t-1个键值
        if (!is_root && node->n < t - 1) {
            return false;
        }
        
        if (node->n > 2 * t - 1) {
            return false;
        }
        
        // 检查键值顺序
        for (int i = 1; i < node->n; i++) {
            if (node->keys[i] <= node->keys[i - 1]) {
                return false;
            }
        }
        
        // 如果不是叶子节点，检查子节点数量和键值范围
        if (!node->is_leaf) {
            if (node->children.size() != node->n + 1) {
                return false;
            }
            
            // 递归验证子节点
            for (int i = 0; i <= node->n; i++) {
                if (!validate_node(node->children[i], false)) {
                    return false;
                }
                
                // 检查子节点键值范围
                if (i > 0) {
                    auto left_child = node->children[i - 1];
                    if (left_child->keys.back() >= node->keys[i - 1]) {
                        return false;
                    }
                }
                
                if (i < node->n) {
                    auto right_child = node->children[i];
                    if (right_child->keys.front() <= node->keys[i]) {
                        return false;
                    }
                }
            }
        }
        
        return true;
    }
};

} // namespace algorithms

#endif // B_TREE_H