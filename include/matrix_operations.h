#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

namespace algorithms {

/**
 * @brief 矩阵类
 *
 * 算法导论第28.1节：矩阵运算基础
 */
class Matrix {
private:
  std::vector<std::vector<double>> data;
  int rows;
  int cols;

public:
  // 构造函数
  Matrix(int r, int c) : rows(r), cols(c) {
    data.resize(rows, std::vector<double>(cols, 0.0));
  }

  Matrix(const std::vector<std::vector<double>> &input) : data(input) {
    rows = input.size();
    if (rows > 0)
      cols = input[0].size();
    else
      cols = 0;
  }

  // 获取元素
  double &operator()(int i, int j) {
    if (i < 0 || i >= rows || j < 0 || j >= cols)
      throw std::out_of_range("Matrix index out of range");
    return data[i][j];
  }

  const double &operator()(int i, int j) const {
    if (i < 0 || i >= rows || j < 0 || j >= cols)
      throw std::out_of_range("Matrix index out of range");
    return data[i][j];
  }

  // 获取行数和列数
  int get_rows() const { return rows; }
  int get_cols() const { return cols; }

  /**
   * @brief 矩阵加法
   *
   * @param other 另一个矩阵
   * @return Matrix 结果矩阵
   */
  Matrix operator+(const Matrix &other) const {
    if (rows != other.rows || cols != other.cols)
      throw std::invalid_argument("Matrix dimensions must match for addition");

    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        result(i, j) = data[i][j] + other(i, j);

    return result;
  }

  /**
   * @brief 矩阵减法
   *
   * @param other 另一个矩阵
   * @return Matrix 结果矩阵
   */
  Matrix operator-(const Matrix &other) const {
    if (rows != other.rows || cols != other.cols)
      throw std::invalid_argument(
          "Matrix dimensions must match for subtraction");

    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        result(i, j) = data[i][j] - other(i, j);

    return result;
  }

  /**
   * @brief 矩阵乘法（标准算法）
   *
   * 时间复杂度：O(n³)
   *
   * @param other 另一个矩阵
   * @return Matrix 结果矩阵
   */
  Matrix operator*(const Matrix &other) const {
    if (cols != other.rows)
      throw std::invalid_argument(
          "Matrix dimensions must match for multiplication");

    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < other.cols; j++)
        for (int k = 0; k < cols; k++)
          result(i, j) += data[i][k] * other(k, j);

    return result;
  }

  /**
   * @brief 标量乘法
   *
   * @param scalar 标量值
   * @return Matrix 结果矩阵
   */
  Matrix operator*(double scalar) const {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        result(i, j) = data[i][j] * scalar;

    return result;
  }

  /**
   * @brief 打印矩阵
   */
  void print() const {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        std::cout << std::setw(8) << std::fixed << std::setprecision(2)
                  << data[i][j];
      }
      std::cout << std::endl;
    }
  }

  /**
   * @brief 生成随机矩阵
   *
   * @param r 行数
   * @param c 列数
   * @param min_val 最小值
   * @param max_val 最大值
   * @return Matrix 随机矩阵
   */
  static Matrix random(int r, int c, double min_val = 0.0,
                       double max_val = 1.0) {
    Matrix result(r, c);
    std::srand(std::time(nullptr));

    for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
        double random_val =
            min_val + (max_val - min_val) * (std::rand() / (RAND_MAX + 1.0));
        result(i, j) = random_val;
      }
    }

    return result;
  }

  /**
   * @brief 生成单位矩阵
   *
   * @param n 矩阵大小
   * @return Matrix 单位矩阵
   */
  static Matrix identity(int n) {
    Matrix result(n, n);
    for (int i = 0; i < n; i++) {
      result(i, i) = 1.0;
    }
    return result;
  }

  /**
   * @brief 矩阵转置
   *
   * @return Matrix 转置矩阵
   */
  Matrix transpose() const {
    Matrix result(cols, rows);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        result(j, i) = data[i][j];
    return result;
  }

  /**
   * @brief 检查矩阵是否对称
   *
   * @return bool 是否对称
   */
  bool is_symmetric() const {
    if (rows != cols)
      return false;

    for (int i = 0; i < rows; i++)
      for (int j = i + 1; j < cols; j++)
        if (data[i][j] != data[j][i])
          return false;

    return true;
  }
};

/**
 * @brief 矩阵运算工具类
 */
class MatrixUtils {
public:
  /**
   * @brief 计算矩阵的迹（对角线元素之和）
   *
   * @param mat 矩阵
   * @return double 迹
   */
  static double trace(const Matrix &mat) {
    if (mat.get_rows() != mat.get_cols())
      throw std::invalid_argument(
          "Matrix must be square for trace calculation");

    double result = 0.0;
    for (int i = 0; i < mat.get_rows(); i++) {
      result += mat(i, i);
    }
    return result;
  }

  /**
   * @brief 计算矩阵的行列式（使用递归方法）
   *
   * 时间复杂度：O(n!)
   *
   * @param mat 矩阵
   * @return double 行列式
   */
  static double determinant(const Matrix &mat) {
    if (mat.get_rows() != mat.get_cols())
      throw std::invalid_argument(
          "Matrix must be square for determinant calculation");

    int n = mat.get_rows();

    // 基础情况
    if (n == 1)
      return mat(0, 0);
    if (n == 2)
      return mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);

    double det = 0.0;
    for (int j = 0; j < n; j++) {
      // 计算代数余子式
      Matrix minor(n - 1, n - 1);
      for (int i1 = 1; i1 < n; i1++) {
        for (int j1 = 0, j2 = 0; j1 < n; j1++) {
          if (j1 == j)
            continue;
          minor(i1 - 1, j2) = mat(i1, j1);
          j2++;
        }
      }

      double sign = (j % 2 == 0) ? 1.0 : -1.0;
      det += sign * mat(0, j) * determinant(minor);
    }

    return det;
  }

  /**
   * @brief 计算矩阵的范数（Frobenius范数）
   *
   * @param mat 矩阵
   * @return double 范数
   */
  static double norm(const Matrix &mat) {
    double result = 0.0;
    for (int i = 0; i < mat.get_rows(); i++)
      for (int j = 0; j < mat.get_cols(); j++)
        result += mat(i, j) * mat(i, j);
    return std::sqrt(result);
  }
};

} // namespace algorithms

#endif // MATRIX_OPERATIONS_H
