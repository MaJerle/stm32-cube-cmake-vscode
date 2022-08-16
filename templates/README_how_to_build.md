# How to build the project

This is a short-version explaining key steps to build project with CMake in VSCode or directly in shell.

Long version: https://github.com/MaJerle/stm32-cube-cmake-vscode

## Pre-requisites

For successful compilation:

* *CMake* and *Ninja-build* tools installed and available in environment path
* *ARM None Eabi GCC* toolchain installed and available in environment path - if you have installed *STM32CubeIDE*, you have GCC already installed there - find it in directory and add it to env path

To further download the built project to STM32 hardware:

* *STM32CubeProgrammer* tool needs to be installed with bin folder in environment path - to have access to *STM32_Programmer_CLI* command - if you have installed *STM32CubeIDE*, you have GCC already installed there - find it in directory and add it to env path

For debugging from inside VSCode

* *ST GDB-Server* tool needs to be installed with bin folder in environment path - you get it together with *STM32CubeIDE* package

## Steps to build the project

* Open terminal window in folder where `CMakeLists.txt` for your project is located
* Run `cmake --preset Debug` to generate build system for ninja using `Debug` preset, that comes with the default `CMakePresets.json` file
* Run `cmake --build --preset Debug` to actually invoke ninja-build and compile with GCC
* Go to `build/Debug` folder - you will find your `.elf` file there (only if build is a pass). This is default build directory for `Debug` preset that comes with the project
* Clean the project with `cmake --build --preset Debug --target clean`
