CMAKE_MINIMUM_REQUIRED(VERSION 2.5)
PROJECT(myproject)

ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(hello)

ADD_CUSTOM_TARGET(distclean
 COMMAND make clean
 COMMAND rm CMakeCache.txt
 COMMAND rm -rf CMakeFiles
 COMMENT "clean-up completed!"
)
