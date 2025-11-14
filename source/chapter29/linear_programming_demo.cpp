#include "linear_programming.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

/**
 * @brief 打印向量
 * @param vec 向量
 * @param name 向量名称
 */
template <typename T>
void print_vector(const std::vector<T> &vec, const std::string &name) {
  std::cout << name << " = [";
  for (size_t i = 0; i < vec.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << vec[i];
    if (i < vec.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl;
}

/**
 * @brief 打印矩阵
 * @param matrix 矩阵
 * @param name 矩阵名称
 */
void print_matrix(const std::vector<std::vector<double>> &matrix,
                  const std::string &name) {
  std::cout << name << " = " << std::endl;
  for (const auto &row : matrix) {
    std::cout << "  [";
    for (size_t j = 0; j < row.size(); ++j) {
      std::cout << std::fixed << std::setprecision(4) << std::setw(8) << row[j];
      if (j < row.size() - 1) {
        std::cout << " ";
      }
    }
    std::cout << "]" << std::endl;
  }
}

/**
 * @brief 打印线性规划问题
 * @param A 约束矩阵
 * @param b 约束右端项
 * @param c 目标函数系数
 */
void print_linear_program(const std::vector<std::vector<double>> &A,
                          const std::vector<double> &b,
                          const std::vector<double> &c) {
  std::cout << "线性规划问题：" << std::endl;
  std::cout << "最大化: z = ";
  for (size_t j = 0; j < c.size(); ++j) {
    if (j > 0) {
      std::cout << " + ";
    }
    std::cout << std::fixed << std::setprecision(2) << c[j] << "x" << (j + 1);
  }
  std::cout << std::endl;

  std::cout << "满足约束：" << std::endl;
  for (size_t i = 0; i < A.size(); ++i) {
    std::cout << "  ";
    for (size_t j = 0; j < A[i].size(); ++j) {
      if (j > 0) {
        std::cout << " + ";
      }
      std::cout << std::fixed << std::setprecision(2) << A[i][j] << "x"
                << (j + 1);
    }
    std::cout << " ≤ " << std::fixed << std::setprecision(2) << b[i]
              << std::endl;
  }
  std::cout << "  x1, x2, ... ≥ 0" << std::endl;
}

/**
 * @brief 打印线性规划求解结果
 * @param result 求解结果
 */
void print_lp_result(const LinearProgramming::LPResult &result) {
  std::cout << "求解结果：" << std::endl;
  std::cout << "  状态: " << result.status << std::endl;
  std::cout << "  可行性: " << (result.feasible ? "可行" : "不可行")
            << std::endl;
  std::cout << "  有界性: " << (result.bounded ? "有界" : "无界") << std::endl;

  if (result.feasible && result.bounded) {
    std::cout << "  最优目标函数值: " << std::fixed << std::setprecision(6)
              << result.objective_value << std::endl;

    if (!result.solution.empty()) {
      std::cout << "  最优解: ";
      for (size_t i = 0; i < result.solution.size(); ++i) {
        std::cout << "x" << (i + 1) << " = " << std::fixed
                  << std::setprecision(6) << result.solution[i];
        if (i < result.solution.size() - 1) {
          std::cout << ", ";
        }
      }
      std::cout << std::endl;
    }
  }
}

/**
 * @brief 性能测试函数
 * @param test_name 测试名称
 * @param test_func 测试函数
 */
void performance_test(const std::string &test_name,
                      std::function<void()> test_func) {
  auto start = std::chrono::high_resolution_clock::now();
  test_func();
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << test_name << " - 时间: " << duration.count() << " 微秒"
            << std::endl;
}

/**
 * @brief 测试算法导论第29章示例问题
 */
void test_textbook_example() {
  std::cout << "=== 算法导论第29章示例问题测试 ===" << std::endl;

  // 示例1：简单的线性规划问题
  std::cout << "\n示例1：简单线性规划问题" << std::endl;
  std::vector<std::vector<double>> A1 = {{1, 1}, {2, 1}, {1, 0}};
  std::vector<double> b1 = {4, 5, 2};
  std::vector<double> c1 = {3, 2};

  print_linear_program(A1, b1, c1);

  auto result1 = LinearProgramming::simplex_method(A1, b1, c1);
  print_lp_result(result1);

  // 验证最优性
  if (result1.feasible && result1.bounded) {
    std::string verification =
        LinearProgramming::verify_optimality(result1.solution, A1, b1, c1);
    std::cout << "  最优性验证: " << verification << std::endl;
  }

  // 示例2：无界问题
  std::cout << "\n示例2：无界线性规划问题" << std::endl;
  std::vector<std::vector<double>> A2 = {{-1, 1}, {1, -2}};
  std::vector<double> b2 = {1, 2};
  std::vector<double> c2 = {1, 1};

  print_linear_program(A2, b2, c2);

  auto result2 = LinearProgramming::simplex_method(A2, b2, c2);
  print_lp_result(result2);

  // 示例3：不可行问题
  std::cout << "\n示例3：不可行线性规划问题" << std::endl;
  std::vector<std::vector<double>> A3 = {{1, 1}, {-1, -1}};
  std::vector<double> b3 = {1, -2};
  std::vector<double> c3 = {1, 1};

  print_linear_program(A3, b3, c3);

  auto result3 = LinearProgramming::simplex_method(A3, b3, c3);
  print_lp_result(result3);
}

/**
 * @brief 测试两阶段单纯形法
 */
void test_two_phase_simplex() {
  std::cout << "\n=== 两阶段单纯形法测试 ===" << std::endl;

  // 需要两阶段法的问题
  std::vector<std::vector<double>> A = {{1, -1}, {-1, 1}, {1, 1}};
  std::vector<double> b = {1, 1, 3};
  std::vector<double> c = {2, 3};

  print_linear_program(A, b, c);

  auto result = LinearProgramming::two_phase_simplex(A, b, c);
  print_lp_result(result);

  if (result.feasible && result.bounded) {
    std::string verification =
        LinearProgramming::verify_optimality(result.solution, A, b, c);
    std::cout << "  最优性验证: " << verification << std::endl;
  }
}

/**
 * @brief 测试对偶单纯形法
 */
void test_dual_simplex() {
  std::cout << "\n=== 对偶单纯形法测试 ===" << std::endl;

  // 适合对偶单纯形法的问题
  std::vector<std::vector<double>> A = {{-1, -1}, {1, 0}, {0, 1}};
  std::vector<double> b = {-3, 2, 2};
  std::vector<double> c = {-1, -1};

  print_linear_program(A, b, c);

  auto result = LinearProgramming::dual_simplex_method(A, b, c);
  print_lp_result(result);

  if (result.feasible && result.bounded) {
    std::string verification =
        LinearProgramming::verify_optimality(result.solution, A, b, c);
    std::cout << "  最优性验证: " << verification << std::endl;
  }
}

/**
 * @brief 测试随机线性规划问题
 */
void test_random_problems() {
  std::cout << "\n=== 随机线性规划问题测试 ===" << std::endl;

  // 测试不同规模的随机问题
  std::vector<std::pair<size_t, size_t>> problem_sizes = {
      {3, 5}, // 小规模
      {5, 8}, // 中等规模
      {8, 12} // 较大规模
  };

  for (const auto &size : problem_sizes) {
    size_t m = size.first;  // 约束数量
    size_t n = size.second; // 变量数量

    std::cout << "\n随机问题 (" << m << "个约束, " << n
              << "个变量):" << std::endl;

    auto random_lp = LinearProgramming::generate_random_lp(m, n, 0.8);
    auto A = std::get<0>(random_lp);
    auto b = std::get<1>(random_lp);
    auto c = std::get<2>(random_lp);

    // 打印问题摘要
    std::cout << "  目标函数系数范围: ["
              << *std::min_element(c.begin(), c.end()) << ", "
              << *std::max_element(c.begin(), c.end()) << "]" << std::endl;
    std::cout << "  约束右端项范围: [" << *std::min_element(b.begin(), b.end())
              << ", " << *std::max_element(b.begin(), b.end()) << "]"
              << std::endl;

    // 性能测试
    performance_test("单纯形法", [&]() {
      auto result = LinearProgramming::simplex_method(A, b, c);

      std::cout << "  结果: " << result.status << std::endl;
      if (result.feasible && result.bounded) {
        std::cout << "  最优值: " << std::fixed << std::setprecision(4)
                  << result.objective_value << std::endl;
      }
    });
  }
}

/**
 * @brief 测试边界情况
 */
void test_edge_cases() {
  std::cout << "\n=== 边界情况测试 ===" << std::endl;

  // 空问题
  std::cout << "\n测试1：空输入" << std::endl;
  try {
    std::vector<std::vector<double>> empty_A;
    std::vector<double> empty_b;
    std::vector<double> empty_c;

    auto result = LinearProgramming::simplex_method(empty_A, empty_b, empty_c);
    print_lp_result(result);
  } catch (const std::exception &e) {
    std::cout << "  异常: " << e.what() << std::endl;
  }

  // 单变量问题
  std::cout << "\n测试2：单变量问题" << std::endl;
  std::vector<std::vector<double>> A_single = {{1}};
  std::vector<double> b_single = {5};
  std::vector<double> c_single = {2};

  print_linear_program(A_single, b_single, c_single);

  auto result_single =
      LinearProgramming::simplex_method(A_single, b_single, c_single);
  print_lp_result(result_single);

  // 零目标函数
  std::cout << "\n测试3：零目标函数" << std::endl;
  std::vector<std::vector<double>> A_zero = {{1, 1}, {1, -1}};
  std::vector<double> b_zero = {2, 1};
  std::vector<double> c_zero = {0, 0};

  print_linear_program(A_zero, b_zero, c_zero);

  auto result_zero = LinearProgramming::simplex_method(A_zero, b_zero, c_zero);
  print_lp_result(result_zero);
}

/**
 * @brief 比较不同算法的性能
 */
void compare_algorithms() {
  std::cout << "\n=== 算法性能比较 ===" << std::endl;

  // 生成一个中等规模的测试问题
  auto random_lp = LinearProgramming::generate_random_lp(6, 10, 0.7);
  auto A = std::get<0>(random_lp);
  auto b = std::get<1>(random_lp);
  auto c = std::get<2>(random_lp);

  std::cout << "测试问题: 6个约束, 10个变量" << std::endl;

  // 单纯形法
  performance_test("标准单纯形法", [&]() {
    auto result = LinearProgramming::simplex_method(A, b, c);
    std::cout << "  结果: "
              << (result.feasible && result.bounded ? "成功" : "失败")
              << std::endl;
  });

  // 两阶段单纯形法
  performance_test("两阶段单纯形法", [&]() {
    auto result = LinearProgramming::two_phase_simplex(A, b, c);
    std::cout << "  结果: "
              << (result.feasible && result.bounded ? "成功" : "失败")
              << std::endl;
  });

  // 对偶单纯形法
  performance_test("对偶单纯形法", [&]() {
    auto result = LinearProgramming::dual_simplex_method(A, b, c);
    std::cout << "  结果: "
              << (result.feasible && result.bounded ? "成功" : "失败")
              << std::endl;
  });
}

int main() {
  std::cout << "=== 算法导论 第29章 - 线性规划算法演示 ===" << std::endl;
  std::cout << std::endl;

  try {
    // 测试算法导论示例问题
    test_textbook_example();

    // 测试两阶段单纯形法
    test_two_phase_simplex();

    // 测试对偶单纯形法
    test_dual_simplex();

    // 测试随机问题
    test_random_problems();

    // 测试边界情况
    test_edge_cases();

    // 比较算法性能
    compare_algorithms();

    std::cout << "\n=== 测试完成 ===" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "测试过程中发生错误: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
