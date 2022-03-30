# Convert STM32CubeIDE project to CMake and Visual Studio Code

This tutorial explains steps to effectively develop and debug STM32 application in *Visual Studio Code*, with *CMake* build automation system and *Ninja* build system

> Windows is used for the sake of this tutorial. Similar steps apply for other operating systems too.

## Install STM32CubeIDE

First install [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html).

It is used for 2 purposes:
- You can start new project with integrated graphical configurator STM32CubeMX
- STM32CubeIDE provides essential tools necessary for later use with VSCode
    - ARM none eabi GCC compiler
    - ST-LINK GDBServer for debugging
    - STM32CubeProgrammer for code downloading
    - Folder with SVD files for STM32 MCUs (optional use)

STM32CubeIDE installation adds drivers for ST-Link debug probe too.

### STM32CubeIDE environmental setup

We need to add `3` paths to environmental settings from STM32CubeIDE installation, one path for each of above-mentioned tools.
In case of my computer, using STM32CubeIDE 1.8 (updated through eclipse, hence my actual installation path is still showing version `1.0.2`) paths are defined as:

- GCC compiler: `c:\ST\STM32CubeIDE_1.0.2\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.9-2020-q2-update.win32_2.0.0.202105311346\tools\bin\`
- ST-Link GDB server: `c:\ST\STM32CubeIDE_1.0.2\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.win32_2.0.100.202109301221\tools\bin\`
- STM32Cube Programmer CLI: `c:\ST\STM32CubeIDE_1.0.2\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.0.100.202110141430\tools\bin\`

> Your paths may differ at version numbers.

Verify correct setup with `3` commands in cmd, by running

```
arm-none-eabi-gcc --version
STM32_Programmer_CLI --version
ST-LINK_gdbserver --version
```

Output shall be something similar to

![STM32CubeIDE environment test](docs/images/gcc-prog-gdb-version-test.png)

## Install CMake and ninja

### CMake
Install [CMake](https://cmake.org/).

> During installation, wizard will ask you to add CMake to environmental paths. If you do not select this option, you should add it manually after installation.

### Ninja

Download [Ninja build system](https://github.com/ninja-build/ninja/releases) from Github releases page.
It comes as portable executable, without need to install anything.
However it must be visible at environment level, like all previous tools.

Verify both in cmd, by running
```
cmake --version
ninja --version
```

Output shall be something similar to

![CMake and Ninja verification](docs/images/cmake-ninja-version-test.png)

## Install Visual Studio Code

Install [VSCode](https://code.visualstudio.com/)

![Visual Studio Code first time](docs/images/vscode-first-time.png)

### Installation of VSCode plugins

VSCode is famous of being lightweight and extremely modular with 3rd party extensions.

For Cortex-M debugging with CMake, these extensions are essential:

- `ms-vscode.cpptools`: Syntax highlighting and other core features for C/C++ development
- `ms-vscode.cmake-tools`: CMake core tools, build system generator tool
- `twxs.cmake`: CMake color highlighting
- `marus25.cortex-debug`: Cortex-M debugging extension, mandatory for STM32 debug from VSCode
- `dan-c-underwood.arm`: ARM Assembly syntax highlighter
- `zixuanwang.linkerscript`: GCC Linker script syntax highlighter

To install them in one shot, copy code below to terminal in VSCode

```
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cmake-tools
code --install-extension twxs.cmake
code --install-extension marus25.cortex-debug
code --install-extension dan-c-underwood.arm
code --install-extension zixuanwang.linkerscript
```

> Go to *Terminal -> New Terminal* to open new terminal window

![VSCode installed plugins](docs/images/vscode-plugins-installed.png)

If you do not like command line for installation, extensions are searchable through VSCode GUI.
Once installed, you should have at least these extensions ready for next steps.
![VSCode installed plugins](docs/images/vscode-plugins-installed-preview.png)

## Tools installed successfully

At this point, all the tools are properly installed and you are ready for next steps.

## Create new project with STM32CubeMX or STM32CubeIDE

Before we move to VSCode tutorial with CMake, we need a project to work on it.
Fast, simple and effective is to use STM32CubeMX or STm32CubeIDE tools and start from there, to have a first buildable and executable project.

> I am using STM32H735G-DK board for these tests and STM32CubeIDE for project generation, but any other STM32 board could be used.

Open STM32CubeIDE and start new project
![STM32CubeIDE - 1](docs/images/cubeide-1.png)

Select STM32 MCU - I am selecting STM32H735IG which is used on STM32H735G-DK board
![STM32CubeIDE - 2](docs/images/cubeide-2.png)

Select project name and path, then create project and wait for Pinout view to open
![STM32CubeIDE - 3](docs/images/cubeide-3.png)

LEDs on DK board are connected to PC2 and PC3, active LOW. Pins can be configured in output push-pull or open-drain mode
![STM32CubeIDE - 4 - 1](docs/images/cubeide-4-1.png)

Set pins as outputs with optional labels as `LED1` and `LED2` respectively
![STM32CubeIDE - 4](docs/images/cubeide-4.png)

If you are using `STM32CubeMX`, go to project manager, set project name and be sure `STM32CubeIDE` is selected as `Toolchain`.
![STM32CubeIDE - 5](docs/images/cubeide-5.png)

Go to advanced settings and select `LL` as drivers for generated code
![STM32CubeIDE - 6](docs/images/cubeide-6.png)
    - LL drivers are used in this example for simplicity

Regenerate the project by pressing below button or saving the project with `CTRL + S` shortcut
![STM32CubeIDE - 7](docs/images/cubeide-7.png)

Yellow highlighted files are sources to build, while linker script is in blue
![STM32CubeIDE - 8](docs/images/cubeide-8.png)

You are now ready to compile the project. Hit `CTRL + B` or click on *hammer* icon to start.
STM32CubeIDE well compiled project, as it can be seen on picture below. It is now ready for flashing the MCU+s flash and start debugging.
![STM32CubeIDE - 9](docs/images/cubeide-9.png)

> This is end of first part, where we successfully created our project. At this point we consider project is ready to be transferred to CMake-based build system.

You can continue your development with STM32CubeIDE in the future, add new sources, modify code, compile, flash the binary and debug directly the microcontroller.
This is preferred STM32 development studio, developed and maintained by STMicroelectronics.

## Transfer project to CMake

Aside STM32CubeIDE, developers use different tools for STM32, such as Keil or IAR compilers.

With release of Visual Studio Code, many developers use the tool for many programming languages and fortunately can also develop STM32 applications with single tool.
If you are one of developers liking VSCode, most elegant way to move forward is to transfer STM32CubeIDE-based project to *CMake*, develop code in VSCode and compile with Ninja build system using GCC compiler. It is fast and lightweight.

> Development in VSCode is for intermediate or experienced users. I suggest to all STM32 beginners to stay with STM32CubeIDE toolchain as it will be very easy to move forward and come to VSCode topic later.

Let's start with CMake setup for project description.

### Prepare CMakeLists.txt file

Every CMake-based application requires `CMakeLists.txt` file *in the root directory*, that describes the project and provides input information for build system generation.

> Root CMakeLists.txt file is also called top-level CMake file

Essential things for CMakeLists.txt file we need to provide:

- Toolchain information, such as GCC configuration
- Project name
- Source files to build with compiler, C, C++ or Assembly files
- Setting include paths for compiler to find functions, defines, ... (`-I`)
- Set linker script path for final linking process
- Set compilation defines, or sometimes called *preprocessor defines* (`-D`)
- Cortex-Mxx and floating point settings for instruction set generation

### Open project in VSCode

We will configure all files inside VSCode directly as it has its own editor.

Open STM32CubeMX/STM32CubeIDE generated project's root folder in VSCode.

- Option 1: Go to the folder with explorer, then right click and select `Open in Code`.
- Option 2: Alternatively, open VScode as new empty solution and add folder to it manually. Use `File -> Open Folder...` to open folder
- Option 3: Go to folder with cmd or powershell tool and type `code .` to run command

Final result should look similar to the one below
![VSCode - Folder is open](docs/images/vscode-1.png)

### Toolchain information

As mentioned before, one of the things we need for CMake is toolchain information.
As same toolchain is usually reused among different projects, it is advised to create this part in separate file for easier reuse. These are generic compiler settings and not directly linked to projects itself.

A simple `.cmake` can be used and later reused among projects. I am using name `gcc-arm-none-eabi.cmake` for this tutorial and below is its example:

```cmake
set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Some default GCC settings
# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX                arm-none-eabi-)
set(FLAGS                           "-fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections")
set(CPP_FLAGS                       "-fno-rtti -fno-exceptions -fno-threadsafe-statics")

