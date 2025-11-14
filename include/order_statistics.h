#ifndef ORDER_STATISTICS_H
#define ORDER_STATISTICS_H

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

namespace algorithms {

/**
 * @brief 中位数和顺序统计量算法实现
 *
 * 实现算法导论第9章中位数和顺序统计量算法，包括：
 * - 最小值和最大值的同时查找
 * - 随机化选择算法（期望线性时间）
 * - 最坏情况线性时间选择算法
 * - 中位数查找
 */
class OrderStatistics {
private:
  /**
   * @brief 随机分区操作
   * @param arr 数组
   * @param low 起始索引
   * @param high 结束索引
   * @return 分区后基准元素的最终位置
   */
  static int randomized_partition(std::vector<int> &arr, int low, int high) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);

    int random_index = dis(gen);
    std::swap(arr[random_index], arr[high]);

    return partition(arr, low, high);
  }

  /**
   * @brief 分区操作
   * @param arr 数组
   * @param low 起始索引
   * @param high 结束索引
   * @return 分区后基准元素的最终位置
   */
  static int partition(std::vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
      if (arr[j] <= pivot) {
        ++i;
        std::swap(arr[i], arr[j]);
      }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
  }

  /**
   * @brief 插入排序（用于小数组优化）
   * @param arr 数组
   * @param low 起始索引
   * @param high 结束索引
   */
  static void insertion_sort(std::vector<int> &arr, int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
      int key = arr[i];
      int j = i - 1;

      while (j >= low && arr[j] > key) {
        arr[j + 1] = arr[j];
        --j;
      }

      arr[j + 1] = key;
    }
  }

  /**
   * @brief 查找中位数的中位数
   * @param arr 数组
   * @param low 起始索引
   * @param high 结束索引
   * @return 中位数的中位数
   */
  static int median_of_medians(std::vector<int> &arr, int low, int high) {
    int n = high - low + 1;

    // 如果元素数量小于等于5，直接排序返回中位数
    if (n <= 5) {
      insertion_sort(arr, low, high);
      return arr[low + (n - 1) / 2];
    }

    // 将数组分成5个一组，找到每组的中位数
    std::vector<int> medians;
    for (int i = low; i <= high; i += 5) {
      int end = std::min(i + 4, high);
      insertion_sort(arr, i, end);
      medians.push_back(arr[i + (end - i) / 2]);
    }

    // 递归找到中位数的中位数
    return select(medians, 0, medians.size() - 1, medians.size() / 2);
  }

public:
  /**
   * @brief 同时查找最小值和最大值 - 算法导论第9.1节
   * @param arr 数组
   * @return 包含最小值和最大值的pair
   */
  static std::pair<int, int> find_min_max(const std::vector<int> &arr) {
    if (arr.empty()) {
      throw std::invalid_argument("数组不能为空");
    }

    int n = arr.size();
    int min_val, max_val;
    int start;

    // 初始化最小值和最大值
    if (n % 2 == 1) {
      min_val = max_val = arr[0];
      start = 1;
    } else {
      if (arr[0] < arr[1]) {
        min_val = arr[0];
        max_val = arr[1];
      } else {
        min_val = arr[1];
        max_val = arr[0];
      }
      start = 2;
    }

    // 每次比较两个元素，总共3(n/2)次比较
    for (int i = start; i < n; i += 2) {
      if (arr[i] < arr[i + 1]) {
        min_val = std::min(min_val, arr[i]);
        max_val = std::max(max_val, arr[i + 1]);
      } else {
        min_val = std::min(min_val, arr[i + 1]);
        max_val = std::max(max_val, arr[i]);
      }
    }

    return {min_val, max_val};
  }

  /**
   * @brief 随机化选择算法 - 算法导论第9.2节（期望线性时间）
   * @param arr 数组
   * @param low 起始索引
   * @param high 结束索引
   * @param i 要查找的第i小的元素（从0开始）
   * @return 第i小的元素
   */
  static int randomized_select(std::vector<int> &arr, int low, int high, int i) {
    if (low == high) {
      return arr[low];
    }

    int q = randomized_partition(arr, low, high);
    int k = q - low;

    if (i == k) {
      return arr[q];
    } else if (i < k) {
      return randomized_select(arr, low, q - 1, i);
    } else {
      return randomized_select(arr, q + 1, high, i - k - 1);
    }
  }

  /**
   * @brief 最坏情况线性时间选择算法 - 算法导论第9.3节
   * @param arr 数组
   * @param low 起始索引
   * @param high 结束索引
   * @param i 要查找的第i小的元素（从0开始）
   * @return 第i小的元素
   */
  static int select(std::vector<int> &arr, int low, int high, int i) {
    if (high - low + 1 <= 5) {
      insertion_sort(arr, low, high);
      return arr[low + i];
    }

    // 找到中位数的中位数作为基准
    int median = median_of_medians(arr, low, high);

    // 找到中位数在数组中的位置
    int median_index = low;
    for (int j = low; j <= high; ++j) {
      if (arr[j] == median) {
        median_index = j;
        break;
      }
    }

    // 将中位数交换到末尾
    std::swap(arr[median_index], arr[high]);

    // 分区
    int q = partition(arr, low, high);
    int k = q - low;

    if (i == k) {
      return arr[q];
    } else if (i < k) {
      return select(arr, low, q - 1, i);
    } else {
      return select(arr, q + 1, high, i - k - 1);
    }
  }

  /**
   * @brief 查找中位数
   * @param arr 数组
   * @return 中位数
   */
  static double find_median(std::vector<int> &arr) {
    if (arr.empty()) {
      throw std::invalid_argument("数组不能为空");
    }

    int n = arr.size();

    if (n % 2 == 1) {
      // 奇数个元素，返回中间值
      return select(arr, 0, n - 1, n / 2);
    } else {
      // 偶数个元素，返回两个中间值的平均值
      int median1 = select(arr, 0, n - 1, n / 2 - 1);
      int median2 = select(arr, 0, n - 1, n / 2);
      return (median1 + median2) / 2.0;
    }
  }

  /**
   * @brief 查找第i小的元素（对外接口）
   * @param arr 数组
   * @param i 要查找的第i小的元素（从1开始）
   * @return 第i小的元素
   */
  static int find_ith_smallest(std::vector<int> &arr, int i) {
    if (arr.empty()) {
      throw std::invalid_argument("数组不能为空");
    }

    if (i < 1 || i > arr.size()) {
      throw std::invalid_argument("i必须在1到数组大小之间");
    }

    return select(arr, 0, arr.size() - 1, i - 1);
  }

  /**
   * @brief 随机化版本查找第i小的元素
   * @param arr 数组
   * @param i 要查找的第i小的元素（从1开始）
   * @return 第i小的元素
   */
  static int find_ith_smallest_randomized(std::vector<int> &arr, int i) {
    if (arr.empty()) {
      throw std::invalid_argument("数组不能为空");
    }

    if (i < 1 || i > arr.size()) {
      throw std::invalid_argument("i必须在1到数组大小之间");
    }

    return randomized_select(arr, 0, arr.size() - 1, i - 1);
  }
};

} // namespace algorithms

#endif // ORDER_STATISTICS_H
