cmake_minimum_required(VERSION 3.10)

project(powermeter  LANGUAGES CXX
	DESCRIPTION "Power meter cluster application"
	)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(mosquitto-cpp REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)
find_package(jsoncpp REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(${PROJECT_NAME}
	main.cpp
	DeviceReader.cpp
	MosquittoHub.cpp
	) 

target_include_directories(${PROJECT_NAME} PRIVATE ../include)

target_link_libraries(${PROJECT_NAME} PRIVATE 
	Threads::Threads
	mosquitto
	mosquitto-cpp::mosquitto-cpp
	Boost::system
	Boost::filesystem
	jsoncpp
	)


