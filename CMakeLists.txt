cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

project( Image_Recognition )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(A A.cpp)
add_executable(B B.cpp)
add_executable(C C.cpp)
add_executable(D D.cpp)
add_executable(setup setup.cpp)

target_link_libraries( A ${OpenCV_LIBS} )
target_link_libraries( B ${OpenCV_LIBS} )
target_link_libraries( C ${OpenCV_LIBS} )
target_link_libraries( D ${OpenCV_LIBS} )