#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <vector>

namespace algorithms {

// 11.1 直接寻址表
template <typename K, typename V> class DirectAddressTable {
private:
  std::vector<std::shared_ptr<V>> table;
  int size;

public:
  DirectAddressTable(int max_key) : size(max_key + 1) {
    table.resize(size, nullptr);
  }

  // 插入操作
  void insert(K key, const V &value) {
    if (key >= 0 && key < size) {
      table[key] = std::make_shared<V>(value);
    }
  }

  // 搜索操作
  std::shared_ptr<V> search(K key) {
    if (key >= 0 && key < size) {
      return table[key];
    }
    return nullptr;
  }

  // 删除操作
  void remove(K key) {
    if (key >= 0 && key < size) {
      table[key] = nullptr;
    }
  }

  // 获取所有元素
  std::vector<std::pair<K, V>> get_all() {
    std::vector<std::pair<K, V>> result;
    for (int i = 0; i < size; i++) {
      if (table[i] != nullptr) {
        result.push_back({i, *table[i]});
      }
    }
    return result;
  }

  // 打印表内容
  void print() {
    std::cout << "直接寻址表内容:" << std::endl;
    for (int i = 0; i < size; i++) {
      if (table[i] != nullptr) {
        std::cout << "键 " << i << ": " << *table[i] << std::endl;
      }
    }
  }
};

// 11.2 散列表 - 链接法解决冲突
template <typename K, typename V> class ChainedHashTable {
private:
  struct Node {
    K key;
    V value;
    std::shared_ptr<Node> next;

    Node(K k, V v) : key(k), value(v), next(nullptr) {}
  };

  std::vector<std::shared_ptr<Node>> table;
  int size;
  std::function<int(K)> hash_function;

public:
  ChainedHashTable(int table_size, std::function<int(K)> hash_func)
      : size(table_size), hash_function(hash_func) {
    table.resize(size, nullptr);
  }

  // 插入操作
  void insert(K key, const V &value) {
    int index = hash_function(key) % size;
    auto new_node = std::make_shared<Node>(key, value);

    if (table[index] == nullptr) {
      table[index] = new_node;
    } else {
      auto current = table[index];
      while (current->next != nullptr) {
        if (current->key == key) {
          current->value = value; // 更新现有键值
          return;
        }
        current = current->next;
      }
      if (current->key == key) {
        current->value = value;
      } else {
        current->next = new_node;
      }
    }
  }

  // 搜索操作
  std::shared_ptr<V> search(K key) {
    int index = hash_function(key) % size;
    auto current = table[index];

    while (current != nullptr) {
      if (current->key == key) {
        return std::make_shared<V>(current->value);
      }
      current = current->next;
    }

    return nullptr;
  }

  // 删除操作
  bool remove(K key) {
    int index = hash_function(key) % size;
    auto current = table[index];
    std::shared_ptr<Node> prev = nullptr;

    while (current != nullptr) {
      if (current->key == key) {
        if (prev == nullptr) {
          table[index] = current->next;
        } else {
          prev->next = current->next;
        }
        return true;
      }
      prev = current;
      current = current->next;
    }

    return false;
  }

  // 获取负载因子
  double load_factor() {
    int count = 0;
    for (int i = 0; i < size; i++) {
      auto current = table[i];
      while (current != nullptr) {
        count++;
        current = current->next;
      }
    }
    return static_cast<double>(count) / size;
  }

  // 打印散列表
  void print() {
    std::cout << "链接法散列表:" << std::endl;
    for (int i = 0; i < size; i++) {
      std::cout << "槽 " << i << ": ";
      auto current = table[i];
      while (current != nullptr) {
        std::cout << "(" << current->key << ", " << current->value << ") ";
        current = current->next;
      }
      std::cout << std::endl;
    }
    std::cout << "负载因子: " << load_factor() << std::endl;
  }
};

// 11.3 散列函数
class HashFunctions {
public:
  // 除法散列法
  static int division_hash(int key, int table_size) { return key % table_size; }

  // 乘法散列法
  static int multiplication_hash(int key, int table_size) {
    const double A = (std::sqrt(5) - 1) / 2; // 黄金比例倒数
    double fractional = key * A - std::floor(key * A);
    return static_cast<int>(table_size * fractional);
  }

  // 全域散列法
  class UniversalHash {
  private:
    int a, b, p, m;

  public:
    UniversalHash(int prime, int table_size) : p(prime), m(table_size) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis_a(1, p - 1);
      std::uniform_int_distribution<> dis_b(0, p - 1);

