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
require 'lib/file_io'
require 'lib/emitters/template'
require 'lib/emitters/text_keys_and_languages_hpp'
require 'lib/emitters/texts_cpp'
require 'lib/emitters/languages_cpp'
require 'lib/emitters/languages_bin'
require 'lib/emitters/unicodes_txt'
require 'lib/emitters/fonts_cpp'
require 'lib/emitters/generated_font_cpp'
require 'lib/emitters/generated_font_hpp'
require 'lib/emitters/unmapped_data_font_cpp'
require 'lib/emitters/unmapped_data_font_hpp'
require 'lib/emitters/cached_font_cpp'
require 'lib/emitters/cached_font_hpp'
require 'lib/emitters/font_cache_cpp'
require 'lib/emitters/font_cache_hpp'
require 'lib/emitters/application_font_provider_hpp'
require 'lib/emitters/application_font_provider_cpp'
require 'lib/emitters/typed_text_database_hpp'
require 'lib/emitters/typed_text_database_cpp'

class Outputter
  def initialize(string_indices, characters, text_entries, typographies, localization_output_directory, fonts_output_directory, font_asset_path, data_format, remap_identical_texts, generate_binary_translations, generate_binary_fonts, generate_font_format)
    @string_indices = string_indices #dictionary of all string indices into the characters array
    @characters = characters         #one array of the needed strings in optimal order
    @text_entries = text_entries
    @typographies = typographies
    @localization_output_directory = localization_output_directory
    @fonts_output_directory = fonts_output_directory
    @font_asset_path = font_asset_path
    @data_format = data_format
    @remap_identical_texts = remap_identical_texts
    @generate_binary_translations = generate_binary_translations
    @generate_binary_fonts = generate_binary_fonts
    @generate_font_format = generate_font_format
  end

  def run

    [ GeneratedFontHpp,
      GeneratedFontCpp,
      UnmappedDataFontHpp,
      UnmappedDataFontCpp,
      CachedFontHpp,
      CachedFontCpp,
      FontCacheHpp,
      FontCacheCpp,
      UnicodesTxt ].each { |template| template.new(@text_entries, @typographies, @fonts_output_directory).run }

    [ ApplicationFontProviderCpp,
      ApplicationFontProviderHpp ].each { |template| template.new(@text_entries, @typographies, @fonts_output_directory, @generate_font_format).run }

    [ TextKeysAndLanguages,
      TypedTextDatabaseHpp].each { |template| template.new(@text_entries, @typographies, @localization_output_directory).run }

    TypedTextDatabaseCpp.new(@text_entries, @typographies, @localization_output_directory, @generate_binary_translations, @generate_font_format).run

    TextsCpp.new(@characters, @text_entries, @typographies, @localization_output_directory, @remap_identical_texts, @generate_binary_translations).run

    LanguagesCpp.new(@string_indices, @text_entries, @localization_output_directory, @remap_identical_texts, @generate_binary_translations).run

    FontsCpp.new(@text_entries, @typographies, @fonts_output_directory, @font_asset_path, @data_format, @generate_binary_fonts, @generate_font_format).run

    if @generate_binary_translations.downcase == 'yes'
      [ LanguagesBin ].each { |template| template.new(@text_entries, @typographies, @localization_output_directory).run }
    end
  end
end

