#########################################################################
#                                                                       #
#     BUILD SRC AS A NATIVE LIBRARY THAT CAN BE LINKED TO THE TESTS     #
#                                                                       #
#########################################################################
cmake_minimum_required(VERSION 3.5)

# If we call cmake from CI with -DAppVersion=v1.2.3 we use it for semantic versioning,
# otherwise we should report the build timestamp for local builds.
# SET(AppVersion "v0.0.0" CACHE STRING "optional semantic version")
# string(REPLACE "v" "" SEMANTIC_APP_VERSION ${AppVersion})

# message(STATUS "building using semantic version " ${SEMANTIC_APP_VERSION})

# project(hello-world-tests VERSION ${SEMANTIC_APP_VERSION} LANGUAGES C CXX)
project(hello-world-tests LANGUAGES C CXX)
set(LIB_NAME ourSampleApp)

add_compile_definitions(TESTING)

# configure_file(CmakeConfig.h.in CmakeConfig.h)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# we use this to get code coverage
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# go get gtest and pull it in
include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
    )

# these are for use in file discovery done below
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../source")


#########################################
#   THIS BLOCK FINDS ALL SOURCE FILES   #
#########################################
set(SEARCH_DIR "${SRC_DIR}")
message(STATUS "SEARCH_DIR: ${SEARCH_DIR}")
MACRO(DISCOVER_ALL_SRC_FILES return_list)
    FILE(GLOB_RECURSE new_list ${SEARCH_DIR}/*.c)
    SET(${return_list} ${new_list})
ENDMACRO()
DISCOVER_ALL_SRC_FILES(APP_SOURCES)
message(STATUS "ALL DISCOVERED SRC FILES: ${APP_SOURCES}")


#########################################
#   THIS BLOCK FINDS ALL HEADER FILES   #
#########################################
MACRO(DISCOVER_ALL_HEADER_FILES return_list)
    FILE(GLOB_RECURSE new_list ${SEARCH_DIR}/*.h)
    SET(${return_list} ${new_list})
ENDMACRO()
DISCOVER_ALL_HEADER_FILES(APP_HEADERS_QUALIFIED_PATHS)
message(STATUS "ALL DISCOVERED HEADER FILES: ${APP_HEADERS_QUALIFIED_PATHS}")


#################################################################
#   THIS BLOCK DISCOVERS ALL DIRECTORIES WITH HEADERS IN THEM   #
#################################################################
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list ${SEARCH_DIR}/*.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()
HEADER_DIRECTORIES(HEADER_DIR_LIST)
message(STATUS "HEADER DIRS: ${HEADER_DIR_LIST}")

#############################################################################
#   THIS BLOCK CONFIGURES THE LIBRARY TO BE BUILT WITH THE ABOVE SETTINGS   #
#############################################################################
add_library(${LIB_NAME} "")
target_sources(
    ${LIB_NAME} # same as library name
    PRIVATE
        ${APP_SOURCES}
    PUBLIC
        ${APP_HEADERS_QUALIFIED_PATHS}
    )
target_include_directories(
    ${LIB_NAME} # same as library name
    PUBLIC
        ${HEADER_DIR_LIST}
    )


###################################################################
#                                                                 #
#     DISCOVER AND BUILD TEST EXECUTABLES (ONE PER TEST FILE)     #
#                                                                 #
###################################################################
set(CMAKE_CXX_STANDARD 14)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mocks)

#######################################
#   THIS BLOCK FINDS ALL TEST FILES   #
#######################################
MACRO(DISCOVER_ALL_TEST_FILES return_list)
    FILE(GLOB_RECURSE new_list ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    SET(file_list "")
    FOREACH(file_path ${new_list})
        string( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" relative_path ${file_path} )
        SET(file_list ${file_list} ${relative_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${file_list})
ENDMACRO()
DISCOVER_ALL_TEST_FILES(TEST_SOURCES)
message(STATUS "ALL DISCOVERED TEST FILES: ${TEST_SOURCES}")


#################################################
#   QUEUE UP AN EXECUTABLE FOR EACH TEST FILE   #
#################################################
foreach( testsourcefile ${TEST_SOURCES} )
    string( REPLACE ".cpp" "" testname ${testsourcefile} )
    GET_FILENAME_COMPONENT(exename ${testname} NAME)
    message(STATUS "TEST NAME: ${exename}")
    add_executable( ${exename} ${testsourcefile} )
    message(STATUS "EXECUTABLE ADDED FOR ${exename}")
    target_include_directories(${exename} PUBLIC "${PROJECT_BINARY_DIR}" )
    message(STATUS "INCLUDES SET FOR ${exename}")
    target_link_libraries( ${exename} gtest_main ${LIB_NAME} )
    message(STATUS "LINKS SET FOR ${exename}")
endforeach( testsourcefile ${TEST_SOURCES} )
