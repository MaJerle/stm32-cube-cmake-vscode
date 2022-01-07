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
require 'nokogiri'
require 'lib/typographies'

class TypographiesXMLReader
  def empty_to_nil(str)
    str ? str.strip.empty? ? nil : str.strip : nil
  end

  def initialize(xml_doc)
    @xml_doc = xml_doc
    @typographies = []
  end

  def run
    @xml_doc.search("TextDatabase/Typographies/Typography").each do |typo_node|
      typo_id = empty_to_nil(typo_node["Id"])
      font = empty_to_nil(typo_node["Font"])
      size = empty_to_nil(typo_node["Size"])
      bpp = empty_to_nil(typo_node["Bpp"])
      fallback_character = empty_to_nil(typo_node["FallbackCharacter"])
      wildcard_characters = empty_to_nil(typo_node["WildcardCharacters"])
      wildcard_widget_characters = empty_to_nil(typo_node["WidgetWildcardCharacters"])
      wildcard_character_ranges = empty_to_nil(typo_node["WildcardCharacterRanges"])
      ellipsis_character = empty_to_nil(typo_node["EllipsisCharacter"])

      fail "ERROR: Attribute 'Id' not specified in line #{typo_node.line} in #{typo_node.to_s}" if typo_id.nil?
      fail "ERROR: Attribute 'Font' not specified in line #{typo_node.line} for <Typography Id=\"#{typo_id}\">" if font.nil?
      fail "ERROR: Attribute 'Size' not specified in line #{typo_node.line} for <Typography Id=\"#{typo_id}\">" if size.nil?
      fail "ERROR: Attribute 'Bpp' not specified in line #{typo_node.line} for <Typography Id=\"#{typo_id}\">" if bpp.nil?

      if !typo_id.match(/^([0-9a-zA-Z_])*$/)
        fail "ERROR: Illegal characters found in line #{typo_node.line} for <Typography Id=\"#{typo_id}\">"
      end

      @typographies.push Typography.new(typo_id, font, size.to_i, bpp.to_i, fallback_character, ellipsis_character, wildcard_characters, wildcard_widget_characters, wildcard_character_ranges)
    end
    @typographies
  end
end
