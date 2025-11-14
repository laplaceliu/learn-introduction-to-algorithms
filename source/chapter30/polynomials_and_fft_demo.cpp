#include "polynomials_and_fft.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

/**
 * @brief 打印复数向量
 * @param vec 复数向量
 * @param name 向量名称
 */
void print_complex_vector(const std::vector<PolynomialsAndFFT::Complex> &vec,
                          const std::string &name) {
  std::cout << name << " = [";
  for (size_t i = 0; i < vec.size(); ++i) {
    double real = vec[i].real();
    double imag = vec[i].imag();

    std::cout << std::fixed << std::setprecision(4) << real;
    if (imag >= 0) {
      std::cout << " + " << std::abs(imag) << "i";
    } else {
      std::cout << " - " << std::abs(imag) << "i";
    }

    if (i < vec.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl;
}

/**
 * @brief 性能测试函数
 * @param test_name 测试名称
 * @param test_func 测试函数
 */
void performance_test(const std::string &test_name,
                      std::function<void()> test_func) {
  auto start = std::chrono::high_resolution_clock::now();
  test_func();
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << test_name << " - 时间: " << duration.count() << " 微秒"
            << std::endl;
}

/**
 * @brief 测试算法导论第30章示例问题
 */
void test_textbook_example() {
  std::cout << "=== 算法导论第30章示例问题测试 ===" << std::endl;

  // 示例1：简单多项式乘法
  std::cout << "\n示例1：简单多项式乘法" << std::endl;
  PolynomialsAndFFT::Polynomial A1({1, 2, 3}); // 1 + 2x + 3x²
  PolynomialsAndFFT::Polynomial B1({4, 5});    // 4 + 5x

  std::cout << "A(x) = " << A1.to_string() << std::endl;
  std::cout << "B(x) = " << B1.to_string() << std::endl;

  // 朴素乘法
  auto naive_result = PolynomialsAndFFT::naive_polynomial_multiply(A1, B1);
  std::cout << "朴素乘法结果: " << naive_result.to_string() << std::endl;

  // FFT乘法
  auto fft_result = PolynomialsAndFFT::fft_polynomial_multiply(A1, B1);
  std::cout << "FFT乘法结果: " << fft_result.to_string() << std::endl;

  // 验证结果
  bool verified =
      PolynomialsAndFFT::verify_polynomial_multiply(A1, B1, fft_result);
  std::cout << "结果验证: " << (verified ? "通过" : "失败") << std::endl;

  // 示例2：FFT计算
  std::cout << "\n示例2：快速傅里叶变换计算" << std::endl;
  std::vector<PolynomialsAndFFT::Complex> input = {
      {1, 0}, {2, 0}, {3, 0}, {4, 0}};

  print_complex_vector(input, "输入序列");

  auto fft_output = PolynomialsAndFFT::iterative_fft(input);
  print_complex_vector(fft_output, "FFT结果");

  auto ifft_output = PolynomialsAndFFT::iterative_fft(fft_output, true);
  print_complex_vector(ifft_output, "逆FFT结果");

  // 检查逆变换是否恢复原序列
  bool fft_reversible = true;
  for (size_t i = 0; i < input.size(); ++i) {
    if (std::abs(input[i].real() - ifft_output[i].real()) > 1e-6 ||
        std::abs(input[i].imag() - ifft_output[i].imag()) > 1e-6) {
      fft_reversible = false;
      break;
    }
  }
  std::cout << "FFT可逆性验证: " << (fft_reversible ? "通过" : "失败")
            << std::endl;
}

/**
 * @brief 测试不同规模的多项式乘法
 */
void test_different_sizes() {
  std::cout << "\n=== 不同规模多项式乘法测试 ===" << std::endl;

  std::vector<size_t> degrees = {3, 7, 15, 31, 63};

  for (size_t degree : degrees) {
    std::cout << "\n测试 " << degree << " 次多项式:" << std::endl;

    auto A = PolynomialsAndFFT::generate_random_polynomial(degree);
    auto B = PolynomialsAndFFT::generate_random_polynomial(degree);

    std::cout << "A(x) 次数: " << A.degree() << std::endl;
    std::cout << "B(x) 次数: " << B.degree() << std::endl;

    // 朴素乘法性能
    performance_test("朴素乘法", [&]() {
      auto result = PolynomialsAndFFT::naive_polynomial_multiply(A, B);
      std::cout << "  结果次数: " << result.degree() << std::endl;
    });

    // FFT乘法性能
    performance_test("FFT乘法", [&]() {
      auto result = PolynomialsAndFFT::fft_polynomial_multiply(A, B);
      std::cout << "  结果次数: " << result.degree() << std::endl;
    });

    // 验证FFT乘法结果
    auto fft_result = PolynomialsAndFFT::fft_polynomial_multiply(A, B);
    bool verified =
        PolynomialsAndFFT::verify_polynomial_multiply(A, B, fft_result);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  }
}

/**
 * @brief 测试FFT算法的正确性
 */
void test_fft_correctness() {
  std::cout << "\n=== FFT算法正确性测试 ===" << std::endl;

  // 测试不同长度的FFT
  std::vector<size_t> lengths = {2, 4, 8, 16, 32};

  for (size_t n : lengths) {
    std::cout << "\n测试长度 " << n << " 的FFT:" << std::endl;

    // 生成随机复数序列
    std::vector<PolynomialsAndFFT::Complex> input(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-10.0, 10.0);

    for (size_t i = 0; i < n; ++i) {
      input[i] = {dis(gen), dis(gen)};
    }

    // 递归FFT
    auto recursive_fft = PolynomialsAndFFT::fft(input);

    // 迭代FFT
    auto iterative_fft = PolynomialsAndFFT::iterative_fft(input);

    // 比较两种实现的结果
    bool fft_consistent = true;
    for (size_t i = 0; i < n; ++i) {
      if (std::abs(recursive_fft[i].real() - iterative_fft[i].real()) > 1e-6 ||
          std::abs(recursive_fft[i].imag() - iterative_fft[i].imag()) > 1e-6) {
        fft_consistent = false;
        break;
      }
    }

    std::cout << "递归FFT与迭代FFT一致性: "
              << (fft_consistent ? "通过" : "失败") << std::endl;

    // 测试逆FFT
    auto inverse_fft = PolynomialsAndFFT::iterative_fft(iterative_fft, true);

    bool fft_reversible = true;
    for (size_t i = 0; i < n; ++i) {
      if (std::abs(input[i].real() - inverse_fft[i].real()) > 1e-6 ||
          std::abs(input[i].imag() - inverse_fft[i].imag()) > 1e-6) {
        fft_reversible = false;
        break;
      }
    }

    std::cout << "FFT可逆性: " << (fft_reversible ? "通过" : "失败")
              << std::endl;
  }
}

/**
 * @brief 测试边界情况
 */
void test_edge_cases() {
  std::cout << "\n=== 边界情况测试 ===" << std::endl;

  // 测试1：零多项式
  std::cout << "\n测试1：零多项式乘法" << std::endl;
  PolynomialsAndFFT::Polynomial zero({0});
  PolynomialsAndFFT::Polynomial A({1, 2, 3});

  auto result1 = PolynomialsAndFFT::fft_polynomial_multiply(zero, A);
  std::cout << "0 * A(x) = " << result1.to_string() << std::endl;

  // 测试2：常数多项式
  std::cout << "\n测试2：常数多项式乘法" << std::endl;
  PolynomialsAndFFT::Polynomial constant({5});
  PolynomialsAndFFT::Polynomial B({1, 1});

  auto result2 = PolynomialsAndFFT::fft_polynomial_multiply(constant, B);
  std::cout << "5 * (1 + x) = " << result2.to_string() << std::endl;

  // 测试3：单次多项式
  std::cout << "\n测试3：单次多项式乘法" << std::endl;
  PolynomialsAndFFT::Polynomial linear({2, 3});       // 2 + 3x
  PolynomialsAndFFT::Polynomial quadratic({1, 4, 1}); // 1 + 4x + x²

  auto result3 = PolynomialsAndFFT::fft_polynomial_multiply(linear, quadratic);
  std::cout << "(2 + 3x) * (1 + 4x + x²) = " << result3.to_string()
            << std::endl;

  // 验证结果
  bool verified =
      PolynomialsAndFFT::verify_polynomial_multiply(linear, quadratic, result3);
  std::cout << "结果验证: " << (verified ? "通过" : "失败") << std::endl;
}

/**
 * @brief 比较朴素乘法和FFT乘法的性能
 */
void compare_multiplication_methods() {
  std::cout << "\n=== 多项式乘法方法性能比较 ===" << std::endl;

  // 测试不同规模的多项式
  std::vector<std::pair<size_t, std::string>> test_cases = {
      {7, "小规模 (7次)"},
      {15, "中等规模 (15次)"},
      {31, "较大规模 (31次)"},
      {63, "大规模 (63次)"}};

  for (const auto &test_case : test_cases) {
    size_t degree = test_case.first;
    std::string description = test_case.second;

    std::cout << "\n" << description << ":" << std::endl;

    auto A = PolynomialsAndFFT::generate_random_polynomial(degree);
    auto B = PolynomialsAndFFT::generate_random_polynomial(degree);

    // 朴素乘法
    performance_test("朴素乘法", [&]() {
      auto result = PolynomialsAndFFT::naive_polynomial_multiply(A, B);
    });

    // FFT乘法
    performance_test("FFT乘法", [&]() {
      auto result = PolynomialsAndFFT::fft_polynomial_multiply(A, B);
    });

    // 验证FFT乘法结果
    auto fft_result = PolynomialsAndFFT::fft_polynomial_multiply(A, B);
    bool verified =
        PolynomialsAndFFT::verify_polynomial_multiply(A, B, fft_result);
    std::cout << "  FFT结果验证: " << (verified ? "通过" : "失败") << std::endl;
  }
}

/**
 * @brief 测试复数运算和单位根
 */
void test_complex_operations() {
  std::cout << "\n=== 复数运算和单位根测试 ===" << std::endl;

  // 测试复数乘法
  PolynomialsAndFFT::Complex z1(3, 4);  // 3 + 4i
  PolynomialsAndFFT::Complex z2(1, -2); // 1 - 2i

  auto product = z1 * z2;
  std::cout << "(3 + 4i) * (1 - 2i) = " << product.real() << " + "
            << product.imag() << "i" << std::endl;

  // 测试单位根
  size_t n = 8;
  double angle = 2.0 * M_PI / n;
  PolynomialsAndFFT::Complex omega_n(std::cos(angle), std::sin(angle));

  std::cout << "\n8次单位根测试:" << std::endl;
  PolynomialsAndFFT::Complex omega(1, 0);
  for (size_t k = 0; k < n; ++k) {
    std::cout << "ω^" << k << " = " << std::fixed << std::setprecision(4)
              << omega.real() << " + " << omega.imag() << "i" << std::endl;
    omega *= omega_n;
  }

  // 验证单位根性质：ω^n = 1
  PolynomialsAndFFT::Complex omega_power(1, 0);
  for (size_t k = 0; k < n; ++k) {
    omega_power *= omega_n;
  }

  bool root_property = std::abs(omega_power.real() - 1.0) < 1e-10 &&
                       std::abs(omega_power.imag()) < 1e-10;
  std::cout << "单位根性质验证 (ω^8 = 1): " << (root_property ? "通过" : "失败")
            << std::endl;
}

int main() {
  std::cout << "=== 算法导论 第30章 - 多项式与快速傅里叶变换算法演示 ==="
            << std::endl;
  std::cout << std::endl;

  try {
    // 测试算法导论示例问题
    test_textbook_example();

    // 测试不同规模的多项式乘法
    test_different_sizes();

    // 测试FFT算法正确性
    test_fft_correctness();

    // 测试边界情况
    test_edge_cases();

    // 比较乘法方法性能
    compare_multiplication_methods();

    // 测试复数运算
    test_complex_operations();

    std::cout << "\n=== 测试完成 ===" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "测试过程中发生错误: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
