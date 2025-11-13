#ifndef PROBABILISTIC_ANALYSIS_H
#define PROBABILISTIC_ANALYSIS_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <unordered_map>
#include <vector>

namespace algorithms {

/**
 * @brief 概率分析和随机算法实现
 *
 * 实现算法导论第5章概率分析和随机算法，包括：
 * - 雇佣问题（在线雇佣问题）
 * - 随机排列生成
 * - 随机化算法分析
 * - 概率分析技术
 */
class ProbabilisticAnalysis {
private:
  /**
   * @brief 候选人结构体
   */
  struct Candidate {
    int id;      // 候选人ID
    int quality; // 候选人质量分数

    Candidate(int id, int quality) : id(id), quality(quality) {}

    bool operator<(const Candidate &other) const {
      return quality < other.quality;
    }

    bool operator>(const Candidate &other) const {
      return quality > other.quality;
    }
  };

public:
  /**
   * @brief 雇佣问题 - 算法导论第5.1节
   *
   * 问题描述：面试n个候选人，每次面试后必须立即决定是否雇佣。
   * 目标：以高概率雇佣到最好的候选人
   *
   * @param candidates 候选人质量数组
   * @param k
   * 策略参数：面试前k个候选人但不雇佣，然后雇佣第一个比前k个都好的候选人
   * @return 雇佣的候选人ID（-1表示没有雇佣）
   */
  static int hiring_problem(const std::vector<int> &candidates, int k) {
    if (candidates.empty())
      return -1;

    // 找到前k个候选人中的最佳质量
    int best_in_k = -1;
    for (int i = 0; i < std::min(k, static_cast<int>(candidates.size())); ++i) {
      if (candidates[i] > best_in_k) {
        best_in_k = candidates[i];
      }
    }

    // 从第k+1个候选人开始，雇佣第一个比前k个都好的候选人
    for (int i = k; i < candidates.size(); ++i) {
      if (candidates[i] > best_in_k) {
        return i; // 返回候选人索引
      }
    }

    // 如果没有找到更好的候选人，雇佣最后一个
    return candidates.size() - 1;
  }

  /**
   * @brief 在线雇佣问题 - 算法导论第5.2节
   *
   * 每次面试需要支付费用，雇佣需要支付更高的费用。
   * 目标：最小化总成本
   *
   * @param candidates 候选人质量数组
   * @param interview_cost 面试成本
   * @param hiring_cost 雇佣成本
   * @return 总成本
   */
  static double online_hiring_problem(const std::vector<int> &candidates,
                                      double interview_cost,
                                      double hiring_cost) {
    if (candidates.empty())
      return 0.0;

    double total_cost = 0.0;
    int best_so_far = -1;

    for (int i = 0; i < candidates.size(); ++i) {
      total_cost += interview_cost; // 支付面试费用

      if (candidates[i] > best_so_far) {
        total_cost += hiring_cost; // 支付雇佣费用
        best_so_far = candidates[i];
      }
    }

    return total_cost;
  }

  /**
   * @brief 随机排列生成 - 算法导论第5.3节
   *
   * 使用Fisher-Yates洗牌算法生成随机排列
   *
   * @param n 排列大小
   * @return 1到n的随机排列
   */
  static std::vector<int> random_permutation(int n) {
    std::vector<int> permutation(n);

    // 初始化顺序排列
    for (int i = 0; i < n; ++i) {
      permutation[i] = i + 1;
    }

    // Fisher-Yates洗牌算法
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = n - 1; i > 0; --i) {
      std::uniform_int_distribution<> dis(0, i);
      int j = dis(gen);
      std::swap(permutation[i], permutation[j]);
    }

    return permutation;
  }

  /**
   * @brief 随机化雇佣问题
   *
   * 使用随机排列来随机化输入顺序
   *
   * @param candidates 候选人质量数组
   * @param k 策略参数
   * @return 雇佣的候选人ID
   */
  static int randomized_hiring_problem(std::vector<int> candidates, int k) {
    // 随机排列候选人
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(candidates.begin(), candidates.end(), gen);

    return hiring_problem(candidates, k);
  }

  /**
   * @brief 生日悖论模拟 - 概率分析示例
   *
   * 计算在n个人中至少有两个人生日相同的概率
   *
   * @param n 人数
   * @param trials 模拟次数
   * @return 概率估计值
   */
  static double birthday_paradox(int n, int trials = 10000) {
    if (n <= 1)
      return 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 365); // 假设一年365天

    int collisions = 0;

    for (int t = 0; t < trials; ++t) {
      std::unordered_map<int, bool> birthdays;
      bool collision_found = false;

      for (int i = 0; i < n; ++i) {
        int birthday = dis(gen);
        if (birthdays.find(birthday) != birthdays.end()) {
          collision_found = true;
          break;
        }
        birthdays[birthday] = true;
      }

      if (collision_found) {
        ++collisions;
      }
    }

