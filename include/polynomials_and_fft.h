#ifndef POLYNOMIALS_AND_FFT_H
#define POLYNOMIALS_AND_FFT_H

#include <algorithm>
#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace algorithms {

/**
 * @brief 多项式与快速傅里叶变换算法实现
 *
 * 实现算法导论第30章多项式与快速傅里叶变换，包括：
 * - 多项式表示和基本运算
 * - 快速傅里叶变换（FFT）
 * - 多项式乘法的FFT算法
 * - 逆FFT算法
 */
class PolynomialsAndFFT {
public:
  /**
   * @brief 复数类型定义
   */
  using Complex = std::complex<double>;

  /**
   * @brief 多项式类
   */
  class Polynomial {
  private:
    std::vector<double> coefficients; // 系数向量，coefficients[i] 是 x^i 的系数

  public:
    /**
     * @brief 默认构造函数
     */
    Polynomial() = default;

    /**
     * @brief 从系数向量构造多项式
     * @param coeffs 系数向量
     */
    Polynomial(const std::vector<double> &coeffs) : coefficients(coeffs) {
      // 去除尾部的零系数
      while (coefficients.size() > 1 && coefficients.back() == 0.0) {
        coefficients.pop_back();
      }
    }

    /**
     * @brief 获取多项式次数
     * @return 多项式次数
     */
    size_t degree() const {
      return coefficients.empty() ? 0 : coefficients.size() - 1;
    }

    /**
     * @brief 获取系数
     * @param i 指数
     * @return x^i 的系数
     */
    double operator[](size_t i) const {
      return i < coefficients.size() ? coefficients[i] : 0.0;
    }

    /**
     * @brief 设置系数
     * @param i 指数
     * @param value 系数值
     */
    void set_coefficient(size_t i, double value) {
      if (i >= coefficients.size()) {
        coefficients.resize(i + 1, 0.0);
      }
      coefficients[i] = value;
    }

    /**
     * @brief 多项式加法
     * @param other 另一个多项式
     * @return 和多项式
     */
    Polynomial operator+(const Polynomial &other) const {
      size_t max_degree = std::max(degree(), other.degree());
      std::vector<double> result(max_degree + 1, 0.0);

      for (size_t i = 0; i <= max_degree; ++i) {
        result[i] = (*this)[i] + other[i];
      }

      return Polynomial(result);
    }

    /**
     * @brief 多项式减法
     * @param other 另一个多项式
     * @return 差多项式
     */
    Polynomial operator-(const Polynomial &other) const {
      size_t max_degree = std::max(degree(), other.degree());
      std::vector<double> result(max_degree + 1, 0.0);

      for (size_t i = 0; i <= max_degree; ++i) {
        result[i] = (*this)[i] - other[i];
      }

      return Polynomial(result);
    }

    /**
     * @brief 多项式标量乘法
     * @param scalar 标量
     * @return 标量乘后的多项式
     */
    Polynomial operator*(double scalar) const {
      std::vector<double> result = coefficients;
      for (double &coeff : result) {
        coeff *= scalar;
      }
      return Polynomial(result);
    }

    /**
     * @brief 多项式求值
     * @param x 自变量值
     * @return 多项式在x处的值
     */
    double evaluate(double x) const {
      double result = 0.0;
      double power = 1.0;

      for (double coeff : coefficients) {
        result += coeff * power;
        power *= x;
      }

      return result;
    }

    /**
     * @brief 转换为字符串表示
     * @return 多项式字符串
     */
    std::string to_string() const {
      if (coefficients.empty()) {
        return "0";
      }

      std::ostringstream oss;
      bool first_term = true;

      for (int i = coefficients.size() - 1; i >= 0; --i) {
        if (coefficients[i] != 0.0) {
          if (!first_term) {
            oss << (coefficients[i] > 0 ? " + " : " - ");
          } else if (coefficients[i] < 0) {
            oss << "-";
          }

          double abs_coeff = std::abs(coefficients[i]);

          if (i == 0) {
            oss << abs_coeff;
          } else if (i == 1) {
            if (abs_coeff != 1.0) {
              oss << abs_coeff << "*x";
            } else {
              oss << "x";
            }
          } else {
            if (abs_coeff != 1.0) {
              oss << abs_coeff << "*x^" << i;
            } else {
              oss << "x^" << i;
            }
          }

          first_term = false;
        }
      }

      return oss.str();
    }

