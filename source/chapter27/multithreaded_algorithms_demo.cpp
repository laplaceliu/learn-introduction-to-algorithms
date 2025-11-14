#include "multithreaded_algorithms.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

/**
 * @brief 性能测试函数
 * @param test_name 测试名称
 * @param test_func 测试函数
 * @return 执行时间（微秒）
 */
long long performance_test(const std::string &test_name,
                           std::function<void()> test_func) {
  auto start = std::chrono::high_resolution_clock::now();
  test_func();
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << test_name << " - 时间: " << duration.count() << " 微秒"
            << std::endl;

  return duration.count();
}

/**
 * @brief 测试多线程矩阵乘法
 */
void test_matrix_multiplication() {
  std::cout << "=== 多线程矩阵乘法测试 ===" << std::endl;

  // 生成测试矩阵
  size_t n = 100; // 矩阵大小
  auto A = MultithreadedAlgorithms::generate_random_matrix(n, n);
  auto B = MultithreadedAlgorithms::generate_random_matrix(n, n);

  std::cout << "矩阵大小: " << n << " x " << n << std::endl;

  // 单线程矩阵乘法
  long long single_thread_time = performance_test("单线程矩阵乘法", [&]() {
    auto C_single =
        MultithreadedAlgorithms::multithreaded_matrix_multiply(A, B, 1);
    bool verified =
        MultithreadedAlgorithms::verify_matrix_multiply(A, B, C_single);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  });

  // 多线程矩阵乘法（使用所有可用核心）
  unsigned int num_cores = std::thread::hardware_concurrency();
  std::cout << "可用CPU核心数: " << num_cores << std::endl;

  long long multi_thread_time = performance_test("多线程矩阵乘法", [&]() {
    auto C_multi =
        MultithreadedAlgorithms::multithreaded_matrix_multiply(A, B, num_cores);
    bool verified =
        MultithreadedAlgorithms::verify_matrix_multiply(A, B, C_multi);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  });

  // 计算加速比
  double speedup = static_cast<double>(single_thread_time) / multi_thread_time;
  std::cout << "加速比: " << std::fixed << std::setprecision(2) << speedup
            << "x" << std::endl;

  // 测试不同线程数的性能
  std::cout << "\n不同线程数性能测试:" << std::endl;
  for (size_t threads : {1, 2, 4, 8}) {
    if (threads <= num_cores) {
      performance_test("线程数 " + std::to_string(threads), [&]() {
        auto C = MultithreadedAlgorithms::multithreaded_matrix_multiply(
            A, B, threads);
      });
    }
  }
}

/**
 * @brief 测试多线程归并排序
 */
void test_merge_sort() {
  std::cout << "\n=== 多线程归并排序测试 ===" << std::endl;

  // 生成测试数据
  size_t size = 10000;
  auto arr = MultithreadedAlgorithms::generate_random_array(size);
  auto arr_copy = arr; // 备份用于单线程测试

  std::cout << "数组大小: " << size << std::endl;

  // 单线程归并排序
  long long single_thread_time = performance_test("单线程归并排序", [&]() {
    std::sort(arr_copy.begin(), arr_copy.end());
    bool verified = MultithreadedAlgorithms::verify_sort(arr_copy);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  });

  // 多线程归并排序
  unsigned int num_cores = std::thread::hardware_concurrency();
  long long multi_thread_time = performance_test("多线程归并排序", [&]() {
    MultithreadedAlgorithms::multithreaded_merge_sort(arr, num_cores);
    bool verified = MultithreadedAlgorithms::verify_sort(arr);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  });

  // 计算加速比
  double speedup = static_cast<double>(single_thread_time) / multi_thread_time;
  std::cout << "加速比: " << std::fixed << std::setprecision(2) << speedup
            << "x" << std::endl;

  // 验证排序结果正确性
  bool sort_correct = true;
  for (size_t i = 1; i < arr.size(); ++i) {
    if (arr[i] < arr[i - 1]) {
      sort_correct = false;
      break;
    }
  }
  std::cout << "排序正确性验证: " << (sort_correct ? "通过" : "失败")
            << std::endl;
}

/**
 * @brief 测试多线程快速排序
 */
void test_quick_sort() {
  std::cout << "\n=== 多线程快速排序测试 ===" << std::endl;

  // 生成测试数据
  size_t size = 10000;
  auto arr = MultithreadedAlgorithms::generate_random_array(size);
  auto arr_copy = arr; // 备份用于单线程测试

  std::cout << "数组大小: " << size << std::endl;

  // 单线程快速排序
  long long single_thread_time = performance_test("单线程快速排序", [&]() {
    std::sort(arr_copy.begin(), arr_copy.end());
    bool verified = MultithreadedAlgorithms::verify_sort(arr_copy);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  });

  // 多线程快速排序
  unsigned int num_cores = std::thread::hardware_concurrency();
  long long multi_thread_time = performance_test("多线程快速排序", [&]() {
    MultithreadedAlgorithms::multithreaded_quick_sort(arr, num_cores);
    bool verified = MultithreadedAlgorithms::verify_sort(arr);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  });

  // 计算加速比
  double speedup = static_cast<double>(single_thread_time) / multi_thread_time;
  std::cout << "加速比: " << std::fixed << std::setprecision(2) << speedup
            << "x" << std::endl;

  // 验证排序结果正确性
  bool sort_correct = true;
  for (size_t i = 1; i < arr.size(); ++i) {
    if (arr[i] < arr[i - 1]) {
      sort_correct = false;
      break;
    }
  }
  std::cout << "排序正确性验证: " << (sort_correct ? "通过" : "失败")
            << std::endl;
}

/**
 * @brief 测试并行前缀和
 */
