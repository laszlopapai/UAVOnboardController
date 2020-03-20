cmake_minimum_required(VERSION 3.5)
project(IoTCore)

set(CMAKE_CXX_STANDARD 17)


SET(CONTROL_FILES
		Control/PIDController.cpp
		Control/PIDController.h
		Control/PIDControllerVector3.hpp
		ILogger.h
		Time.cpp
		Time.h
)

set(MATH_FILES
		Math/Vector3.cpp
		Math/Vector3.h
		Math/Quaternion.cpp
		Math/Quaternion.h
		Math/AngleConvert.cpp
		Math/AngleConvert.h
		Math/ColorConvert.cpp
		Math/ColorConvert.h
)

set(NETWORK_FILES
		Network/UDPClient.cpp
		Network/UDPClient.h
		Network/UDPServer.cpp
		Network/UDPServer.h
		Network/IPackage.h
		Network/PackageClient.h
		Network/PackageServer.h
)

set(THREAD_FILES
		Thread/AsyncProcess.cpp
		Thread/AsyncProcess.h
		Thread/IFilter.h
		Thread/NotifyPipe.hpp
		Thread/AwaitStopSignal.cpp
		Thread/AwaitStopSignal.h
		Thread/AutoAwaitStopSignal.h
		Thread/AtomicMethod.hpp
		Thread/IRunning.h
		Thread/IObserver.h
		Thread/IObservable.h
		Thread/Utils.cpp
		Thread/Utils.h

		Data/StreamPart.h
		Data/StreamPart.cpp
		Data/StreamBinder.h
		Data/StreamNode.h
)

source_group("Control" FILES ${CONTROL_FILES})
source_group("Math" FILES ${MATH_FILES})
source_group("Network" FILES ${NETWORK_FILES})
source_group("Thread" FILES ${THREAD_FILES})

include_directories(${PROJECT_SOURCE_DIR})
add_library(
	IoTCore

	${CONTROL_FILES}
	${MATH_FILES}
	${NETWORK_FILES}
	${THREAD_FILES}
)
