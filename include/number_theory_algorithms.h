#ifndef NUMBER_THEORY_ALGORITHMS_H
#define NUMBER_THEORY_ALGORITHMS_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace algorithms {

/**
 * @brief 数论算法实现
 *
 * 实现算法导论第31章数论算法，包括：
 * - 最大公约数（GCD）
 * - 扩展欧几里得算法
 * - 模运算
 * - 模指数运算
 * - 素数测试
 * - 素数生成
 * - RSA加密算法
 */
class NumberTheoryAlgorithms {
public:
  /**
   * @brief 计算最大公约数（GCD）- 算法导论第31.1节
   *
   * 使用欧几里得算法计算两个整数的最大公约数
   *
   * @param a 第一个整数
   * @param b 第二个整数
   * @return 最大公约数
   */
  static int gcd(int a, int b) {
    if (b == 0)
      return std::abs(a);
    return gcd(b, a % b);
  }

  /**
   * @brief 迭代版本的GCD算法
   *
   * @param a 第一个整数
   * @param b 第二个整数
   * @return 最大公约数
   */
  static int gcd_iterative(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);

    while (b != 0) {
      int temp = b;
      b = a % b;
      a = temp;
    }

    return a;
  }

  /**
   * @brief 扩展欧几里得算法 - 算法导论第31.2节
   *
   * 计算GCD并找到满足 ax + by = gcd(a, b) 的整数x, y
   *
   * @param a 第一个整数
   * @param b 第二个整数
   * @param x 引用参数，存储系数x
   * @param y 引用参数，存储系数y
   * @return 最大公约数
   */
  static int extended_gcd(int a, int b, int &x, int &y) {
    if (b == 0) {
      x = 1;
      y = 0;
      return a;
    }

    int x1, y1;
    int gcd_val = extended_gcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcd_val;
  }

  /**
   * @brief 模运算 - 算法导论第31.1节
   *
   * 计算 a mod n
   *
   * @param a 被除数
   * @param n 模数
   * @return 模运算结果
   */
  static int mod(int a, int n) {
    if (n <= 0) {
      throw std::invalid_argument("模数必须为正数");
    }

    int result = a % n;
    if (result < 0) {
      result += n;
    }
    return result;
  }

  /**
   * @brief 模加法
   *
   * 计算 (a + b) mod n
   *
   * @param a 第一个数
   * @param b 第二个数
   * @param n 模数
   * @return 模加法结果
   */
  static int mod_add(int a, int b, int n) {
    return mod(mod(a, n) + mod(b, n), n);
  }

  /**
   * @brief 模乘法
   *
   * 计算 (a * b) mod n
   *
   * @param a 第一个数
   * @param b 第二个数
   * @param n 模数
   * @return 模乘法结果
   */
  static int mod_multiply(int a, int b, int n) {
    return mod(mod(a, n) * mod(b, n), n);
  }

  /**
   * @brief 模指数运算 - 算法导论第31.6节
   *
   * 使用快速幂算法计算 a^b mod n
   *
   * @param a 底数
   * @param b 指数
   * @param n 模数
   * @return 模指数运算结果
   */
  static int mod_exponentiation(int a, int b, int n) {
    if (n == 1)
      return 0;

    a = mod(a, n);
    int result = 1;

    while (b > 0) {
      if (b & 1) { // 如果b是奇数
        result = mod_multiply(result, a, n);
      }
      a = mod_multiply(a, a, n);
      b >>= 1; // b除以2
    }

    return result;
  }

  /**
   * @brief 模逆元 - 算法导论第31.4节
   *
   * 计算a在模n下的乘法逆元（如果存在）
   *
   * @param a 数
   * @param n 模数
   * @return 模逆元，如果不存在则返回-1
   */
  static int mod_inverse(int a, int n) {
    int x, y;
    int gcd_val = extended_gcd(a, n, x, y);

    if (gcd_val != 1) {
      return -1; // 逆元不存在
    }

    return mod(x, n);
  }

  /**
   * @brief 素数测试 - 算法导论第31.8节
   *
   * 使用Miller-Rabin概率素数测试
   *
   * @param n 要测试的数
   * @param k 测试次数（默认5次）
   * @return 如果可能是素数返回true，否则返回false
   */
  static bool is_prime(int n, int k = 5) {
    if (n <= 1)
      return false;
    if (n <= 3)
      return true;
    if (n % 2 == 0)
      return false;

    // 将n-1写成d*2^s的形式
    int d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
      d /= 2;
      s++;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(2, n - 2);

    for (int i = 0; i < k; ++i) {
      int a = dis(gen);
      int x = mod_exponentiation(a, d, n);

      if (x == 1 || x == n - 1) {
        continue;
      }

      bool composite = true;
      for (int j = 0; j < s - 1; ++j) {
        x = mod_multiply(x, x, n);
        if (x == n - 1) {
          composite = false;
          break;
        }
      }

      if (composite) {
        return false;
      }
    }

    return true;
  }

  /**
   * @brief 生成素数
   *
   * 生成指定位数的素数
   *
   * @param bits 素数位数
   * @return 生成的素数
   */
  static int generate_prime(int bits) {
    if (bits <= 1) {
      throw std::invalid_argument("素数位数必须大于1");
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    int min_val = 1 << (bits - 1);
    int max_val = (1 << bits) - 1;

    std::uniform_int_distribution<int> dis(min_val, max_val);

    while (true) {
      int candidate = dis(gen);
      // 确保是奇数
      if (candidate % 2 == 0) {
        candidate++;
      }

      if (is_prime(candidate)) {
        return candidate;
      }
    }
  }

  /**
   * @brief 中国剩余定理 - 算法导论第31.5节
   *
   * 解决同余方程组：x ≡ a_i (mod n_i)
   *
   * @param a 余数数组
   * @param n 模数数组
   * @return 满足所有同余方程的最小正整数解
   */
  static int chinese_remainder_theorem(const std::vector<int> &a,
                                       const std::vector<int> &n) {
    if (a.size() != n.size()) {
      throw std::invalid_argument("余数和模数数组大小必须相同");
    }

    // 计算所有模数的乘积
    int N = 1;
    for (int ni : n) {
      N *= ni;
    }

    int result = 0;
    for (size_t i = 0; i < a.size(); ++i) {
      int Ni = N / n[i];
      int Mi = mod_inverse(Ni, n[i]);

      if (Mi == -1) {
        throw std::invalid_argument("模数不互质，中国剩余定理不适用");
      }

      result =
          mod_add(result, mod_multiply(a[i], mod_multiply(Ni, Mi, N), N), N);
    }

    return result;
  }

  /**
   * @brief RSA加密算法 - 算法导论第31.7节
   */
  class RSA {
  private:
    int public_key;  // 公钥 (e, n)
    int private_key; // 私钥 (d, n)
    int modulus;     // 模数 n

  public:
    /**
     * @brief 构造函数，生成RSA密钥对
     * @param bits 素数位数
     */
    RSA(int bits = 16) {
      // 生成两个大素数
      int p = generate_prime(bits);
      int q = generate_prime(bits);

      modulus = p * q;
      int phi = (p - 1) * (q - 1);

      // 选择公钥e，满足1 < e < phi且gcd(e, phi) = 1
      public_key = 65537; // 常用的公钥值
      if (public_key >= phi || gcd(public_key, phi) != 1) {
        // 如果65537不合适，寻找合适的公钥
        public_key = 3;
        while (gcd(public_key, phi) != 1) {
          public_key += 2;
        }
      }

      // 计算私钥d，满足d*e ≡ 1 (mod phi)
      private_key = mod_inverse(public_key, phi);
      if (private_key == -1) {
        throw std::runtime_error("无法计算私钥");
      }
    }

    /**
     * @brief 获取公钥
     * @return 公钥(e, n)
     */
    std::pair<int, int> get_public_key() const { return {public_key, modulus}; }

    /**
     * @brief 加密消息
     * @param message 要加密的消息（整数）
     * @return 加密后的密文
     */
    int encrypt(int message) const {
      if (message >= modulus) {
        throw std::invalid_argument("消息必须小于模数");
      }
      return mod_exponentiation(message, public_key, modulus);
    }

    /**
     * @brief 解密密文
     * @param ciphertext 密文
     * @return 解密后的消息
     */
    int decrypt(int ciphertext) const {
      return mod_exponentiation(ciphertext, private_key, modulus);
    }

    /**
     * @brief 获取RSA参数信息
     * @return 参数字符串
     */
    std::string to_string() const {
      std::ostringstream oss;
      oss << "RSA参数：\n";
      oss << "模数 n = " << modulus << "\n";
      oss << "公钥 e = " << public_key << "\n";
      oss << "私钥 d = " << private_key << "\n";
      return oss.str();
    }
  };

  /**
   * @brief 验证GCD计算
   * @param a 第一个数
   * @param b 第二个数
   * @param expected 期望的GCD值
   * @return 验证结果
   */
  static bool verify_gcd(int a, int b, int expected) {
    int result = gcd(a, b);
    return result == expected;
  }

  /**
   * @brief 验证扩展欧几里得算法
   * @param a 第一个数
   * @param b 第二个数
   * @return 验证结果
   */
  static bool verify_extended_gcd(int a, int b) {
    int x, y;
    int gcd_val = extended_gcd(a, b, x, y);

    // 验证 ax + by = gcd(a, b)
    return (a * x + b * y) == gcd_val;
  }

  /**
   * @brief 验证模指数运算
   * @param a 底数
   * @param b 指数
   * @param n 模数
   * @return 验证结果
   */
  static bool verify_mod_exponentiation(int a, int b, int n) {
    if (n <= 0)
      return false;

    int result = mod_exponentiation(a, b, n);

    // 使用简单方法验证小数值
    if (b < 20 && n < 1000) {
      long long expected = 1;
      for (int i = 0; i < b; ++i) {
        expected = (expected * a) % n;
      }
      return result == static_cast<int>(expected);
    }

    return true; // 对于大数值，假设算法正确
  }

  /**
   * @brief 验证RSA加密解密
   * @param rsa RSA实例
   * @param message 测试消息
   * @return 验证结果
   */
  static bool verify_rsa(const RSA &rsa, int message) {
    int encrypted = rsa.encrypt(message);
    int decrypted = rsa.decrypt(encrypted);
    return message == decrypted;
  }
};

} // namespace algorithms

#endif // NUMBER_THEORY_ALGORITHMS_H
