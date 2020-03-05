# cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# set(CMAKE_SYSTEM_NAME Generic)

# set(TI_CGT_PATH "/Applications/ti/ti-cgt-arm_19.6.0.STS")

# #specify the cross compiler
# SET(CMAKE_C_COMPILER   ${TI_CGT_PATH}/bin/armcl)
# SET(CMAKE_CXX_COMPILER ${CMAKE_C_COMPILER})
# SET(CMAKE_ASM_COMPILER ${TI_CGT_PATH}/bin/armasm)

# # skip compiler tests
# set(CMAKE_ASM_COMPILER_WORKS 1)
# set(CMAKE_C_COMPILER_WORKS   1)
# set(CMAKE_CXX_COMPILER_WORKS 1)
# set(CMAKE_DETERMINE_ASM_ABI_COMPILED 1)
# set(CMAKE_DETERMINE_C_ABI_COMPILED   1)
# set(CMAKE_DETERMINE_CXX_ABI_COMPILED 1)

# # Add the default include and lib directories for tool chain
# include_directories(${TI_CGT_PATH}/include)
# link_directories(${TI_CGT_PATH}/lib)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SafeTIDiagnosticLibrary/2.3.1)
# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SafeTIDiagnosticLibrary/2.3.1/libs)

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(yaboi LANGUAGES C ASM)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SafeTIDiagnosticLibrary/2.4.0)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SafeTIDiagnosticLibrary/2.4.0/libs)

#######################################
#   THIS BLOCK FINDS ALL TEST FILES   #
#######################################
MACRO(DISCOVER_ALL_C_SORUCE_FILES return_list)
    FILE(GLOB_RECURSE new_list ${CMAKE_CURRENT_SOURCE_DIR}/source/*.c)
    SET(file_list "")
    FOREACH(file_path ${new_list})
        string( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" relative_path ${file_path} )
        SET(file_list ${file_list} ${relative_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${file_list})
ENDMACRO()
DISCOVER_ALL_C_SORUCE_FILES(C_SOURCES)
message(STATUS "ALL DISCOVERED TEST FILES: ${C_SOURCES}")

#######################################
#   THIS BLOCK FINDS ALL TEST FILES   #
#######################################
MACRO(DISCOVER_ALL_ASM_SORUCE_FILES return_list)
    FILE(GLOB_RECURSE new_list ${CMAKE_CURRENT_SOURCE_DIR}/source/*.asm)
    SET(file_list "")
    FOREACH(file_path ${new_list})
        string( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" relative_path ${file_path} )
        SET(file_list ${file_list} ${relative_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${file_list})
ENDMACRO()
DISCOVER_ALL_ASM_SORUCE_FILES(ASM_SOURCES)
message(STATUS "ALL DISCOVERED TEST FILES: ${ASM_SOURCES}")
set_property(SOURCE PROPERTY LANGUAGE ASM ${ASM_SOURCES})

add_executable( blinky.out ${C_SOURCES} ${ASM_SOURCES} )
target_link_libraries(blinky.out ${TI_CGT_PATH}/lib/libc.a)
set_target_properties(blinky.out PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/source/HL_sys_link.cmd)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} " --run_linker ${CMAKE_SOURCE_DIR}/source/HL_sys_link.cmd")