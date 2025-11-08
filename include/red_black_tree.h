#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <memory>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <functional>

namespace algorithms {

// 红黑树节点颜色
enum class Color { RED, BLACK };

// 红黑树节点
template<typename T>
class RBNode : public std::enable_shared_from_this<RBNode<T>> {
public:
    T key;
    Color color;
    std::shared_ptr<RBNode<T>> left;
    std::shared_ptr<RBNode<T>> right;
    std::shared_ptr<RBNode<T>> parent;

    RBNode(const T& k, Color c = Color::RED) 
        : key(k), color(c), left(nullptr), right(nullptr), parent(nullptr) {}
    
    // 判断是否为叶子节点（NIL节点）
    bool is_leaf() const {
        return left == nullptr && right == nullptr;
    }
    
    // 判断是否为根节点
    bool is_root() const {
        return parent == nullptr;
    }
    
    // 获取兄弟节点
    std::shared_ptr<RBNode<T>> sibling() const {
        if (parent == nullptr) return nullptr;
        return (this == parent->left.get()) ? parent->right : parent->left;
    }
    
    // 获取叔叔节点
    std::shared_ptr<RBNode<T>> uncle() const {
        if (parent == nullptr || parent->parent == nullptr) return nullptr;
        return parent->sibling();
    }
    
    // 获取祖父节点
    std::shared_ptr<RBNode<T>> grandparent() const {
        if (parent == nullptr) return nullptr;
        return parent->parent;
    }
};

// 红黑树
template<typename T>
class RedBlackTree {
private:
    std::shared_ptr<RBNode<T>> root;
    std::shared_ptr<RBNode<T>> nil; // NIL节点
    
    // 创建NIL节点
    std::shared_ptr<RBNode<T>> create_nil() {
        auto node = std::make_shared<RBNode<T>>(T{}, Color::BLACK);
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    
    // 左旋
    void left_rotate(std::shared_ptr<RBNode<T>> x) {
        if (x == nullptr || x->right == nil) return;
        
        auto y = x->right;
        x->right = y->left;
        
        if (y->left != nil) {
            y->left->parent = x;
        }
        
        y->parent = x->parent;
        
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        
        y->left = x;
        x->parent = y;
    }
    
    // 右旋
    void right_rotate(std::shared_ptr<RBNode<T>> y) {
        if (y == nullptr || y->left == nil) return;
        
        auto x = y->left;
        y->left = x->right;
        
        if (x->right != nil) {
            x->right->parent = y;
        }
        
        x->parent = y->parent;
        
        if (y->parent == nil) {
            root = x;
        } else if (y == y->parent->right) {
            y->parent->right = x;
        } else {
            y->parent->left = x;
        }
        
        x->right = y;
        y->parent = x;
    }
    
    // 插入修复
    void insert_fixup(std::shared_ptr<RBNode<T>> z) {
        while (z->parent != nil && z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
                auto y = z->parent->parent->right; // 叔叔节点
                
                if (y != nil && y->color == Color::RED) {
                    // 情况1：叔叔是红色
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // 情况2：z是右孩子
                        z = z->parent;
                        left_rotate(z);
                    }
                    // 情况3：z是左孩子
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    right_rotate(z->parent->parent);
                }
            } else {
                // 对称情况
                auto y = z->parent->parent->left; // 叔叔节点
                
                if (y != nil && y->color == Color::RED) {
                    // 情况1：叔叔是红色
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        // 情况2：z是左孩子
                        z = z->parent;
                        right_rotate(z);
                    }
                    // 情况3：z是右孩子
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    left_rotate(z->parent->parent);
                }
            }
        }
        
        root->color = Color::BLACK;
    }
    
    // 移植操作
    void transplant(std::shared_ptr<RBNode<T>> u, std::shared_ptr<RBNode<T>> v) {
        if (u->parent == nil) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }
    
    // 查找最小节点
    std::shared_ptr<RBNode<T>> minimum(std::shared_ptr<RBNode<T>> node) const {
        while (node != nil && node->left != nil) {
            node = node->left;
        }
        return node;
    }
    
    // 删除修复
    void delete_fixup(std::shared_ptr<RBNode<T>> x) {
        while (x != root && x->color == Color::BLACK) {
            if (x == x->parent->left) {
                auto w = x->parent->right; // 兄弟节点
                
                if (w->color == Color::RED) {
                    // 情况1：兄弟是红色
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    left_rotate(x->parent);
                    w = x->parent->right;
                }
                
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                    // 情况2：兄弟的两个孩子都是黑色
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->right->color == Color::BLACK) {
                        // 情况3：兄弟的右孩子是黑色，左孩子是红色
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        right_rotate(w);
                        w = x->parent->right;
                    }
                    // 情况4：兄弟的右孩子是红色
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    left_rotate(x->parent);
                    x = root;
                }
            } else {
                // 对称情况
                auto w = x->parent->left; // 兄弟节点
                
                if (w->color == Color::RED) {
                    // 情况1：兄弟是红色
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    right_rotate(x->parent);
                    w = x->parent->left;
                }
                
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    // 情况2：兄弟的两个孩子都是黑色
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->left->color == Color::BLACK) {
                        // 情况3：兄弟的左孩子是黑色，右孩子是红色
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        left_rotate(w);
                        w = x->parent->left;
                    }
                    // 情况4：兄弟的左孩子是红色
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    right_rotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = Color::BLACK;
    }
    
    // 中序遍历辅助函数
    void inorder_helper(std::shared_ptr<RBNode<T>> node, std::vector<T>& result) const {
        if (node != nil) {
            inorder_helper(node->left, result);
            result.push_back(node->key);
            inorder_helper(node->right, result);
        }
    }
    
    // 前序遍历辅助函数
    void preorder_helper(std::shared_ptr<RBNode<T>> node, std::vector<T>& result) const {
        if (node != nil) {
            result.push_back(node->key);
            preorder_helper(node->left, result);
            preorder_helper(node->right, result);
        }
    }
    
    // 后序遍历辅助函数
    void postorder_helper(std::shared_ptr<RBNode<T>> node, std::vector<T>& result) const {
        if (node != nil) {
            postorder_helper(node->left, result);
            postorder_helper(node->right, result);
            result.push_back(node->key);
        }
    }

