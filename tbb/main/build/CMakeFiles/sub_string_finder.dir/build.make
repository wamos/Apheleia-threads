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
include CMakeFiles/sub_string_finder.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sub_string_finder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sub_string_finder.dir/flags.make

CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o: CMakeFiles/sub_string_finder.dir/flags.make
CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o: ../sub_string_finder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shw328/Apheleia/tbb/main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o -c /home/shw328/Apheleia/tbb/main/sub_string_finder.cpp

CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shw328/Apheleia/tbb/main/sub_string_finder.cpp > CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.i

CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shw328/Apheleia/tbb/main/sub_string_finder.cpp -o CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.s

CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o.requires:

.PHONY : CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o.requires

CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o.provides: CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o.requires
	$(MAKE) -f CMakeFiles/sub_string_finder.dir/build.make CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o.provides.build
.PHONY : CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o.provides

CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o.provides.build: CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o


# Object files for target sub_string_finder
sub_string_finder_OBJECTS = \
"CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o"

# External object files for target sub_string_finder
sub_string_finder_EXTERNAL_OBJECTS =

sub_string_finder: CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o
sub_string_finder: CMakeFiles/sub_string_finder.dir/build.make
sub_string_finder: tbb_cmake_build/tbb_cmake_build_subdir_release/libtbb.so.2
sub_string_finder: tbb_cmake_build/tbb_cmake_build_subdir_release/libtbbmalloc_proxy.so.2
sub_string_finder: tbb_cmake_build/tbb_cmake_build_subdir_release/libtbbmalloc.so.2
sub_string_finder: CMakeFiles/sub_string_finder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shw328/Apheleia/tbb/main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sub_string_finder"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sub_string_finder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sub_string_finder.dir/build: sub_string_finder

.PHONY : CMakeFiles/sub_string_finder.dir/build

CMakeFiles/sub_string_finder.dir/requires: CMakeFiles/sub_string_finder.dir/sub_string_finder.cpp.o.requires

.PHONY : CMakeFiles/sub_string_finder.dir/requires

CMakeFiles/sub_string_finder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sub_string_finder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sub_string_finder.dir/clean

CMakeFiles/sub_string_finder.dir/depend:
	cd /home/shw328/Apheleia/tbb/main/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shw328/Apheleia/tbb/main /home/shw328/Apheleia/tbb/main /home/shw328/Apheleia/tbb/main/build /home/shw328/Apheleia/tbb/main/build /home/shw328/Apheleia/tbb/main/build/CMakeFiles/sub_string_finder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sub_string_finder.dir/depend
