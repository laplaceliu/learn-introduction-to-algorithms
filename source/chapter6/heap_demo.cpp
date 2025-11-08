#include "heap.h"
#include <iostream>
#include <vector>

using namespace algorithms;

int main() {
    std::cout << "=== 算法导论 6.1节 - 堆数据结构演示 ===" << std::endl;
    std::cout << std::endl;
    
    // 测试1：基本堆操作
    std::cout << "测试1：基本堆操作" << std::endl;
    std::vector<int> test_data = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    
    std::cout << "原始数据: ";
    for (int val : test_data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    MaxHeap heap(test_data);
    heap.print();
    
    std::cout << "最大值: " << heap.maximum() << std::endl;
    
    // 测试2：插入操作
    std::cout << std::endl << "测试2：插入操作" << std::endl;
    heap.insert(20);
    heap.insert(5);
    heap.print();
    std::cout << "插入后最大值: " << heap.maximum() << std::endl;
    
    // 测试3：提取最大值
    std::cout << std::endl << "测试3：提取最大值" << std::endl;
    std::cout << "提取的最大值: " << heap.extract_max() << std::endl;
    heap.print();
    std::cout << "当前最大值: " << heap.maximum() << std::endl;
    
    // 测试4：堆排序
    std::cout << std::endl << "测试4：堆排序" << std::endl;
    std::vector<int> sort_test = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    std::cout << "排序前: ";
    for (int val : sort_test) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> sorted = heap_sort(sort_test);
    std::cout << "排序后: ";
    for (int val : sorted) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试5：空堆操作
    std::cout << std::endl << "测试5：空堆操作" << std::endl;
    MaxHeap empty_heap;
    std::cout << "空堆大小: " << empty_heap.size() << std::endl;
    std::cout << "空堆是否为空: " << (empty_heap.empty() ? "是" : "否") << std::endl;
    
    // 插入一些数据到空堆
    empty_heap.insert(10);
    empty_heap.insert(5);
    empty_heap.insert(15);
    empty_heap.print();
    
    std::cout << std::endl << "=== 演示结束 ===" << std::endl;
    
    return 0;
}