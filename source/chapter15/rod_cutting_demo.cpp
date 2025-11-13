#include "rod_cutting.h"
#include <iostream>
#include <string>
#include <vector>

using namespace algorithms;

void test_basic_example() {
  std::cout << "=== 测试基本示例（算法导论图15-1） ===" << std::endl;

  // 使用算法导论图15-1的价格表
  auto price_table = get_example_price_table();
  RodCutting rc(price_table);

  rc.print_price_table();
  std::cout << std::endl;

  // 测试不同长度的钢条切割
  std::vector<int> test_lengths = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (int length : test_lengths) {
    int memo_result = rc.cut_rod_memoized(length);
    int dp_result = rc.cut_rod_bottom_up(length);
    auto solution = rc.cut_rod_with_solution(length);

    std::cout << "钢条长度 " << length << ":" << std::endl;
    std::cout << "  递归解法（带备忘录）: " << memo_result << std::endl;
    std::cout << "  动态规划解法: " << dp_result << std::endl;
    std::cout << "  最大收益: " << solution.max_revenue << std::endl;
    std::cout << "  最优切割方案: ";

    if (solution.cuts.empty()) {
      std::cout << "不切割";
    } else {
      for (size_t i = 0; i < solution.cuts.size(); i++) {
        std::cout << solution.cuts[i];
        if (i < solution.cuts.size() - 1) {
          std::cout << " + ";
        }
      }
    }
    std::cout << std::endl;

    // 验证两种方法结果一致
    if (memo_result != dp_result || dp_result != solution.max_revenue) {
      std::cout << "错误：不同方法的结果不一致！" << std::endl;
    }
    std::cout << std::endl;
  }
}