public:
    RedBlackTree() {
        nil = create_nil();
        root = nil;
    }
    
    // 判断树是否为空
    bool empty() const {
        return root == nil;
    }
    
    // 插入操作
    void insert(const T& key) {
        auto z = std::make_shared<RBNode<T>>(key);
        z->left = nil;
        z->right = nil;
        z->color = Color::RED;
        
        auto y = nil;
        auto x = root;
        
        // 找到插入位置
        while (x != nil) {
            y = x;
            if (z->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        
        z->parent = y;
        
        if (y == nil) {
            // 树为空
            root = z;
        } else if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }
        
        insert_fixup(z);
    }
    
    // 查找操作
    std::shared_ptr<RBNode<T>> search(const T& key) const {
        auto x = root;
        while (x != nil && key != x->key) {
            if (key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return (x != nil) ? x : nullptr;
    }
    
    // 删除操作
    bool remove(const T& key) {
        auto z = search(key);
        if (z == nullptr) {
            return false; // 节点不存在
        }
        
        auto y = z;
        auto y_original_color = y->color;
        std::shared_ptr<RBNode<T>> x;
        
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        if (y_original_color == Color::BLACK) {
            delete_fixup(x);
        }
        
        return true;
    }
    
    // 获取最小值
    T minimum() const {
        auto min_node = minimum(root);
        if (min_node == nil) {
            throw std::runtime_error("Tree is empty");
        }
        return min_node->key;
    }
    
    // 获取最大值
    T maximum() const {
        auto x = root;
        while (x != nil && x->right != nil) {
            x = x->right;
        }
        if (x == nil) {
            throw std::runtime_error("Tree is empty");
        }
        return x->key;
    }
    
    // 中序遍历
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
    
    // 后序遍历
    std::vector<T> postorder_traversal() const {
        std::vector<T> result;
        postorder_helper(root, result);
        return result;
    }
    
    // 层序遍历
    std::vector<T> level_order_traversal() const {
        std::vector<T> result;
        if (root == nil) return result;
        
        std::queue<std::shared_ptr<RBNode<T>>> q;
        q.push(root);
        
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            
            if (current != nil) {
                result.push_back(current->key);
                
                if (current->left != nil) {
                    q.push(current->left);
                }
                if (current->right != nil) {
                    q.push(current->right);
                }
            }
        }
        
        return result;
    }
    
    // 获取树的高度（黑高度）
    int black_height() const {
        int height = 0;
        auto x = root;
        
        while (x != nil) {
            if (x->color == Color::BLACK) {
                height++;
            }
            x = x->left;
        }
        
        return height;
    }
    
    // 验证红黑树性质
    bool is_valid() const {
        if (root == nil) return true;
        
        // 性质2：根节点是黑色
        if (root->color != Color::BLACK) {
            return false;
        }
        
        // 验证其他性质
        return validate_properties(root);
    }
    
    // 验证红黑树性质的辅助函数
    bool validate_properties(std::shared_ptr<RBNode<T>> node) const {
        if (node == nil) return true;
        
        // 性质4：红色节点的孩子必须是黑色
        if (node->color == Color::RED) {
            if ((node->left != nil && node->left->color == Color::RED) ||
                (node->right != nil && node->right->color == Color::RED)) {
                return false;
            }
        }
        
        // 递归验证左右子树
        return validate_properties(node->left) && validate_properties(node->right);
    }
    
    // 打印树结构（用于调试）
    void print_tree() const {
        if (root == nil) {
            std::cout << "Empty tree" << std::endl;
            return;
        }
        
        std::queue<std::shared_ptr<RBNode<T>>> q;
        q.push(root);
        
        while (!q.empty()) {
            int level_size = q.size();
            
            for (int i = 0; i < level_size; i++) {
                auto current = q.front();
                q.pop();
                
                if (current == nil) {
                    std::cout << "NIL ";
                } else {
                    std::cout << current->key << "(" 
                              << (current->color == Color::RED ? "R" : "B") << ") ";
                    q.push(current->left);
                    q.push(current->right);
                }
            }
            std::cout << std::endl;
        }
    }
    
    // 清空树
    void clear() {
        root = nil;
    }
};

} // namespace algorithms

#endif // RED_BLACK_TREE_H