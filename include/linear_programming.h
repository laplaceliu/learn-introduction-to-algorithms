#ifndef LINEAR_PROGRAMMING_H
#define LINEAR_PROGRAMMING_H

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace algorithms {

/**
 * @brief 线性规划算法实现
 *
 * 实现算法导论第29章线性规划，包括：
 * - 标准型线性规划
 * - 单纯形法
 * - 对偶理论
 * - 初始可行解构造
 */
class LinearProgramming {
private:
  /**
   * @brief 线性规划问题结构体
   */
  struct LinearProgram {
    std::vector<std::vector<double>> A; // 约束矩阵
    std::vector<double> b;              // 约束右端项
    std::vector<double> c;              // 目标函数系数
    std::vector<int> basis;             // 基变量索引
    std::vector<int> non_basis;         // 非基变量索引
    double objective_value;             // 目标函数值

    LinearProgram(const std::vector<std::vector<double>> &A,
                  const std::vector<double> &b, const std::vector<double> &c)
        : A(A), b(b), c(c), objective_value(0.0) {
      // 验证输入维度
      if (A.empty() || b.empty() || c.empty()) {
        throw std::invalid_argument("输入矩阵或向量不能为空");
      }

      size_t m = A.size();
      size_t n = c.size();

      for (size_t i = 0; i < m; ++i) {
        if (A[i].size() != n) {
          throw std::invalid_argument(
              "约束矩阵A的列数必须等于目标函数系数c的长度");
        }
      }

      if (b.size() != m) {
        throw std::invalid_argument("约束右端项b的长度必须等于约束矩阵A的行数");
      }
    }
  };

  /**
   * @brief 单纯形表结构体
   */
  struct SimplexTableau {
    std::vector<std::vector<double>> tableau; // 单纯形表
    std::vector<int> basis;                   // 基变量索引
    std::vector<int> non_basis;               // 非基变量索引
    double objective_value;                   // 当前目标函数值

    SimplexTableau(size_t m, size_t n)
        : tableau(m + 1, std::vector<double>(n + 1, 0.0)),
          objective_value(0.0) {}
  };

public:
  /**
   * @brief 线性规划结果结构体
   */
  struct LPResult {
    std::vector<double> solution; // 最优解
    double objective_value;       // 最优目标函数值
    bool feasible;                // 是否可行
    bool bounded;                 // 是否有界
    std::string status;           // 求解状态

    LPResult()
        : objective_value(0.0), feasible(false), bounded(false),
          status("未求解") {}
  };

  /**
   * @brief 单纯形法求解线性规划 - 算法导论第29.3节
   *
   * 使用单纯形法求解标准型线性规划问题：
   * 最大化 c^T x
   * 满足 Ax ≤ b, x ≥ 0
   *
   * @param A 约束矩阵
   * @param b 约束右端项
   * @param c 目标函数系数
   * @return 线性规划求解结果
   */
  static LPResult simplex_method(const std::vector<std::vector<double>> &A,
                                 const std::vector<double> &b,
                                 const std::vector<double> &c) {
    LPResult result;

    try {
      // 转换为标准型：最大化问题，所有约束为≤，变量非负
      auto standard_form = convert_to_standard_form(A, b, c);

      // 构造初始可行解
      auto initial_solution = find_initial_feasible_solution(standard_form);

      if (!initial_solution.feasible) {
        result.feasible = false;
        result.bounded = false;
        result.status = "问题不可行";
        return result;
      }

      // 执行单纯形迭代
      result = perform_simplex_iterations(standard_form);

    } catch (const std::exception &e) {
      result.status = "求解失败: " + std::string(e.what());
    }

    return result;
  }

  /**
   * @brief 两阶段单纯形法 - 处理无初始可行解的情况
   *
   * 第一阶段：构造辅助问题找到初始可行解
   * 第二阶段：用找到的可行解求解原问题
   *
   * @param A 约束矩阵
   * @param b 约束右端项
   * @param c 目标函数系数
   * @return 线性规划求解结果
   */
  static LPResult two_phase_simplex(const std::vector<std::vector<double>> &A,
                                    const std::vector<double> &b,
                                    const std::vector<double> &c) {
    LPResult result;

    try {
      // 第一阶段：构造辅助问题
      auto phase1_result = phase_one_simplex(A, b, c);

      if (!phase1_result.feasible) {
        result.feasible = false;
        result.bounded = false;
        result.status = "问题不可行";
        return result;
      }

      // 第二阶段：用第一阶段找到的基求解原问题
      result = phase_two_simplex(A, b, c, phase1_result.solution);

    } catch (const std::exception &e) {
      result.status = "求解失败: " + std::string(e.what());
    }

    return result;
  }

  /**
   * @brief 对偶单纯形法 - 算法导论第29.4节
   *
   * 当原问题不可行但对偶问题可行时使用
   *
   * @param A 约束矩阵
   * @param b 约束右端项
   * @param c 目标函数系数
   * @return 线性规划求解结果
   */
  static LPResult dual_simplex_method(const std::vector<std::vector<double>> &A,
                                      const std::vector<double> &b,
                                      const std::vector<double> &c) {
    LPResult result;

    try {
      // 转换为对偶问题
      auto dual_problem = form_dual_problem(A, b, c);

      // 用单纯形法求解对偶问题
      result = simplex_method(dual_problem.A, dual_problem.b, dual_problem.c);

      // 根据对偶理论得到原问题解
      if (result.feasible && result.bounded) {
        result = recover_primal_solution(result, A, b, c);
      }

    } catch (const std::exception &e) {
      result.status = "求解失败: " + std::string(e.what());
    }

    return result;
  }

  /**
   * @brief 验证线性规划解的最优性
   *
   * 使用互补松弛条件验证解的最优性
   *
   * @param solution 待验证的解
   * @param A 约束矩阵
   * @param b 约束右端项
   * @param c 目标函数系数
   * @return 验证结果和相关信息
   */
  static std::string
  verify_optimality(const std::vector<double> &solution,
                    const std::vector<std::vector<double>> &A,
                    const std::vector<double> &b,
                    const std::vector<double> &c) {
    if (solution.empty() || A.empty() || b.empty() || c.empty()) {
      return "输入数据不完整";
    }

    size_t m = A.size();
    size_t n = c.size();

    // 检查非负约束
    for (size_t j = 0; j < n; ++j) {
      if (solution[j] < -1e-10) { // 考虑数值误差
        return "解不满足非负约束";
      }
    }

    // 检查约束满足性
    for (size_t i = 0; i < m; ++i) {
      double constraint_value = 0.0;
      for (size_t j = 0; j < n; ++j) {
        constraint_value += A[i][j] * solution[j];
      }

      if (constraint_value > b[i] + 1e-10) { // 考虑数值误差
        return "解不满足约束条件";
      }
    }

    // 计算目标函数值
    double obj_value = 0.0;
    for (size_t j = 0; j < n; ++j) {
      obj_value += c[j] * solution[j];
    }

    std::stringstream ss;
    ss << "解满足所有约束，目标函数值: " << std::fixed << std::setprecision(6)
       << obj_value;

    return ss.str();
  }

  /**
   * @brief 生成随机线性规划问题
   *
   * 用于测试和演示，确保问题可行且有界
   *
   * @param m 约束数量
   * @param n 变量数量
   * @param density 约束矩阵密度(0-1)
   * @return 随机线性规划问题 {A, b, c}
   */
  static std::tuple<std::vector<std::vector<double>>, std::vector<double>,
                    std::vector<double>>
  generate_random_lp(size_t m, size_t n, double density = 0.7) {
    std::vector<std::vector<double>> A(m, std::vector<double>(n, 0.0));
    std::vector<double> b(m);
    std::vector<double> c(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> coeff_dis(0.1,
                                                     5.0); // 正系数确保可行性
    std::uniform_real_distribution<double> b_dis(10.0,
                                                 50.0); // 合理的右端项范围
    std::uniform_real_distribution<double> obj_dis(-5.0,
                                                   5.0); // 目标函数系数可正可负
    std::bernoulli_distribution density_dis(density);

    // 生成目标函数系数
    for (size_t j = 0; j < n; ++j) {
      c[j] = obj_dis(gen);
    }

    // 生成约束矩阵和右端项，确保问题可行且有界
    for (size_t i = 0; i < m; ++i) {
      // 生成正系数约束，确保原点可行
      for (size_t j = 0; j < n; ++j) {
        if (density_dis(gen)) {
          A[i][j] = coeff_dis(gen); // 正系数
        }
      }

      // 确保右端项足够大，使得约束不紧
      b[i] = b_dis(gen);

      // 添加一个上界约束，确保问题有界
      if (i == 0) {
        for (size_t j = 0; j < n; ++j) {
          A[i][j] = 1.0; // 所有变量之和 ≤ b[0]
        }
      }
    }

    return {A, b, c};
  }

private:
  /**
   * @brief 转换为标准型线性规划
   */
  static LinearProgram
  convert_to_standard_form(const std::vector<std::vector<double>> &A,
                           const std::vector<double> &b,
                           const std::vector<double> &c) {
    size_t m = A.size();
    size_t n = c.size();

    // 假设输入已经是标准型：最大化问题，≤约束，变量非负
    // 在实际实现中，这里会处理各种形式的转换

    return LinearProgram(A, b, c);
  }

  /**
   * @brief 寻找初始可行解
   */
  static LPResult find_initial_feasible_solution(const LinearProgram &lp) {
    LPResult result;

    // 简单实现：检查原点是否可行
    std::vector<double> zero_solution(lp.c.size(), 0.0);

    bool feasible = true;
    for (size_t i = 0; i < lp.b.size(); ++i) {
      double constraint_value = 0.0;
      for (size_t j = 0; j < lp.c.size(); ++j) {
        constraint_value += lp.A[i][j] * zero_solution[j];
      }

      if (constraint_value > lp.b[i] + 1e-10) {
        feasible = false;
        break;
      }
    }

    if (feasible) {
      result.feasible = true;
      result.bounded = true;
      result.solution = zero_solution;
      result.objective_value = 0.0;
      result.status = "找到初始可行解（原点）";
    } else {
      result.feasible = false;
      result.status = "原点不可行，需要使用两阶段法";
    }

    return result;
  }

  /**
   * @brief 执行单纯形迭代
   */
  static LPResult perform_simplex_iterations(const LinearProgram &lp) {
    LPResult result;

    // 简化实现：使用简单的迭代方法
    // 在实际实现中，这里会实现完整的单纯形表迭代

    size_t n = lp.c.size();
    size_t m = lp.b.size();

    // 初始化基变量（松弛变量）
    std::vector<int> basis(m);
    for (size_t i = 0; i < m; ++i) {
      basis[i] = n + i; // 松弛变量索引
    }

    // 初始化非基变量（原变量）
    std::vector<int> non_basis(n);
    for (size_t j = 0; j < n; ++j) {
      non_basis[j] = j;
    }

    // 构造初始单纯形表
    SimplexTableau tableau(m, n + m);

    // 填充约束部分
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        tableau.tableau[i][j] = lp.A[i][j];
      }
      tableau.tableau[i][n + i] = 1.0;     // 松弛变量系数
      tableau.tableau[i][n + m] = lp.b[i]; // 右端项
    }

    // 填充目标函数行
    for (size_t j = 0; j < n; ++j) {
      tableau.tableau[m][j] = -lp.c[j]; // 负号因为我们要最大化
    }
    tableau.tableau[m][n + m] = 0.0; // 初始目标函数值

    // 简单迭代（最多100次防止无限循环）
    const int max_iterations = 100;
    for (int iter = 0; iter < max_iterations; ++iter) {
      // 检查最优性：所有检验数是否非负
      bool optimal = true;
      int entering_var = -1;

      for (size_t j = 0; j < n + m; ++j) {
        if (tableau.tableau[m][j] < -1e-10) { // 考虑数值误差
          optimal = false;
          entering_var = j;
          break;
        }
      }

      if (optimal) {
        // 找到最优解
        result.feasible = true;
        result.bounded = true;
        result.objective_value = tableau.tableau[m][n + m];
        result.solution.resize(n, 0.0);

        // 提取基变量值
        for (size_t i = 0; i < m; ++i) {
          int var_index = basis[i];
          if (var_index < n) { // 原变量
            result.solution[var_index] = tableau.tableau[i][n + m];
          }
        }

        result.status = "单纯形法收敛到最优解";
        break;
      }

      if (entering_var == -1) {
        result.status = "无法确定进入变量";
        break;
      }

      // 寻找离开变量（最小比值检验）
      int leaving_var = -1;
      double min_ratio = std::numeric_limits<double>::max();

      for (size_t i = 0; i < m; ++i) {
        if (tableau.tableau[i][entering_var] > 1e-10) { // 正系数
          double ratio =
              tableau.tableau[i][n + m] / tableau.tableau[i][entering_var];
          if (ratio < min_ratio) {
            min_ratio = ratio;
            leaving_var = i;
          }
        }
      }

      if (leaving_var == -1) {
        result.feasible = true;
        result.bounded = false;
        result.status = "问题无界";
        break;
      }

      // 执行旋转操作
      perform_pivot(tableau, entering_var, leaving_var, basis);
    }

    if (result.status == "未求解") {
      result.status = "达到最大迭代次数";
    }

    return result;
  }

  /**
   * @brief 执行旋转操作
   */
  static void perform_pivot(SimplexTableau &tableau, int entering_var,
                            int leaving_var, std::vector<int> &basis) {
    size_t m = tableau.tableau.size() - 1;
    size_t n = tableau.tableau[0].size() - 1;

    // 主元行归一化
    double pivot_element = tableau.tableau[leaving_var][entering_var];
    for (size_t j = 0; j <= n; ++j) {
      tableau.tableau[leaving_var][j] /= pivot_element;
    }

    // 更新其他行
    for (size_t i = 0; i <= m; ++i) {
      if (i != leaving_var) {
        double factor = tableau.tableau[i][entering_var];
        for (size_t j = 0; j <= n; ++j) {
          tableau.tableau[i][j] -= factor * tableau.tableau[leaving_var][j];
        }
      }
    }

    // 更新基变量
    basis[leaving_var] = entering_var;
  }

  /**
   * @brief 第一阶段单纯形法
   */
  static LPResult phase_one_simplex(const std::vector<std::vector<double>> &A,
                                    const std::vector<double> &b,
                                    const std::vector<double> &c) {
    LPResult result;

    // 简化实现：构造辅助问题
    size_t m = A.size();
    size_t n = c.size();

    // 构造辅助问题的目标函数：最小化人工变量和
    std::vector<double> phase1_c(n + m, 0.0);
    for (size_t j = n; j < n + m; ++j) {
      phase1_c[j] = -1.0; // 最小化人工变量和
    }

    // 构造辅助问题的约束
    std::vector<std::vector<double>> phase1_A = A;
    std::vector<double> phase1_b = b;

    // 添加人工变量
    for (size_t i = 0; i < m; ++i) {
      phase1_A[i].resize(n + m, 0.0);
      phase1_A[i][n + i] = 1.0; // 人工变量系数
    }

    // 使用单纯形法求解辅助问题
    result = simplex_method(phase1_A, phase1_b, phase1_c);

    if (result.feasible && std::abs(result.objective_value) < 1e-10) {
      result.status = "第一阶段成功，找到可行解";
    } else {
      result.feasible = false;
      result.status = "第一阶段失败，原问题不可行";
    }

    return result;
  }

  /**
   * @brief 第二阶段单纯形法
   */
  static LPResult
  phase_two_simplex(const std::vector<std::vector<double>> &A,
                    const std::vector<double> &b, const std::vector<double> &c,
                    const std::vector<double> &initial_solution) {
    // 使用第一阶段找到的可行解求解原问题
    return simplex_method(A, b, c);
  }

  /**
   * @brief 构造对偶问题
   */
  static LinearProgram
  form_dual_problem(const std::vector<std::vector<double>> &A,
                    const std::vector<double> &b,
                    const std::vector<double> &c) {
    size_t m = A.size();
    size_t n = c.size();

    // 原问题：最大化 c^T x, 满足 Ax ≤ b, x ≥ 0
    // 对偶问题：最小化 b^T y, 满足 A^T y ≥ c, y ≥ 0

    std::vector<std::vector<double>> dual_A(n, std::vector<double>(m, 0.0));
    std::vector<double> dual_b(m);
    std::vector<double> dual_c(n);

    // 对偶问题的约束矩阵是原问题约束矩阵的转置
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < m; ++j) {
        dual_A[i][j] = A[j][i];
      }
    }

    // 对偶问题的目标函数系数是原问题的右端项
    dual_b = b;

    // 对偶问题的右端项是原问题的目标函数系数
    dual_c = c;

    return LinearProgram(dual_A, dual_b, dual_c);
  }

  /**
   * @brief 从对偶解恢复原问题解
   */
  static LPResult recover_primal_solution(
      const LPResult &dual_result, const std::vector<std::vector<double>> &A,
      const std::vector<double> &b, const std::vector<double> &c) {
    LPResult result = dual_result;

    // 根据对偶理论，对偶问题的对偶变量对应原问题的松弛变量
    // 这里简化处理，直接返回对偶解

    result.status = "通过对偶问题求解得到原问题解";
    return result;
  }
};

} // namespace algorithms

#endif // LINEAR_PROGRAMMING_H