    /**
     * @brief 获取系数向量
     * @return 系数向量
     */
    const std::vector<double> &get_coefficients() const { return coefficients; }
  };

  /**
   * @brief 朴素多项式乘法 - 算法导论第30.1节
   *
   * 时间复杂度 O(n²) 的朴素多项式乘法
   *
   * @param A 第一个多项式
   * @param B 第二个多项式
   * @return 乘积多项式
   */
  static Polynomial naive_polynomial_multiply(const Polynomial &A,
                                              const Polynomial &B) {
    size_t n = A.degree() + B.degree() + 1;
    std::vector<double> result(n, 0.0);

    for (size_t i = 0; i <= A.degree(); ++i) {
      for (size_t j = 0; j <= B.degree(); ++j) {
        result[i + j] += A[i] * B[j];
      }
    }

    return Polynomial(result);
  }

  /**
   * @brief 快速傅里叶变换 - 算法导论第30.2节
   *
   * 递归实现的FFT算法
   *
   * @param a 输入序列（复数）
   * @param inverse 是否为逆变换
   * @return FFT结果
   */
  static std::vector<Complex> fft(const std::vector<Complex> &a,
                                  bool inverse = false) {
    size_t n = a.size();

    // 基本情况
    if (n == 1) {
      return a;
    }

    // 检查n是否为2的幂
    if ((n & (n - 1)) != 0) {
      throw std::invalid_argument("FFT输入长度必须是2的幂");
    }

    // 计算主n次单位根
    double angle = 2.0 * M_PI / n * (inverse ? -1 : 1);
    Complex omega_n(std::cos(angle), std::sin(angle));
    Complex omega(1.0, 0.0);

    // 分割输入为偶数和奇数索引部分
    std::vector<Complex> a_even(n / 2);
    std::vector<Complex> a_odd(n / 2);

    for (size_t i = 0; i < n / 2; ++i) {
      a_even[i] = a[2 * i];
      a_odd[i] = a[2 * i + 1];
    }

    // 递归计算FFT
    std::vector<Complex> y_even = fft(a_even, inverse);
    std::vector<Complex> y_odd = fft(a_odd, inverse);

    // 合并结果
    std::vector<Complex> y(n);
    for (size_t k = 0; k < n / 2; ++k) {
      Complex t = omega * y_odd[k];
      y[k] = y_even[k] + t;
      y[k + n / 2] = y_even[k] - t;

      if (inverse) {
        // 逆变换需要除以2
        y[k] /= 2.0;
        y[k + n / 2] /= 2.0;
      }

      omega *= omega_n;
    }

    return y;
  }

  /**
   * @brief 迭代快速傅里叶变换
   *
   * 非递归实现的FFT，性能更好
   *
   * @param a 输入序列
   * @param inverse 是否为逆变换
   * @return FFT结果
   */
  static std::vector<Complex> iterative_fft(const std::vector<Complex> &a,
                                            bool inverse = false) {
    size_t n = a.size();

    // 检查n是否为2的幂
    if ((n & (n - 1)) != 0) {
      throw std::invalid_argument("FFT输入长度必须是2的幂");
    }

    // 位反转置换
    std::vector<Complex> A = a;
    for (size_t i = 1, j = 0; i < n; ++i) {
      size_t bit = n >> 1;
      while (j >= bit) {
        j -= bit;
        bit >>= 1;
      }
      j += bit;
      if (i < j) {
        std::swap(A[i], A[j]);
      }
    }

    // 迭代计算FFT
    for (size_t len = 2; len <= n; len <<= 1) {
      double angle = 2.0 * M_PI / len * (inverse ? -1 : 1);
      Complex omega_len(std::cos(angle), std::sin(angle));

      for (size_t i = 0; i < n; i += len) {
        Complex omega(1.0, 0.0);
        for (size_t j = 0; j < len / 2; ++j) {
          Complex u = A[i + j];
          Complex v = omega * A[i + j + len / 2];

          A[i + j] = u + v;
          A[i + j + len / 2] = u - v;

          omega *= omega_len;
        }
      }
    }

    if (inverse) {
      for (size_t i = 0; i < n; ++i) {
        A[i] /= n;
      }
    }

    return A;
  }

