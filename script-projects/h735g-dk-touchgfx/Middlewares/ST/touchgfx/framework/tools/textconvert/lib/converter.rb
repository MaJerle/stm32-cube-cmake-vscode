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
require 'lib/sanitizer'
require 'lib/text_entries_excel_reader'
require 'lib/typographies_excel_reader'
require 'lib/xml_writer'

class Converter
  def run(file_name, xml_file_name, xml_file_version, framebuffer_bpp)
    text_entries = TextEntriesExcelReader.new(file_name).run
    typographies = TypographiesExcelReader.new(file_name).run
    Sanitizer.new(text_entries, typographies, framebuffer_bpp).run
    XMLWriter.new.run(xml_file_name, xml_file_version, text_entries, typographies)
  end
end
