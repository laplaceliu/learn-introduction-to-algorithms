#include "probabilistic_analysis.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

/**
 * @brief 生成随机候选人质量数组
 * @param n 候选人数量
 * @param min_quality 最小质量
 * @param max_quality 最大质量
 * @return 候选人质量数组
 */
std::vector<int> generate_candidates(int n, int min_quality = 1,
                                     int max_quality = 100) {
  std::vector<int> candidates(n);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min_quality, max_quality);

  for (int i = 0; i < n; ++i) {
    candidates[i] = dis(gen);
  }

  return candidates;
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

int main() {
  std::cout << "=== 算法导论 第5章 - 概率分析和随机算法演示 ===" << std::endl;
  std::cout << std::endl;

  // 测试1：基本雇佣问题
  std::cout << "测试1：基本雇佣问题" << std::endl;
  std::vector<int> candidates1 = {86, 35, 92, 41, 78, 95, 63, 29, 88, 71};
  ProbabilisticAnalysis::print_array(candidates1, "候选人质量");

  int k = 3; // 策略参数：面试前3个但不雇佣
  int hired_index = ProbabilisticAnalysis::hiring_problem(candidates1, k);

  std::cout << "策略参数 k = " << k << std::endl;
  std::cout << "雇佣的候选人索引: " << hired_index << std::endl;
  std::cout << "雇佣的候选人质量: " << candidates1[hired_index] << std::endl;

  // 找到最佳候选人
  int best_candidate =
      *std::max_element(candidates1.begin(), candidates1.end());
  std::cout << "最佳候选人质量: " << best_candidate << std::endl;
  std::cout << "是否雇佣到最佳候选人: "
            << (candidates1[hired_index] == best_candidate ? "是" : "否")
            << std::endl;

  // 测试2：在线雇佣问题
  std::cout << std::endl << "测试2：在线雇佣问题" << std::endl;
  double interview_cost = 1.0;
  double hiring_cost = 10.0;

  double total_cost = ProbabilisticAnalysis::online_hiring_problem(
      candidates1, interview_cost, hiring_cost);
  std::cout << "面试成本: " << interview_cost << ", 雇佣成本: " << hiring_cost
            << std::endl;
  std::cout << "总成本: " << total_cost << std::endl;

  // 测试3：随机排列生成
  std::cout << std::endl << "测试3：随机排列生成" << std::endl;
  int n = 10;
  std::vector<int> permutation = ProbabilisticAnalysis::random_permutation(n);
  ProbabilisticAnalysis::print_array(permutation, "随机排列");

  // 验证排列的正确性
  bool valid_permutation = true;
  for (int i = 1; i <= n; ++i) {
    if (!ProbabilisticAnalysis::contains(permutation, i)) {
      valid_permutation = false;
      break;
    }
  }
  std::cout << "排列验证: " << (valid_permutation ? "通过" : "失败")
            << std::endl;

  // 测试4：随机化雇佣问题
  std::cout << std::endl << "测试4：随机化雇佣问题" << std::endl;
  int hired_random =
      ProbabilisticAnalysis::randomized_hiring_problem(candidates1, k);
  std::cout << "随机化后雇佣的候选人索引: " << hired_random << std::endl;
  std::cout << "随机化后雇佣的候选人质量: " << candidates1[hired_random]
            << std::endl;

  // 测试5：生日悖论模拟
  std::cout << std::endl << "测试5：生日悖论模拟" << std::endl;
  std::vector<int> group_sizes = {10, 20, 23, 30, 40, 50};
  int trials = 10000;

  std::cout << "模拟次数: " << trials << std::endl;
  std::cout << std::fixed << std::setprecision(2);

  for (int size : group_sizes) {
    double probability = ProbabilisticAnalysis::birthday_paradox(size, trials);
    std::cout << "人数=" << size << ": " << probability * 100 << "%"
              << std::endl;
  }

  // 测试6：球与箱子问题
  std::cout << std::endl << "测试6：球与箱子问题" << std::endl;
  std::vector<std::pair<int, int>> test_cases = {
      {100, 10}, // 100个球，10个箱子
      {100, 20}, // 100个球，20个箱子
      {100, 50}, // 100个球，50个箱子
      {100, 100} // 100个球，100个箱子
  };

  for (const auto &[balls, bins] : test_cases) {
    double expected_max_load =
        ProbabilisticAnalysis::balls_and_bins(balls, bins, trials);
    std::cout << "球=" << balls << ", 箱子=" << bins
              << ": 最大负载期望值=" << expected_max_load << std::endl;
  }

  // 测试7：随机化快速选择
  std::cout << std::endl << "测试7：随机化快速选择算法" << std::endl;
  std::vector<int> select_test = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  ProbabilisticAnalysis::print_array(select_test, "原始数组");

  // 选择第i小的元素
  for (int i = 1; i <= 5; ++i) {
    std::vector<int> arr_copy = select_test;
    int ith_smallest = ProbabilisticAnalysis::randomized_select(
        arr_copy, 0, arr_copy.size() - 1, i);
    std::cout << "第" << i << "小的元素: " << ith_smallest << std::endl;
  }

  // 测试8：概率分析 - 最优k值分析
  std::cout << std::endl << "测试8：雇佣问题最优k值分析" << std::endl;
  int candidate_count = 10;
  int analysis_trials = 5000;

  auto optimal_k_analysis = ProbabilisticAnalysis::analyze_optimal_k(
      candidate_count, analysis_trials);
  ProbabilisticAnalysis::print_probability_analysis(
      optimal_k_analysis, "不同k值下雇佣到最佳候选人的概率");

  // 找到最优k值
  auto best_k = std::max_element(
      optimal_k_analysis.begin(), optimal_k_analysis.end(),
      [](const auto &a, const auto &b) { return a.second < b.second; });

  if (best_k != optimal_k_analysis.end()) {
    std::cout << "最优k值: " << best_k->first
              << " (成功率: " << best_k->second * 100 << "%)" << std::endl;
  }

  // 测试9：性能比较
  std::cout << std::endl << "测试9：性能比较" << std::endl;
  std::vector<int> large_candidates = generate_candidates(1000);

  performance_test("随机排列生成 (n=1000)",
                   []() { ProbabilisticAnalysis::random_permutation(1000); });

  performance_test("生日悖论模拟 (n=23, trials=10000)", []() {
    ProbabilisticAnalysis::birthday_paradox(23, 10000);
  });

  performance_test("球与箱子问题 (balls=100, bins=10, trials=10000)", []() {
    ProbabilisticAnalysis::balls_and_bins(100, 10, 10000);
  });

  // 测试10：算法导论示例
  std::cout << std::endl << "测试10：算法导论第5.1节示例" << std::endl;
  std::vector<int> clrs_example = {1, 2, 3, 4, 5,
                                   6, 7, 8, 9, 10}; // 假设质量递增

  // 随机排列示例
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(clrs_example.begin(), clrs_example.end(), gen);

  ProbabilisticAnalysis::print_array(clrs_example, "随机排列后的候选人质量");

  // 应用不同k值的策略
  for (int k_test : {1, 3, 5}) {
    int hired = ProbabilisticAnalysis::hiring_problem(clrs_example, k_test);
    std::cout << "k=" << k_test << "时雇佣的候选人: 索引=" << hired
              << ", 质量=" << clrs_example[hired] << std::endl;
  }

  std::cout << std::endl << "=== 演示结束 ===" << std::endl;

  return 0;
}
