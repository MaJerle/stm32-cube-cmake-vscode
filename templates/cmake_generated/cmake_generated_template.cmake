cmake_minimum_required(VERSION 3.22)

# Core MCU flags, CPU, instruction set and FPU setup
set(cpu_PARAMS ${cpu_PARAMS}
    -mthumb

    # Other parameters
    # -mcpu, -mfloat, -mfloat-abi, ...
    {{sr:cpu_params}}
)

# Set linker script
set(linker_script_SRC ${linker_script_SRC}
    {{sr:linker_script_SRC}}
)

# Sources
set(sources_SRCS ${sources_SRCS}
    {{sr:sources_SRCS}}
)

# Include directories
set(include_c_DIRS ${include_c_DIRS}
    {{sr:include_c_DIRS}}
)
set(include_cxx_DIRS ${include_cxx_DIRS}
    {{sr:include_cxx_DIRS}}
)
set(include_asm_DIRS ${include_asm_DIRS}
    {{sr:include_asm_DIRS}}
)

# Symbols definition
set(symbols_c_SYMB ${symbols_c_SYMB}
    {{sr:symbols_c_SYMB}}
)
set(symbols_cxx_SYMB ${symbols_cxx_SYMB}
    {{sr:symbols_cxx_SYMB}}
)
set(symbols_asm_SYMB ${symbols_asm_SYMB}
    {{sr:symbols_asm_SYMB}}
)

# Link directories
set(link_DIRS ${link_DIRS}
    {{sr:link_DIRS}}
)

# Link directories
set(link_LIBS ${link_LIBS}
    {{sr:link_LIBS}}
)
