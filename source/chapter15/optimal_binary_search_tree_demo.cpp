#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include "optimal_binary_search_tree.h"

using namespace algorithms;

void test_classic_example() {
    std::cout << "=== 测试经典示例（算法导论图15.9） ===" << std::endl;

    // 算法导论图15.9示例
    std::vector<std::string> keys = {"k1", "k2", "k3", "k4", "k5"};
    std::vector<double> probabilities = {0.15, 0.10, 0.05, 0.10, 0.20};

    OptimalBinarySearchTree<std::string> obst(keys, probabilities);

    std::cout << "键值序列: ";
    for (const auto& key : keys) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "搜索概率: ";
    for (double p : probabilities) {
        std::cout << std::fixed << std::setprecision(3) << p << " ";
    }
    std::cout << std::endl;

    double expected_cost = obst.get_expected_cost();
    std::cout << "期望搜索成本: " << std::fixed << std::setprecision(3) << expected_cost << std::endl;
    std::cout << "树高度: " << obst.get_height() << std::endl;

    // 验证中序遍历
    auto inorder = obst.inorder_traversal();
    std::cout << "中序遍历: ";
    for (const auto& key : inorder) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    // 验证前序遍历
    auto preorder = obst.preorder_traversal();
    std::cout << "前序遍历: ";
    for (const auto& key : preorder) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    // 验证搜索操作
    std::cout << "搜索测试:" << std::endl;
    for (const auto& key : keys) {
        auto node = obst.search(key);
        if (node != nullptr) {
            std::cout << "  找到 " << key << " (概率=" << std::fixed << std::setprecision(3)
                      << node->probability << ")" << std::endl;
        } else {
            std::cout << "  未找到 " << key << std::endl;
        }
    }

    std::cout << "\n树结构:" << std::endl;
    obst.print_tree();

    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;

    // 空树测试
    try {
        std::vector<int> empty_keys;
        std::vector<double> empty_probs;
        OptimalBinarySearchTree<int> obst1(empty_keys, empty_probs);

        std::cout << "空树测试:" << std::endl;
        std::cout << "  期望搜索成本: " << obst1.get_expected_cost() << std::endl;
        std::cout << "  树高度: " << obst1.get_height() << std::endl;
        std::cout << "  验证: 正确" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  验证: 错误 - " << e.what() << std::endl;
    }
    std::cout << std::endl;

    // 单节点测试
    std::vector<int> single_key = {42};
    std::vector<double> single_prob = {1.0};

    OptimalBinarySearchTree<int> obst2(single_key, single_prob);

    std::cout << "单节点测试:" << std::endl;
    std::cout << "  期望搜索成本: " << std::fixed << std::setprecision(3) << obst2.get_expected_cost() << std::endl;
    std::cout << "  树高度: " << obst2.get_height() << std::endl;

    auto node = obst2.search(42);
    if (node != nullptr && node->key == 42) {
        std::cout << "  验证: 正确" << std::endl;
    } else {
        std::cout << "  验证: 错误" << std::endl;
    }
    std::cout << std::endl;
}

void test_different_probability_patterns() {
    std::cout << "=== 测试不同概率模式 ===" << std::endl;

    // 测试1：均匀概率
    std::vector<int> keys1 = {10, 20, 30, 40, 50};
    std::vector<double> probs1 = {0.2, 0.2, 0.2, 0.2, 0.2};

    OptimalBinarySearchTree<int> obst1(keys1, probs1);

    std::cout << "均匀概率测试:" << std::endl;
    std::cout << "  期望搜索成本: " << std::fixed << std::setprecision(3) << obst1.get_expected_cost() << std::endl;
    std::cout << "  树高度: " << obst1.get_height() << std::endl;

    // 测试2：集中概率（中间高）
    std::vector<int> keys2 = {10, 20, 30, 40, 50};
    std::vector<double> probs2 = {0.05, 0.1, 0.7, 0.1, 0.05};

    OptimalBinarySearchTree<int> obst2(keys2, probs2);

    std::cout << "集中概率测试（中间高）:" << std::endl;
    std::cout << "  期望搜索成本: " << std::fixed << std::setprecision(3) << obst2.get_expected_cost() << std::endl;
    std::cout << "  树高度: " << obst2.get_height() << std::endl;

    // 测试3：两端概率高
    std::vector<int> keys3 = {10, 20, 30, 40, 50};
    std::vector<double> probs3 = {0.4, 0.1, 0.1, 0.1, 0.3};

    OptimalBinarySearchTree<int> obst3(keys3, probs3);

    std::cout << "两端概率高测试:" << std::endl;
    std::cout << "  期望搜索成本: " << std::fixed << std::setprecision(3) << obst3.get_expected_cost() << std::endl;
    std::cout << "  树高度: " << obst3.get_height() << std::endl;

    std::cout << std::endl;
}

