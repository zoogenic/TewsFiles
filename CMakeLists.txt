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

set ( CMAKE_CXX_FLAGS "-W -Wall -std=c++11 -std=c++1y" )
set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3" )
set ( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DBOOST_DISABLE_ASSERTS" )

add_definitions ( "-D_FILE_OFFSET_BITS=64" )

#--------------------------------------------------------------------------------------------------
# Generates file ${PROJECT}Version.cpp
#--------------------------------------------------------------------------------------------------
configure_file ( src/${PROJECT}Version.cpp.in
                     ${PROJECT}Version.cpp )

#--------------------------------------------------------------------------------------------------
#   Locate boost
#--------------------------------------------------------------------------------------------------
find_package(Boost COMPONENTS program_options filesystem system REQUIRED)

#--------------------------------------------------------------------------------------------------
# Search paths (-I)
#--------------------------------------------------------------------------------------------------
include_directories ( build
                      src
                    )
#--------------------------------------------------------------------------------------------------
#
#--------------------------------------------------------------------------------------------------
set ( HEADERS
    src/${PROJECT}Version.h
    
    )

set ( SOURCES
    build/${PROJECT}Version.cpp
    src/tewsfiles.cpp
    )

#--------------------------------------------------------------------------------------------------
#   Libraries list to link with
#--------------------------------------------------------------------------------------------------
set ( LIBRARIES
    curl
    z
    )


#--------------------------------------------------------------------------------------------------
#   Adding executables
#--------------------------------------------------------------------------------------------------

add_executable (${EXECUTABLE} ${HEADERS} ${SOURES} src/tewsfiles.cpp )
target_link_libraries (
    ${EXECUTABLE}
    ${LIBRARIES}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

#--------------------------------------------------------------------------------------------------
#   target distclean
#--------------------------------------------------------------------------------------------------

add_custom_target ( distclean )
add_custom_command( TARGET distclean
    POST_BUILD
    COMMAND make clean && ls | xargs rm -r
    COMMENT "Cleanup all targets"
    VERBATIM
    )
