#include "hash_table.h"
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace algorithms;

// 测试11.1 直接寻址表
void test_direct_address_table() {
  std::cout << "=== 测试直接寻址表 (11.1) ===" << std::endl;

  DirectAddressTable<int, std::string> dat(10);

  // 插入操作
  dat.insert(1, "Alice");
  dat.insert(3, "Bob");
  dat.insert(5, "Charlie");
  dat.insert(7, "David");

  // 打印表内容
  dat.print();

  // 搜索操作
  auto result1 = dat.search(3);
  auto result2 = dat.search(2);

  std::cout << "搜索键3: " << (result1 ? *result1 : "未找到") << std::endl;
  std::cout << "搜索键2: " << (result2 ? *result2 : "未找到") << std::endl;

  // 删除操作
  dat.remove(3);
  std::cout << "删除键3后:" << std::endl;
  dat.print();

  std::cout << std::endl;
}

// 测试11.2 链接法散列表
void test_chained_hash_table() {
  std::cout << "=== 测试链接法散列表 (11.2) ===" << std::endl;

  // 使用除法散列法
  auto hash_func = [](int key) { return key; };
  ChainedHashTable<int, std::string> cht(7, hash_func);

  // 插入一些键值对
  std::vector<std::pair<int, std::string>> test_data = {
      {10, "Apple"},      {24, "Banana"}, {32, "Cherry"}, {17, "Date"},
      {31, "Elderberry"}, {14, "Fig"},    {45, "Grape"},  {28, "Honeydew"}};

  for (const auto &data : test_data) {
    cht.insert(data.first, data.second);
  }

  // 打印散列表
  cht.print();

  // 测试搜索
  auto search_result = cht.search(32);
  std::cout << "搜索键32: " << (search_result ? *search_result : "未找到")
            << std::endl;

  // 测试删除
  cht.remove(24);
  std::cout << "删除键24后:" << std::endl;
  cht.print();

  std::cout << std::endl;
}

// 测试11.3 散列函数
void test_hash_functions() {
  std::cout << "=== 测试散列函数 (11.3) ===" << std::endl;

  int table_size = 10;
  std::vector<int> test_keys = {123, 456, 789, 234, 567, 890, 345, 678, 901};

  std::cout << "除法散列法:" << std::endl;
  for (int key : test_keys) {
    int hash = HashFunctions::division_hash(key, table_size);
    std::cout << "键 " << key << " -> 散列值 " << hash << std::endl;
  }

  std::cout << "\n乘法散列法:" << std::endl;
  for (int key : test_keys) {
    int hash = HashFunctions::multiplication_hash(key, table_size);
    std::cout << "键 " << key << " -> 散列值 " << hash << std::endl;
  }

  std::cout << "\n全域散列法:" << std::endl;
  HashFunctions::UniversalHash universal_hash(97, table_size);
  for (int key : test_keys) {
    int hash = universal_hash(key);
    std::cout << "键 " << key << " -> 散列值 " << hash << std::endl;
  }

  std::cout << "\n字符串散列:" << std::endl;
  std::vector<std::string> string_keys = {"hello", "world", "hash", "table",
                                          "algorithm"};
  for (const auto &key : string_keys) {
    int hash = HashFunctions::string_hash(key, table_size);
    std::cout << "键 '" << key << "' -> 散列值 " << hash << std::endl;
  }

  std::cout << std::endl;
}

// 测试11.4 开放寻址法
void test_open_addressing() {
  std::cout << "=== 测试开放寻址法 (11.4) ===" << std::endl;

  // 使用线性探查
  auto linear_hash = [](int key, int i) {
    return (key + i) % 11; // 表大小为11
  };

  OpenAddressingHashTable<int, std::string> oaht(11, linear_hash);

  // 插入测试数据
  std::vector<std::pair<int, std::string>> test_data = {
      {10, "Alice"}, {22, "Bob"},   {31, "Charlie"}, {4, "David"}, {15, "Eve"},
      {28, "Frank"}, {17, "Grace"}, {88, "Henry"},   {59, "Ivy"}};

  for (const auto &data : test_data) {
    oaht.insert(data.first, data.second);
  }

  // 打印散列表
  oaht.print();

  // 测试搜索
  auto search_result = oaht.search(22);
  std::cout << "搜索键22: " << (search_result ? *search_result : "未找到")
            << std::endl;

  // 测试删除
  oaht.remove(22);
  std::cout << "删除键22后:" << std::endl;
  oaht.print();

  std::cout << std::endl;
}

// 测试11.5 完全散列
void test_perfect_hashing() {
  std::cout << "=== 测试完全散列 (11.5) ===" << std::endl;

  // 创建测试数据
  std::vector<std::pair<int, std::string>> test_data = {
      {10, "Apple"},      {20, "Banana"}, {30, "Cherry"}, {40, "Date"},
      {50, "Elderberry"}, {60, "Fig"},    {70, "Grape"},  {80, "Honeydew"}};

  PerfectHashTable<int, std::string> pht(test_data);

  // 打印完全散列表结构
  pht.print();

  // 测试搜索
  for (const auto &data : test_data) {
    auto result = pht.search(data.first);
    std::cout << "搜索键 " << data.first << ": "
              << (result ? *result : "未找到") << std::endl;
  }

  // 测试不存在的键
  auto not_found = pht.search(99);
  std::cout << "搜索不存在的键99: " << (not_found ? *not_found : "未找到")
            << std::endl;

  std::cout << std::endl;
}

