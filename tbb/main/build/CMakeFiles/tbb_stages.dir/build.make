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
CMAKE_SOURCE_DIR = /home/shw328/Apheleia/tbb/main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shw328/Apheleia/tbb/main/build

# Include any dependencies generated for this target.
include CMakeFiles/tbb_stages.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tbb_stages.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tbb_stages.dir/flags.make

CMakeFiles/tbb_stages.dir/tbb-send.cpp.o: CMakeFiles/tbb_stages.dir/flags.make
CMakeFiles/tbb_stages.dir/tbb-send.cpp.o: ../tbb-send.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shw328/Apheleia/tbb/main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tbb_stages.dir/tbb-send.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tbb_stages.dir/tbb-send.cpp.o -c /home/shw328/Apheleia/tbb/main/tbb-send.cpp

CMakeFiles/tbb_stages.dir/tbb-send.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tbb_stages.dir/tbb-send.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shw328/Apheleia/tbb/main/tbb-send.cpp > CMakeFiles/tbb_stages.dir/tbb-send.cpp.i

CMakeFiles/tbb_stages.dir/tbb-send.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tbb_stages.dir/tbb-send.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shw328/Apheleia/tbb/main/tbb-send.cpp -o CMakeFiles/tbb_stages.dir/tbb-send.cpp.s

CMakeFiles/tbb_stages.dir/tbb-send.cpp.o.requires:

.PHONY : CMakeFiles/tbb_stages.dir/tbb-send.cpp.o.requires

CMakeFiles/tbb_stages.dir/tbb-send.cpp.o.provides: CMakeFiles/tbb_stages.dir/tbb-send.cpp.o.requires
	$(MAKE) -f CMakeFiles/tbb_stages.dir/build.make CMakeFiles/tbb_stages.dir/tbb-send.cpp.o.provides.build
.PHONY : CMakeFiles/tbb_stages.dir/tbb-send.cpp.o.provides

CMakeFiles/tbb_stages.dir/tbb-send.cpp.o.provides.build: CMakeFiles/tbb_stages.dir/tbb-send.cpp.o


CMakeFiles/tbb_stages.dir/Socket.cpp.o: CMakeFiles/tbb_stages.dir/flags.make
CMakeFiles/tbb_stages.dir/Socket.cpp.o: ../Socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shw328/Apheleia/tbb/main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/tbb_stages.dir/Socket.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tbb_stages.dir/Socket.cpp.o -c /home/shw328/Apheleia/tbb/main/Socket.cpp

CMakeFiles/tbb_stages.dir/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tbb_stages.dir/Socket.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shw328/Apheleia/tbb/main/Socket.cpp > CMakeFiles/tbb_stages.dir/Socket.cpp.i

CMakeFiles/tbb_stages.dir/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tbb_stages.dir/Socket.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shw328/Apheleia/tbb/main/Socket.cpp -o CMakeFiles/tbb_stages.dir/Socket.cpp.s

CMakeFiles/tbb_stages.dir/Socket.cpp.o.requires:

.PHONY : CMakeFiles/tbb_stages.dir/Socket.cpp.o.requires

CMakeFiles/tbb_stages.dir/Socket.cpp.o.provides: CMakeFiles/tbb_stages.dir/Socket.cpp.o.requires
	$(MAKE) -f CMakeFiles/tbb_stages.dir/build.make CMakeFiles/tbb_stages.dir/Socket.cpp.o.provides.build
.PHONY : CMakeFiles/tbb_stages.dir/Socket.cpp.o.provides

CMakeFiles/tbb_stages.dir/Socket.cpp.o.provides.build: CMakeFiles/tbb_stages.dir/Socket.cpp.o


# Object files for target tbb_stages
tbb_stages_OBJECTS = \
"CMakeFiles/tbb_stages.dir/tbb-send.cpp.o" \
"CMakeFiles/tbb_stages.dir/Socket.cpp.o"

# External object files for target tbb_stages
tbb_stages_EXTERNAL_OBJECTS =

tbb_stages: CMakeFiles/tbb_stages.dir/tbb-send.cpp.o
tbb_stages: CMakeFiles/tbb_stages.dir/Socket.cpp.o
tbb_stages: CMakeFiles/tbb_stages.dir/build.make
tbb_stages: tbb_cmake_build/tbb_cmake_build_subdir_release/libtbb.so.2
tbb_stages: tbb_cmake_build/tbb_cmake_build_subdir_release/libtbbmalloc_proxy.so.2
tbb_stages: tbb_cmake_build/tbb_cmake_build_subdir_release/libtbbmalloc.so.2
tbb_stages: CMakeFiles/tbb_stages.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shw328/Apheleia/tbb/main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable tbb_stages"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tbb_stages.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tbb_stages.dir/build: tbb_stages

.PHONY : CMakeFiles/tbb_stages.dir/build

CMakeFiles/tbb_stages.dir/requires: CMakeFiles/tbb_stages.dir/tbb-send.cpp.o.requires
CMakeFiles/tbb_stages.dir/requires: CMakeFiles/tbb_stages.dir/Socket.cpp.o.requires

.PHONY : CMakeFiles/tbb_stages.dir/requires

CMakeFiles/tbb_stages.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tbb_stages.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tbb_stages.dir/clean

CMakeFiles/tbb_stages.dir/depend:
	cd /home/shw328/Apheleia/tbb/main/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shw328/Apheleia/tbb/main /home/shw328/Apheleia/tbb/main /home/shw328/Apheleia/tbb/main/build /home/shw328/Apheleia/tbb/main/build /home/shw328/Apheleia/tbb/main/build/CMakeFiles/tbb_stages.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tbb_stages.dir/depend

