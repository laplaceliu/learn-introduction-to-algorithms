#ifndef MATRIX_INVERSION_H
#define MATRIX_INVERSION_H

#include "linear_systems.h"
#include "matrix_operations.h"
#include <stdexcept>
#include <vector>

namespace algorithms {

/**
 * @brief 矩阵求逆器
 *
 * 算法导论第28.3节：矩阵求逆
 */
class MatrixInverter {
public:
  /**
   * @brief 使用高斯-约当消元法求矩阵逆
   *
   * 时间复杂度：O(n³)
   *
   * @param A 输入矩阵
   * @return Matrix 逆矩阵
   */
  static Matrix gaussJordanInversion(const Matrix &A) {
    int n = A.getRows();
    if (n != A.getCols())
      throw std::invalid_argument("Matrix must be square for inversion");

    // 创建增广矩阵 [A | I]
    Matrix aug(n, 2 * n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        aug(i, j) = A(i, j);
      }
      aug(i, i + n) = 1.0; // 单位矩阵
    }

    // 高斯-约当消元
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
        for (int j = 0; j < 2 * n; j++) {
          std::swap(aug(i, j), aug(maxRow, j));
        }
      }

      // 检查奇异矩阵
      if (std::abs(aug(i, i)) < 1e-10) {
        throw std::runtime_error("Matrix is singular and cannot be inverted");
      }

      // 归一化主元行
      double pivot = aug(i, i);
      for (int j = 0; j < 2 * n; j++) {
        aug(i, j) /= pivot;
      }

      // 消元
      for (int k = 0; k < n; k++) {
        if (k != i) {
          double factor = aug(k, i);
          for (int j = 0; j < 2 * n; j++) {
            aug(k, j) -= factor * aug(i, j);
          }
        }
      }
    }

    // 提取逆矩阵
    Matrix inv(n, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        inv(i, j) = aug(i, j + n);
      }
    }

    return inv;
  }

  /**
   * @brief 使用LU分解求矩阵逆
   *
   * 时间复杂度：O(n³)
   *
   * @param A 输入矩阵
   * @return Matrix 逆矩阵
   */
  static Matrix luInversion(const Matrix &A) {
    int n = A.getRows();
    if (n != A.getCols())
      throw std::invalid_argument("Matrix must be square for inversion");

    // 对每个单位向量求解线性方程组
    Matrix inv(n, n);

    for (int j = 0; j < n; j++) {
      // 创建单位向量
      std::vector<double> b(n, 0.0);
      b[j] = 1.0;

      // 使用LU分解求解
      std::vector<double> x = LinearSystemSolver::luDecomposition(A, b);

      // 将解存入逆矩阵的第j列
      for (int i = 0; i < n; i++) {
        inv(i, j) = x[i];
      }
    }

    return inv;
  }

  /**
   * @brief 使用伴随矩阵法求矩阵逆
   *
   * 时间复杂度：O(n!)，仅适用于小矩阵
   *
   * @param A 输入矩阵
   * @return Matrix 逆矩阵
   */
  static Matrix adjugateInversion(const Matrix &A) {
    int n = A.getRows();
    if (n != A.getCols())
      throw std::invalid_argument("Matrix must be square for inversion");

    // 计算行列式
    double det = MatrixUtils::determinant(A);
    if (std::abs(det) < 1e-10) {
      throw std::runtime_error("Matrix is singular and cannot be inverted");
    }

    // 计算伴随矩阵
    Matrix adj(n, n);

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        // 计算代数余子式
        Matrix minor(n - 1, n - 1);
        for (int i1 = 0, i2 = 0; i1 < n; i1++) {
          if (i1 == i)
            continue;
          for (int j1 = 0, j2 = 0; j1 < n; j1++) {
            if (j1 == j)
              continue;
            minor(i2, j2) = A(i1, j1);
            j2++;
          }
          i2++;
        }

        double cofactor = MatrixUtils::determinant(minor);
        if ((i + j) % 2 == 1) {
          cofactor = -cofactor;
        }

        adj(j, i) = cofactor; // 转置
      }
    }

    // 计算逆矩阵：A⁻¹ = adj(A) / det(A)
    Matrix inv = adj * (1.0 / det);

    return inv;
  }

  /**
   * @brief 检查矩阵是否可逆
   *
   * @param A 矩阵
   * @return bool 是否可逆
   */
  static bool isInvertible(const Matrix &A) {
    if (A.getRows() != A.getCols())
      return false;

    try {
      double det = MatrixUtils::determinant(A);
      return std::abs(det) > 1e-10;
    } catch (...) {
      return false;
    }
  }

  /**
   * @brief 验证逆矩阵的正确性
   *
   * @param A 原矩阵
   * @param A_inv 逆矩阵
   * @param tolerance 容差
   * @return bool 是否正确
   */
  static bool verifyInversion(const Matrix &A, const Matrix &A_inv,
                              double tolerance = 1e-6) {
    int n = A.getRows();

    // 检查 A * A_inv ≈ I
    Matrix product = A * A_inv;
    Matrix identity = Matrix::identity(n);

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (std::abs(product(i, j) - identity(i, j)) > tolerance) {
          return false;
        }
      }
    }

    // 检查 A_inv * A ≈ I
    product = A_inv * A;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (std::abs(product(i, j) - identity(i, j)) > tolerance) {
          return false;
        }
      }
    }

    return true;
  }

  /**
   * @brief 计算矩阵的条件数
   *
   * 条件数 = ||A|| * ||A⁻¹||
   *
   * @param A 矩阵
   * @return double 条件数
   */
  static double conditionNumber(const Matrix &A) {
    if (!isInvertible(A)) {
      throw std::runtime_error("Matrix is not invertible");
    }

    Matrix A_inv = gaussJordanInversion(A);
    double norm_A = MatrixUtils::norm(A);
    double norm_A_inv = MatrixUtils::norm(A_inv);

    return norm_A * norm_A_inv;
  }
};

} // namespace algorithms

#endif // MATRIX_INVERSION_H