# Define compiler settings
set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc ${FLAGS})
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++ ${FLAGS} ${CPP_FLAGS})
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
```

Create a file in the root of the folder.
![VSCode - 2 - CMake - Toolchain](docs/images/vscode-2-cmake-toolchain.png)

> If CMake highlighter plugin is installed, VSCode will nicely colorize CMake commands for you

Toolchain setup is complete. You can freely close the file and move to next step.

### Create main CMakeLists.txt file

We need to create main `CMakeLists.txt`, also called *root* CMake file.

> Make sure you really name it `CMakeLists.txt` with correct upper and lowercase characters.

I prepared simple template file for you, that can be reused for all of your projects in the future. You will just need to change things like project name, source files, include paths, etc.

```cmake
cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(your-project-name)
enable_language(C CXX ASM)

#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    -mthumb

    # This needs attention to properly set for used MCU
    -mcpu=cortex-m7
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/path-to-linker-script.ld)
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

#
# List of source files to compile
#
set(sources_SRCS
    # Put here your source files, one in each line, relative to CMakeLists.txt file location
)

#
# Include directories
#
set(include_path_DIRS
    # Put here your include dirs, one in each line, relative to CMakeLists.txt file location
)

#
# Symbols definition
#
set(symbols_SYMB
    # Put here your symbols (preprocessor defines), one in each line
    # Encapsulate them with double quotes for safety purpose
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    -Og -g3 -ggdb
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -u _printf_float                # STDIO float formatting support
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
```

Now we need to fill it properly.
Source files are the same as in STM32CubeIDE project. You can check previous image with highlighted sources in yellow color.

Symbols and include paths can be found in STM32CubeIDE under project settings. `2` pictures below are showing how it is in the case of demo project.

![STM32CubeIDE - include paths](docs/images/cubeide-include-paths.png)
![STM32CubeIDE - symbols](docs/images/cubeide-symbols.png)

Cortex-Mxx setup needs a special attention, especially with floating point setup.
For `STM32H735xx`, settings should be set as below. 
```cmake
set(CPU_PARAMETERS
    -mthumb
    -mcpu=cortex-m7         # Set Cortex-M CPU
    -mfpu=fpv5-d16          # Set Floating point type
    -mfloat-abi=hard        # Hardware ABI mode
)
```
![STM32CubeIDE - MCU settings](docs/images/cubeide-mcu-settings.png)

General rule for settings would be as per table below

|STM32 Family | -mcpu           | -mfpu         | -mfloat-abi |
|-------------|-----------------|---------------|-------------|
| STM32F0     | `cortex-m0`     | `Not used`    | `soft`      |
| STM32F1     | `cortex-m3`     | `Not used`    | `soft`      |
| STM32F2     | `cortex-m3`     | `Not used`    | `soft`      |
| STM32F3     | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32F4     | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32F7 SP  | `cortex-m7`     | `fpv5-sp-d16` | `hard`      |
| STM32F7 DP  | `cortex-m7`     | `fpv5-d16`    | `hard`      |
| STM32G0     | `cortex-m0plus` | `Not used`    | `soft`      |
| STM32G4     | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32H7     | `cortex-m7`     | `fpv5-d16`    | `hard`      |
| STM32L0     | `cortex-m0plus` | `Not used`    | `soft`      |
| STM32L1     | `cortex-m3`     | `Not used`    | `soft`      |
| STM32L4     | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32L5     | `cortex-m33`    | `fpv5-sp-d16` | `hard`      |
| STM32U5     | `cortex-m33`    | `fpv5-sp-d16` | `hard`      |
| STM32WB     | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32WL CM4 | `cortex-m4`     | `Not used`    | `soft`      |
| STM32WL CM0 | `cortex-m0plus` | `Not used`    | `soft`      |

> This table is a subject of potential mistakes, not tested with GCC compiler

> Go to [STM32F7xx official site](https://www.st.com/en/microcontrollers-microprocessors/stm32f7-series.html) and check if your device has single or double precision FPU.

Final CMakeLists.txt file after source files, include paths, MCU core settings and defines are set:
```cmake
cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(STM32H735G-DK-LED)          # Modified
enable_language(C CXX ASM)

#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    -mthumb

    # This needs attention to properly set for used MCU
    -mcpu=cortex-m7                 # Modified
    -mfpu=fpv5-d16                  # Modified
    -mfloat-abi=hard                # Modified
)

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/STM32H735IGKX_FLASH.ld) # Modified
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

