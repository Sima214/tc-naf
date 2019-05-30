## Initialize cmake.
cmake_minimum_required( VERSION 3.9.4 )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
if( NOT CMAKE_BUILD_TYPE )
	message( STATUS "Setting build type to 'Release' as none was specified." )
	set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
	## Set the possible values of build type for cmake-gui
	SET_PROPERTY( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo" )
endif()

include( CheckIPOSupported )

## Configure project and settings.
set( PROJECT_VERSION_MAJOR 0 )
set( PROJECT_VERSION_MINOR 1 )
set( PROJECT_VERSION_RELEASE 1 )
set( PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_RELEASE}" )
set( TCNAF_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
set( TCNAF_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
set( TCNAF_VERSION_RELEASE ${PROJECT_VERSION_RELEASE} )
set( TCNAF_VERSION ${PROJECT_VERSION} )
set( PROJECT_NAME "tcnaf")
project( ${PROJECT_NAME} VERSION ${TCNAF_VERSION} DESCRIPTION "A toy compiler, part of a university project." )

## Include SS-CE.
message( STATUS "Importing SS-CE..." )
set( MODULE_TESTING OFF )
set( MODULE_AI OFF )
set( MODULE_AI_SEARCH OFF )
set( MODULE_AI_SEARCH_UNINFORMED OFF )
set( MODULE_AI_SEARCH_INFORMED OFF )
add_subdirectory("SS-CE" "SS-CE")
message( STATUS "Done importing SS-CE." )
include_directories( ${SSCE_EXPORT_INCLUDE} )

## Generate source.


## Find sources.
file( GLOB_RECURSE SRCS "${PROJECT_SOURCE_DIR}/src/*.c*" )
include_directories( "${PROJECT_SOURCE_DIR}/src/" )

## Define the executable
add_executable( ${PROJECT_NAME} ${SRCS} )
target_link_libraries( ${PROJECT_NAME} ssce )

## Compiler options.
set( CMAKE_C_STANDARD 99 )
set( CMAKE_CXX_STANDARD 11 )
if( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
    message( "Debug build detected, LTO not enabled." )
else()
    check_ipo_supported( RESULT ipo_supported OUTPUT ipo_error )
    if( ${ipo_supported} )
        message( STATUS "IPO/LTO is supported and enabled." )
        set_property( TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE )
    else()
        message(STATUS "IPO/LTO is not supported: <${ipo_error}>.")
    endif()
endif()
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-expansion-to-defined -fvisibility=hidden" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexec-charset=UTF-8" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -masm=intel" )