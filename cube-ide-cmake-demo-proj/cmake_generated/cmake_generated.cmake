cmake_minimum_required(VERSION 3.22)

# Core MCU flags, CPU, instruction set and FPU setup
set(cpu_PARAMS ${cpu_PARAMS}
    -mthumb

    # Other parameters
    # -mcpu, -mfloat, -mfloat-abi, ...
    -mcpu=cortex-m7
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

# Set linker script
set(linker_script_SRC ${linker_script_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H735IGKX_FLASH.ld
)

# Sources
set(sources_SRCS ${sources_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32h7xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/system_stm32h7xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Startup/startup_stm32h735igkx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_utils.c
)

# Include directories
set(include_c_DIRS ${include_c_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
)
set(include_cxx_DIRS ${include_cxx_DIRS}
    
)
set(include_asm_DIRS ${include_asm_DIRS}
    
)

# Symbols definition
set(symbols_c_SYMB ${symbols_c_SYMB}
    "DEBUG"
    "STM32H735xx"
    "USE_FULL_LL_DRIVER"
    "HSE_VALUE=25000000"
    "HSE_STARTUP_TIMEOUT=100"
    "LSE_STARTUP_TIMEOUT=5000"
    "LSE_VALUE=32768"
    "EXTERNAL_CLOCK_VALUE=12288000"
    "HSI_VALUE=64000000"
    "LSI_VALUE=32000"
    "VDD_VALUE=3300"
)
set(symbols_cxx_SYMB ${symbols_cxx_SYMB}
    
)
set(symbols_asm_SYMB ${symbols_asm_SYMB}
    "DEBUG"
)

# Link directories
set(link_DIRS ${link_DIRS}
    
)

# Link directories
set(link_LIBS ${link_LIBS}
    
)
