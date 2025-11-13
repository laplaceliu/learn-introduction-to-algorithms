#ifndef DIVIDE_AND_CONQUER_H
#define DIVIDE_AND_CONQUER_H

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

namespace algorithms {

/**
 * @brief 分治策略算法实现
 *
 * 实现算法导论第4章分治策略，包括：
 * - 最大子数组问题
 * - 矩阵乘法的分治算法
 * - 递归式求解方法
 */
class DivideAndConquer {
private:
  /**
   * @brief 最大子数组结果结构体
   */
  struct MaxSubarrayResult {
    int low;  // 子数组起始索引
    int high; // 子数组结束索引
    int sum;  // 子数组和

    MaxSubarrayResult(int low, int high, int sum)
        : low(low), high(high), sum(sum) {}
  };

public:
  /**
   * @brief 最大子数组问题 - 算法导论第4.1节
   *
   * 使用分治策略找到具有最大和的连续子数组
   *
   * @param arr 输入数组
   * @return 最大子数组的结果（起始索引，结束索引，和）
   */
  static MaxSubarrayResult find_maximum_subarray(const std::vector<int> &arr) {
    if (arr.empty()) {
      return MaxSubarrayResult(-1, -1, 0);
    }
    return find_maximum_subarray(arr, 0, arr.size() - 1);
  }

  /**
   * @brief 暴力求解最大子数组问题
   *
   * 时间复杂度O(n²)，用于对比分治算法的性能
   *
   * @param arr 输入数组
   * @return 最大子数组的结果
   */
  static MaxSubarrayResult
  find_maximum_subarray_brute_force(const std::vector<int> &arr) {
    if (arr.empty()) {
      return MaxSubarrayResult(-1, -1, 0);
    }

    int max_sum = INT_MIN;
    int max_low = 0;
    int max_high = 0;

    for (int i = 0; i < arr.size(); ++i) {
      int current_sum = 0;
      for (int j = i; j < arr.size(); ++j) {
        current_sum += arr[j];
        if (current_sum > max_sum) {
          max_sum = current_sum;
          max_low = i;
          max_high = j;
        }
      }
    }

    return MaxSubarrayResult(max_low, max_high, max_sum);
  }

  /**
   * @brief Kadane算法求解最大子数组问题
   *
   * 线性时间复杂度O(n)的算法
   *
   * @param arr 输入数组
   * @return 最大子数组的结果
   */
  static MaxSubarrayResult
  find_maximum_subarray_kadane(const std::vector<int> &arr) {
    if (arr.empty()) {
      return MaxSubarrayResult(-1, -1, 0);
    }

    int max_so_far = INT_MIN;
    int max_ending_here = 0;
    int start = 0;
    int end = 0;
    int temp_start = 0;

    for (int i = 0; i < arr.size(); ++i) {
      max_ending_here += arr[i];

      if (max_ending_here > max_so_far) {
        max_so_far = max_ending_here;
        start = temp_start;
        end = i;
      }

      if (max_ending_here < 0) {
        max_ending_here = 0;
        temp_start = i + 1;
      }
    }

    return MaxSubarrayResult(start, end, max_so_far);
  }

  /**
   * @brief 矩阵乘法 - 算法导论第4.2节
   *
   * 标准的矩阵乘法算法，时间复杂度O(n³)
   *
   * @param A 矩阵A
   * @param B 矩阵B
   * @return 矩阵乘积C = A × B
   */
  static std::vector<std::vector<int>>
  matrix_multiply(const std::vector<std::vector<int>> &A,
                  const std::vector<std::vector<int>> &B) {
    int n = A.size();
    if (n == 0 || A[0].size() != n || B.size() != n || B[0].size() != n) {
      throw std::invalid_argument("矩阵尺寸不匹配");
    }

    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        for (int k = 0; k < n; ++k) {
          C[i][j] += A[i][k] * B[k][j];
        }
      }
    }

