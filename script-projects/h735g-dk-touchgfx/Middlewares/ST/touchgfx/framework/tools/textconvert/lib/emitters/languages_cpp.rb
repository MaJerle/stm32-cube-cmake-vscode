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

class LanguagesCpp
  def initialize(string_indices, text_entries, output_directory, remap_identical_texts, generate_binary_translations)
    @string_indices = string_indices #dictionary of all string indices into the characters array
    @text_entries = text_entries
    @output_directory = output_directory
    @remap_identical_texts = remap_identical_texts
    @generate_binary_translations = generate_binary_translations
  end
  def run
    # First remove any unused LanguageXX.cpp files (ie. remove
    # LanguageGB.cpp before creating LanguageGb.cpp on windows which
    # ignores case on filenames)
    Dir.glob("#{@output_directory}/src/Language*.cpp").each do |file|
      m = /Language(.*).cpp/.match(file)
      xx = m[1]
      if !@text_entries.languages.any? { |l| l.capitalize == xx }
        File.delete(file) if File.exist?(file)
      end
    end

    @text_entries.languages.each do |language|
      LanguageXxCpp.new(@string_indices, @text_entries, @output_directory, @remap_identical_texts, @generate_binary_translations, language).run
    end
  end
end

class LanguageXxCpp < Template
  Presenter = Struct.new(:text_id, :int_array)

  def initialize(string_indices, text_entries, output_directory, remap_identical_texts, generate_binary_translations, language)
    @string_indices = string_indices #dictionary of all string indices into the characters array
    @remap_identical_texts = remap_identical_texts
    @generate_binary_translations = generate_binary_translations
    @language = language
    super(text_entries, [], output_directory)
    @cache = {}
  end

  def cache_file
    File.join(@output_directory, "cache/LanguageCpp_#{@language.capitalize}.cache")
  end
  def output_filename
    File.join(@output_directory, output_path)
  end
  def texts
    @text_entries.entries.map(&:cpp_text_id)
  end
  def run
    #build cache dictionary
    @cache["remap"] = @remap_identical_texts
    @cache["language"] = @language
    @cache["language_index"] = @text_entries.languages.index(@language)
    if remap_strings?
      #save text ids and index
      list = [] #list of index,textid
      entries.each_with_index do |entry, index|
        list[index] = [string_index(entry), entry.text_id]
      end
      @cache["indices"] = list
    else
      #save texts
      texts = []
      entries.each_with_index do |entry, index|
        texts << [entry.text_id, entry.int_array]
      end
      @cache["texts"] = texts
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
      #generate LanguageXX.cpp
      super
    end
  end

  def remap_strings?
    @remap_identical_texts=="yes"
  end

  def generate_binary_files?
    @generate_binary_translations=="yes"
  end

  def language
    @language
  end

  def entries
    #only generate entries once
    @cached_entries ||=
      begin
        entries = text_entries
        entries = handle_no_entries(entries, "DO_NOT_USE")
        present(entries)
      end
  end

  def entries_texts_const_initialization
    entries.map { |entry| "    #{entry.text_id}_#{language.capitalize}" }.join(",\n")
  end

  def string_index(entry)
    index = @string_indices[entry.int_array]
    index.to_s
  end

  def input_path
    File.join(root_dir,'Templates','LanguageXX.cpp.temp')
  end

  def output_path
    "src/Language#{language.capitalize}.cpp"
  end

  private

  def handle_no_entries(entries, text)
    if entries.empty?
      empty_entry = TextEntry.new(text, "typography")
      empty_entry.add_translation(language, "")
      [empty_entry]
    else
      entries
    end
  end

  def present(entries)
    entries.map do |entry|
      Presenter.new(entry.cpp_text_id, entry.translation_in(language).unicodes)
    end
  end

end
