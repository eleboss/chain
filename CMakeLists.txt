cmake_minimum_required (VERSION 2.6)
project (chain_nor)


SET( CMAKE_CXX_FLAGS "-std=c++11 ")

find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )


find_package( PkgConfig )
pkg_check_modules( EIGEN3 REQUIRED eigen3 )
include_directories( ${EIGEN3_INCLUDE_DIRS} )


add_executable(chain stnor_chain.cpp)
add_executable(asy stnor_chain_asy.cpp)


target_link_libraries(asy ${OpenCV_LIBS} )
target_link_libraries (asy ${EIGEN3_INCLUDE_DIRS})


target_link_libraries(chain ${OpenCV_LIBS} )
target_link_libraries (chain ${EIGEN3_INCLUDE_DIRS})

