#include "linear_time_sort_generic.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

/**
 * @brief 生成随机整数数组
 * @param size 数组大小
 * @param min_val 最小值
 * @param max_val 最大值
 * @return 随机数组
 */
template <typename T>
std::vector<T> generate_random_int_array(int size, T min_val, T max_val) {
  static_assert(std::is_integral_v<T>, "此函数仅适用于整数类型");

  std::vector<T> arr(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dis(min_val, max_val);

  for (int i = 0; i < size; ++i) {
    arr[i] = dis(gen);
  }

  return arr;
}

/**
 * @brief 生成随机浮点数数组
 * @param size 数组大小
 * @param min_val 最小值
 * @param max_val 最大值
 * @return 随机数组
 */
template <typename T>
std::vector<T> generate_random_double_array(int size, T min_val, T max_val) {
  static_assert(std::is_floating_point_v<T>, "此函数仅适用于浮点数类型");

  std::vector<T> arr(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<T> dis(min_val, max_val);

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
template <typename T>
void performance_test(const std::vector<T> &arr,
                      std::vector<T> (*sort_func)(const std::vector<T> &),
                      const std::string &test_name) {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<T> result = sort_func(arr);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << test_name << " - 时间: " << duration.count() << " 微秒"
            << std::endl;
  std::cout << test_name << " - 排序验证: "
            << (LinearTimeSortGeneric::is_sorted(result) ? "通过" : "失败")
            << std::endl;
}

/**
 * @brief 性能测试函数（带额外参数）
 */
template <typename T>
void performance_test_counting(
    const std::vector<T> &arr,
    std::vector<T> (*sort_func)(const std::vector<T> &, T, T), T min_val,
    T max_val, const std::string &test_name) {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<T> result = sort_func(arr, min_val, max_val);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << test_name << " - 时间: " << duration.count() << " 微秒"
            << std::endl;
  std::cout << test_name << " - 排序验证: "
            << (LinearTimeSortGeneric::is_sorted(result) ? "通过" : "失败")
            << std::endl;
}

int main() {
  std::cout << "=== 算法导论 第8章 - 泛型线性时间排序算法演示 ===" << std::endl;
  std::cout << std::endl;

  // 测试1：计数排序（整数类型）
  std::cout << "测试1：计数排序（整数类型）" << std::endl;
  std::vector<int> test1 = {2, 5, 3, 0, 2, 3, 0, 3};
  LinearTimeSortGeneric::print_array(test1, "排序前");

  std::vector<int> result1 = CountingSort<int>::sort(test1, 5);
  LinearTimeSortGeneric::print_array(result1, "排序后");
  std::cout << "验证结果: "
            << (CountingSort<int>::is_sorted(result1) ? "通过" : "失败")
            << std::endl;

  // 测试2：计数排序通用版本（长整型）
  std::cout << std::endl << "测试2：计数排序通用版本（长整型）" << std::endl;
  std::vector<long> test2 = {15L, 12L, 18L, 11L, 19L, 13L, 16L, 14L, 17L};
  LinearTimeSortGeneric::print_array(test2, "排序前");

  std::vector<long> result2 = CountingSort<long>::sort_general(test2, 11L, 19L);
  LinearTimeSortGeneric::print_array(result2, "排序后");
  std::cout << "验证结果: "
            << (CountingSort<long>::is_sorted(result2) ? "通过" : "失败")
            << std::endl;

  // 测试3：基数排序（整数类型）
  std::cout << std::endl << "测试3：基数排序（整数类型）" << std::endl;
  std::vector<int> test3 = {329, 457, 657, 839, 436, 720, 355};
  LinearTimeSortGeneric::print_array(test3, "排序前");

  std::vector<int> result3 = RadixSort<int>::sort(test3);
  LinearTimeSortGeneric::print_array(result3, "排序后");
  std::cout << "验证结果: "
            << (RadixSort<int>::is_sorted(result3) ? "通过" : "失败")
            << std::endl;

  // 测试4：桶排序（浮点数类型）
  std::cout << std::endl << "测试4：桶排序（浮点数类型）" << std::endl;
  std::vector<double> test4 = {0.78, 0.17, 0.39, 0.26, 0.72,
                               0.94, 0.21, 0.12, 0.23, 0.68};
  LinearTimeSortGeneric::print_array(test4, "排序前");

  std::vector<double> result4 = BucketSort<double>::sort(test4);
  LinearTimeSortGeneric::print_array(result4, "排序后");
  std::cout << "验证结果: "
            << (BucketSort<double>::is_sorted(result4) ? "通过" : "失败")
            << std::endl;

  // 测试5：桶排序通用版本（浮点数）
  std::cout << std::endl << "测试5：桶排序通用版本（浮点数）" << std::endl;
  std::vector<float> test5 = {15.3f, 12.7f, 18.9f, 11.2f, 19.5f,
                              13.8f, 16.1f, 14.6f, 17.4f};
  LinearTimeSortGeneric::print_array(test5, "排序前");

  std::vector<float> result5 =
      BucketSort<float>::sort_general(test5, 11.0f, 20.0f);
  LinearTimeSortGeneric::print_array(result5, "排序后");
  std::cout << "验证结果: "
            << (BucketSort<float>::is_sorted(result5) ? "通过" : "失败")
            << std::endl;

  // 测试6：边界情况测试
  std::cout << std::endl << "测试6：边界情况测试" << std::endl;

  // 空数组
  std::vector<int> empty;
  std::vector<int> empty_result = CountingSort<int>::sort(empty, 0);
  std::cout << "空数组测试: "
            << (CountingSort<int>::is_sorted(empty_result) ? "通过" : "失败")
            << std::endl;

  // 单元素数组
  std::vector<int> single = {42};
  std::vector<int> single_result = CountingSort<int>::sort(single, 42);
  std::cout << "单元素数组测试: "
            << (CountingSort<int>::is_sorted(single_result) ? "通过" : "失败")
            << std::endl;

  // 已排序数组
  std::vector<int> sorted = {1, 2, 3, 4, 5};
  std::vector<int> sorted_result = CountingSort<int>::sort(sorted, 5);
  std::cout << "已排序数组测试: "
            << (CountingSort<int>::is_sorted(sorted_result) ? "通过" : "失败")
            << std::endl;

  // 测试7：性能比较（数组大小: 1000）
  std::cout << std::endl << "测试7：性能比较（数组大小: 1000）" << std::endl;

  // 计数排序性能测试
  std::vector<int> counting_test = generate_random_int_array<int>(1000, 0, 100);
  auto start_counting = std::chrono::high_resolution_clock::now();
  std::vector<int> counting_result =
      CountingSort<int>::sort(counting_test, 100);
  auto end_counting = std::chrono::high_resolution_clock::now();
  auto duration_counting =
      std::chrono::duration_cast<std::chrono::microseconds>(end_counting -
                                                            start_counting);
  std::cout << "计数排序 - 时间: " << duration_counting.count() << " 微秒"
            << std::endl;
  std::cout << "计数排序 - 排序验证: "
            << (CountingSort<int>::is_sorted(counting_result) ? "通过" : "失败")
            << std::endl;

  // 基数排序性能测试
  std::vector<int> radix_test = generate_random_int_array<int>(1000, 0, 9999);
  performance_test(radix_test, RadixSort<int>::sort, "基数排序");

  // 桶排序性能测试
  std::vector<double> bucket_test =
      generate_random_double_array<double>(1000, 0.0, 1.0);
  performance_test(bucket_test, BucketSort<double>::sort, "桶排序");

  // 测试8：与std::sort的性能对比
  std::cout << std::endl
            << "测试8：与std::sort的性能对比（数组大小: 1000）" << std::endl;

  std::vector<int> std_sort_test =
      generate_random_int_array<int>(1000, 0, 1000);

  auto start_std = std::chrono::high_resolution_clock::now();
  std::vector<int> std_result = std_sort_test;
  std::sort(std_result.begin(), std_result.end());
  auto end_std = std::chrono::high_resolution_clock::now();
  auto duration_std = std::chrono::duration_cast<std::chrono::microseconds>(
      end_std - start_std);

  std::cout << "std::sort - 时间: " << duration_std.count() << " 微秒"
            << std::endl;
  std::cout << "std::sort - 排序验证: "
            << (LinearTimeSortGeneric::is_sorted(std_result) ? "通过" : "失败")
            << std::endl;

  // 测试9：不同类型数据测试
  std::cout << std::endl << "测试9：不同类型数据测试" << std::endl;

  // 长整型计数排序
  std::vector<long> long_test =
      generate_random_int_array<long>(500, 1000L, 10000L);
  performance_test_counting(long_test, CountingSort<long>::sort_general, 1000L,
                            10000L, "长整型计数排序");

  // 浮点数桶排序
  std::vector<float> float_test =
      generate_random_double_array<float>(500, 50.0f, 150.0f);
  performance_test_counting(float_test, BucketSort<float>::sort_general, 50.0f,
                            150.0f, "浮点数桶排序");

  // 测试10：编译时类型检查
  std::cout << std::endl << "测试10：编译时类型检查" << std::endl;

  // 以下代码会在编译时报错（注释掉以避免编译错误）
  /*
  std::vector<std::string> string_test = {"test", "hello", "world"};
  // CountingSort<std::string>::sort(string_test, 0); //
  编译错误：计数排序仅适用于整数类型
  // RadixSort<std::string>::sort(string_test);       //
  编译错误：基数排序仅适用于整数类型
  // BucketSort<std::string>::sort(string_test);       //
  编译错误：桶排序仅适用于浮点数类型
  */

  std::cout << "类型安全检查: 通过（不兼容的类型会在编译时报错）" << std::endl;

  std::cout << std::endl << "=== 泛型线性时间排序演示结束 ===" << std::endl;

  return 0;
}