void test_edge_cases() {
  std::cout << "=== 测试边界情况 ===" << std::endl;

  // 测试空价格表
  std::vector<int> empty_table;
  RodCutting rc_empty(empty_table);

  std::cout << "空价格表测试:" << std::endl;
  std::cout << "  长度0: " << rc_empty.cut_rod_memoized(0) << std::endl;
  std::cout << "  长度5: " << rc_empty.cut_rod_memoized(5) << std::endl;
  std::cout << std::endl;

  // 测试自定义价格表
  std::vector<int> custom_table = {0, 3, 5, 8, 9, 10, 17, 17, 20};
  RodCutting rc_custom(custom_table);

  rc_custom.print_price_table();
  std::cout << std::endl;

  // 测试超出价格表长度的钢条
  std::cout << "超出价格表长度测试:" << std::endl;
  for (int length : {12, 15, 20}) {
    auto solution = rc_custom.cut_rod_with_solution(length);
    std::cout << "  长度 " << length << " - 最大收益: " << solution.max_revenue;
    std::cout << ", 切割方案: ";

    for (size_t i = 0; i < solution.cuts.size(); i++) {
      std::cout << solution.cuts[i];
      if (i < solution.cuts.size() - 1) {
        std::cout << " + ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void test_performance_comparison() {
  std::cout << "=== 性能对比测试 ===" << std::endl;

  auto price_table = get_example_price_table();
  RodCutting rc(price_table);

  // 测试中等大小的钢条
  int test_length = 20;

  std::cout << "测试钢条长度: " << test_length << std::endl;

  // 测试动态规划解法
  int dp_result = rc.cut_rod_bottom_up(test_length);

  // 测试带备忘录的递归解法
  int memo_result = rc.cut_rod_memoized(test_length);

  std::cout << "动态规划解法结果: " << dp_result << std::endl;
  std::cout << "带备忘录递归解法结果: " << memo_result << std::endl;

  if (dp_result == memo_result) {
    std::cout << "结果验证: 正确" << std::endl;
  } else {
    std::cout << "结果验证: 错误" << std::endl;
  }

  std::cout << "注：性能对比需要包含chrono头文件，此处仅验证结果正确性"
            << std::endl;
  std::cout << std::endl;
}

void test_different_price_patterns() {
  std::cout << "=== 测试不同价格模式 ===" << std::endl;

  // 测试1：线性增长价格
  std::vector<int> linear_prices = {0};
  for (int i = 1; i <= 10; i++) {
    linear_prices.push_back(i * 2); // 价格线性增长
  }

  RodCutting rc_linear(linear_prices);
  std::cout << "线性增长价格模式:" << std::endl;
  rc_linear.print_price_table();

  for (int length : {5, 8, 10}) {
    auto solution = rc_linear.cut_rod_with_solution(length);
    std::cout << "  长度 " << length << " - 最优方案: 不切割" << std::endl;
    std::cout << "  收益: " << solution.max_revenue << std::endl;
  }
  std::cout << std::endl;

  // 测试2：递减边际收益
  std::vector<int> decreasing_prices = {0,  10, 19, 27, 34, 40,
                                        45, 49, 52, 54, 55};
  RodCutting rc_decreasing(decreasing_prices);

  std::cout << "递减边际收益模式:" << std::endl;
  rc_decreasing.print_price_table();

  for (int length : {4, 7, 10}) {
    auto solution = rc_decreasing.cut_rod_with_solution(length);
    std::cout << "  长度 " << length << " - 最大收益: " << solution.max_revenue;
    std::cout << ", 切割方案: ";

    for (size_t i = 0; i < solution.cuts.size(); i++) {
      std::cout << solution.cuts[i];
      if (i < solution.cuts.size() - 1) {
        std::cout << " + ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void test_algorithm_correctness() {
  std::cout << "=== 算法正确性验证 ===" << std::endl;

  auto price_table = get_example_price_table();
  RodCutting rc(price_table);

  // 验证算法导论中的几个关键例子
  struct TestCase {
    int length;
    int expected_revenue;
    std::vector<int> expected_cuts;
  };

  std::vector<TestCase> test_cases = {
      {4, 10, {2, 2}}, // 算法导论图15-2示例
      {7, 18, {1, 6}}, // 或者 {2, 2, 3} 等，收益相同
      {8, 22, {2, 6}}, // 或者 {2, 2, 2, 2} 等
      {10, 30, {10}}   // 不切割是最优的
  };

  for (const auto &test_case : test_cases) {
    auto solution = rc.cut_rod_with_solution(test_case.length);

    std::cout << "钢条长度 " << test_case.length << ":" << std::endl;
    std::cout << "  期望收益: " << test_case.expected_revenue << std::endl;
    std::cout << "  实际收益: " << solution.max_revenue << std::endl;
    std::cout << "  实际切割: ";

    for (size_t i = 0; i < solution.cuts.size(); i++) {
      std::cout << solution.cuts[i];
      if (i < solution.cuts.size() - 1) {
        std::cout << " + ";
      }
    }
    std::cout << std::endl;

    // 验证收益是否正确
    if (solution.max_revenue == test_case.expected_revenue) {
      std::cout << "  收益验证: 正确" << std::endl;
    } else {
      std::cout << "  收益验证: 错误" << std::endl;
    }

    // 验证切割方案收益是否等于最大收益
    int calculated_revenue = 0;
    for (int cut : solution.cuts) {
      calculated_revenue += rc.get_price(cut);
    }

    if (calculated_revenue == solution.max_revenue) {
      std::cout << "  方案验证: 正确" << std::endl;
    } else {
      std::cout << "  方案验证: 错误" << std::endl;
    }

    std::cout << std::endl;
  }
}

int main() {
  std::cout << "第15章 钢条切割演示程序" << std::endl;
  std::cout << "========================" << std::endl;

  test_basic_example();
  test_edge_cases();
  test_different_price_patterns();
  test_algorithm_correctness();

  // 注意：性能对比测试需要包含chrono头文件
  // test_performance_comparison();

  std::cout << "所有测试完成！" << std::endl;

  return 0;
}
