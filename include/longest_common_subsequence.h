#ifndef LONGEST_COMMON_SUBSEQUENCE_H
#define LONGEST_COMMON_SUBSEQUENCE_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace algorithms {

class LongestCommonSubsequence {
private:
  std::string seq1;
  std::string seq2;

public:
  LongestCommonSubsequence(const std::string &s1, const std::string &s2)
      : seq1(s1), seq2(s2) {}

  // 计算LCS长度（自底向上动态规划）
  int lcs_length() {
    int m = seq1.length();
    int n = seq2.length();

    if (m == 0 || n == 0) {
      return 0;
    }

    // 创建DP表
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // 填充DP表
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        if (seq1[i - 1] == seq2[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1] + 1;
        } else {
          dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
      }
    }

    return dp[m][n];
  }

  // 计算LCS长度并返回LCS字符串
  struct LCSResult {
    int length;
    std::string lcs_string;

    LCSResult() : length(0) {}
    LCSResult(int len, const std::string &str) : length(len), lcs_string(str) {}
  };

  LCSResult lcs_with_string() {
    int m = seq1.length();
    int n = seq2.length();

    if (m == 0 || n == 0) {
      return LCSResult(0, "");
    }

    // 创建DP表和方向表
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<char>> direction(m + 1,
                                             std::vector<char>(n + 1, ' '));

    // 填充DP表和方向表
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        if (seq1[i - 1] == seq2[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1] + 1;
          direction[i][j] = 'D'; // 对角线方向 (Diagonal)
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
          dp[i][j] = dp[i - 1][j];
          direction[i][j] = 'U'; // 向上方向 (Up)
        } else {
          dp[i][j] = dp[i][j - 1];
          direction[i][j] = 'L'; // 向左方向 (Left)
        }
      }
    }

    // 重构LCS字符串
    std::string lcs_str = reconstruct_lcs(direction, m, n);

    return LCSResult(dp[m][n], lcs_str);
  }

  // 打印DP表（用于调试和可视化）
  void print_dp_table() {
    int m = seq1.length();
    int n = seq2.length();

    if (m == 0 || n == 0) {
      std::cout << "序列为空" << std::endl;
      return;
    }

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<char>> direction(m + 1,
                                             std::vector<char>(n + 1, ' '));

    // 填充DP表和方向表
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        if (seq1[i - 1] == seq2[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1] + 1;
          direction[i][j] = 'D';
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
          dp[i][j] = dp[i - 1][j];
          direction[i][j] = 'U';
        } else {
          dp[i][j] = dp[i][j - 1];
          direction[i][j] = 'L';
        }
      }
    }

    // 打印DP表
    std::cout << "DP表（最长公共子序列长度）:" << std::endl;
    std::cout << "    ";
    for (int j = 0; j <= n; j++) {
      if (j == 0) {
        std::cout << "  ";
      } else {
        std::cout << std::setw(3) << seq2[j - 1];
      }
    }
    std::cout << std::endl;

    for (int i = 0; i <= m; i++) {
      if (i == 0) {
        std::cout << "  ";
      } else {
        std::cout << std::setw(2) << seq1[i - 1];
      }

      for (int j = 0; j <= n; j++) {
        std::cout << std::setw(3) << dp[i][j];
      }
      std::cout << std::endl;
    }

    // 打印方向表
    std::cout << "\n方向表 (D=对角线, U=向上, L=向左):" << std::endl;
    std::cout << "    ";
    for (int j = 0; j <= n; j++) {
      if (j == 0) {
        std::cout << "  ";
      } else {
        std::cout << std::setw(3) << seq2[j - 1];
      }
    }
    std::cout << std::endl;

    for (int i = 0; i <= m; i++) {
      if (i == 0) {
        std::cout << "  ";
      } else {
        std::cout << std::setw(2) << seq1[i - 1];
      }

      for (int j = 0; j <= n; j++) {
        std::cout << std::setw(3) << direction[i][j];
      }
      std::cout << std::endl;
    }
  }

  // 获取序列1
  std::string get_sequence1() const { return seq1; }

  // 获取序列2
  std::string get_sequence2() const { return seq2; }

  // 设置新序列
  void set_sequences(const std::string &s1, const std::string &s2) {
    seq1 = s1;
    seq2 = s2;
  }

private:
  // 重构LCS字符串
  std::string reconstruct_lcs(const std::vector<std::vector<char>> &direction,
                              int i, int j) {
    if (i == 0 || j == 0) {
      return "";
    }

    if (direction[i][j] == 'D') {
      return reconstruct_lcs(direction, i - 1, j - 1) + seq1[i - 1];
    } else if (direction[i][j] == 'U') {
      return reconstruct_lcs(direction, i - 1, j);
    } else {
      return reconstruct_lcs(direction, i, j - 1);
    }
  }
};

// 算法导论中的经典示例
void print_classic_example() {
  std::cout << "=== 算法导论经典示例 ===" << std::endl;

  // 示例1：算法导论图15-8
  std::string s1 = "ABCBDAB";
  std::string s2 = "BDCAB";

  LongestCommonSubsequence lcs1(s1, s2);
  auto result1 = lcs1.lcs_with_string();

  std::cout << "序列1: " << s1 << std::endl;
  std::cout << "序列2: " << s2 << std::endl;
  std::cout << "最长公共子序列长度: " << result1.length << std::endl;
  std::cout << "最长公共子序列: " << result1.lcs_string << std::endl;
  std::cout << "期望结果: BCAB (长度4)" << std::endl;

  // 示例2：另一个经典示例
  std::string s3 = "XMJYAUZ";
  std::string s4 = "MZJAWXU";

  LongestCommonSubsequence lcs2(s3, s4);
  auto result2 = lcs2.lcs_with_string();

  std::cout << "\n序列1: " << s3 << std::endl;
  std::cout << "序列2: " << s4 << std::endl;
  std::cout << "最长公共子序列长度: " << result2.length << std::endl;
  std::cout << "最长公共子序列: " << result2.lcs_string << std::endl;
  std::cout << "期望结果: MJAU (长度4)" << std::endl;
}

} // namespace algorithms

#endif // LONGEST_COMMON_SUBSEQUENCE_H
