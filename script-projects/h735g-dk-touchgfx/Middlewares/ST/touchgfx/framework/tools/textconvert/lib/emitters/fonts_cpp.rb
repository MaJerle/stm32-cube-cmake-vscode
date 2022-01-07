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
class FontsCpp
  def self.font_convert=(font_convert)
    @@font_convert = font_convert
  end

  def initialize(text_entries, typographies, output_directory, font_asset_path, data_format, generate_binary_fonts, generate_font_format)
    @typographies = typographies
    @output_directory = output_directory
    @font_asset_path = font_asset_path
    @data_format = data_format
    @generate_binary_fonts = generate_binary_fonts
    @generate_font_format = generate_font_format
  end
  def run
    unique_typographies = @typographies.map{ |t| Typography.new("", t.font_file, t.font_size, t.bpp, t.fallback_character, t.ellipsis_character) }.uniq

    #remove old Table, Kerning, Font files
    #1. Create a list of font names
    font_names = unique_typographies.collect do |typography|
      "#{typography.cpp_name}_#{typography.font_size}_#{typography.bpp}bpp"
    end
    local_path = "#{@output_directory}/src".gsub('\\','/')

    #2, scan for Kerning files, delete files not using a font in font_names
    Dir["#{local_path}/Kerning_*.cpp"].each do |kerning_file|
      if font_names.none? {|font_name| kerning_file == "#{local_path}/Kerning_#{font_name}.cpp" }
        FileUtils.rm(kerning_file)
      end
    end

    #3, scan for Table files
    Dir["#{local_path}/Table_*.cpp"].each do |table_file|
      if font_names.none? {|font_name| table_file == "#{local_path}/Table_#{font_name}.cpp" }
        FileUtils.rm(table_file)
      end
    end

    #4, scan for Font files, remove unused
    Dir["#{local_path}/Font_*.cpp"].each do |font_file|
      if font_names.none? {|font_name| font_file.match /#{local_path}\/Font_#{font_name}_\d+.cpp/ }
        FileUtils.rm(font_file)
      end
    end

    #5, scan for cache files
    local_path = "#{@output_directory}/cache".gsub('\\','/')
    Dir["#{local_path}/Font_*Cpp.cache"].each do |cache_file|
      if font_names.none? {|font_name| cache_file == "#{local_path}/Font_#{font_name}Cpp.cache" }
        FileUtils.rm(cache_file)
      end
    end

    context_tables_is_generated = {}
    generate_contextual_table = false
    unique_typographies.sort_by { |t| sprintf("%s %04d %d",t.font_file,t.font_size,t.bpp) }.each do |typography|
      fonts_directory = @output_directory
      font_file = "#{@font_asset_path}/#{typography.font_file}"
      font_index = fontmap["getFont_#{typography.cpp_name}_#{typography.font_size}_#{typography.bpp}bpp"]
      fallback_char = typography[:fallback_character]
      fallback_char ||= 0
      ellipsis_char = typography[:ellipsis_character]
      ellipsis_char ||= 0
      byte_align = @data_format.match("A#{typography.bpp}") ? "-ba" : ""
      #generate contextual forms table for font if not already done
      generate_contextual_table = context_tables_is_generated[typography.cpp_name] ? "no" : "yes"
      context_tables_is_generated[typography.cpp_name] = true #set done for next font with this name
      cmd = "\"#{@@font_convert}\" \
-f \"#{font_file}\" \
-i #{font_index} \
-w #{typography.font_size} \
-r #{typography.font_size} \
-o \"#{fonts_directory}\" \
-c \"#{@output_directory}/UnicodeList#{typography.cpp_name}_#{typography.font_size}_#{typography.bpp}.txt\" \
-n \"#{typography.cpp_name}\" \
-b #{typography.bpp} \
-d #{fallback_char} \
-e #{ellipsis_char} \
-ct #{generate_contextual_table} \
-bf #{@generate_binary_fonts} \
-ff #{@generate_font_format} \
#{byte_align}"
      #puts "Command: #{cmd}"
      output = `#{cmd}`.force_encoding('iso-8859-1')
      #puts "FontConverter: #{output}\n"
      if !$?.success?
        puts cmd
        puts output
        fail "ERROR: While generating font from #{font_file}"
      elsif output.match(/WARNING/i)
        puts output
      end
    end
  end

  def fonts
    @fonts ||=
      begin
        @typographies.map{ |t| Typography.new("", t.font_file, t.font_size, t.bpp) }.uniq.collect do |f|
          "getFont_#{f.cpp_name}_#{f.font_size}_#{f.bpp}bpp"
        end
      end
  end

  def fontmap
    @fontmap ||=
      begin
        @fontmap = Hash.new
        fonts.each_with_index do |f, i|
          fontmap[f] = i
        end
        fontmap
      end
  end

end
