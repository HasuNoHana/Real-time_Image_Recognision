# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# Include any dependencies generated for this target.
include CMakeFiles/C.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/C.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/C.dir/flags.make

CMakeFiles/C.dir/C.cpp.o: CMakeFiles/C.dir/flags.make
CMakeFiles/C.dir/C.cpp.o: C.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alicja/SCZR/Real-time_Image_Recognision/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/C.dir/C.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/C.dir/C.cpp.o -c /home/alicja/SCZR/Real-time_Image_Recognision/C.cpp

CMakeFiles/C.dir/C.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/C.dir/C.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alicja/SCZR/Real-time_Image_Recognision/C.cpp > CMakeFiles/C.dir/C.cpp.i

CMakeFiles/C.dir/C.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/C.dir/C.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alicja/SCZR/Real-time_Image_Recognision/C.cpp -o CMakeFiles/C.dir/C.cpp.s

CMakeFiles/C.dir/C.cpp.o.requires:

.PHONY : CMakeFiles/C.dir/C.cpp.o.requires

CMakeFiles/C.dir/C.cpp.o.provides: CMakeFiles/C.dir/C.cpp.o.requires
	$(MAKE) -f CMakeFiles/C.dir/build.make CMakeFiles/C.dir/C.cpp.o.provides.build
.PHONY : CMakeFiles/C.dir/C.cpp.o.provides

CMakeFiles/C.dir/C.cpp.o.provides.build: CMakeFiles/C.dir/C.cpp.o


# Object files for target C
C_OBJECTS = \
"CMakeFiles/C.dir/C.cpp.o"

# External object files for target C
C_EXTERNAL_OBJECTS =

C : CMakeFiles/C.dir/C.cpp.o
C : CMakeFiles/C.dir/build.make
C : /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.9
C : /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.9
C : CMakeFiles/C.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alicja/SCZR/Real-time_Image_Recognision/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable C"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/C.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/C.dir/build: C

.PHONY : CMakeFiles/C.dir/build

CMakeFiles/C.dir/requires: CMakeFiles/C.dir/C.cpp.o.requires

.PHONY : CMakeFiles/C.dir/requires

CMakeFiles/C.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/C.dir/cmake_clean.cmake
.PHONY : CMakeFiles/C.dir/clean

CMakeFiles/C.dir/depend:
	cd /home/alicja/SCZR/Real-time_Image_Recognision && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alicja/SCZR/Real-time_Image_Recognision /home/alicja/SCZR/Real-time_Image_Recognision /home/alicja/SCZR/Real-time_Image_Recognision /home/alicja/SCZR/Real-time_Image_Recognision /home/alicja/SCZR/Real-time_Image_Recognision/CMakeFiles/C.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/C.dir/depend

