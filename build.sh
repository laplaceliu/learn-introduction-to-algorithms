#!/bin/bash

echo "=== 构建算法导论学习项目 ==="

# 创建构建目录
mkdir -p build
cd build

# 运行CMake配置
cmake ..

# 编译项目
make -j$(nproc)

echo "=== 构建完成 ==="
echo "可执行文件位于: build/bin/"
echo "运行示例: ./build/bin/heap_demo"