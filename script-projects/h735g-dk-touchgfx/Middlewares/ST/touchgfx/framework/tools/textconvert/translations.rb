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

$:.unshift File.dirname(__FILE__)

require 'lib/translation_io'
require 'lib/version'

WINDOWS_LINE_ENDINGS = "\r\n"
UNIX_LINE_ENDINGS = "\n"
#on windows/mingw file.write will it self translate \n to \r\n, on linux not
LINE_ENDINGS = RUBY_PLATFORM.match(/linux/) ? WINDOWS_LINE_ENDINGS : UNIX_LINE_ENDINGS

def root_dir
  # Get the dirname of this (main.rb) file:
  @root_dir ||= File.dirname(__FILE__)
end

class Main
  def self.banner
    <<-BANNER
Export and import the text database for translation using e.g. Excel

Usage: #{File.basename($0)} {export|import} assets/texts/texts.xml translations.xlsx [language]*

Export: Will create the "translations.xlsx" by extracting the texts for
        all languages from the given assets/texts/texts.xml

Import: Will merge the text translations from "translations.xlsx" into
        the given assets/texts/texts.xml. Only the actual texts are
        imported, not alignment and text direction.

If no languages are specified, all languages will be handled,
otherwise only the specified languages are imported/exported.
BANNER
  end

  abort self.banner if ARGV.count < 3

  option = ARGV.shift
  abort self.banner if option!='import' && option!='export'
  file_name = ARGV.shift # texts.xml
  translation_name = ARGV.shift # translations.xlsx
  languages = ARGV # Remaining arguments are the languages

  begin
    translation_io = TranslationIO.new(file_name, translation_name)
    if option == 'export'
      translation_io.exportExcel(languages)
    else option=='import'
      translation_io.importExcel(languages)
    end

  rescue SystemExit => e

  rescue Exception => e
    STDERR.puts e
    abort "An error occurred during translations #{option}!"
  end
end