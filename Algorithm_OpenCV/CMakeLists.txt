CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(rawToCV)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -O3")

INCLUDE_DIRECTORIES(${CMAKE_PREFIX_PATH}/include)
INCLUDE_DIRECTORIES(./src)

FIND_LIBRARY(systemc systemc ${CMAKE_PREFIX_PATH}/lib-linux64)
LINK_DIRECTORIES(${CMAKE_PREFIX_PATH}/lib-linux64)

ADD_DEFINITIONS(-D SC_INCLUDE_DYNAMIC_PROCESSES)

ADD_EXECUTABLE(${PROJECT_NAME} src/main.cpp src/factory.h src/structures.h src/config.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} raw opencv_core opencv_highgui opencv_imgproc opencv_videoio opencv_features2d opencv_objdetect opencv_video)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )