#ifndef LINEAR_TIME_SORT_H
#define LINEAR_TIME_SORT_H

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>

namespace algorithms {

/**
 * @brief 线性时间排序算法实现
 *
 * 实现算法导论第8章线性时间排序算法，包括：
 * - 计数排序（8.2节）
 * - 基数排序（8.3节）
 * - 桶排序（8.4节）
 */
class LinearTimeSort {
public:
  /**
   * @brief 计数排序 - 算法导论第8.2节
   * @param arr 待排序数组，元素范围为[0, k]
   * @param k 数组中元素的最大值
   * @return 排序后的数组
   */
  static std::vector<int> counting_sort(const std::vector<int> &arr, int k) {
    if (arr.empty())
      return {};

    // 创建计数数组，大小为k+1
    std::vector<int> count(k + 1, 0);

    // 统计每个元素出现的次数
    for (int num : arr) {
      if (num < 0 || num > k) {
        throw std::invalid_argument("元素超出范围[0, k]");
      }
      count[num]++;
    }

    // 计算累计计数
    for (int i = 1; i <= k; ++i) {
      count[i] += count[i - 1];
    }

    // 创建结果数组
    std::vector<int> result(arr.size());

    // 从后往前遍历原数组，保证稳定性
    for (int i = arr.size() - 1; i >= 0; --i) {
      int num = arr[i];
      result[count[num] - 1] = num;
      count[num]--;
    }

    return result;
  }

  /**
   * @brief 计数排序的通用版本，支持任意范围的整数
   * @param arr 待排序数组
   * @param min_val 元素的最小值
   * @param max_val 元素的最大值
   * @return 排序后的数组
   */
  static std::vector<int> counting_sort_general(const std::vector<int> &arr,
                                                int min_val, int max_val) {
    if (arr.empty())
      return {};

    int range = max_val - min_val + 1;

    // 创建计数数组
    std::vector<int> count(range, 0);

    // 统计每个元素出现的次数
    for (int num : arr) {
      if (num < min_val || num > max_val) {
        throw std::invalid_argument("元素超出范围[min_val, max_val]");
      }
      count[num - min_val]++;
    }

    // 计算累计计数
    for (int i = 1; i < range; ++i) {
      count[i] += count[i - 1];
    }

    // 创建结果数组
    std::vector<int> result(arr.size());

    // 从后往前遍历原数组，保证稳定性
    for (int i = arr.size() - 1; i >= 0; --i) {
      int num = arr[i];
      result[count[num - min_val] - 1] = num;
      count[num - min_val]--;
    }

    return result;
  }

  /**
   * @brief 基数排序 - 算法导论第8.3节
   * @param arr 待排序数组
   * @return 排序后的数组
   */
  static std::vector<int> radix_sort(const std::vector<int> &arr) {
    if (arr.empty())
      return {};

    // 找到数组中的最大值，确定最大位数
    int max_val = *std::max_element(arr.begin(), arr.end());

    // 对每一位进行计数排序
    std::vector<int> result = arr;

    for (int exp = 1; max_val / exp > 0; exp *= 10) {
      result = counting_sort_by_digit(result, exp);
    }

    return result;
  }

  /**
   * @brief 按指定位数进行计数排序（辅助函数）
   * @param arr 待排序数组
   * @param exp 当前位数（1, 10, 100, ...）
   * @return 按当前位排序后的数组
   */
  static std::vector<int> counting_sort_by_digit(const std::vector<int> &arr,
                                                 int exp) {
    std::vector<int> count(10, 0);

    // 统计每个数字在当前位的出现次数
    for (int num : arr) {
      int digit = (num / exp) % 10;
      count[digit]++;
    }

    // 计算累计计数
    for (int i = 1; i < 10; ++i) {
      count[i] += count[i - 1];
    }

    // 创建结果数组
    std::vector<int> result(arr.size());

    // 从后往前遍历原数组，保证稳定性
    for (int i = arr.size() - 1; i >= 0; --i) {
      int num = arr[i];
      int digit = (num / exp) % 10;
      result[count[digit] - 1] = num;
      count[digit]--;
    }

    return result;
  }

  /**
   * @brief 桶排序 - 算法导论第8.4节
   * @param arr 待排序数组，元素范围为[0, 1)
   * @return 排序后的数组
   */
  static std::vector<double> bucket_sort(const std::vector<double> &arr) {
    if (arr.empty())
      return {};

    // 验证输入范围
    for (double num : arr) {
      if (num < 0.0 || num >= 1.0) {
        throw std::invalid_argument("桶排序要求元素在[0, 1)范围内");
      }
    }

    int n = arr.size();

    // 创建n个桶
    std::vector<std::vector<double>> buckets(n);

    // 将元素分配到桶中
    for (double num : arr) {
      int bucket_index = static_cast<int>(n * num);
      buckets[bucket_index].push_back(num);
    }

    // 对每个桶进行排序（使用插入排序）
    for (auto &bucket : buckets) {
      insertion_sort(bucket);
    }

    // 合并所有桶
    std::vector<double> result;
    for (const auto &bucket : buckets) {
      result.insert(result.end(), bucket.begin(), bucket.end());
    }

    return result;
  }

  /**
   * @brief 桶排序的通用版本，支持任意范围的浮点数
   * @param arr 待排序数组
   * @param min_val 元素的最小值
   * @param max_val 元素的最大值
   * @return 排序后的数组
   */
  static std::vector<double> bucket_sort_general(const std::vector<double> &arr,
                                                 double min_val,
                                                 double max_val) {
    if (arr.empty())
      return {};

    int n = arr.size();

    // 创建n个桶
    std::vector<std::vector<double>> buckets(n);

    // 将元素分配到桶中
    for (double num : arr) {
      if (num < min_val || num > max_val) {
        throw std::invalid_argument("元素超出范围[min_val, max_val]");
      }

      // 将元素映射到[0, 1)范围
      double normalized = (num - min_val) / (max_val - min_val);
      int bucket_index = static_cast<int>(n * normalized);

      // 处理边界情况：当num等于max_val时
      if (bucket_index == n) {
        bucket_index = n - 1;
      }

      buckets[bucket_index].push_back(num);
    }

    // 对每个桶进行排序（使用插入排序）
    for (auto &bucket : buckets) {
      insertion_sort(bucket);
    }

    // 合并所有桶
    std::vector<double> result;
    for (const auto &bucket : buckets) {
      result.insert(result.end(), bucket.begin(), bucket.end());
    }

    return result;
  }

  /**
   * @brief 验证数组是否已排序
   * @param arr 待验证数组
   * @return 如果数组已排序返回true，否则返回false
   */
  template <typename T> static bool is_sorted(const std::vector<T> &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
      if (arr[i] < arr[i - 1]) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief 打印数组内容
   * @param arr 待打印数组
   * @param label 打印标签
   */
  template <typename T>
  static void print_array(const std::vector<T> &arr,
                          const std::string &label = "") {
    if (!label.empty()) {
      std::cout << label << ": ";
    }
    for (const T &val : arr) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }

private:
  /**
   * @brief 插入排序（用于桶排序中的桶内排序）
   * @param arr 待排序数组
   */
  static void insertion_sort(std::vector<double> &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
      double key = arr[i];
      int j = i - 1;

      while (j >= 0 && arr[j] > key) {
        arr[j + 1] = arr[j];
        j--;
      }

      arr[j + 1] = key;
    }
  }
};

} // namespace algorithms

#endif // LINEAR_TIME_SORT_H
