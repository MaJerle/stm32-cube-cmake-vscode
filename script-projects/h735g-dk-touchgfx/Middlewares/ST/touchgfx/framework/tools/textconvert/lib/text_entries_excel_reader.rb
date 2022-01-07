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
require 'lib/excel_reader'

class TextEntriesExcelReader
  attr_reader :reader

  def initialize(file_name)
    header_row_number = 3
    header_column_number = 2
    @reader = ExcelReader.new(file_name, "Translation", header_row_number, header_column_number)
    @text_entries = TextEntries.new
  end

  def language_capitalization(lang)
    lang_upcase = lang.upcase
    @languages.find { |l| l.upcase == lang_upcase }
  end
  
  def run
    reader.read_header do |header|
      @alignments = header.select { |column| column.match(/^.*-ALIGNMENT$/i) }
      @directions = header.select { |column| column.match(/^.*-DIRECTION$/i) }
      @typographies = header.select { |column| column.match(/^.*-TYPOGRAPHY$/i) }
      @languages = header.select { |column| column.match(/^(\w{1,3})$/i ) }
    end

    # Check for undefined languages in language specific typographies
    @typographies.each do |typography|
      language, _ = typography.upcase.split('-')
      fail "ERROR: Unknown language in column #{language}-TYPOGRAPHY" if !@languages.any?{ |lang| lang.upcase == language }
    end

    # Check for undefined languages in language specific alignments
    @alignments.each do |alignment|
      language, _ = alignment.upcase.split('-')
      fail "ERROR: Unknown language in column #{language}-ALIGNMENT" if not @languages.any?{ |lang| lang.upcase == language }
    end

    # Check for undefined languages in language specific directions
    @directions.each do |direction|
      language, _ = direction.upcase.split('-')
      fail "ERROR: Unknown language in column #{language}-DIRECTION" if not @languages.any?{ |lang| lang.upcase == language }
    end

    reader.read_rows do |row|
      text_id = row[:"Text ID"]
      default_typography = row[:"Typography Name"]
      default_alignment = row[:Alignment]
      if row.exists?(:Direction)
        default_direction = row[:Direction]
      end

      text_id = text_id.strip if text_id
      default_typography = default_typography.strip if default_typography
      default_alignment = default_alignment.strip if default_alignment
      default_direction = default_direction.strip if default_direction

      if text_id && default_typography
        unless text_id.match(/^([0-9a-zA-Z_])*$/)
          fail "ERROR: Illegal characters found in Text ID '#{text_id}'"
        end

        text_entry = TextEntry.new(text_id, default_typography, default_alignment, default_direction)

        @typographies.each do |typography|
          language, _ = typography.split('-')
          language = language_capitalization(language)
          t = row[typography]
          t = t.strip if t
          text_entry.add_typography(language, t)
        end

        @alignments.each do |alignment|
          language, _ = alignment.split('-')
          language = language_capitalization(language)
          a = row[alignment]
          a = a.strip if a
          text_entry.add_alignment(language, a)
        end

        @directions.each do |direction|
          language, _ = direction.split('-')
          language = language_capitalization(language)
          d = row[direction]
          d = d.strip if d
          text_entry.add_direction(language, d)
        end

        @languages.each do |language|
          # Do *not* strip leading/trailing whitespace from translations.
          text_entry.add_translation(language, row[language])
        end

        @text_entries.add(text_entry)
      end
    end
    @text_entries
  end

end
