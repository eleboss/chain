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
CMAKE_SOURCE_DIR = /home/eleboss/ShapeDVS/chain

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eleboss/ShapeDVS/chain/build

# Include any dependencies generated for this target.
include CMakeFiles/asy.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/asy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/asy.dir/flags.make

CMakeFiles/asy.dir/stnor_chain_asy.cpp.o: CMakeFiles/asy.dir/flags.make
CMakeFiles/asy.dir/stnor_chain_asy.cpp.o: ../stnor_chain_asy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eleboss/ShapeDVS/chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/asy.dir/stnor_chain_asy.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/asy.dir/stnor_chain_asy.cpp.o -c /home/eleboss/ShapeDVS/chain/stnor_chain_asy.cpp

CMakeFiles/asy.dir/stnor_chain_asy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/asy.dir/stnor_chain_asy.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eleboss/ShapeDVS/chain/stnor_chain_asy.cpp > CMakeFiles/asy.dir/stnor_chain_asy.cpp.i

CMakeFiles/asy.dir/stnor_chain_asy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/asy.dir/stnor_chain_asy.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eleboss/ShapeDVS/chain/stnor_chain_asy.cpp -o CMakeFiles/asy.dir/stnor_chain_asy.cpp.s

CMakeFiles/asy.dir/stnor_chain_asy.cpp.o.requires:

.PHONY : CMakeFiles/asy.dir/stnor_chain_asy.cpp.o.requires

CMakeFiles/asy.dir/stnor_chain_asy.cpp.o.provides: CMakeFiles/asy.dir/stnor_chain_asy.cpp.o.requires
	$(MAKE) -f CMakeFiles/asy.dir/build.make CMakeFiles/asy.dir/stnor_chain_asy.cpp.o.provides.build
.PHONY : CMakeFiles/asy.dir/stnor_chain_asy.cpp.o.provides

CMakeFiles/asy.dir/stnor_chain_asy.cpp.o.provides.build: CMakeFiles/asy.dir/stnor_chain_asy.cpp.o


# Object files for target asy
asy_OBJECTS = \
"CMakeFiles/asy.dir/stnor_chain_asy.cpp.o"

# External object files for target asy
asy_EXTERNAL_OBJECTS =

asy: CMakeFiles/asy.dir/stnor_chain_asy.cpp.o
asy: CMakeFiles/asy.dir/build.make
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_stitching3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_superres3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_videostab3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_aruco3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_bgsegm3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_bioinspired3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_ccalib3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_cvv3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_dpm3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_face3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_fuzzy3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_hdf3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_img_hash3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_line_descriptor3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_optflow3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_reg3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_rgbd3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_saliency3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_stereo3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_structured_light3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_surface_matching3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_tracking3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_xfeatures2d3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_ximgproc3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_xobjdetect3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_xphoto3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_shape3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_photo3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_datasets3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_plot3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_text3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_dnn3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_ml3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_video3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_calib3d3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_features2d3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_highgui3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_videoio3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_viz3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_phase_unwrapping3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_flann3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_imgcodecs3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_objdetect3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_imgproc3.so.3.3.1
asy: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_core3.so.3.3.1
asy: CMakeFiles/asy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eleboss/ShapeDVS/chain/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable asy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/asy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/asy.dir/build: asy

.PHONY : CMakeFiles/asy.dir/build

CMakeFiles/asy.dir/requires: CMakeFiles/asy.dir/stnor_chain_asy.cpp.o.requires

.PHONY : CMakeFiles/asy.dir/requires

CMakeFiles/asy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/asy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/asy.dir/clean

CMakeFiles/asy.dir/depend:
	cd /home/eleboss/ShapeDVS/chain/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eleboss/ShapeDVS/chain /home/eleboss/ShapeDVS/chain /home/eleboss/ShapeDVS/chain/build /home/eleboss/ShapeDVS/chain/build /home/eleboss/ShapeDVS/chain/build/CMakeFiles/asy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/asy.dir/depend
