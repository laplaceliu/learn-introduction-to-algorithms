#include "divide_and_conquer.h"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

/**
 * @brief 生成随机数组
 * @param size 数组大小
 * @param min_val 最小值
 * @param max_val 最大值
 * @return 随机数组
 */
std::vector<int> generate_random_array(int size, int min_val = -50,
                                       int max_val = 50) {
  std::vector<int> arr(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min_val, max_val);

  for (int i = 0; i < size; ++i) {
    arr[i] = dis(gen);
  }

  return arr;
}

/**
 * @brief 生成随机矩阵
 * @param size 矩阵大小
 * @param min_val 最小值
 * @param max_val 最大值
 * @return 随机矩阵
 */
std::vector<std::vector<int>> generate_random_matrix(int size, int min_val = 0,
                                                     int max_val = 10) {
  std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min_val, max_val);

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix[i][j] = dis(gen);
    }
  }

  return matrix;
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
 * @brief 验证两个矩阵是否相等
 * @param A 矩阵A
 * @param B 矩阵B
 * @return 如果相等返回true
 */
bool matrices_equal(const std::vector<std::vector<int>> &A,
                    const std::vector<std::vector<int>> &B) {
  if (A.size() != B.size())
    return false;

  for (size_t i = 0; i < A.size(); ++i) {
    if (A[i].size() != B[i].size())
      return false;

    for (size_t j = 0; j < A[i].size(); ++j) {
      if (A[i][j] != B[i][j])
        return false;
    }
  }

  return true;
}

