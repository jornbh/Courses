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
CMAKE_SOURCE_DIR = /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1

# Include any dependencies generated for this target.
include CMakeFiles/helloWorld.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/helloWorld.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/helloWorld.dir/flags.make

CMakeFiles/helloWorld.dir/src/taskD.c.o: CMakeFiles/helloWorld.dir/flags.make
CMakeFiles/helloWorld.dir/src/taskD.c.o: src/taskD.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/helloWorld.dir/src/taskD.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/helloWorld.dir/src/taskD.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskD.c

CMakeFiles/helloWorld.dir/src/taskD.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/helloWorld.dir/src/taskD.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskD.c > CMakeFiles/helloWorld.dir/src/taskD.c.i

CMakeFiles/helloWorld.dir/src/taskD.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/helloWorld.dir/src/taskD.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskD.c -o CMakeFiles/helloWorld.dir/src/taskD.c.s

CMakeFiles/helloWorld.dir/src/taskD.c.o.requires:

.PHONY : CMakeFiles/helloWorld.dir/src/taskD.c.o.requires

CMakeFiles/helloWorld.dir/src/taskD.c.o.provides: CMakeFiles/helloWorld.dir/src/taskD.c.o.requires
	$(MAKE) -f CMakeFiles/helloWorld.dir/build.make CMakeFiles/helloWorld.dir/src/taskD.c.o.provides.build
.PHONY : CMakeFiles/helloWorld.dir/src/taskD.c.o.provides

CMakeFiles/helloWorld.dir/src/taskD.c.o.provides.build: CMakeFiles/helloWorld.dir/src/taskD.c.o


CMakeFiles/helloWorld.dir/src/time_operators.c.o: CMakeFiles/helloWorld.dir/flags.make
CMakeFiles/helloWorld.dir/src/time_operators.c.o: src/time_operators.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/helloWorld.dir/src/time_operators.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/helloWorld.dir/src/time_operators.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/time_operators.c

CMakeFiles/helloWorld.dir/src/time_operators.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/helloWorld.dir/src/time_operators.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/time_operators.c > CMakeFiles/helloWorld.dir/src/time_operators.c.i

CMakeFiles/helloWorld.dir/src/time_operators.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/helloWorld.dir/src/time_operators.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/time_operators.c -o CMakeFiles/helloWorld.dir/src/time_operators.c.s

CMakeFiles/helloWorld.dir/src/time_operators.c.o.requires:

.PHONY : CMakeFiles/helloWorld.dir/src/time_operators.c.o.requires

CMakeFiles/helloWorld.dir/src/time_operators.c.o.provides: CMakeFiles/helloWorld.dir/src/time_operators.c.o.requires
	$(MAKE) -f CMakeFiles/helloWorld.dir/build.make CMakeFiles/helloWorld.dir/src/time_operators.c.o.provides.build
.PHONY : CMakeFiles/helloWorld.dir/src/time_operators.c.o.provides

CMakeFiles/helloWorld.dir/src/time_operators.c.o.provides.build: CMakeFiles/helloWorld.dir/src/time_operators.c.o


CMakeFiles/helloWorld.dir/src/taskA.c.o: CMakeFiles/helloWorld.dir/flags.make
CMakeFiles/helloWorld.dir/src/taskA.c.o: src/taskA.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/helloWorld.dir/src/taskA.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/helloWorld.dir/src/taskA.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskA.c

CMakeFiles/helloWorld.dir/src/taskA.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/helloWorld.dir/src/taskA.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskA.c > CMakeFiles/helloWorld.dir/src/taskA.c.i

CMakeFiles/helloWorld.dir/src/taskA.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/helloWorld.dir/src/taskA.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskA.c -o CMakeFiles/helloWorld.dir/src/taskA.c.s

CMakeFiles/helloWorld.dir/src/taskA.c.o.requires:

.PHONY : CMakeFiles/helloWorld.dir/src/taskA.c.o.requires

CMakeFiles/helloWorld.dir/src/taskA.c.o.provides: CMakeFiles/helloWorld.dir/src/taskA.c.o.requires
	$(MAKE) -f CMakeFiles/helloWorld.dir/build.make CMakeFiles/helloWorld.dir/src/taskA.c.o.provides.build
.PHONY : CMakeFiles/helloWorld.dir/src/taskA.c.o.provides

CMakeFiles/helloWorld.dir/src/taskA.c.o.provides.build: CMakeFiles/helloWorld.dir/src/taskA.c.o


CMakeFiles/helloWorld.dir/src/taskB.c.o: CMakeFiles/helloWorld.dir/flags.make
CMakeFiles/helloWorld.dir/src/taskB.c.o: src/taskB.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/helloWorld.dir/src/taskB.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/helloWorld.dir/src/taskB.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskB.c

