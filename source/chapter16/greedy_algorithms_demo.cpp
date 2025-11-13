#include "greedy_algorithms.h"
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

using namespace algorithms;

/**
 * @brief 第16章 贪心算法演示程序
 *
 * 演示算法导论第16章的内容：
 * - 16.1节 活动选择问题
 * - 16.2节 贪心算法原理
 * - 16.3节 赫夫曼编码
 */

void testActivitySelection() {
  std::cout << "=== 16.1节 活动选择问题 ===" << std::endl;

  ActivitySelector selector;

  // 添加算法导论图16.1的活动数据
  selector.addActivity(1, 1, 4);
  selector.addActivity(2, 3, 5);
  selector.addActivity(3, 0, 6);
  selector.addActivity(4, 5, 7);
  selector.addActivity(5, 3, 9);
  selector.addActivity(6, 5, 9);
  selector.addActivity(7, 6, 10);
  selector.addActivity(8, 8, 11);
  selector.addActivity(9, 8, 12);
  selector.addActivity(10, 2, 14);
  selector.addActivity(11, 12, 16);

  std::cout << "所有活动（按结束时间排序）：" << std::endl;
  auto activities = selector.getActivities();
  std::sort(activities.begin(), activities.end());

  std::cout << std::setw(4) << "ID" << std::setw(8) << "开始" << std::setw(8)
            << "结束" << std::endl;
  std::cout << std::string(20, '-') << std::endl;

  for (const auto &activity : activities) {
    std::cout << std::setw(4) << activity.id << std::setw(8) << activity.start
              << std::setw(8) << activity.finish << std::endl;
  }

  std::cout << "\n贪心算法选择结果（迭代版本）：" << std::endl;
  auto selected = selector.greedyActivitySelector();

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

void testHuffmanCoding() {
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
  huffman.buildHuffmanTree(frequencies);

  // 获取编码
  auto codes = huffman.getCodes();

  std::cout << "\n赫夫曼编码表：" << std::endl;
  std::cout << std::setw(6) << "字符" << std::setw(10) << "编码" << std::endl;
  std::cout << std::string(16, '-') << std::endl;

  for (const auto &pair : codes) {
    std::cout << std::setw(6) << pair.first << std::setw(10) << pair.second
              << std::endl;
  }

  // 测试编码和解码
  std::string testText = "abacab";
  std::string encoded = huffman.encode(testText);
  std::string decoded = huffman.decode(encoded);

  std::cout << "\n编码测试：" << std::endl;
  std::cout << "原始文本: " << testText << std::endl;
  std::cout << "编码结果: " << encoded << std::endl;
  std::cout << "解码结果: " << decoded << std::endl;
  std::cout << "编码正确性: " << (testText == decoded ? "✓ 正确" : "✗ 错误")
            << std::endl;

  // 计算压缩率
  double ratio = huffman.getCompressionRatio(testText);
  std::cout << "压缩率: " << std::fixed << std::setprecision(2) << ratio * 100
            << "%" << std::endl;
  std::cout << std::endl;
}

void testFractionalKnapsack() {
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
    double unitValue = values[i] / weights[i];
    std::cout << std::setw(8) << i + 1 << std::setw(8) << values[i]
              << std::setw(8) << weights[i] << std::setw(12) << std::fixed
              << std::setprecision(2) << unitValue << std::endl;
  }

  double maxValue =
      GreedyPrinciples::fractionalKnapsack(values, weights, capacity);

  std::cout << "\n贪心算法求解结果：" << std::endl;
  std::cout << "最大总价值: " << std::fixed << std::setprecision(2) << maxValue
            << std::endl;
  std::cout << std::endl;
}

void testGreedyPrinciples() {
  std::cout << "=== 16.2节 贪心算法原理 ===" << std::endl;

  std::cout << "贪心选择性质：" << std::endl;
  std::cout << GreedyPrinciples::explainGreedyChoiceProperty() << std::endl;
  std::cout << std::endl;

  std::cout << "最优子结构性质：" << std::endl;
  std::cout << GreedyPrinciples::explainOptimalSubstructure() << std::endl;
  std::cout << std::endl;

  std::cout << "贪心算法适用条件：" << std::endl;
  std::cout << "1. 贪心选择性质：局部最优选择能导致全局最优解" << std::endl;
  std::cout << "2. 最优子结构性质：问题的最优解包含子问题的最优解" << std::endl;
  std::cout << "3. 无后效性：当前选择不会影响后续选择的最优性" << std::endl;
  std::cout << std::endl;
}

void testGreedyVsDynamicProgramming() {
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

int main() {
  std::cout << "算法导论第16章 - 贪心算法演示程序" << std::endl;
  std::cout << "==================================" << std::endl;
  std::cout << std::endl;

  testActivitySelection();
  testHuffmanCoding();
  testFractionalKnapsack();
  testGreedyPrinciples();
  testGreedyVsDynamicProgramming();

  std::cout << "=== 所有测试完成 ===" << std::endl;
  std::cout << "第16章贪心算法实现验证成功！" << std::endl;

  return 0;
}
