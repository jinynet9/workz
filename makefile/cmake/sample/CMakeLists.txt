
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

PROJECT ( PooBar )

#SET ( CMAKE_VERBOSE_MAKEFILE ON )

#SET ( SRC_FILES main.c foo.c bar.c )
file ( GLOB_RECURSE
	SRC_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	)

ADD_EXECUTABLE ( app.out ${SRC_FILES} )

ADD_CUSTOM_TARGET ( distclean
	COMMAND make clean
	COMMAND rm CMakeCache.txt
	COMMAND rm -rf CMakeFiles
	COMMENT "clean-up completed!"
)

