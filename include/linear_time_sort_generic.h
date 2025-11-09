#ifndef LINEAR_TIME_SORT_GENERIC_H
#define LINEAR_TIME_SORT_GENERIC_H

#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <iostream>
#include <iterator>
#include <queue>
#include <type_traits>
#include <limits>

namespace algorithms {

/**
 * @brief 泛型线性时间排序算法实现
 * 
 * 实现算法导论第8章线性时间排序算法，支持多种数据类型
 * - 计数排序（8.2节）- 适用于整数类型
 * - 基数排序（8.3节）- 适用于整数类型
 * - 桶排序（8.4节）- 适用于浮点数类型
 */

/**
 * @brief 计数排序 - 适用于整数类型
 */
template<typename T>
class CountingSort {
    static_assert(std::is_integral_v<T>, "计数排序仅适用于整数类型");
    
public:
    /**
     * @brief 计数排序 - 算法导论第8.2节
     * @param arr 待排序数组，元素范围为[0, k]
     * @param k 数组中元素的最大值
     * @return 排序后的数组
     */
    static std::vector<T> sort(const std::vector<T>& arr, T k) {
        if (arr.empty()) return {};
        
        // 创建计数数组，大小为k+1
        std::vector<size_t> count(static_cast<size_t>(k) + 1, 0);
        
        // 统计每个元素出现的次数
        for (T num : arr) {
            if (num < 0 || num > k) {
                throw std::invalid_argument("元素超出范围[0, k]");
            }
            count[static_cast<size_t>(num)]++;
        }
        
        // 计算累计计数
        for (size_t i = 1; i <= static_cast<size_t>(k); ++i) {
            count[i] += count[i - 1];
        }
        
        // 创建结果数组
        std::vector<T> result(arr.size());
        
        // 从后往前遍历原数组，保证稳定性
        for (int i = arr.size() - 1; i >= 0; --i) {
            T num = arr[i];
            result[count[static_cast<size_t>(num)] - 1] = num;
            count[static_cast<size_t>(num)]--;
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
    static std::vector<T> sort_general(const std::vector<T>& arr, T min_val, T max_val) {
        if (arr.empty()) return {};
        
        size_t range = static_cast<size_t>(max_val - min_val) + 1;
        
        // 创建计数数组
        std::vector<size_t> count(range, 0);
        
        // 统计每个元素出现的次数
        for (T num : arr) {
            if (num < min_val || num > max_val) {
                throw std::invalid_argument("元素超出范围[min_val, max_val]");
            }
            count[static_cast<size_t>(num - min_val)]++;
        }
        
        // 计算累计计数
        for (size_t i = 1; i < range; ++i) {
            count[i] += count[i - 1];
        }
        
        // 创建结果数组
        std::vector<T> result(arr.size());
        
        // 从后往前遍历原数组，保证稳定性
        for (int i = arr.size() - 1; i >= 0; --i) {
            T num = arr[i];
            result[count[static_cast<size_t>(num - min_val)] - 1] = num;
            count[static_cast<size_t>(num - min_val)]--;
        }
        
        return result;
    }
    
    /**
     * @brief 验证数组是否已排序
     * @param arr 待验证数组
     * @return 如果数组已排序返回true，否则返回false
     */
    static bool is_sorted(const std::vector<T>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i] < arr[i - 1]) {
                return false;
            }
        }
        return true;
    }
};

/**
 * @brief 基数排序 - 适用于整数类型
 */
template<typename T>
class RadixSort {
    static_assert(std::is_integral_v<T>, "基数排序仅适用于整数类型");
    
private:
    /**
     * @brief 按指定位数进行计数排序（辅助函数）
     * @param arr 待排序数组
     * @param exp 当前位数（1, 10, 100, ...）
     * @return 按当前位排序后的数组
     */
    static std::vector<T> counting_sort_by_digit(const std::vector<T>& arr, T exp) {
        std::vector<size_t> count(10, 0);
        
        // 统计每个数字在当前位的出现次数
        for (T num : arr) {
            int digit = static_cast<int>((num / exp) % 10);
            count[digit]++;
        }
        
        // 计算累计计数
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }
        
        // 创建结果数组
        std::vector<T> result(arr.size());
        
        // 从后往前遍历原数组，保证稳定性
        for (int i = arr.size() - 1; i >= 0; --i) {
            T num = arr[i];
            int digit = static_cast<int>((num / exp) % 10);
            result[count[digit] - 1] = num;
            count[digit]--;
        }
        
        return result;
    }
    
