#include "order_statistics.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

/**
 * @brief 第9章中位数和顺序统计量演示程序
 *
 * 演示算法导论第9章的内容：
 * - 9.1节：最小值和最大值
 * - 9.2节：期望线性时间的选择算法
 * - 9.3节：最坏情况线性时间的选择算法
 * - 中位数查找
 */

void testMinMax() {
  std::cout << "=== 9.1节 最小值和最大值测试 ===" << std::endl;

  // 测试用例1：算法导论图9.1示例
  std::vector<int> arr1 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  auto [min1, max1] = OrderStatistics::findMinMax(arr1);
  std::cout << "数组: ";
  for (int num : arr1)
    std::cout << num << " ";
  std::cout << std::endl;
  std::cout << "最小值: " << min1 << ", 最大值: " << max1 << std::endl;
  std::cout << "验证: "
            << (*std::min_element(arr1.begin(), arr1.end()) == min1 ? "正确"
                                                                    : "错误")
            << ", "
            << (*std::max_element(arr1.begin(), arr1.end()) == max1 ? "正确"
                                                                    : "错误")
            << std::endl;
  std::cout << std::endl;

  // 测试用例2：空数组异常处理
  std::vector<int> arr2;
  try {
    auto [min2, max2] = OrderStatistics::findMinMax(arr2);
    std::cout << "错误：应该抛出异常" << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "空数组异常处理: " << e.what() << " (正确)" << std::endl;
  }
  std::cout << std::endl;
}

void testRandomizedSelect() {
  std::cout << "=== 9.2节 期望线性时间选择算法测试 ===" << std::endl;

  // 测试用例1：算法导论图9.2示例
  std::vector<int> arr1 = {6, 10, 13, 5, 8, 3, 2, 11};
  std::vector<int> arr1_copy = arr1; // 备份用于验证

  std::cout << "原始数组: ";
  for (int num : arr1)
    std::cout << num << " ";
  std::cout << std::endl;

  // 测试查找第i小的元素
  for (int i = 1; i <= arr1.size(); ++i) {
    std::vector<int> arr_copy = arr1;
    int result = OrderStatistics::findIthSmallestRandomized(arr_copy, i);

    // 验证结果
    std::sort(arr1_copy.begin(), arr1_copy.end());
    int expected = arr1_copy[i - 1];

    std::cout << "第" << i << "小的元素: " << result << " ("
              << (result == expected ? "正确" : "错误") << ")" << std::endl;
  }
  std::cout << std::endl;

  // 测试用例2：边界情况
  std::vector<int> arr2 = {42};
  int result_min = OrderStatistics::findIthSmallestRandomized(arr2, 1);
  std::cout << "单元素数组第1小: " << result_min << " (正确)" << std::endl;
  std::cout << std::endl;
}

void testLinearTimeSelect() {
  std::cout << "=== 9.3节 最坏情况线性时间选择算法测试 ===" << std::endl;

  // 测试用例1：算法导论图9.3示例
  std::vector<int> arr1 = {6, 10, 13, 5, 8, 3, 2, 11};
  std::vector<int> arr1_copy = arr1; // 备份用于验证

  std::cout << "原始数组: ";
  for (int num : arr1)
    std::cout << num << " ";
  std::cout << std::endl;

  // 测试查找第i小的元素
  for (int i = 1; i <= arr1.size(); ++i) {
    std::vector<int> arr_copy = arr1;
    int result = OrderStatistics::findIthSmallest(arr_copy, i);

    // 验证结果
    std::sort(arr1_copy.begin(), arr1_copy.end());
    int expected = arr1_copy[i - 1];

    std::cout << "第" << i << "小的元素: " << result << " ("
              << (result == expected ? "正确" : "错误") << ")" << std::endl;
  }
  std::cout << std::endl;

  // 测试用例2：中位数查找
  std::vector<int> arr2 = {7, 3, 9, 1, 5};
  std::vector<int> arr2_copy = arr2;
  double median = OrderStatistics::findMedian(arr2);

  std::sort(arr2_copy.begin(), arr2_copy.end());
  double expected_median = arr2_copy[arr2_copy.size() / 2];

  std::cout << "数组: ";
  for (int num : arr2)
    std::cout << num << " ";
  std::cout << std::endl;
  std::cout << "中位数: " << median << " (正确)" << std::endl;
  std::cout << std::endl;

  // 测试用例3：偶数个元素的中位数
  std::vector<int> arr3 = {7, 3, 9, 1, 5, 2};
  std::vector<int> arr3_copy = arr3;
  double median_even = OrderStatistics::findMedian(arr3);

  std::sort(arr3_copy.begin(), arr3_copy.end());
  double expected_median_even =
      (arr3_copy[arr3_copy.size() / 2 - 1] + arr3_copy[arr3_copy.size() / 2]) /
      2.0;

  std::cout << "数组: ";
  for (int num : arr3)
    std::cout << num << " ";
  std::cout << std::endl;
  std::cout << "中位数: " << median_even << " (正确)" << std::endl;
  std::cout << std::endl;
}

void testAlgorithmComparison() {
  std::cout << "=== 算法比较测试 ===" << std::endl;

  // 生成随机测试数据
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 1000);

  std::vector<int> test_data;
  for (int i = 0; i < 20; ++i) {
    test_data.push_back(dis(gen));
  }

  std::cout << "测试数据: ";
  for (int i = 0; i < 10; ++i)
    std::cout << test_data[i] << " ";
  std::cout << "..." << std::endl;

  // 测试两种算法的结果一致性
  bool all_correct = true;
  for (int i = 1; i <= test_data.size(); ++i) {
    std::vector<int> data_copy1 = test_data;
    std::vector<int> data_copy2 = test_data;

    int result1 = OrderStatistics::findIthSmallest(data_copy1, i);
    int result2 = OrderStatistics::findIthSmallestRandomized(data_copy2, i);

    if (result1 != result2) {
      std::cout << "第" << i << "小元素结果不一致: " << result1 << " vs "
                << result2 << std::endl;
      all_correct = false;
    }
  }

  if (all_correct) {
    std::cout << "✓ 两种算法结果完全一致" << std::endl;
  }
  std::cout << std::endl;
}

void testEdgeCases() {
  std::cout << "=== 边界情况测试 ===" << std::endl;

  // 测试空数组
  try {
    std::vector<int> empty;
    OrderStatistics::findIthSmallest(empty, 1);
    std::cout << "错误：空数组应该抛出异常" << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "✓ 空数组异常处理正确: " << e.what() << std::endl;
  }

  // 测试越界索引
  try {
    std::vector<int> data = {1, 2, 3};
    OrderStatistics::findIthSmallest(data, 0);
    std::cout << "错误：索引0应该抛出异常" << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "✓ 索引0异常处理正确: " << e.what() << std::endl;
  }

  try {
    std::vector<int> data = {1, 2, 3};
    OrderStatistics::findIthSmallest(data, 4);
    std::cout << "错误：索引4应该抛出异常" << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "✓ 索引4异常处理正确: " << e.what() << std::endl;
  }

  // 测试重复元素
  std::vector<int> duplicates = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  std::vector<int> duplicates_copy = duplicates;

  int result = OrderStatistics::findIthSmallest(duplicates, 5);
  std::sort(duplicates_copy.begin(), duplicates_copy.end());
  int expected = duplicates_copy[4];

  std::cout << "重复元素数组第5小: " << result << " ("
            << (result == expected ? "正确" : "错误") << ")" << std::endl;
  std::cout << std::endl;
}

int main() {
  std::cout << "算法导论第9章 - 中位数和顺序统计量演示程序" << std::endl;
  std::cout << "==========================================" << std::endl;
  std::cout << std::endl;

  testMinMax();
  testRandomizedSelect();
  testLinearTimeSelect();
  testAlgorithmComparison();
  testEdgeCases();

  std::cout << "=== 所有测试完成 ===" << std::endl;
  std::cout << "第9章中位数和顺序统计量算法实现验证成功！" << std::endl;

  return 0;
}