int main() {
  std::cout << "=== 算法导论 第4章 - 分治策略算法演示 ===" << std::endl;
  std::cout << std::endl;

  // 测试1：最大子数组问题 - 算法导论第4.1节示例
  std::cout << "测试1：最大子数组问题（算法导论第4.1节示例）" << std::endl;
  std::vector<int> stock_prices = {13, -3, -25, 20, -3,  -16, -23, 18,
                                   20, -7, 12,  -5, -22, 15,  -4,  7};
  DivideAndConquer::print_array(stock_prices, "股票价格变化");

  // 分治算法
  auto result_dc = DivideAndConquer::find_maximum_subarray(stock_prices);
  std::cout << "分治算法结果: 起始=" << result_dc.low
            << ", 结束=" << result_dc.high << ", 最大和=" << result_dc.sum
            << std::endl;

  // 暴力算法
  auto result_bf =
      DivideAndConquer::find_maximum_subarray_brute_force(stock_prices);
  std::cout << "暴力算法结果: 起始=" << result_bf.low
            << ", 结束=" << result_bf.high << ", 最大和=" << result_bf.sum
            << std::endl;

  // Kadane算法
  auto result_kadane =
      DivideAndConquer::find_maximum_subarray_kadane(stock_prices);
  std::cout << "Kadane算法结果: 起始=" << result_kadane.low
            << ", 结束=" << result_kadane.high
            << ", 最大和=" << result_kadane.sum << std::endl;

  // 验证结果一致性
  bool consistent =
      (result_dc.sum == result_bf.sum) && (result_dc.sum == result_kadane.sum);
  std::cout << "算法结果一致性验证: " << (consistent ? "通过" : "失败")
            << std::endl;

  // 测试2：矩阵乘法 - 算法导论第4.2节
  std::cout << std::endl << "测试2：矩阵乘法" << std::endl;
  std::vector<std::vector<int>> A = {{1, 3}, {7, 5}};
  std::vector<std::vector<int>> B = {{6, 8}, {4, 2}};

  DivideAndConquer::print_matrix(A, "矩阵A");
  DivideAndConquer::print_matrix(B, "矩阵B");

  try {
    auto C_standard = DivideAndConquer::matrix_multiply(A, B);
    DivideAndConquer::print_matrix(C_standard, "标准矩阵乘法结果");

    auto C_strassen = DivideAndConquer::strassen_matrix_multiply(A, B);
    DivideAndConquer::print_matrix(C_strassen, "Strassen矩阵乘法结果");

    bool matrices_match = matrices_equal(C_standard, C_strassen);
    std::cout << "矩阵乘法结果验证: " << (matrices_match ? "通过" : "失败")
              << std::endl;
  } catch (const std::exception &e) {
    std::cout << "矩阵乘法错误: " << e.what() << std::endl;
  }

  // 测试3：递归式求解 - 算法导论第4.3-4.5节
  std::cout << std::endl << "测试3：递归式求解（主定理应用）" << std::endl;

  // 归并排序的递归式：T(n) = 2T(n/2) + O(n)
  std::cout << "归并排序: T(n) = 2T(n/2) + O(n) => "
            << DivideAndConquer::solve_recurrence(2, 2, "O(n)") << std::endl;

  // 二分查找的递归式：T(n) = T(n/2) + O(1)
  std::cout << "二分查找: T(n) = T(n/2) + O(1) => "
            << DivideAndConquer::solve_recurrence(1, 2, "O(1)") << std::endl;

  // Strassen矩阵乘法的递归式：T(n) = 7T(n/2) + O(n²)
  std::cout << "Strassen算法: T(n) = 7T(n/2) + O(n²) => "
            << DivideAndConquer::solve_recurrence(7, 2, "O(n²)") << std::endl;

  // 标准矩阵乘法的递归式：T(n) = 8T(n/2) + O(n²)
  std::cout << "标准矩阵乘法: T(n) = 8T(n/2) + O(n²) => "
            << DivideAndConquer::solve_recurrence(8, 2, "O(n²)") << std::endl;

  // 测试4：归并排序 - 分治策略的经典示例
  std::cout << std::endl << "测试4：归并排序" << std::endl;
  std::vector<int> sort_test = {5, 2, 4, 7, 1, 3, 2, 6};
  DivideAndConquer::print_array(sort_test, "排序前");

  DivideAndConquer::merge_sort(sort_test);
  DivideAndConquer::print_array(sort_test, "排序后");

  std::cout << "排序验证: "
            << (DivideAndConquer::is_sorted(sort_test) ? "通过" : "失败")
            << std::endl;

  // 测试5：边界情况测试
  std::cout << std::endl << "测试5：边界情况测试" << std::endl;

  // 空数组的最大子数组
  std::vector<int> empty;
  auto empty_result = DivideAndConquer::find_maximum_subarray(empty);
  std::cout << "空数组最大子数组: 起始=" << empty_result.low
            << ", 结束=" << empty_result.high << ", 和=" << empty_result.sum
            << std::endl;

  // 单元素数组
  std::vector<int> single = {42};
  auto single_result = DivideAndConquer::find_maximum_subarray(single);
  std::cout << "单元素数组最大子数组: 起始=" << single_result.low
            << ", 结束=" << single_result.high << ", 和=" << single_result.sum
            << std::endl;

  // 全负数数组
  std::vector<int> all_negative = {-5, -2, -8, -1, -3};
  auto negative_result = DivideAndConquer::find_maximum_subarray(all_negative);
  std::cout << "全负数数组最大子数组: 起始=" << negative_result.low
            << ", 结束=" << negative_result.high
            << ", 和=" << negative_result.sum << std::endl;

  // 测试6：性能比较
  std::cout << std::endl << "测试6：性能比较（数组大小: 100）" << std::endl;
  std::vector<int> perf_test = generate_random_array(100);

  performance_test("分治算法最大子数组", [&]() {
    DivideAndConquer::find_maximum_subarray(perf_test);
  });

  performance_test("暴力算法最大子数组", [&]() {
    DivideAndConquer::find_maximum_subarray_brute_force(perf_test);
  });

  performance_test("Kadane算法最大子数组", [&]() {
    DivideAndConquer::find_maximum_subarray_kadane(perf_test);
  });

  performance_test("归并排序", [&]() {
    std::vector<int> sort_copy = perf_test;
    DivideAndConquer::merge_sort(sort_copy);
  });

  // 测试7：矩阵乘法性能比较
  std::cout << std::endl
            << "测试7：矩阵乘法性能比较（矩阵大小: 16x16）" << std::endl;
  auto matrix_test = generate_random_matrix(16);

  performance_test("标准矩阵乘法", [&]() {
    try {
      DivideAndConquer::matrix_multiply(matrix_test, matrix_test);
    } catch (const std::exception &e) {
      // 忽略异常
    }
  });

  performance_test("Strassen矩阵乘法", [&]() {
    try {
      DivideAndConquer::strassen_matrix_multiply(matrix_test, matrix_test);
    } catch (const std::exception &e) {
      // 忽略异常
    }
  });

  // 测试8：算法导论第4.1节练习
  std::cout << std::endl << "测试8：算法导论第4.1节练习" << std::endl;

  // 练习4.1-1：全负数数组的情况
  std::vector<int> exercise_4_1_1 = {-1, -2, -3, -4, -5};
  auto ex_result = DivideAndConquer::find_maximum_subarray(exercise_4_1_1);
  DivideAndConquer::print_array(exercise_4_1_1, "练习4.1-1数组");
  std::cout << "最大子数组: 起始=" << ex_result.low
            << ", 结束=" << ex_result.high << ", 和=" << ex_result.sum
            << std::endl;

  // 练习4.1-2：暴力算法与分治算法对比
  std::vector<int> exercise_4_1_2 = generate_random_array(20, -10, 10);
  auto dc_result = DivideAndConquer::find_maximum_subarray(exercise_4_1_2);
  auto bf_result =
      DivideAndConquer::find_maximum_subarray_brute_force(exercise_4_1_2);

  bool exercise_correct = (dc_result.sum == bf_result.sum);
  std::cout << "练习4.1-2验证: " << (exercise_correct ? "通过" : "失败")
            << std::endl;

  // 测试9：大规模测试
  std::cout << std::endl << "测试9：大规模测试（数组大小: 1000）" << std::endl;
  std::vector<int> large_test = generate_random_array(1000);

  performance_test("分治算法（大规模）", [&]() {
    DivideAndConquer::find_maximum_subarray(large_test);
  });

  performance_test("Kadane算法（大规模）", [&]() {
    DivideAndConquer::find_maximum_subarray_kadane(large_test);
  });

  // 验证大规模测试结果的一致性
  auto large_dc = DivideAndConquer::find_maximum_subarray(large_test);
  auto large_kadane =
      DivideAndConquer::find_maximum_subarray_kadane(large_test);

  bool large_consistent = (large_dc.sum == large_kadane.sum);
  std::cout << "大规模测试结果一致性: " << (large_consistent ? "通过" : "失败")
            << std::endl;

  std::cout << std::endl << "=== 演示结束 ===" << std::endl;

  return 0;
}
