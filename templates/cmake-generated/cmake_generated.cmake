cmake_minimum_required(VERSION 3.22)

project({{sr:project_name}})

# Core MCU flags, CPU, instruction set and FPU setup
set(cpu_PARAMS
    -mthumb

    # Other parameters
    # -mcpu, -mfloat, -mfloat-abi, ...
    {{sr:cpu_params}}
)

# Set linker script
set(linker_script_SRC {{sr:linker_script_SRC}})

# Source files
{{sr:set_source_folder_files}}

# Include directories
set(include_c_DIRS
    {{sr:include_c_DIRS}}
)
set(include_cxx_DIRS
    {{sr:include_cxx_DIRS}}
)
set(include_asm_DIRS
    {{sr:include_asm_DIRS}}
)

# Symbols definition
set(symbols_c_SYMB
    {{sr:symbols_c_SYMB}}
)
set(symbols_cxx_SYMB
    {{sr:symbols_cxx_SYMB}}
)
set(symbols_asm_SYMB
    {{sr:symbols_asm_SYMB}}
)

# Link directories setup
# Must be before executable is added
set(link_DIRS
    {{sr:link_DIRS}}
)

# Executable files
add_executable(${EXECUTABLE} {{sr:set_source_folder_files_variables}})
