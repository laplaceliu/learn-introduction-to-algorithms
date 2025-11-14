#ifndef LINEAR_SYSTEMS_H
#define LINEAR_SYSTEMS_H

#include "matrix_operations.h"
#include <cmath>
#include <stdexcept>
#include <vector>

namespace algorithms {

/**
 * @brief 线性方程组求解器
 *
 * 算法导论第28.2节：线性方程组求解
 */
class LinearSystemSolver {
public:
  /**
   * @brief 使用高斯消元法求解线性方程组
   *
   * 时间复杂度：O(n³)
   *
   * @param A 系数矩阵
   * @param b 常数向量
   * @return std::vector<double> 解向量
   */
  static std::vector<double> gaussianElimination(const Matrix &A,
                                                 const std::vector<double> &b) {
    int n = A.getRows();
    if (n != A.getCols() || n != b.size())
      throw std::invalid_argument(
          "Matrix dimensions must match for Gaussian elimination");

    // 创建增广矩阵
    Matrix aug(n, n + 1);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        aug(i, j) = A(i, j);
      }
      aug(i, n) = b[i];
    }

    // 前向消元
    for (int i = 0; i < n; i++) {
      // 寻找主元
      int maxRow = i;
      for (int k = i + 1; k < n; k++) {
        if (std::abs(aug(k, i)) > std::abs(aug(maxRow, i))) {
          maxRow = k;
        }
      }

      // 交换行
      if (maxRow != i) {
        for (int j = i; j <= n; j++) {
          std::swap(aug(i, j), aug(maxRow, j));
        }
      }

      // 检查奇异矩阵
      if (std::abs(aug(i, i)) < 1e-10) {
        throw std::runtime_error("Matrix is singular or nearly singular");
      }

      // 消元
      for (int k = i + 1; k < n; k++) {
        double factor = aug(k, i) / aug(i, i);
        for (int j = i; j <= n; j++) {
          aug(k, j) -= factor * aug(i, j);
        }
      }
    }

    // 回代
    std::vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
      x[i] = aug(i, n);
      for (int j = i + 1; j < n; j++) {
        x[i] -= aug(i, j) * x[j];
      }
      x[i] /= aug(i, i);
    }

    return x;
  }

  /**
   * @brief 使用LU分解求解线性方程组
   *
   * 时间复杂度：O(n³)
   *
   * @param A 系数矩阵
   * @param b 常数向量
   * @return std::vector<double> 解向量
   */
  static std::vector<double> luDecomposition(const Matrix &A,
                                             const std::vector<double> &b) {
    int n = A.getRows();
    if (n != A.getCols() || n != b.size())
      throw std::invalid_argument(
          "Matrix dimensions must match for LU decomposition");

    // LU分解
    Matrix L(n, n);
    Matrix U(n, n);

    for (int i = 0; i < n; i++) {
      // 计算U的第i行
      for (int j = i; j < n; j++) {
        double sum = 0.0;
        for (int k = 0; k < i; k++) {
          sum += L(i, k) * U(k, j);
        }
        U(i, j) = A(i, j) - sum;
      }

      // 计算L的第i列
      for (int j = i; j < n; j++) {
        if (i == j) {
          L(i, i) = 1.0;
        } else {
          double sum = 0.0;
          for (int k = 0; k < i; k++) {
            sum += L(j, k) * U(k, i);
          }
          L(j, i) = (A(j, i) - sum) / U(i, i);
        }
      }
    }

    // 前向替换：解 Ly = b
    std::vector<double> y(n, 0.0);
    for (int i = 0; i < n; i++) {
      y[i] = b[i];
      for (int j = 0; j < i; j++) {
        y[i] -= L(i, j) * y[j];
      }
      y[i] /= L(i, i);
    }

    // 回代：解 Ux = y
    std::vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
      x[i] = y[i];
      for (int j = i + 1; j < n; j++) {
        x[i] -= U(i, j) * x[j];
      }
      x[i] /= U(i, i);
    }

    return x;
  }

  /**
   * @brief 使用雅可比迭代法求解线性方程组
   *
   * 迭代方法，适用于对角占优矩阵
   *
   * @param A 系数矩阵
   * @param b 常数向量
   * @param max_iterations 最大迭代次数
   * @param tolerance 容差
   * @return std::vector<double> 解向量
   */
  static std::vector<double> jacobiIteration(const Matrix &A,
                                             const std::vector<double> &b,
                                             int max_iterations = 1000,
                                             double tolerance = 1e-10) {
    int n = A.getRows();
    if (n != A.getCols() || n != b.size())
      throw std::invalid_argument(
          "Matrix dimensions must match for Jacobi iteration");

    std::vector<double> x(n, 0.0);
    std::vector<double> x_new(n, 0.0);

    for (int iter = 0; iter < max_iterations; iter++) {
      double max_error = 0.0;

      for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
          if (j != i) {
            sum += A(i, j) * x[j];
          }
        }
        x_new[i] = (b[i] - sum) / A(i, i);

        double error = std::abs(x_new[i] - x[i]);
        if (error > max_error) {
          max_error = error;
        }
      }

      x = x_new;

      if (max_error < tolerance) {
        break;
      }
    }

    return x;
  }

  /**
   * @brief 检查矩阵是否对角占优
   *
   * @param A 矩阵
   * @return bool 是否对角占优
   */
  static bool isDiagonallyDominant(const Matrix &A) {
    int n = A.getRows();
    if (n != A.getCols())
      return false;

    for (int i = 0; i < n; i++) {
      double diagonal = std::abs(A(i, i));
      double sum = 0.0;

      for (int j = 0; j < n; j++) {
        if (j != i) {
          sum += std::abs(A(i, j));
        }
      }

      if (diagonal <= sum) {
        return false;
      }
    }

    return true;
  }

  /**
   * @brief 计算残差
   *
   * @param A 系数矩阵
   * @param b 常数向量
   * @param x 解向量
   * @return double 残差范数
   */
  static double residual(const Matrix &A, const std::vector<double> &b,
                         const std::vector<double> &x) {
    int n = A.getRows();
    double residual_norm = 0.0;

    for (int i = 0; i < n; i++) {
      double sum = 0.0;
      for (int j = 0; j < n; j++) {
        sum += A(i, j) * x[j];
      }
      residual_norm += (sum - b[i]) * (sum - b[i]);
    }

    return std::sqrt(residual_norm);
  }
};

} // namespace algorithms

#endif // LINEAR_SYSTEMS_H
