cmake_minimum_required(VERSION 3.22)

message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

#
# Core project settings
#
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
project(STM32H735G-DK)
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
    -mcpu=cortex-m7
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/STM32CubeIDE/STM32H735IGKX_FLASH.ld)
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

#
# Source files
#
set(linked_SRCS
    ${PROJ_PATH}/STM32H735G-DK.ioc
	${PROJ_PATH}/Core/Src/system_stm32h7xx.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_crc.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_crc_ex.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ospi.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/list.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
	${PROJ_PATH}/Core/Src/freertos.c
	${PROJ_PATH}/Core/Src/main.c
	${PROJ_PATH}/Core/Src/stm32h7xx_hal_msp.c
	${PROJ_PATH}/Core/Src/stm32h7xx_hal_timebase_tim.c
	${PROJ_PATH}/Core/Src/stm32h7xx_it.c
	${PROJ_PATH}/Drivers/BSP/Components/ft5336/ft5336.c
	${PROJ_PATH}/Drivers/BSP/Components/ft5336/ft5336_reg.c
	${PROJ_PATH}/Drivers/BSP/Components/mx25lm51245g/mx25lm51245g.c
	${PROJ_PATH}/Drivers/BSP/Components/s70kl1281/s70kl1281.c
	${PROJ_PATH}/Drivers/BSP/STM32H735G-DK/stm32h735g_discovery_bus.c
	${PROJ_PATH}/Drivers/BSP/STM32H735G-DK/stm32h735g_discovery_ospi.c
	${PROJ_PATH}/Drivers/BSP/STM32H735G-DK/stm32h735g_discovery_ts.c
	${PROJ_PATH}/TouchGFX/App/app_touchgfx.c
	${PROJ_PATH}/TouchGFX/target/CortexMMCUInstrumentation.cpp
	${PROJ_PATH}/TouchGFX/target/STM32TouchController.cpp
	${PROJ_PATH}/TouchGFX/target/TouchGFXGPIO.cpp
	${PROJ_PATH}/TouchGFX/target/TouchGFXHAL.cpp
	${PROJ_PATH}/TouchGFX/target/generated/OSWrappers.cpp
	${PROJ_PATH}/TouchGFX/target/generated/STM32DMA.cpp
	${PROJ_PATH}/TouchGFX/target/generated/TouchGFXConfiguration.cpp
	${PROJ_PATH}/TouchGFX/target/generated/TouchGFXGeneratedHAL.cpp
	${PROJ_PATH}/TouchGFX/gui/src/common/FrontendApplication.cpp
	${PROJ_PATH}/TouchGFX/gui/src/containers/ScrollList_myContainer.cpp
	${PROJ_PATH}/TouchGFX/gui/src/model/Model.cpp
	${PROJ_PATH}/TouchGFX/gui/src/screen1_screen/Screen1Presenter.cpp
	${PROJ_PATH}/TouchGFX/gui/src/screen1_screen/Screen1View.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/ApplicationFontProvider.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/CachedFont.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/FontCache.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Font_verdana_10_4bpp_0.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Font_verdana_20_4bpp_0.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Font_verdana_40_4bpp_0.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/GeneratedFont.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Kerning_verdana_10_4bpp.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Kerning_verdana_20_4bpp.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Kerning_verdana_40_4bpp.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Table_verdana_10_4bpp.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Table_verdana_20_4bpp.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/Table_verdana_40_4bpp.cpp
	${PROJ_PATH}/TouchGFX/generated/fonts/src/UnmappedDataFont.cpp
	${PROJ_PATH}/TouchGFX/generated/gui_generated/src/common/FrontendApplicationBase.cpp
	${PROJ_PATH}/TouchGFX/generated/gui_generated/src/containers/ScrollList_myContainerBase.cpp
	${PROJ_PATH}/TouchGFX/generated/gui_generated/src/screen1_screen/Screen1ViewBase.cpp
	${PROJ_PATH}/TouchGFX/generated/images/src/BitmapDatabase.cpp
	${PROJ_PATH}/TouchGFX/generated/images/src/__designer/image_Blue_Buttons_Round_large.cpp
	${PROJ_PATH}/TouchGFX/generated/images/src/__designer/image_Blue_Icons_Add_new_32.cpp
	${PROJ_PATH}/TouchGFX/generated/images/src/__designer/image_Dark_Backgrounds_Main_bg_texture_320x240px.cpp
	${PROJ_PATH}/TouchGFX/generated/texts/src/LanguageGb.cpp
	${PROJ_PATH}/TouchGFX/generated/texts/src/Texts.cpp
	${PROJ_PATH}/TouchGFX/generated/texts/src/TypedTextDatabase.cpp
)


