#include <iostream>
#include <vector>
#include <string>
#include "longest_common_subsequence.h"

using namespace algorithms;

void test_classic_examples() {
    std::cout << "=== 测试经典示例 ===" << std::endl;

    // 算法导论图15-8示例
    std::string s1 = "ABCBDAB";
    std::string s2 = "BDCAB";

    LongestCommonSubsequence lcs1(s1, s2);
    auto result1 = lcs1.lcs_with_string();

    std::cout << "示例1（算法导论图15-8）:" << std::endl;
    std::cout << "  序列1: " << s1 << std::endl;
    std::cout << "  序列2: " << s2 << std::endl;
    std::cout << "  最长公共子序列长度: " << result1.length << std::endl;
    std::cout << "  最长公共子序列: " << result1.lcs_string << std::endl;
    std::cout << "  期望结果: BCAB (长度4)" << std::endl;

    if (result1.length == 4 && (result1.lcs_string == "BCAB" || result1.lcs_string == "BDAB")) {
        std::cout << "  验证: 正确" << std::endl;
    } else {
        std::cout << "  验证: 错误" << std::endl;
    }
    std::cout << std::endl;

    // 另一个经典示例
    std::string s3 = "XMJYAUZ";
    std::string s4 = "MZJAWXU";

    LongestCommonSubsequence lcs2(s3, s4);
    auto result2 = lcs2.lcs_with_string();

    std::cout << "示例2:" << std::endl;
    std::cout << "  序列1: " << s3 << std::endl;
    std::cout << "  序列2: " << s4 << std::endl;
    std::cout << "  最长公共子序列长度: " << result2.length << std::endl;
    std::cout << "  最长公共子序列: " << result2.lcs_string << std::endl;
    std::cout << "  期望结果: MJAU (长度4)" << std::endl;

    if (result2.length == 4 && result2.lcs_string == "MJAU") {
        std::cout << "  验证: 正确" << std::endl;
    } else {
        std::cout << "  验证: 错误" << std::endl;
    }
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;

    // 空序列测试
    std::string empty1 = "";
    std::string empty2 = "";
    std::string non_empty = "ABC";

    LongestCommonSubsequence lcs1(empty1, empty2);
    auto result1 = lcs1.lcs_with_string();

    std::cout << "空序列测试:" << std::endl;
    std::cout << "  序列1: \"\"" << std::endl;
    std::cout << "  序列2: \"\"" << std::endl;
    std::cout << "  最长公共子序列长度: " << result1.length << std::endl;
    std::cout << "  最长公共子序列: \"" << result1.lcs_string << "\"" << std::endl;

    if (result1.length == 0 && result1.lcs_string.empty()) {
        std::cout << "  验证: 正确" << std::endl;
    } else {
        std::cout << "  验证: 错误" << std::endl;
    }
    std::cout << std::endl;

    // 一个空序列测试
    LongestCommonSubsequence lcs2(empty1, non_empty);
    auto result2 = lcs2.lcs_with_string();

    std::cout << "一个空序列测试:" << std::endl;
    std::cout << "  序列1: \"\"" << std::endl;
    std::cout << "  序列2: " << non_empty << std::endl;
    std::cout << "  最长公共子序列长度: " << result2.length << std::endl;
    std::cout << "  最长公共子序列: \"" << result2.lcs_string << "\"" << std::endl;

    if (result2.length == 0 && result2.lcs_string.empty()) {
        std::cout << "  验证: 正确" << std::endl;
    } else {
        std::cout << "  验证: 错误" << std::endl;
    }
    std::cout << std::endl;
}

void test_identical_sequences() {
    std::cout << "=== 测试相同序列 ===" << std::endl;

    std::string s1 = "ABCDEFG";
    std::string s2 = "ABCDEFG";

    LongestCommonSubsequence lcs(s1, s2);
    auto result = lcs.lcs_with_string();

    std::cout << "相同序列测试:" << std::endl;
    std::cout << "  序列1: " << s1 << std::endl;
    std::cout << "  序列2: " << s2 << std::endl;
    std::cout << "  最长公共子序列长度: " << result.length << std::endl;
    std::cout << "  最长公共子序列: " << result.lcs_string << std::endl;

    if (result.length == s1.length() && result.lcs_string == s1) {
        std::cout << "  验证: 正确" << std::endl;
    } else {
        std::cout << "  验证: 错误" << std::endl;
    }
    std::cout << std::endl;
}

void test_no_common_subsequence() {
    std::cout << "=== 测试无公共子序列 ===" << std::endl;

    std::string s1 = "ABCD";
    std::string s2 = "EFGH";

    LongestCommonSubsequence lcs(s1, s2);
    auto result = lcs.lcs_with_string();

    std::cout << "无公共子序列测试:" << std::endl;
    std::cout << "  序列1: " << s1 << std::endl;
    std::cout << "  序列2: " << s2 << std::endl;
    std::cout << "  最长公共子序列长度: " << result.length << std::endl;
    std::cout << "  最长公共子序列: \"" << result.lcs_string << "\"" << std::endl;

    if (result.length == 0 && result.lcs_string.empty()) {
        std::cout << "  验证: 正确" << std::endl;
    } else {
        std::cout << "  验证: 错误" << std::endl;
    }
    std::cout << std::endl;
}

