# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/haselab17/creatwp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/haselab17/creatwp/bin

# Include any dependencies generated for this target.
include CMakeFiles/list_pra.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/list_pra.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/list_pra.dir/flags.make

CMakeFiles/list_pra.dir/src/adrewp.cpp.o: CMakeFiles/list_pra.dir/flags.make
CMakeFiles/list_pra.dir/src/adrewp.cpp.o: ../src/adrewp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/haselab17/creatwp/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/list_pra.dir/src/adrewp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/list_pra.dir/src/adrewp.cpp.o -c /home/haselab17/creatwp/src/adrewp.cpp

CMakeFiles/list_pra.dir/src/adrewp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/list_pra.dir/src/adrewp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/haselab17/creatwp/src/adrewp.cpp > CMakeFiles/list_pra.dir/src/adrewp.cpp.i

CMakeFiles/list_pra.dir/src/adrewp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/list_pra.dir/src/adrewp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/haselab17/creatwp/src/adrewp.cpp -o CMakeFiles/list_pra.dir/src/adrewp.cpp.s

# Object files for target list_pra
list_pra_OBJECTS = \
"CMakeFiles/list_pra.dir/src/adrewp.cpp.o"

# External object files for target list_pra
list_pra_EXTERNAL_OBJECTS =

list_pra: CMakeFiles/list_pra.dir/src/adrewp.cpp.o
list_pra: CMakeFiles/list_pra.dir/build.make
list_pra: CMakeFiles/list_pra.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/haselab17/creatwp/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable list_pra"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/list_pra.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/list_pra.dir/build: list_pra

.PHONY : CMakeFiles/list_pra.dir/build

CMakeFiles/list_pra.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/list_pra.dir/cmake_clean.cmake
.PHONY : CMakeFiles/list_pra.dir/clean

CMakeFiles/list_pra.dir/depend:
	cd /home/haselab17/creatwp/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/haselab17/creatwp /home/haselab17/creatwp /home/haselab17/creatwp/bin /home/haselab17/creatwp/bin /home/haselab17/creatwp/bin/CMakeFiles/list_pra.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/list_pra.dir/depend
