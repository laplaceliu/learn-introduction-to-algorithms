#ifndef INTERVAL_TREE_H
#define INTERVAL_TREE_H

#include "red_black_tree.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <vector>

namespace algorithms {

// 区间定义
struct Interval {
  int low;
  int high;

  Interval(int l = 0, int h = 0) : low(l), high(h) {
    if (low > high) {
      std::swap(low, high);
    }
  }

  // 判断区间是否重叠
  bool overlaps_with(const Interval &other) const {
    return !(high < other.low || low > other.high);
  }

  // 判断区间是否包含点
  bool contains(int point) const { return low <= point && point <= high; }

  // 重载比较运算符（按low排序）
  bool operator<(const Interval &other) const { return low < other.low; }

  bool operator==(const Interval &other) const {
    return low == other.low && high == other.high;
  }

  // 输出区间
  friend std::ostream &operator<<(std::ostream &os, const Interval &interval) {
    os << "[" << interval.low << ", " << interval.high << "]";
    return os;
  }
};

// 区间树节点
template <typename T> class IntervalTreeNode : public RBNode<T> {
public:
  Interval interval;
  T max; // 以该节点为根的子树中所有区间的最大端点值

  IntervalTreeNode(const Interval &i, Color c = Color::RED)
      : RBNode<T>(i.low, c), interval(i), max(i.high) {}

  // 更新max值
  void update_max() {
    this->max = interval.high;
    if (this->left != nullptr) {
      auto left_node =
          std::static_pointer_cast<IntervalTreeNode<T>>(this->left);
      this->max = std::max(this->max, left_node->max);
    }
    if (this->right != nullptr) {
      auto right_node =
          std::static_pointer_cast<IntervalTreeNode<T>>(this->right);
      this->max = std::max(this->max, right_node->max);
    }
  }
};

// 区间树
template <typename T> class IntervalTree {
private:
  std::shared_ptr<IntervalTreeNode<T>> root;
  std::shared_ptr<IntervalTreeNode<T>> nil;

  // 创建NIL节点
  std::shared_ptr<IntervalTreeNode<T>> create_nil() {
    auto node = std::make_shared<IntervalTreeNode<T>>(Interval(), Color::BLACK);
    node->max = std::numeric_limits<T>::min();
    node->left = nullptr;
    node->right = nullptr;
    return node;
  }

  // 左旋（重写以更新max值）
  void left_rotate(std::shared_ptr<IntervalTreeNode<T>> x) {
    if (x == nullptr || x->right == nil)
      return;

    auto y = std::static_pointer_cast<IntervalTreeNode<T>>(x->right);
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

    // 更新max值
    x->update_max();
    y->update_max();
  }

  // 右旋（重写以更新max值）
  void right_rotate(std::shared_ptr<IntervalTreeNode<T>> y) {
    if (y == nullptr || y->left == nil)
      return;

    auto x = std::static_pointer_cast<IntervalTreeNode<T>>(y->left);
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

    // 更新max值
    y->update_max();
    x->update_max();
  }

  // 插入修复（重写以更新max值）
  void insert_fixup(std::shared_ptr<IntervalTreeNode<T>> z) {
    while (z->parent != nil && z->parent->color == Color::RED) {
      if (z->parent == z->parent->parent->left) {
        auto y = std::static_pointer_cast<IntervalTreeNode<T>>(
            z->parent->parent->right);

        if (y != nil && y->color == Color::RED) {
          z->parent->color = Color::BLACK;
          y->color = Color::BLACK;
          z->parent->parent->color = Color::RED;
          z = std::static_pointer_cast<IntervalTreeNode<T>>(z->parent->parent);
        } else {
          if (z == z->parent->right) {
            z = std::static_pointer_cast<IntervalTreeNode<T>>(z->parent);
            left_rotate(z);
          }
          z->parent->color = Color::BLACK;
          z->parent->parent->color = Color::RED;
          right_rotate(
              std::static_pointer_cast<IntervalTreeNode<T>>(z->parent->parent));
        }
      } else {
        auto y = std::static_pointer_cast<IntervalTreeNode<T>>(
            z->parent->parent->left);

        if (y != nil && y->color == Color::RED) {
          z->parent->color = Color::BLACK;
          y->color = Color::BLACK;
          z->parent->parent->color = Color::RED;
          z = std::static_pointer_cast<IntervalTreeNode<T>>(z->parent->parent);
        } else {
          if (z == z->parent->left) {
            z = std::static_pointer_cast<IntervalTreeNode<T>>(z->parent);
            right_rotate(z);
          }
          z->parent->color = Color::BLACK;
          z->parent->parent->color = Color::RED;
          left_rotate(
              std::static_pointer_cast<IntervalTreeNode<T>>(z->parent->parent));
        }
      }
    }
    root->color = Color::BLACK;
  }

  // 移植操作（重写以更新max值）
  void transplant(std::shared_ptr<IntervalTreeNode<T>> u,
                  std::shared_ptr<IntervalTreeNode<T>> v) {
    if (u->parent == nil) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;

    // 更新max值
    auto current = v;
    while (current != nil) {
      current->update_max();
      current = std::static_pointer_cast<IntervalTreeNode<T>>(current->parent);
    }
  }

  // 查找最小节点
  std::shared_ptr<IntervalTreeNode<T>>
  minimum(std::shared_ptr<IntervalTreeNode<T>> node) const {
    while (node != nil && node->left != nil) {
      node = std::static_pointer_cast<IntervalTreeNode<T>>(node->left);
    }
    return node;
  }

  // 删除修复（重写以更新max值）
  void delete_fixup(std::shared_ptr<IntervalTreeNode<T>> x) {
    while (x != root && x->color == Color::BLACK) {
      if (x == x->parent->left) {
        auto w =
            std::static_pointer_cast<IntervalTreeNode<T>>(x->parent->right);

        if (w->color == Color::RED) {
          w->color = Color::BLACK;
          x->parent->color = Color::RED;
          left_rotate(std::static_pointer_cast<IntervalTreeNode<T>>(x->parent));
          w = std::static_pointer_cast<IntervalTreeNode<T>>(x->parent->right);
        }

        if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
          w->color = Color::RED;
          x = std::static_pointer_cast<IntervalTreeNode<T>>(x->parent);
        } else {
          if (w->right->color == Color::BLACK) {
            w->left->color = Color::BLACK;
            w->color = Color::RED;
            right_rotate(w);
            w = std::static_pointer_cast<IntervalTreeNode<T>>(x->parent->right);
          }
          w->color = x->parent->color;
          x->parent->color = Color::BLACK;
          w->right->color = Color::BLACK;
          left_rotate(std::static_pointer_cast<IntervalTreeNode<T>>(x->parent));
          x = root;
        }
      } else {
        auto w = std::static_pointer_cast<IntervalTreeNode<T>>(x->parent->left);

        if (w->color == Color::RED) {
          w->color = Color::BLACK;
          x->parent->color = Color::RED;
          right_rotate(
              std::static_pointer_cast<IntervalTreeNode<T>>(x->parent));
          w = std::static_pointer_cast<IntervalTreeNode<T>>(x->parent->left);
        }

        if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
          w->color = Color::RED;
          x = std::static_pointer_cast<IntervalTreeNode<T>>(x->parent);
        } else {
          if (w->left->color == Color::BLACK) {
            w->right->color = Color::BLACK;
            w->color = Color::RED;
            left_rotate(w);
            w = std::static_pointer_cast<IntervalTreeNode<T>>(x->parent->left);
          }
          w->color = x->parent->color;
          x->parent->color = Color::BLACK;
          w->left->color = Color::BLACK;
          right_rotate(
              std::static_pointer_cast<IntervalTreeNode<T>>(x->parent));
          x = root;
        }
      }
    }
    x->color = Color::BLACK;
  }

  // 区间搜索辅助函数
  std::shared_ptr<IntervalTreeNode<T>>
  interval_search_helper(std::shared_ptr<IntervalTreeNode<T>> node,
                         const Interval &interval) const {

    if (node == nil)
      return nullptr;

    // 如果当前节点的区间与查询区间重叠，返回当前节点
    if (node->interval.overlaps_with(interval)) {
      return node;
    }

    // 检查左子树
    if (node->left != nil) {
      auto left_node =
          std::static_pointer_cast<IntervalTreeNode<T>>(node->left);
      if (left_node->max >= interval.low) {
        auto result = interval_search_helper(left_node, interval);
        if (result != nullptr)
          return result;
      }
    }

    // 检查右子树
    if (node->right != nil) {
      auto right_node =
          std::static_pointer_cast<IntervalTreeNode<T>>(node->right);
      return interval_search_helper(right_node, interval);
    }

    return nullptr;
  }

  // 中序遍历辅助函数
  void inorder_helper(std::shared_ptr<IntervalTreeNode<T>> node,
                      std::vector<Interval> &result) const {
    if (node != nil) {
      inorder_helper(std::static_pointer_cast<IntervalTreeNode<T>>(node->left),
                     result);
      result.push_back(node->interval);
      inorder_helper(std::static_pointer_cast<IntervalTreeNode<T>>(node->right),
                     result);
    }
  }