#
# List of source files to compile
#
set(sources_SRCS                    # Modified
    ${PROJ_PATH}/Core/Src/main.c
    ${PROJ_PATH}/Core/Src/stm32h7xx_it.c
    ${PROJ_PATH}/Core/Src/syscalls.c
    ${PROJ_PATH}/Core/Src/sysmem.c
    ${PROJ_PATH}/Core/Src/system_stm32h7xx.c
    ${PROJ_PATH}/Core/Startup/startup_stm32h735igkx.s
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_exti.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_gpio.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_pwr.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rcc.c
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_utils.c
)

#
# Include directories
#
set(include_path_DIRS               # Modified
    ${PROJ_PATH}/Core/Inc
    ${PROJ_PATH}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${PROJ_PATH}/Drivers/CMSIS/Include
)

#
# Symbols definition
#
set(symbols_SYMB                    # Modified
    "DEBUG"
    "STM32H735xx"
    "USE_FULL_LL_DRIVER"
    "HSE_VALUE=25000000"
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    -Og -g3 -ggdb
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -u _printf_float                # STDIO float formatting support
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
```

In VSCode, well highlighted, it looks like this
![VSCode - final CMakeLists.txt](docs/images/vscode-3-cmake-final.png)

CMake source files are now created and we are ready to proceed to build *build system input files*, in our case we will run CMake engine to prepare build structure for *Ninja build system*

Open VSCode integrated terminal and run command
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="gcc-arm-none-eabi.cmake" -Bbuild -G Ninja
```

It should well complete the execution with similar output as on picture below, plus a new `build` folder should be added to the project. If CMake cannot generate build instructions for Ninja, you will get list of errors in the same terminal window.
![VSCode - final CMakeLists.txt](docs/images/vscode-3-cmake-run.png)

> Every time you modify CMakeLists.txt file, you have to run above command to re-generate build system instructions, otherwise your file changes are not affected for build system.

### Run CMake command automatically

*CMake-Tools* extension can be configured to create and run aforementioned command automatically on every file modification, but requires some additional steps.

> Personally I highly recommend to do these steps. It will boost your productivity in the future.

Start by creating a new `.vscode/cmake-kits.json` file and copy below text to it. This is a special file name for *CMake-Tools* extension and defines list of *CMake Kits*, or list of compiler settings.
```json
[
    {
      "name": "GCC arm-none-eabi - custom toolchain setup",
      "compilers": {
        "C": "arm-none-eabi-gcc",
        "CXX": "arm-none-eabi-g++"
      },
      "toolchainFile": "gcc-arm-none-eabi.cmake"
    }
]
```
![VSCode - CMake setup - quick start](docs/images/vscode-4-cmake-kits.png)

> Restart VSCode after this step to refresh changes

When you are back, hit `CTRL + SHIFT + P` to open command palette and type `CMake: Quick start`
![VSCode - CMake setup - quick start](docs/images/vscode-5-cmake-quick-start.png)

*CMake-Tools* extension will now get notified that there is `CMakeLists.txt` file and that it must take care of it. It will ask you to pick *CMake kit* (compiler setup to use with `CMakeLists.txt` file).
Previously we created `cmake-kits.json` file with added custom config named `GCC arm-none-eabi - custom toolchain setup`. Extension will use our file to find custom kits.

If you do not see it on the list, force re-scan and try to select kit again.
At the bottom of your VSCode window is *No active kit* that is clickable to change the kit.
![VSCode - CMake setup - quick start](docs/images/vscode-6-scan-for-kits.png)

After rescan process, our custom kit is now available and can be selected
![VSCode - CMake setup - kit selection](docs/images/vscode-7-kit-found.png)

Very good, now that we reached so far, our next step is to again run CMake to generate build configuration for *Ninja*, mainly to test if it works well.

There are `2` ways of executing CMake generation step:

1. Use terminal and manually run command as mentioned before
![VSCode - CMake setup - run cmake build system generation](docs/images/vscode-3-cmake-run.png)
    - Command can be added to `tasks.json` file for future use

2. Or let *CMake-Tools* plugin to [re]generate it for you each time file `CMakeLists.txt` is changed and saved. Open `CMakeLists.txt` file, write & delete a character to mark file as *dirty*, finally save file with `CTRL + S`. *CMake-Tools* extension should run build system generation each time `CMakeLists.txt` file is saved. You should see this in `Output` tab
![VSCode - CMake setup - auto run with Output information](docs/images/vscode-8-cmake-run-auto.png)

For sure you can take a break or a beer at this point, and continue in `5` minutes. You did a great job so far.

### Build project with ninja

Our project is ready for building and linking. Unless CMake build generation step failed, we should have `build` directory ready to invoke *Ninja* compiler.

During CMake generation step, *Ninja* was already selected as build system with `-G Ninja` parameter.
To run actual build of source files with GCC compiler, run `cmake --build "build"` command to execute build using ninja build system.
![VSCode - Ninja build - build finished](docs/images/vscode-9-build-finish.png)

> If it builds well, final step on the output is print of memory use with different sections.

As a result, we got some output in `build` directory:

- `project-name.elf` file with complete executable information
- `project-name.hex` HEX file
- `project-name.bin` BIN file
- `project-name.map` map file

In default configuration, `.hex` and `.bin` files are not generated nor *memory usage* is displayed.
Our prepared `CMakeLists.txt` file includes `POST_BUILD` options, to execute additional commands after successful build.
Code is already in your CMakeLists.txt file, so no need to do anything, just observe.

It executes command to:
- Print used size of each region + final executable memory consumption
- Generate `.hex` file from executable
- Generate `.bin` file from executable

```cmake
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
```

> To disable `.bin` file generation, simply delete `POST_BUILD` line for `.bin` and regenerate CMake build system commands.
> Generating `.bin` files may have a negative effect when memory is split between internal and external flash memories. It may generate very large files (>= 2GB) with plenty of non-used zeros. 

There is a list of useful commands to keep in mind during project development:

- Build changes:  `cmake --build "build"`
- Clean project:  `cmake --build "build" --target clean`
- Re-build project, with clean first: `cmake --build "build" --clean-first -v`
- Flash project: `STM32_Programmer_CLI --connect port=swd --download build/project-name.elf -hardRst`

Its easy to forget full syntax, rather let's create `.vscode/tasks.json` file with commands list, for quick run:
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "cppbuild",
            "label": "Build project",
            "command": "cmake",
            "args": ["--build", "\"build\"", "-j", "8"],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": ["$gcc"],
            "group": {
                "kind": "build",
                "isDefault": true       //This is default task
            }
        },
        {
            "type": "shell",
            "label": "Re-build project",
            "command": "cmake",
            "args": ["--build", "\"build\"", "--clean-first", "-v", "-j", "8"],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": ["$gcc"]
        },
        {
            "type": "shell",
            "label": "Clean project",
            "command": "cmake",
            "args": ["--build", "\"build\"", "--target", "clean"],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": []
        },
        {
            "type": "shell",
            "label": "Flash project",
            "command": "STM32_Programmer_CLI",
            "args": [
                "--connect",
                "port=swd",
                "--download",
                "${command:cmake.launchTargetPath}",
                "-hardRst"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": []
        },
        {
            "type": "shell",
            "label": "Run CMake configuration",
            "command": "cmake",
            "args": [
                "--no-warn-unused-cli",
                "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE",
                "-DCMAKE_BUILD_TYPE:STRING=Debug",
                "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=gcc-arm-none-eabi.cmake",
                "-Bbuild",
                "-G", "Ninja"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": []
        }
    ]
}
```

Tasks defined in `tasks.json` can be invoked in VSCode interface using `Terminal -> Run Task` or with `CTRL + ALT + T` shortcut
![VSCode - Tasks.json file](docs/images/vscode-10-tasks.png)

*Build Project* task is configured as *default*, which will get executed when we run default task, or press shortcut `CTRL + SHIFT + B`.
```json
"group": {
    "kind": "build",
    "isDefault": true
}
```

### CMake build system & build command with single click

*CMake-Tools* is super powerful extension, provides many features for development.
We have explained the hard-way how to move from source `CMakeLists.txt` file to finally fully built `.elf` project file,
but there is another way to speed complete process.

At the bottom of the project, in blue line, is a `Build` button, which essentially does the following:

- Runs cmake command to generate build system (runs only is not already available or modified by user, otherwise skips to next step)
- Runs `ninja` to build changed files and generate output `.elf` file

![VSCode - Generate and build with single click](docs/images/vscode-cmake-tools-build-button.png)

### List project files with CMake-Tools plugin

*CMake-Tools* VSCode plugin comes with very nice feature, that being listing all files in the project.
When project uses files outside *root folder* tree, there is no way to see them in VSCode by default, unless you add another folder to project workspace, but then you *destroy* some of the features listed above.

*CMake-Tools* extension well parses `CMakeLists.txt` file and is able to display all the source files, currently part of the CMake build system generation and later part of GCC build thanks to *Ninja*.
On the left side of the screen, you will find an icon for *CMake* build, marked red on picture below.
![VSCode - List files part of CMake build system generation](docs/images/vscode-cmake-tools-list-files.png)

It draws virtual folder tree according to source (executable) files path listed in `CMakeLists.txt` file.

For the sake of this demonstration purpose, I created a file `demo_file.c`, one folder up from `CMakeLists.txt` location and added it to the project.
After CMake build system generation, we can see virtual file added in *CMake-Tools browser*.
![VSCode - List files part of CMake build system generation](docs/images/vscode-cmake-tools-list-files-ext.png)

Thanks to this feature, we can have a full control over files being part of build and can quickly find files to modify, even if these are outside workspace folder directory.

### GCC Problem matcher

Another nice *Build Project* task parameter is `"problemMatcher": ["$gcc"],` set to GCC, which means that terminal output is parsed against GCC standard format and in case of warnings or errors, it will display nice messages in *Problems* view.
![VSCode - Tasks.json file](docs/images/vscode-10-tasks-1.png)

We reached at the end of CMake configuration and build setup.
You can freely modify C source code and and/remove files from/to project.
This is now fully working GCC-based compilation system running in VSCode.

> Do not forget to regenerate CMake when `CMakeLists.txt` file gets modified, or use *Build* button to do it for you.

### Stop receiving virtual C/C++ errors

As you may have noticed, some lines in C files are red-underlined, reporting a `could not find resource` error, but when compiled, all is working just fine.
![VSCode - Debug session](docs/images/vscode-dbg-1.png)

This is reported by `CppTools` extension as it cannot find resources by default, as Intellisense is not aware of include paths or preprocessor defines.

> It will still compile well as include paths are defined in `CMakeLists.txt`, just VSCode Intellisense editor won't work by default.

To overcome this problem, let's create `.vscode/c_cpp_properties.json` file and copy below text to it
```json
{
    "version": 4,
    "configurations": [
        {
            "name": "STM32",
            "includePath": [],      //Kepp empty, ms-vscode.cmake-tools extension will provide it for you
            "defines": [],          //Keep empty, ms-vscode.cmake-tools extension will provide it for you
            "compilerPath": "",
            "cStandard": "gnu17",
            "cppStandard": "gnu++14",
            "intelliSenseMode": "${default}",

            /* Use this and all the include paths will come from CMake configuration instead */
            "configurationProvider": "ms-vscode.cmake-tools"
        }
    ]
}
```
![VSCode - C/C++ virtual errors](docs/images/vscode-c-cpp-1-file.png)

We provided settings for `C/C++` extension, mainly for Intellisense feature, and configure it in a way to use `CMake-Tools` extension to find include paths and list of defines (preprocessor defined).

No errors are visible anymore and Intellisense is now fully operational.
You can test it by going to one resource (ex. with mouse over a function name), then click `CTRL + left mouse click` command and you should jump to definition location directly.
![VSCode - No errors anymore](docs/images/vscode-c-cpp-2-no-errors.png)

> `.vscode/c_cpp_properties.json` is used for `CppTools` extension purpose.

## Debug project with cortex-debug

Our `.elf` file has been built in previous section and can't wait to be uploaded into MCU flash and executed by *Cortex-M core*.
We will use `Cortex-Debug` extension for debugging purpose, that will also flash firmware for us.

First thing is to create `.vscode/launch.json` file and copy below content to it:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Microcontroller - STLink-V3",
            "cwd": "${workspaceFolder}",        //Path from where commands are executed
            "type": "cortex-debug",             //Debug 
            "executable": "${command:cmake.launchTargetPath}", //or fixed file path: build/project-name.elf
            "request": "launch",                //Use "attach" to connect to target w/o elf download
            "servertype": "stlink",             //Use stlink setup of cortex-M debug
            "device": "STM32H735IG",            //MCU used
            "interface": "swd",                 //Interface setup
            "serialNumber": "",                 //Set ST-Link ID if you use multiple at the same time
            "runToEntryPoint": "main",          //Run to main and stop there
            "svdFile": "STM32H73x.svd",         //SVD file to see reisters
            "v1": false,
            "showDevDebugOutput": "both",

            /* Will get automatically detected if STM32CubeIDE is installed to default directory
               or it can be manually provided if necessary.. */
            //"serverpath": "c:\\ST\\STM32CubeIDE_1.7.0\\STM32CubeIDE\\plugins\\com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.win32_2.0.100.202109301221\\tools\\bin\\ST-LINK_gdbserver.exe",
            //"armToolchainPath": "c:\\ST\\STM32CubeIDE_1.7.0\\STM32CubeIDE\\plugins\\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.9-2020-q2-update.win32_2.0.0.202105311346\\tools\\bin",
            //"stm32cubeprogrammer": "c:\\Program Files\\STMicroelectronics\\STM32Cube\\STM32CubeProgrammer\\bin",

            /* If you use external loader, add additional arguments */
            //"serverArgs": ["--extload", "path/to/ext/loader.stldr"],
        }
    ]
}
```

