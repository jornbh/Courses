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
CMAKE_SOURCE_DIR = /home/jorn/Documents/Fag/Sanntidssystemer/Ex5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jorn/Documents/Fag/Sanntidssystemer/Ex5

# Include any dependencies generated for this target.
include CMakeFiles/Sanntid_oving_4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Sanntid_oving_4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Sanntid_oving_4.dir/flags.make

CMakeFiles/Sanntid_oving_4.dir/src/other.c.o: CMakeFiles/Sanntid_oving_4.dir/flags.make
CMakeFiles/Sanntid_oving_4.dir/src/other.c.o: src/other.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ex5/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Sanntid_oving_4.dir/src/other.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Sanntid_oving_4.dir/src/other.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ex5/src/other.c

CMakeFiles/Sanntid_oving_4.dir/src/other.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Sanntid_oving_4.dir/src/other.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ex5/src/other.c > CMakeFiles/Sanntid_oving_4.dir/src/other.c.i

CMakeFiles/Sanntid_oving_4.dir/src/other.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Sanntid_oving_4.dir/src/other.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ex5/src/other.c -o CMakeFiles/Sanntid_oving_4.dir/src/other.c.s

CMakeFiles/Sanntid_oving_4.dir/src/other.c.o.requires:

.PHONY : CMakeFiles/Sanntid_oving_4.dir/src/other.c.o.requires

CMakeFiles/Sanntid_oving_4.dir/src/other.c.o.provides: CMakeFiles/Sanntid_oving_4.dir/src/other.c.o.requires
	$(MAKE) -f CMakeFiles/Sanntid_oving_4.dir/build.make CMakeFiles/Sanntid_oving_4.dir/src/other.c.o.provides.build
.PHONY : CMakeFiles/Sanntid_oving_4.dir/src/other.c.o.provides

CMakeFiles/Sanntid_oving_4.dir/src/other.c.o.provides.build: CMakeFiles/Sanntid_oving_4.dir/src/other.c.o


CMakeFiles/Sanntid_oving_4.dir/src/main.c.o: CMakeFiles/Sanntid_oving_4.dir/flags.make
CMakeFiles/Sanntid_oving_4.dir/src/main.c.o: src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ex5/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Sanntid_oving_4.dir/src/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Sanntid_oving_4.dir/src/main.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ex5/src/main.c

CMakeFiles/Sanntid_oving_4.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Sanntid_oving_4.dir/src/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ex5/src/main.c > CMakeFiles/Sanntid_oving_4.dir/src/main.c.i

CMakeFiles/Sanntid_oving_4.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Sanntid_oving_4.dir/src/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ex5/src/main.c -o CMakeFiles/Sanntid_oving_4.dir/src/main.c.s

CMakeFiles/Sanntid_oving_4.dir/src/main.c.o.requires:

.PHONY : CMakeFiles/Sanntid_oving_4.dir/src/main.c.o.requires

CMakeFiles/Sanntid_oving_4.dir/src/main.c.o.provides: CMakeFiles/Sanntid_oving_4.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/Sanntid_oving_4.dir/build.make CMakeFiles/Sanntid_oving_4.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/Sanntid_oving_4.dir/src/main.c.o.provides

CMakeFiles/Sanntid_oving_4.dir/src/main.c.o.provides.build: CMakeFiles/Sanntid_oving_4.dir/src/main.c.o


# Object files for target Sanntid_oving_4
Sanntid_oving_4_OBJECTS = \
"CMakeFiles/Sanntid_oving_4.dir/src/other.c.o" \
"CMakeFiles/Sanntid_oving_4.dir/src/main.c.o"

# External object files for target Sanntid_oving_4
Sanntid_oving_4_EXTERNAL_OBJECTS =

Sanntid_oving_4/Sanntid_oving_4: CMakeFiles/Sanntid_oving_4.dir/src/other.c.o
Sanntid_oving_4/Sanntid_oving_4: CMakeFiles/Sanntid_oving_4.dir/src/main.c.o
Sanntid_oving_4/Sanntid_oving_4: CMakeFiles/Sanntid_oving_4.dir/build.make
Sanntid_oving_4/Sanntid_oving_4: CMakeFiles/Sanntid_oving_4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ex5/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Sanntid_oving_4/Sanntid_oving_4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Sanntid_oving_4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Sanntid_oving_4.dir/build: Sanntid_oving_4/Sanntid_oving_4

.PHONY : CMakeFiles/Sanntid_oving_4.dir/build

CMakeFiles/Sanntid_oving_4.dir/requires: CMakeFiles/Sanntid_oving_4.dir/src/other.c.o.requires
CMakeFiles/Sanntid_oving_4.dir/requires: CMakeFiles/Sanntid_oving_4.dir/src/main.c.o.requires

.PHONY : CMakeFiles/Sanntid_oving_4.dir/requires

CMakeFiles/Sanntid_oving_4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Sanntid_oving_4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Sanntid_oving_4.dir/clean

CMakeFiles/Sanntid_oving_4.dir/depend:
	cd /home/jorn/Documents/Fag/Sanntidssystemer/Ex5 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jorn/Documents/Fag/Sanntidssystemer/Ex5 /home/jorn/Documents/Fag/Sanntidssystemer/Ex5 /home/jorn/Documents/Fag/Sanntidssystemer/Ex5 /home/jorn/Documents/Fag/Sanntidssystemer/Ex5 /home/jorn/Documents/Fag/Sanntidssystemer/Ex5/CMakeFiles/Sanntid_oving_4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Sanntid_oving_4.dir/depend