    return C;
  }

  /**
   * @brief Strassen矩阵乘法 - 算法导论第4.2节
   *
   * 使用分治策略的矩阵乘法，时间复杂度O(n^log₂7) ≈ O(n^2.81)
   *
   * @param A 矩阵A
   * @param B 矩阵B
   * @return 矩阵乘积C = A × B
   */
  static std::vector<std::vector<int>>
  strassen_matrix_multiply(const std::vector<std::vector<int>> &A,
                           const std::vector<std::vector<int>> &B) {
    int n = A.size();
    if (n == 0 || A[0].size() != n || B.size() != n || B[0].size() != n) {
      throw std::invalid_argument("矩阵尺寸不匹配");
    }

    // 基础情况：1x1矩阵
    if (n == 1) {
      return {{A[0][0] * B[0][0]}};
    }

    // 递归情况：分治策略
    int half = n / 2;

    // 分割矩阵A和B
    auto A11 = partition_matrix(A, 0, 0, half);
    auto A12 = partition_matrix(A, 0, half, half);
    auto A21 = partition_matrix(A, half, 0, half);
    auto A22 = partition_matrix(A, half, half, half);

    auto B11 = partition_matrix(B, 0, 0, half);
    auto B12 = partition_matrix(B, 0, half, half);
    auto B21 = partition_matrix(B, half, 0, half);
    auto B22 = partition_matrix(B, half, half, half);

    // 计算Strassen的7个乘积
    auto P1 =
        strassen_matrix_multiply(matrix_add(A11, A22), matrix_add(B11, B22));
    auto P2 = strassen_matrix_multiply(matrix_add(A21, A22), B11);
    auto P3 = strassen_matrix_multiply(A11, matrix_subtract(B12, B22));
    auto P4 = strassen_matrix_multiply(A22, matrix_subtract(B21, B11));
    auto P5 = strassen_matrix_multiply(matrix_add(A11, A12), B22);
    auto P6 = strassen_matrix_multiply(matrix_subtract(A21, A11),
                                       matrix_add(B11, B12));
    auto P7 = strassen_matrix_multiply(matrix_subtract(A12, A22),
                                       matrix_add(B21, B22));

    // 计算结果矩阵的四个部分
    auto C11 = matrix_add(matrix_subtract(matrix_add(P1, P4), P5), P7);
    auto C12 = matrix_add(P3, P5);
    auto C21 = matrix_add(P2, P4);
    auto C22 = matrix_add(matrix_subtract(matrix_add(P1, P3), P2), P6);

    // 合并结果矩阵
    return combine_matrices(C11, C12, C21, C22);
  }

  /**
   * @brief 求解递归式 - 算法导论第4.3-4.5节
   *
   * 使用主定理求解分治算法的递归式
   *
   * @param a 子问题数量
   * @param b 子问题规模缩小因子
   * @param f_n 合并步骤的时间复杂度
   * @return 递归式的解
   */
  static std::string solve_recurrence(int a, int b, const std::string &f_n) {
    // 主定理的三种情况
    double log_b_a = log(a) / log(b);

    if (f_n == "O(1)") {
      if (a > 1) {
        return "Θ(n^" + std::to_string(log_b_a) + ")";
      } else {
        return "Θ(log n)";
      }
    } else if (f_n == "O(n)") {
      if (log_b_a > 1) {
        return "Θ(n^" + std::to_string(log_b_a) + ")";
      } else if (abs(log_b_a - 1) < 1e-9) {
        return "Θ(n log n)";
      } else {
        return "Θ(n)";
      }
    } else if (f_n == "O(n²)") {
      if (log_b_a > 2) {
        return "Θ(n^" + std::to_string(log_b_a) + ")";
      } else if (abs(log_b_a - 2) < 1e-9) {
        return "Θ(n² log n)";
      } else {
        return "Θ(n²)";
      }
    }

    return "需要更复杂的主定理分析";
  }

  /**
   * @brief 归并排序 - 分治策略的经典示例
   *
   * 算法导论第2.3.1节，作为分治策略的示例
   *
   * @param arr 待排序数组
   */
  static void merge_sort(std::vector<int> &arr) {
    if (arr.size() <= 1)
      return;

    std::vector<int> temp(arr.size());
    merge_sort(arr, 0, arr.size() - 1, temp);
  }

  /**
   * @brief 验证数组是否已排序
   *
   * @param arr 待验证数组
   * @return 如果数组已排序返回true
   */
  static bool is_sorted(const std::vector<int> &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
      if (arr[i] < arr[i - 1]) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief 打印数组
   *
   * @param arr 数组
   * @param label 标签
   */
  static void print_array(const std::vector<int> &arr,
                          const std::string &label = "") {
    if (!label.empty()) {
      std::cout << label << ": ";
    }
    for (int val : arr) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }

  /**
   * @brief 打印矩阵
   *
   * @param matrix 矩阵
   * @param label 标签
   */
  static void print_matrix(const std::vector<std::vector<int>> &matrix,
                           const std::string &label = "") {
    if (!label.empty()) {
      std::cout << label << ":" << std::endl;
    }
    for (const auto &row : matrix) {
      for (int val : row) {
        std::cout << val << " ";
      }
      std::cout << std::endl;
    }
  }

private:
  /**
   * @brief 递归求解最大子数组问题
   */
  static MaxSubarrayResult find_maximum_subarray(const std::vector<int> &arr,
                                                 int low, int high) {
    if (low == high) {
      return MaxSubarrayResult(low, high, arr[low]);
    }

    int mid = low + (high - low) / 2;

    auto left = find_maximum_subarray(arr, low, mid);
    auto right = find_maximum_subarray(arr, mid + 1, high);
    auto cross = find_max_crossing_subarray(arr, low, mid, high);

    if (left.sum >= right.sum && left.sum >= cross.sum) {
      return left;
    } else if (right.sum >= left.sum && right.sum >= cross.sum) {
      return right;
    } else {
      return cross;
    }
  }

  /**
   * @brief 找到跨越中点的最大子数组
   */
  static MaxSubarrayResult
  find_max_crossing_subarray(const std::vector<int> &arr, int low, int mid,
                             int high) {
    int left_sum = INT_MIN;
    int sum = 0;
    int max_left = mid;

    for (int i = mid; i >= low; --i) {
      sum += arr[i];
      if (sum > left_sum) {
        left_sum = sum;
        max_left = i;
      }
    }

    int right_sum = INT_MIN;
    sum = 0;
    int max_right = mid + 1;

    for (int i = mid + 1; i <= high; ++i) {
      sum += arr[i];
      if (sum > right_sum) {
        right_sum = sum;
        max_right = i;
      }
    }

    return MaxSubarrayResult(max_left, max_right, left_sum + right_sum);
  }

  /**
   * @brief 归并排序的递归实现
   */
  static void merge_sort(std::vector<int> &arr, int left, int right,
                         std::vector<int> &temp) {
    if (left < right) {
      int mid = left + (right - left) / 2;
      merge_sort(arr, left, mid, temp);
      merge_sort(arr, mid + 1, right, temp);
      merge(arr, left, mid, right, temp);
    }
  }

  /**
   * @brief 合并两个已排序的子数组
   */
  static void merge(std::vector<int> &arr, int left, int mid, int right,
                    std::vector<int> &temp) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
      if (arr[i] <= arr[j]) {
        temp[k++] = arr[i++];
      } else {
        temp[k++] = arr[j++];
      }
    }

    while (i <= mid) {
      temp[k++] = arr[i++];
    }

    while (j <= right) {
      temp[k++] = arr[j++];
    }

    for (k = left; k <= right; ++k) {
      arr[k] = temp[k];
    }
  }

  /**
   * @brief 矩阵加法
   */
  static std::vector<std::vector<int>>
  matrix_add(const std::vector<std::vector<int>> &A,
             const std::vector<std::vector<int>> &B) {
    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n));

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        C[i][j] = A[i][j] + B[i][j];
      }
    }

    return C;
  }

  /**
   * @brief 矩阵减法
   */
  static std::vector<std::vector<int>>
  matrix_subtract(const std::vector<std::vector<int>> &A,
                  const std::vector<std::vector<int>> &B) {
    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n));

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        C[i][j] = A[i][j] - B[i][j];
      }
    }

    return C;
  }

  /**
   * @brief 分割矩阵
   */
  static std::vector<std::vector<int>>
  partition_matrix(const std::vector<std::vector<int>> &matrix, int start_row,
                   int start_col, int size) {
    std::vector<std::vector<int>> result(size, std::vector<int>(size));

    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        result[i][j] = matrix[start_row + i][start_col + j];
      }
    }

    return result;
  }

  /**
   * @brief 合并矩阵
   */
  static std::vector<std::vector<int>>
  combine_matrices(const std::vector<std::vector<int>> &C11,
                   const std::vector<std::vector<int>> &C12,
                   const std::vector<std::vector<int>> &C21,
                   const std::vector<std::vector<int>> &C22) {
    int n = C11.size() * 2;
    std::vector<std::vector<int>> result(n, std::vector<int>(n));

    int half = n / 2;

    for (int i = 0; i < half; ++i) {
      for (int j = 0; j < half; ++j) {
        result[i][j] = C11[i][j];
        result[i][j + half] = C12[i][j];
        result[i + half][j] = C21[i][j];
        result[i + half][j + half] = C22[i][j];
      }
    }

    return result;
  }
};

} // namespace algorithms

#endif // DIVIDE_AND_CONQUER_H
