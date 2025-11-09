#include <iostream>
#include <vector>
#include <string>
#include "interval_tree.h"

using namespace algorithms;

void test_basic_operations() {
    std::cout << "=== 测试基本操作 ===" << std::endl;

    IntervalTree<int> it;

    // 测试插入（算法导论图14-4示例）
    std::cout << "插入区间: [16,21], [8,9], [25,30], [5,8], [15,23], [17,19], [26,26], [0,3], [6,10], [19,20]" << std::endl;
    it.insert(Interval(16, 21));
    it.insert(Interval(8, 9));
    it.insert(Interval(25, 30));
    it.insert(Interval(5, 8));
    it.insert(Interval(15, 23));
    it.insert(Interval(17, 19));
    it.insert(Interval(26, 26));
    it.insert(Interval(0, 3));
    it.insert(Interval(6, 10));
    it.insert(Interval(19, 20));

    // 测试中序遍历
    std::cout << "中序遍历（按low排序）: ";
    auto inorder = it.inorder_traversal();
    for (const auto& interval : inorder) {
        std::cout << interval << " ";
    }
    std::cout << std::endl;

    // 测试最小值和最大值
    std::cout << "最小区间: " << it.minimum() << std::endl;
    std::cout << "最大区间: " << it.maximum() << std::endl;

    // 测试精确搜索
    std::cout << "搜索区间 [15,23]: " << (it.search(Interval(15, 23)) != nullptr ? "找到" : "未找到") << std::endl;
    std::cout << "搜索区间 [10,15]: " << (it.search(Interval(10, 15)) != nullptr ? "找到" : "未找到") << std::endl;

    std::cout << "是否有效区间树: " << (it.is_valid() ? "是" : "否") << std::endl;

    std::cout << "树结构:" << std::endl;
    it.print_tree();
    std::cout << std::endl;
}

