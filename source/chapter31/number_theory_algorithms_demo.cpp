#include "number_theory_algorithms.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace algorithms;

/**
 * @brief 性能测试函数
 * @param test_name 测试名称
 * @param test_func 测试函数
 * @return 执行时间（微秒）
 */
long long performance_test(const std::string &test_name,
                           std::function<void()> test_func) {
  auto start = std::chrono::high_resolution_clock::now();
  test_func();
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << test_name << " - 时间: " << duration.count() << " 微秒"
            << std::endl;

  return duration.count();
}

/**
 * @brief 测试最大公约数算法
 */
void test_gcd() {
  std::cout << "=== 最大公约数（GCD）算法测试 ===" << std::endl;

  // 测试用例
  std::vector<std::tuple<int, int, int>> test_cases = {
      {48, 18, 6}, {56, 42, 14}, {1071, 462, 21}, {0, 5, 5},
      {17, 0, 17}, {-48, 18, 6}, {48, -18, 6}};

  for (const auto &test_case : test_cases) {
    int a = std::get<0>(test_case);
    int b = std::get<1>(test_case);
    int expected = std::get<2>(test_case);

    int result_recursive = NumberTheoryAlgorithms::gcd(a, b);
    int result_iterative = NumberTheoryAlgorithms::gcd_iterative(a, b);

    bool verified_recursive = (result_recursive == expected);
    bool verified_iterative = (result_iterative == expected);
    bool consistent = (result_recursive == result_iterative);

    std::cout << "GCD(" << a << ", " << b << ") = " << result_recursive
              << " | ";
    std::cout << "递归验证: " << (verified_recursive ? "通过" : "失败")
              << " | ";
    std::cout << "迭代验证: " << (verified_iterative ? "通过" : "失败")
              << " | ";
    std::cout << "一致性: " << (consistent ? "通过" : "失败") << std::endl;
  }

  // 性能测试
  std::cout << "\nGCD算法性能测试:" << std::endl;
  performance_test("递归GCD", []() {
    for (int i = 0; i < 1000; ++i) {
      NumberTheoryAlgorithms::gcd(1071, 462);
    }
  });

  performance_test("迭代GCD", []() {
    for (int i = 0; i < 1000; ++i) {
      NumberTheoryAlgorithms::gcd_iterative(1071, 462);
    }
  });
}

/**
 * @brief 测试扩展欧几里得算法
 */
void test_extended_gcd() {
  std::cout << "\n=== 扩展欧几里得算法测试 ===" << std::endl;

  std::vector<std::pair<int, int>> test_cases = {
      {48, 18}, {56, 42}, {1071, 462}, {17, 13}};

  for (const auto &test_case : test_cases) {
    int a = test_case.first;
    int b = test_case.second;

    int x, y;
    int gcd_val = NumberTheoryAlgorithms::extended_gcd(a, b, x, y);

    bool verified = (a * x + b * y) == gcd_val;

    std::cout << "扩展GCD(" << a << ", " << b << ") = " << gcd_val << ", ";
    std::cout << "x = " << x << ", y = " << y << " | ";
    std::cout << "验证: " << (verified ? "通过" : "失败") << std::endl;
  }
}

/**
 * @brief 测试模运算
 */
void test_modular_arithmetic() {
  std::cout << "\n=== 模运算测试 ===" << std::endl;

  // 模运算测试
  std::cout << "模运算测试:" << std::endl;
  std::cout << "17 mod 5 = " << NumberTheoryAlgorithms::mod(17, 5)
            << " (期望: 2)" << std::endl;
  std::cout << "-17 mod 5 = " << NumberTheoryAlgorithms::mod(-17, 5)
            << " (期望: 3)" << std::endl;

  // 模加法测试
  std::cout << "\n模加法测试:" << std::endl;
  std::cout << "(17 + 8) mod 5 = " << NumberTheoryAlgorithms::mod_add(17, 8, 5)
            << " (期望: 0)" << std::endl;

  // 模乘法测试
  std::cout << "\n模乘法测试:" << std::endl;
  std::cout << "(17 * 3) mod 5 = "
            << NumberTheoryAlgorithms::mod_multiply(17, 3, 5) << " (期望: 1)"
            << std::endl;

  // 模逆元测试
  std::cout << "\n模逆元测试:" << std::endl;
  int inverse = NumberTheoryAlgorithms::mod_inverse(3, 11);
  std::cout << "3在模11下的逆元 = " << inverse << " (期望: 4)" << std::endl;
  if (inverse != -1) {
    std::cout << "验证: 3 * " << inverse << " mod 11 = "
              << NumberTheoryAlgorithms::mod_multiply(3, inverse, 11)
              << " (期望: 1)" << std::endl;
  }
}