And you are ready to go! Hit `F5` and you should enter debug session with your MCU.
![VSCode - Debug session](docs/images/vscode-dbg-1.png)

> Be sure to have ST-Link debug probe software at its latest version.

### Debug to main

![VSCode - Debug session - breakpoints - step over - step into](docs/images/vscode-dbg-1.png)

You have full control over stepping and can set breakpoints like you would in STM32CubeIDE.

### MCU registers with SVD

If you have MCU SVD file, add its path in `launch.json` configuration, and you will see all peripheral registers in MCU.
![VSCode - Debug session - SVD](docs/images/vscode-dbg-3.png)

### Memory view

To view memory, open command palette with `CTRL + SHIFT + P` and type `memory`

First select command to view memory
![VSCode - Debug session - Memory command](docs/images/vscode-dbg-4-memory.png)
Select memory start address
![VSCode - Debug session - Memory address](docs/images/vscode-dbg-4-memory-address.png)
And memory length to fetch
![VSCode - Debug session - Memory length](docs/images/vscode-dbg-4-memory-len.png)
Nice view of MCU memory
![VSCode - Debug session - Memory view](docs/images/vscode-dbg-4-memory-view.png)

### Assembly stepping

You can step with assembly instructions

Open Command Palette and type `Cortex` and pick disassembly, then type function to disassemble.
It is possible to later step by step assembly instructions too.

