cmake_minimum_required(VERSION 3.5)
project(IoTDevices)

set(CMAKE_CXX_STANDARD 17)

set(CAMERA_SOURCE_FILES
	Camera/CameraCapture.cpp
	Camera/CameraCapture.h
)

set(PCA9685_SOURCE_FILES
	PCA9685/PCA9685.cpp
	PCA9685/PCA9685.h
	PCA9685/PCA9685PwmController.cpp
	PCA9685/PCA9685PwmController.h
)

set(BMP180_SOURCE_FILES
	BMP180/BMP180Barometer.cpp
	BMP180/BMP180Barometer.h
	BMP180/BarometerData.h
)

set(HCSR04_SOURCE_FILES
	HCSR04/HCSR04Rangefinder.cpp
	HCSR04/HCSR04Rangefinder.h
)

set(L3GD20H_SOURCE_FILES
	L3GD20H/L3GD20HGyroscope.cpp
	L3GD20H/L3GD20HGyroscope.h
)

set(LSM303D_SOURCE_FILES
	LSM303D/LSM303DAccelerometer.cpp
	LSM303D/LSM303DAccelerometer.h
	LSM303D/LSM303DMagnetometer.cpp
	LSM303D/LSM303DMagnetometer.h
)

set(FILTER_SOURCE_FILES
	Filter/TimedVector3.h
	Filter/KalmanFilter.cpp
	Filter/KalmanFilter.h
	Filter/KalmanFilterVector3.hpp
	Filter/GyroscopeFilter.cpp
	Filter/GyroscopeFilter.h
	Filter/AccelerometerFilter.cpp
	Filter/AccelerometerFilter.h
	Filter/MagnetometerFilter.cpp
	Filter/MagnetometerFilter.h
	Filter/CalibratedFilter.h
	Filter/C2ComplementerFilter.cpp
	Filter/C2ComplementerFilter.h
)

source_group("Camera" FILES ${CAMERA_SOURCE_FILES})
source_group("PCA9685" FILES ${PCA9685_SOURCE_FILES})
source_group("BMP180" FILES ${BMP180_SOURCE_FILES})
source_group("HCSR04" FILES ${HCSR04_SOURCE_FILES})
source_group("L3GD20H" FILES ${L3GD20H_SOURCE_FILES})
source_group("LSM303D" FILES ${LSM303D_SOURCE_FILES})
source_group("Filter" FILES ${FILTER_SOURCE_FILES})


add_library(
	IoTDevices

	${CAMERA_SOURCE_FILES}
	${PCA9685_SOURCE_FILES}
	${BMP180_SOURCE_FILES}
	${HCSR04_SOURCE_FILES}
	${L3GD20H_SOURCE_FILES}
	${LSM303D_SOURCE_FILES}
	${FILTER_SOURCE_FILES}
)

## OpenCV ivoke into the project
set(OpenCV_DIR ../3rdparty/.CMakeFiles/opencv)
find_package( OpenCV REQUIRED )

include_directories(../Core)
include_directories(${PROJECT_SOURCE_DIR})

target_link_libraries(IoTDevices ${OpenCV_LIBS})