    return static_cast<double>(collisions) / trials;
  }

  /**
   * @brief 球与箱子问题 - 概率分析示例
   *
   * 将b个球随机投入n个箱子，分析箱子负载
   *
   * @param balls 球的数量
   * @param bins 箱子的数量
   * @param trials 模拟次数
   * @return 最大负载的期望值
   */
  static double balls_and_bins(int balls, int bins, int trials = 10000) {
    if (bins <= 0)
      return 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, bins - 1);

    double total_max_load = 0.0;

    for (int t = 0; t < trials; ++t) {
      std::vector<int> bin_loads(bins, 0);
      int max_load = 0;

      for (int i = 0; i < balls; ++i) {
        int bin = dis(gen);
        bin_loads[bin]++;
        max_load = std::max(max_load, bin_loads[bin]);
      }

      total_max_load += max_load;
    }

    return total_max_load / trials;
  }

  /**
   * @brief 随机化快速选择算法 - 算法导论第9.2节
   *
   * 随机化选择第i小的元素，期望时间复杂度O(n)
   *
   * @param arr 输入数组
   * @param left 左边界
   * @param right 右边界
   * @param i 要选择的顺序统计量（1-based）
   * @return 第i小的元素
   */
  static int randomized_select(std::vector<int> &arr, int left, int right,
                               int i) {
    if (left == right) {
      return arr[left];
    }

    // 随机分区
    int pivot_index = randomized_partition(arr, left, right);
    int k = pivot_index - left + 1; // 分区后基准元素的排名

    if (i == k) {
      return arr[pivot_index];
    } else if (i < k) {
      return randomized_select(arr, left, pivot_index - 1, i);
    } else {
      return randomized_select(arr, pivot_index + 1, right, i - k);
    }
  }

  /**
   * @brief 随机分区操作
   *
   * @param arr 数组
   * @param left 左边界
   * @param right 右边界
   * @return 分区后基准元素的位置
   */
  static int randomized_partition(std::vector<int> &arr, int left, int right) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(left, right);
    int random_index = dis(gen);

    std::swap(arr[random_index], arr[right]);

    return partition(arr, left, right);
  }

  /**
   * @brief 分区操作
   *
   * @param arr 数组
   * @param left 左边界
   * @param right 右边界
   * @return 分区后基准元素的位置
   */
  static int partition(std::vector<int> &arr, int left, int right) {
    int pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; ++j) {
      if (arr[j] <= pivot) {
        ++i;
        std::swap(arr[i], arr[j]);
      }
    }

    std::swap(arr[i + 1], arr[right]);
    return i + 1;
  }

  /**
   * @brief 概率分析：雇佣问题最优k值分析
   *
   * 分析不同k值下雇佣到最佳候选人的概率
   *
   * @param n 候选人数量
   * @param trials 模拟次数
   * @return 每个k值对应的成功概率
   */
  static std::map<int, double> analyze_optimal_k(int n, int trials = 10000) {
    std::map<int, double> success_rates;

    // 生成最佳候选人的位置（1-based）
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int k = 1; k <= n; ++k) {
      int successes = 0;

      for (int t = 0; t < trials; ++t) {
        // 生成随机排列
        std::vector<int> candidates(n);
        for (int i = 0; i < n; ++i) {
          candidates[i] = i + 1;
        }
        std::shuffle(candidates.begin(), candidates.end(), gen);

        // 找到最佳候选人的位置
        int best_candidate =
            *std::max_element(candidates.begin(), candidates.end());
        int best_position = -1;
        for (int i = 0; i < n; ++i) {
          if (candidates[i] == best_candidate) {
            best_position = i;
            break;
          }
        }

        // 应用雇佣策略
        int hired_position = hiring_problem(candidates, k);

        if (hired_position == best_position) {
          ++successes;
        }
      }

      success_rates[k] = static_cast<double>(successes) / trials;
    }

    return success_rates;
  }

  /**
   * @brief 验证数组是否包含特定元素
   *
   * @param arr 数组
   * @param value 要查找的值
   * @return 如果找到返回true
   */
  static bool contains(const std::vector<int> &arr, int value) {
    return std::find(arr.begin(), arr.end(), value) != arr.end();
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
   * @brief 打印概率分析结果
   *
   * @param results 概率分析结果
   * @param title 标题
   */
  static void print_probability_analysis(const std::map<int, double> &results,
                                         const std::string &title) {
    std::cout << title << ":" << std::endl;
    for (const auto &[key, value] : results) {
      std::cout << "  k=" << key << ": " << value * 100 << "%" << std::endl;
    }
  }
};

} // namespace algorithms

#endif // PROBABILISTIC_ANALYSIS_H
