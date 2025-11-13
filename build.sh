#!/bin/bash

echo "=== 构建算法导论学习项目 ==="

# 创建构建目录
mkdir -p build
cd build

# 运行CMake配置
cmake ..

# 检测系统并设置并行编译参数
if command -v nproc >/dev/null 2>&1; then
    # Linux 系统
    CPU_COUNT=$(nproc)
elif command -v sysctl >/dev/null 2>&1; then
    # macOS 系统
    CPU_COUNT=$(sysctl -n hw.ncpu)
else
    # 其他系统，默认使用2个核心
    CPU_COUNT=2
fi

echo "检测到系统 CPU 核心数: $CPU_COUNT"

# 编译项目
make -j$CPU_COUNT

echo "=== 构建完成 ==="
echo "可执行文件位于: build/bin/"
echo "运行示例: ./build/bin/heap_demo"