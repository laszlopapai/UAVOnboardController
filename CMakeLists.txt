cmake_minimum_required(VERSION 3.5)
project(UAV)

set(CMAKE_CXX_STANDARD 17)

set(GCC_READABLE_SYMBOLE "-rdynamic")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_READABLE_SYMBOLE}")

## The UAVController project source files
set(UAV_SOURCE_FILES
	Application.cpp
	Application.h

	UAV/AircraftApp.cpp
	UAV/AircraftApp.h

	UAV/ConnectionHandler.cpp
	UAV/ConnectionHandler.h
		
	UAV/DeviceHandler.cpp
	UAV/DeviceHandler.h

	UAV/Logger.cpp
	UAV/Logger.h

	#UAV/SensorReferenceSystemA.cpp
	#UAV/SensorReferenceSystemA.h

	UAV/SensorReferenceSystemB.cpp
	UAV/SensorReferenceSystemB.h

	UAV/Pilot.cpp
	UAV/Pilot.h
)

set(UAV_INTERACT_SOURCE_FILES
	UAV/Interact/Gyroscope.cpp
	UAV/Interact/Gyroscope.h
	UAV/Interact/Accelerometer.cpp
	UAV/Interact/Accelerometer.h
	UAV/Interact/Magnetometer.cpp
	UAV/Interact/Magnetometer.h
	UAV/Interact/PulseWidthModulator.cpp
	UAV/Interact/PulseWidthModulator.h
	UAV/Interact/Barometer.cpp
	UAV/Interact/Barometer.h
)

set(UAV_STRUCTURE_SOURCE_FILES
	UAV/Structures/RemoteIn.h
	UAV/Structures/RemoteOut.h
	UAV/Structures/PulseWidthSet.h
	UAV/Structures/RemoteCameraOut.h
	UAV/Structures/PulseWidthSet.cpp
)

set(UAV_AIRCRAFTS_SOURCE_FILES
	UAV/Aircrafts/IAircraft.h
	UAV/Aircrafts/Quadrocopter.h
	UAV/Aircrafts/Quadrocopter.cpp
)

source_group("" FILES ${UAV_SOURCE_FILES})
source_group("Interacts" FILES ${UAV_INTERACT_SOURCE_FILES})
source_group("Analyze" FILES ${UAV_ANANLYZE_SOURCE_FILES})
source_group("Structures" FILES ${UAV_STRUCTURE_SOURCE_FILES})
source_group("Aircrafts" FILES ${UAV_AIRCRAFTS_SOURCE_FILES})

## OpenCV ivoke into the project
set(OpenCV_DIR 3rdparty/.CMakeFiles/opencv)
find_package( OpenCV REQUIRED )

## WiringPi include into the project
IF(NOT WIRINGPI_FOUND)
	FIND_LIBRARY(wiringPi_LIBS NAMES libwiringPi.so PATHS "${PROJECT_SOURCE_DIR}/3rdparty/.CMakeBuild/wiringPi/lib" DOC "WiringPi library")
	FIND_PATH(wiringPi_INCLUDE_DIRS wiringPi.h "${PROJECT_SOURCE_DIR}/3rdparty/.CMakeBuild/wiringPi/include" DOC "WiringPi includes")
	include_directories("${PROJECT_SOURCE_DIR}/3rdparty/.CMakeBuild/wiringPi/include")
ENDIF(NOT WIRINGPI_FOUND)

## UDPRemote Include into the project // As its mine project that is separated from the superproject
include_directories(Devices)
add_subdirectory(${PROJECT_SOURCE_DIR}/Devices)
include_directories(Core)
add_subdirectory(${PROJECT_SOURCE_DIR}/Core)
add_subdirectory(${PROJECT_SOURCE_DIR}/Test)


include_directories(${PROJECT_SOURCE_DIR}/UAV)
add_executable(
	UAV

	${UAV_SOURCE_FILES}
	${UAV_INTERACT_SOURCE_FILES}
	${UAV_STRUCTURE_SOURCE_FILES}
	${UAV_AIRCRAFTS_SOURCE_FILES}
)
## Link the libraries above together for compiler
target_link_libraries(
	UAV

	${wiringPi_LIBS}
	pthread
	crypt
	rt
	m
	${OpenCV_LIBS}
	IoTDevices
	IoTCore
)