public:
    /**
     * @brief 基数排序 - 算法导论第8.3节
     * @param arr 待排序数组
     * @return 排序后的数组
     */
    static std::vector<T> sort(const std::vector<T>& arr) {
        if (arr.empty()) return {};
        
        // 找到数组中的最大值，确定最大位数
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        // 对每一位进行计数排序
        std::vector<T> result = arr;
        
        for (T exp = 1; max_val / exp > 0; exp *= 10) {
            result = counting_sort_by_digit(result, exp);
        }
        
        return result;
    }
    
    /**
     * @brief 验证数组是否已排序
     * @param arr 待验证数组
     * @return 如果数组已排序返回true，否则返回false
     */
    static bool is_sorted(const std::vector<T>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i] < arr[i - 1]) {
                return false;
            }
        }
        return true;
    }
};

/**
 * @brief 桶排序 - 适用于浮点数类型
 */
template<typename T>
class BucketSort {
    static_assert(std::is_floating_point_v<T>, "桶排序仅适用于浮点数类型");
    
private:
    /**
     * @brief 插入排序（用于桶排序中的桶内排序）
     * @param arr 待排序数组
     */
    static void insertion_sort(std::vector<T>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            T key = arr[i];
            int j = i - 1;
            
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            
            arr[j + 1] = key;
        }
    }
    
public:
    /**
     * @brief 桶排序 - 算法导论第8.4节
     * @param arr 待排序数组，元素范围为[0, 1)
     * @return 排序后的数组
     */
    static std::vector<T> sort(const std::vector<T>& arr) {
        if (arr.empty()) return {};
        
        // 验证输入范围
        for (T num : arr) {
            if (num < 0.0 || num >= 1.0) {
                throw std::invalid_argument("桶排序要求元素在[0, 1)范围内");
            }
        }
        
        int n = arr.size();
        
        // 创建n个桶
        std::vector<std::vector<T>> buckets(n);
        
        // 将元素分配到桶中
        for (T num : arr) {
            int bucket_index = static_cast<int>(n * num);
            buckets[bucket_index].push_back(num);
        }
        
        // 对每个桶进行排序（使用插入排序）
        for (auto& bucket : buckets) {
            insertion_sort(bucket);
        }
        
        // 合并所有桶
        std::vector<T> result;
        for (const auto& bucket : buckets) {
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
    static std::vector<T> sort_general(const std::vector<T>& arr, T min_val, T max_val) {
        if (arr.empty()) return {};
        
        int n = arr.size();
        
        // 创建n个桶
        std::vector<std::vector<T>> buckets(n);
        
        // 将元素分配到桶中
        for (T num : arr) {
            if (num < min_val || num > max_val) {
                throw std::invalid_argument("元素超出范围[min_val, max_val]");
            }
            
            // 将元素映射到[0, 1)范围
            T normalized = (num - min_val) / (max_val - min_val);
            int bucket_index = static_cast<int>(n * normalized);
            
            // 处理边界情况：当num等于max_val时
            if (bucket_index == n) {
                bucket_index = n - 1;
            }
            
            buckets[bucket_index].push_back(num);
        }
        
        // 对每个桶进行排序（使用插入排序）
        for (auto& bucket : buckets) {
            insertion_sort(bucket);
        }
        
        // 合并所有桶
        std::vector<T> result;
        for (const auto& bucket : buckets) {
            result.insert(result.end(), bucket.begin(), bucket.end());
        }
        
        return result;
    }
    
    /**
     * @brief 验证数组是否已排序
     * @param arr 待验证数组
     * @return 如果数组已排序返回true，否则返回false
     */
    static bool is_sorted(const std::vector<T>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i] < arr[i - 1]) {
                return false;
            }
        }
        return true;
    }
};

/**
 * @brief 线性时间排序的统一接口类
 */
class LinearTimeSortGeneric {
public:
    /**
     * @brief 验证数组是否已排序（通用版本）
     * @param arr 待验证数组
     * @return 如果数组已排序返回true，否则返回false
     */
    template<typename T>
    static bool is_sorted(const std::vector<T>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i] < arr[i - 1]) {
                return false;
            }
        }
        return true;
    }
    
    /**
     * @brief 打印数组内容（通用版本）
     * @param arr 待打印数组
     * @param label 打印标签
     */
    template<typename T>
    static void print_array(const std::vector<T>& arr, const std::string& label = "") {
        if (!label.empty()) {
            std::cout << label << ": ";
        }
        for (const T& val : arr) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

// 常用类型的别名
using CountingSortInt = CountingSort<int>;
using CountingSortLong = CountingSort<long>;
using RadixSortInt = RadixSort<int>;
using RadixSortLong = RadixSort<long>;
using BucketSortFloat = BucketSort<float>;
using BucketSortDouble = BucketSort<double>;

} // namespace algorithms

#endif // LINEAR_TIME_SORT_GENERIC_H