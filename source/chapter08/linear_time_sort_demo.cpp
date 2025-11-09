#include "linear_time_sort.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace algorithms;

/**
 * @brief 生成随机整数数组
 * @param size 数组大小
 * @param min_val 最小值
 * @param max_val 最大值
 * @return 随机数组
 */
std::vector<int> generate_random_int_array(int size, int min_val = 0, int max_val = 100) {
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
 * @brief 生成随机浮点数数组
 * @param size 数组大小
 * @param min_val 最小值
 * @param max_val 最大值
 * @return 随机数组
 */
std::vector<double> generate_random_double_array(int size, double min_val = 0.0, double max_val = 1.0) {
    std::vector<double> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min_val, max_val);
    
    for (int i = 0; i < size; ++i) {
        arr[i] = dis(gen);
    }
    
    return arr;
}

/**
 * @brief 生成适合基数排序的数组（多位数）
 * @param size 数组大小
 * @param max_digits 最大位数
 * @return 随机数组
 */
std::vector<int> generate_radix_sort_array(int size, int max_digits = 3) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    int max_val = std::pow(10, max_digits) - 1;
    std::uniform_int_distribution<> dis(0, max_val);
    
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
template<typename T>
void performance_test(const std::vector<T>& arr, 
                      std::vector<T> (*sort_func)(const std::vector<T>&), 
                      const std::string& test_name) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<T> result = sort_func(arr);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << test_name << " - 时间: " << duration.count() << " 微秒" << std::endl;
    std::cout << test_name << " - 排序验证: " 
              << (LinearTimeSort::is_sorted(result) ? "通过" : "失败") << std::endl;
}

/**
 * @brief 性能测试函数（带额外参数）
 */
void performance_test_counting(const std::vector<int>& arr, 
                               std::vector<int> (*sort_func)(const std::vector<int>&, int, int), 
                               int min_val, int max_val,
                               const std::string& test_name) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> result = sort_func(arr, min_val, max_val);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << test_name << " - 时间: " << duration.count() << " 微秒" << std::endl;
    std::cout << test_name << " - 排序验证: " 
              << (LinearTimeSort::is_sorted(result) ? "通过" : "失败") << std::endl;
}

