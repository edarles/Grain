# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/emmanuelle/Desktop/Grain

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/emmanuelle/Desktop/Grain

# Utility rule file for Grains_autogen.

# Include the progress variables for this target.
include CMakeFiles/Grains_autogen.dir/progress.make

CMakeFiles/Grains_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/emmanuelle/Desktop/Grain/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target Grains"
	/opt/local/bin/cmake -E cmake_autogen /Users/emmanuelle/Desktop/Grain/CMakeFiles/Grains_autogen.dir/ ""

Grains_autogen: CMakeFiles/Grains_autogen
Grains_autogen: CMakeFiles/Grains_autogen.dir/build.make

.PHONY : Grains_autogen

# Rule to build all files generated by this target.
CMakeFiles/Grains_autogen.dir/build: Grains_autogen

.PHONY : CMakeFiles/Grains_autogen.dir/build

CMakeFiles/Grains_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Grains_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Grains_autogen.dir/clean

CMakeFiles/Grains_autogen.dir/depend:
	cd /Users/emmanuelle/Desktop/Grain && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/emmanuelle/Desktop/Grain /Users/emmanuelle/Desktop/Grain /Users/emmanuelle/Desktop/Grain /Users/emmanuelle/Desktop/Grain /Users/emmanuelle/Desktop/Grain/CMakeFiles/Grains_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Grains_autogen.dir/depend