CMakeFiles/helloWorld.dir/src/taskB.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/helloWorld.dir/src/taskB.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskB.c > CMakeFiles/helloWorld.dir/src/taskB.c.i

CMakeFiles/helloWorld.dir/src/taskB.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/helloWorld.dir/src/taskB.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskB.c -o CMakeFiles/helloWorld.dir/src/taskB.c.s

CMakeFiles/helloWorld.dir/src/taskB.c.o.requires:

.PHONY : CMakeFiles/helloWorld.dir/src/taskB.c.o.requires

CMakeFiles/helloWorld.dir/src/taskB.c.o.provides: CMakeFiles/helloWorld.dir/src/taskB.c.o.requires
	$(MAKE) -f CMakeFiles/helloWorld.dir/build.make CMakeFiles/helloWorld.dir/src/taskB.c.o.provides.build
.PHONY : CMakeFiles/helloWorld.dir/src/taskB.c.o.provides

CMakeFiles/helloWorld.dir/src/taskB.c.o.provides.build: CMakeFiles/helloWorld.dir/src/taskB.c.o


CMakeFiles/helloWorld.dir/src/taskE.c.o: CMakeFiles/helloWorld.dir/flags.make
CMakeFiles/helloWorld.dir/src/taskE.c.o: src/taskE.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/helloWorld.dir/src/taskE.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/helloWorld.dir/src/taskE.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskE.c

CMakeFiles/helloWorld.dir/src/taskE.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/helloWorld.dir/src/taskE.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskE.c > CMakeFiles/helloWorld.dir/src/taskE.c.i

CMakeFiles/helloWorld.dir/src/taskE.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/helloWorld.dir/src/taskE.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskE.c -o CMakeFiles/helloWorld.dir/src/taskE.c.s

CMakeFiles/helloWorld.dir/src/taskE.c.o.requires:

.PHONY : CMakeFiles/helloWorld.dir/src/taskE.c.o.requires

CMakeFiles/helloWorld.dir/src/taskE.c.o.provides: CMakeFiles/helloWorld.dir/src/taskE.c.o.requires
	$(MAKE) -f CMakeFiles/helloWorld.dir/build.make CMakeFiles/helloWorld.dir/src/taskE.c.o.provides.build
.PHONY : CMakeFiles/helloWorld.dir/src/taskE.c.o.provides

CMakeFiles/helloWorld.dir/src/taskE.c.o.provides.build: CMakeFiles/helloWorld.dir/src/taskE.c.o


CMakeFiles/helloWorld.dir/src/array.c.o: CMakeFiles/helloWorld.dir/flags.make
CMakeFiles/helloWorld.dir/src/array.c.o: src/array.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/helloWorld.dir/src/array.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/helloWorld.dir/src/array.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/array.c

CMakeFiles/helloWorld.dir/src/array.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/helloWorld.dir/src/array.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/array.c > CMakeFiles/helloWorld.dir/src/array.c.i

CMakeFiles/helloWorld.dir/src/array.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/helloWorld.dir/src/array.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/array.c -o CMakeFiles/helloWorld.dir/src/array.c.s

CMakeFiles/helloWorld.dir/src/array.c.o.requires:

.PHONY : CMakeFiles/helloWorld.dir/src/array.c.o.requires

CMakeFiles/helloWorld.dir/src/array.c.o.provides: CMakeFiles/helloWorld.dir/src/array.c.o.requires
	$(MAKE) -f CMakeFiles/helloWorld.dir/build.make CMakeFiles/helloWorld.dir/src/array.c.o.provides.build
.PHONY : CMakeFiles/helloWorld.dir/src/array.c.o.provides

CMakeFiles/helloWorld.dir/src/array.c.o.provides.build: CMakeFiles/helloWorld.dir/src/array.c.o


CMakeFiles/helloWorld.dir/src/main.c.o: CMakeFiles/helloWorld.dir/flags.make
CMakeFiles/helloWorld.dir/src/main.c.o: src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/helloWorld.dir/src/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/helloWorld.dir/src/main.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/main.c

CMakeFiles/helloWorld.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/helloWorld.dir/src/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/main.c > CMakeFiles/helloWorld.dir/src/main.c.i

CMakeFiles/helloWorld.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/helloWorld.dir/src/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/main.c -o CMakeFiles/helloWorld.dir/src/main.c.s

CMakeFiles/helloWorld.dir/src/main.c.o.requires:

.PHONY : CMakeFiles/helloWorld.dir/src/main.c.o.requires

CMakeFiles/helloWorld.dir/src/main.c.o.provides: CMakeFiles/helloWorld.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/helloWorld.dir/build.make CMakeFiles/helloWorld.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/helloWorld.dir/src/main.c.o.provides

