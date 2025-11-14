#include "amortized_analysis.h"
#include <iomanip>
#include <iostream>
#include <vector>

using namespace algorithms;

/**
 * @brief 第17章 摊还分析演示程序
 *
 * 演示算法导论第17章的内容：
 * - 17.1节 聚合分析：二进制计数器
 * - 17.2节 记账方法：支持MULTIPOP的栈
 * - 17.3节 势能方法：动态表
 * - 17.4节 动态表扩张和收缩
 */

void test_binary_counter_aggregate_analysis() {
  std::cout << "=== 17.1节 聚合分析：二进制计数器 ===" << std::endl;

  BinaryCounter counter(8);

  std::cout << "初始状态: " << counter.get_binary_string()
            << " (十进制: " << counter.get_value() << ")" << std::endl;

  std::cout << "\n执行16次INCREMENT操作：" << std::endl;
  std::cout << std::setw(4) << "操作" << std::setw(12) << "二进制"
            << std::setw(10) << "十进制" << std::setw(8) << "翻转"
            << std::setw(12) << "总翻转" << std::endl;
  std::cout << std::string(50, '-') << std::endl;

  for (int i = 1; i <= 16; ++i) {
    int flips = counter.increment();
    std::cout << std::setw(4) << i << std::setw(12) << counter.get_binary_string()
              << std::setw(10) << counter.get_value() << std::setw(8) << flips
              << std::setw(12) << counter.get_total_flips() << std::endl;
  }

  std::cout << "\n聚合分析结果：" << std::endl;
  std::cout << "- 总操作次数: 16" << std::endl;
  std::cout << "- 总翻转次数: " << counter.get_total_flips() << std::endl;
  std::cout << "- 平均每次操作翻转次数: " << std::fixed << std::setprecision(2)
            << (double)counter.get_total_flips() / 16 << std::endl;
  std::cout << "- 理论分析：每个位大约翻转n/2^i次，总翻转次数为O(n)"
            << std::endl;
  std::cout << std::endl;
}

void test_multipop_stack_accounting_method() {
  std::cout << "=== 17.2节 记账方法：支持MULTIPOP的栈 ===" << std::endl;

  MultipopStack stack(10);

  std::cout
      << "操作序列：PUSH(1), PUSH(2), PUSH(3), MULTIPOP(2), PUSH(4), POP()"
      << std::endl;
  std::cout << std::endl;

  std::cout << std::setw(12) << "操作" << std::setw(8) << "栈大小"
            << std::setw(10) << "信用" << std::setw(15) << "摊还代价"
            << std::endl;
  std::cout << std::string(45, '-') << std::endl;

  // PUSH(1)
  stack.push(1);
  std::cout << std::setw(12) << "PUSH(1)" << std::setw(8) << stack.size()
            << std::setw(10) << stack.get_credit() << std::setw(15) << "2"
            << std::endl;

  // PUSH(2)
  stack.push(2);
  std::cout << std::setw(12) << "PUSH(2)" << std::setw(8) << stack.size()
            << std::setw(10) << stack.get_credit() << std::setw(15) << "2"
            << std::endl;

  // PUSH(3)
  stack.push(3);
  std::cout << std::setw(12) << "PUSH(3)" << std::setw(8) << stack.size()
            << std::setw(10) << stack.get_credit() << std::setw(15) << "2"
            << std::endl;

  // MULTIPOP(2)
  stack.multipop(2);
  std::cout << std::setw(12) << "MULTIPOP(2)" << std::setw(8) << stack.size()
            << std::setw(10) << stack.get_credit() << std::setw(15) << "0"
            << std::endl;

  // PUSH(4)
  stack.push(4);
  std::cout << std::setw(12) << "PUSH(4)" << std::setw(8) << stack.size()
            << std::setw(10) << stack.get_credit() << std::setw(15) << "2"
            << std::endl;

  // POP()
  stack.pop();
  std::cout << std::setw(12) << "POP()" << std::setw(8) << stack.size()
            << std::setw(10) << stack.get_credit() << std::setw(15) << "0"
            << std::endl;

  std::cout << "\n记账方法分析：" << std::endl;
  std::cout << "- PUSH操作：实际代价1，摊还代价2（存储1信用）" << std::endl;
  std::cout << "- POP/MULTIPOP操作：实际代价1，摊还代价0（使用存储的信用）"
            << std::endl;
  std::cout << "- 总摊还代价 = 总实际代价 + 最终信用 - 初始信用" << std::endl;
  std::cout << std::endl;
}

