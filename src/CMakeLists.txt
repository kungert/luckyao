cmake_minimum_required(VERSION 3.5)

project(luckyao)

# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 定义参与编译的源代码文件 
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)


add_library(luckyao SHARED ${SRC_LIST})


add_subdirectory(examples)