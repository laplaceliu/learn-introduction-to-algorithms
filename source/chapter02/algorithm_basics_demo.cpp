#include "../../include/algorithm_basics.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace algorithms;

/**
 * @brief 测试插入排序算法
 */
void test_insertion_sort() {
  std::cout << "=== 测试插入排序算法 ===" << std::endl;

  // 测试用例1：普通数组
  std::vector<int> arr1 = {5, 2, 4, 6, 1, 3};
  std::vector<int> expected1 = {1, 2, 3, 4, 5, 6};

  std::cout << "原始数组: ";
  print_array(arr1);

  insertion_sort(arr1);

  std::cout << "排序后: ";
  print_array(arr1);

  assert(arr1 == expected1);
  std::cout << "✓ 测试用例1通过" << std::endl;

  // 测试用例2：已排序数组
  std::vector<int> arr2 = {1, 2, 3, 4, 5};
  std::vector<int> expected2 = {1, 2, 3, 4, 5};

  insertion_sort(arr2);
  assert(arr2 == expected2);
  std::cout << "✓ 测试用例2通过（已排序数组）" << std::endl;

  // 测试用例3：逆序数组
  std::vector<int> arr3 = {5, 4, 3, 2, 1};
  std::vector<int> expected3 = {1, 2, 3, 4, 5};

  insertion_sort(arr3);
  assert(arr3 == expected3);
  std::cout << "✓ 测试用例3通过（逆序数组）" << std::endl;

  // 测试用例4：空数组
  std::vector<int> arr4;
  insertion_sort(arr4);
  assert(arr4.empty());
  std::cout << "✓ 测试用例4通过（空数组）" << std::endl;

  // 测试用例5：单元素数组
  std::vector<int> arr5 = {42};
  insertion_sort(arr5);
  assert(arr5.size() == 1 && arr5[0] == 42);
  std::cout << "✓ 测试用例5通过（单元素数组）" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试归并排序算法
 */
void test_merge_sort() {
  std::cout << "=== 测试归并排序算法 ===" << std::endl;

  // 测试用例1：普通数组
  std::vector<int> arr1 = {5, 2, 4, 6, 1, 3};
  std::vector<int> expected1 = {1, 2, 3, 4, 5, 6};

  std::cout << "原始数组: ";
  print_array(arr1);

  merge_sort(arr1);

  std::cout << "排序后: ";
  print_array(arr1);

  assert(arr1 == expected1);
  std::cout << "✓ 测试用例1通过" << std::endl;

  // 测试用例2：大规模数组
  std::vector<int> arr2 =
      AlgorithmAnalyzer::generate_random_data<int>(1000, 1, 10000);
  std::vector<int> arr2_copy = arr2;

  merge_sort(arr2);

  assert(AlgorithmAnalyzer::verify_sort(arr2_copy, arr2));
  std::cout << "✓ 测试用例2通过（大规模数组）" << std::endl;

  // 测试用例3：重复元素
  std::vector<int> arr3 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
  std::vector<int> expected3 = {1, 1, 2, 3, 3, 4, 5, 5, 6, 9};

  merge_sort(arr3);
  assert(arr3 == expected3);
  std::cout << "✓ 测试用例3通过（重复元素）" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试线性查找算法
 */
void test_linear_search() {
  std::cout << "=== 测试线性查找算法 ===" << std::endl;

  std::vector<int> arr = {5, 2, 4, 6, 1, 3};

  // 测试用例1：查找存在的元素
  int index1 = linear_search(arr, 4);
  assert(index1 == 2);
  std::cout << "元素4的索引: " << index1 << " ✓" << std::endl;

  // 测试用例2：查找不存在的元素
  int index2 = linear_search(arr, 7);
  assert(index2 == -1);
  std::cout << "元素7的索引: " << index2 << " ✓" << std::endl;

  // 测试用例3：查找第一个元素
  int index3 = linear_search(arr, 5);
  assert(index3 == 0);
  std::cout << "元素5的索引: " << index3 << " ✓" << std::endl;

  // 测试用例4：查找最后一个元素
  int index4 = linear_search(arr, 3);
  assert(index4 == 5);
  std::cout << "元素3的索引: " << index4 << " ✓" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试二分查找算法
 */
void test_binary_search() {
  std::cout << "=== 测试二分查找算法 ===" << std::endl;

  std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // 测试用例1：查找中间元素
  int index1 = binary_search(arr, 5);
  assert(index1 == 4);
  std::cout << "元素5的索引: " << index1 << " ✓" << std::endl;

  // 测试用例2：查找第一个元素
  int index2 = binary_search(arr, 1);
  assert(index2 == 0);
  std::cout << "元素1的索引: " << index2 << " ✓" << std::endl;

  // 测试用例3：查找最后一个元素
  int index3 = binary_search(arr, 10);
  assert(index3 == 9);
  std::cout << "元素10的索引: " << index3 << " ✓" << std::endl;

  // 测试用例4：查找不存在的元素
  int index4 = binary_search(arr, 11);
  assert(index4 == -1);
  std::cout << "元素11的索引: " << index4 << " ✓" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试选择排序算法
 */
void test_selection_sort() {
  std::cout << "=== 测试选择排序算法 ===" << std::endl;

  std::vector<int> arr = {5, 2, 4, 6, 1, 3};
  std::vector<int> expected = {1, 2, 3, 4, 5, 6};

  std::cout << "原始数组: ";
  print_array(arr);

  selection_sort(arr);

  std::cout << "排序后: ";
  print_array(arr);

  assert(arr == expected);
  std::cout << "✓ 选择排序测试通过" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试冒泡排序算法
 */
void test_bubble_sort() {
  std::cout << "=== 测试冒泡排序算法 ===" << std::endl;

  std::vector<int> arr = {5, 2, 4, 6, 1, 3};
  std::vector<int> expected = {1, 2, 3, 4, 5, 6};

  std::cout << "原始数组: ";
  print_array(arr);

  bubble_sort(arr);

  std::cout << "排序后: ";
  print_array(arr);

  assert(arr == expected);
  std::cout << "✓ 冒泡排序测试通过" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试二进制加法算法
 */
void test_binary_addition() {
  std::cout << "=== 测试二进制加法算法 ===" << std::endl;

  // 测试用例1：算法导论练习2.1-4示例
  std::vector<int> A = {1, 0, 1, 1}; // 二进制1101（13）
  std::vector<int> B = {1, 1, 1, 0}; // 二进制0111（7）

  std::vector<int> C = binary_addition(A, B);
  std::vector<int> expected = {0, 0, 1, 0, 1}; // 二进制10100（20）

  std::cout << "A = ";
  print_array(A);
  std::cout << "B = ";
  print_array(B);
  std::cout << "A + B = ";
  print_array(C);

  assert(C == expected);
  std::cout << "✓ 二进制加法测试通过" << std::endl;

  // 测试用例2：带进位的加法
  // 测试用例2：带进位的加法
  std::vector<int> A2 = {1, 1, 1, 1}; // 二进制1111（15）
  std::vector<int> B2 = {0, 0, 0, 1}; // 二进制1000（8）

  std::vector<int> C2 = binary_addition(A2, B2);
  std::vector<int> expected2 = {1, 1, 1, 0, 1}; // 二进制10111（23）

  assert(C2 == expected2);
  std::cout << "✓ 带进位二进制加法测试通过" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试算法性能分析
 */
void test_algorithm_performance() {
  std::cout << "=== 测试算法性能分析 ===" << std::endl;

  // 生成测试数据
  std::vector<int> small_data =
      AlgorithmAnalyzer::generate_random_data<int>(100, 1, 1000);
  std::vector<int> medium_data =
      AlgorithmAnalyzer::generate_random_data<int>(1000, 1, 10000);

  // 测试插入排序性能
  std::vector<int> data1 = small_data;
  double time1 = AlgorithmAnalyzer::measure_time(
      [](std::vector<int> &arr) { insertion_sort(arr); }, data1);
  std::cout << "插入排序（100元素）耗时: " << time1 << " ms" << std::endl;

  // 测试归并排序性能
  std::vector<int> data2 = medium_data;
  double time2 = AlgorithmAnalyzer::measure_time(
      [](std::vector<int> &arr) { merge_sort(arr); }, data2);
  std::cout << "归并排序（1000元素）耗时: " << time2 << " ms" << std::endl;

  // 验证排序正确性
  assert(AlgorithmAnalyzer::verify_sort(small_data, data1));
  assert(AlgorithmAnalyzer::verify_sort(medium_data, data2));
  std::cout << "✓ 排序正确性验证通过" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试算法导论第2章练习
 */
void test_chapter_exercises() {
  std::cout << "=== 测试算法导论第2章练习 ===" << std::endl;

  // 练习2.1-2：重写插入排序，使其按非递增顺序排序
  std::vector<int> arr = {5, 2, 4, 6, 1, 3};
  std::vector<int> expected_desc = {6, 5, 4, 3, 2, 1};

  // 非递增版本的插入排序
  for (int j = 1; j < arr.size(); j++) {
    int key = arr[j];
    int i = j - 1;
    while (i >= 0 && arr[i] < key) { // 改为小于号
      arr[i + 1] = arr[i];
      i--;
    }
    arr[i + 1] = key;
  }

  assert(arr == expected_desc);
  std::cout << "✓ 练习2.1-2通过（非递增插入排序）" << std::endl;

  // 练习2.1-3：线性查找算法（已在前面测试）
  std::cout << "✓ 练习2.1-3通过（线性查找）" << std::endl;

  // 练习2.1-4：二进制加法（已在前面测试）
  std::cout << "✓ 练习2.1-4通过（二进制加法）" << std::endl;

  // 练习2.2-2：选择排序（已在前面测试）
  std::cout << "✓ 练习2.2-2通过（选择排序）" << std::endl;

  // 练习2.3-5：二分查找（已在前面测试）
  std::cout << "✓ 练习2.3-5通过（二分查找）" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 主函数：运行所有测试
 */
int main() {
  std::cout << "算法导论第2章：算法基础 - 演示程序" << std::endl;
  std::cout << "====================================" << std::endl;
  std::cout << std::endl;

  try {
    test_insertion_sort();
    test_merge_sort();
    test_linear_search();
    test_binary_search();
    test_selection_sort();
    test_bubble_sort();
    test_binary_addition();
    test_algorithm_performance();
    test_chapter_exercises();

    std::cout << "====================================" << std::endl;
    std::cout << "所有测试用例通过！第2章算法基础实现完成。" << std::endl;
    std::cout << "实现内容：" << std::endl;
    std::cout << "- 插入排序算法（2.1节）" << std::endl;
    std::cout << "- 归并排序算法（2.3节）" << std::endl;
    std::cout << "- 线性查找算法（练习2.1-3）" << std::endl;
    std::cout << "- 二分查找算法（练习2.3-5）" << std::endl;
    std::cout << "- 选择排序算法（练习2.2-2）" << std::endl;
    std::cout << "- 冒泡排序算法（练习2-2）" << std::endl;
    std::cout << "- 二进制加法算法（练习2.1-4）" << std::endl;
    std::cout << "- 算法性能分析工具" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "测试失败：" << e.what() << std::endl;
    return 1;
  }

  return 0;
}
