#include <iostream>
#include <vector>
#include <string>
#include "../../include/red_black_tree.h"

using namespace algorithms;

void test_basic_operations() {
    std::cout << "=== 测试基本操作 ===" << std::endl;
    
    RedBlackTree<int> rbt;
    
    // 测试插入
    std::cout << "插入元素: 41, 38, 31, 12, 19, 8" << std::endl;
    rbt.insert(41);
    rbt.insert(38);
    rbt.insert(31);
    rbt.insert(12);
    rbt.insert(19);
    rbt.insert(8);
    
    // 测试遍历
    std::cout << "中序遍历: ";
    auto inorder = rbt.inorder_traversal();
    for (int val : inorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "前序遍历: ";
    auto preorder = rbt.preorder_traversal();
    for (int val : preorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "层序遍历: ";
    auto levelorder = rbt.level_order_traversal();
    for (int val : levelorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试最小值和最大值
    std::cout << "最小值: " << rbt.minimum() << std::endl;
    std::cout << "最大值: " << rbt.maximum() << std::endl;
    
    // 测试查找
    std::cout << "查找 19: " << (rbt.search(19) != nullptr ? "找到" : "未找到") << std::endl;
    std::cout << "查找 25: " << (rbt.search(25) != nullptr ? "找到" : "未找到") << std::endl;
    
    // 测试红黑树性质
    std::cout << "黑高度: " << rbt.black_height() << std::endl;
    std::cout << "是否有效红黑树: " << (rbt.is_valid() ? "是" : "否") << std::endl;
    
    std::cout << "树结构:" << std::endl;
    rbt.print_tree();
    std::cout << std::endl;
}

void test_delete_operations() {
    std::cout << "=== 测试删除操作 ===" << std::endl;
    
    RedBlackTree<int> rbt;
    
    // 构建测试树（算法导论图13-4示例）
    std::vector<int> values = {11, 2, 14, 1, 7, 15, 5, 8, 4};
    for (int val : values) {
        rbt.insert(val);
    }
    
    std::cout << "初始树中序遍历: ";
    auto initial = rbt.inorder_traversal();
    for (int val : initial) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "初始树结构:" << std::endl;
    rbt.print_tree();
    
    // 测试删除叶子节点
    std::cout << "删除叶子节点 1: " << (rbt.remove(1) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete1 = rbt.inorder_traversal();
    for (int val : after_delete1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    std::cout << "删除后树结构:" << std::endl;
    rbt.print_tree();
    
    // 测试删除只有一个子节点的节点
    std::cout << "删除节点 7: " << (rbt.remove(7) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete2 = rbt.inorder_traversal();
    for (int val : after_delete2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试删除有两个子节点的节点
    std::cout << "删除节点 11: " << (rbt.remove(11) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete3 = rbt.inorder_traversal();
    for (int val : after_delete3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "最终树结构:" << std::endl;
    rbt.print_tree();
    std::cout << "是否有效红黑树: " << (rbt.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;
    
    // 测试空树
    RedBlackTree<int> empty_rbt;
    std::cout << "空树是否为空: " << (empty_rbt.empty() ? "是" : "否") << std::endl;
    
    try {
        empty_rbt.minimum();
        std::cout << "ERROR: 应该抛出异常" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "正确捕获空树最小值异常: " << e.what() << std::endl;
    }
    
    // 测试单节点树
    RedBlackTree<int> single_rbt;
    single_rbt.insert(42);
    std::cout << "单节点树最小值: " << single_rbt.minimum() << std::endl;
    std::cout << "单节点树最大值: " << single_rbt.maximum() << std::endl;
    std::cout << "单节点树黑高度: " << single_rbt.black_height() << std::endl;
    std::cout << "是否有效红黑树: " << (single_rbt.is_valid() ? "是" : "否") << std::endl;
    
    // 测试重复插入
    RedBlackTree<int> duplicate_rbt;
    duplicate_rbt.insert(5);
    duplicate_rbt.insert(5); // 重复插入
    duplicate_rbt.insert(3);
    duplicate_rbt.insert(7);
    
    std::cout << "重复插入后中序遍历: ";
    auto duplicate_inorder = duplicate_rbt.inorder_traversal();
    for (int val : duplicate_inorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "是否有效红黑树: " << (duplicate_rbt.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void test_string_rbt() {
    std::cout << "=== 测试字符串红黑树 ===" << std::endl;
    
    RedBlackTree<std::string> string_rbt;
    
    // 插入一些字符串
    string_rbt.insert("apple");
    string_rbt.insert("banana");
    string_rbt.insert("cherry");
    string_rbt.insert("date");
    string_rbt.insert("elderberry");
    
    std::cout << "字符串红黑树中序遍历: ";
    auto string_inorder = string_rbt.inorder_traversal();
    for (const auto& str : string_inorder) {
        std::cout << str << " ";
    }
    std::cout << std::endl;
    
    std::cout << "最小值: " << string_rbt.minimum() << std::endl;
    std::cout << "最大值: " << string_rbt.maximum() << std::endl;
    
    // 测试查找
    std::cout << "查找 'banana': " << (string_rbt.search("banana") != nullptr ? "找到" : "未找到") << std::endl;
    std::cout << "查找 'fig': " << (string_rbt.search("fig") != nullptr ? "找到" : "未找到") << std::endl;
    
    std::cout << "是否有效红黑树: " << (string_rbt.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void test_complex_operations() {
    std::cout << "=== 测试复杂操作 ===" << std::endl;
    
    RedBlackTree<int> rbt;
    
    // 插入大量数据测试平衡性
    std::vector<int> data = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    for (int val : data) {
        rbt.insert(val);
    }
    
    std::cout << "插入15个元素后中序遍历: ";
    auto inorder = rbt.inorder_traversal();
    for (int val : inorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "树高度: " << rbt.black_height() << std::endl;
    std::cout << "是否有效红黑树: " << (rbt.is_valid() ? "是" : "否") << std::endl;
    
    // 测试删除多个节点
    std::vector<int> to_delete = {50, 30, 70, 20};
    for (int val : to_delete) {
        std::cout << "删除节点 " << val << ": " << (rbt.remove(val) ? "成功" : "失败") << std::endl;
    }
    
    std::cout << "删除后中序遍历: ";
    auto after_delete = rbt.inorder_traversal();
    for (int val : after_delete) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "删除后树高度: " << rbt.black_height() << std::endl;
    std::cout << "是否有效红黑树: " << (rbt.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "第13章 红黑树演示程序" << std::endl;
    std::cout << "======================" << std::endl;
    
    test_basic_operations();
    test_delete_operations();
    test_edge_cases();
    test_string_rbt();
    test_complex_operations();
    
    std::cout << "所有测试完成！" << std::endl;
    
    return 0;
}