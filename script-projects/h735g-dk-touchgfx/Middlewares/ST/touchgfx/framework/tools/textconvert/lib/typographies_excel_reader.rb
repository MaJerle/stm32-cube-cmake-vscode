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
require 'lib/typographies'

class TypographiesExcelReader
  attr_reader :reader

  def initialize(file_name)
    header_row_number = 3
    header_column_number = 2
    @reader = ExcelReader.new(file_name, 'Typography', header_row_number, header_column_number)
    @typographies = []
  end

  def run
    reader.read_rows do |row|
      name = row[:'Typography Name']
      font = row[:font]
      size = row[:size]
      bpp = row[:bpp]
      if row.exists?(:'Fallback Character')
        fallback_character = row[:'Fallback Character']
      end
      if row.exists?(:'Wildcard Characters')
        wildcard_characters = row[:'Wildcard Characters']
      end
      if row.exists?(:'Widget Wildcard Characters')
        widget_wildcard_characters = row[:'Widget Wildcard Characters']
      end
      if row.exists?(:'Character Ranges') # New name
        wildcard_ranges = row[:'Character Ranges']
      elsif row.exists?(:'Wildcard Ranges') # Old name
        wildcard_ranges = row[:'Wildcard Ranges']
      end
      if row.exists?(:'Ellipsis Character')
        ellipsis_character = row[:'Ellipsis Character']
      end

      if name
        name = name.strip
        unless name.match(/^([0-9a-zA-Z_])*$/)
          fail "ERROR: Illegal characters found in Text ID '#{name}'"
        end
      end
      font = font.strip if font
      size = size.strip if size
      bpp = bpp.strip if bpp
      fallback_character = fallback_character.strip if fallback_character
      wildcard_characters = wildcard_characters.strip if wildcard_characters
      widget_wildcard_characters = widget_wildcard_characters.strip if widget_wildcard_characters
      wildcard_ranges = wildcard_ranges.strip if wildcard_ranges
      ellipsis_character = ellipsis_character.strip if ellipsis_character

      if name && font && size && bpp
        @typographies.push Typography.new(name, font, size.to_i, bpp.to_i, fallback_character, ellipsis_character, wildcard_characters, widget_wildcard_characters, wildcard_ranges)
      end
    end
    @typographies
  end

end
