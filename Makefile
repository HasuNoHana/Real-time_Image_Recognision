# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alicja/SCZR/Real-time_Image_Recognision

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alicja/SCZR/Real-time_Image_Recognision

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/cmake-gui -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/alicja/SCZR/Real-time_Image_Recognision/CMakeFiles /home/alicja/SCZR/Real-time_Image_Recognision/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/alicja/SCZR/Real-time_Image_Recognision/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named A

# Build rule for target.
A : cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 A
.PHONY : A

# fast build rule for target.
A/fast:
	$(MAKE) -f CMakeFiles/A.dir/build.make CMakeFiles/A.dir/build
.PHONY : A/fast

#=============================================================================
# Target rules for targets named B

# Build rule for target.
B : cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 B
.PHONY : B

# fast build rule for target.
B/fast:
	$(MAKE) -f CMakeFiles/B.dir/build.make CMakeFiles/B.dir/build
.PHONY : B/fast

#=============================================================================
# Target rules for targets named C

# Build rule for target.
C : cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 C
.PHONY : C

# fast build rule for target.
C/fast:
	$(MAKE) -f CMakeFiles/C.dir/build.make CMakeFiles/C.dir/build
.PHONY : C/fast

#=============================================================================
# Target rules for targets named D

# Build rule for target.
D : cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 D
.PHONY : D

# fast build rule for target.
D/fast:
	$(MAKE) -f CMakeFiles/D.dir/build.make CMakeFiles/D.dir/build
.PHONY : D/fast

A.o: A.cpp.o

.PHONY : A.o

# target to build an object file
A.cpp.o:
	$(MAKE) -f CMakeFiles/A.dir/build.make CMakeFiles/A.dir/A.cpp.o
.PHONY : A.cpp.o

A.i: A.cpp.i

.PHONY : A.i

# target to preprocess a source file
A.cpp.i:
	$(MAKE) -f CMakeFiles/A.dir/build.make CMakeFiles/A.dir/A.cpp.i
.PHONY : A.cpp.i

A.s: A.cpp.s

.PHONY : A.s

# target to generate assembly for a file
A.cpp.s:
	$(MAKE) -f CMakeFiles/A.dir/build.make CMakeFiles/A.dir/A.cpp.s
.PHONY : A.cpp.s

B.o: B.cpp.o

.PHONY : B.o

# target to build an object file
B.cpp.o:
	$(MAKE) -f CMakeFiles/B.dir/build.make CMakeFiles/B.dir/B.cpp.o
.PHONY : B.cpp.o

B.i: B.cpp.i

.PHONY : B.i

# target to preprocess a source file
B.cpp.i:
	$(MAKE) -f CMakeFiles/B.dir/build.make CMakeFiles/B.dir/B.cpp.i
.PHONY : B.cpp.i

B.s: B.cpp.s

.PHONY : B.s

# target to generate assembly for a file
B.cpp.s:
	$(MAKE) -f CMakeFiles/B.dir/build.make CMakeFiles/B.dir/B.cpp.s
.PHONY : B.cpp.s

C.o: C.cpp.o

.PHONY : C.o

# target to build an object file
C.cpp.o:
	$(MAKE) -f CMakeFiles/C.dir/build.make CMakeFiles/C.dir/C.cpp.o
.PHONY : C.cpp.o

C.i: C.cpp.i

.PHONY : C.i

# target to preprocess a source file
C.cpp.i:
	$(MAKE) -f CMakeFiles/C.dir/build.make CMakeFiles/C.dir/C.cpp.i
.PHONY : C.cpp.i

C.s: C.cpp.s

.PHONY : C.s

# target to generate assembly for a file
C.cpp.s:
	$(MAKE) -f CMakeFiles/C.dir/build.make CMakeFiles/C.dir/C.cpp.s
.PHONY : C.cpp.s

D.o: D.cpp.o

.PHONY : D.o

# target to build an object file
D.cpp.o:
	$(MAKE) -f CMakeFiles/D.dir/build.make CMakeFiles/D.dir/D.cpp.o
.PHONY : D.cpp.o

D.i: D.cpp.i

.PHONY : D.i

# target to preprocess a source file
D.cpp.i:
	$(MAKE) -f CMakeFiles/D.dir/build.make CMakeFiles/D.dir/D.cpp.i
.PHONY : D.cpp.i

D.s: D.cpp.s

.PHONY : D.s

# target to generate assembly for a file
D.cpp.s:
	$(MAKE) -f CMakeFiles/D.dir/build.make CMakeFiles/D.dir/D.cpp.s
.PHONY : D.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... A"
	@echo "... B"
	@echo "... C"
	@echo "... edit_cache"
	@echo "... D"
	@echo "... A.o"
	@echo "... A.i"
	@echo "... A.s"
	@echo "... B.o"
	@echo "... B.i"
	@echo "... B.s"
	@echo "... C.o"
	@echo "... C.i"
	@echo "... C.s"
	@echo "... D.o"
	@echo "... D.i"
	@echo "... D.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

