#include "matrix_chain_multiplication.h"
#include <iostream>
#include <vector>

using namespace algorithms;

void test_algorithm_intro_example() {
  std::cout << "=== 测试算法导论示例（图15.3） ===" << std::endl;

  // 算法导论图15.3示例：矩阵链 <A1, A2, A3, A4, A5, A6>
  // 维度序列：<5, 10, 3, 12, 5, 50, 6>
  std::vector<int> dimensions = {5, 10, 3, 12, 5, 50, 6};

  MatrixChainMultiplication mcm(dimensions);

  std::cout
      << "矩阵链: A1(5×10), A2(10×3), A3(3×12), A4(12×5), A5(5×50), A6(50×6)"
      << std::endl;
  std::cout << "最小标量乘法次数: " << mcm.get_minimum_scalar_multiplications()
            << std::endl;
  std::cout << "最优括号化方案: " << mcm.get_optimal_parenthesization()
            << std::endl;

  // 验证算法导论中的结果
  std::cout << "\n验证子链结果:" << std::endl;
  std::cout << "m[2,5] = " << mcm.get_chain_cost(2, 5) << " (期望: 330)"
            << std::endl;
  std::cout << "s[2,5] = " << mcm.get_split_position(2, 5) << " (期望: 4)"
            << std::endl;

  std::cout << std::endl;
}

void test_small_chain() {
  std::cout << "=== 测试小规模矩阵链 ===" << std::endl;

  // 3个矩阵：A1(10×100), A2(100×5), A3(5×50)
  std::vector<int> dimensions = {10, 100, 5, 50};

  MatrixChainMultiplication mcm(dimensions);

  std::cout << "矩阵链: A1(10×100), A2(100×5), A3(5×50)" << std::endl;
  std::cout << "最小标量乘法次数: " << mcm.get_minimum_scalar_multiplications()
            << std::endl;
  std::cout << "最优括号化方案: " << mcm.get_optimal_parenthesization()
            << std::endl;

  // 手动计算验证
  // 方案1: (A1A2)A3 -> 10*100*5 + 10*5*50 = 5000 + 2500 = 7500
  // 方案2: A1(A2A3) -> 100*5*50 + 10*100*50 = 25000 + 50000 = 75000
  std::cout << "验证: 最优方案应该是 (A1A2)A3，乘法次数7500" << std::endl;

  std::cout << std::endl;
}

void test_memoized_version() {
  std::cout << "=== 测试备忘录版本 ===" << std::endl;

  std::vector<int> dimensions = {5, 10, 3, 12, 5, 50, 6};

  MatrixChainMemoized mcm_memo(dimensions);

  std::cout << "备忘录版本结果:" << std::endl;
  std::cout << "最小标量乘法次数: "
            << mcm_memo.get_minimum_scalar_multiplications() << std::endl;
  std::cout << "最优括号化方案: " << mcm_memo.get_optimal_parenthesization()
            << std::endl;

  std::cout << std::endl;
}

void test_dp_table_analysis() {
  std::cout << "=== 测试DP表分析 ===" << std::endl;

  // 简单示例用于分析DP表
  std::vector<int> dimensions = {30, 35, 15, 5, 10, 20, 25};

  MatrixChainMultiplication mcm(dimensions);

  std::cout << "矩阵链: A1(30×35), A2(35×15), A3(15×5), A4(5×10), A5(10×20), "
               "A6(20×25)"
            << std::endl;
  std::cout << "最小标量乘法次数: " << mcm.get_minimum_scalar_multiplications()
            << std::endl;
  std::cout << "最优括号化方案: " << mcm.get_optimal_parenthesization()
            << std::endl;

  std::cout << "\nDP表分析:" << std::endl;
  mcm.print_dp_tables();

  std::cout << std::endl;
}

void test_edge_cases() {
  std::cout << "=== 测试边界情况 ===" << std::endl;

  // 测试两个矩阵
  try {
    std::vector<int> dims2 = {10, 20};
    MatrixChainMultiplication mcm2(dims2);
    std::cout << "两个矩阵 A1(10×20): " << std::endl;
    std::cout << "最小乘法次数: " << mcm2.get_minimum_scalar_multiplications()
              << " (期望: 0)" << std::endl;
    std::cout << "括号化方案: " << mcm2.get_optimal_parenthesization()
              << std::endl;
  } catch (const std::exception &e) {
    std::cout << "异常: " << e.what() << std::endl;
  }

  // 测试无效输入
  try {
    std::vector<int> dims_invalid = {10};
    MatrixChainMultiplication mcm_invalid(dims_invalid);
    std::cout << "ERROR: 应该抛出异常" << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "正确捕获无效输入异常: " << e.what() << std::endl;
  }

  // 测试空输入
  try {
    std::vector<int> dims_empty = {};
    MatrixChainMultiplication mcm_empty(dims_empty);
    std::cout << "ERROR: 应该抛出异常" << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "正确捕获空输入异常: " << e.what() << std::endl;
  }

  std::cout << std::endl;
}

