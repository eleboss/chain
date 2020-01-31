cmake_minimum_required (VERSION 2.6)
project (chain_nor)


SET( CMAKE_CXX_FLAGS "-std=c++11 ")

find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )


find_package( PkgConfig )
pkg_check_modules( EIGEN3 REQUIRED eigen3 )
include_directories( ${EIGEN3_INCLUDE_DIRS} )

add_executable(csae chainSAE.cpp)
add_executable(stackchain stnor_stackchain.cpp)


target_link_libraries(csae ${OpenCV_LIBS} )
target_link_libraries (csae ${EIGEN3_INCLUDE_DIRS})

target_link_libraries(stackchain ${OpenCV_LIBS} )
target_link_libraries (stackchain ${EIGEN3_INCLUDE_DIRS})