      a = dis_a(gen);
      b = dis_b(gen);
    }

    int operator()(int key) { return ((a * key + b) % p) % m; }
  };

  // 字符串散列函数（简单版本）
  static int string_hash(const std::string &key, int table_size) {
    int hash = 0;
    for (char c : key) {
      hash = (hash * 31 + c) % table_size;
    }
    return hash;
  }
};

// 11.4 开放寻址法
template <typename K, typename V> class OpenAddressingHashTable {
private:
  enum SlotStatus { EMPTY, OCCUPIED, DELETED };

  struct Slot {
    K key;
    V value;
    SlotStatus status;

    Slot() : status(EMPTY) {}
  };

  std::vector<Slot> table;
  int size;
  std::function<int(K, int)> hash_function;

  // 线性探查
  int linear_probing(K key, int i) {
    return (hash_function(key, 0) + i) % size;
  }

  // 二次探查
  int quadratic_probing(K key, int i) {
    int c1 = 1, c2 = 3; // 常数
    return (hash_function(key, 0) + c1 * i + c2 * i * i) % size;
  }

  // 双重散列
  int double_hashing(K key, int i) {
    int h1 = hash_function(key, 0);
    int h2 = 1 + (key % (size - 1)); // 第二个散列函数
    return (h1 + i * h2) % size;
  }

public:
  OpenAddressingHashTable(int table_size, std::function<int(K, int)> hash_func)
      : size(table_size), hash_function(hash_func) {
    table.resize(size);
  }

  // 插入操作
  bool insert(K key, const V &value) {
    for (int i = 0; i < size; i++) {
      int index = hash_function(key, i);

      if (table[index].status != OCCUPIED) {
        table[index].key = key;
        table[index].value = value;
        table[index].status = OCCUPIED;
        return true;
      }
    }

    return false; // 表已满
  }

  // 搜索操作
  std::shared_ptr<V> search(K key) {
    for (int i = 0; i < size; i++) {
      int index = hash_function(key, i);

      if (table[index].status == EMPTY) {
        return nullptr; // 未找到
      }

      if (table[index].status == OCCUPIED && table[index].key == key) {
        return std::make_shared<V>(table[index].value);
      }
    }

    return nullptr;
  }

  // 删除操作
  bool remove(K key) {
    for (int i = 0; i < size; i++) {
      int index = hash_function(key, i);

      if (table[index].status == EMPTY) {
        return false; // 未找到
      }

      if (table[index].status == OCCUPIED && table[index].key == key) {
        table[index].status = DELETED;
        return true;
      }
    }

    return false;
  }

  // 获取负载因子
  double load_factor() {
    int count = 0;
    for (int i = 0; i < size; i++) {
      if (table[i].status == OCCUPIED) {
        count++;
      }
    }
    return static_cast<double>(count) / size;
  }

  // 打印散列表
  void print() {
    std::cout << "开放寻址法散列表:" << std::endl;
    for (int i = 0; i < size; i++) {
      std::cout << "槽 " << i << ": ";
      if (table[i].status == OCCUPIED) {
        std::cout << "(" << table[i].key << ", " << table[i].value << ")";
      } else if (table[i].status == DELETED) {
        std::cout << "[DELETED]";
      } else {
        std::cout << "[EMPTY]";
      }
      std::cout << std::endl;
    }
    std::cout << "负载因子: " << load_factor() << std::endl;
  }
};

