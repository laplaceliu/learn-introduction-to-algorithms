#ifndef ROD_CUTTING_H
#define ROD_CUTTING_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>

namespace algorithms {

class RodCutting {
private:
    std::vector<int> prices; // 价格表：prices[i]表示长度为i的钢条的价格
    
public:
    RodCutting(const std::vector<int>& price_table) : prices(price_table) {
        // 确保价格表包含长度0的价格（为0）
        if (prices.empty()) {
            prices.push_back(0);
        }
        if (prices[0] != 0) {
            prices.insert(prices.begin(), 0);
        }
    }
    
    // 自顶向下的递归解法（带备忘录）
    int cut_rod_memoized(int n) {
        if (n < 0 || n >= static_cast<int>(prices.size())) {
            return 0;
        }
        
        std::vector<int> memo(n + 1, -1);
        return cut_rod_memoized_aux(n, memo);
    }
    
    // 自底向上的动态规划解法
    int cut_rod_bottom_up(int n) {
        if (n < 0 || n >= static_cast<int>(prices.size())) {
            return 0;
        }
        
        std::vector<int> dp(n + 1, 0);
        
        for (int i = 1; i <= n; i++) {
            int max_val = std::numeric_limits<int>::min();
            for (int j = 1; j <= i; j++) {
                if (j < static_cast<int>(prices.size())) {
                    max_val = std::max(max_val, prices[j] + dp[i - j]);
                } else {
                    max_val = std::max(max_val, dp[i - j]);
                }
            }
            dp[i] = max_val;
        }
        
        return dp[n];
    }
    
    // 扩展版本：返回最大收益和切割方案
    struct CuttingSolution {
        int max_revenue;
        std::vector<int> cuts; // 切割方案：每个元素表示一段的长度
        
        CuttingSolution() : max_revenue(0) {}
        CuttingSolution(int revenue, const std::vector<int>& cut_scheme) 
            : max_revenue(revenue), cuts(cut_scheme) {}
    };
    
    CuttingSolution cut_rod_with_solution(int n) {
        if (n < 0 || n >= static_cast<int>(prices.size())) {
            return CuttingSolution(0, {});
        }
        
        std::vector<int> dp(n + 1, 0);
        std::vector<int> first_cut(n + 1, 0); // 记录第一段切割长度
        
        for (int i = 1; i <= n; i++) {
            int max_val = std::numeric_limits<int>::min();
            int best_cut = 0;
            
            for (int j = 1; j <= i; j++) {
                int current_val;
                if (j < static_cast<int>(prices.size())) {
                    current_val = prices[j] + dp[i - j];
                } else {
                    current_val = dp[i - j];
                }
                
                if (current_val > max_val) {
                    max_val = current_val;
                    best_cut = j;
                }
            }
            
            dp[i] = max_val;
            first_cut[i] = best_cut;
        }
        
        // 重构切割方案
        std::vector<int> cuts;
        int remaining = n;
        while (remaining > 0) {
            int cut = first_cut[remaining];
            cuts.push_back(cut);
            remaining -= cut;
        }
        
        return CuttingSolution(dp[n], cuts);
    }
    
    // 打印价格表
    void print_price_table() const {
        std::cout << "钢条切割价格表:" << std::endl;
        std::cout << "长度(i)";
        for (size_t i = 0; i < prices.size(); i++) {
            std::cout << "\t" << i;
        }
        std::cout << std::endl;
        
        std::cout << "价格(p[i])";
        for (size_t i = 0; i < prices.size(); i++) {
            std::cout << "\t" << prices[i];
        }
        std::cout << std::endl;
    }
    
    // 获取价格表大小
    size_t get_price_table_size() const {
        return prices.size();
    }
    
    // 获取指定长度的价格
    int get_price(int length) const {
        if (length >= 0 && length < static_cast<int>(prices.size())) {
            return prices[length];
        }
        return 0;
    }

private:
    // 自顶向下递归辅助函数
    int cut_rod_memoized_aux(int n, std::vector<int>& memo) {
        if (n == 0) {
            return 0;
        }
        
        if (memo[n] != -1) {
            return memo[n];
        }
        
        int max_val = std::numeric_limits<int>::min();
        
        for (int i = 1; i <= n; i++) {
            if (i < static_cast<int>(prices.size())) {
                max_val = std::max(max_val, prices[i] + cut_rod_memoized_aux(n - i, memo));
            } else {
                max_val = std::max(max_val, cut_rod_memoized_aux(n - i, memo));
            }
        }
        
        memo[n] = max_val;
        return max_val;
    }
};

// 算法导论图15-1的示例价格表
std::vector<int> get_example_price_table() {
    return {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
}

} // namespace algorithms

#endif // ROD_CUTTING_H