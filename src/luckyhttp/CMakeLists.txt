# cmake_minimum_required(VERSION 3.5)


# 配置编译选项
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
# set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/luckyhttp/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/luckyhttp/src HTTP_SRC_LIST)

add_executable(luckyhttp  ${HTTP_SRC_LIST})

target_link_libraries(luckyhttp 
    luckyao
    pthread
)