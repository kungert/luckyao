# cmake_minimum_required(VERSION 3.5)
# project(examples)

# cmake => makefile   make
# luckyao最终编译成so动态库，设置动态库的路径，放在根目录的lib文件夹下面
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置调试信息 以及 启动C++11语言标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(./ SRC_LIST)

add_executable(example  ${SRC_LIST})

target_link_libraries(example 
    luckyao
)