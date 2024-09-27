CMAKE_MINIMUM_REQUIRED(VERSION 2.5)
PROJECT(HelloProject)
ADD_EXECUTABLE(hello main.cpp)

INCLUDE_DIRECTORIES(../library/include)
FIND_LIBRARY(UTILITY_LIB NAMES utility PATHS ../library/lib)
TARGET_LINK_LIBRARIES(hello ${UTILITY_LIB})

ADD_CUSTOM_TARGET(distclean
 COMMAND make clean
 COMMAND rm CMakeCache.txt
 COMMAND rm -rf CMakeFiles
 COMMENT "clean-up completed!"
)
