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
CMAKE_SOURCE_DIR = /home/war/myCode/tuchuang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/war/myCode/tuchuang/build

# Include any dependencies generated for this target.
include src/CMakeFiles/tuchuang.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/tuchuang.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/tuchuang.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/tuchuang.dir/flags.make

src/CMakeFiles/tuchuang.dir/main.cpp.o: src/CMakeFiles/tuchuang.dir/flags.make
src/CMakeFiles/tuchuang.dir/main.cpp.o: ../src/main.cpp
src/CMakeFiles/tuchuang.dir/main.cpp.o: src/CMakeFiles/tuchuang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/war/myCode/tuchuang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/tuchuang.dir/main.cpp.o"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/tuchuang.dir/main.cpp.o -MF CMakeFiles/tuchuang.dir/main.cpp.o.d -o CMakeFiles/tuchuang.dir/main.cpp.o -c /home/war/myCode/tuchuang/src/main.cpp

src/CMakeFiles/tuchuang.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tuchuang.dir/main.cpp.i"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/war/myCode/tuchuang/src/main.cpp > CMakeFiles/tuchuang.dir/main.cpp.i

src/CMakeFiles/tuchuang.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tuchuang.dir/main.cpp.s"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/war/myCode/tuchuang/src/main.cpp -o CMakeFiles/tuchuang.dir/main.cpp.s

src/CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o: src/CMakeFiles/tuchuang.dir/flags.make
src/CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o: ../src/conf/TcConf.cpp
src/CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o: src/CMakeFiles/tuchuang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/war/myCode/tuchuang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o -MF CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o.d -o CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o -c /home/war/myCode/tuchuang/src/conf/TcConf.cpp

src/CMakeFiles/tuchuang.dir/conf/TcConf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tuchuang.dir/conf/TcConf.cpp.i"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/war/myCode/tuchuang/src/conf/TcConf.cpp > CMakeFiles/tuchuang.dir/conf/TcConf.cpp.i

src/CMakeFiles/tuchuang.dir/conf/TcConf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tuchuang.dir/conf/TcConf.cpp.s"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/war/myCode/tuchuang/src/conf/TcConf.cpp -o CMakeFiles/tuchuang.dir/conf/TcConf.cpp.s

src/CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o: src/CMakeFiles/tuchuang.dir/flags.make
src/CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o: ../src/db/redis/RedisPool.cpp
src/CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o: src/CMakeFiles/tuchuang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/war/myCode/tuchuang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o -MF CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o.d -o CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o -c /home/war/myCode/tuchuang/src/db/redis/RedisPool.cpp

src/CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.i"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/war/myCode/tuchuang/src/db/redis/RedisPool.cpp > CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.i

src/CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.s"
	cd /home/war/myCode/tuchuang/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/war/myCode/tuchuang/src/db/redis/RedisPool.cpp -o CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.s

# Object files for target tuchuang
tuchuang_OBJECTS = \
"CMakeFiles/tuchuang.dir/main.cpp.o" \
"CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o" \
"CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o"

# External object files for target tuchuang
tuchuang_EXTERNAL_OBJECTS =

src/tuchuang: src/CMakeFiles/tuchuang.dir/main.cpp.o
src/tuchuang: src/CMakeFiles/tuchuang.dir/conf/TcConf.cpp.o
src/tuchuang: src/CMakeFiles/tuchuang.dir/db/redis/RedisPool.cpp.o
src/tuchuang: src/CMakeFiles/tuchuang.dir/build.make
src/tuchuang: src/CMakeFiles/tuchuang.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/war/myCode/tuchuang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable tuchuang"
	cd /home/war/myCode/tuchuang/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tuchuang.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/tuchuang.dir/build: src/tuchuang
.PHONY : src/CMakeFiles/tuchuang.dir/build

src/CMakeFiles/tuchuang.dir/clean:
	cd /home/war/myCode/tuchuang/build/src && $(CMAKE_COMMAND) -P CMakeFiles/tuchuang.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/tuchuang.dir/clean

src/CMakeFiles/tuchuang.dir/depend:
	cd /home/war/myCode/tuchuang/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/war/myCode/tuchuang /home/war/myCode/tuchuang/src /home/war/myCode/tuchuang/build /home/war/myCode/tuchuang/build/src /home/war/myCode/tuchuang/build/src/CMakeFiles/tuchuang.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/tuchuang.dir/depend