// 测试散列函数分布
void test_hash_distribution() {
  std::cout << "=== 测试散列函数分布 ===" << std::endl;

  int table_size = 20;
  int num_keys = 100;

  std::cout << "除法散列法分布:" << std::endl;
  HashTableUtils::test_hash_distribution(
      [table_size](int key) {
        return HashFunctions::division_hash(key, table_size);
      },
      table_size, num_keys);

  std::cout << "\n乘法散列法分布:" << std::endl;
  HashTableUtils::test_hash_distribution(
      [table_size](int key) {
        return HashFunctions::multiplication_hash(key, table_size);
      },
      table_size, num_keys);

  std::cout << std::endl;
}

// 性能比较测试
void test_performance_comparison() {
  std::cout << "=== 性能比较测试 ===" << std::endl;

  // 生成测试操作序列
  std::vector<std::pair<int, std::string>> operations;
  int num_operations = 1000;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> key_dis(1, 1000);
  std::uniform_int_distribution<> op_dis(0, 2);

  for (int i = 0; i < num_operations; i++) {
    int key = key_dis(gen);
    std::string op;
    switch (op_dis(gen)) {
    case 0:
      op = "insert";
      break;
    case 1:
      op = "search";
      break;
    case 2:
      op = "delete";
      break;
    }
    operations.push_back({key, op});
  }

  // 测试链接法散列表性能
  auto chained_hash = [](int key) { return key; };
  ChainedHashTable<int, std::string> cht(101, chained_hash);

  std::cout << "链接法散列表性能:" << std::endl;
  HashTableUtils::performance_test(cht, operations);

  // 测试开放寻址法性能
  auto open_hash = [](int key, int i) { return (key + i) % 101; };
  OpenAddressingHashTable<int, std::string> oaht(101, open_hash);

  std::cout << "开放寻址法性能:" << std::endl;
  HashTableUtils::performance_test(oaht, operations);

  std::cout << std::endl;
}

// 测试算法导论示例
void test_clrs_examples() {
  std::cout << "=== 算法导论示例测试 ===" << std::endl;

  // 示例1: 图11.1 - 直接寻址表
  std::cout << "示例1 (图11.1 - 直接寻址表):" << std::endl;
  DirectAddressTable<int, std::string> example_dat(9);
  example_dat.insert(2, "卫星1");
  example_dat.insert(3, "卫星4");
  example_dat.insert(5, "卫星2");
  example_dat.insert(8, "卫星3");
  example_dat.print();

  // 示例2: 图11.2 - 链接法散列表
  std::cout << "\n示例2 (图11.2 - 链接法散列表):" << std::endl;
  auto simple_hash = [](int key) { return key % 9; };
  ChainedHashTable<int, std::string> example_cht(9, simple_hash);

  std::vector<std::pair<int, std::string>> example_data = {
      {5, "卫星1"},  {28, "卫星2"}, {19, "卫星3"}, {15, "卫星4"}, {20, "卫星5"},
      {33, "卫星6"}, {12, "卫星7"}, {17, "卫星8"}, {10, "卫星9"}};

  for (const auto &data : example_data) {
    example_cht.insert(data.first, data.second);
  }

  example_cht.print();

  // 示例3: 图11.5 - 开放寻址法
  std::cout << "\n示例3 (图11.5 - 开放寻址法):" << std::endl;
  auto example_open_hash = [](int key, int i) { return (key + i) % 11; };
  OpenAddressingHashTable<int, std::string> example_oaht(11, example_open_hash);

  std::vector<std::pair<int, std::string>> example_open_data = {
      {10, "A"}, {22, "B"}, {31, "C"}, {4, "D"}, {15, "E"},
      {28, "F"}, {17, "G"}, {88, "H"}, {59, "I"}};

  for (const auto &data : example_open_data) {
    example_oaht.insert(data.first, data.second);
  }

  example_oaht.print();

  std::cout << std::endl;
}

// 边界情况测试
void test_edge_cases() {
  std::cout << "=== 边界情况测试 ===" << std::endl;

  // 测试空表
  DirectAddressTable<int, std::string> empty_dat(5);
  std::cout << "空直接寻址表:" << std::endl;
  empty_dat.print();

  // 测试重复插入
  auto simple_hash = [](int key) { return key % 5; };
  ChainedHashTable<int, std::string> dup_cht(5, simple_hash);
  dup_cht.insert(1, "第一次");
  dup_cht.insert(1, "第二次");

  auto dup_result = dup_cht.search(1);
  std::cout << "重复插入后搜索键1: " << (dup_result ? *dup_result : "未找到")
            << std::endl;

  // 测试删除不存在的键
  bool remove_result = dup_cht.remove(99);
  std::cout << "删除不存在的键99: " << (remove_result ? "成功" : "失败")
            << std::endl;

  // 测试负载因子
  std::cout << "当前负载因子: " << dup_cht.load_factor() << std::endl;

  std::cout << std::endl;
}

int main() {
  std::cout << "算法导论第11章 散列表演示程序" << std::endl;
  std::cout << "================================" << std::endl;
  std::cout << std::endl;

  test_direct_address_table();
  test_chained_hash_table();
  test_hash_functions();
  test_open_addressing();
  test_perfect_hashing();
  test_hash_distribution();
  test_performance_comparison();
  test_clrs_examples();
  test_edge_cases();

  std::cout << "所有测试完成!" << std::endl;

  return 0;
}
