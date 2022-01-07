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
require 'lib/outputter'
require 'lib/sanitizer'
require 'lib/string_collector'
require 'lib/text_entries_xml_reader'
require 'lib/typographies_xml_reader'
require 'lib/xml_reader'

class Generator
  def run(file_name, output_path, text_output_path, font_asset_path, data_format, remap_identical_texts, generate_binary_translations, generate_binary_fonts, framebuffer_bpp, generate_font_format)
    xml_doc = XMLReader.new.read(file_name)
    text_entries = TextEntriesXMLReader.new(xml_doc).run
    typographies = TypographiesXMLReader.new(xml_doc).run
    Sanitizer.new(text_entries, typographies, framebuffer_bpp).run
    if remap_identical_texts=='yes'
      string_indices, characters = StringCollector.new(text_entries, typographies).run
    end
    Outputter.new(string_indices, characters, text_entries, typographies, text_output_path, output_path, font_asset_path, data_format, remap_identical_texts, generate_binary_translations, generate_binary_fonts, generate_font_format).run
  end
end
