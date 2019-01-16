CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(sim)

SET(CMAKE_PREFIX_PATH /usr/local/systemc-2.3.3)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -O3")

INCLUDE_DIRECTORIES(${CMAKE_PREFIX_PATH}/include)
INCLUDE_DIRECTORIES(./src)

FIND_LIBRARY(systemc systemc ${CMAKE_PREFIX_PATH}/lib-linux64)
LINK_DIRECTORIES(${CMAKE_PREFIX_PATH}/lib-linux64)

ADD_DEFINITIONS(-D SC_INCLUDE_DYNAMIC_PROCESSES)

ADD_EXECUTABLE(${PROJECT_NAME} src/main.cpp src/Top.cpp src/Top.h src/Initiator.cpp src/Initiator.h src/Target.cpp src/Target.h src/TransmissionInterface.h src/ADC.cpp src/ADC.h src/Interconnect.cpp src/Interconnect.h src/SIMD.cpp src/SIMD.h src/CPU_Bayer.cpp src/CPU_Bayer.h src/CPU_2.cpp src/CPU_2.h src/Analog_ACC.cpp src/Analog_ACC.h src/VSoC.cpp src/VSoC.h src/SensorReader.cpp src/SensorReader.h src/GlobalConfiguration.cpp src/GlobalConfiguration.h src/ImageFactory.cpp src/ImageFactory.h)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} systemc)

