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

class TextKeysAndLanguages < Template
  def initialize(text_entries, typographies, output_directory)
    super
    @cache = {}
  end
  def countries
    text_entries.languages.map { |language| language.upcase }.join(",\n    ")
  end
  def texts
    text_entries.entries.map(&:cpp_text_id)
  end
  def input_path
    File.join(root_dir,'Templates','TextKeysAndLanguages.hpp.temp')
  end
  def output_path
    'include/texts/TextKeysAndLanguages.hpp'
  end
  def cache_file
    File.join(@output_directory, 'cache/TextKeysAndLanguages.cache')
  end
  def output_filename
    File.join(@output_directory, output_path)
  end
  def run
    @cache["languages"] = text_entries.languages
    @cache["textids"] = texts;

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

    if !File::exists?(output_filename) || new_cache_file || $Force_Generate_TextKeysAndLanguages
      #generate TextKeysAndLanguages.hpp
      super
    end
  end
end