CMakeFiles/helloWorld.dir/src/main.c.o.provides.build: CMakeFiles/helloWorld.dir/src/main.c.o


CMakeFiles/helloWorld.dir/src/taskC.c.o: CMakeFiles/helloWorld.dir/flags.make
CMakeFiles/helloWorld.dir/src/taskC.c.o: src/taskC.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/helloWorld.dir/src/taskC.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/helloWorld.dir/src/taskC.c.o   -c /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskC.c

CMakeFiles/helloWorld.dir/src/taskC.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/helloWorld.dir/src/taskC.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskC.c > CMakeFiles/helloWorld.dir/src/taskC.c.i

CMakeFiles/helloWorld.dir/src/taskC.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/helloWorld.dir/src/taskC.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/src/taskC.c -o CMakeFiles/helloWorld.dir/src/taskC.c.s

CMakeFiles/helloWorld.dir/src/taskC.c.o.requires:

.PHONY : CMakeFiles/helloWorld.dir/src/taskC.c.o.requires

CMakeFiles/helloWorld.dir/src/taskC.c.o.provides: CMakeFiles/helloWorld.dir/src/taskC.c.o.requires
	$(MAKE) -f CMakeFiles/helloWorld.dir/build.make CMakeFiles/helloWorld.dir/src/taskC.c.o.provides.build
.PHONY : CMakeFiles/helloWorld.dir/src/taskC.c.o.provides

CMakeFiles/helloWorld.dir/src/taskC.c.o.provides.build: CMakeFiles/helloWorld.dir/src/taskC.c.o


# Object files for target helloWorld
helloWorld_OBJECTS = \
"CMakeFiles/helloWorld.dir/src/taskD.c.o" \
"CMakeFiles/helloWorld.dir/src/time_operators.c.o" \
"CMakeFiles/helloWorld.dir/src/taskA.c.o" \
"CMakeFiles/helloWorld.dir/src/taskB.c.o" \
"CMakeFiles/helloWorld.dir/src/taskE.c.o" \
"CMakeFiles/helloWorld.dir/src/array.c.o" \
"CMakeFiles/helloWorld.dir/src/main.c.o" \
"CMakeFiles/helloWorld.dir/src/taskC.c.o"

# External object files for target helloWorld
helloWorld_EXTERNAL_OBJECTS =

helloWorld: CMakeFiles/helloWorld.dir/src/taskD.c.o
helloWorld: CMakeFiles/helloWorld.dir/src/time_operators.c.o
helloWorld: CMakeFiles/helloWorld.dir/src/taskA.c.o
helloWorld: CMakeFiles/helloWorld.dir/src/taskB.c.o
helloWorld: CMakeFiles/helloWorld.dir/src/taskE.c.o
helloWorld: CMakeFiles/helloWorld.dir/src/array.c.o
helloWorld: CMakeFiles/helloWorld.dir/src/main.c.o
helloWorld: CMakeFiles/helloWorld.dir/src/taskC.c.o
helloWorld: CMakeFiles/helloWorld.dir/build.make
helloWorld: CMakeFiles/helloWorld.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking C executable helloWorld"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/helloWorld.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/helloWorld.dir/build: helloWorld

.PHONY : CMakeFiles/helloWorld.dir/build

CMakeFiles/helloWorld.dir/requires: CMakeFiles/helloWorld.dir/src/taskD.c.o.requires
CMakeFiles/helloWorld.dir/requires: CMakeFiles/helloWorld.dir/src/time_operators.c.o.requires
CMakeFiles/helloWorld.dir/requires: CMakeFiles/helloWorld.dir/src/taskA.c.o.requires
CMakeFiles/helloWorld.dir/requires: CMakeFiles/helloWorld.dir/src/taskB.c.o.requires
CMakeFiles/helloWorld.dir/requires: CMakeFiles/helloWorld.dir/src/taskE.c.o.requires
CMakeFiles/helloWorld.dir/requires: CMakeFiles/helloWorld.dir/src/array.c.o.requires
CMakeFiles/helloWorld.dir/requires: CMakeFiles/helloWorld.dir/src/main.c.o.requires
CMakeFiles/helloWorld.dir/requires: CMakeFiles/helloWorld.dir/src/taskC.c.o.requires

.PHONY : CMakeFiles/helloWorld.dir/requires

CMakeFiles/helloWorld.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/helloWorld.dir/cmake_clean.cmake
.PHONY : CMakeFiles/helloWorld.dir/clean

CMakeFiles/helloWorld.dir/depend:
	cd /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1 /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1 /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1 /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1 /home/jorn/Documents/Fag/Sanntidssystemer/Ovinger/Oving1/CMakeFiles/helloWorld.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/helloWorld.dir/depend

