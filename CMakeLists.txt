
cmake_minimum_required(VERSION 3.12) 

project(gUnittest)
# 相对路径
add_subdirectory(googletest) 

include_directories(googletest/include) 

# 添加一个测试：名称和可执行文件
add_executable(test test.cpp) 

target_link_libraries(test PRIVATE gtest) 

