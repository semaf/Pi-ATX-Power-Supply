cmake_minimum_required(VERSION 3.10)

project(pac1934-test CXX)
find_package(Threads REQUIRED)
find_package(paho-mqttpp3 REQUIRED)
set(HEADER_FILES include/callbacktimer.h)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_executable(${PROJECT_NAME}
		src/main.cpp
		src/device.cpp
		${HEADER_FILES}
		) 
message(STATUS ${PAHO_CPP_LIB})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PAHO_CPP_LIB})


