cmake_minimum_required(VERSION 3.5)
project(UAVControllerTest)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
IF(NOT GTEST_FOUND)
	FIND_LIBRARY(GTEST_LIBS NAMES libgtest.a PATHS ${PROJECT_SOURCE_DIR}/../3rdparty/.CMakeBuild/googletest/lib DOC "GoogleTest library")
	FIND_PATH(GTEST_INCLUDE_DIRS gtest/gtest.h ${PROJECT_SOURCE_DIR}/../3rdparty/.CMakeBuild/googletest/include DOC "GoogleTest includes")
ENDIF(NOT GTEST_FOUND)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/../3rdparty/googletest/googletest/include)

set(SOURCE_FILES
	test.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/../Core)
include_directories(${PROJECT_SOURCE_DIR}/../Devices)
include_directories(${PROJECT_SOURCE_DIR}/../UAV)
include_directories(${PROJECT_SOURCE_DIR}/../Test)

add_executable(UAVControllerTest ${SOURCE_FILES})


target_link_libraries(
	UAVControllerTest
	
	${GTEST_LIBS}
	crypt
	pthread
	${wiringPi_LIBS}
	IoTDevices
	IoTCore
)
