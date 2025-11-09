#ifndef STRING_MATCHING_H
#define STRING_MATCHING_H

#include <string>
#include <vector>
#include <functional>
#include <cmath>
#include <iostream>
#include <unordered_map>

namespace algorithms {

// 32.1 朴素字符串匹配算法
class NaiveStringMatcher {
public:
    // 朴素字符串匹配
    static std::vector<int> search(const std::string& text, const std::string& pattern) {
        std::vector<int> matches;
        int n = text.length();
        int m = pattern.length();
        
        for (int s = 0; s <= n - m; s++) {
            bool match = true;
            for (int i = 0; i < m; i++) {
                if (text[s + i] != pattern[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                matches.push_back(s);
            }
        }
        
        return matches;
    }
    
    // 带偏移量显示的匹配
    static void search_with_offset(const std::string& text, const std::string& pattern) {
        auto matches = search(text, pattern);
        
        std::cout << "文本: " << text << std::endl;
        std::cout << "模式: " << pattern << std::endl;
        std::cout << "匹配位置: ";
        for (int pos : matches) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
        
        // 显示偏移量
        if (!matches.empty()) {
            std::cout << "偏移量显示:" << std::endl;
            std::cout << text << std::endl;
            for (int pos : matches) {
                std::cout << std::string(pos, ' ') << pattern << std::endl;
            }
        }
    }
};

// 32.2 Rabin-Karp算法
class RabinKarpMatcher {
private:
    static const int PRIME = 101;  // 大素数
    static const int BASE = 256;   // 字符集大小（ASCII）
    
public:
    // Rabin-Karp字符串匹配
    static std::vector<int> search(const std::string& text, const std::string& pattern) {
        std::vector<int> matches;
        int n = text.length();
        int m = pattern.length();
        
        if (m == 0 || n < m) {
            return matches;
        }
        
        // 计算模式哈希和第一个文本窗口哈希
        long long pattern_hash = 0;
        long long text_hash = 0;
        long long h = 1;
        
        // h = BASE^(m-1) % PRIME
        for (int i = 0; i < m - 1; i++) {
            h = (h * BASE) % PRIME;
        }
        
        // 计算初始哈希值
        for (int i = 0; i < m; i++) {
            pattern_hash = (BASE * pattern_hash + pattern[i]) % PRIME;
            text_hash = (BASE * text_hash + text[i]) % PRIME;
        }
        
        // 滑动窗口
        for (int s = 0; s <= n - m; s++) {
            // 检查哈希值是否匹配
            if (pattern_hash == text_hash) {
                // 哈希匹配，检查实际字符
                bool match = true;
                for (int i = 0; i < m; i++) {
                    if (text[s + i] != pattern[i]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    matches.push_back(s);
                }
            }
            
            // 计算下一个窗口的哈希值
            if (s < n - m) {
                text_hash = (BASE * (text_hash - text[s] * h) + text[s + m]) % PRIME;
                
                // 确保哈希值为正
                if (text_hash < 0) {
                    text_hash += PRIME;
                }
            }
        }
        
        return matches;
    }
    
    // 显示哈希计算过程
    static void search_with_hash(const std::string& text, const std::string& pattern) {
        auto matches = search(text, pattern);
        
        std::cout << "文本: " << text << std::endl;
        std::cout << "模式: " << pattern << std::endl;
        std::cout << "模式哈希: " << calculate_hash(pattern, pattern.length()) << std::endl;
        std::cout << "匹配位置: ";
        for (int pos : matches) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }
    
private:
    static long long calculate_hash(const std::string& str, int length) {
        long long hash = 0;
        for (int i = 0; i < length; i++) {
            hash = (BASE * hash + str[i]) % PRIME;
        }
        return hash;
    }
};

// 32.3 利用有限自动机进行字符串匹配
class FiniteAutomatonMatcher {
private:
    // 计算转移函数
    static std::vector<std::vector<int>> compute_transition_function(const std::string& pattern, int alphabet_size = 256) {
        int m = pattern.length();
        std::vector<std::vector<int>> transition(m + 1, std::vector<int>(alphabet_size, 0));
        
        for (int q = 0; q <= m; q++) {
            for (int a = 0; a < alphabet_size; a++) {
                int k = std::min(m, q + 1);
                
                // 找到P_k是P_q a的后缀的最大k
                while (k > 0) {
                    if (pattern[k - 1] == a) {
                        bool is_suffix = true;
                        for (int i = 0; i < k - 1; i++) {
                            if (pattern[i] != pattern[q - k + 1 + i]) {
                                is_suffix = false;
                                break;
                            }
                        }
                        if (is_suffix) {
                            break;
                        }
                    }
                    k--;
                }
                
                transition[q][a] = k;
            }
        }
        
        return transition;
    }
    
public:
    // 有限自动机字符串匹配
    static std::vector<int> search(const std::string& text, const std::string& pattern) {
        std::vector<int> matches;
        int n = text.length();
        int m = pattern.length();
        
        if (m == 0) {
            return matches;
        }
        
        auto transition = compute_transition_function(pattern);
        int q = 0;  // 当前状态
        
        for (int i = 0; i < n; i++) {
            q = transition[q][static_cast<unsigned char>(text[i])];
            if (q == m) {
                matches.push_back(i - m + 1);
            }
        }
        
        return matches;
    }
    
    // 显示状态转移过程
    static void search_with_transition(const std::string& text, const std::string& pattern) {
        auto matches = search(text, pattern);
        
        std::cout << "文本: " << text << std::endl;
        std::cout << "模式: " << pattern << std::endl;
        
        auto transition = compute_transition_function(pattern);
        std::cout << "状态转移表:" << std::endl;
        for (int q = 0; q <= pattern.length(); q++) {
            std::cout << "状态 " << q << ": ";
            for (char c : {'a', 'b', 'c'}) {  // 只显示部分字符
                if (c <= 'z' && c >= 'a') {
                    std::cout << c << "->" << transition[q][c] << " ";
                }
            }
            std::cout << std::endl;
        }
        
        std::cout << "匹配位置: ";
        for (int pos : matches) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }
};

// 32.4 Knuth-Morris-Pratt算法
class KMPMatcher {
private:
    // 计算前缀函数（部分匹配表）
    static std::vector<int> compute_prefix_function(const std::string& pattern) {
        int m = pattern.length();
        std::vector<int> pi(m, 0);
        int k = 0;
        
        for (int q = 1; q < m; q++) {
            while (k > 0 && pattern[k] != pattern[q]) {
                k = pi[k - 1];
            }
            if (pattern[k] == pattern[q]) {
                k++;
            }
            pi[q] = k;
        }
        
        return pi;
    }
    
public:
    // KMP字符串匹配
    static std::vector<int> search(const std::string& text, const std::string& pattern) {
        std::vector<int> matches;
        int n = text.length();
        int m = pattern.length();
        
        if (m == 0) {
            return matches;
        }
        
        auto pi = compute_prefix_function(pattern);
        int q = 0;  // 匹配的字符数
        
        for (int i = 0; i < n; i++) {
            while (q > 0 && pattern[q] != text[i]) {
                q = pi[q - 1];
            }
            if (pattern[q] == text[i]) {
                q++;
            }
            if (q == m) {
                matches.push_back(i - m + 1);
                q = pi[q - 1];
            }
        }
        
        return matches;
    }
    
    // 显示前缀函数和匹配过程
    static void search_with_prefix(const std::string& text, const std::string& pattern) {
        auto matches = search(text, pattern);
        
        std::cout << "文本: " << text << std::endl;
        std::cout << "模式: " << pattern << std::endl;
        
        auto pi = compute_prefix_function(pattern);
        std::cout << "前缀函数: ";
        for (int i = 0; i < pattern.length(); i++) {
            std::cout << "pi[" << i << "]=" << pi[i] << " ";
        }
        std::cout << std::endl;
        
        std::cout << "匹配位置: ";
        for (int pos : matches) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }
};

// 字符串匹配工具类
class StringMatchingUtils {
public:
    // 比较不同算法的性能
    static void compare_algorithms(const std::string& text, const std::string& pattern) {
        std::cout << "=== 字符串匹配算法比较 ===" << std::endl;
        std::cout << "文本长度: " << text.length() << std::endl;
        std::cout << "模式长度: " << pattern.length() << std::endl;
        std::cout << "文本: " << (text.length() > 50 ? text.substr(0, 50) + "..." : text) << std::endl;
        std::cout << "模式: " << pattern << std::endl;
        
        // 朴素算法
        auto naive_matches = NaiveStringMatcher::search(text, pattern);
        std::cout << "朴素算法匹配数: " << naive_matches.size() << std::endl;
        
        // Rabin-Karp算法
        auto rk_matches = RabinKarpMatcher::search(text, pattern);
        std::cout << "Rabin-Karp匹配数: " << rk_matches.size() << std::endl;
        
        // 有限自动机算法
        auto fa_matches = FiniteAutomatonMatcher::search(text, pattern);
        std::cout << "有限自动机匹配数: " << fa_matches.size() << std::endl;
        
        // KMP算法
        auto kmp_matches = KMPMatcher::search(text, pattern);
        std::cout << "KMP算法匹配数: " << kmp_matches.size() << std::endl;
        
        // 验证结果一致性
        bool consistent = (naive_matches == rk_matches) && 
                         (rk_matches == fa_matches) && 
                         (fa_matches == kmp_matches);
        std::cout << "算法结果一致性: " << (consistent ? "一致" : "不一致") << std::endl;
        
        if (!consistent) {
            std::cout << "警告：算法结果不一致！" << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    // 生成测试用例
    static std::string generate_test_text(int length, const std::string& alphabet = "abcdefghijklmnopqrstuvwxyz") {
        std::string result;
        result.reserve(length);
        
        for (int i = 0; i < length; i++) {
            result += alphabet[rand() % alphabet.length()];
        }
        
        return result;
    }
    
    // 生成包含模式多次出现的文本
    static std::string generate_text_with_pattern(const std::string& pattern, int occurrences, int text_length) {
        std::string text = generate_test_text(text_length);
        
        // 在随机位置插入模式
        for (int i = 0; i < occurrences; i++) {
            int pos = rand() % (text_length - pattern.length());
            for (int j = 0; j < pattern.length(); j++) {
                text[pos + j] = pattern[j];
            }
        }
        
        return text;
    }
};

} // namespace algorithms

#endif // STRING_MATCHING_H