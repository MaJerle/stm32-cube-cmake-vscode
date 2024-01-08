# This is converter generated file, and shall not be touched by user
#
# It is always re-generated if converter script is called multiple times
# Use CMakeLists.txt to apply user changes
cmake_minimum_required(VERSION 3.22)

# Core MCU flags, CPU, instruction set and FPU setup
set(cpu_PARAMS ${cpu_PARAMS}
    -mthumb

    # -mcpu, -mfloat, -mfloat-abi config
    -mcpu=cortex-m7
    -mfloat-abi=hard
    -mfpu=fpv5-d16
)

# Linker script
set(linker_script_SRC ${linker_script_SRC}${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeIDE/STM32H735IGKX_FLASH.ld)

# Sources
set(sources_SRCS ${sources_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/freertos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32h7xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32h7xx_hal_timebase_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32h7xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/system_stm32h7xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/Components/ft5336/ft5336.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/Components/ft5336/ft5336_reg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/Components/mx25lm51245g/mx25lm51245g.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/Components/s70kl1281/s70kl1281.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/STM32H735G-DK/stm32h735g_discovery_bus.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/STM32H735G-DK/stm32h735g_discovery_ospi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/STM32H735G-DK/stm32h735g_discovery_ts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_crc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ospi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeIDE/Application/User/Core/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeIDE/Application/User/Core/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeIDE/Application/User/Startup/startup_stm32h735igkx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H735G-DK.ioc
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/App/app_touchgfx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/ApplicationFontProvider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/CachedFont.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/FontCache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Font_verdana_10_4bpp_0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Font_verdana_20_4bpp_0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Font_verdana_40_4bpp_0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/GeneratedFont.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Kerning_verdana_10_4bpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Kerning_verdana_20_4bpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Kerning_verdana_40_4bpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Table_verdana_10_4bpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Table_verdana_20_4bpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/Table_verdana_40_4bpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/src/UnmappedDataFont.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/gui_generated/src/common/FrontendApplicationBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/gui_generated/src/containers/ScrollList_myContainerBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/gui_generated/src/screen1_screen/Screen1ViewBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/images/src/BitmapDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/images/src/__designer/image_Blue_Buttons_Round_large.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/images/src/__designer/image_Blue_Icons_Add_new_32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/images/src/__designer/image_Dark_Backgrounds_Main_bg_texture_320x240px.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/texts/src/LanguageGb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/texts/src/Texts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/texts/src/TypedTextDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/gui/src/common/FrontendApplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/gui/src/containers/ScrollList_myContainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/gui/src/model/Model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/gui/src/screen1_screen/Screen1Presenter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/gui/src/screen1_screen/Screen1View.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/CortexMMCUInstrumentation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/STM32TouchController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/TouchGFXGPIO.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/TouchGFXHAL.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/generated/OSWrappers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/generated/STM32DMA.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/generated/TouchGFXConfiguration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/generated/TouchGFXGeneratedHAL.cpp
)

# Include directories
set(include_c_DIRS ${include_c_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/Components/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/STM32H735G-DK
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/touchgfx/framework/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/App
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/gui_generated/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/images/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/texts/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/videos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/gui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/generated
)
set(include_cxx_DIRS ${include_cxx_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/Components/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/STM32H735G-DK
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/touchgfx/framework/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/App
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/fonts/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/gui_generated/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/images/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/texts/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/generated/videos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/gui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target
    ${CMAKE_CURRENT_SOURCE_DIR}/TouchGFX/target/generated
)
set(include_asm_DIRS ${include_asm_DIRS})

# Symbols definition
set(symbols_c_SYMB ${symbols_c_SYMB}
    "DEBUG"
    "STM32H735xx"
    "USE_HAL_DRIVER"
)
set(symbols_cxx_SYMB ${symbols_cxx_SYMB}
    "DEBUG"
    "STM32H735xx"
    "USE_HAL_DRIVER"
)
set(symbols_asm_SYMB ${symbols_asm_SYMB}
    "DEBUG"
)

# Link directories
set(link_DIRS ${link_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/touchgfx/lib/core/cortex_m7/gcc
)

# Link libraries
set(link_LIBS ${link_LIBS}:libtouchgfx-float-abi-hard.a)

# Compiler options
set(compiler_OPTS ${compiler_OPTS})

# Linker options
set(linker_OPTS ${linker_OPTS})