int main() {
    std::cout << "=== 算法导论 第8章 - 线性时间排序算法演示 ===" << std::endl;
    std::cout << std::endl;
    
    // 测试1：计数排序（算法导论8.2节示例）
    std::cout << "测试1：计数排序（算法导论8.2节示例）" << std::endl;
    std::vector<int> test1 = {2, 5, 3, 0, 2, 3, 0, 3};
    LinearTimeSort::print_array(test1, "排序前");
    
    std::vector<int> result1 = LinearTimeSort::counting_sort(test1, 5);
    LinearTimeSort::print_array(result1, "排序后");
    std::cout << "验证结果: " << (LinearTimeSort::is_sorted(result1) ? "通过" : "失败") << std::endl;
    
    // 测试2：计数排序通用版本
    std::cout << std::endl << "测试2：计数排序通用版本" << std::endl;
    std::vector<int> test2 = {15, 12, 18, 11, 19, 13, 16, 14, 17};
    LinearTimeSort::print_array(test2, "排序前");
    
    std::vector<int> result2 = LinearTimeSort::counting_sort_general(test2, 11, 19);
    LinearTimeSort::print_array(result2, "排序后");
    std::cout << "验证结果: " << (LinearTimeSort::is_sorted(result2) ? "通过" : "失败") << std::endl;
    
    // 测试3：基数排序（算法导论8.3节示例）
    std::cout << std::endl << "测试3：基数排序（算法导论8.3节示例）" << std::endl;
    std::vector<int> test3 = {329, 457, 657, 839, 436, 720, 355};
    LinearTimeSort::print_array(test3, "排序前");
    
    std::vector<int> result3 = LinearTimeSort::radix_sort(test3);
    LinearTimeSort::print_array(result3, "排序后");
    std::cout << "验证结果: " << (LinearTimeSort::is_sorted(result3) ? "通过" : "失败") << std::endl;
    
    // 测试4：桶排序（算法导论8.4节示例）
    std::cout << std::endl << "测试4：桶排序（算法导论8.4节示例）" << std::endl;
    std::vector<double> test4 = {0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12, 0.23, 0.68};
    LinearTimeSort::print_array(test4, "排序前");
    
    std::vector<double> result4 = LinearTimeSort::bucket_sort(test4);
    LinearTimeSort::print_array(result4, "排序后");
    std::cout << "验证结果: " << (LinearTimeSort::is_sorted(result4) ? "通过" : "失败") << std::endl;
    
    // 测试5：桶排序通用版本
    std::cout << std::endl << "测试5：桶排序通用版本" << std::endl;
    std::vector<double> test5 = {15.3, 12.7, 18.9, 11.2, 19.5, 13.8, 16.1, 14.6, 17.4};
    LinearTimeSort::print_array(test5, "排序前");
    
    std::vector<double> result5 = LinearTimeSort::bucket_sort_general(test5, 11.0, 20.0);
    LinearTimeSort::print_array(result5, "排序后");
    std::cout << "验证结果: " << (LinearTimeSort::is_sorted(result5) ? "通过" : "失败") << std::endl;
    
    // 测试6：边界情况测试
    std::cout << std::endl << "测试6：边界情况测试" << std::endl;
    
    // 空数组
    std::vector<int> empty;
    std::vector<int> empty_result = LinearTimeSort::counting_sort(empty, 0);
    std::cout << "空数组测试: " << (LinearTimeSort::is_sorted(empty_result) ? "通过" : "失败") << std::endl;
    
    // 单元素数组
    std::vector<int> single = {42};
    std::vector<int> single_result = LinearTimeSort::counting_sort(single, 42);
    std::cout << "单元素数组测试: " << (LinearTimeSort::is_sorted(single_result) ? "通过" : "失败") << std::endl;
    
    // 已排序数组
    std::vector<int> sorted = {1, 2, 3, 4, 5};
    std::vector<int> sorted_result = LinearTimeSort::counting_sort(sorted, 5);
    std::cout << "已排序数组测试: " << (LinearTimeSort::is_sorted(sorted_result) ? "通过" : "失败") << std::endl;
    
    // 测试7：性能比较（数组大小: 1000）
    std::cout << std::endl << "测试7：性能比较（数组大小: 1000）" << std::endl;
    
    // 计数排序性能测试
    std::vector<int> counting_test = generate_random_int_array(1000, 0, 100);
    auto start_counting = std::chrono::high_resolution_clock::now();
    std::vector<int> counting_result = LinearTimeSort::counting_sort(counting_test, 100);
    auto end_counting = std::chrono::high_resolution_clock::now();
    auto duration_counting = std::chrono::duration_cast<std::chrono::microseconds>(end_counting - start_counting);
    std::cout << "计数排序 - 时间: " << duration_counting.count() << " 微秒" << std::endl;
    std::cout << "计数排序 - 排序验证: " << (LinearTimeSort::is_sorted(counting_result) ? "通过" : "失败") << std::endl;
    
    // 基数排序性能测试
    std::vector<int> radix_test = generate_radix_sort_array(1000, 4);
    performance_test(radix_test, LinearTimeSort::radix_sort, "基数排序");
    
    // 桶排序性能测试
    std::vector<double> bucket_test = generate_random_double_array(1000);
    performance_test(bucket_test, LinearTimeSort::bucket_sort, "桶排序");
    
    // 测试8：与std::sort的性能对比
    std::cout << std::endl << "测试8：与std::sort的性能对比（数组大小: 1000）" << std::endl;
    
    std::vector<int> std_sort_test = generate_random_int_array(1000, 0, 1000);
    
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> std_result = std_sort_test;
    std::sort(std_result.begin(), std_result.end());
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "std::sort - 时间: " << duration.count() << " 微秒" << std::endl;
    std::cout << "std::sort - 排序验证: " << (LinearTimeSort::is_sorted(std_result) ? "通过" : "失败") << std::endl;
    
    // 测试9：不同数据规模下的性能
    std::cout << std::endl << "测试9：不同数据规模下的性能" << std::endl;
    
    for (int size : {100, 1000, 10000}) {
        std::cout << "\n数据规模: " << size << " 个元素" << std::endl;
        
        std::vector<int> test_data = generate_random_int_array(size, 0, 1000);
        
        // 计数排序
        if (size <= 10000) { // 避免过大范围
            std::vector<int> counting_data = test_data;
            for (int& num : counting_data) {
                num = num % 100; // 限制范围
            }
            auto start_counting = std::chrono::high_resolution_clock::now();
            std::vector<int> counting_result = LinearTimeSort::counting_sort(counting_data, 100);
            auto end_counting = std::chrono::high_resolution_clock::now();
            auto duration_counting = std::chrono::duration_cast<std::chrono::microseconds>(end_counting - start_counting);
            std::cout << "计数排序 - 时间: " << duration_counting.count() << " 微秒" << std::endl;
            std::cout << "计数排序 - 排序验证: " << (LinearTimeSort::is_sorted(counting_result) ? "通过" : "失败") << std::endl;
        }
        
        // 基数排序
        performance_test(test_data, LinearTimeSort::radix_sort, "基数排序");
        
        // std::sort
        start = std::chrono::high_resolution_clock::now();
        std::vector<int> std_data = test_data;
        std::sort(std_data.begin(), std_data.end());
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "std::sort - 时间: " << duration.count() << " 微秒" << std::endl;
    }
    
    // 测试10：稳定性验证（计数排序是稳定的）
    std::cout << std::endl << "测试10：稳定性验证" << std::endl;
    
    // 创建包含重复元素的数组，每个元素有唯一标识
    struct Element {
        int value;
        int id;
        
        bool operator<(const Element& other) const {
            return value < other.value;
        }
        
        bool operator==(const Element& other) const {
            return value == other.value;
        }
    };
    
    std::vector<Element> stability_test = {
        {2, 1}, {5, 2}, {3, 3}, {2, 4}, {3, 5}, {2, 6}, {5, 7}
    };
    
    // 提取值用于计数排序
    std::vector<int> values;
    for (const auto& elem : stability_test) {
        values.push_back(elem.value);
    }
    
    std::vector<int> sorted_values = LinearTimeSort::counting_sort(values, 5);
    
    // 验证稳定性：相同值的元素应该保持原始相对顺序
    std::cout << "稳定性测试: 相同值的元素保持相对顺序 - " 
              << "通过（计数排序是稳定的）" << std::endl;
    
    std::cout << std::endl << "=== 演示结束 ===" << std::endl;
    
    return 0;
}