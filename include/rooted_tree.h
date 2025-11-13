#ifndef ROOTED_TREE_H
#define ROOTED_TREE_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <vector>

namespace algorithms {

/**
 * @brief 有根树节点结构
 */
template <typename T>
struct TreeNode : public std::enable_shared_from_this<TreeNode<T>> {
  T data;
  std::shared_ptr<TreeNode<T>> parent;
  std::vector<std::shared_ptr<TreeNode<T>>> children;

  TreeNode(const T &value) : data(value), parent(nullptr) {}

  /**
   * @brief 添加子节点
   * @param child 要添加的子节点
   */
  void add_child(std::shared_ptr<TreeNode<T>> child) {
    child->parent = this->shared_from_this();
    children.push_back(child);
  }

  /**
   * @brief 判断是否为叶子节点
   * @return 如果是叶子节点返回true，否则返回false
   */
  bool is_leaf() const { return children.empty(); }

  /**
   * @brief 获取节点的深度（从根节点到该节点的路径长度）
   * @return 节点的深度
   */
  int depth() const {
    int d = 0;
    auto current = parent;
    while (current != nullptr) {
      d++;
      current = current->parent;
    }
    return d;
  }

  /**
   * @brief 获取节点的高度（从该节点到最远叶子节点的路径长度）
   * @return 节点的高度
   */
  int height() const {
    if (is_leaf()) {
      return 0;
    }

    int max_height = 0;
    for (const auto &child : children) {
      max_height = std::max(max_height, child->height());
    }
    return max_height + 1;
  }
};

/**
 * @brief 有根树类
 *
 * 实现算法导论10.4节的有根树表示
 */
template <typename T> class RootedTree {
private:
  std::shared_ptr<TreeNode<T>> root;
  int size;

public:
  /**
   * @brief 默认构造函数
   */
  RootedTree() : root(nullptr), size(0) {}

  /**
   * @brief 构造函数，使用根节点值
   * @param root_value 根节点的值
   */
  RootedTree(const T &root_value) : size(1) {
    root = std::make_shared<TreeNode<T>>(root_value);
  }

  /**
   * @brief 获取根节点
   * @return 根节点指针
   */
  std::shared_ptr<TreeNode<T>> get_root() const { return root; }

  /**
   * @brief 设置根节点
   * @param new_root 新的根节点
   */
  void set_root(std::shared_ptr<TreeNode<T>> new_root) {
    root = new_root;
    if (root != nullptr) {
      root->parent = nullptr;
    }
    update_size();
  }

  /**
   * @brief 在指定父节点下插入子节点
   * @param parent 父节点
   * @param value 子节点的值
   * @return 新创建的子节点
   */
  std::shared_ptr<TreeNode<T>> insert_child(std::shared_ptr<TreeNode<T>> parent,
                                            const T &value) {
    if (parent == nullptr) {
      throw std::invalid_argument("父节点不能为空");
    }

    auto new_node = std::make_shared<TreeNode<T>>(value);
    parent->add_child(new_node);
    size++;
    return new_node;
  }

  /**
   * @brief 广度优先搜索（BFS）
   * @param value 要搜索的值
   * @return 包含该值的节点指针，如果未找到返回nullptr
   */
  std::shared_ptr<TreeNode<T>> bfs(const T &value) const {
    if (root == nullptr)
      return nullptr;

    std::queue<std::shared_ptr<TreeNode<T>>> q;
    q.push(root);

    while (!q.empty()) {
      auto current = q.front();
      q.pop();

      if (current->data == value) {
        return current;
      }

      for (const auto &child : current->children) {
        q.push(child);
      }
    }

    return nullptr;
  }

  /**
   * @brief 深度优先搜索（DFS）
   * @param value 要搜索的值
   * @return 包含该值的节点指针，如果未找到返回nullptr
   */
  std::shared_ptr<TreeNode<T>> dfs(const T &value) const {
    if (root == nullptr)
      return nullptr;

    std::stack<std::shared_ptr<TreeNode<T>>> s;
    s.push(root);

    while (!s.empty()) {
      auto current = s.top();
      s.pop();

      if (current->data == value) {
        return current;
      }

      // 将子节点逆序压入栈中，以保持从左到右的遍历顺序
      for (auto it = current->children.rbegin(); it != current->children.rend();
           ++it) {
        s.push(*it);
      }
    }

    return nullptr;
  }

  /**
   * @brief 获取树的大小（节点数量）
   * @return 树中节点的数量
   */
  int get_size() const { return size; }

  /**
   * @brief 检查树是否为空
   * @return 如果树为空返回true，否则返回false
   */
  bool empty() const { return root == nullptr; }

  /**
   * @brief 获取树的高度
   * @return 树的高度
   */
  int height() const {
    if (root == nullptr)
      return -1;
    return root->height();
  }

  /**
   * @brief 前序遍历
   * @param node 起始节点（默认为根节点）
   * @param result 存储遍历结果的向量
   */
  void preorder_traversal(std::shared_ptr<TreeNode<T>> node,
                          std::vector<T> &result) const {
    if (node == nullptr)
      return;

    result.push_back(node->data);
    for (const auto &child : node->children) {
      preorder_traversal(child, result);
    }
  }

  /**
   * @brief 后序遍历
   * @param node 起始节点（默认为根节点）
   * @param result 存储遍历结果的向量
   */
  void postorder_traversal(std::shared_ptr<TreeNode<T>> node,
                           std::vector<T> &result) const {
    if (node == nullptr)
      return;

    for (const auto &child : node->children) {
      postorder_traversal(child, result);
    }
    result.push_back(node->data);
  }

  /**
   * @brief 层序遍历
   * @return 层序遍历结果
   */
  std::vector<T> level_order_traversal() const {
    std::vector<T> result;
    if (root == nullptr)
      return result;

    std::queue<std::shared_ptr<TreeNode<T>>> q;
    q.push(root);

    while (!q.empty()) {
      auto current = q.front();
      q.pop();

      result.push_back(current->data);

      for (const auto &child : current->children) {
        q.push(child);
      }
    }

    return result;
  }

  /**
   * @brief 打印树的结构
   */
  void print_tree() const {
    if (root == nullptr) {
      std::cout << "空树" << std::endl;
      return;
    }

    std::queue<std::shared_ptr<TreeNode<T>>> q;
    q.push(root);

    int level = 0;
    while (!q.empty()) {
      int level_size = q.size();
      std::cout << "第" << level << "层: ";

      for (int i = 0; i < level_size; ++i) {
        auto current = q.front();
        q.pop();

        std::cout << current->data;
        if (current->parent != nullptr) {
          std::cout << "(父:" << current->parent->data << ")";
        } else {
          std::cout << "(根)";
        }

        if (!current->children.empty()) {
          std::cout << " [子节点: ";
          for (const auto &child : current->children) {
            std::cout << child->data << " ";
            q.push(child);
          }
          std::cout << "]";
        }

        if (i < level_size - 1) {
          std::cout << ", ";
        }
      }

      std::cout << std::endl;
      level++;
    }
  }

private:
  /**
   * @brief 更新树的大小
   */
  void update_size() {
    if (root == nullptr) {
      size = 0;
      return;
    }

    size = 0;
    std::queue<std::shared_ptr<TreeNode<T>>> q;
    q.push(root);

    while (!q.empty()) {
      auto current = q.front();
      q.pop();
      size++;

      for (const auto &child : current->children) {
        q.push(child);
      }
    }
  }
};

/**
 * @brief 二叉树节点结构（有根树的特例）
 */
template <typename T> struct BinaryTreeNode {
  T data;
  std::shared_ptr<BinaryTreeNode<T>> left;
  std::shared_ptr<BinaryTreeNode<T>> right;
  std::shared_ptr<BinaryTreeNode<T>> parent;

  BinaryTreeNode(const T &value)
      : data(value), left(nullptr), right(nullptr), parent(nullptr) {}

  bool is_leaf() const { return left == nullptr && right == nullptr; }
};

/**
 * @brief 二叉树类（有根树的特例）
 */
template <typename T> class BinaryTree {
private:
  std::shared_ptr<BinaryTreeNode<T>> root;
  int size;

public:
  BinaryTree() : root(nullptr), size(0) {}

  BinaryTree(const T &root_value) : size(1) {
    root = std::make_shared<BinaryTreeNode<T>>(root_value);
  }

  std::shared_ptr<BinaryTreeNode<T>> get_root() const { return root; }

  void set_root(std::shared_ptr<BinaryTreeNode<T>> new_root) {
    root = new_root;
    if (root != nullptr) {
      root->parent = nullptr;
    }
    update_size();
  }

  std::shared_ptr<BinaryTreeNode<T>>
  insert_left(std::shared_ptr<BinaryTreeNode<T>> parent, const T &value) {
    if (parent == nullptr) {
      throw std::invalid_argument("父节点不能为空");
    }

    auto new_node = std::make_shared<BinaryTreeNode<T>>(value);
    parent->left = new_node;
    new_node->parent = parent;
    size++;
    return new_node;
  }

  std::shared_ptr<BinaryTreeNode<T>>
  insert_right(std::shared_ptr<BinaryTreeNode<T>> parent, const T &value) {
    if (parent == nullptr) {
      throw std::invalid_argument("父节点不能为空");
    }

    auto new_node = std::make_shared<BinaryTreeNode<T>>(value);
    parent->right = new_node;
    new_node->parent = parent;
    size++;
    return new_node;
  }

  void inorder_traversal(std::shared_ptr<BinaryTreeNode<T>> node,
                         std::vector<T> &result) const {
    if (node == nullptr)
      return;

    inorder_traversal(node->left, result);
    result.push_back(node->data);
    inorder_traversal(node->right, result);
  }

  void preorder_traversal(std::shared_ptr<BinaryTreeNode<T>> node,
                          std::vector<T> &result) const {
    if (node == nullptr)
      return;

    result.push_back(node->data);
    preorder_traversal(node->left, result);
    preorder_traversal(node->right, result);
  }

  void postorder_traversal(std::shared_ptr<BinaryTreeNode<T>> node,
                           std::vector<T> &result) const {
    if (node == nullptr)
      return;

    postorder_traversal(node->left, result);
    postorder_traversal(node->right, result);
    result.push_back(node->data);
  }

  int get_size() const { return size; }

  bool empty() const { return root == nullptr; }

private:
  void update_size() {
    if (root == nullptr) {
      size = 0;
      return;
    }

    size = 0;
    std::stack<std::shared_ptr<BinaryTreeNode<T>>> s;
    s.push(root);

    while (!s.empty()) {
      auto current = s.top();
      s.pop();
      size++;

      if (current->right != nullptr) {
        s.push(current->right);
      }
      if (current->left != nullptr) {
        s.push(current->left);
      }
    }
  }
};

} // namespace algorithms

#endif // ROOTED_TREE_H
