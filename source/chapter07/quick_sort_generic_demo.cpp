#include "quick_sort_generic.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <string>

using namespace algorithms;

/**
 * @brief 生成随机数组
 * @param size 数组大小
 * @param min_val 最小值
 * @param max_val 最大值
 * @return 随机数组
 */
template<typename T>
std::vector<T> generate_random_array(int size, T min_val, T max_val) {
    std::vector<T> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dis(min_val, max_val);
        for (int i = 0; i < size; ++i) {
            arr[i] = dis(gen);
        }
    } else {
        std::uniform_real_distribution<T> dis(min_val, max_val);
        for (int i = 0; i < size; ++i) {
            arr[i] = dis(gen);
        }
    }
    
    return arr;
}

int main() {
    std::cout << "=== 算法导论 第7章 - 泛型快速排序算法演示 ===" << std::endl;
    std::cout << std::endl;
    
    // 测试1：整数类型快速排序
    std::cout << "测试1：整数类型快速排序" << std::endl;
    std::vector<int> test1 = {3, 7, 8, 5, 2, 1, 9, 5, 4};
    QuickSortGeneric<int>::print_array(test1, "排序前");
    
    QuickSortGeneric<int>::sort(test1);
    QuickSortGeneric<int>::print_array(test1, "排序后");
    std::cout << "验证结果: " << (QuickSortGeneric<int>::is_sorted(test1) ? "通过" : "失败") << std::endl;
    
    // 测试2：浮点数类型快速排序
    std::cout << std::endl << "测试2：浮点数类型快速排序" << std::endl;
    std::vector<double> test2 = {3.5, 7.2, 8.1, 5.8, 2.3, 1.9, 9.6, 5.1, 4.7};
    QuickSortGeneric<double>::print_array(test2, "排序前");
    
    QuickSortGeneric<double>::sort(test2);
    QuickSortGeneric<double>::print_array(test2, "排序后");
    std::cout << "验证结果: " << (QuickSortGeneric<double>::is_sorted(test2) ? "通过" : "失败") << std::endl;
    
    // 测试3：字符串类型快速排序
    std::cout << std::endl << "测试3：字符串类型快速排序" << std::endl;
    std::vector<std::string> test3 = {"banana", "apple", "cherry", "date", "elderberry"};
    QuickSortGeneric<std::string>::print_array(test3, "排序前");
    
    QuickSortGeneric<std::string>::sort(test3);
    QuickSortGeneric<std::string>::print_array(test3, "排序后");
    std::cout << "验证结果: " << (QuickSortGeneric<std::string>::is_sorted(test3) ? "通过" : "失败") << std::endl;
    
    // 测试4：降序排序
    std::cout << std::endl << "测试4：降序排序" << std::endl;
    std::vector<int> test4 = {3, 7, 8, 5, 2, 1, 9, 5, 4};
    QuickSortGeneric<int>::print_array(test4, "排序前");
    
    QuickSortGeneric<int>::sort(test4, QuickSortGeneric<int>::descending_comparator());
    QuickSortGeneric<int>::print_array(test4, "降序排序后");
    
    // 验证降序排序
    bool descending_sorted = true;
    for (size_t i = 1; i < test4.size(); ++i) {
        if (test4[i] > test4[i - 1]) {
            descending_sorted = false;
            break;
        }
    }
    std::cout << "降序排序验证: " << (descending_sorted ? "通过" : "失败") << std::endl;
    
    // 测试5：自定义比较函数
    std::cout << std::endl << "测试5：自定义比较函数（按绝对值排序）" << std::endl;
    std::vector<int> test5 = {-3, 7, -8, 5, -2, 1, 9, -5, 4};
    QuickSortGeneric<int>::print_array(test5, "排序前");
    
    auto abs_comparator = [](const int& a, const int& b) {
        return std::abs(a) <= std::abs(b);
    };
    
    QuickSortGeneric<int>::sort(test5, abs_comparator);
    QuickSortGeneric<int>::print_array(test5, "按绝对值排序后");
    
    // 测试6：三路快速排序处理重复元素
    std::cout << std::endl << "测试6：三路快速排序处理重复元素" << std::endl;
    std::vector<int> test6 = {5, 2, 5, 1, 5, 3, 5, 4, 5, 2};
    QuickSortGeneric<int>::print_array(test6, "排序前");
    
    QuickSortGeneric<int>::three_way_sort(test6);
    QuickSortGeneric<int>::print_array(test6, "三路快速排序后");
    std::cout << "验证结果: " << (QuickSortGeneric<int>::is_sorted(test6) ? "通过" : "失败") << std::endl;
    
    // 测试7：性能比较（数组大小: 1000）
    std::cout << std::endl << "测试7：性能比较（数组大小: 1000）" << std::endl;
    
    // 整数性能测试
    std::vector<int> int_test = generate_random_array<int>(1000, 0, 1000);
    auto start_int = std::chrono::high_resolution_clock::now();
    QuickSortGeneric<int>::sort(int_test);
    auto end_int = std::chrono::high_resolution_clock::now();
    auto duration_int = std::chrono::duration_cast<std::chrono::microseconds>(end_int - start_int);
    std::cout << "整数快速排序 - 时间: " << duration_int.count() << " 微秒" << std::endl;
    std::cout << "整数快速排序 - 排序验证: " << (QuickSortGeneric<int>::is_sorted(int_test) ? "通过" : "失败") << std::endl;
    
    // 浮点数性能测试
    std::vector<double> double_test = generate_random_array<double>(1000, 0.0, 1000.0);
    auto start_double = std::chrono::high_resolution_clock::now();
    QuickSortGeneric<double>::sort(double_test);
    auto end_double = std::chrono::high_resolution_clock::now();
    auto duration_double = std::chrono::duration_cast<std::chrono::microseconds>(end_double - start_double);
    std::cout << "浮点数快速排序 - 时间: " << duration_double.count() << " 微秒" << std::endl;
    std::cout << "浮点数快速排序 - 排序验证: " << (QuickSortGeneric<double>::is_sorted(double_test) ? "通过" : "失败") << std::endl;
    
    // 随机化快速排序性能测试
    std::vector<int> randomized_test = int_test;
    auto start_random = std::chrono::high_resolution_clock::now();
    QuickSortGeneric<int>::randomized_sort(randomized_test);
    auto end_random = std::chrono::high_resolution_clock::now();
    auto duration_random = std::chrono::duration_cast<std::chrono::microseconds>(end_random - start_random);
    std::cout << "随机化快速排序 - 时间: " << duration_random.count() << " 微秒" << std::endl;
    std::cout << "随机化快速排序 - 排序验证: " << (QuickSortGeneric<int>::is_sorted(randomized_test) ? "通过" : "失败") << std::endl;
    
    // 测试8：边界情况测试
    std::cout << std::endl << "测试8：边界情况测试" << std::endl;
    
    // 空数组
    std::vector<int> empty;
    QuickSortGeneric<int>::sort(empty);
    std::cout << "空数组测试: " << (QuickSortGeneric<int>::is_sorted(empty) ? "通过" : "失败") << std::endl;
    
    // 单元素数组
    std::vector<int> single = {42};
    QuickSortGeneric<int>::sort(single);
    std::cout << "单元素数组测试: " << (QuickSortGeneric<int>::is_sorted(single) ? "通过" : "失败") << std::endl;
    
    // 已排序数组
    std::vector<int> sorted = {1, 2, 3, 4, 5};
    QuickSortGeneric<int>::sort(sorted);
    std::cout << "已排序数组测试: " << (QuickSortGeneric<int>::is_sorted(sorted) ? "通过" : "失败") << std::endl;
    
    // 逆序数组
    std::vector<int> reverse = {5, 4, 3, 2, 1};
    QuickSortGeneric<int>::sort(reverse);
    std::cout << "逆序数组测试: " << (QuickSortGeneric<int>::is_sorted(reverse) ? "通过" : "失败") << std::endl;
    
    std::cout << std::endl << "=== 泛型快速排序演示结束 ===" << std::endl;
    
    return 0;
}