![VSCode - Debug session - Memory view](docs/images/vscode-dbg-5-disassembly.png)

Many other features are available.

## Conclusion

This is all for the tutorial.
We showed how to create first project with STM32CubeIDE or STM32CubeMX to have its structure, sources and graphical configuration, later transferred to VSCode, CMake and Cortex-debug.

Full project from this tutorial is available in `cube-ide-cmake-demo-proj` folder

# Automatic STM32CubeIDE script

Part of this repository is also `converter.py` experimetal script, target being taking location of your STM32CubeIDE generated project as an input, finding `.cproject` and `.project` files and generate appropriate `CMakeLists.txt` file, to allow users to use VSCode environment, fully automatically.

> It is very experimental use case, however it works well for basic projects generated with STM32CubeIDE. It has not been tested extensively for the moment and bugs may still appear.

## Features

- Uses base project folder as input path parameter
- Tries to find and parse .cproject and .project files
- Parses linked files of .c, .cpp or .s types
- Parses "Source directories" and scans for files inside. This is typical STM32CubeMX configuration where no "linked-files" are used, but "source folders" instead
- Supports "build" configuration mode only, release configuration is considered advanced feature
- Supports C, CXX and ASM compilers and linker
- Generates include paths for each compiler type (C, CXX, ASM)
- Generates symbols list for each compiler type (C, CXX, ASM)
- Determines Cortex-Mxx from STM32xx name
- Adjusts FPU and float-ABI settings
- Finds linker script
- Supports static library linkage
- Tested with
  - Simple project generated with STM32CubeMX for STM32H735 and STM32G474
  - More complex project generated with TouchGFX-Designer
    - All types of files
    - C++, C, ASM, static library
    - Linked files
    - Source folders
    . ...
- Experimental purpose only

## Tools needed:

- VSCode with aforementioned extensions
- CMake tool installed and in environment path
- Ninja build system installed and in environment path
- ARM none eabi compiler in environment path (comes with STM32CubeIDE)
- STM32CubeIDE or STM32CubeMX to generate project
- Python 3

> Following aforementioned tutorial will make sure all the tools are installed, except python.

## How to use it

Run script with arguments:
```
python converter.py --path "path1" ["path2" ["pathn", [...]]]
```

As an example, giving demo projects in `script-projects/` dir, script shall be executed as
```
python converter.py --path "script-projects/h735g-dk-touchgfx/" "script-projects/h735g-dk-usart/"
```
`CMakeLists.txt` will be generated in the provided paths, but only if converter is able to find `.project` and `.cproject` files inside project directory

## Known limitations

- No support for dual-core devices
- No support for Cortex-M33 with TrustZone configuration
- Simple parsing of "linker script" field is too simple -> needs stronger processing
- Sometimes *.c and *.cpp from build are included

## Contribution

Do not hesitate to propose changes you believe will improve this script.
It should be a community project, work in synergy with worldwide ideas.
