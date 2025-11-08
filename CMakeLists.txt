cmake_minimum_required(VERSION 3.10)
project(learn-introduction-to-algorithms)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)

# 查找所有源文件
file(GLOB_RECURSE SOURCE_FILES "source/*.cpp")

# 为每个源文件创建可执行程序
foreach(source_file ${SOURCE_FILES})
    # 获取文件名（不含扩展名）
    get_filename_component(executable_name ${source_file} NAME_WE)
    
    # 创建可执行文件
    add_executable(${executable_name} ${source_file})
    
    # 设置输出目录
    set_target_properties(${executable_name} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()