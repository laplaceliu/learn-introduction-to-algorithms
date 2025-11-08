# 算法导论学习项目

这是一个基于CMake的C++项目，用于学习和实现《算法导论》中的经典算法和数据结构。

## 项目特点

- **模块化设计**: 每个算法/数据结构都有独立的实现和演示程序
- **标准实现**: 严格按照《算法导论》的算法描述和操作规范实现
- **完整测试**: 每个实现都包含详细的演示程序和边界情况测试
- **现代C++**: 使用C++17标准，包含智能指针、模板等现代特性

## 目录结构

```
learn-introduction-to-algorithms/
├── CMakeLists.txt          # CMake配置文件
├── build.sh                # 构建脚本
├── README.md               # 项目说明文档
├── include/                 # 扁平结构头文件目录
│   ├── heap.h              # 6.1节堆数据结构
│   ├── priority_queue.h    # 6.4节优先队列
│   ├── stack_queue.h       # 10.1节栈和队列
│   ├── linked_list.h       # 10.2节链表
│   └── rooted_tree.h       # 10.4节有根树
└── source/                 # 章节结构源文件目录
    ├── chapter6/
    │   ├── heap_demo.cpp              # 6.1节堆演示程序
    │   └── priority_queue_demo.cpp    # 6.4节优先队列演示程序
    └── chapter10/
        ├── stack_queue_demo.cpp       # 10.1节栈和队列演示程序
        ├── linked_list_demo.cpp       # 10.2节链表演示程序
        └── rooted_tree_demo.cpp       # 10.4节有根树演示程序
```

## 已实现内容

### 第6章 堆排序

#### 6.1节 堆
- **最大堆实现**: 基于数组的最大堆数据结构
- **核心操作**:
  - `max_heapify`: 维护堆性质
  - `build_max_heap`: 构建最大堆
  - 堆排序算法
- **功能特性**:
  - 插入元素（上浮操作）
  - 提取最大值
  - 堆排序
  - 完整的错误处理

#### 6.4节 优先队列
- **基本优先队列**: 基于最大堆的优先队列
- **增强版优先队列**: 支持INCREASE-KEY操作
- **标准操作**:
  - `INSERT(S, x)`: 插入元素
  - `MAXIMUM(S)`: 返回最大元素
  - `EXTRACT-MAX(S)`: 提取并删除最大元素
  - `INCREASE-KEY(S, x, k)`: 增加元素的关键字

### 第10章 基本数据结构

#### 10.1节 栈和队列
- **栈实现**: 基于数组的栈数据结构
  - `PUSH(S, x)`: 压入元素
  - `POP(S)`: 弹出元素
  - `STACK-EMPTY(S)`: 检查栈是否为空
- **队列实现**: 基于循环数组的队列
  - `ENQUEUE(Q, x)`: 入队
  - `DEQUEUE(Q)`: 出队
  - `QUEUE-EMPTY(Q)`: 检查队列是否为空
- **扩展实现**:
  - 双端队列（Deque）
  - 用两个栈实现队列（练习10.1-6）

#### 10.2节 链表
- **单向链表**: 支持标准链表操作
  - `LIST-SEARCH(L, k)`: 搜索包含关键字k的元素
  - `LIST-INSERT(L, x)`: 在链表前端插入元素
  - `LIST-DELETE(L, x)`: 删除元素x
- **双向链表**: 支持双向遍历和操作
- **循环链表**: 尾节点指向头节点的循环结构

#### 10.4节 有根树的表示
- **通用有根树**: 支持任意分支的有根树结构
  - 节点包含父指针和子节点向量
  - 深度和高度计算
  - 叶子节点判断
- **遍历算法**:
  - 前序遍历
  - 后序遍历
  - 层序遍历（广度优先）
- **搜索算法**:
  - 广度优先搜索（BFS）
  - 深度优先搜索（DFS）
- **二叉树特例**: 作为有根树的特例实现
  - 左子树和右子树支持
  - 中序遍历、前序遍历、后序遍历

## 构建和运行

### 环境要求
- CMake 3.10+
- 支持C++17的编译器（GCC 7+, Clang 5+, MSVC 2017+）

### 构建项目
```bash
# 使用构建脚本（推荐）
./build.sh

# 或者手动构建
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 运行演示程序
```bash
# 堆演示
./build/bin/heap_demo

# 优先队列演示
./build/bin/priority_queue_demo

# 栈和队列演示
./build/bin/stack_queue_demo

# 链表演示
./build/bin/linked_list_demo

# 有根树演示
./build/bin/rooted_tree_demo
```