// 11.5 完全散列（简化实现）
template <typename K, typename V> class PerfectHashTable {
private:
  struct SecondaryTable {
    std::vector<std::pair<K, V>> table;
    int a, b, size;

    SecondaryTable(int table_size) : size(table_size) {
      if (table_size > 0) {
        table.resize(table_size);

        // 初始化所有槽位
        for (int i = 0; i < table_size; i++) {
          table[i] = {K(), V()}; // 使用默认值初始化
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        // 处理table_size为1的特殊情况
        if (table_size == 1) {
          a = 1; // 当表大小为1时，a只能为1
          b = 0; // b只能为0
        } else {
          std::uniform_int_distribution<> dis_a(1, table_size - 1);
          std::uniform_int_distribution<> dis_b(0, table_size - 1);

          a = dis_a(gen);
          b = dis_b(gen);
        }
      } else {
        // 当table_size为0时，设置默认值并确保table为空向量
        a = 1;
        b = 0;
        table.clear(); // 确保table为空
      }
    }

    int hash(K key) const {
      if (size == 0)
        return 0;
      return ((a * static_cast<int>(key) + b) % size);
    }

    bool insert(K key, const V &value) {
      if (size == 0)
        return false;
      int index = hash(key);
      if (index >= 0 && index < table.size()) {
        if (table[index].first == K() || table[index].first == key) {
          table[index] = {key, value};
          return true;
        }
      }
      return false; // 冲突或索引越界
    }

    std::shared_ptr<V> search(K key) const {
      if (size == 0)
        return nullptr;
      int index = hash(key);
      if (index >= 0 && index < table.size() && table[index].first == key) {
        return std::make_shared<V>(table[index].second);
      }
      return nullptr;
    }
  };

  std::vector<SecondaryTable> secondary_tables;
  std::vector<int> primary_table;
  int primary_size;
  int total_elements;

public:
  PerfectHashTable(const std::vector<std::pair<K, V>> &elements)
      : total_elements(elements.size()) {
    if (elements.empty()) {
      primary_size = 1;
      primary_table.resize(primary_size, 0);
      secondary_tables.push_back(SecondaryTable(0));
      return;
    }

    // 选择主表大小
    primary_size = total_elements;
    if (primary_size == 0)
      primary_size = 1; // 确保主表大小至少为1
    primary_table.resize(primary_size, 0);

    // 统计每个槽的元素数量
    for (const auto &elem : elements) {
      int primary_index = elem.first % primary_size;
      if (primary_index >= 0 && primary_index < primary_size) {
        primary_table[primary_index]++;
      }
    }

    // 为每个槽创建二级表
    secondary_tables.reserve(primary_size); // 预分配空间
    for (int i = 0; i < primary_size; i++) {
      int secondary_size = primary_table[i] * primary_table[i]; // 平方大小
      secondary_tables.push_back(SecondaryTable(std::max(1, secondary_size)));
    }

    // 插入所有元素
    for (const auto &elem : elements) {
      int primary_index = elem.first % primary_size;
      if (primary_index >= 0 && primary_index < secondary_tables.size()) {
        if (!secondary_tables[primary_index].insert(elem.first, elem.second)) {
          // 如果插入失败，需要重新构建（简化实现中我们假设总能成功）
          std::cout << "警告：完全散列构建时发生冲突" << std::endl;
        }
      }
    }
  }

  // 搜索操作
  std::shared_ptr<V> search(K key) const {
    if (secondary_tables.empty())
      return nullptr;
    int primary_index = key % primary_size;
    if (primary_index < secondary_tables.size()) {
      return secondary_tables[primary_index].search(key);
    }
    return nullptr;
  }

  // 打印完全散列表
  void print() {
    std::cout << "完全散列表结构:" << std::endl;
    std::cout << "主表大小: " << primary_size << std::endl;

    for (int i = 0; i < primary_size && i < secondary_tables.size(); i++) {
      std::cout << "主槽 " << i
                << " -> 二级表大小: " << secondary_tables[i].size << std::endl;
    }
  }
};

// 散列表工具类
class HashTableUtils {
public:
  // 测试散列函数分布
  static void test_hash_distribution(std::function<int(int)> hash_func,
                                     int table_size, int num_keys) {
    std::vector<int> distribution(table_size, 0);

    for (int i = 0; i < num_keys; i++) {
      int key = i;
      int index = hash_func(key) % table_size;
      distribution[index]++;
    }

    std::cout << "散列函数分布测试:" << std::endl;
    std::cout << "表大小: " << table_size << ", 键数量: " << num_keys
              << std::endl;

    int max_count = *std::max_element(distribution.begin(), distribution.end());
    int min_count = *std::min_element(distribution.begin(), distribution.end());

    std::cout << "最大槽元素数: " << max_count << std::endl;
    std::cout << "最小槽元素数: " << min_count << std::endl;
    std::cout << "平均槽元素数: " << static_cast<double>(num_keys) / table_size
              << std::endl;
  }

  // 性能比较
  template <typename HashTableType>
  static void
  performance_test(HashTableType &hash_table,
                   const std::vector<std::pair<int, std::string>> &operations) {
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &op : operations) {
      if (op.second == "insert") {
        hash_table.insert(op.first, "value" + std::to_string(op.first));
      } else if (op.second == "search") {
        hash_table.search(op.first);
      } else if (op.second == "delete") {
        hash_table.remove(op.first);
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "操作数量: " << operations.size()
              << ", 耗时: " << duration.count() << " 微秒" << std::endl;
  }
};

} // namespace algorithms

#endif // HASH_TABLE_H
