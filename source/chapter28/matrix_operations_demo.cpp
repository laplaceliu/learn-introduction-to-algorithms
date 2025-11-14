#include "linear_systems.h"
#include "matrix_inversion.h"
#include "matrix_operations.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

using namespace algorithms;

/**
 * @brief 测试28.1节：矩阵运算基础
 */
void test_matrix_operations() {
  std::cout << "=== 测试28.1节：矩阵运算基础 ===" << std::endl;

  // 测试用例1：矩阵创建和基本操作
  std::cout << "\n1. 矩阵创建和基本操作测试：" << std::endl;
  Matrix A(2, 3);
  A(0, 0) = 1.0;
  A(0, 1) = 2.0;
  A(0, 2) = 3.0;
  A(1, 0) = 4.0;
  A(1, 1) = 5.0;
  A(1, 2) = 6.0;

  std::cout << "矩阵A：" << std::endl;
  A.print();

  assert(A.get_rows() == 2);
  assert(A.get_cols() == 3);
  assert(A(0, 0) == 1.0);
  assert(A(1, 2) == 6.0);
  std::cout << "✓ 矩阵创建和基本操作测试通过" << std::endl;

  // 测试用例2：矩阵加法和减法
  std::cout << "\n2. 矩阵加法和减法测试：" << std::endl;
  Matrix B(2, 3);
  B(0, 0) = 2.0;
  B(0, 1) = 3.0;
  B(0, 2) = 4.0;
  B(1, 0) = 5.0;
  B(1, 1) = 6.0;
  B(1, 2) = 7.0;

  Matrix C = A + B;
  Matrix D = A - B;

  std::cout << "矩阵A + B：" << std::endl;
  C.print();

  std::cout << "矩阵A - B：" << std::endl;
  D.print();

  assert(C(0, 0) == 3.0);
  assert(C(1, 2) == 13.0);
  assert(D(0, 0) == -1.0);
  assert(D(1, 2) == -1.0);
  std::cout << "✓ 矩阵加法和减法测试通过" << std::endl;

  // 测试用例3：矩阵乘法
  std::cout << "\n3. 矩阵乘法测试：" << std::endl;
  Matrix E(3, 2);
  E(0, 0) = 1.0;
  E(0, 1) = 2.0;
  E(1, 0) = 3.0;
  E(1, 1) = 4.0;
  E(2, 0) = 5.0;
  E(2, 1) = 6.0;

  Matrix F = A * E;

  std::cout << "矩阵A * E：" << std::endl;
  F.print();

  assert(F.get_rows() == 2);
  assert(F.get_cols() == 2);
  assert(std::abs(F(0, 0) - 22.0) < 1e-10);
  assert(std::abs(F(1, 1) - 64.0) < 1e-10);
  std::cout << "✓ 矩阵乘法测试通过" << std::endl;

  // 测试用例4：标量乘法和转置
  std::cout << "\n4. 标量乘法和转置测试：" << std::endl;
  Matrix G = A * 2.0;
  Matrix H = A.transpose();

  std::cout << "矩阵A * 2：" << std::endl;
  G.print();

  std::cout << "矩阵A的转置：" << std::endl;
  H.print();

  assert(G(0, 0) == 2.0);
  assert(G(1, 2) == 12.0);
  assert(H.get_rows() == 3);
  assert(H.get_cols() == 2);
  assert(H(0, 0) == 1.0);
  assert(H(2, 1) == 6.0);
  std::cout << "✓ 标量乘法和转置测试通过" << std::endl;

  // 测试用例5：单位矩阵和对称性检查
  std::cout << "\n5. 单位矩阵和对称性检查测试：" << std::endl;
  Matrix I = Matrix::identity(3);

  std::cout << "3x3单位矩阵：" << std::endl;
  I.print();

  Matrix J(2, 2);
  J(0, 0) = 1.0;
  J(0, 1) = 2.0;
  J(1, 0) = 2.0;
  J(1, 1) = 1.0;

  assert(I.is_symmetric());
  assert(J.is_symmetric());
  assert(!A.is_symmetric());
  std::cout << "✓ 单位矩阵和对称性检查测试通过" << std::endl;

  // 测试用例6：矩阵工具函数
  std::cout << "\n6. 矩阵工具函数测试：" << std::endl;
  Matrix K(2, 2);
  K(0, 0) = 1.0;
  K(0, 1) = 2.0;
  K(1, 0) = 3.0;
  K(1, 1) = 4.0;

  double trace = MatrixUtils::trace(K);
  double det = MatrixUtils::determinant(K);
  double norm = MatrixUtils::norm(K);

  std::cout << "矩阵K的迹：" << trace << std::endl;
  std::cout << "矩阵K的行列式：" << det << std::endl;
  std::cout << "矩阵K的范数：" << norm << std::endl;

  assert(std::abs(trace - 5.0) < 1e-10);
  assert(std::abs(det + 2.0) < 1e-10);
  assert(std::abs(norm - std::sqrt(30.0)) < 1e-10);
  std::cout << "✓ 矩阵工具函数测试通过" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 测试28.2节：线性方程组求解
 */
void test_linear_systems() {
  std::cout << "=== 测试28.2节：线性方程组求解 ===" << std::endl;

  // 测试用例1：高斯消元法
  std::cout << "\n1. 高斯消元法测试：" << std::endl;
  Matrix A(3, 3);
  A(0, 0) = 2.0;
  A(0, 1) = 1.0;
  A(0, 2) = -1.0;
  A(1, 0) = -3.0;
  A(1, 1) = -1.0;
  A(1, 2) = 2.0;
  A(2, 0) = -2.0;
  A(2, 1) = 1.0;
  A(2, 2) = 2.0;

  std::vector<double> b = {8.0, -11.0, -3.0};

  std::vector<double> x_gauss = LinearSystemSolver::gaussian_elimination(A, b);

  std::cout << "高斯消元法解：[";
  for (size_t i = 0; i < x_gauss.size(); i++) {
    std::cout << x_gauss[i];
    if (i < x_gauss.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  double residual_gauss = LinearSystemSolver::residual(A, b, x_gauss);
  std::cout << "残差：" << residual_gauss << std::endl;

  assert(std::abs(x_gauss[0] - 2.0) < 1e-10);
  assert(std::abs(x_gauss[1] - 3.0) < 1e-10);
  assert(std::abs(x_gauss[2] - -1.0) < 1e-10);
  assert(residual_gauss < 1e-10);
  std::cout << "✓ 高斯消元法测试通过" << std::endl;

  // 测试用例2：LU分解法
  std::cout << "\n2. LU分解法测试：" << std::endl;
  std::vector<double> x_lu = LinearSystemSolver::lu_decomposition(A, b);

  std::cout << "LU分解法解：[";
  for (size_t i = 0; i < x_lu.size(); i++) {
    std::cout << x_lu[i];
    if (i < x_lu.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  double residual_lu = LinearSystemSolver::residual(A, b, x_lu);
  std::cout << "残差：" << residual_lu << std::endl;

  assert(std::abs(x_lu[0] - 2.0) < 1e-10);
  assert(std::abs(x_lu[1] - 3.0) < 1e-10);
  assert(std::abs(x_lu[2] - -1.0) < 1e-10);
  assert(residual_lu < 1e-10);
  std::cout << "✓ LU分解法测试通过" << std::endl;

  // 测试用例3：雅可比迭代法（对角占优矩阵）
  std::cout << "\n3. 雅可比迭代法测试：" << std::endl;
  Matrix B(3, 3);
  B(0, 0) = 10.0;
  B(0, 1) = -1.0;
  B(0, 2) = 2.0;
  B(1, 0) = -1.0;
  B(1, 1) = 11.0;
  B(1, 2) = -1.0;
  B(2, 0) = 2.0;
  B(2, 1) = -1.0;
  B(2, 2) = 10.0;

  std::vector<double> c = {6.0, 25.0, -11.0};

  assert(LinearSystemSolver::is_diagonally_dominant(B));

  std::vector<double> x_jacobi =
      LinearSystemSolver::jacobi_iteration(B, c, 100, 1e-10);

  std::cout << "雅可比迭代法解：[";
  for (size_t i = 0; i < x_jacobi.size(); i++) {
    std::cout << x_jacobi[i];
    if (i < x_jacobi.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  double residual_jacobi = LinearSystemSolver::residual(B, c, x_jacobi);
  std::cout << "残差：" << residual_jacobi << std::endl;

  assert(residual_jacobi < 1e-6);
  std::cout << "✓ 雅可比迭代法测试通过" << std::endl;

  // 测试用例4：奇异矩阵检测
  std::cout << "\n4. 奇异矩阵检测测试：" << std::endl;
  Matrix C(2, 2);
  C(0, 0) = 1.0;
  C(0, 1) = 2.0;
  C(1, 0) = 2.0;
  C(1, 1) = 4.0;

  std::vector<double> d = {1.0, 2.0};

  try {
    std::vector<double> x_singular =
        LinearSystemSolver::gaussian_elimination(C, d);
    std::cout << "✗ 奇异矩阵检测失败" << std::endl;
  } catch (const std::runtime_error &e) {
    std::cout << "✓ 奇异矩阵检测成功：" << e.what() << std::endl;
  }

  std::cout << std::endl;
}

/**
 * @brief 测试28.3节：矩阵求逆
 */
void test_matrix_inversion() {
  std::cout << "=== 测试28.3节：矩阵求逆 ===" << std::endl;

  // 测试用例1：高斯-约当消元法求逆
  std::cout << "\n1. 高斯-约当消元法求逆测试：" << std::endl;
  Matrix A(3, 3);
  A(0, 0) = 2.0;
  A(0, 1) = -1.0;
  A(0, 2) = 0.0;
  A(1, 0) = -1.0;
  A(1, 1) = 2.0;
  A(1, 2) = -1.0;
  A(2, 0) = 0.0;
  A(2, 1) = -1.0;
  A(2, 2) = 2.0;

  Matrix A_inv_gj = MatrixInverter::gauss_jordan_inversion(A);

  std::cout << "原矩阵A：" << std::endl;
  A.print();
  std::cout << "逆矩阵A⁻¹（高斯-约当法）：" << std::endl;
  A_inv_gj.print();

  bool valid_gj = MatrixInverter::verify_inversion(A, A_inv_gj);
  std::cout << "逆矩阵验证：" << (valid_gj ? "通过" : "失败") << std::endl;

  assert(valid_gj);
  std::cout << "✓ 高斯-约当消元法求逆测试通过" << std::endl;

  // 测试用例2：LU分解法求逆
  std::cout << "\n2. LU分解法求逆测试：" << std::endl;
  Matrix A_inv_lu = MatrixInverter::lu_inversion(A);

  std::cout << "逆矩阵A⁻¹（LU分解法）：" << std::endl;
  A_inv_lu.print();

  bool valid_lu = MatrixInverter::verify_inversion(A, A_inv_lu);
  std::cout << "逆矩阵验证：" << (valid_lu ? "通过" : "失败") << std::endl;

  assert(valid_lu);
  std::cout << "✓ LU分解法求逆测试通过" << std::endl;

  // 测试用例3：伴随矩阵法求逆（小矩阵）
  std::cout << "\n3. 伴随矩阵法求逆测试：" << std::endl;
  Matrix B(2, 2);
  B(0, 0) = 4.0;
  B(0, 1) = 7.0;
  B(1, 0) = 2.0;
  B(1, 1) = 6.0;

  Matrix B_inv_adj = MatrixInverter::adjugate_inversion(B);

  std::cout << "原矩阵B：" << std::endl;
  B.print();
  std::cout << "逆矩阵B⁻¹（伴随矩阵法）：" << std::endl;
  B_inv_adj.print();

  bool valid_adj = MatrixInverter::verify_inversion(B, B_inv_adj);
  std::cout << "逆矩阵验证：" << (valid_adj ? "通过" : "失败") << std::endl;

  assert(valid_adj);
  std::cout << "✓ 伴随矩阵法求逆测试通过" << std::endl;

  // 测试用例4：可逆性检查和条件数
  std::cout << "\n4. 可逆性检查和条件数测试：" << std::endl;
  bool invertible_A = MatrixInverter::is_invertible(A);
  bool invertible_B = MatrixInverter::is_invertible(B);

  Matrix C(2, 2);
  C(0, 0) = 1.0;
  C(0, 1) = 1.0;
  C(1, 0) = 1.0;
  C(1, 1) = 1.0;
  bool invertible_C = MatrixInverter::is_invertible(C);

  std::cout << "矩阵A可逆：" << (invertible_A ? "是" : "否") << std::endl;
  std::cout << "矩阵B可逆：" << (invertible_B ? "是" : "否") << std::endl;
  std::cout << "矩阵C可逆：" << (invertible_C ? "是" : "否") << std::endl;

  double cond_A = MatrixInverter::condition_number(A);
  std::cout << "矩阵A的条件数：" << cond_A << std::endl;

  assert(invertible_A);
  assert(invertible_B);
  assert(!invertible_C);
  assert(cond_A > 0);
  std::cout << "✓ 可逆性检查和条件数测试通过" << std::endl;

  std::cout << std::endl;
}

/**
 * @brief 主函数：运行所有测试
 */
int main() {
  std::cout << "算法导论第28章：矩阵运算 - 演示程序" << std::endl;
  std::cout << "====================================" << std::endl;
  std::cout << std::endl;

  try {
    test_matrix_operations();
    test_linear_systems();
    test_matrix_inversion();

    std::cout << "====================================" << std::endl;
    std::cout << "所有测试用例通过！第28章矩阵运算实现完成。" << std::endl;
    std::cout << "实现内容：" << std::endl;
    std::cout << "- 28.1节：矩阵运算基础（矩阵类、基本运算、工具函数）"
              << std::endl;
    std::cout << "- 28.2节：线性方程组求解（高斯消元、LU分解、雅可比迭代）"
              << std::endl;
    std::cout << "- 28.3节：矩阵求逆（高斯-约当、LU分解、伴随矩阵法）"
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "测试失败：" << e.what() << std::endl;
    return 1;
  }

  return 0;
}