/**
 * @brief 测试模指数运算
 */
void test_modular_exponentiation() {
  std::cout << "\n=== 模指数运算测试 ===" << std::endl;

  std::vector<std::tuple<int, int, int>> test_cases = {
      {2, 10, 13}, // 2^10 mod 13
      {7, 4, 11},  // 7^4 mod 11
      {3, 5, 7},   // 3^5 mod 7
      {5, 3, 13}   // 5^3 mod 13
  };

  for (const auto &test_case : test_cases) {
    int a = std::get<0>(test_case);
    int b = std::get<1>(test_case);
    int n = std::get<2>(test_case);

    int result = NumberTheoryAlgorithms::mod_exponentiation(a, b, n);
    bool verified = NumberTheoryAlgorithms::verify_mod_exponentiation(a, b, n);

    std::cout << a << "^" << b << " mod " << n << " = " << result << " | ";
    std::cout << "验证: " << (verified ? "通过" : "失败") << std::endl;
  }

  // 性能测试
  std::cout << "\n模指数运算性能测试:" << std::endl;
  performance_test("大数模指数", []() {
    NumberTheoryAlgorithms::mod_exponentiation(1234567, 1000, 10000019);
  });
}

/**
 * @brief 测试素数算法
 */
void test_prime_algorithms() {
  std::cout << "\n=== 素数算法测试 ===" << std::endl;

  // 素数测试
  std::vector<int> test_numbers = {2, 3, 17, 19, 21, 25, 29, 31, 37, 49};

  std::cout << "素数测试:" << std::endl;
  for (int num : test_numbers) {
    bool is_prime = NumberTheoryAlgorithms::is_prime(num);
    std::cout << num << " 是素数: " << (is_prime ? "是" : "否") << std::endl;
  }

  // 生成素数测试
  std::cout << "\n生成素数测试:" << std::endl;
  for (int bits : {8, 10, 12}) {
    int prime = NumberTheoryAlgorithms::generate_prime(bits);
    bool verified = NumberTheoryAlgorithms::is_prime(prime);
    std::cout << "生成 " << bits << " 位素数: " << prime << " | ";
    std::cout << "验证: " << (verified ? "通过" : "失败") << std::endl;
  }
}

/**
 * @brief 测试中国剩余定理
 */
void test_chinese_remainder_theorem() {
  std::cout << "\n=== 中国剩余定理测试 ===" << std::endl;

  // 测试用例1: x ≡ 2 (mod 3), x ≡ 3 (mod 5), x ≡ 2 (mod 7)
  std::vector<int> a1 = {2, 3, 2};
  std::vector<int> n1 = {3, 5, 7};

  try {
    int result1 = NumberTheoryAlgorithms::chinese_remainder_theorem(a1, n1);
    std::cout << "同余方程组: x ≡ 2 (mod 3), x ≡ 3 (mod 5), x ≡ 2 (mod 7)"
              << std::endl;
    std::cout << "解: x = " << result1 << std::endl;

    // 验证解
    bool verified = true;
    for (size_t i = 0; i < a1.size(); ++i) {
      if (NumberTheoryAlgorithms::mod(result1, n1[i]) != a1[i]) {
        verified = false;
        break;
      }
    }
    std::cout << "验证: " << (verified ? "通过" : "失败") << std::endl;
  } catch (const std::exception &e) {
    std::cout << "错误: " << e.what() << std::endl;
  }

  // 测试用例2: x ≡ 1 (mod 2), x ≡ 2 (mod 3), x ≡ 3 (mod 5)
  std::vector<int> a2 = {1, 2, 3};
  std::vector<int> n2 = {2, 3, 5};

  try {
    int result2 = NumberTheoryAlgorithms::chinese_remainder_theorem(a2, n2);
    std::cout << "\n同余方程组: x ≡ 1 (mod 2), x ≡ 2 (mod 3), x ≡ 3 (mod 5)"
              << std::endl;
    std::cout << "解: x = " << result2 << std::endl;

    // 验证解
    bool verified = true;
    for (size_t i = 0; i < a2.size(); ++i) {
      if (NumberTheoryAlgorithms::mod(result2, n2[i]) != a2[i]) {
        verified = false;
        break;
      }
    }
    std::cout << "验证: " << (verified ? "通过" : "失败") << std::endl;
  } catch (const std::exception &e) {
    std::cout << "错误: " << e.what() << std::endl;
  }
}

