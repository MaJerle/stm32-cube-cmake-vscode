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
# coding: iso-8859-1
require 'nokogiri'
require 'rubyxl'
require 'rubyXL/convenience_methods'
require 'lib/text_entries'
require 'lib/text_entries_xml_reader'
require 'lib/typographies_xml_reader'
require 'lib/xml_reader'

# From https://github.com/weshatheleopard/rubyXL/wiki/How-to
class RubyXL::Cell
  def unlock
    xf = get_cell_xf.dup
    xf.protection = xf.protection&.dup || RubyXL::Protection.new
    xf.protection.locked = false
    xf.apply_protection = true
    self.style_index = workbook.register_new_xf(xf)
  end
end
class RubyXL::Worksheet
  def add_frozen_split(row:, column:)
    worksheetview = RubyXL::WorksheetView.new
    worksheetview.pane = RubyXL::Pane.new(:top_left_cell => RubyXL::Reference.new(row,column),
                                          :y_split => row,
                                          :x_split => column,
                                          :state => 'frozenSplit',
                                          :activePane => 'bottomRight')
    worksheetviews = RubyXL::WorksheetViews.new
    worksheetviews << worksheetview
    self.sheet_views = worksheetviews
  end
end

class TranslationIO
  def initialize(file_name, translation_name)
    xml_doc = XMLReader.new.read(file_name)
    @text_entries = TextEntriesXMLReader.new(xml_doc).run
    @typographies = TypographiesXMLReader.new(xml_doc).run
    @translation_name = translation_name
    @file_name = file_name
    @xml_file_version = xml_doc.at("TextDatabase")["Version"]
  end

  WHITE = 'FFFFFF'
  BLACK = '000000'

  HEADER_BACKGROUND = '4472C4'
  HEADER_FOREGROUND = WHITE
  FIRST_ROW_BACKGROUND = 'B4C6E7'
  FIRST_ROW_FOREGROUND = BLACK
  SECOND_ROW_BACKGROUND = 'D9E1F2'
  SECOND_ROW_FOREGROUND = BLACK

  SHEET_NAME = 'TouchGFX Translation'
  TEXT_ID = 'Text Id'

  def exportExcel(languages)
    workbook = RubyXL::Workbook.new
    worksheet = workbook[0]
    worksheet.sheet_name = SHEET_NAME

    existing_languages = @text_entries.languages
    if languages.empty?
      # Empty string means "put TextID in this column"
      languages = [ '' ] + existing_languages
    else
      # Empty string means "put TextID in this column"
      languages = [''] + languages.map do |lang|
        lang_upcase = lang.upcase
        actual_language = existing_languages.find { |l| l.upcase == lang_upcase }
        fail "ERROR: Unknown export language: #{lang}" if !actual_language
        actual_language
      end
    end
    languages.uniq!
    languages.each_with_index do |lang, column|
      cell = worksheet.add_cell(0, column, lang.empty? ? TEXT_ID : lang)
      cell.change_font_color(HEADER_FOREGROUND)
      cell.change_fill(HEADER_BACKGROUND)
      cell.change_font_bold(true)
      worksheet.change_column_width(column, lang.empty? ? 20 : 30)
    end

    # This line is only needed if the font_size of each cell is to be updated (inside the loop below)
    # typography_map = @typographies.inject({}) { |map,typo| map[typo.name] = typo; map }
    @text_entries.each_with_index do |text, row|
      languages.each_with_index do |lang, column|
        cell = worksheet.add_cell(row+1, column, lang.empty? ? text.text_id : text.translation_in(lang).text)
        cell.change_vertical_alignment('top')
        if (row % 2) == 0
          cell.change_font_color(FIRST_ROW_FOREGROUND)
          cell.change_fill(FIRST_ROW_BACKGROUND)
        else
          cell.change_font_color(SECOND_ROW_FOREGROUND)
          cell.change_fill(SECOND_ROW_BACKGROUND)
        end
        cell.change_font_bold(lang.empty?)
        cell.change_border(:bottom, 'thin')
        if !lang.empty?
          cell.change_border(:right, 'thin')
          cell.change_text_wrap(true)
          # Lines only needed if the font size of each cell is to be updated
          #typography_name = text.typographies[lang] || text.typography
          #cell.change_font_size((typography_map[typography_name].font_size / 1.5).to_i)
          alignment = text.alignments[lang] || text.alignment
          cell.change_horizontal_alignment(alignment.downcase)
          cell.unlock
        end
      end
    end

    worksheet.add_frozen_split(:row => 1, :column => 1)
    worksheet.sheet_protection = RubyXL::WorksheetProtection.new(sheet: true, format_columns: false, format_rows: false)

    workbook.write(@translation_name)
  end

  def importExcel(languages)
    require 'lib/xml_writer'

    workbook = RubyXL::Parser.parse(@translation_name)
    worksheet = workbook.worksheets.find { |sheet| sheet.sheet_name == SHEET_NAME }
    fail "ERROR: \"#{@translation_name}\" does not contain a sheet called \"#{SHEET_NAME}\"" if !worksheet

    header = [] # Collect the header with correctly capitalized languages
    text_id_column = nil # Which column contains the TEXT_ID
    import_columns = [] # Which columns to import
    column = 0
    while column < worksheet[0].size
      if worksheet[0][column]
        lang_cell = worksheet[0][column].value
        if !lang_cell.empty?
          lang_upcase = lang_cell.upcase
          if lang_upcase == TEXT_ID.upcase
            text_id_column = column
            fail "ERROR: Multiple columns contain \"#{TEXT_ID}\"" if header.include?('')
            header << ''
          else
            # Find the language with the correct capitalization
            orig_lang = @text_entries.languages.find { |l| l.upcase == lang_upcase }
            # Fail if all languages should be imported AND the language from the spreadsheet is illegal
            fail "ERROR: Text Database does not contain language \"#{lang_cell}\", create the language in the TouchGFX Designer" if languages.empty? && !orig_lang
            # if no languages specified, import all. Otherwise only import if language is wanted
            if languages.empty? || languages.any? { |l| l.upcase == lang_upcase }
              import_columns += [ column ]
            end
            fail "ERROR: Multiple columns contain translations for language \"#{orig_lang}\"" if header.include?(orig_lang)
            header << orig_lang
          end
        end
      end
      column += 1
    end

    upper_languages = languages.map { |lang| lang.upcase }
    upper_existing_languages = @text_entries.languages.map { |lang| lang.upcase }
    # Did we ask to import a language (on the command line) which does not exist in the spreadsheet?
    fail "ERROR: Unknown language(s) #{(upper_languages - upper_existing_languages)*','}" if !(upper_languages - upper_existing_languages).empty?
    fail "ERROR: Missing column \"#{TEXT_ID}\"" if !text_id_column

    # Row 0 is the header
    row = 1
    all_text_ids = []
    while row < worksheet.sheet_data.rows.size
      if worksheet[row] && worksheet[row][text_id_column]
        text_id = worksheet[row][text_id_column].value
        if text_id && !text_id.empty?
          fail "ERROR: Extra translations of Text Id \"#{text_id}\" given in line #{row}" if all_text_ids.include?(text_id)
          import_columns.each do |column|
            text_entry = @text_entries.text_id(text_id)
            fail "ERROR: The Text Id \"#{text_id}\" in line #{row} does not exist in the database" if !text_entry
            cell = worksheet[row][column]
            cell_text = cell ? cell.value.to_s : ''
            text_entry.add_translation(header[column], cell_text)
            #puts "Setting #{text_id}.#{header[column]} = #{worksheet[row][column].value}"
          end
          all_text_ids << text_id
        end
      end
      row += 1
    end

    all_predefined_ids = @text_entries.all_text_ids
    if !(all_predefined_ids - all_text_ids).empty?
      puts "WARNING: \"#{@translation_name}\" does not contain the following Text Id's: #{(all_predefined_ids - all_text_ids)*', '}"
    end

    XMLWriter.new.run(@file_name, @xml_file_version, @text_entries, @typographies)
  end
end
