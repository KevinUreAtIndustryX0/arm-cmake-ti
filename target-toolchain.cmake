set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Turn off Test building for ARM
set(BUILD_TESTING OFF CACHE INTERNAL "")
if(DEFINED ENV{TI_CGT_PATH})
    set(TI_CGT_PATH "$ENV{TI_CGT_PATH}" CACHE PATH "Ti Arm Compiler root path" FORCE)
else()
    message("TI_CGT_PATH environment variable is not defined!")
endif()

if(NOT CMAKE_FIND_ROOT_PATH)
  set(CMAKE_FIND_ROOT_PATH ${TI_CGT_PATH})
endif()

set(TI_CGT_BIN_PATH ${TI_CGT_PATH}/bin/ CACHE INTERNAL "")

# specify the cross compiler
set(CMAKE_C_COMPILER ${TI_CGT_BIN_PATH}/armcl)
set(CMAKE_CXX_COMPILER ${TI_CGT_BIN_PATH}/armcl)
set(CMAKE_ASM_COMPILER ${TI_CGT_BIN_PATH}/armcl)
set(CMAKE_ASM_COMPILER ${TI_CGT_BIN_PATH}/armcl)
# set(CMAKE_AR ${TI_CGT_BIN_PATH}/armar)
# set(CMAKE_LINKER ${TI_CGT_BIN_PATH}/armlnk)

SET(CMAKE_C_COMPILER_TARGET)

# skip compiler tests
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_ASM_COMPILER_FORCED TRUE)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_ASM_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER_WORKS   1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_DETERMINE_ASM_ABI_COMPILED 1)
set(CMAKE_DETERMINE_C_ABI_COMPILED   1)
set(CMAKE_DETERMINE_CXX_ABI_COMPILED 1)

SET(CMAKE_CXX_COMPILER ${CMAKE_C_COMPILER})

# Add the default include and lib directories for tool chain
include_directories(${TI_CGT_PATH}/include)
link_directories(${TI_CGT_PATH}/lib)

# set target environment
set(CMAKE_FIND_ROOT_PATH ${TI_CGT_PATH})
# "C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7R5 --code_state=32 --float_support=VFPv3D16 -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi -z -m"TMS570LC43_RTOS_LEDs.map" --heap_size=0x800 --stack_size=0x800 -i"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/lib" -i"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="TMS570LC43_RTOS_LEDs_linkInfo.xml" --rom_model --be32 -o "TMS570LC43_RTOS_LEDs.out" $(ORDERED_OBJS)
set(PLATFORM_CONFIG_C_FLAGS "--c11 -mv7R5 --code_state=32 --float_support=VFPv3D16 -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --preproc_with_compile" CACHE STRING "platform config c flags")

# combine flags to C and C++ flags
SET(CMAKE_C_FLAGS "${PLATFORM_CONFIG_C_FLAGS} ${CMAKE_C_FLAGS}" CACHE STRING "platform config c flags")
SET(CMAKE_CXX_FLAGS "${PLATFORM_CONFIG_CXX_FLAGS} ${CMAKE_CXX_FLAGS}" CACHE STRING "platform config c flags")
SET(CMAKE_ASM_FLAGS ${CMAKE_C_FLAGS}  CACHE STRING "platform config asm flags")
set(CMAKE_EXE_LINKER_FLAGS "-mv7R5 --define=DEBUG_PRINT --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi -z -m\"CCU_v2_Firmware.map\" --heap_size=0x800 --stack_size=0x800 --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info=CCU_v2_Firmware_linkInfo.xml --rom_model --be32 " CACHE STRING "platform config linker exe files")
