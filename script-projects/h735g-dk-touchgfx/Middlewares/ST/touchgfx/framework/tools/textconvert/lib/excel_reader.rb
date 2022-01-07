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
require 'roo'
require 'lib/text_entries'

class ExcelRow

  def initialize(excel_file, header, row_number, first_column)
    @excel_file = excel_file
    @header = header
    @row_number = row_number
    @first_column = first_column
  end

  def [](column_header)
    value_at(@row_number, column_number(column_header.to_s))
  end

  def exists?(name)
    !@header[name.to_s.downcase].nil?
  end

  private

  def column_number(name)
    column_index = @header[name.downcase]
    fail "ERROR: #{name} column not found in excel file" if column_index.nil?
    column_index + @first_column
  end

  def value_at(row, col)
    value = @excel_file.cell(row,col).to_s
    if value.empty?
      nil
    else
      check_encoding(value)
      value
    end
  end

  def check_encoding(value)
    puts value if value.force_encoding("UTF-8").valid_encoding? == false
  end

end

class ExcelReader

  def initialize(file_name, sheet, header_row, first_column)
    @excel_file = Roo::Excelx.new file_name
    @sheet = sheet
    @excel_file.default_sheet = sheet
    @header_row = header_row
    @first_column = first_column
    @header = {}
    header.each_with_index do |cell, ix|
      @header[cell.downcase] = ix
    end
  end

  def read_header
    yield header
  end

  def read_rows
    (@header_row + 1).upto(last_row_number) do |row_number|
      yield row(row_number)
    end
  end

  private

  def last_row_number
    @excel_file.last_row
  end

  def header
    @excel_file.row(@header_row).compact.map(&:strip)
  end

  def row(row_number)
    ExcelRow.new(@excel_file, @header, row_number, @first_column)
  end

end