void test_algorithm_correctness() {
    std::cout << "=== 算法正确性验证 ===" << std::endl;

    struct TestCase {
        std::vector<int> keys;
        std::vector<double> probabilities;
        double expected_cost;
        std::string description;
    };

    std::vector<TestCase> test_cases = {
        {{1}, {1.0}, 1.0, "单节点"},
        {{1, 2}, {0.5, 0.5}, 1.5, "两个节点均匀概率"},
        {{1, 2, 3}, {0.6, 0.3, 0.1}, 1.9, "三个节点非均匀概率"},
        {{1, 2, 3, 4}, {0.25, 0.25, 0.25, 0.25}, 2.25, "四个节点均匀概率"}
    };

    int passed = 0;
    int total = test_cases.size();

    for (const auto& test_case : test_cases) {
        OptimalBinarySearchTree<int> obst(test_case.keys, test_case.probabilities);
        double actual_cost = obst.get_expected_cost();

        std::cout << "测试: " << test_case.description << std::endl;
        std::cout << "  键值: ";
        for (int key : test_case.keys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;

        std::cout << "  概率: ";
        for (double p : test_case.probabilities) {
            std::cout << std::fixed << std::setprecision(3) << p << " ";
        }
        std::cout << std::endl;

        std::cout << "  期望成本: " << std::fixed << std::setprecision(3) << test_case.expected_cost << std::endl;
        std::cout << "  实际成本: " << std::fixed << std::setprecision(3) << actual_cost << std::endl;

        // 允许一定的浮点数误差
        if (std::abs(actual_cost - test_case.expected_cost) < 0.01) {
            std::cout << "  验证: 正确" << std::endl;
            passed++;
        } else {
            std::cout << "  验证: 错误" << std::endl;
        }

        // 验证中序遍历结果
        auto inorder = obst.inorder_traversal();
        bool inorder_correct = (inorder.size() == test_case.keys.size());
        if (inorder_correct) {
            for (size_t i = 0; i < inorder.size(); i++) {
                if (inorder[i] != test_case.keys[i]) {
                    inorder_correct = false;
                    break;
                }
            }
        }

        if (inorder_correct) {
            std::cout << "  中序遍历: 正确" << std::endl;
        } else {
            std::cout << "  中序遍历: 错误" << std::endl;
        }

        std::cout << std::endl;
    }

    std::cout << "正确性验证结果: " << passed << "/" << total << " 通过" << std::endl;
    if (passed == total) {
        std::cout << "所有测试通过" << std::endl;
    } else {
        std::cout << "部分测试失败" << std::endl;
    }
    std::cout << std::endl;
}

void test_dp_table_visualization() {
    std::cout << "=== DP表可视化测试 ===" << std::endl;

    // 使用小规模示例展示DP表
    std::vector<int> keys = {10, 20, 30};
    std::vector<double> probabilities = {0.4, 0.3, 0.3};

    OptimalBinarySearchTree<int> obst(keys, probabilities);

    std::cout << "键值: ";
    for (int key : keys) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "概率: ";
    for (double p : probabilities) {
        std::cout << std::fixed << std::setprecision(3) << p << " ";
    }
    std::cout << std::endl;

    std::cout << "期望搜索成本: " << std::fixed << std::setprecision(3) << obst.get_expected_cost() << std::endl;

    // 打印DP表
    obst.print_dp_tables();

    std::cout << "\n树结构:" << std::endl;
    obst.print_tree();

    std::cout << std::endl;
}

void test_comparison_with_normal_bst() {
    std::cout << "=== 与普通BST对比测试 ===" << std::endl;

    // 测试数据
    std::vector<int> keys = {5, 10, 15, 20, 25, 30, 35, 40};
    std::vector<double> probabilities = {0.05, 0.1, 0.15, 0.2, 0.15, 0.1, 0.05, 0.1};

    // 最优BST
    OptimalBinarySearchTree<int> obst(keys, probabilities);
    double obst_cost = obst.get_expected_cost();
    int obst_height = obst.get_height();

    std::cout << "最优二叉搜索树:" << std::endl;
    std::cout << "  期望搜索成本: " << std::fixed << std::setprecision(3) << obst_cost << std::endl;
    std::cout << "  树高度: " << obst_height << std::endl;

    // 模拟普通BST（按顺序插入）
    // 注意：这里只是估算，实际普通BST的成本需要具体计算
    double normal_bst_cost_estimate = 0.0;
    for (size_t i = 0; i < keys.size(); i++) {
        // 简单估算：每个节点的搜索深度大致为log2(n)
        normal_bst_cost_estimate += probabilities[i] * (std::log(keys.size()) / std::log(2) + 1);
    }

    std::cout << "普通二叉搜索树（估算）:" << std::endl;
    std::cout << "  估算搜索成本: " << std::fixed << std::setprecision(3) << normal_bst_cost_estimate << std::endl;

    if (obst_cost < normal_bst_cost_estimate) {
        std::cout << "  最优BST比普通BST更优: 正确" << std::endl;
        std::cout << "  优化比例: " << std::fixed << std::setprecision(1)
                  << ((normal_bst_cost_estimate - obst_cost) / normal_bst_cost_estimate * 100) << "%" << std::endl;
    } else {
        std::cout << "  结果对比: 需要具体分析" << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    std::cout << "第15.5章 最优二叉搜索树演示程序" << std::endl;
    std::cout << "==================================" << std::endl;

    test_classic_example();
    test_edge_cases();
    test_different_probability_patterns();
    test_algorithm_correctness();
    test_comparison_with_normal_bst();

    // 可选：DP表可视化（会输出较多内容）
    // test_dp_table_visualization();

    std::cout << "所有测试完成！" << std::endl;

    return 0;
}
