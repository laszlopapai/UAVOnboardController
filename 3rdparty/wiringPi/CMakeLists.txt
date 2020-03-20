project(wiringPi)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER gcc)

SET(GCC_COVERAGE_COMPILE_FLAGS "-O2 -D_GNU_SOURCE -Wformat=2 -Wall -Wextra -Winline -I. -pipe -fPIC -lm -lpthread -lrt -lcrypt")
SET(GCC_COVERAGE_LINK_FLAGS    "-lm -lpthread -lrt -lcrypt")

# Include header files
include_directories(wiringPi/wiringPi)

set (SOURCE_FILES 
	wiringPi/wiringPi/wiringPi.c
	wiringPi/wiringPi/wiringSerial.c
	wiringPi/wiringPi/wiringShift.c
	wiringPi/wiringPi/piHiPri.c
	wiringPi/wiringPi/piThread.c
	wiringPi/wiringPi/wiringPiSPI.c
	wiringPi/wiringPi/wiringPiI2C.c
	wiringPi/wiringPi/softPwm.c
	wiringPi/wiringPi/softTone.c
	
	wiringPi/wiringPi/mcp23008.c
	wiringPi/wiringPi/mcp23016.c
	wiringPi/wiringPi/mcp23017.c
	wiringPi/wiringPi/mcp23s08.c
	wiringPi/wiringPi/mcp23s17.c
	wiringPi/wiringPi/sr595.c
	wiringPi/wiringPi/pcf8574.c
	wiringPi/wiringPi/pcf8591.c
	wiringPi/wiringPi/mcp3002.c
	wiringPi/wiringPi/mcp3004.c
	wiringPi/wiringPi/mcp4802.c
	wiringPi/wiringPi/mcp3422.c
	wiringPi/wiringPi/max31855.c
	wiringPi/wiringPi/max5322.c
	wiringPi/wiringPi/ads1115.c
	wiringPi/wiringPi/sn3218.c
	wiringPi/wiringPi/bmp180.c
	wiringPi/wiringPi/htu21d.c
	wiringPi/wiringPi/ds18b20.c
	wiringPi/wiringPi/rht03.c
	
	wiringPi/wiringPi/drcSerial.c
	wiringPi/wiringPi/drcNet.c
	wiringPi/wiringPi/pseudoPins.c
	wiringPi/wiringPi/wpiExtensions.c
)


# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set (WiringLibs wiringPi pthread crypt rt m)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Install library headers
file(GLOB HEADERS wiringPi/wiringPi/*.h)
install(FILES ${HEADERS} DESTINATION include)
