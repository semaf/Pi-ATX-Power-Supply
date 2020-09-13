cmake_minimum_required(VERSION 3.10)
project(powermeter  LANGUAGES CXX
	DESCRIPTION "Power meter cluster application"
	)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(mosquitto-cpp REQUIRED)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_executable(${PROJECT_NAME}
	main.cpp
	device.cpp
	) 

target_include_directories(${PROJECT_NAME} PRIVATE ../include)
target_link_libraries(${PROJECT_NAME} PRIVATE 
	Threads::Threads
	mosquitto
	mosquitto-cpp::mosquitto-cpp
	)


