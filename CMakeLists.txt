cmake_minimum_required(VERSION 3.17)
project(FilterApplication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(OpenCV_DIR "/home/johanna/Downloads/opencv_4.1.0_gcc7.4.0_ubuntu1804_x64/opencv_4.1.0_gcc7.4.0_ubuntu1804_x64")
find_package(OpenCV REQUIRED)
include_directories("/home/johanna/Downloads/opencv_4.1.0_gcc7.4.0_ubuntu1804_x64/opencv_4.1.0_gcc7.4.0_ubuntu1804_x64/include")

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(FilterApplication main.cpp ImageManipulation.cpp ImageManipulation.h Utils.cpp Utils.h)
target_link_libraries(FilterApplication Qt5::Core Qt5::Gui Qt5::Widgets ${OpenCV_LIBS})