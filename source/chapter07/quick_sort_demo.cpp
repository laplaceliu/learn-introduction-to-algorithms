#include "quick_sort.h"
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
std::vector<int> generate_random_array(int size, int min_val = 1,
                                       int max_val = 100) {
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
 * @brief 生成包含重复元素的数组
 * @param size 数组大小
 * @return 包含重复元素的数组
 */
std::vector<int> generate_array_with_duplicates(int size) {
  std::vector<int> arr(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 10); // 小范围增加重复概率

  for (int i = 0; i < size; ++i) {
    arr[i] = dis(gen);
  }

  return arr;
}

/**
 * @brief 性能测试函数
 * @param arr 测试数组
 * @param sort_func 排序函数
 * @param test_name 测试名称
 */
void performance_test(std::vector<int> arr,
                      void (*sort_func)(std::vector<int> &),
                      const std::string &test_name) {
  auto start = std::chrono::high_resolution_clock::now();
  sort_func(arr);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << test_name << " - 时间: " << duration.count() << " 微秒"
            << std::endl;
  std::cout << test_name
            << " - 排序验证: " << (QuickSort::is_sorted(arr) ? "通过" : "失败")
            << std::endl;
}

int main() {
  std::cout << "=== 算法导论 第7章 - 快速排序算法演示 ===" << std::endl;
  std::cout << std::endl;

  // 测试1：基本快速排序
  std::cout << "测试1：基本快速排序" << std::endl;
  std::vector<int> test1 = {3, 7, 8, 5, 2, 1, 9, 5, 4};
  QuickSort::print_array(test1, "排序前");

  QuickSort::sort(test1);
  QuickSort::print_array(test1, "排序后");
  std::cout << "验证结果: " << (QuickSort::is_sorted(test1) ? "通过" : "失败")
            << std::endl;

  // 测试2：随机化快速排序
  std::cout << std::endl << "测试2：随机化快速排序" << std::endl;
  std::vector<int> test2 = {9, 7, 5, 11, 12, 2, 14, 3, 10, 6};
  QuickSort::print_array(test2, "排序前");

  QuickSort::randomized_sort(test2);
  QuickSort::print_array(test2, "排序后");
  std::cout << "验证结果: " << (QuickSort::is_sorted(test2) ? "通过" : "失败")
            << std::endl;

  // 测试3：三路快速排序（处理重复元素）
  std::cout << std::endl << "测试3：三路快速排序（处理重复元素）" << std::endl;
  std::vector<int> test3 = {5, 2, 5, 1, 5, 3, 5, 4, 5, 2};
  QuickSort::print_array(test3, "排序前");

  QuickSort::three_way_sort(test3);
  QuickSort::print_array(test3, "排序后");
  std::cout << "验证结果: " << (QuickSort::is_sorted(test3) ? "通过" : "失败")
            << std::endl;

  // 测试4：边界情况测试
  std::cout << std::endl << "测试4：边界情况测试" << std::endl;

  // 空数组
  std::vector<int> empty;
  QuickSort::sort(empty);
  std::cout << "空数组测试: " << (QuickSort::is_sorted(empty) ? "通过" : "失败")
            << std::endl;

  // 单元素数组
  std::vector<int> single = {42};
  QuickSort::sort(single);
  std::cout << "单元素数组测试: "
            << (QuickSort::is_sorted(single) ? "通过" : "失败") << std::endl;

  // 已排序数组
  std::vector<int> sorted = {1, 2, 3, 4, 5};
  QuickSort::sort(sorted);
  std::cout << "已排序数组测试: "
            << (QuickSort::is_sorted(sorted) ? "通过" : "失败") << std::endl;

  // 逆序数组
  std::vector<int> reverse = {5, 4, 3, 2, 1};
  QuickSort::sort(reverse);
  std::cout << "逆序数组测试: "
            << (QuickSort::is_sorted(reverse) ? "通过" : "失败") << std::endl;

  // 测试5：性能比较
  std::cout << std::endl << "测试5：性能比较（数组大小: 1000）" << std::endl;
  std::vector<int> perf_test = generate_random_array(1000);

  performance_test(perf_test, QuickSort::sort, "基本快速排序");

  std::vector<int> perf_test2 = perf_test; // 复制数组
  performance_test(perf_test2, QuickSort::randomized_sort, "随机化快速排序");

  // 测试6：大量重复元素的性能测试
  std::cout << std::endl
            << "测试6：大量重复元素性能测试（数组大小: 1000）" << std::endl;
  std::vector<int> dup_test = generate_array_with_duplicates(1000);

  performance_test(dup_test, QuickSort::sort, "基本快速排序（重复元素）");

  std::vector<int> dup_test2 = dup_test; // 复制数组
  performance_test(dup_test2, QuickSort::three_way_sort,
                   "三路快速排序（重复元素）");

  // 测试7：算法导论示例
  std::cout << std::endl << "测试7：算法导论第7.1节示例" << std::endl;
  std::vector<int> clrs_example = {2, 8, 7, 1, 3, 5, 6, 4};
  QuickSort::print_array(clrs_example, "原始数组");

  // 演示分区过程
  std::vector<int> partition_demo = clrs_example;
  int pivot_index =
      QuickSort::partition(partition_demo, 0, partition_demo.size() - 1);
  QuickSort::print_array(partition_demo, "分区后");
  std::cout << "基准元素位置: " << pivot_index
            << " (值: " << partition_demo[pivot_index] << ")" << std::endl;

  // 完整排序
  QuickSort::sort(clrs_example);
  QuickSort::print_array(clrs_example, "完整排序后");

  std::cout << std::endl << "=== 演示结束 ===" << std::endl;

  return 0;
}
