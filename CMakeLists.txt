cmake_minimum_required(VERSION 3.13)
project(BackgroundSubtraction)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(BackgroundSubtraction main.cpp)

target_link_libraries(BackgroundSubtraction ${OpenCV_LIBS})