public:
  IntervalTree() {
    nil = create_nil();
    root = nil;
  }

  // 判断树是否为空
  bool empty() const { return root == nil; }

  // 插入区间
  void insert(const Interval &interval) {
    auto z = std::make_shared<IntervalTreeNode<T>>(interval);
    z->left = nil;
    z->right = nil;
    z->color = Color::RED;

    auto y = nil;
    auto x = root;

    // 找到插入位置（按low排序）
    while (x != nil) {
      y = x;
      if (z->interval.low < x->interval.low) {
        x = std::static_pointer_cast<IntervalTreeNode<T>>(x->left);
      } else {
        x = std::static_pointer_cast<IntervalTreeNode<T>>(x->right);
      }
    }

    z->parent = y;

    if (y == nil) {
      root = z;
    } else if (z->interval.low < y->interval.low) {
      y->left = z;
    } else {
      y->right = z;
    }

    // 更新max值
    auto current = z;
    while (current != nil) {
      current->update_max();
      current = std::static_pointer_cast<IntervalTreeNode<T>>(current->parent);
    }

    insert_fixup(z);
  }

  // 区间搜索：查找与给定区间重叠的区间
  std::shared_ptr<IntervalTreeNode<T>>
  interval_search(const Interval &interval) const {
    return interval_search_helper(root, interval);
  }

  // 精确搜索：查找完全匹配的区间
  std::shared_ptr<IntervalTreeNode<T>> search(const Interval &interval) const {
    auto x = root;
    while (x != nil && !(x->interval == interval)) {
      if (interval.low < x->interval.low) {
        x = std::static_pointer_cast<IntervalTreeNode<T>>(x->left);
      } else {
        x = std::static_pointer_cast<IntervalTreeNode<T>>(x->right);
      }
    }
    return (x != nil) ? x : nullptr;
  }

  // 删除区间
  bool remove(const Interval &interval) {
    auto z = search(interval);
    if (z == nullptr) {
      return false; // 区间不存在
    }

    auto y = z;
    auto y_original_color = y->color;
    std::shared_ptr<IntervalTreeNode<T>> x;

    if (z->left == nil) {
      x = std::static_pointer_cast<IntervalTreeNode<T>>(z->right);
      transplant(z, x);
    } else if (z->right == nil) {
      x = std::static_pointer_cast<IntervalTreeNode<T>>(z->left);
      transplant(z, x);
    } else {
      y = minimum(std::static_pointer_cast<IntervalTreeNode<T>>(z->right));
      y_original_color = y->color;
      x = std::static_pointer_cast<IntervalTreeNode<T>>(y->right);

      if (y->parent == z) {
        x->parent = y;
      } else {
        transplant(y, x);
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

  // 获取最小区间（按low）
  Interval minimum() const {
    auto min_node = minimum(root);
    if (min_node == nil) {
      throw std::runtime_error("Tree is empty");
    }
    return min_node->interval;
  }

  // 获取最大区间（按low）
  Interval maximum() const {
    auto x = root;
    while (x != nil && x->right != nil) {
      x = std::static_pointer_cast<IntervalTreeNode<T>>(x->right);
    }
    if (x == nil) {
      throw std::runtime_error("Tree is empty");
    }
    return x->interval;
  }

  // 中序遍历（按low排序）
  std::vector<Interval> inorder_traversal() const {
    std::vector<Interval> result;
    inorder_helper(root, result);
    return result;
  }

  // 验证区间树性质
  bool is_valid() const {
    if (root == nil)
      return true;

    // 验证红黑树性质
    if (root->color != Color::BLACK) {
      return false;
    }

    // 验证max值正确性
    return validate_max_values(root);
  }

  // 验证max值正确性的辅助函数
  bool validate_max_values(std::shared_ptr<IntervalTreeNode<T>> node) const {
    if (node == nil)
      return true;

    T expected_max = node->interval.high;

    if (node->left != nil) {
      auto left_node =
          std::static_pointer_cast<IntervalTreeNode<T>>(node->left);
      expected_max = std::max(expected_max, left_node->max);
    }

    if (node->right != nil) {
      auto right_node =
          std::static_pointer_cast<IntervalTreeNode<T>>(node->right);
      expected_max = std::max(expected_max, right_node->max);
    }

    if (node->max != expected_max) {
      return false;
    }

    return validate_max_values(
               std::static_pointer_cast<IntervalTreeNode<T>>(node->left)) &&
           validate_max_values(
               std::static_pointer_cast<IntervalTreeNode<T>>(node->right));
  }

  // 打印树结构（用于调试）
  void print_tree() const {
    if (root == nil) {
      std::cout << "Empty tree" << std::endl;
      return;
    }

    std::queue<std::shared_ptr<IntervalTreeNode<T>>> q;
    q.push(root);

    while (!q.empty()) {
      int level_size = q.size();

      for (int i = 0; i < level_size; i++) {
        auto current = q.front();
        q.pop();

        if (current == nil) {
          std::cout << "NIL ";
        } else {
          std::cout << current->interval << "(max:" << current->max << ","
                    << (current->color == Color::RED ? "R" : "B") << ") ";
          q.push(std::static_pointer_cast<IntervalTreeNode<T>>(current->left));
          q.push(std::static_pointer_cast<IntervalTreeNode<T>>(current->right));
        }
      }
      std::cout << std::endl;
    }
  }

  // 清空树
  void clear() { root = nil; }
};

} // namespace algorithms

#endif // INTERVAL_TREE_H
