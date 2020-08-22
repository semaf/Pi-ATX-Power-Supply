cmake_minimum_required(VERSION 3.10)

project(pac1934-test C)
find_library(LIBIIO libiio.so)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DIIO_CHECK_RET")
add_executable(${PROJECT_NAME} main.c) 

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBIIO})

