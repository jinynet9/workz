# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jinynet9/work/makefile/cmake/hello-project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jinynet9/work/makefile/cmake/hello-project

# Utility rule file for distclean.

# Include the progress variables for this target.
include hello/CMakeFiles/distclean.dir/progress.make

hello/CMakeFiles/distclean:
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jinynet9/work/makefile/cmake/hello-project/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "clean-up completed!"
	cd /home/jinynet9/work/makefile/cmake/hello-project/hello && make clean
	cd /home/jinynet9/work/makefile/cmake/hello-project/hello && rm CMakeCache.txt
	cd /home/jinynet9/work/makefile/cmake/hello-project/hello && rm -rf CMakeFiles

distclean: hello/CMakeFiles/distclean
distclean: hello/CMakeFiles/distclean.dir/build.make
.PHONY : distclean

# Rule to build all files generated by this target.
hello/CMakeFiles/distclean.dir/build: distclean
.PHONY : hello/CMakeFiles/distclean.dir/build

hello/CMakeFiles/distclean.dir/clean:
	cd /home/jinynet9/work/makefile/cmake/hello-project/hello && $(CMAKE_COMMAND) -P CMakeFiles/distclean.dir/cmake_clean.cmake
.PHONY : hello/CMakeFiles/distclean.dir/clean

hello/CMakeFiles/distclean.dir/depend:
	cd /home/jinynet9/work/makefile/cmake/hello-project && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jinynet9/work/makefile/cmake/hello-project /home/jinynet9/work/makefile/cmake/hello-project/hello /home/jinynet9/work/makefile/cmake/hello-project /home/jinynet9/work/makefile/cmake/hello-project/hello /home/jinynet9/work/makefile/cmake/hello-project/hello/CMakeFiles/distclean.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hello/CMakeFiles/distclean.dir/depend

