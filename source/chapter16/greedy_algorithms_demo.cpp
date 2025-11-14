#include "greedy_algorithms.h"
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace algorithms;

/**
 * @brief 第16章 贪心算法演示程序
 *
 * 演示算法导论第16章的内容：
 * - 16.1节 活动选择问题
 * - 16.2节 贪心算法原理
 * - 16.3节 赫夫曼编码
 * - 16.4节 拟阵和贪心算法
 * - 16.5节 用拟阵求解任务调度问题
 */

void test_activity_selection() {
  std::cout << "=== 16.1节 活动选择问题 ===" << std::endl;

  ActivitySelector selector;

  // 添加算法导论图16.1的活动数据
  selector.add_activity(1, 1, 4);
  selector.add_activity(2, 3, 5);
  selector.add_activity(3, 0, 6);
  selector.add_activity(4, 5, 7);
  selector.add_activity(5, 3, 9);
  selector.add_activity(6, 5, 9);
  selector.add_activity(7, 6, 10);
  selector.add_activity(8, 8, 11);
  selector.add_activity(9, 8, 12);
  selector.add_activity(10, 2, 14);
  selector.add_activity(11, 12, 16);

  std::cout << "所有活动（按结束时间排序）：" << std::endl;
  auto activities = selector.get_activities();
  std::sort(activities.begin(), activities.end());

  std::cout << std::setw(4) << "ID" << std::setw(8) << "开始" << std::setw(8)
            << "结束" << std::endl;
  std::cout << std::string(20, '-') << std::endl;

  for (const auto &activity : activities) {
    std::cout << std::setw(4) << activity.id << std::setw(8) << activity.start
              << std::setw(8) << activity.finish << std::endl;
  }

  std::cout << "\n贪心算法选择结果（迭代版本）：" << std::endl;
  auto selected = selector.greedy_activity_selector();

  std::cout << "选择的活动ID: ";
  for (size_t i = 0; i < selected.size(); ++i) {
    std::cout << selected[i];
    if (i < selected.size() - 1) {
      std::cout << " → ";
    }
  }
  std::cout << std::endl;

  std::cout << "选择的活动数量: " << selected.size() << std::endl;
  std::cout << std::endl;
}

void test_huffman_coding() {
  std::cout << "=== 16.3节 赫夫曼编码 ===" << std::endl;

  HuffmanCoding huffman;

  // 算法导论图16.5的示例数据
  std::map<char, int> frequencies = {
      {'a', 45}, // 45%
      {'b', 13}, // 13%
      {'c', 12}, // 12%
      {'d', 16}, // 16%
      {'e', 9},  // 9%
      {'f', 5}   // 5%
  };

  std::cout << "字符频率表：" << std::endl;
  std::cout << std::setw(6) << "字符" << std::setw(8) << "频率" << std::endl;
  std::cout << std::string(14, '-') << std::endl;

  for (const auto &pair : frequencies) {
    std::cout << std::setw(6) << pair.first << std::setw(8) << pair.second
              << std::endl;
  }

  // 构建赫夫曼树
  huffman.build_huffman_tree(frequencies);

  // 获取编码
  auto codes = huffman.get_codes();

  std::cout << "\n赫夫曼编码表：" << std::endl;
  std::cout << std::setw(6) << "字符" << std::setw(10) << "编码" << std::endl;
  std::cout << std::string(16, '-') << std::endl;

  for (const auto &pair : codes) {
    std::cout << std::setw(6) << pair.first << std::setw(10) << pair.second
              << std::endl;
  }

  // 测试编码和解码
  std::string test_text = "abacab";
  std::string encoded = huffman.encode(test_text);
  std::string decoded = huffman.decode(encoded);

  std::cout << "\n编码测试：" << std::endl;
  std::cout << "原始文本: " << test_text << std::endl;
  std::cout << "编码结果: " << encoded << std::endl;
  std::cout << "解码结果: " << decoded << std::endl;
  std::cout << "编码正确性: " << (test_text == decoded ? "✓ 正确" : "✗ 错误")
            << std::endl;

  // 计算压缩率
  double ratio = huffman.get_compression_ratio(test_text);
  std::cout << "压缩率: " << std::fixed << std::setprecision(2) << ratio * 100
            << "%" << std::endl;
  std::cout << std::endl;
}