void test_dp_table_visualization() {
    std::cout << "=== DP表可视化测试 ===" << std::endl;

    std::string s1 = "ABCBDAB";
    std::string s2 = "BDCAB";

    LongestCommonSubsequence lcs(s1, s2);

    std::cout << "序列1: " << s1 << std::endl;
    std::cout << "序列2: " << s2 << std::endl;
    std::cout << std::endl;

    // 打印DP表和方向表
    lcs.print_dp_table();

    auto result = lcs.lcs_with_string();
    std::cout << "\n最长公共子序列: " << result.lcs_string << " (长度" << result.length << ")" << std::endl;
    std::cout << std::endl;
}

void test_algorithm_correctness() {
    std::cout << "=== 算法正确性验证 ===" << std::endl;

    struct TestCase {
        std::string seq1;
        std::string seq2;
        int expected_length;
        std::vector<std::string> possible_lcs; // 可能有多个LCS
    };

    std::vector<TestCase> test_cases = {
        {"A", "A", 1, {"A"}},
        {"A", "B", 0, {""}},
        {"AB", "BA", 1, {"A", "B"}},
        {"ABCD", "ACBD", 3, {"ABD", "ACD"}},
        {"AGGTAB", "GXTXAYB", 4, {"GTAB"}},
        {"ABCDGH", "AEDFHR", 3, {"ADH"}},
        {"ABCD", "ABCD", 4, {"ABCD"}},
        {"AAA", "AAA", 3, {"AAA"}},
        {"ABC", "DEF", 0, {""}},
        {"", "", 0, {""}},
        {"", "ABC", 0, {""}},
        {"ABC", "", 0, {""}}
    };

    int passed = 0;
    int total = test_cases.size();

    for (const auto& test_case : test_cases) {
        LongestCommonSubsequence lcs(test_case.seq1, test_case.seq2);
        auto result = lcs.lcs_with_string();

        bool length_correct = (result.length == test_case.expected_length);
        bool lcs_valid = false;

        // 检查LCS字符串是否在可能的选项中
        for (const auto& possible : test_case.possible_lcs) {
            if (result.lcs_string == possible) {
                lcs_valid = true;
                break;
            }
        }

        std::cout << "测试: \"" << test_case.seq1 << "\" 和 \"" << test_case.seq2 << "\"" << std::endl;
        std::cout << "  期望长度: " << test_case.expected_length << ", 实际长度: " << result.length << std::endl;
        std::cout << "  实际LCS: \"" << result.lcs_string << "\"" << std::endl;

        if (length_correct && lcs_valid) {
            std::cout << "  验证: 正确" << std::endl;
            passed++;
        } else {
            std::cout << "  验证: 错误" << std::endl;
            if (!length_correct) {
                std::cout << "    长度不正确" << std::endl;
            }
            if (!lcs_valid) {
                std::cout << "    LCS字符串不在预期选项中" << std::endl;
            }
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

void test_large_sequences() {
    std::cout << "=== 测试较长序列 ===" << std::endl;

    // 测试DNA序列（简化版）
    std::string dna1 = "ATCGATCGATCG";
    std::string dna2 = "TGCATGCATGCAT";

    LongestCommonSubsequence lcs(dna1, dna2);
    auto result = lcs.lcs_with_string();

    std::cout << "DNA序列测试:" << std::endl;
    std::cout << "  序列1: " << dna1 << std::endl;
    std::cout << "  序列2: " << dna2 << std::endl;
    std::cout << "  最长公共子序列长度: " << result.length << std::endl;
    std::cout << "  最长公共子序列: " << result.lcs_string << std::endl;

    // 验证LCS的正确性
    bool valid = true;
    int pos1 = 0, pos2 = 0;

    for (char c : result.lcs_string) {
        // 检查字符是否在序列1中按顺序出现
        size_t found1 = dna1.find(c, pos1);
        if (found1 == std::string::npos) {
            valid = false;
            break;
        }
        pos1 = found1 + 1;

        // 检查字符是否在序列2中按顺序出现
        size_t found2 = dna2.find(c, pos2);
        if (found2 == std::string::npos) {
            valid = false;
            break;
        }
        pos2 = found2 + 1;
    }

    if (valid) {
        std::cout << "  LCS验证: 正确（是有效的公共子序列）" << std::endl;
    } else {
        std::cout << "  LCS验证: 错误" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "第15.4章 最长公共子序列演示程序" << std::endl;
    std::cout << "================================" << std::endl;

    test_classic_examples();
    test_edge_cases();
    test_identical_sequences();
    test_no_common_subsequence();
    test_algorithm_correctness();
    test_large_sequences();

    // 可选：DP表可视化（会输出较多内容）
    // test_dp_table_visualization();

    std::cout << "所有测试完成！" << std::endl;

    return 0;
}
