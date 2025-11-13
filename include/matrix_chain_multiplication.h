#ifndef MATRIX_CHAIN_MULTIPLICATION_H
#define MATRIX_CHAIN_MULTIPLICATION_H

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <vector>

namespace algorithms {

class MatrixChainMultiplication {
private:
  std::vector<int>
      dimensions; // 矩阵维度序列：A1是p0×p1，A2是p1×p2，...，An是p_{n-1}×p_n
  std::vector<std::vector<int>>
      m; // m[i][j]：计算矩阵链A_i...A_j的最小标量乘法次数
  std::vector<std::vector<int>> s; // s[i][j]：最优括号化方案的分割位置

  // 初始化DP表
  void initialize_tables(int n) {
    m.resize(n + 1, std::vector<int>(n + 1, 0));
    s.resize(n + 1, std::vector<int>(n + 1, 0));

    // 对角线元素（单个矩阵）的乘法次数为0
    for (int i = 1; i <= n; i++) {
      m[i][i] = 0;
    }
  }

  // 计算最优括号化方案
  void compute_optimal_parenthesization(int n) {
    for (int l = 2; l <= n; l++) { // l是链的长度
      for (int i = 1; i <= n - l + 1; i++) {
        int j = i + l - 1;
        m[i][j] = INT_MAX;

        for (int k = i; k <= j - 1; k++) {
          int cost = m[i][k] + m[k + 1][j] +
                     dimensions[i - 1] * dimensions[k] * dimensions[j];

          if (cost < m[i][j]) {
            m[i][j] = cost;
            s[i][j] = k;
          }
        }
      }
    }
  }

  // 递归构建括号化字符串
  std::string construct_parenthesization(int i, int j) {
    if (i == j) {
      return "A" + std::to_string(i);
    } else {
      return "(" + construct_parenthesization(i, s[i][j]) +
             construct_parenthesization(s[i][j] + 1, j) + ")";
    }
  }

public:
  MatrixChainMultiplication(const std::vector<int> &dims) : dimensions(dims) {
    if (dimensions.size() < 2) {
      throw std::invalid_argument("至少需要2个维度值");
    }

    int n = dimensions.size() - 1; // 矩阵数量
    initialize_tables(n);
    compute_optimal_parenthesization(n);
  }

  // 获取最小标量乘法次数
  int get_minimum_scalar_multiplications() const {
    int n = dimensions.size() - 1;
    return m[1][n];
  }

  // 获取最优括号化方案
  std::string get_optimal_parenthesization() {
    int n = dimensions.size() - 1;
    return construct_parenthesization(1, n);
  }

  // 打印DP表（用于调试和分析）
  void print_dp_tables() const {
    int n = dimensions.size() - 1;

    std::cout << "维度序列: ";
    for (int i = 0; i < dimensions.size(); i++) {
      std::cout << dimensions[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "\n最小乘法次数表 m[i][j]:" << std::endl;
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        if (i <= j) {
          std::cout << m[i][j] << "\t";
        } else {
          std::cout << "\t";
        }
      }
      std::cout << std::endl;
    }

    std::cout << "\n分割位置表 s[i][j]:" << std::endl;
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        if (i < j) {
          std::cout << s[i][j] << "\t";
        } else {
          std::cout << "\t";
        }
      }
      std::cout << std::endl;
    }
  }

  // 获取特定子链的最小乘法次数
  int get_chain_cost(int i, int j) const {
    int n = dimensions.size() - 1;
    if (i < 1 || j > n || i > j) {
      throw std::out_of_range("无效的子链范围");
    }
    return m[i][j];
  }

  // 获取特定子链的最优分割位置
  int get_split_position(int i, int j) const {
    int n = dimensions.size() - 1;
    if (i < 1 || j > n || i >= j) {
      throw std::out_of_range("无效的子链范围");
    }
    return s[i][j];
  }

  // 获取矩阵数量
  int get_matrix_count() const { return dimensions.size() - 1; }

  // 获取维度序列
  std::vector<int> get_dimensions() const { return dimensions; }
};

// 矩阵链乘法的备忘录版本（自顶向下）
class MatrixChainMemoized {
private:
  std::vector<int> dimensions;
  std::vector<std::vector<int>> m;
  std::vector<std::vector<int>> s;

  int lookup_chain(int i, int j) {
    if (m[i][j] < INT_MAX) {
      return m[i][j];
    }

    if (i == j) {
      m[i][j] = 0;
    } else {
      for (int k = i; k <= j - 1; k++) {
        int cost = lookup_chain(i, k) + lookup_chain(k + 1, j) +
                   dimensions[i - 1] * dimensions[k] * dimensions[j];

        if (cost < m[i][j]) {
          m[i][j] = cost;
          s[i][j] = k;
        }
      }
    }

    return m[i][j];
  }

  std::string construct_parenthesization(int i, int j) {
    if (i == j) {
      return "A" + std::to_string(i);
    } else {
      return "(" + construct_parenthesization(i, s[i][j]) +
             construct_parenthesization(s[i][j] + 1, j) + ")";
    }
  }

public:
  MatrixChainMemoized(const std::vector<int> &dims) : dimensions(dims) {
    if (dimensions.size() < 2) {
      throw std::invalid_argument("至少需要2个维度值");
    }

    int n = dimensions.size() - 1;
    m.resize(n + 1, std::vector<int>(n + 1, INT_MAX));
    s.resize(n + 1, std::vector<int>(n + 1, 0));

    lookup_chain(1, n);
  }

  int get_minimum_scalar_multiplications() const {
    int n = dimensions.size() - 1;
    return m[1][n];
  }

  std::string get_optimal_parenthesization() {
    int n = dimensions.size() - 1;
    return construct_parenthesization(1, n);
  }

  void print_dp_tables() const {
    int n = dimensions.size() - 1;

    std::cout << "维度序列: ";
    for (int i = 0; i < dimensions.size(); i++) {
      std::cout << dimensions[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "\n最小乘法次数表 m[i][j]:" << std::endl;
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        if (i <= j) {
          std::cout << m[i][j] << "\t";
        } else {
          std::cout << "\t";
        }
      }
      std::cout << std::endl;
    }
  }
};

} // namespace algorithms

#endif // MATRIX_CHAIN_MULTIPLICATION_H