void test_interval_search() {
    std::cout << "=== 测试区间搜索 ===" << std::endl;

    IntervalTree<int> it;

    // 构建测试树
    std::vector<Interval> intervals = {
        Interval(16, 21), Interval(8, 9), Interval(25, 30), Interval(5, 8),
        Interval(15, 23), Interval(17, 19), Interval(26, 26), Interval(0, 3),
        Interval(6, 10), Interval(19, 20)
    };

    for (const auto& interval : intervals) {
        it.insert(interval);
    }

    // 测试各种区间搜索
    std::vector<Interval> search_intervals = {
        Interval(22, 25),  // 应该找到 [15,23] 或 [16,21]
        Interval(11, 14),  // 应该找到 [8,9] 或 [6,10]
        Interval(31, 35),  // 不应该找到
        Interval(2, 4),    // 应该找到 [0,3]
        Interval(7, 7),    // 应该找到 [5,8] 或 [6,10] 或 [8,9]
        Interval(24, 27)   // 应该找到 [25,30] 或 [26,26]
    };

    for (const auto& search_interval : search_intervals) {
        auto result = it.interval_search(search_interval);
        std::cout << "搜索区间 " << search_interval << ": ";
        if (result != nullptr) {
            std::cout << "找到 " << result->interval;
        } else {
            std::cout << "未找到";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void test_delete_operations() {
    std::cout << "=== 测试删除操作 ===" << std::endl;

    IntervalTree<int> it;

    // 构建测试树
    std::vector<Interval> intervals = {
        Interval(16, 21), Interval(8, 9), Interval(25, 30), Interval(5, 8),
        Interval(15, 23), Interval(17, 19), Interval(26, 26)
    };

    for (const auto& interval : intervals) {
        it.insert(interval);
    }

    std::cout << "初始树中序遍历: ";
    auto initial = it.inorder_traversal();
    for (const auto& interval : initial) {
        std::cout << interval << " ";
    }
    std::cout << std::endl;

    // 测试删除叶子节点
    std::cout << "删除叶子节点 [26,26]: " << (it.remove(Interval(26, 26)) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete1 = it.inorder_traversal();
    for (const auto& interval : after_delete1) {
        std::cout << interval << " ";
    }
    std::cout << std::endl;

    // 测试删除只有一个子节点的节点
    std::cout << "删除节点 [25,30]: " << (it.remove(Interval(25, 30)) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete2 = it.inorder_traversal();
    for (const auto& interval : after_delete2) {
        std::cout << interval << " ";
    }
    std::cout << std::endl;

    // 测试删除有两个子节点的节点
    std::cout << "删除节点 [16,21]: " << (it.remove(Interval(16, 21)) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete3 = it.inorder_traversal();
    for (const auto& interval : after_delete3) {
        std::cout << interval << " ";
    }
    std::cout << std::endl;

    std::cout << "是否有效区间树: " << (it.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;

    // 测试空树
    IntervalTree<int> empty_it;
    std::cout << "空树是否为空: " << (empty_it.empty() ? "是" : "否") << std::endl;

    try {
        empty_it.minimum();
        std::cout << "ERROR: 应该抛出异常" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "正确捕获空树最小值异常: " << e.what() << std::endl;
    }

    // 测试单节点树
    IntervalTree<int> single_it;
    single_it.insert(Interval(10, 20));
    std::cout << "单节点树最小区间: " << single_it.minimum() << std::endl;
    std::cout << "单节点树最大区间: " << single_it.maximum() << std::endl;

    // 测试区间搜索边界
    auto result = single_it.interval_search(Interval(15, 25));
    std::cout << "搜索重叠区间 [15,25]: " << (result != nullptr ? "找到" : "未找到") << std::endl;

    result = single_it.interval_search(Interval(30, 40));
    std::cout << "搜索不重叠区间 [30,40]: " << (result != nullptr ? "找到" : "未找到") << std::endl;

    std::cout << "是否有效区间树: " << (single_it.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void test_complex_intervals() {
    std::cout << "=== 测试复杂区间 ===" << std::endl;

    IntervalTree<int> it;

    // 插入各种复杂区间
    std::vector<Interval> complex_intervals = {
        Interval(1, 100),      // 大区间
        Interval(50, 50),      // 单点区间
        Interval(10, 20),      // 正常区间
        Interval(5, 15),       // 部分重叠
        Interval(25, 35),      // 正常区间
        Interval(30, 40),      // 部分重叠
        Interval(0, 5),        // 小区间
        Interval(95, 100)      // 小区间
    };

    for (const auto& interval : complex_intervals) {
        it.insert(interval);
    }

    std::cout << "复杂区间树中序遍历: ";
    auto inorder = it.inorder_traversal();
    for (const auto& interval : inorder) {
        std::cout << interval << " ";
    }
    std::cout << std::endl;

    // 测试各种搜索
    std::vector<Interval> test_searches = {
        Interval(12, 18),      // 应该找到多个
        Interval(45, 55),      // 应该找到 [1,100] 和 [50,50]
        Interval(60, 70),      // 应该找到 [1,100]
        Interval(105, 110),    // 不应该找到
        Interval(-5, -1),      // 不应该找到
        Interval(22, 28)       // 应该找到 [10,20] 和 [25,35]
    };

    for (const auto& search_interval : test_searches) {
        auto result = it.interval_search(search_interval);
        std::cout << "搜索 " << search_interval << ": ";
        if (result != nullptr) {
            std::cout << "找到 " << result->interval;
        } else {
            std::cout << "未找到";
        }
        std::cout << std::endl;
    }

    std::cout << "是否有效区间树: " << (it.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "第14章 区间树演示程序" << std::endl;
    std::cout << "======================" << std::endl;

    test_basic_operations();
    test_interval_search();
    test_delete_operations();
    test_edge_cases();
    test_complex_intervals();

    std::cout << "所有测试完成！" << std::endl;

    return 0;
}
