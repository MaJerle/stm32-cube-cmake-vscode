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
require 'json'

class ApplicationFontProviderCpp < Template
  def initialize(text_entries, typographies, output_directory, generate_font_format)
    super(text_entries, typographies, output_directory)
    @generate_font_format = generate_font_format
    @cache = {}
  end
  def input_path
    File.join(root_dir,'Templates','ApplicationFontProvider.cpp.temp')
  end
  def output_path
    'src/ApplicationFontProvider.cpp'
  end
  def cache_file
    File.join(@output_directory, 'cache/ApplicationFontProvider.cache')
  end
  def output_filename
    File.join(@output_directory, output_path)
  end
  def run
    @cache["typographies"] = typographies.collect{|t| [t.name, t.font_file, t.font_size, t.bpp] }
    @cache["generate_font_format"] = @generate_font_format
    new_cache_file = false
    if not File::exists?(cache_file)
      new_cache_file = true
    else
      #cache file exists, compare data with cache file
      old_cache = JSON.parse(File.read(cache_file))
      new_cache_file = (old_cache != @cache)
    end

    if new_cache_file
      #write new cache file
      FileIO.write_file_silent(cache_file, @cache.to_json)
    end

    if !File::exists?(output_filename) || new_cache_file
      #generate ApplicationFontProvider.cpp
      super
    end
  end
  def font_index(index)
    #map typographies to index of first using same font = font index
    @font_index ||=
      begin
        list = {}
        typographies.each_with_index do |typography, index|
          name = "#{typography.cpp_name}_#{typography.font_size}_#{typography.bpp}bpp"
          if not list[name]
            list[name] = list.size
          end
        end
        list
      end
    typography = typographies[index]
    name = "#{typography.cpp_name}_#{typography.font_size}_#{typography.bpp}bpp"
    @font_index[name]
  end
  def save_flashreader
    @generate_font_format == "1"
  end
end
