cmake_minimum_required(VERSION 3.10)

project(mosquitto-cpp)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

######## install targets ########

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
    INCLUDES DESTINATION include
    )
