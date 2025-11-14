#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace algorithms {

/**
 * @brief 斐波那契堆节点
 */
template <typename T> struct FibonacciHeapNode {
  T key;       // 关键字
  int degree;  // 度数（子节点数量）
  bool marked; // 标记（是否失去过子节点）

  std::shared_ptr<FibonacciHeapNode<T>> parent; // 父节点
  std::shared_ptr<FibonacciHeapNode<T>> child;  // 任意一个子节点
  std::shared_ptr<FibonacciHeapNode<T>> left;   // 左兄弟节点
  std::shared_ptr<FibonacciHeapNode<T>> right;  // 右兄弟节点

  FibonacciHeapNode(const T &k)
      : key(k), degree(0), marked(false), parent(nullptr), child(nullptr),
        left(nullptr), right(nullptr) {}
};

/**
 * @brief 斐波那契堆
 */
template <typename T> class FibonacciHeap {
private:
  std::shared_ptr<FibonacciHeapNode<T>> min_node; // 最小节点
  int node_count;                                 // 节点总数

  /**
   * @brief 将节点y插入到节点x的左边
   */
  void link_nodes(std::shared_ptr<FibonacciHeapNode<T>> x,
                  std::shared_ptr<FibonacciHeapNode<T>> y) {
    if (!x || !y)
      return;

    y->right = x;
    y->left = x->left;
    x->left->right = y;
    x->left = y;
  }

  /**
   * @brief 将节点插入到根链表中
   */
  void insert_to_root_list(std::shared_ptr<FibonacciHeapNode<T>> node) {
    if (!min_node) {
      // 堆为空，创建循环链表
      node->left = node;
      node->right = node;
      min_node = node;
    } else {
      // 插入到min_node的左边
      link_nodes(min_node, node);

      // 更新最小节点
      if (node->key < min_node->key) {
        min_node = node;
      }
    }
  }

  /**
   * @brief 从根链表中移除节点
   */
  void remove_from_root_list(std::shared_ptr<FibonacciHeapNode<T>> node) {
    if (!node)
      return;

    node->left->right = node->right;
    node->right->left = node->left;

    if (node == min_node && node->right == node) {
      min_node = nullptr;
    } else if (node == min_node) {
      min_node = node->right;
    }

    node->left = node;
    node->right = node;
  }

  /**
   * @brief 将节点y作为节点x的子节点
   */
  void link(std::shared_ptr<FibonacciHeapNode<T>> y,
            std::shared_ptr<FibonacciHeapNode<T>> x) {
    // 从根链表中移除y
    remove_from_root_list(y);

    // 将y作为x的子节点
    if (!x->child) {
      x->child = y;
      y->left = y;
      y->right = y;
    } else {
      link_nodes(x->child, y);
    }

    y->parent = x;
    x->degree++;
    y->marked = false;
  }

  /**
   * @brief 合并根链表
   */
  void consolidate() {
    if (!min_node)
      return;

    // 计算最大度数（基于节点数量的对数）
    int max_degree = static_cast<int>(log2(node_count)) + 1;
    std::vector<std::shared_ptr<FibonacciHeapNode<T>>> degree_table(
        max_degree + 1, nullptr);

    // 收集根链表中的所有节点
    std::vector<std::shared_ptr<FibonacciHeapNode<T>>> root_nodes;
    auto current = min_node;
    do {
      root_nodes.push_back(current);
      current = current->right;
    } while (current != min_node);

    // 处理每个根节点
    for (auto node : root_nodes) {
      auto x = node;
      int d = x->degree;

      // 合并相同度数的树
      while (degree_table[d]) {
        auto y = degree_table[d];

        // 确保x是关键值较小的节点
        if (x->key > y->key) {
          std::swap(x, y);
        }

        // 将y链接到x
        link(y, x);
        degree_table[d] = nullptr;
        d++;
      }

      degree_table[d] = x;
    }

    // 重建根链表
    min_node = nullptr;
    for (int i = 0; i <= max_degree; i++) {
      if (degree_table[i]) {
        insert_to_root_list(degree_table[i]);
      }
    }
  }

  /**
   * @brief 级联剪切
   */
  void cascading_cut(std::shared_ptr<FibonacciHeapNode<T>> node) {
    auto parent = node->parent;
    if (parent) {
      if (!node->marked) {
        node->marked = true;
      } else {
        // 剪切节点并递归处理父节点
        cut(node, parent);
        cascading_cut(parent);
      }
    }
  }

  /**
   * @brief 剪切节点
   */
  void cut(std::shared_ptr<FibonacciHeapNode<T>> node,
           std::shared_ptr<FibonacciHeapNode<T>> parent) {
    // 从父节点的子链表中移除node
    if (node->right == node) {
      parent->child = nullptr;
    } else {
      node->left->right = node->right;
      node->right->left = node->left;
      if (parent->child == node) {
        parent->child = node->right;
      }
    }

    parent->degree--;

    // 将node插入根链表
    node->parent = nullptr;
    node->marked = false;
    insert_to_root_list(node);
  }

public:
  /**
   * @brief 构造函数
   */
  FibonacciHeap() : min_node(nullptr), node_count(0) {}

  /**
   * @brief 检查堆是否为空
   */
  bool is_empty() const { return min_node == nullptr; }

  /**
   * @brief 获取堆中的最小关键字
   */
  T get_min() const {
    if (is_empty()) {
      throw std::runtime_error("堆为空");
    }
    return min_node->key;
  }

  /**
   * @brief 插入新节点
   */
  void insert(const T &key) {
    auto new_node = std::make_shared<FibonacciHeapNode<T>>(key);
    insert_to_root_list(new_node);
    node_count++;
  }

  /**
   * @brief 合并两个斐波那契堆
   */
  void merge(FibonacciHeap &other) {
    if (other.is_empty())
      return;

    if (is_empty()) {
      min_node = other.min_node;
    } else {
      // 连接两个根链表
      auto this_last = min_node->left;
      auto other_first = other.min_node;
      auto other_last = other.min_node->left;

      this_last->right = other_first;
      other_first->left = this_last;
      min_node->left = other_last;
      other_last->right = min_node;

      // 更新最小节点
      if (other.min_node->key < min_node->key) {
        min_node = other.min_node;
      }
    }

    node_count += other.node_count;

    // 清空另一个堆
    other.min_node = nullptr;
    other.node_count = 0;
  }

  /**
   * @brief 提取最小节点
   */
  T extract_min() {
    if (is_empty()) {
      throw std::runtime_error("堆为空");
    }

    auto min = min_node;
    T min_key = min->key;

    // 将min的所有子节点加入根链表
    if (min->child) {
      auto child = min->child;
      do {
        auto next_child = child->right;
        child->parent = nullptr;
        insert_to_root_list(child);
        child = next_child;
      } while (child != min->child);
    }

    // 从根链表中移除min
    remove_from_root_list(min);

    if (min == min->right) {
      min_node = nullptr;
    } else {
      min_node = min->right;
      consolidate();
    }

    node_count--;
    return min_key;
  }

  /**
   * @brief 减小关键字
   */
  void decrease_key(std::shared_ptr<FibonacciHeapNode<T>> node,
                    const T &new_key) {
    if (new_key > node->key) {
      throw std::invalid_argument("新关键字不能大于原关键字");
    }

    node->key = new_key;
    auto parent = node->parent;

    if (parent && node->key < parent->key) {
      cut(node, parent);
      cascading_cut(parent);
    }

    if (node->key < min_node->key) {
      min_node = node;
    }
  }

  /**
   * @brief 删除节点
   */
  void delete_node(std::shared_ptr<FibonacciHeapNode<T>> node) {
    // 将节点关键字减小到最小值，然后提取
    decrease_key(node, std::numeric_limits<T>::lowest());
    extract_min();
  }

  /**
   * @brief 获取堆的大小
   */
  int size() const { return node_count; }

  /**
   * @brief 打印堆结构（用于调试）
   */
  void print() const {
    if (is_empty()) {
      std::cout << "堆为空" << std::endl;
      return;
    }

    std::cout << "斐波那契堆结构:" << std::endl;
    std::cout << "最小关键字: " << min_node->key << std::endl;
    std::cout << "节点总数: " << node_count << std::endl;

    std::vector<std::shared_ptr<FibonacciHeapNode<T>>> root_nodes;
    auto current = min_node;
    do {
      root_nodes.push_back(current);
      current = current->right;
    } while (current != min_node);

    std::cout << "根链表: ";
    for (auto node : root_nodes) {
      std::cout << node->key << " ";
    }
    std::cout << std::endl;
  }
};

} // namespace algorithms

#endif // FIBONACCI_HEAP_H
