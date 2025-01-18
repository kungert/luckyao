cmake_minimum_required(VERSION 3.5)

project(luckyao)

# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include/)
# include_directories(${PROJECT_SOURCE_DIR}/include/base)
# include_directories(${PROJECT_SOURCE_DIR}/include/loop)
# include_directories(${PROJECT_SOURCE_DIR}/include/reflect)
# include_directories(${PROJECT_SOURCE_DIR}/include/tcp)
# include_directories(${PROJECT_SOURCE_DIR}/include/http)

# 定义参与编译的源代码文件 
aux_source_directory(${PROJECT_SOURCE_DIR}/src/base SRC_BASE)
 
aux_source_directory(${PROJECT_SOURCE_DIR}/src/loop SRC_LOOP)
  
aux_source_directory(${PROJECT_SOURCE_DIR}/src/reflect SRC_RELECT)
 
aux_source_directory(${PROJECT_SOURCE_DIR}/src/tcp SRC_TCP)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/web SRC_HTTP)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/json SRC_JSON)


add_library(luckyao SHARED 
    ${SRC_BASE}
    ${SRC_LOOP}
    ${SRC_RELECT}
    ${SRC_TCP}
    ${SRC_HTTP}
    ${SRC_JSON}
    )


add_subdirectory(examples)
add_subdirectory(LuckyApp)