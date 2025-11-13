#ifndef QUICK_SORT_GENERIC_H
#define QUICK_SORT_GENERIC_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <type_traits>
#include <vector>

namespace algorithms {

/**
 * @brief 泛型快速排序算法实现
 *
 * 实现算法导论第7章快速排序算法，支持任意可比较类型
 * - 基本快速排序
 * - 随机化快速排序
 * - 尾递归优化版本
 * - 三路快速排序（处理重复元素）
 */
template <typename T> class QuickSortGeneric {
private:
  /**
   * @brief 默认比较函数
   */
  static bool default_compare(const T &a, const T &b) { return a <= b; }

  /**
   * @brief 默认相等比较函数
   */
  static bool default_equal(const T &a, const T &b) { return a == b; }

public:
  /**
   * @brief 分区操作 - 算法导论第7.1节
   * @param arr 待排序数组
   * @param low 分区起始索引
   * @param high 分区结束索引
   * @param comp 比较函数
   * @return 分区后基准元素的最终位置
   */
  static int
  partition(std::vector<T> &arr, int low, int high,
            std::function<bool(const T &, const T &)> comp = default_compare) {
    T pivot = arr[high]; // 选择最后一个元素作为基准
    int i = low - 1;     // 小于基准的区域的边界

    for (int j = low; j < high; ++j) {
      if (comp(arr[j], pivot)) {
        ++i;
        std::swap(arr[i], arr[j]);
      }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
  }

  /**
   * @brief 随机分区操作 - 算法导论第7.3节
   * @param arr 待排序数组
   * @param low 分区起始索引
   * @param high 分区结束索引
   * @param comp 比较函数
   * @return 分区后基准元素的最终位置
   */
  static int randomized_partition(
      std::vector<T> &arr, int low, int high,
      std::function<bool(const T &, const T &)> comp = default_compare) {
    // 生成随机索引
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);
    int random_index = dis(gen);

    // 将随机选择的元素与最后一个元素交换
    std::swap(arr[random_index], arr[high]);

    return partition(arr, low, high, comp);
  }

  /**
   * @brief 三路分区操作 - 处理重复元素
   * @param arr 待排序数组
   * @param low 分区起始索引
   * @param high 分区结束索引
   * @param comp 比较函数
   * @param equal 相等比较函数
   * @return 包含相等区域边界的pair
   */
  static std::pair<int, int> three_way_partition(
      std::vector<T> &arr, int low, int high,
      std::function<bool(const T &, const T &)> comp = default_compare,
      std::function<bool(const T &, const T &)> equal = default_equal) {
    T pivot = arr[high];
    int lt = low - 1; // 小于基准的区域边界
    int gt = high;    // 大于基准的区域边界
    int i = low;      // 当前检查的元素

    while (i < gt) {
      if (comp(arr[i], pivot) && !equal(arr[i], pivot)) {
        ++lt;
        std::swap(arr[lt], arr[i]);
        ++i;
      } else if (!comp(arr[i], pivot) && !equal(arr[i], pivot)) {
        --gt;
        std::swap(arr[i], arr[gt]);
      } else {
        ++i;
      }
    }

    // 将基准元素放到正确位置
    std::swap(arr[gt], arr[high]);

    return {lt + 1, gt};
  }

  /**
   * @brief 基本快速排序 - 算法导论第7.1节
   * @param arr 待排序数组
   * @param low 排序起始索引
   * @param high 排序结束索引
   * @param comp 比较函数
   */
  static void
  quick_sort(std::vector<T> &arr, int low, int high,
             std::function<bool(const T &, const T &)> comp = default_compare) {
    if (low < high) {
      int pivot_index = partition(arr, low, high, comp);
      quick_sort(arr, low, pivot_index - 1, comp);
      quick_sort(arr, pivot_index + 1, high, comp);
    }
  }

  /**
   * @brief 随机化快速排序 - 算法导论第7.3节
   * @param arr 待排序数组
   * @param low 排序起始索引
   * @param high 排序结束索引
   * @param comp 比较函数
   */
  static void randomized_quick_sort(
      std::vector<T> &arr, int low, int high,
      std::function<bool(const T &, const T &)> comp = default_compare) {
    if (low < high) {
      int pivot_index = randomized_partition(arr, low, high, comp);
      randomized_quick_sort(arr, low, pivot_index - 1, comp);
      randomized_quick_sort(arr, pivot_index + 1, high, comp);
    }
  }

  /**
   * @brief 尾递归优化快速排序 - 算法导论第7.4节
   * @param arr 待排序数组
   * @param low 排序起始索引
   * @param high 排序结束索引
   * @param comp 比较函数
   */
  static void tail_recursive_quick_sort(
      std::vector<T> &arr, int low, int high,
      std::function<bool(const T &, const T &)> comp = default_compare) {
    while (low < high) {
      int pivot_index = partition(arr, low, high, comp);

      // 递归处理较小的子数组
      if (pivot_index - low < high - pivot_index) {
        tail_recursive_quick_sort(arr, low, pivot_index - 1, comp);
        low = pivot_index + 1;
      } else {
        tail_recursive_quick_sort(arr, pivot_index + 1, high, comp);
        high = pivot_index - 1;
      }
    }
  }

  /**
   * @brief 三路快速排序 - 处理重复元素
   * @param arr 待排序数组
   * @param low 排序起始索引
   * @param high 排序结束索引
   * @param comp 比较函数
   * @param equal 相等比较函数
   */
  static void three_way_quick_sort(
      std::vector<T> &arr, int low, int high,
      std::function<bool(const T &, const T &)> comp = default_compare,
      std::function<bool(const T &, const T &)> equal = default_equal) {
    if (low < high) {
      auto [lt, gt] = three_way_partition(arr, low, high, comp, equal);
      three_way_quick_sort(arr, low, lt - 1, comp, equal);
      three_way_quick_sort(arr, gt + 1, high, comp, equal);
    }
  }

  /**
   * @brief 快速排序的便捷接口
   * @param arr 待排序数组
   * @param comp 比较函数
   */
  static void
  sort(std::vector<T> &arr,
       std::function<bool(const T &, const T &)> comp = default_compare) {
    if (arr.empty())
      return;
    quick_sort(arr, 0, arr.size() - 1, comp);
  }

  /**
   * @brief 随机化快速排序的便捷接口
   * @param arr 待排序数组
   * @param comp 比较函数
   */
  static void randomized_sort(
      std::vector<T> &arr,
      std::function<bool(const T &, const T &)> comp = default_compare) {
    if (arr.empty())
      return;
    randomized_quick_sort(arr, 0, arr.size() - 1, comp);
  }

  /**
   * @brief 三路快速排序的便捷接口
   * @param arr 待排序数组
   * @param comp 比较函数
   * @param equal 相等比较函数
   */
  static void three_way_sort(
      std::vector<T> &arr,
      std::function<bool(const T &, const T &)> comp = default_compare,
      std::function<bool(const T &, const T &)> equal = default_equal) {
    if (arr.empty())
      return;
    three_way_quick_sort(arr, 0, arr.size() - 1, comp, equal);
  }

  /**
   * @brief 验证数组是否已排序
   * @param arr 待验证数组
   * @param comp 比较函数
   * @return 如果数组已排序返回true，否则返回false
   */
  static bool
  is_sorted(const std::vector<T> &arr,
            std::function<bool(const T &, const T &)> comp = default_compare) {
    for (size_t i = 1; i < arr.size(); ++i) {
      if (!comp(arr[i - 1], arr[i]) && !default_equal(arr[i - 1], arr[i])) {
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

  /**
   * @brief 创建降序比较函数
   * @return 降序比较函数
   */
  static std::function<bool(const T &, const T &)> descending_comparator() {
    return [](const T &a, const T &b) { return a >= b; };
  }

  /**
   * @brief 创建自定义比较函数
   * @param func 比较函数
   * @return 包装后的比较函数
   */
  static std::function<bool(const T &, const T &)>
  custom_comparator(std::function<bool(const T &, const T &)> func) {
    return func;
  }
};

// 常用类型的特化版本
using QuickSortInt = QuickSortGeneric<int>;
using QuickSortDouble = QuickSortGeneric<double>;
using QuickSortFloat = QuickSortGeneric<float>;
using QuickSortString = QuickSortGeneric<std::string>;

} // namespace algorithms

#endif // QUICK_SORT_GENERIC_H
