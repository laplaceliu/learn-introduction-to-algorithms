#include <iostream>
#include <vector>
#include <string>
#include "binary_search_tree.h"

using namespace algorithms;

void test_basic_operations() {
    std::cout << "=== 测试基本操作 ===" << std::endl;

    BinarySearchTree<int> bst;

    // 测试插入
    std::cout << "插入元素: 5, 3, 7, 2, 4, 6, 8" << std::endl;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(2);
    bst.insert(4);
    bst.insert(6);
    bst.insert(8);

    // 测试遍历
    std::cout << "中序遍历: ";
    auto inorder = bst.inorder_traversal();
    for (int val : inorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "前序遍历: ";
    auto preorder = bst.preorder_traversal();
    for (int val : preorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "后序遍历: ";
    auto postorder = bst.postorder_traversal();
    for (int val : postorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "层序遍历: ";
    auto levelorder = bst.level_order_traversal();
    for (int val : levelorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 测试最小值和最大值
    std::cout << "最小值: " << bst.minimum() << std::endl;
    std::cout << "最大值: " << bst.maximum() << std::endl;

    // 测试查找
    std::cout << "查找 4: " << (bst.search(4) != nullptr ? "找到" : "未找到") << std::endl;
    std::cout << "查找 10: " << (bst.search(10) != nullptr ? "找到" : "未找到") << std::endl;

    // 测试后继和前驱
    std::cout << "4的后继: " << bst.successor(4) << std::endl;
    std::cout << "5的前驱: " << bst.predecessor(5) << std::endl;

    std::cout << "树高度: " << bst.height() << std::endl;
    std::cout << "是否有效BST: " << (bst.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void test_delete_operations() {
    std::cout << "=== 测试删除操作 ===" << std::endl;

    BinarySearchTree<int> bst;

    // 构建测试树
    std::vector<int> values = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};
    for (int val : values) {
        bst.insert(val);
    }

    std::cout << "初始树中序遍历: ";
    auto initial = bst.inorder_traversal();
    for (int val : initial) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 测试删除叶子节点
    std::cout << "删除叶子节点 2: " << (bst.remove(2) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete1 = bst.inorder_traversal();
    for (int val : after_delete1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 测试删除只有一个子节点的节点
    std::cout << "删除只有一个子节点的节点 7: " << (bst.remove(7) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete2 = bst.inorder_traversal();
    for (int val : after_delete2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 测试删除有两个子节点的节点
    std::cout << "删除有两个子节点的节点 6: " << (bst.remove(6) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete3 = bst.inorder_traversal();
    for (int val : after_delete3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 测试删除根节点
    std::cout << "删除根节点 15: " << (bst.remove(15) ? "成功" : "失败") << std::endl;
    std::cout << "删除后中序遍历: ";
    auto after_delete4 = bst.inorder_traversal();
    for (int val : after_delete4) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "是否有效BST: " << (bst.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;

    // 测试空树
    BinarySearchTree<int> empty_bst;
    std::cout << "空树是否为空: " << (empty_bst.empty() ? "是" : "否") << std::endl;

    try {
        empty_bst.minimum();
        std::cout << "ERROR: 应该抛出异常" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "正确捕获空树最小值异常: " << e.what() << std::endl;
    }

    // 测试单节点树
    BinarySearchTree<int> single_bst;
    single_bst.insert(42);
    std::cout << "单节点树最小值: " << single_bst.minimum() << std::endl;
    std::cout << "单节点树最大值: " << single_bst.maximum() << std::endl;

    try {
        single_bst.successor(42);
        std::cout << "ERROR: 应该抛出异常" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "正确捕获单节点后继异常: " << e.what() << std::endl;
    }

    // 测试重复插入
    BinarySearchTree<int> duplicate_bst;
    duplicate_bst.insert(5);
    duplicate_bst.insert(5); // 重复插入
    duplicate_bst.insert(3);
    duplicate_bst.insert(7);

    std::cout << "重复插入后中序遍历: ";
    auto duplicate_inorder = duplicate_bst.inorder_traversal();
    for (int val : duplicate_inorder) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "是否有效BST: " << (duplicate_bst.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void test_string_bst() {
    std::cout << "=== 测试字符串BST ===" << std::endl;

    BinarySearchTree<std::string> string_bst;

    // 插入一些字符串
    string_bst.insert("apple");
    string_bst.insert("banana");
    string_bst.insert("cherry");
    string_bst.insert("date");
    string_bst.insert("elderberry");

    std::cout << "字符串BST中序遍历: ";
    auto string_inorder = string_bst.inorder_traversal();
    for (const auto& str : string_inorder) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    std::cout << "最小值: " << string_bst.minimum() << std::endl;
    std::cout << "最大值: " << string_bst.maximum() << std::endl;

    // 测试查找
    std::cout << "查找 'banana': " << (string_bst.search("banana") != nullptr ? "找到" : "未找到") << std::endl;
    std::cout << "查找 'fig': " << (string_bst.search("fig") != nullptr ? "找到" : "未找到") << std::endl;

    std::cout << "是否有效BST: " << (string_bst.is_valid() ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "第12章 二叉搜索树演示程序" << std::endl;
    std::cout << "========================" << std::endl;

    test_basic_operations();
    test_delete_operations();
    test_edge_cases();
    test_string_bst();

    std::cout << "所有测试完成！" << std::endl;

    return 0;
}
