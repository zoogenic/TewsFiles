 cmake_minimum_required(VERSION 2.8)

#--------------------------------------------------------------------------------------------------
# Project name
#--------------------------------------------------------------------------------------------------
set ( PROJECT TewsFiles )
set ( PROJECT_DESCRIPTION "Application for solving E8 Grobner Shirshov base" )
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 0 )
set ( PROJECT_VERSION_PATCH 1 )

string ( TOLOWER ${PROJECT} PROJECT_LC )
project ( ${PROJECT_LC} )
set ( EXECUTABLE ${PROJECT_LC} )

#--------------------------------------------------------------------------------------------------
# Setting CMake  ( -DCMAKE_BUILD_TYPE=[Debug,Release] )
#--------------------------------------------------------------------------------------------------

#--------------------------------------------------------------------------------------------------
# Output directories
#--------------------------------------------------------------------------------------------------
set ( BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin )

set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR} )
set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR} )

set ( CMAKE_CXX_FLAGS "-W -Wall -std=c++11" )
set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3" )
set ( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DBOOST_DISABLE_ASSERTS" )

add_definitions ( "-D_FILE_OFFSET_BITS=64" )

#--------------------------------------------------------------------------------------------------
# Generates file ${PROJECT}Version.cpp
#--------------------------------------------------------------------------------------------------
configure_file ( src/${PROJECT}Version.cpp.in
                     ${PROJECT}Version.cpp )

#--------------------------------------------------------------------------------------------------
# Search paths (-I)
#--------------------------------------------------------------------------------------------------
include_directories ( build
                      src
                    )
#--------------------------------------------------------------------------------------------------
#
#--------------------------------------------------------------------------------------------------
set ( HEADERS_FILES
    src/${PROJECT}Version.h
    
    )

set ( SOURES_FILES
    build/${PROJECT}Version.cpp
    src/tewsfiles.cpp
    )

#--------------------------------------------------------------------------------------------------
#   Adding executables
#--------------------------------------------------------------------------------------------------

add_executable (${EXECUTABLE} ${HEADERS_FILES} ${SOURES_FILES} src/tewsfiles.cpp )
