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

def root_dir
  # Get the dirname of this (videoconvert.rb) file:
  @root_dir ||= File.dirname(__FILE__)
end

class Main
  def self.banner
    <<-BANNER
Convert video files.

Usage: #{File.basename($0)} {root_folder} asset_folder generated_folder

Example: #{File.basename($0)} assets/videos generated/videos
         will process files in assets/videos and place the result in generated/videos

Example: #{File.basename($0)} /c/Project assets/videos generated/videos
         will process files in /c/Project/assets/videos and place the result in /c/Project/generated/videos
BANNER
  end

  def self.write_file(file_name, content)
    FileUtils.mkdir_p(File.dirname(file_name))
    unless File.exist?(file_name) && content == File.open(file_name, 'r') { |f| f.read() }
    puts "Generating #{file_name}"
      File.open(file_name, 'w') { |f| f.write(content) }
    end
  end


  def self.bin_to_obj(bin, out_dir)

    symbol_file_name = "#{out_dir}/obj/rename_symbols.txt"
    bin_file_name = File.basename(bin)

    puts "Generating elf32 object from binary file #{out_dir}/bin/#{bin_file_name}..."
    `arm-none-eabi-objcopy.exe -B arm --rename-section .data=ExtFlashSection,contents,alloc,load,readonly,data -I binary -O elf32-littlearm \"#{out_dir}/bin/#{bin_file_name}\" \"#{out_dir}/obj/#{bin_file_name}.o\"`

    #Rename the symbols
    output_folder_name_subst = "#{out_dir+'/bin'}".gsub('.', '_').gsub('/', '_')
    bin_file_name_subst = File.basename(bin).gsub('.', '_').gsub('/', '_')
    str = output_folder_name_subst + "_" + bin_file_name_subst

    content =
        "_binary_#{str}_start video_#{bin_file_name_subst}_start\n" +
        "_binary_#{str}_end video_#{bin_file_name_subst}_end\n" +
        "_binary_#{str}_size video_#{bin_file_name_subst}_size\n"

    puts "Renaming symbols..."
    File.open(symbol_file_name, 'w') { |f| f.write(content) }

    # Bulk rename using objcopy --redefine-syms
    `arm-none-eabi-objcopy.exe --redefine-syms #{symbol_file_name} \"#{out_dir}/obj/#{bin_file_name}.o\"`

    #Remove temp file
    FileUtils.rm(symbol_file_name)
  end

  root_dir = '.'
  if ARGV.count == 3
    root_dir = ARGV.shift
  end

  if ARGV.count != 2
    abort self.banner
  end

  Dir.chdir(root_dir) do
    avi_dir = ARGV.shift.gsub('\\', '/')
    out_dir = ARGV.shift.gsub('\\', '/')
    bin_dir = File.join(out_dir, 'bin')
    obj_dir = File.join(out_dir, 'obj')
    avis = Dir[File.join(avi_dir, '**', '*.avi')]

    # Create the output folder:
    FileUtils.mkdir_p(bin_dir)

    # Create mapping from existing .avi files to the .bin files
    avi2bin = {}
    avi2obj = {}
    obj2avi = {}
    bin2avi = {}

    avis.each do |avi|
      bin = File.join(bin_dir, File.basename(avi.gsub(/avi$/i,'bin')))
      obj = File.join(obj_dir, File.basename(avi.gsub(/avi$/i,'bin.o')))
      avi2bin[avi] = bin
      avi2obj[avi] = obj

      if bin2avi[bin]
        abort "Duplication avi files: #{bin2avi[bin]} and #{avi}"
      end

      bin2avi[bin] = avi
      obj2avi[obj] = avi
    end

    # Remove bin files that have no corresponding avi file
    Dir[File.join(bin_dir, '*.bin')].each do |bin|
      if !bin2avi[bin]
        puts "Removing #{bin}"
        FileUtils.rm_f(bin)
      end
    end

    # Remove obj files that have no corresponding avi file
    Dir[File.join(obj_dir, '*.bin.o')].each do |obj|
      if !obj2avi[obj]
        puts "Removing #{obj}"
        FileUtils.rm_f(obj)
      end
    end

    # Now process each avi file
    avi2bin.each do |avi,bin|
      if !File.exist?(bin) || (File.mtime(avi) > File.mtime(bin))
        puts "Generating #{bin}"
        FileUtils.cp(avi, bin)
      end
    end

    # Convert .bin to .obj
    FileUtils.mkdir_p("#{out_dir}/obj")
    avi2obj.each do |avi, obj|
      bin = avi2bin[avi]

      if !File.exist?(obj) || (File.mtime(bin) > File.mtime(obj))
        puts "Converting #{bin} to #{obj}"
        bin_to_obj(bin, out_dir)
      end
    end

    # Generate header file
    hpp_content = ""
    keil_export = ""
    keil_incbin = ""

    avi2bin.each do |avi,bin|
      name = File.basename(bin).gsub('.', '_').gsub('/', '_')
      length = File.size(avi);

      symbol = "video_#{name}"
      start_symbol = "#{symbol}_start"
      length_symbol = "#{symbol}_length"

      hpp_content += "const uint32_t #{length_symbol} = #{length};\n"
      hpp_content += "#ifdef SIMULATOR\n"
      hpp_content += "extern const uint8_t* #{start_symbol};\n"
      hpp_content += "#else\n"
      hpp_content += "extern const uint8_t #{start_symbol}[];\n"
      hpp_content += "#endif\n\n"

      keil_export += "\tEXPORT\t#{start_symbol}\n"
      keil_incbin += "\n#{start_symbol}\n\tINCBIN\t../TouchGFX/#{bin}\n"
    end

    hpp_file = File.join(out_dir, 'include', 'videos', 'VideoDatabase.hpp')
    write_file(hpp_file,
               "// Generated by videoconvert. Please, do not edit!\n\n"+
               "#ifndef TOUCHGFX_VIDEODATABASE_HPP\n"+
               "#define TOUCHGFX_VIDEODATABASE_HPP\n"+
               "\n#include <touchgfx/hal/Types.hpp>\n\n"+
               hpp_content+
               "#endif // TOUCHGFX_VIDEODATABASE_HPP\n")

    project_file = Dir['../*.ioc'].first
    toolchain_is_keil = false # Assume it is not Keil
    if project_file
      # From cubemx_project_selector.rb in touchgfx-cli:
      content = File.read(project_file, :encoding=>'utf-8')
      target_toolchain = content.match(/ProjectManager.TargetToolchain=([\w\s\d\.\-]+)/) || abort("Unable to parse #{File.expand_path(@project_file)}")
      toolchain = target_toolchain.captures.first
      toolchain_is_keil = toolchain.match(/MDK-ARM/) # "ProjectManager.TargetToolchain=MDK-ARM..." is Keil
    end

    keil_path = [out_dir, 'src', 'keil', 'Videos.s']
    keil_file = File.join(keil_path)
    # Only generate Videos.s if there are video files AND we are using Keil:
    if avi2bin.empty? || !toolchain_is_keil
      # Try to remove existing Videos.s file and folder structure (if it is empty)
      begin
        FileUtils.rm(keil_file) if File.exist?(keil_file)
        keil_folder = File.join(keil_path[0...-1])
        FileUtils.rmdir(keil_folder) if File.directory?(keil_folder)
        src_folder = File.join(keil_path[0...-2])
        FileUtils.rmdir(File.join(keil_path[0...-2])) if File.directory?(src_folder);
      rescue
      end
    else
      # There are videos and we are on Keil. Generate Videos.s
      write_file(keil_file,
                 "; Generated by videoconvert. Please, do not edit!\n\n"+
                 "\tAREA\tExtFlashSection, DATA, READONLY\n\n"+
                 keil_export+
                 keil_incbin+
                 "\n\tEND\n")
    end
  end
end