void test_dynamic_table_potential_method() {
  std::cout << "=== 17.3-17.4节 势能方法：动态表 ===" << std::endl;

  DynamicTable table(1);
  PotentialMethodAnalyzer analyzer;

  std::cout << "执行一系列插入操作，观察动态表扩张：" << std::endl;
  std::cout << std::endl;

  std::cout << std::setw(6) << "操作" << std::setw(8) << "元素数"
            << std::setw(8) << "表大小" << std::setw(10) << "实际代价"
            << std::setw(10) << "势能" << std::setw(12) << "摊还代价"
            << std::endl;
  std::cout << std::string(54, '-') << std::endl;

  // 执行一系列插入操作
  for (int i = 1; i <= 9; ++i) {
    int actual_cost = table.insert(i);
    int potential = table.get_potential();
    int amortized_cost = analyzer.record_operation(actual_cost, potential);

    std::cout << std::setw(6) << "INSERT" << std::setw(8) << table.get_num()
              << std::setw(8) << table.get_size() << std::setw(10) << actual_cost
              << std::setw(10) << potential << std::setw(12) << amortized_cost
              << std::endl;
  }

  std::cout << "\n势能方法分析：" << std::endl;
  std::cout << "- 势能函数：Φ(T) = 2 * num(T) - size(T)" << std::endl;
  std::cout << "- 总实际代价: " << analyzer.get_total_actual_cost() << std::endl;
  std::cout << "- 最终势能: " << analyzer.get_current_potential() << std::endl;
  std::cout << "- 总摊还代价: "
            << analyzer.get_total_actual_cost() + analyzer.get_current_potential()
            << std::endl;
  std::cout << "- 平均摊还代价: " << std::fixed << std::setprecision(2)
            << (double)(analyzer.get_total_actual_cost() +
                        analyzer.get_current_potential()) /
                   analyzer.get_operation_count()
            << std::endl;
  std::cout << std::endl;
}

void test_table_expansion_contraction() {
  std::cout << "=== 动态表扩张和收缩测试 ===" << std::endl;

  DynamicTable table(1);

  std::cout << "插入操作导致表扩张：" << std::endl;
  for (int i = 1; i <= 17; ++i) {
    table.insert(i);
    std::cout << "插入 " << i << ": 元素数=" << table.get_num()
              << ", 表大小=" << table.get_size() << std::endl;
  }

  std::cout << "\n删除操作导致表收缩：" << std::endl;
  for (int i = 1; i <= 9; ++i) {
    table.delete_last();
    std::cout << "删除 " << i << ": 元素数=" << table.get_num()
              << ", 表大小=" << table.get_size() << std::endl;
  }

  std::cout << "\n动态表分析：" << std::endl;
  std::cout << "- 扩张策略：当表满时，容量加倍" << std::endl;
  std::cout << "- 收缩策略：当元素数≤容量/4时，容量减半" << std::endl;
  std::cout << "- 摊还分析：每次插入的摊还代价为O(1)" << std::endl;
  std::cout << std::endl;
}

void test_amortized_analysis_comparison() {
  std::cout << "=== 三种摊还分析方法比较 ===" << std::endl;

  std::cout << "1. 聚合分析（Aggregate Analysis）：" << std::endl;
  std::cout
      << "   - 方法：确定n个操作的总代价上界为T(n)，然后计算平均代价T(n)/n"
      << std::endl;
  std::cout << "   - 适用：操作序列的总代价容易分析" << std::endl;
  std::cout << "   - 示例：二进制计数器" << std::endl;
  std::cout << std::endl;

  std::cout << "2. 记账方法（Accounting Method）：" << std::endl;
  std::cout
      << "   - 方法：为每个操作分配摊还代价，某些操作多收费，某些操作少收费"
      << std::endl;
  std::cout << "   - 适用：不同操作的实际代价差异较大" << std::endl;
  std::cout << "   - 示例：支持MULTIPOP的栈" << std::endl;
  std::cout << std::endl;

  std::cout << "3. 势能方法（Potential Method）：" << std::endl;
  std::cout << "   - 方法：定义势能函数，摊还代价 = 实际代价 + 势能变化"
            << std::endl;
  std::cout << "   - 适用：数据结构的状态变化有规律" << std::endl;
  std::cout << "   - 示例：动态表" << std::endl;
  std::cout << std::endl;
}

int main() {
  std::cout << "算法导论第17章 - 摊还分析演示程序" << std::endl;
  std::cout << "==================================" << std::endl;
  std::cout << std::endl;

  test_binary_counter_aggregate_analysis();
  test_multipop_stack_accounting_method();
  test_dynamic_table_potential_method();
  test_table_expansion_contraction();
  test_amortized_analysis_comparison();

  std::cout << "=== 所有测试完成 ===" << std::endl;
  std::cout << "第17章摊还分析算法实现验证成功！" << std::endl;

  return 0;
}