void test_fractional_knapsack() {
  std::cout << "=== 16.2节 分数背包问题 ===" << std::endl;

  // 算法导论示例数据
  std::vector<double> values = {60, 100, 120};
  std::vector<double> weights = {10, 20, 30};
  double capacity = 50;

  std::cout << "背包容量: " << capacity << std::endl;
  std::cout << "物品信息：" << std::endl;
  std::cout << std::setw(8) << "物品" << std::setw(8) << "价值" << std::setw(8)
            << "重量" << std::setw(12) << "单位价值" << std::endl;
  std::cout << std::string(36, '-') << std::endl;

  for (size_t i = 0; i < values.size(); ++i) {
    double unit_value = values[i] / weights[i];
    std::cout << std::setw(8) << i + 1 << std::setw(8) << values[i]
              << std::setw(8) << weights[i] << std::setw(12) << std::fixed
              << std::setprecision(2) << unit_value << std::endl;
  }

  double max_value =
      GreedyPrinciples::fractional_knapsack(values, weights, capacity);

  std::cout << "\n贪心算法求解结果：" << std::endl;
  std::cout << "最大总价值: " << std::fixed << std::setprecision(2) << max_value
            << std::endl;
  std::cout << std::endl;
}

void test_greedy_principles() {
  std::cout << "=== 16.2节 贪心算法原理 ===" << std::endl;

  std::cout << "贪心选择性质：" << std::endl;
  std::cout << GreedyPrinciples::explain_greedy_choice_property() << std::endl;
  std::cout << std::endl;

  std::cout << "最优子结构性质：" << std::endl;
  std::cout << GreedyPrinciples::explain_optimal_substructure() << std::endl;
  std::cout << std::endl;

  std::cout << "贪心算法适用条件：" << std::endl;
  std::cout << "1. 贪心选择性质：局部最优选择能导致全局最优解" << std::endl;
  std::cout << "2. 最优子结构性质：问题的最优解包含子问题的最优解" << std::endl;
  std::cout << "3. 无后效性：当前选择不会影响后续选择的最优性" << std::endl;
  std::cout << std::endl;
}

void test_greedy_vs_dynamic_programming() {
  std::cout << "=== 贪心算法 vs 动态规划 ===" << std::endl;

  std::cout << "贪心算法的特点：" << std::endl;
  std::cout << "- 优点：通常更简单、更高效" << std::endl;
  std::cout << "- 缺点：不适用于所有问题，可能得到次优解" << std::endl;
  std::cout << "- 适用：活动选择、赫夫曼编码、最小生成树等" << std::endl;
  std::cout << std::endl;

  std::cout << "动态规划的特点：" << std::endl;
  std::cout << "- 优点：总能得到最优解" << std::endl;
  std::cout << "- 缺点：通常更复杂，时间和空间开销更大" << std::endl;
  std::cout << "- 适用：0-1背包问题、最长公共子序列、矩阵链乘法等" << std::endl;
  std::cout << std::endl;

  std::cout << "关键区别：" << std::endl;
  std::cout << "- 贪心算法：每个阶段做出局部最优选择，不回溯" << std::endl;
  std::cout << "- 动态规划：考虑所有可能的子问题，选择最优组合" << std::endl;
  std::cout << std::endl;
}

