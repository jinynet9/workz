#!/bin/bash
#
# edit CMakeLists.txt
#

CLEAN_FILES=" \
	CMakeCache.txt \
	cmake_install.cmake \
	Makefile \
	CMakeFiles \
	"

purge_conf() {
    echo -n "Purging all before autogen ... "
    [ -f Makefile ] && make clean
    rm -rf $CLEAN_FILES
    find . -name Makefile.in -exec rm -f {} \;
    find . -name Makefile -exec rm -f {} \;
    find . -name .deps -exec rm -rf {} \;
    echo "DONE"
}

usage_exit() {
    echo "usage: $0 [-p] [-r]"
    echo "  -p  purge all before autogen"
    echo "  -r  reconfigure (purge & config)"
    exit 1
}

while getopts "pr" opt
do
    case "$opt" in
    	"p")
	    purge_conf
	    exit 0
	    ;;
	"r")
	    purge_conf
	    ;;
	*)
	    usage_exit
	    ;;
    esac
done

cmake CMakeLists.txt

[ $? -ne 0 ] && { echo "cmake: FAILED"; exit 1; }

exit 0

