CMAKE_MINIMUM_REQUIRED(VERSION 2.5)
PROJECT(utility_project)
ADD_LIBRARY(utility SHARED utility.cpp)

INSTALL(TARGETS utility DESTINATION ${CMAKE_BINARY_DIR}/lib)
INSTALL(FILES utility.h DESTINATION ${CMAKE_BINARY_DIR}/include)

ADD_CUSTOM_TARGET(distclean
 COMMAND make clean
 COMMAND rm CMakeCache.txt
 COMMAND rm -rf CMakeFiles
 COMMENT "clean-up completed!"
)