/**
 * @brief 测试RSA加密算法
 */
void test_rsa_algorithm() {
  std::cout << "\n=== RSA加密算法测试 ===" << std::endl;

  // 创建RSA实例
  NumberTheoryAlgorithms::RSA rsa(8); // 使用8位素数生成RSA密钥

  std::cout << rsa.to_string() << std::endl;

  // 测试加密解密
  std::vector<int> test_messages = {42, 123, 255, 1024};

  for (int message : test_messages) {
    try {
      int encrypted = rsa.encrypt(message);
      int decrypted = rsa.decrypt(encrypted);
      bool verified = (message == decrypted);

      std::cout << "消息: " << message << " -> 加密: " << encrypted
                << " -> 解密: " << decrypted;
      std::cout << " | 验证: " << (verified ? "通过" : "失败") << std::endl;
    } catch (const std::exception &e) {
      std::cout << "消息 " << message << " 处理失败: " << e.what() << std::endl;
    }
  }

  // 性能测试
  std::cout << "\nRSA加密解密性能测试:" << std::endl;
  performance_test("RSA加密", [&rsa]() {
    for (int i = 0; i < 10; ++i) {
      rsa.encrypt(123);
    }
  });

  int encrypted = rsa.encrypt(123);
  performance_test("RSA解密", [&rsa, encrypted]() {
    for (int i = 0; i < 10; ++i) {
      rsa.decrypt(encrypted);
    }
  });
}

/**
 * @brief 测试算法导论第31章示例问题
 */
void test_textbook_examples() {
  std::cout << "\n=== 算法导论第31章示例问题测试 ===" << std::endl;

  // 示例31.1: GCD计算
  std::cout << "示例31.1: GCD(30, 21) = " << NumberTheoryAlgorithms::gcd(30, 21)
            << " (期望: 3)" << std::endl;

  // 示例31.2: 扩展欧几里得算法
  int x, y;
  int gcd_val = NumberTheoryAlgorithms::extended_gcd(99, 78, x, y);
  std::cout << "示例31.2: 扩展GCD(99, 78) = " << gcd_val << ", x = " << x
            << ", y = " << y << std::endl;
  std::cout << "验证: 99*" << x << " + 78*" << y << " = " << (99 * x + 78 * y)
            << " (期望: " << gcd_val << ")" << std::endl;

  // 示例31.6: 模指数运算
  std::cout << "示例31.6: 7^560 mod 561 = "
            << NumberTheoryAlgorithms::mod_exponentiation(7, 560, 561)
            << std::endl;
}

int main() {
  std::cout << "=== 算法导论 第31章 - 数论算法演示 ===" << std::endl;
  std::cout << std::endl;

  try {
    // 测试算法导论示例问题
    test_textbook_examples();

    // 测试最大公约数算法
    test_gcd();

    // 测试扩展欧几里得算法
    test_extended_gcd();

    // 测试模运算
    test_modular_arithmetic();

    // 测试模指数运算
    test_modular_exponentiation();

    // 测试素数算法
    test_prime_algorithms();

    // 测试中国剩余定理
    test_chinese_remainder_theorem();

    // 测试RSA加密算法
    test_rsa_algorithm();

    std::cout << "\n=== 测试完成 ===" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "测试过程中发生错误: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
