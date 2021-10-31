cmake_minimum_required(VERSION 3.4)
set(CMAKE_TOOLCHAIN_FILE C:/software/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")

project(Code1)

set(OpenCV_DIR "C:/software/opencv/build/x64/MinGw")
find_package(OpenCV REQUIRED) 
# include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
# aux_source_directory(./src SrcFiles)
INCLUDE_DIRECTORIES(C:/software/opencv/include/opencv)
INCLUDE_DIRECTORIES(C:/software/opencv/build/include/opencv2)
INCLUDE_DIRECTORIES(C:/software/opencv/build/include)

# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")
# set(CMAKE_CXX_STANDARD 11)

# set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

add_executable(OpencvTest contour.cpp)
target_link_libraries(OpencvTest OpencvLib)
# TARGET_LINK_LIBRARIES(OpencvLib C:/software/opencv/build/x64/MinGw/bin/libopencv_world454.dll)
# TARGET_LINK_LIBRARIES(OpencvLib C:/software/opencv/build/x64/MinGw/bin/opencv_videoio_ffmpeg454_64.dll)

