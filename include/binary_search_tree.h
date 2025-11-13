#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <vector>

namespace algorithms {

// 二叉搜索树节点
template <typename T>
class BSTNode : public std::enable_shared_from_this<BSTNode<T>> {
public:
  T key;
  std::shared_ptr<BSTNode<T>> left;
  std::shared_ptr<BSTNode<T>> right;
  std::shared_ptr<BSTNode<T>> parent;

  BSTNode(const T &k)
      : key(k), left(nullptr), right(nullptr), parent(nullptr) {}

  // 虚析构函数以支持多态
  virtual ~BSTNode() = default;

  // 判断是否为叶子节点
  bool is_leaf() const { return left == nullptr && right == nullptr; }

  // 判断是否为根节点
  bool is_root() const { return parent == nullptr; }
};

// 二叉搜索树
template <typename T> class BinarySearchTree {
private:
  std::shared_ptr<BSTNode<T>> root;

  // 中序遍历辅助函数
  void inorder_helper(std::shared_ptr<BSTNode<T>> node,
                      std::vector<T> &result) const {
    if (node != nullptr) {
      inorder_helper(node->left, result);
      result.push_back(node->key);
      inorder_helper(node->right, result);
    }
  }

  // 前序遍历辅助函数
  void preorder_helper(std::shared_ptr<BSTNode<T>> node,
                       std::vector<T> &result) const {
    if (node != nullptr) {
      result.push_back(node->key);
      preorder_helper(node->left, result);
      preorder_helper(node->right, result);
    }
  }

  // 后序遍历辅助函数
  void postorder_helper(std::shared_ptr<BSTNode<T>> node,
                        std::vector<T> &result) const {
    if (node != nullptr) {
      postorder_helper(node->left, result);
      postorder_helper(node->right, result);
      result.push_back(node->key);
    }
  }

  // 查找最小节点
  std::shared_ptr<BSTNode<T>> minimum(std::shared_ptr<BSTNode<T>> node) const {
    while (node != nullptr && node->left != nullptr) {
      node = node->left;
    }
    return node;
  }

  // 查找最大节点
  std::shared_ptr<BSTNode<T>> maximum(std::shared_ptr<BSTNode<T>> node) const {
    while (node != nullptr && node->right != nullptr) {
      node = node->right;
    }
    return node;
  }

  // 查找后继节点
  std::shared_ptr<BSTNode<T>>
  successor(std::shared_ptr<BSTNode<T>> node) const {
    if (node == nullptr)
      return nullptr;

    // 如果右子树存在，后继是右子树的最小节点
    if (node->right != nullptr) {
      return minimum(node->right);
    }

    // 否则向上查找第一个左孩子是祖先的节点
    std::shared_ptr<BSTNode<T>> current = node;
    std::shared_ptr<BSTNode<T>> parent = node->parent;

    while (parent != nullptr && current == parent->right) {
      current = parent;
      parent = parent->parent;
    }

    return parent;
  }

  // 查找前驱节点
  std::shared_ptr<BSTNode<T>>
  predecessor(std::shared_ptr<BSTNode<T>> node) const {
    if (node == nullptr)
      return nullptr;

    // 如果左子树存在，前驱是左子树的最大节点
    if (node->left != nullptr) {
      return maximum(node->left);
    }

    // 否则向上查找第一个右孩子是祖先的节点
    std::shared_ptr<BSTNode<T>> current = node;
    std::shared_ptr<BSTNode<T>> parent = node->parent;

    while (parent != nullptr && current == parent->left) {
      current = parent;
      parent = parent->parent;
    }

    return parent;
  }

  // 移植操作：用子树v替换子树u
  void transplant(std::shared_ptr<BSTNode<T>> u,
                  std::shared_ptr<BSTNode<T>> v) {
    if (u == nullptr)
      return;

    if (u->parent == nullptr) {
      // u是根节点
      root = v;
    } else if (u == u->parent->left) {
      // u是左孩子
      u->parent->left = v;
    } else {
      // u是右孩子
      u->parent->right = v;
    }

    if (v != nullptr) {
      v->parent = u->parent;
    }
  }

public:
  BinarySearchTree() : root(nullptr) {}

  // 判断树是否为空
  bool empty() const { return root == nullptr; }

  // 插入操作
  void insert(const T &key) {
    auto new_node = std::make_shared<BSTNode<T>>(key);
    std::shared_ptr<BSTNode<T>> current = root;
    std::shared_ptr<BSTNode<T>> parent = nullptr;

    // 找到插入位置
    while (current != nullptr) {
      parent = current;
      if (key < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    new_node->parent = parent;

    if (parent == nullptr) {
      // 树为空
      root = new_node;
    } else if (key < parent->key) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }
  }

  // 查找操作
  std::shared_ptr<BSTNode<T>> search(const T &key) const {
    auto current = root;
    while (current != nullptr && key != current->key) {
      if (key < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return current;
  }

  // 删除操作
  bool remove(const T &key) {
    auto node = search(key);
    if (node == nullptr) {
      return false; // 节点不存在
    }

    if (node->left == nullptr) {
      // 没有左子树
      transplant(node, node->right);
    } else if (node->right == nullptr) {
      // 没有右子树
      transplant(node, node->left);
    } else {
      // 有两个子树
      auto successor_node = minimum(node->right);

      if (successor_node->parent != node) {
        transplant(successor_node, successor_node->right);
        successor_node->right = node->right;
        successor_node->right->parent = successor_node;
      }

      transplant(node, successor_node);
      successor_node->left = node->left;
      successor_node->left->parent = successor_node;
    }

    return true;
  }

  // 获取最小值
  T minimum() const {
    auto min_node = minimum(root);
    if (min_node == nullptr) {
      throw std::runtime_error("Tree is empty");
    }
    return min_node->key;
  }

  // 获取最大值
  T maximum() const {
    auto max_node = maximum(root);
    if (max_node == nullptr) {
      throw std::runtime_error("Tree is empty");
    }
    return max_node->key;
  }

  // 获取后继
  T successor(const T &key) const {
    auto node = search(key);
    if (node == nullptr) {
      throw std::runtime_error("Key not found");
    }

    auto succ = successor(node);
    if (succ == nullptr) {
      throw std::runtime_error("No successor exists");
    }

    return succ->key;
  }

  // 获取前驱
  T predecessor(const T &key) const {
    auto node = search(key);
    if (node == nullptr) {
      throw std::runtime_error("Key not found");
    }

    auto pred = predecessor(node);
    if (pred == nullptr) {
      throw std::runtime_error("No predecessor exists");
    }

    return pred->key;
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
    if (root == nullptr)
      return result;

    std::queue<std::shared_ptr<BSTNode<T>>> q;
    q.push(root);

    while (!q.empty()) {
      auto current = q.front();
      q.pop();
      result.push_back(current->key);

      if (current->left != nullptr) {
        q.push(current->left);
      }
      if (current->right != nullptr) {
        q.push(current->right);
      }
    }

    return result;
  }

  // 获取树的高度
  int height() const {
    if (root == nullptr)
      return -1;

    std::queue<std::shared_ptr<BSTNode<T>>> q;
    q.push(root);
    int height = -1;

    while (!q.empty()) {
      int level_size = q.size();
      height++;

      for (int i = 0; i < level_size; i++) {
        auto current = q.front();
        q.pop();

        if (current->left != nullptr) {
          q.push(current->left);
        }
        if (current->right != nullptr) {
          q.push(current->right);
        }
      }
    }

    return height;
  }

  // 验证二叉搜索树性质
  bool is_valid() const {
    if (root == nullptr)
      return true;

    std::stack<std::shared_ptr<BSTNode<T>>> s;
    std::shared_ptr<BSTNode<T>> current = root;
    T prev;
    bool first = true;

    while (current != nullptr || !s.empty()) {
      while (current != nullptr) {
        s.push(current);
        current = current->left;
      }

      current = s.top();
      s.pop();

      if (!first && current->key <= prev) {
        return false;
      }

      first = false;
      prev = current->key;
      current = current->right;
    }

    return true;
  }

  // 清空树
  void clear() { root = nullptr; }

  // 打印树结构（用于调试）
  void print_tree() const {
    if (root == nullptr) {
      std::cout << "Empty tree" << std::endl;
      return;
    }

    std::queue<std::shared_ptr<BSTNode<T>>> q;
    q.push(root);

    while (!q.empty()) {
      int level_size = q.size();

      for (int i = 0; i < level_size; i++) {
        auto current = q.front();
        q.pop();

        if (current == nullptr) {
          std::cout << "null ";
        } else {
          std::cout << current->key << " ";
          q.push(current->left);
          q.push(current->right);
        }
      }
      std::cout << std::endl;
    }
  }
};

} // namespace algorithms

#endif // BINARY_SEARCH_TREE_H
