

# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13...3.30.2)
include(pico_sdk_import.cmake)

#set(PICO_SDK_PATH "/home/tc/pico/pico-sdk")
# Import PICO_SDK_PATH from environment if not already set as a CMake variable
if(NOT DEFINED PICO_SDK_PATH)
	set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
	message("Pico Path: ${PICO_SDK_PATH}")
	if(NOT PICO_SDK_PATH)
		message(FATAL_ERROR "PICO_SDK_PATH is not set. Please set it as an environment variable.")
	endif()
endif()

#set(PICO_PLATFORM rp2350) This gets set automatically based on the board selection.

message(STATUS "Using Pico SDK at: ${PICO_SDK_PATH}")
#set(PROJECT_HOME ${CMAKE_SOURCE_DIR})
#enable exceptions for the project
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
#enable Run Time Type Information
set(PICO_CXX_ENABLE_RTTI 1)
#set(PICO_SDK_HOME ${P})
message("Pico home: ${PICO_SDK_PATH}")

# Configure before sdk init!
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(PICO_DEOPTIMIZED_DEBUG 1)
#	add_compile_options($<$<CONFIG:Debug>:-O0 -g>)
#	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	message("Building with asserts enabled.")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	message("Building with asserts disabled.")
endif()

project(pico-min C CXX ASM)	# CHANGE ME!
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pico_sdk_init()

message(STATUS "Starting configuration.")
include(CMakePrintHelpers)
cmake_policy(SET CMP0054 NEW)	# ensures that quoted strings inside if() are always treated as literal strings, not as variable names.
add_compile_options(-Wno-psabi) # suppress "parameter passing ABI changed" notes

# Handle board revision via a single variable
set(BOARD_REV "A" CACHE STRING "Board revision (0, A, B)")
set_property(CACHE BOARD_REV PROPERTY STRINGS 0 A B)

#TODO: Consider changing the add_compile_definitions to target_compile_definitions. This scopes the command to a directory.
if(BOARD_REV STREQUAL "0")
	add_compile_definitions(BOARD_PICO_MIN_REV_0)
	message(STATUS "Building for pico-min Rev 0")
elseif(BOARD_REV STREQUAL "A")
	add_compile_definitions(BOARD_PICO_MIN_REV_A)
	#	target_compile_definitions(${PROJECT_NAME} PRIVATE BOARD_PICO_MIN_REV_A)
	message(STATUS "Building for pico-min Rev A")
elseif(BOARD_REV STREQUAL "B")
	add_compile_definitions(BOARD_PICO_MIN_REV_B)
	#	target_compile_definitions(${PROJECT_NAME} PRIVATE BOARD_PICO_MIN_REV_B)
	message(STATUS "Building for pico-min Rev B")
else()
	message(FATAL_ERROR "Unknown BOARD_REV: ${BOARD_REV}")
endif()

add_subdirectory(lib/cmdctl)
add_subdirectory(lib/core)
add_subdirectory(lib/devices)
add_subdirectory(lib/drivers)
add_subdirectory(lib/tests)
add_subdirectory(lib/app)
add_subdirectory(src)

message(STATUS "\nAfter add_executable and targets\n")
cmake_print_variables(PROJECT_NAME)
cmake_print_variables(PROJECT_SOURCE_DIR)
cmake_print_variables(PROJECT_BINARY_DIR)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)
cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_CXX_FLAGS_RELEASE)
cmake_print_variables(CMAKE_CXX_FLAGS_DEBUG)
cmake_print_variables(CMAKE_CXX_FLAGS)

cmake_print_variables($ENV{PICO_SDK_PATH})

