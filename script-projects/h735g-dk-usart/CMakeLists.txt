cmake_minimum_required(VERSION 3.22)

message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

#
# Core project settings
#
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
project(h735g-dk-usart)
enable_language(C CXX ASM)
message("Build type: "              ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)

#
# Core MCU flags, CPU, instruction set and FPU setup
#
set(CPU_PARAMETERS
    -mthumb
    # Other parameters
    # -mcpu, -mfloat, -mfloat-abi, ...
    -mcpu=cortex-m7
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/STM32H735IGKX_FLASH.ld)
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

#
# Source files
#
set(src_core_src_SRCS 
    ${PROJ_PATH}/Core/Src/main.c
    ${PROJ_PATH}/Core/Src/stm32h7xx_hal_msp.c
    ${PROJ_PATH}/Core/Src/stm32h7xx_it.c
    ${PROJ_PATH}/Core/Src/syscalls.c
    ${PROJ_PATH}/Core/Src/sysmem.c
    ${PROJ_PATH}/Core/Src/system_stm32h7xx.c)

set(src_core_startup_SRCS 
    ${PROJ_PATH}/Core/Startup/startup_stm32h735igkx.s)

set(src_fatfs_app_SRCS 
    ${PROJ_PATH}/FATFS/App/fatfs.c)

set(src_fatfs_target_SRCS 
    ${PROJ_PATH}/FATFS/Target/user_diskio.c)

set(src_middlewares_third_party_fatfs_src_SRCS 
    ${PROJ_PATH}/Middlewares/Third_Party/FatFs/src/diskio.c
    ${PROJ_PATH}/Middlewares/Third_Party/FatFs/src/ff.c
    ${PROJ_PATH}/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c)

set(src_middlewares_third_party_fatfs_src_option_SRCS 
    ${PROJ_PATH}/Middlewares/Third_Party/FatFs/src/option/syscall.c)

set(src_drivers_stm32h7xx_hal_driver_src_SRCS 
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c)

#
# Include directories
#
set(include_c_DIRS
    ${PROJ_PATH}/FATFS/Target
    ${PROJ_PATH}/FATFS/App
    ${PROJ_PATH}/Core/Inc
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    ${PROJ_PATH}/Middlewares/Third_Party/FatFs/src
    ${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${PROJ_PATH}/Drivers/CMSIS/Include
)
set(include_cxx_DIRS
    
)
set(include_asm_DIRS
    
)

#
# Symbols definition
#
set(symbols_c_SYMB
    "DEBUG"
    "USE_HAL_DRIVER"
    "STM32H735xx"
)
set(symbols_cxx_SYMB
    
)
set(symbols_asm_SYMB
    "DEBUG"
)

#
# Link directories setup
# Must be before executable is added
#
set(link_DIRS
    
)
link_directories(${EXECUTABLE} ${link_DIRS})

#
# Executable files
#
add_executable(${EXECUTABLE} 
    ${src_core_src_SRCS}
    ${src_core_startup_SRCS}
    ${src_fatfs_app_SRCS}
    ${src_fatfs_target_SRCS}
    ${src_middlewares_third_party_fatfs_src_SRCS}
    ${src_middlewares_third_party_fatfs_src_option_SRCS}
    ${src_drivers_stm32h7xx_hal_driver_src_SRCS})

#
# Add linked libraries for linker
#
set(link_LIBS
    
)
target_link_libraries(${EXECUTABLE} ${link_LIBS})

#
# Project symbols
#
target_compile_definitions(${EXECUTABLE} PRIVATE
    # Language specific only
    $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
    $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
    $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

    # Configuration specific
    $<$<CONFIG:Debug>:
        DEBUG
    >
    $<$<CONFIG:Release>: >
)

#
# Add include paths for each of the compiler
#
target_include_directories(${EXECUTABLE} PRIVATE
    # Language specific only
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${include_cxx_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>

    # Configuration specific
    $<$<CONFIG:Debug>: >
    $<$<CONFIG:Release>: >
)

# Compiler and linker options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:C>:

    >
    $<$<COMPILE_LANGUAGE:CXX>:
        #-Wno-volatile
        #-Wold-style-cast
        #-Wuseless-cast
        #-Wsuggest-override
    >
    $<$<COMPILE_LANGUAGE:ASM>:
        -x assembler-with-cpp
        -MMD
        -MP
    >
    $<$<CONFIG:Debug>:
        -Og -g3 -ggdb
    >
    $<$<CONFIG:Release>:
        -Og -g0
    >
)

# Setup linker parameters
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -u _printf_float              # STDIO float formatting support (remove if not used)
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)

# Convert to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)

message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")
