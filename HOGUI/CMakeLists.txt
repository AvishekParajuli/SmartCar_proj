set(ExecutableName "hogUI")
cmake_minimum_required(VERSION 2.8)
project( $ExecutableName )
SET(CMAKE_BUILD_TYPE debug)
find_package( OpenCV REQUIRED )
add_executable( ${ExecutableName} ${ExecutableName}.cpp )
target_link_libraries( ${ExecutableName} ${OpenCV_LIBS} )
