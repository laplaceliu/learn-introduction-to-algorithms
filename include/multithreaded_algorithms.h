#ifndef MULTITHREADED_ALGORITHMS_H
#define MULTITHREADED_ALGORITHMS_H

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
#include <thread>
#include <vector>

namespace algorithms {

/**
 * @brief 多线程算法实现
 *
 * 实现算法导论第27章多线程算法，包括：
 * - 多线程矩阵乘法
 * - 多线程归并排序
 * - 多线程快速排序
 * - 线程池实现
 * - 并行前缀和
 */
class MultithreadedAlgorithms {
public:
  /**
   * @brief 线程池类
   */
  class ThreadPool {
  private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

  public:
    /**
     * @brief 构造函数
     * @param num_threads 线程数量
     */
    ThreadPool(size_t num_threads) : stop(false) {
      for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this] {
          while (true) {
            std::function<void()> task;
            {
              std::unique_lock<std::mutex> lock(this->queue_mutex);
              this->condition.wait(
                  lock, [this] { return this->stop || !this->tasks.empty(); });

              if (this->stop && this->tasks.empty()) {
                return;
              }

              task = std::move(this->tasks.front());
              this->tasks.pop();
            }

            task();
          }
        });
      }
    }

    /**
     * @brief 添加任务到线程池
     * @tparam F 函数类型
     * @tparam Args 参数类型
     * @param f 函数
     * @param args 参数
     * @return std::future 用于获取结果
     */
    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args)
        -> std::future<typename std::invoke_result_t<F, Args...>> {
      using return_type = typename std::invoke_result_t<F, Args...>;

      auto task = std::make_shared<std::packaged_task<return_type()>>(
          std::bind(std::forward<F>(f), std::forward<Args>(args)...));

      std::future<return_type> result = task->get_future();
      {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop) {
          throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks.emplace([task]() { (*task)(); });
      }

      condition.notify_one();
      return result;
    }

    /**
     * @brief 析构函数
     */
    ~ThreadPool() {
      {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
      }

      condition.notify_all();
      for (std::thread &worker : workers) {
        worker.join();
      }
    }
  };

  /**
   * @brief 多线程矩阵乘法 - 算法导论第27.1节
   *
   * 使用分治法的多线程矩阵乘法
   *
   * @param A 第一个矩阵
   * @param B 第二个矩阵
   * @param num_threads 线程数量
   * @return 乘积矩阵
   */
  static std::vector<std::vector<double>> multithreaded_matrix_multiply(
      const std::vector<std::vector<double>> &A,
      const std::vector<std::vector<double>> &B,
      size_t num_threads = std::thread::hardware_concurrency()) {

    size_t n = A.size();
    size_t m = B.size();
    size_t p = B[0].size();

    if (A[0].size() != m) {
      throw std::invalid_argument("矩阵维度不匹配");
    }

    std::vector<std::vector<double>> C(n, std::vector<double>(p, 0.0));

    ThreadPool pool(num_threads);
    std::vector<std::future<void>> futures;

    // 将矩阵乘法任务分块
    size_t block_size = (n + num_threads - 1) / num_threads;

    for (size_t thread_id = 0; thread_id < num_threads; ++thread_id) {
      size_t start_row = thread_id * block_size;
      size_t end_row = std::min(start_row + block_size, n);

      if (start_row >= end_row)
        break;

      futures.emplace_back(
          pool.enqueue([&A, &B, &C, start_row, end_row, n, m, p] {
            for (size_t i = start_row; i < end_row; ++i) {
              for (size_t j = 0; j < p; ++j) {
                double sum = 0.0;
                for (size_t k = 0; k < m; ++k) {
                  sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
              }
            }
          }));
    }

    // 等待所有任务完成
    for (auto &future : futures) {
      future.get();
    }

    return C;
  }

  /**
   * @brief 多线程归并排序 - 算法导论第27.3节
   *
   * 使用分治法的多线程归并排序
   *
   * @param arr 待排序数组
   * @param num_threads 线程数量
   */
  static void multithreaded_merge_sort(
      std::vector<int> &arr,
      size_t num_threads = std::thread::hardware_concurrency()) {
    if (arr.size() <= 1)
      return;

    ThreadPool pool(num_threads);
    std::atomic<size_t> active_tasks{0};
    std::mutex task_mutex;
    std::condition_variable task_cv;

    // 递归排序函数
    std::function<void(size_t, size_t)> sort_range = [&](size_t left,
                                                         size_t right) {
      if (left >= right)
        return;

      size_t mid = left + (right - left) / 2;

      // 使用更保守的阈值控制，避免递归深度过大
      if (right - left > 5000 && active_tasks.load() < num_threads * 2) {
        {
          std::unique_lock<std::mutex> lock(task_mutex);
          active_tasks += 2;
        }

        auto left_future = pool.enqueue([&, left, mid] {
          sort_range(left, mid);
          {
            std::unique_lock<std::mutex> lock(task_mutex);
            active_tasks--;
          }
          task_cv.notify_one();
        });

        auto right_future = pool.enqueue([&, mid, right] {
          sort_range(mid + 1, right);
          {
            std::unique_lock<std::mutex> lock(task_mutex);
            active_tasks--;
          }
          task_cv.notify_one();
        });

        left_future.get();
        right_future.get();
      } else {
        // 串行执行，避免死锁
        sort_range(left, mid);
        sort_range(mid + 1, right);
      }

      // 合并两个有序子数组
      std::vector<int> temp(right - left + 1);
      size_t i = left, j = mid + 1, k = 0;

      while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
          temp[k++] = arr[i++];
        } else {
          temp[k++] = arr[j++];
        }
      }

      while (i <= mid)
        temp[k++] = arr[i++];
      while (j <= right)
        temp[k++] = arr[j++];

      for (size_t idx = 0; idx < k; ++idx) {
        arr[left + idx] = temp[idx];
      }
    };

    sort_range(0, arr.size() - 1);
  }

  /**
   * @brief 多线程快速排序 - 算法导论第27.3节
   *
   * 使用分治法的多线程快速排序
   *
   * @param arr 待排序数组
   * @param num_threads 线程数量
   */
  static void multithreaded_quick_sort(
      std::vector<int> &arr,
      size_t num_threads = std::thread::hardware_concurrency()) {
    if (arr.size() <= 1)
      return;

    ThreadPool pool(num_threads);
    std::atomic<size_t> active_tasks{0};
    std::mutex task_mutex;
    std::condition_variable task_cv;

    // 递归排序函数
    std::function<void(size_t, size_t)> quick_sort_range = [&](size_t low,
                                                               size_t high) {
      if (low >= high)
        return;

      // 选择最后一个元素作为枢轴
      int pivot = arr[high];
      size_t i = low;

      for (size_t j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
          std::swap(arr[i], arr[j]);
          ++i;
        }
      }

      std::swap(arr[i], arr[high]);

      size_t pivot_index = i;

      // 并行处理左右子数组
      if (high - low > 5000 && active_tasks.load() < num_threads * 2) {
        {
          std::unique_lock<std::mutex> lock(task_mutex);
          active_tasks += 2;
        }

        auto left_future = pool.enqueue([&, low, pivot_index] {
          quick_sort_range(low, pivot_index - 1);
          {
            std::unique_lock<std::mutex> lock(task_mutex);
            active_tasks--;
          }
          task_cv.notify_one();
        });

        auto right_future = pool.enqueue([&, pivot_index, high] {
          quick_sort_range(pivot_index + 1, high);
          {
            std::unique_lock<std::mutex> lock(task_mutex);
            active_tasks--;
          }
          task_cv.notify_one();
        });

        left_future.get();
        right_future.get();
      } else {
        // 串行执行，避免死锁
        quick_sort_range(low, pivot_index - 1);
        quick_sort_range(pivot_index + 1, high);
      }
    };

    quick_sort_range(0, arr.size() - 1);
  }

  /**
   * @brief 并行前缀和 - 算法导论第27.2节
   *
   * 使用多线程计算前缀和
   *
   * @param arr 输入数组
   * @param num_threads 线程数量
   * @return 前缀和数组
   */
  static std::vector<int> parallel_prefix_sum(
      const std::vector<int> &arr,
      size_t num_threads = std::thread::hardware_concurrency()) {
    if (arr.empty())
      return {};

    size_t n = arr.size();
    std::vector<int> prefix_sum(n);

    if (n <= 1) {
      if (n == 1)
        prefix_sum[0] = arr[0];
      return prefix_sum;
    }

    ThreadPool pool(num_threads);

    // 第一步：计算每个块的前缀和
    size_t block_size = (n + num_threads - 1) / num_threads;
    std::vector<int> block_sums(num_threads, 0);
    std::vector<std::future<void>> futures;

    for (size_t thread_id = 0; thread_id < num_threads; ++thread_id) {
      size_t start = thread_id * block_size;
      size_t end = std::min(start + block_size, n);

      if (start >= end)
        break;

      futures.emplace_back(
          pool.enqueue([&arr, &prefix_sum, &block_sums, thread_id, start, end] {
            int sum = 0;
            for (size_t i = start; i < end; ++i) {
              sum += arr[i];
              prefix_sum[i] = sum;
            }
            block_sums[thread_id] = sum;
          }));
    }

    for (auto &future : futures) {
      future.get();
    }

    // 第二步：计算块的前缀和
    for (size_t i = 1; i < num_threads; ++i) {
      block_sums[i] += block_sums[i - 1];
    }

    // 第三步：将块的前缀和加到每个块的元素上
    futures.clear();

    for (size_t thread_id = 1; thread_id < num_threads; ++thread_id) {
      size_t start = thread_id * block_size;
      size_t end = std::min(start + block_size, n);

      if (start >= end)
        break;

      int block_prefix = block_sums[thread_id - 1];

      futures.emplace_back(
          pool.enqueue([&prefix_sum, block_prefix, start, end] {
            for (size_t i = start; i < end; ++i) {
              prefix_sum[i] += block_prefix;
            }
          }));
    }

    for (auto &future : futures) {
      future.get();
    }

    return prefix_sum;
  }

  /**
   * @brief 生成随机矩阵
   * @param rows 行数
   * @param cols 列数
   * @param min_val 最小值
   * @param max_val 最大值
   * @return 随机矩阵
   */
  static std::vector<std::vector<double>>
  generate_random_matrix(size_t rows, size_t cols, double min_val = 0.0,
                         double max_val = 10.0) {
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min_val, max_val);

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        matrix[i][j] = dis(gen);
      }
    }

    return matrix;
  }

  /**
   * @brief 生成随机数组
   * @param size 数组大小
   * @param min_val 最小值
   * @param max_val 最大值
   * @return 随机数组
   */
  static std::vector<int> generate_random_array(size_t size, int min_val = 0,
                                                int max_val = 100) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min_val, max_val);

    for (size_t i = 0; i < size; ++i) {
      arr[i] = dis(gen);
    }

    return arr;
  }

  /**
   * @brief 验证矩阵乘法结果
   * @param A 第一个矩阵
   * @param B 第二个矩阵
   * @param C 乘积矩阵
   * @return 验证结果
   */
  static bool
  verify_matrix_multiply(const std::vector<std::vector<double>> &A,
                         const std::vector<std::vector<double>> &B,
                         const std::vector<std::vector<double>> &C) {
    size_t n = A.size();
    size_t m = B.size();
    size_t p = B[0].size();

    if (C.size() != n || C[0].size() != p) {
      return false;
    }

    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < p; ++j) {
        double expected = 0.0;
        for (size_t k = 0; k < m; ++k) {
          expected += A[i][k] * B[k][j];
        }

        if (std::abs(C[i][j] - expected) > 1e-6) {
          return false;
        }
      }
    }

    return true;
  }

  /**
   * @brief 验证排序结果
   * @param arr 排序后的数组
   * @return 验证结果
   */
  static bool verify_sort(const std::vector<int> &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
      if (arr[i] < arr[i - 1]) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief 验证前缀和结果
   * @param arr 原数组
   * @param prefix_sum 前缀和数组
   * @return 验证结果
   */
  static bool verify_prefix_sum(const std::vector<int> &arr,
                                const std::vector<int> &prefix_sum) {
    if (arr.size() != prefix_sum.size()) {
      return false;
    }

    int sum = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
      sum += arr[i];
      if (prefix_sum[i] != sum) {
        return false;
      }
    }

    return true;
  }
};

} // namespace algorithms

#endif // MULTITHREADED_ALGORITHMS_H
