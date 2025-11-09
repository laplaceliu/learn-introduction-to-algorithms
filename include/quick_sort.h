#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <iterator>

namespace algorithms {

/**
 * @brief 快速排序算法实现
 * 
 * 实现算法导论第7章快速排序算法，包括：
 * - 基本快速排序
 * - 随机化快速排序
 * - 尾递归优化版本
 * - 三路快速排序（处理重复元素）
 */
class QuickSort {
private:
public:
    /**
     * @brief 分区操作 - 算法导论第7.1节
     * @param arr 待排序数组
     * @param low 分区起始索引
     * @param high 分区结束索引
     * @return 分区后基准元素的最终位置
     */
    static int partition(std::vector<int>& arr, int low, int high) {
        int pivot = arr[high];  // 选择最后一个元素作为基准
        int i = low - 1;        // 小于基准的区域的边界
        
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
     * @brief 随机分区操作 - 算法导论第7.3节
     * @param arr 待排序数组
     * @param low 分区起始索引
     * @param high 分区结束索引
     * @return 分区后基准元素的最终位置
     */
    static int randomized_partition(std::vector<int>& arr, int low, int high) {
        // 生成随机索引
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(low, high);
        int random_index = dis(gen);
        
        // 将随机选择的元素与最后一个元素交换
        std::swap(arr[random_index], arr[high]);
        
        return partition(arr, low, high);
    }
    
    /**
     * @brief 三路分区操作 - 处理重复元素
     * @param arr 待排序数组
     * @param low 分区起始索引
     * @param high 分区结束索引
     * @return 包含相等区域边界的pair
     */
    static std::pair<int, int> three_way_partition(std::vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int lt = low - 1;   // 小于基准的区域边界
        int gt = high;      // 大于基准的区域边界
        int i = low;        // 当前检查的元素
        
        while (i < gt) {
            if (arr[i] < pivot) {
                ++lt;
                std::swap(arr[lt], arr[i]);
                ++i;
            } else if (arr[i] > pivot) {
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

public:
    /**
     * @brief 基本快速排序 - 算法导论第7.1节
     * @param arr 待排序数组
     * @param low 排序起始索引
     * @param high 排序结束索引
     */
    static void quick_sort(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int pivot_index = partition(arr, low, high);
            quick_sort(arr, low, pivot_index - 1);
            quick_sort(arr, pivot_index + 1, high);
        }
    }
    
    /**
     * @brief 随机化快速排序 - 算法导论第7.3节
     * @param arr 待排序数组
     * @param low 排序起始索引
     * @param high 排序结束索引
     */
    static void randomized_quick_sort(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int pivot_index = randomized_partition(arr, low, high);
            randomized_quick_sort(arr, low, pivot_index - 1);
            randomized_quick_sort(arr, pivot_index + 1, high);
        }
    }
    
    /**
     * @brief 尾递归优化快速排序 - 算法导论第7.4节
     * @param arr 待排序数组
     * @param low 排序起始索引
     * @param high 排序结束索引
     */
    static void tail_recursive_quick_sort(std::vector<int>& arr, int low, int high) {
        while (low < high) {
            int pivot_index = partition(arr, low, high);
            
            // 递归处理较小的子数组
            if (pivot_index - low < high - pivot_index) {
                tail_recursive_quick_sort(arr, low, pivot_index - 1);
                low = pivot_index + 1;
            } else {
                tail_recursive_quick_sort(arr, pivot_index + 1, high);
                high = pivot_index - 1;
            }
        }
    }
    
    /**
     * @brief 三路快速排序 - 处理重复元素
     * @param arr 待排序数组
     * @param low 排序起始索引
     * @param high 排序结束索引
     */
    static void three_way_quick_sort(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            auto [lt, gt] = three_way_partition(arr, low, high);
            three_way_quick_sort(arr, low, lt - 1);
            three_way_quick_sort(arr, gt + 1, high);
        }
    }
    
    /**
     * @brief 快速排序的便捷接口
     * @param arr 待排序数组
     */
    static void sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        quick_sort(arr, 0, arr.size() - 1);
    }
    
    /**
     * @brief 随机化快速排序的便捷接口
     * @param arr 待排序数组
     */
    static void randomized_sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        randomized_quick_sort(arr, 0, arr.size() - 1);
    }
    
    /**
     * @brief 三路快速排序的便捷接口
     * @param arr 待排序数组
     */
    static void three_way_sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        three_way_quick_sort(arr, 0, arr.size() - 1);
    }
    
    /**
     * @brief 验证数组是否已排序
     * @param arr 待验证数组
     * @return 如果数组已排序返回true，否则返回false
     */
    static bool is_sorted(const std::vector<int>& arr) {
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
    static void print_array(const std::vector<int>& arr, const std::string& label = "") {
        if (!label.empty()) {
            std::cout << label << ": ";
        }
        for (int val : arr) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

} // namespace algorithms

#endif // QUICK_SORT_H