#ifndef ALGORITHM_BASICS_H
#define ALGORITHM_BASICS_H

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <set>
#include <vector>

namespace algorithms {

/**
 * @brief 合并两个已排序的子数组
 *
 * @tparam T 元素类型
 * @param arr 数组
 * @param left 左边界
 * @param mid 中间位置
 * @param right 右边界
 */
template <typename T>
void merge(std::vector<T> &arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  // 创建临时数组
  std::vector<T> L(n1), R(n2);

  // 复制数据到临时数组
  for (int i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  // 合并临时数组回原数组
  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  // 复制剩余元素
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

/**
 * @brief 插入排序算法
 *
 * 算法导论第2.1节：插入排序
 * 时间复杂度：最好情况O(n)，最坏情况O(n²)，平均情况O(n²)
 * 空间复杂度：O(1)
 *
 * @tparam T 元素类型
 * @param arr 待排序数组
 */
template <typename T> void insertion_sort(std::vector<T> &arr) {
  int n = arr.size();
  for (int j = 1; j < n; j++) {
    T key = arr[j];
    int i = j - 1;

    // 将arr[0..j-1]中大于key的元素向右移动
    while (i >= 0 && arr[i] > key) {
      arr[i + 1] = arr[i];
      i--;
    }
    arr[i + 1] = key;
  }
}

/**
 * @brief 归并排序算法
 *
 * 算法导论第2.3节：归并排序
 * 时间复杂度：O(n log n)
 * 空间复杂度：O(n)
 *
 * @tparam T 元素类型
 * @param arr 待排序数组
 * @param left 左边界
 * @param right 右边界
 */
template <typename T>
void merge_sort(std::vector<T> &arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;

    // 递归排序左右两部分
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);

    // 合并已排序的两部分
    merge(arr, left, mid, right);
  }
}

/**
 * @brief 归并排序的包装函数
 *
 * @tparam T 元素类型
 * @param arr 待排序数组
 */
template <typename T> void merge_sort(std::vector<T> &arr) {
  if (arr.empty())
    return;
  merge_sort(arr, 0, arr.size() - 1);
}

/**
 * @brief 线性查找算法
 *
 * 算法导论第2.1节练习2.1-3
 * 时间复杂度：O(n)
 *
 * @tparam T 元素类型
 * @param arr 数组
 * @param target 目标值
 * @return int 目标值的索引，如果未找到返回-1
 */
template <typename T>
int linear_search(const std::vector<T> &arr, const T &target) {
  for (int i = 0; i < arr.size(); i++) {
    if (arr[i] == target) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief 二分查找算法
 *
 * 算法导论第2.3节练习2.3-5
 * 时间复杂度：O(log n)
 * 前提：数组必须已排序
 *
 * @tparam T 元素类型
 * @param arr 已排序数组
 * @param target 目标值
 * @return int 目标值的索引，如果未找到返回-1
 */
template <typename T>
int binary_search(const std::vector<T> &arr, const T &target) {
  int left = 0;
  int right = arr.size() - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;

    if (arr[mid] == target) {
      return mid;
    } else if (arr[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
}

/**
 * @brief 选择排序算法
 *
 * 算法导论第2.2节练习2.2-2
 * 时间复杂度：O(n²)
 *
 * @tparam T 元素类型
 * @param arr 待排序数组
 */
template <typename T> void selection_sort(std::vector<T> &arr) {
  int n = arr.size();
  for (int i = 0; i < n - 1; i++) {
    // 找到最小元素的索引
    int min_idx = i;
    for (int j = i + 1; j < n; j++) {
      if (arr[j] < arr[min_idx]) {
        min_idx = j;
      }
    }

    // 交换当前元素和最小元素
    if (min_idx != i) {
      std::swap(arr[i], arr[min_idx]);
    }
  }
}

/**
 * @brief 冒泡排序算法
 *
 * 算法导论第2.2节练习2-2
 * 时间复杂度：O(n²)
 *
 * @tparam T 元素类型
 * @param arr 待排序数组
 */
template <typename T> void bubble_sort(std::vector<T> &arr) {
  int n = arr.size();
  for (int i = 0; i < n - 1; i++) {
    // 最后一次交换的位置
    int last_swap = 0;
    for (int j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
        last_swap = j + 1;
      }
    }

    // 如果没有发生交换，说明数组已经有序
    if (last_swap == 0)
      break;
  }
}

/**
 * @brief 计算两个n位二进制整数的和
 *
 * 算法导论第2.1节练习2.1-4
 *
 * @param A 第一个二进制数（低位在前）
 * @param B 第二个二进制数（低位在前）
 * @return std::vector<int> 和的二进制表示（低位在前）
 */
std::vector<int> binary_addition(const std::vector<int> &A,
                                 const std::vector<int> &B) {
  int n = A.size();
  std::vector<int> C(n + 1, 0);
  int carry = 0;

  for (int i = 0; i < n; i++) {
    int sum = A[i] + B[i] + carry;
    C[i] = sum % 2;
    carry = sum / 2;
  }
  C[n] = carry;

  return C;
}

/**
 * @brief 算法性能分析工具
 */
class AlgorithmAnalyzer {
public:
  /**
   * @brief 测量算法执行时间
   *
   * @tparam Func 函数类型
   * @param func 要测量的函数
   * @param args 函数参数
   * @return double 执行时间（毫秒）
   */
  template <typename Func, typename... Args>
  static double measure_time(Func &&func, Args &&...args) {
    auto start = std::chrono::high_resolution_clock::now();
    std::forward<Func>(func)(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double, std::milli>(end - start).count();
  }

  /**
   * @brief 验证排序算法的正确性
   *
   * @tparam T 元素类型
   * @param original 原始数组
   * @param sorted 排序后的数组
   * @return bool 排序是否正确
   */
  template <typename T>
  static bool verify_sort(const std::vector<T> &original,
                          const std::vector<T> &sorted) {
    // 检查大小
    if (original.size() != sorted.size()) {
      return false;
    }

    // 检查是否有序
    for (int i = 1; i < sorted.size(); i++) {
      if (sorted[i] < sorted[i - 1]) {
        return false;
      }
    }

    // 检查元素是否一致（使用multiset比较）
    std::multiset<T> original_set(original.begin(), original.end());
    std::multiset<T> sorted_set(sorted.begin(), sorted.end());

    return original_set == sorted_set;
  }

  /**
   * @brief 生成随机测试数据
   *
   * @tparam T 元素类型
   * @param size 数据大小
   * @param min_val 最小值
   * @param max_val 最大值
   * @return std::vector<T> 随机生成的数组
   */
  template <typename T>
  static std::vector<T> generate_random_data(int size, T min_val, T max_val) {
    std::vector<T> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());

    if constexpr (std::is_integral_v<T>) {
      std::uniform_int_distribution<T> dis(min_val, max_val);
      for (int i = 0; i < size; i++) {
        data[i] = dis(gen);
      }
    } else {
      std::uniform_real_distribution<T> dis(min_val, max_val);
      for (int i = 0; i < size; i++) {
        data[i] = dis(gen);
      }
    }

    return data;
  }
};

/**
 * @brief 打印数组
 *
 * @tparam T 元素类型
 * @param arr 数组
 * @param name 数组名称
 */
template <typename T>
void print_array(const std::vector<T> &arr, const std::string &name = "") {
  if (!name.empty()) {
    std::cout << name << ": ";
  }

  std::cout << "[";
  for (int i = 0; i < arr.size(); i++) {
    std::cout << arr[i];
    if (i < arr.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl;
}

} // namespace algorithms

#endif // ALGORITHM_BASICS_H
