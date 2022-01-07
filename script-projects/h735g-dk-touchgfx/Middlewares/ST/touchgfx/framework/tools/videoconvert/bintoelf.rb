#!env ruby
# Copyright (c) 2018(-2021) STMicroelectronics.
# All rights reserved.
#
# This file is part of the TouchGFX 4.18.1 distribution.
#
# This software is licensed under terms that can be found in the LICENSE file in
# the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
###############################################################################/
require 'fileutils'

###### SETUP ###############
input = ARGV[0]
output = ARGV[1]
bins = Dir[input + "/bin/*.bin"]
FileUtils.mkdir_p("#{output}/obj") #Create obj folder
symbol_file_name = "#{output}/obj/rename_symbols.txt"
###############################################

############### Process files ########################
bins.each do |bin| 
    bin_file_name = File.basename(bin)
    
    puts "Generating elf32 object from binary file #{input}/bin/#{bin_file_name}..."
    `arm-none-eabi-objcopy.exe -B arm --rename-section .data=ExtFlashSection,contents,alloc,load,readonly,data -I binary -O elf32-littlearm #{input}/bin/#{bin_file_name} #{output}/obj/#{bin_file_name}.o`

    #Rename the symbols
    output_folder_name_subst = "#{output+'/bin'}".gsub('.', '_').gsub('/', '_')
    bin_file_name_subst = File.basename(bin).gsub('.', '_').gsub('/', '_')
    str = output_folder_name_subst + "_" + bin_file_name_subst

    content = 
        "_binary_#{str}_start video_#{bin_file_name_subst}_start\n" +
        "_binary_#{str}_end video_#{bin_file_name_subst}_end\n" +
        "_binary_#{str}_size video_#{bin_file_name_subst}_size\n"

    puts "Renaming symbols..."
    FileUtils.mkdir_p(File.dirname(symbol_file_name))
    unless File.exist?(symbol_file_name) && content == File.open(symbol_file_name, 'r') { |f| f.read() }
        File.open(symbol_file_name, 'w') { |f| f.write(content) }
    end

    # Bulk rename using objcopy --redefine-syms
    `arm-none-eabi-objcopy.exe --redefine-syms #{output}/obj/rename_symbols.txt #{output}/obj/#{bin_file_name}.o`
end

#Remove temp file
FileUtils.rm(symbol_file_name)