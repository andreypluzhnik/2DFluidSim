# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andrey/Projects/FluidSim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andrey/Projects/FluidSim/build

# Include any dependencies generated for this target.
include CMakeFiles/GLAD.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GLAD.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GLAD.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GLAD.dir/flags.make

CMakeFiles/GLAD.dir/include/glad/glad.c.o: CMakeFiles/GLAD.dir/flags.make
CMakeFiles/GLAD.dir/include/glad/glad.c.o: ../include/glad/glad.c
CMakeFiles/GLAD.dir/include/glad/glad.c.o: CMakeFiles/GLAD.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrey/Projects/FluidSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/GLAD.dir/include/glad/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/GLAD.dir/include/glad/glad.c.o -MF CMakeFiles/GLAD.dir/include/glad/glad.c.o.d -o CMakeFiles/GLAD.dir/include/glad/glad.c.o -c /home/andrey/Projects/FluidSim/include/glad/glad.c

CMakeFiles/GLAD.dir/include/glad/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/GLAD.dir/include/glad/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/andrey/Projects/FluidSim/include/glad/glad.c > CMakeFiles/GLAD.dir/include/glad/glad.c.i

CMakeFiles/GLAD.dir/include/glad/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/GLAD.dir/include/glad/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/andrey/Projects/FluidSim/include/glad/glad.c -o CMakeFiles/GLAD.dir/include/glad/glad.c.s

# Object files for target GLAD
GLAD_OBJECTS = \
"CMakeFiles/GLAD.dir/include/glad/glad.c.o"

# External object files for target GLAD
GLAD_EXTERNAL_OBJECTS =

libGLAD.a: CMakeFiles/GLAD.dir/include/glad/glad.c.o
libGLAD.a: CMakeFiles/GLAD.dir/build.make
libGLAD.a: CMakeFiles/GLAD.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andrey/Projects/FluidSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libGLAD.a"
	$(CMAKE_COMMAND) -P CMakeFiles/GLAD.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GLAD.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GLAD.dir/build: libGLAD.a
.PHONY : CMakeFiles/GLAD.dir/build

CMakeFiles/GLAD.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GLAD.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GLAD.dir/clean

CMakeFiles/GLAD.dir/depend:
	cd /home/andrey/Projects/FluidSim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andrey/Projects/FluidSim /home/andrey/Projects/FluidSim /home/andrey/Projects/FluidSim/build /home/andrey/Projects/FluidSim/build /home/andrey/Projects/FluidSim/build/CMakeFiles/GLAD.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GLAD.dir/depend

