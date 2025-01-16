# cmake_minimum_required(VERSION 3.5)


# 配置编译选项
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
# set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/LuckyApp/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/LuckyApp/src HTTP_SRC_LIST)

add_executable(LuckyApp  ${HTTP_SRC_LIST})

target_link_libraries(LuckyApp 
    luckyao
    pthread
)