# Required Cmake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

# Project name and version
PROJECT ("CPSim_ROS2")
SET ( PROJECT_VERSION_MAJOR 0 )
SET ( PROJECT_VERSION_MINOR 1 )

# Build configuration and makefile creation
SET ( CMAKE_BUILD_TYPE Debug )
SET ( CMAKE_VERBOSE_MAKEFILE true )

# Source file list for Build
SET ( OUTPUT_ELF
        "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.out"
        )
SET ( SRC_FILES
        # set main file first
        Engine/main.cpp 
        # set Initializer files 
        Engine/Initializer.h Engine/Initializer.cpp
        # set Logger files
        Engine/Logger.h Engine/Logger.cpp
        # set offline_guider files
        Engine/OfflineGuider.h Engine/OfflineGuider.cpp
        # set ScheduleGenerator files
        Engine/ScheduleGenerator.h Engine/ScheduleGenerator.cpp 
        # set Executor files
        Engine/Executor.h Engine/Executor.cpp 
        # set ECU files
        Engine/ECU.h Engine/ECU.cpp
        # set Task files
        Engine/Task.h Engine/Task.cpp
        # set Job files
        Engine/Job.h Engine/Job.cpp
        # set Utils files
        Engine/Utils.h Engine/Utils.cpp
        )

# Compiler Information
SET ( CMAKE_C_COMPILER "g++" )

# Common Header Includes
# INCLUDE_DIRECTORIES ( include driver/include )

# Compile Options, Linking Options
# -g : Debug
# -wall : warn to every vague codes
 ADD_COMPILE_OPTIONS ( -std=c++1z )
# SET ( CMAKE_EXE_LINKER_FLAGS "-static -Wl, --gc-sections" )

# Common Linking Libraries
# LINK_LIBRARIES( dl pthread )

# "Debug" Configruation options
SET ( CMAKE_C_FLAGS_DEBUG "-DDEBUG -DC_FLAGS" )
SET ( CMAKE_EXE_LINKER_FLAGS_DEBUG "-DDEBUG -DLINKER_FLAGS" )
 
# "Release" configuration options
SET ( CMAKE_C_FLAGS_RELEASE "-DRELEASE -DC_FLAGS" )
SET ( CMAKE_EXE_LINKER_FLAGS_RELEASE "-DRELEASE -DLINKER_FLAGS" )

# Output Directory
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE} )
SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib )
SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib )

# Add source files at here using this formats.
add_executable ( ${OUTPUT_ELF} ${SRC_FILES})