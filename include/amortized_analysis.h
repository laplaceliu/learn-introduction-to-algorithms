#ifndef AMORTIZED_ANALYSIS_H
#define AMORTIZED_ANALYSIS_H

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace algorithms {

/**
 * @brief 第17章 摊还分析实现
 *
 * 实现算法导论第17章的摊还分析方法和经典数据结构：
 * - 17.1节 聚合分析
 * - 17.2节 记账方法
 * - 17.3节 势能方法
 * - 17.4节 动态表
 */

/**
 * @brief 二进制计数器 - 17.1节聚合分析示例
 *
 * 实现一个二进制计数器，通过聚合分析证明n次INCREMENT操作的总代价为O(n)
 */
class BinaryCounter {
private:
  std::vector<bool> bits; // 二进制位，从低位到高位存储
  int flip_count;         // 记录翻转操作的总次数

public:
  /**
   * @brief 构造函数
   * @param size 计数器的位数
   */
  BinaryCounter(int size = 8) : flip_count(0) { bits.resize(size, false); }

  /**
   * @brief 执行INCREMENT操作
   * @return 翻转的总次数（实际代价）
   */
  int increment() {
    int i = 0;
    int flips = 0;

    // 从最低位开始，找到第一个0位
    while (i < bits.size() && bits[i]) {
      bits[i] = false; // 翻转1为0
      flips++;
      i++;
    }

    if (i < bits.size()) {
      bits[i] = true; // 翻转0为1
      flips++;
    } else {
      throw std::overflow_error("计数器溢出");
    }

    flip_count += flips;
    return flips;
  }

  /**
   * @brief 获取当前计数值
   * @return 十进制表示的计数值
   */
  int get_value() const {
    int value = 0;
    for (int i = 0; i < bits.size(); ++i) {
      if (bits[i]) {
        value += (1 << i);
      }
    }
    return value;
  }

  /**
   * @brief 获取二进制表示
   * @return 二进制字符串（高位在前）
   */
  std::string get_binary_string() const {
    std::string result;
    for (int i = bits.size() - 1; i >= 0; --i) {
      result += bits[i] ? '1' : '0';
    }
    return result;
  }

  /**
   * @brief 获取总翻转次数
   * @return 所有INCREMENT操作的总翻转次数
   */
  int get_total_flips() const { return flip_count; }

  /**
   * @brief 重置计数器
   */
  void reset() {
    std::fill(bits.begin(), bits.end(), false);
    flip_count = 0;
  }
};

/**
 * @brief 栈操作 - 17.2节记账方法示例
 *
 * 实现支持MULTIPOP操作的栈，使用记账方法分析摊还代价
 */
class MultipopStack {
private:
  std::vector<int> data;
  int top_index;
  int capacity;
  int credit; // 记账方法的信用

public:
  /**
   * @brief 构造函数
   * @param size 栈的容量
   */
  MultipopStack(int size = 100) : top_index(-1), capacity(size), credit(0) {
    data.resize(capacity);
  }

  /**
   * @brief PUSH操作 - 摊还代价为2
   * @param x 要压入的元素
   */
  void push(int x) {
    if (top_index >= capacity - 1) {
      throw std::overflow_error("栈溢出");
    }
    data[++top_index] = x;
    credit += 1; // 为将来的POP操作存储信用
  }

  /**
   * @brief POP操作 - 摊还代价为0
   * @return 弹出的元素
   */
  int pop() {
    if (top_index < 0) {
      throw std::underflow_error("栈下溢");
    }
    credit -= 1; // 使用存储的信用
    return data[top_index--];
  }

  /**
   * @brief MULTIPOP操作 - 摊还代价为0
   * @param k 要弹出的元素数量
   */
  void multipop(int k) {
    if (k > top_index + 1) {
      k = top_index + 1;
    }

    for (int i = 0; i < k; ++i) {
      pop();
    }
  }

  /**
   * @brief 检查栈是否为空
   * @return 栈是否为空
   */
  bool empty() const { return top_index < 0; }

  /**
   * @brief 获取当前信用
   * @return 当前信用值
   */
  int get_credit() const { return credit; }

  /**
   * @brief 获取栈大小
   * @return 栈中元素数量
   */
  int size() const { return top_index + 1; }
};

/**
 * @brief 动态表 - 17.4节动态表扩张和收缩
 *
 * 实现动态表，支持自动扩张和收缩，使用势能方法分析摊还代价
 */
class DynamicTable {
private:
  std::vector<int> table;
  int num;            // 表中元素数量
  int size;           // 表的容量
  int expansion_cost; // 扩张操作的总代价
  int insertion_cost; // 插入操作的总代价

  /**
   * @brief 扩张表
   */
  void expand_table() {
    int new_size = size * 2;
    std::vector<int> new_table(new_size);

    // 复制元素到新表
    for (int i = 0; i < num; ++i) {
      new_table[i] = table[i];
    }

    table = std::move(new_table);
    size = new_size;
    expansion_cost += num; // 扩张代价为当前元素数量
  }

  /**
   * @brief 收缩表
   */
  void contract_table() {
    int new_size = size / 2;
    std::vector<int> new_table(new_size);

    // 复制元素到新表
    for (int i = 0; i < num; ++i) {
      new_table[i] = table[i];
    }

    table = std::move(new_table);
    size = new_size;
    expansion_cost += num; // 收缩代价为当前元素数量
  }

public:
  /**
   * @brief 构造函数
   * @param initial_size 初始表大小
   */
  DynamicTable(int initial_size = 1)
      : num(0), size(initial_size), expansion_cost(0), insertion_cost(0) {
    table.resize(size);
  }

  /**
   * @brief 插入元素
   * @param x 要插入的元素
   * @return 实际代价
   */
  int insert(int x) {
    int actual_cost = 1; // 基本插入代价

    if (num == size) {
      expand_table();
      actual_cost += num; // 加上扩张代价
    }

    table[num++] = x;
    insertion_cost += actual_cost;
    return actual_cost;
  }

  /**
   * @brief 删除最后一个元素
   * @return 实际代价
   */
  int delete_last() {
    if (num == 0) {
      throw std::underflow_error("表为空");
    }

    int actual_cost = 1; // 基本删除代价

    if (num <= size / 4 && size > 1) {
      contract_table();
      actual_cost += num; // 加上收缩代价
    }

    num--;
    return actual_cost;
  }

  /**
   * @brief 获取表的大小
   * @return 表的容量
   */
  int get_size() const { return size; }

  /**
   * @brief 获取元素数量
   * @return 表中元素数量
   */
  int get_num() const { return num; }

  /**
   * @brief 获取总扩张代价
   * @return 扩张操作的总代价
   */
  int get_expansion_cost() const { return expansion_cost; }

  /**
   * @brief 获取总插入代价
   * @return 插入操作的总代价
   */
  int get_insertion_cost() const { return insertion_cost; }

  /**
   * @brief 获取势能函数值
   * @return 当前势能
   */
  int get_potential() const { return 2 * num - size; }

  /**
   * @brief 重置表
   */
  void reset() {
    num = 0;
    size = 1;
    table.resize(1);
    expansion_cost = 0;
    insertion_cost = 0;
  }
};

/**
 * @brief 势能方法分析器
 *
 * 用于演示势能方法的摊还分析
 */
class PotentialMethodAnalyzer {
private:
  int actual_cost_total;
  int potential_previous;
  int potential_current;
  int operation_count;

public:
  /**
   * @brief 构造函数
   */
  PotentialMethodAnalyzer()
      : actual_cost_total(0), potential_previous(0), potential_current(0),
        operation_count(0) {}

  /**
   * @brief 记录一次操作
   * @param actual_cost 实际代价
   * @param new_potential 新的势能值
   * @return 摊还代价
   */
  int record_operation(int actual_cost, int new_potential) {
    int amortized_cost = actual_cost + new_potential - potential_current;

    actual_cost_total += actual_cost;
    potential_previous = potential_current;
    potential_current = new_potential;
    operation_count++;

    return amortized_cost;
  }

  /**
   * @brief 获取总实际代价
   * @return 总实际代价
   */
  int get_total_actual_cost() const { return actual_cost_total; }

  /**
   * @brief 获取当前势能
   * @return 当前势能值
   */
  int get_current_potential() const { return potential_current; }

  /**
   * @brief 获取操作次数
   * @return 操作次数
   */
  int get_operation_count() const { return operation_count; }

  /**
   * @brief 重置分析器
   */
  void reset() {
    actual_cost_total = 0;
    potential_previous = 0;
    potential_current = 0;
    operation_count = 0;
  }
};

} // namespace algorithms

#endif // AMORTIZED_ANALYSIS_H