  /**
   * @brief 使用FFT的多项式乘法 - 算法导论第30.2节
   *
   * 时间复杂度 O(n log n) 的多项式乘法
   *
   * @param A 第一个多项式
   * @param B 第二个多项式
   * @return 乘积多项式
   */
  static Polynomial fft_polynomial_multiply(const Polynomial &A,
                                            const Polynomial &B) {
    size_t degree_A = A.degree();
    size_t degree_B = B.degree();
    size_t product_degree = degree_A + degree_B;

    // 找到大于等于 product_degree + 1 的最小的2的幂
    size_t n = 1;
    while (n <= product_degree) {
      n <<= 1;
    }

    // 将多项式系数扩展到长度为n的复数向量
    std::vector<Complex> a(n, Complex(0.0, 0.0));
    std::vector<Complex> b(n, Complex(0.0, 0.0));

    for (size_t i = 0; i <= degree_A; ++i) {
      a[i] = Complex(A[i], 0.0);
    }

    for (size_t i = 0; i <= degree_B; ++i) {
      b[i] = Complex(B[i], 0.0);
    }

    // 计算FFT
    std::vector<Complex> A_fft = iterative_fft(a);
    std::vector<Complex> B_fft = iterative_fft(b);

    // 点值相乘
    std::vector<Complex> C_fft(n);
    for (size_t i = 0; i < n; ++i) {
      C_fft[i] = A_fft[i] * B_fft[i];
    }

    // 计算逆FFT
    std::vector<Complex> c = iterative_fft(C_fft, true);

    // 提取实数部分作为多项式系数
    std::vector<double> result_coeffs(product_degree + 1);
    for (size_t i = 0; i <= product_degree; ++i) {
      result_coeffs[i] = std::round(c[i].real()); // 四舍五入处理浮点误差
    }

    return Polynomial(result_coeffs);
  }

  /**
   * @brief 生成随机多项式
   *
   * 用于测试和演示
   *
   * @param degree 多项式次数
   * @param min_coeff 最小系数值
   * @param max_coeff 最大系数值
   * @return 随机多项式
   */
  static Polynomial generate_random_polynomial(size_t degree,
                                               double min_coeff = -10.0,
                                               double max_coeff = 10.0) {
    std::vector<double> coeffs(degree + 1);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min_coeff, max_coeff);

    for (size_t i = 0; i <= degree; ++i) {
      coeffs[i] = dis(gen);
    }

    return Polynomial(coeffs);
  }

  /**
   * @brief 验证多项式乘法结果
   *
   * 通过随机点求值验证乘法结果的正确性
   *
   * @param A 第一个多项式
   * @param B 第二个多项式
   * @param product 乘积多项式
   * @param num_test_points 测试点数量
   * @return 验证结果
   */
  static bool verify_polynomial_multiply(const Polynomial &A,
                                         const Polynomial &B,
                                         const Polynomial &product,
                                         int num_test_points = 10) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-10.0, 10.0);

    for (int i = 0; i < num_test_points; ++i) {
      double x = dis(gen);
      double expected = A.evaluate(x) * B.evaluate(x);
      double actual = product.evaluate(x);

      if (std::abs(expected - actual) > 1e-6) {
        return false;
      }
    }

    return true;
  }
};

} // namespace algorithms

#endif // POLYNOMIALS_AND_FFT_H