void test_prefix_sum() {
  std::cout << "\n=== 并行前缀和测试 ===" << std::endl;

  // 生成测试数据
  size_t size = 10000;
  auto arr = MultithreadedAlgorithms::generate_random_array(size, 1, 10);

  std::cout << "数组大小: " << size << std::endl;

  // 串行前缀和
  long long single_thread_time = performance_test("串行前缀和", [&]() {
    std::vector<int> prefix_sum_serial(arr.size());
    int sum = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
      sum += arr[i];
      prefix_sum_serial[i] = sum;
    }
    bool verified =
        MultithreadedAlgorithms::verify_prefix_sum(arr, prefix_sum_serial);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  });

  // 并行前缀和
  unsigned int num_cores = std::thread::hardware_concurrency();
  long long multi_thread_time = performance_test("并行前缀和", [&]() {
    auto prefix_sum_parallel =
        MultithreadedAlgorithms::parallel_prefix_sum(arr, num_cores);
    bool verified =
        MultithreadedAlgorithms::verify_prefix_sum(arr, prefix_sum_parallel);
    std::cout << "  结果验证: " << (verified ? "通过" : "失败") << std::endl;
  });

  // 计算加速比
  double speedup = static_cast<double>(single_thread_time) / multi_thread_time;
  std::cout << "加速比: " << std::fixed << std::setprecision(2) << speedup
            << "x" << std::endl;

  // 验证结果正确性
  auto prefix_sum_parallel =
      MultithreadedAlgorithms::parallel_prefix_sum(arr, num_cores);
  bool prefix_correct =
      MultithreadedAlgorithms::verify_prefix_sum(arr, prefix_sum_parallel);
  std::cout << "前缀和正确性验证: " << (prefix_correct ? "通过" : "失败")
            << std::endl;
}

/**
 * @brief 测试线程池功能
 */
void test_thread_pool() {
  std::cout << "\n=== 线程池功能测试 ===" << std::endl;

  // 创建线程池
  unsigned int num_cores = std::thread::hardware_concurrency();
  MultithreadedAlgorithms::ThreadPool pool(num_cores);

  std::cout << "线程池大小: " << num_cores << std::endl;

  // 测试并行执行多个任务
  std::vector<std::future<int>> futures;
  int num_tasks = 10;

  std::cout << "提交 " << num_tasks << " 个任务到线程池" << std::endl;

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num_tasks; ++i) {
    futures.emplace_back(pool.enqueue([i] {
      // 模拟计算任务
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      return i * i;
    }));
  }

  // 等待所有任务完成并收集结果
  std::vector<int> results;
  for (auto &future : futures) {
    results.push_back(future.get());
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "所有任务完成时间: " << duration.count() << " 毫秒" << std::endl;

  // 验证结果
  bool results_correct = true;
  for (int i = 0; i < num_tasks; ++i) {
    if (results[i] != i * i) {
      results_correct = false;
      break;
    }
  }

  std::cout << "任务结果验证: " << (results_correct ? "通过" : "失败")
            << std::endl;
}

/**
 * @brief 测试不同数据规模的性能
 */
void test_scalability() {
  std::cout << "\n=== 可扩展性测试 ===" << std::endl;

  unsigned int num_cores = std::thread::hardware_concurrency();

  // 测试不同规模的矩阵乘法
  std::vector<size_t> matrix_sizes = {50, 100, 200, 400};

  std::cout << "矩阵乘法可扩展性测试:" << std::endl;
  for (size_t size : matrix_sizes) {
    auto A = MultithreadedAlgorithms::generate_random_matrix(size, size);
    auto B = MultithreadedAlgorithms::generate_random_matrix(size, size);

    std::cout << "\n矩阵大小: " << size << " x " << size << std::endl;

    long long single_time = performance_test("单线程", [&]() {
      auto C = MultithreadedAlgorithms::multithreaded_matrix_multiply(A, B, 1);
    });

    long long multi_time = performance_test("多线程", [&]() {
      auto C = MultithreadedAlgorithms::multithreaded_matrix_multiply(
          A, B, num_cores);
    });

    double speedup = static_cast<double>(single_time) / multi_time;
    std::cout << "加速比: " << std::fixed << std::setprecision(2) << speedup
              << "x" << std::endl;
  }

  // 测试不同规模的排序
  std::vector<size_t> sort_sizes = {1000, 5000, 10000, 20000};

  std::cout << "\n排序可扩展性测试:" << std::endl;
  for (size_t size : sort_sizes) {
    auto arr = MultithreadedAlgorithms::generate_random_array(size);

    std::cout << "\n数组大小: " << size << std::endl;

    long long single_time = performance_test(
        "单线程", [&]() { std::sort(arr.begin(), arr.end()); });

    long long multi_time = performance_test("多线程", [&]() {
      MultithreadedAlgorithms::multithreaded_merge_sort(arr, num_cores);
    });

    double speedup = static_cast<double>(single_time) / multi_time;
    std::cout << "加速比: " << std::fixed << std::setprecision(2) << speedup
              << "x" << std::endl;
  }
}

int main() {
  std::cout << "=== 算法导论 第27章 - 多线程算法演示 ===" << std::endl;
  std::cout << "检测到CPU核心数: " << std::thread::hardware_concurrency()
            << std::endl;
  std::cout << std::endl;

  try {
    // 测试线程池功能
    test_thread_pool();

    // 测试多线程矩阵乘法
    test_matrix_multiplication();

    // 测试多线程归并排序
    test_merge_sort();

    // 测试多线程快速排序
    test_quick_sort();

    // 测试并行前缀和
    test_prefix_sum();

    // 测试可扩展性
    test_scalability();

    std::cout << "\n=== 测试完成 ===" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "测试过程中发生错误: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