void test_chain_operations() {
  std::cout << "=== 测试子链操作 ===" << std::endl;

  std::vector<int> dimensions = {5, 10, 3, 12, 5, 50, 6};
  MatrixChainMultiplication mcm(dimensions);

  std::cout << "测试特定子链的最小乘法次数:" << std::endl;

  try {
    std::cout << "m[1,3] = " << mcm.get_chain_cost(1, 3) << std::endl;
    std::cout << "m[2,4] = " << mcm.get_chain_cost(2, 4) << std::endl;
    std::cout << "m[3,6] = " << mcm.get_chain_cost(3, 6) << std::endl;

    std::cout << "\n测试特定子链的分割位置:" << std::endl;
    std::cout << "s[1,4] = " << mcm.get_split_position(1, 4) << std::endl;
    std::cout << "s[2,5] = " << mcm.get_split_position(2, 5) << std::endl;
    std::cout << "s[3,6] = " << mcm.get_split_position(3, 6) << std::endl;

  } catch (const std::exception &e) {
    std::cout << "异常: " << e.what() << std::endl;
  }

  // 测试无效范围
  try {
    mcm.get_chain_cost(0, 3);
    std::cout << "ERROR: 应该抛出异常" << std::endl;
  } catch (const std::out_of_range &e) {
    std::cout << "正确捕获无效范围异常: " << e.what() << std::endl;
  }

  std::cout << std::endl;
}

void test_comparison_both_versions() {
  std::cout << "=== 比较两种实现版本 ===" << std::endl;

  std::vector<int> dimensions = {5, 10, 3, 12, 5, 50, 6};

  MatrixChainMultiplication mcm_dp(dimensions);
  MatrixChainMemoized mcm_memo(dimensions);

  std::cout << "动态规划版本结果:" << std::endl;
  std::cout << "最小乘法次数: " << mcm_dp.get_minimum_scalar_multiplications()
            << std::endl;
  std::cout << "括号化方案: " << mcm_dp.get_optimal_parenthesization()
            << std::endl;

  std::cout << "\n备忘录版本结果:" << std::endl;
  std::cout << "最小乘法次数: " << mcm_memo.get_minimum_scalar_multiplications()
            << std::endl;
  std::cout << "括号化方案: " << mcm_memo.get_optimal_parenthesization()
            << std::endl;

  // 验证结果一致性
  if (mcm_dp.get_minimum_scalar_multiplications() ==
      mcm_memo.get_minimum_scalar_multiplications()) {
    std::cout << "\n✓ 两种版本结果一致" << std::endl;
  } else {
    std::cout << "\n✗ 两种版本结果不一致" << std::endl;
  }

  std::cout << std::endl;
}

void test_larger_chain() {
  std::cout << "=== 测试较大规模矩阵链 ===" << std::endl;

  // 8个矩阵的链
  std::vector<int> dimensions = {10, 20, 30, 40, 30, 20, 10, 50, 5};

  MatrixChainMultiplication mcm(dimensions);

  std::cout << "8个矩阵的链:" << std::endl;
  std::cout << "最小标量乘法次数: " << mcm.get_minimum_scalar_multiplications()
            << std::endl;
  std::cout << "最优括号化方案: " << mcm.get_optimal_parenthesization()
            << std::endl;

  std::cout << "矩阵数量: " << mcm.get_matrix_count() << std::endl;

  std::cout << std::endl;
}

int main() {
  std::cout << "第15.2章 矩阵链乘法演示程序" << std::endl;
  std::cout << "=============================" << std::endl;

  test_algorithm_intro_example();
  test_small_chain();
  test_memoized_version();
  test_dp_table_analysis();
  test_edge_cases();
  test_chain_operations();
  test_comparison_both_versions();
  test_larger_chain();

  std::cout << "所有测试完成！" << std::endl;

  return 0;
}
