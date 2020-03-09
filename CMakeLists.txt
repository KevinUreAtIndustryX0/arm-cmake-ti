cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(yaboi LANGUAGES C ASM)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hal/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SafeTIDiagnosticLibrary/2.4.0)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SafeTIDiagnosticLibrary/2.4.0/libs)
link_libraries(-llibc.a)

###########################################
#   THIS BLOCK FINDS ALL C SOURCE FILES   #
###########################################
MACRO(DISCOVER_ALL_C_SOURCE_FILES return_list)
    FILE(GLOB_RECURSE new_list ${CMAKE_CURRENT_SOURCE_DIR}/SOURCE/*.c)
    SET(file_list "")
    FOREACH(file_path ${new_list})
        string( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" relative_path ${file_path} )
        SET(file_list ${file_list} ${relative_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${file_list})
ENDMACRO()
DISCOVER_ALL_C_SOURCE_FILES(C_SOURCES)
message(STATUS "ALL DISCOVERED SOURCE FILES: ${C_SOURCES}")

########################################
#   THIS BLOCK FINDS ALL HAL C FILES   #
########################################
MACRO(DISCOVER_ALL_C_HAL_FILES return_list)
    FILE(GLOB_RECURSE new_list ${CMAKE_CURRENT_SOURCE_DIR}/hal/*.c)
    SET(file_list "")
    FOREACH(file_path ${new_list})
        string( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" relative_path ${file_path} )
        SET(file_list ${file_list} ${relative_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${file_list})
ENDMACRO()
DISCOVER_ALL_C_HAL_FILES(HAL_C)
message(STATUS "ALL DISCOVERED HAL C FILES: ${HAL_C}")

##########################################
#   THIS BLOCK FINDS ALL HAL ASM FILES   #
##########################################
MACRO(DISCOVER_ALL_ASM_HAL_FILES return_list)
    FILE(GLOB_RECURSE new_list ${CMAKE_CURRENT_SOURCE_DIR}/hal/*.asm)
    SET(file_list "")
    FOREACH(file_path ${new_list})
        string( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" relative_path ${file_path} )
        SET(file_list ${file_list} ${relative_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${file_list})
ENDMACRO()
DISCOVER_ALL_ASM_HAL_FILES(HAL_ASM)
message(STATUS "ALL DISCOVERED HAL ASM FILES: ${HAL_ASM}")
set_property(SOURCE PROPERTY LANGUAGE ASM ${HAL_ASM})

MESSAGE( STATUS "Compile flags for C:         " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "Compile flags for asm:       " ${CMAKE_ASM_FLAGS} )
MESSAGE( STATUS "linker flags for asm:        " ${CMAKE_EXE_LINKER_FLAGS} )
MESSAGE( STATUS "linker executable is         " ${CMAKE_C_LINK_EXECUTABLE} )
MESSAGE( STATUS "here is the compiler you are using " ${CMAKE_C_COMPILER})

add_executable( blinky.out ${C_SOURCES} ${HAL_C} ${HAL_ASM} )
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_SOURCE_DIR}/hal/HL_sys_link.cmd")
add_custom_command(
    TARGET blinky.out
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -I elf32-little -O binary blinky.out blinky.bin
    COMMENT "Converting blinky.out to blinky.bin"
)
add_custom_command(
    TARGET blinky.out
    POST_BUILD
    COMMAND armhex -o blinky.hex blinky.out
    COMMENT "Converting blinky.out to blinky.hex"
)
