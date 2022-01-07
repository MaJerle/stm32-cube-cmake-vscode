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
class TextsCpp < Template
  def initialize(characters, text_entries, typographies, output_directory, remap_identical_texts, generate_binary_translations)
    @characters = characters         #one array of the needed strings in optimal order
    @remap_identical_texts = remap_identical_texts
    @generate_binary_translations = generate_binary_translations
    super(text_entries, typographies, output_directory)
    @cache = {}
  end

  def run
    #build @cache and compare with file if exists
    @cache["remap"] = @remap_identical_texts

    if remap_strings?
      #record language list and strings
      @cache["languages"] = countries
      @cache["characters"] = @characters
    else
      #record list of languages only
      @cache["languages"] = countries
    end

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
      #generate TypedTextDatabase.cpp
      super
    end
  end

  def remap_strings?
    @remap_identical_texts=="yes"
  end
  def generate_binary_files?
    @generate_binary_translations=="yes"
  end
  def countries
    text_entries.languages.map { |language| language.capitalize }
  end
  def countries_texts
    if countries.empty?
      "0"
    else
      countries.map{ |country| "texts#{country}" }.join(",\n    ")
    end
  end
  def is_rtl
    text_entries.is_rtl
  end
  def input_path
    File.join(root_dir,'Templates','Texts.cpp.temp')
  end
  def output_path
    'src/Texts.cpp'
  end
  def cache_file
    File.join(@output_directory, 'cache/TextsCpp.cache')
  end
  def output_filename
    File.join(@output_directory, output_path)
  end
  def all_unicodes
    if @characters.length==0
      return "0 // No characters in application"
    end
    comment = ""
    offset = 0
    initial_offset = 0
    @characters.inject("") do |txt, i|
      last = (offset == @characters.length-1)
      txt << "0x#{i.to_s(16)}#{last ? '' : ','} "
      offset += 1
      if i==0 #end of current word, change line
        txt << "// @#{initial_offset} \"#{comment}\""
        txt << "\n    " unless last
        comment = ""
        initial_offset = offset
      elsif i==2
        comment << "<>"
      elsif i>=32 && i <127
        comment << i.chr
      else
        comment << '?'
      end
      txt
    end
  end
end