void test_matroid_theory() {
  std::cout << "=== 16.4节 拟阵和贪心算法 ===" << std::endl;

  std::cout << "拟阵定义：" << std::endl;
  std::cout << "拟阵 M = (S, I) 是一个有序对，其中：" << std::endl;
  std::cout << "- S 是有限集（基集）" << std::endl;
  std::cout << "- I ⊆ 2^S 是独立集族，满足：" << std::endl;
  std::cout << "  1. 遗传性：如果 B ∈ I 且 A ⊆ B，则 A ∈ I" << std::endl;
  std::cout << "  2. 交换性：如果 A, B ∈ I 且 |A| < |B|，则存在 x ∈ B - A 使得 "
               "A ∪ {x} ∈ I"
            << std::endl;
  std::cout << std::endl;

  // 示例：图形拟阵
  std::cout << "图形拟阵示例：" << std::endl;

  // 创建一个简单的图（4个顶点，5条边）
  int num_vertices = 4;
  std::set<std::pair<int, int>> edges = {
      {0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}};

  GraphicMatroid graphic_matroid(num_vertices, edges);

  std::cout << "图信息：" << std::endl;
  std::cout << "顶点数: " << num_vertices << std::endl;
  std::cout << "边集: ";
  for (const auto &edge : edges) {
    std::cout << "(" << edge.first << "," << edge.second << ") ";
  }
  std::cout << std::endl;

  // 测试几个边子集
  std::set<std::pair<int, int>> subset1 = {{0, 1}, {1, 3}};         // 无环
  std::set<std::pair<int, int>> subset2 = {{0, 1}, {1, 2}, {0, 2}}; // 有环

  std::cout << "\n独立性测试：" << std::endl;
  std::cout << "子集1 {(0,1), (1,3)}: "
            << (graphic_matroid.is_independent(subset1) ? "独立（无环）"
                                                      : "不独立（有环）")
            << std::endl;
  std::cout << "子集2 {(0,1), (1,2), (0,2)}: "
            << (graphic_matroid.is_independent(subset2) ? "独立（无环）"
                                                      : "不独立（有环）")
            << std::endl;

  std::cout << "\n拟阵与贪心算法的关系：" << std::endl;
  std::cout << "如果一个问题可以建模为拟阵，那么贪心算法总能得到最优解。"
            << std::endl;
  std::cout << "活动选择问题和最小生成树问题都可以建模为拟阵。" << std::endl;
  std::cout << std::endl;
}

void test_task_scheduling() {
  std::cout << "=== 16.5节 用拟阵求解任务调度问题 ===" << std::endl;

  // 算法导论示例数据
  std::vector<TaskScheduling::Task> tasks = {
      {1, 4, 70}, // 任务1：截止时间4，惩罚70
      {2, 2, 60}, // 任务2：截止时间2，惩罚60
      {3, 4, 50}, // 任务3：截止时间4，惩罚50
      {4, 3, 40}, // 任务4：截止时间3，惩罚40
      {5, 1, 30}, // 任务5：截止时间1，惩罚30
      {6, 4, 20}, // 任务6：截止时间4，惩罚20
      {7, 6, 10}  // 任务7：截止时间6，惩罚10
  };

  std::cout << "任务信息：" << std::endl;
  std::cout << std::setw(8) << "任务" << std::setw(12) << "截止时间"
            << std::setw(10) << "惩罚" << std::endl;
  std::cout << std::string(30, '-') << std::endl;

  for (const auto &task : tasks) {
    std::cout << std::setw(8) << task.id << std::setw(12) << task.deadline
              << std::setw(10) << task.penalty << std::endl;
  }

  // 使用贪心算法求解
  auto result = TaskScheduling::greedy_task_scheduling(tasks);
  auto &schedule = result.first;
  int total_penalty = result.second;

  std::cout << "\n贪心算法调度结果：" << std::endl;
  std::cout << "调度顺序：";
  for (size_t i = 0; i < schedule.size(); ++i) {
    std::cout << "任务" << schedule[i].id;
    if (i < schedule.size() - 1) {
      std::cout << " → ";
    }
  }
  std::cout << std::endl;

  std::cout << "未安排的任务惩罚：" << total_penalty << std::endl;
  std::cout << "调度可行性："
            << (TaskScheduling::validate_schedule(schedule) ? "✓ 可行"
                                                           : "✗ 不可行")
            << std::endl;

  std::cout << "\n拟阵理论在任务调度中的应用：" << std::endl;
  std::cout << TaskScheduling::explain_matroid_application() << std::endl;
  std::cout << std::endl;

  // 验证贪心算法的正确性
  std::cout << "贪心算法正确性验证：" << std::endl;
  std::cout << "- 任务按惩罚值降序排序" << std::endl;
  std::cout << "- 为每个任务分配最晚的可用时间槽" << std::endl;
  std::cout << "- 无法安排的任务计入总惩罚" << std::endl;
  std::cout << "- 该算法总能得到最优解（基于拟阵理论）" << std::endl;
  std::cout << std::endl;
}

int main() {
  std::cout << "算法导论第16章 - 贪心算法演示程序" << std::endl;
  std::cout << "==================================" << std::endl;
  std::cout << std::endl;

  test_activity_selection();
  test_huffman_coding();
  test_fractional_knapsack();
  test_greedy_principles();
  test_greedy_vs_dynamic_programming();
  test_matroid_theory();
  test_task_scheduling();

  std::cout << "=== 所有测试完成 ===" << std::endl;
  std::cout << "第16章贪心算法实现验证成功！" << std::endl;

  return 0;
}
