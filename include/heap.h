#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <algorithm>
#include <iostream>

namespace algorithms {

/**
 * @brief 堆数据结构实现
 * 
 * 实现最大堆的基本操作，包括：
 * - 堆的构建
 * - 堆排序
 * - 堆的基本操作（插入、删除、获取最大值等）
 */
class MaxHeap {
private:
    std::vector<int> data;
    
    /**
     * @brief 维护堆性质：使以i为根的子树成为最大堆
     * @param i 子树的根节点索引
     * @param heap_size 堆的大小
     */
    void max_heapify(int i, int heap_size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        
        if (left < heap_size && data[left] > data[largest]) {
            largest = left;
        }
        
        if (right < heap_size && data[right] > data[largest]) {
            largest = right;
        }
        
        if (largest != i) {
            std::swap(data[i], data[largest]);
            max_heapify(largest, heap_size);
        }
    }
    
    /**
     * @brief 构建最大堆
     */
    void build_max_heap() {
        int heap_size = data.size();
        for (int i = heap_size / 2 - 1; i >= 0; --i) {
            max_heapify(i, heap_size);
        }
    }

public:
    /**
     * @brief 默认构造函数
     */
    MaxHeap() = default;
    
    /**
     * @brief 从数组构造堆
     * @param arr 输入数组
     */
    MaxHeap(const std::vector<int>& arr) : data(arr) {
        build_max_heap();
    }
    
    /**
     * @brief 插入元素
     * @param value 要插入的值
     */
    void insert(int value) {
        data.push_back(value);
        
        // 上浮新插入的元素
        int i = data.size() - 1;
        while (i > 0 && data[(i - 1) / 2] < data[i]) {
            std::swap(data[i], data[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
    
    /**
     * @brief 获取最大值
     * @return 堆中的最大值
     */
    int maximum() const {
        if (data.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return data[0];
    }
    
    /**
     * @brief 删除并返回最大值
     * @return 被删除的最大值
     */
    int extract_max() {
        if (data.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        
        int max_val = data[0];
        data[0] = data.back();
        data.pop_back();
        
        if (!data.empty()) {
            max_heapify(0, data.size());
        }
        
        return max_val;
    }
    
    /**
     * @brief 堆排序
     * @return 排序后的数组
     */
    std::vector<int> sort() {
        std::vector<int> result = data;
        int heap_size = result.size();
        
        // 构建最大堆
        for (int i = heap_size / 2 - 1; i >= 0; --i) {
            max_heapify(i, heap_size);
        }
        
        // 逐个提取元素
        for (int i = heap_size - 1; i > 0; --i) {
            std::swap(result[0], result[i]);
            max_heapify(0, i);
        }
        
        return result;
    }
    
    /**
     * @brief 获取堆的大小
     * @return 堆中元素的数量
     */
    size_t size() const {
        return data.size();
    }
    
    /**
     * @brief 检查堆是否为空
     * @return 如果堆为空返回true，否则返回false
     */
    bool empty() const {
        return data.empty();
    }
    
    /**
     * @brief 打印堆的内容
     */
    void print() const {
        std::cout << "Heap: ";
        for (int val : data) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

/**
 * @brief 堆排序函数
 * @param arr 待排序的数组
 * @return 排序后的数组
 */
std::vector<int> heap_sort(const std::vector<int>& arr) {
    MaxHeap heap(arr);
    return heap.sort();
}

} // namespace algorithms

#endif // HEAP_H