# Source files in the same path as ".cproject/.project" but nowhere included in file descriptions
set(all_project_dir_SRCS
    ${PROJ_PATH}/STM32CubeIDE/Application/User/Core/syscalls.c
	${PROJ_PATH}/STM32CubeIDE/Application/User/Core/sysmem.c
	${PROJ_PATH}/STM32CubeIDE/Application/User/Startup/startup_stm32h735igkx.s
)

#
# Include directories
#
set(include_c_DIRS
    ${PROJ_PATH}/Core/Inc
	${PROJ_PATH}/TouchGFX/App
	${PROJ_PATH}/TouchGFX/target/generated
	${PROJ_PATH}/TouchGFX/target
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Inc
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/include
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
	${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
	${PROJ_PATH}/Drivers/CMSIS/Include
	${PROJ_PATH}/Drivers/BSP/Components/Common
	${PROJ_PATH}/Drivers/BSP/STM32H735G-DK
	${PROJ_PATH}/Middlewares/ST/touchgfx/framework/include
	${PROJ_PATH}/TouchGFX/generated/fonts/include
	${PROJ_PATH}/TouchGFX/generated/gui_generated/include
	${PROJ_PATH}/TouchGFX/generated/images/include
	${PROJ_PATH}/TouchGFX/generated/texts/include
	${PROJ_PATH}/TouchGFX/gui/include
	${PROJ_PATH}/TouchGFX/generated/videos/include
)
set(include_cxx_DIRS
    ${PROJ_PATH}/Core/Inc
	${PROJ_PATH}/TouchGFX/App
	${PROJ_PATH}/TouchGFX/target/generated
	${PROJ_PATH}/TouchGFX/target
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Inc
	${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/include
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
	${PROJ_PATH}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
	${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
	${PROJ_PATH}/Drivers/CMSIS/Include
	${PROJ_PATH}/Drivers/BSP/Components/Common
	${PROJ_PATH}/Drivers/BSP/STM32H735G-DK
	${PROJ_PATH}/Middlewares/ST/touchgfx/framework/include
	${PROJ_PATH}/TouchGFX/generated/fonts/include
	${PROJ_PATH}/TouchGFX/generated/gui_generated/include
	${PROJ_PATH}/TouchGFX/generated/images/include
	${PROJ_PATH}/TouchGFX/generated/texts/include
	${PROJ_PATH}/TouchGFX/gui/include
	${PROJ_PATH}/TouchGFX/generated/videos/include
)
set(include_asm_DIRS
    
)

#
# Symbols definition
#
set(symbols_c_SYMB
    "USE_HAL_DRIVER"
	"DEBUG"
	"STM32H735xx"
)
set(symbols_cxx_SYMB
    "USE_HAL_DRIVER"
	"DEBUG"
	"STM32H735xx"
)
set(symbols_asm_SYMB
    "DEBUG"
)

#
# Link directories setup
# Must be before executable is added
#
set(link_DIRS
    ${PROJ_PATH}/Middlewares/ST/touchgfx/lib/core/cortex_m7/gcc
)
link_directories(${EXECUTABLE} ${link_DIRS})

#
# Executable files
#
add_executable(${EXECUTABLE}
    ${linked_SRCS}
    ${all_project_dir_SRCS} 
	)

#
# Add linked libraries for linker
#
set(link_LIBS
    :libtouchgfx-float-abi-hard.a
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
    -T${MCU_LINKER_SCRIPT}${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
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

# Conver to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)

message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")