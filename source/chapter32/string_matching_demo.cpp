#include "string_matching.h"
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace algorithms;

// 测试朴素字符串匹配算法
void test_naive_matcher() {
  std::cout << "=== 测试朴素字符串匹配算法 (32.1) ===" << std::endl;

  // 基本测试
  std::string text1 = "ABABABACABA";
  std::string pattern1 = "ABA";

  NaiveStringMatcher::search_with_offset(text1, pattern1);

  // 边界情况测试
  std::cout << "\n边界情况测试:" << std::endl;

  // 空模式
  auto empty_pattern = NaiveStringMatcher::search("hello", "");
  std::cout << "空模式匹配数: " << empty_pattern.size() << std::endl;

  // 模式比文本长
  auto long_pattern = NaiveStringMatcher::search("hi", "hello");
  std::cout << "模式比文本长匹配数: " << long_pattern.size() << std::endl;

  // 精确匹配
  auto exact_match = NaiveStringMatcher::search("hello", "hello");
  std::cout << "精确匹配位置: ";
  for (int pos : exact_match) {
    std::cout << pos << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
}

// 测试Rabin-Karp算法
void test_rabin_karp_matcher() {
  std::cout << "=== 测试Rabin-Karp算法 (32.2) ===" << std::endl;

  // 基本测试
  std::string text1 = "ABABABACABA";
  std::string pattern1 = "ABA";

  RabinKarpMatcher::search_with_hash(text1, pattern1);

  // 测试哈希冲突
  std::cout << "\n哈希冲突测试:" << std::endl;
  std::string text2 = "ABCDEFGHIJ";
  std::string pattern2 = "ABC";

  auto matches = RabinKarpMatcher::search(text2, pattern2);
  std::cout << "文本: " << text2 << std::endl;
  std::cout << "模式: " << pattern2 << std::endl;
  std::cout << "匹配位置: ";
  for (int pos : matches) {
    std::cout << pos << " ";
  }
  std::cout << std::endl;

  // 测试多个匹配
  std::cout << "\n多个匹配测试:" << std::endl;
  std::string text3 = "AAAAAA";
  std::string pattern3 = "AA";

  auto multi_matches = RabinKarpMatcher::search(text3, pattern3);
  std::cout << "文本: " << text3 << std::endl;
  std::cout << "模式: " << pattern3 << std::endl;
  std::cout << "匹配位置: ";
  for (int pos : multi_matches) {
    std::cout << pos << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
}

// 测试有限自动机算法
void test_finite_automaton_matcher() {
  std::cout << "=== 测试有限自动机算法 (32.3) ===" << std::endl;

  // 基本测试
  std::string text1 = "ABABABACABA";
  std::string pattern1 = "ABA";

  FiniteAutomatonMatcher::search_with_transition(text1, pattern1);

  // 测试不同模式
  std::cout << "\n不同模式测试:" << std::endl;

  std::string text2 = "AABAACAADAABAABA";
  std::string pattern2 = "AABA";

  FiniteAutomatonMatcher::search_with_transition(text2, pattern2);

  // 测试单字符模式
  std::cout << "\n单字符模式测试:" << std::endl;
  std::string text3 = "ABCDEFG";
  std::string pattern3 = "D";

  auto matches = FiniteAutomatonMatcher::search(text3, pattern3);
  std::cout << "文本: " << text3 << std::endl;
  std::cout << "模式: " << pattern3 << std::endl;
  std::cout << "匹配位置: ";
  for (int pos : matches) {
    std::cout << pos << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
}

// 测试KMP算法
void test_kmp_matcher() {
  std::cout << "=== 测试KMP算法 (32.4) ===" << std::endl;

  // 基本测试
  std::string text1 = "ABABABACABA";
  std::string pattern1 = "ABA";

  KMPMatcher::search_with_prefix(text1, pattern1);

  // 测试算法导论示例
  std::cout << "\n算法导论示例测试:" << std::endl;
  std::string text2 = "bacbababaabcbab";
  std::string pattern2 = "ababaca";

  KMPMatcher::search_with_prefix(text2, pattern2);

  // 测试重复模式
  std::cout << "\n重复模式测试:" << std::endl;
  std::string text3 = "AAAAAAA";
  std::string pattern3 = "AAA";

  auto matches = KMPMatcher::search(text3, pattern3);
  std::cout << "文本: " << text3 << std::endl;
  std::cout << "模式: " << pattern3 << std::endl;
  std::cout << "匹配位置: ";
  for (int pos : matches) {
    std::cout << pos << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
}

// 性能比较测试
void test_performance_comparison() {
  std::cout << "=== 性能比较测试 ===" << std::endl;

  // 生成测试数据
  std::string long_text = StringMatchingUtils::generate_test_text(1000);
  std::string pattern = "abc";

  // 插入一些模式出现
  for (int i = 0; i < 10; i++) {
    int pos = rand() % (long_text.length() - pattern.length());
    for (int j = 0; j < pattern.length(); j++) {
      long_text[pos + j] = pattern[j];
    }
  }

  // 朴素算法性能
  auto start = std::chrono::high_resolution_clock::now();
  auto naive_matches = NaiveStringMatcher::search(long_text, pattern);
  auto end = std::chrono::high_resolution_clock::now();
  auto naive_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // Rabin-Karp算法性能
  start = std::chrono::high_resolution_clock::now();
  auto rk_matches = RabinKarpMatcher::search(long_text, pattern);
  end = std::chrono::high_resolution_clock::now();
  auto rk_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // 有限自动机算法性能
  start = std::chrono::high_resolution_clock::now();
  auto fa_matches = FiniteAutomatonMatcher::search(long_text, pattern);
  end = std::chrono::high_resolution_clock::now();
  auto fa_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // KMP算法性能
  start = std::chrono::high_resolution_clock::now();
  auto kmp_matches = KMPMatcher::search(long_text, pattern);
  end = std::chrono::high_resolution_clock::now();
  auto kmp_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "文本长度: " << long_text.length() << std::endl;
  std::cout << "模式长度: " << pattern.length() << std::endl;
  std::cout << "匹配次数: " << naive_matches.size() << std::endl;
  std::cout << std::endl;

  std::cout << "性能比较:" << std::endl;
  std::cout << "朴素算法: " << naive_duration.count() << " 微秒" << std::endl;
  std::cout << "Rabin-Karp: " << rk_duration.count() << " 微秒" << std::endl;
  std::cout << "有限自动机: " << fa_duration.count() << " 微秒" << std::endl;
  std::cout << "KMP算法: " << kmp_duration.count() << " 微秒" << std::endl;

  // 验证结果一致性
  bool consistent = (naive_matches == rk_matches) &&
                    (rk_matches == fa_matches) && (fa_matches == kmp_matches);
  std::cout << "结果一致性: " << (consistent ? "一致" : "不一致") << std::endl;

  std::cout << std::endl;
}

// 算法导论示例测试
void test_clrs_examples() {
  std::cout << "=== 算法导论示例测试 ===" << std::endl;

  // 示例1: 图32.1
  std::cout << "示例1 (图32.1):" << std::endl;
  std::string text1 = "acaabc";
  std::string pattern1 = "aab";

  StringMatchingUtils::compare_algorithms(text1, pattern1);

  // 示例2: 图32.4
  std::cout << "示例2 (图32.4):" << std::endl;
  std::string text2 = "aaababaabaababaab";
  std::string pattern2 = "aabab";

  StringMatchingUtils::compare_algorithms(text2, pattern2);

  // 示例3: 图32.7 (Rabin-Karp)
  std::cout << "示例3 (图32.7 - Rabin-Karp):" << std::endl;
  std::string text3 = "2359023141526739921";
  std::string pattern3 = "31415";

  RabinKarpMatcher::search_with_hash(text3, pattern3);

  // 示例4: 图32.10 (有限自动机)
  std::cout << "示例4 (图32.10 - 有限自动机):" << std::endl;
  std::string text4 = "aaababaabaababaab";
  std::string pattern4 = "ababaca";

  FiniteAutomatonMatcher::search_with_transition(text4, pattern4);

  // 示例5: 图32.11 (KMP)
  std::cout << "示例5 (图32.11 - KMP):" << std::endl;
  std::string text5 = "bacbababaabcbab";
  std::string pattern5 = "ababaca";

  KMPMatcher::search_with_prefix(text5, pattern5);

  std::cout << std::endl;
}

// 边界情况和错误处理测试
void test_edge_cases() {
  std::cout << "=== 边界情况和错误处理测试 ===" << std::endl;

  // 测试空文本
  std::cout << "空文本测试:" << std::endl;
  auto empty_text = NaiveStringMatcher::search("", "hello");
  std::cout << "空文本匹配数: " << empty_text.size() << std::endl;

  // 测试空模式和空文本
  auto both_empty = NaiveStringMatcher::search("", "");
  std::cout << "空文本和空模式匹配数: " << both_empty.size() << std::endl;

  // 测试Unicode字符
  std::cout << "\nUnicode字符测试:" << std::endl;
  std::string unicode_text = "Hello 世界 你好";
  std::string unicode_pattern = "世界";

  auto unicode_matches = KMPMatcher::search(unicode_text, unicode_pattern);
  std::cout << "Unicode文本: " << unicode_text << std::endl;
  std::cout << "Unicode模式: " << unicode_pattern << std::endl;
  std::cout << "匹配位置: ";
  for (int pos : unicode_matches) {
    std::cout << pos << " ";
  }
  std::cout << std::endl;

  // 测试特殊字符
  std::cout << "\n特殊字符测试:" << std::endl;
  std::string special_text = "Line 1\nLine 2\tTab";
  std::string special_pattern = "\n";

  auto special_matches =
      FiniteAutomatonMatcher::search(special_text, special_pattern);
  std::cout << "特殊字符文本匹配数: " << special_matches.size() << std::endl;

  std::cout << std::endl;
}

int main() {
  std::cout << "算法导论第32章 字符串匹配演示程序" << std::endl;
  std::cout << "==================================" << std::endl;
  std::cout << std::endl;

  // 设置随机种子
  srand(static_cast<unsigned int>(time(nullptr)));

  test_naive_matcher();
  test_rabin_karp_matcher();
  test_finite_automaton_matcher();
  test_kmp_matcher();
  test_performance_comparison();
  test_clrs_examples();
  test_edge_cases();

  std::cout << "所有测试完成!" << std::endl;

  return 0;
}
