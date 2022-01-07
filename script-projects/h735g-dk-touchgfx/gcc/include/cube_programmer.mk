stm32cube_programmer_path := STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin
stm32cube_programmer_filename := STM32_Programmer_CLI.exe
stm32cube_programmer_filepath := $(stm32cube_programmer_path)/$(stm32cube_programmer_filename)
stm32cube_loader_relative_path := ExternalLoader/$(stldr)
stm32cube_loader := $(stm32cube_programmer_path)/$(stm32cube_loader_relative_path)

program_files_win32 = /c/Program Files (x86)
program_files_win64 = /c/Program Files

program_files_path := $(program_files_win64)

# fall back to 32bit program files if 64bit is not found
ifeq ("$(shell find "$(program_files_win64)/$(stm32cube_programmer_filepath)" 2> nul )","")
program_files_path := $(program_files_win32)
endif

st_stm32cube_programmer_path := $(program_files_path)/$(stm32cube_programmer_path)
st_stm32cube_programmer_filepath := $(program_files_path)/$(stm32cube_programmer_filepath)
st_stm32cube_external_loader := $(program_files_path)/$(stm32cube_loader)

