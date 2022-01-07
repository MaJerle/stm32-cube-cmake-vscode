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
require 'lib/text_entries'

class TextEntriesXMLReader
  def initialize(xml_doc)
    @xml_doc = xml_doc
    @text_entries = TextEntries.new
  end

  def empty_to_nil(str)
    str ? str.strip.empty? ? nil : str.strip : nil
  end

  def get_value_or_nil(a)
    a.nil? ? nil : (a.value.nil? ? nil : a.value.strip)
  end
  
  def run
    texts = @xml_doc.xpath("//Text").map do |text|
      text_id = empty_to_nil(text["Id"])
      default_typography = empty_to_nil(text["TypographyId"])
      default_alignment = empty_to_nil(text["Alignment"])
      default_direction = empty_to_nil(text["Direction"])
      fail "ERROR: Attribute 'Id' not specified in line #{text.line} in #{text.to_s}" if text_id.nil?
      fail "ERROR: Attribute 'TypographyId' not specified in line #{text.line} for <Text Id=\"#{text_id}\">" if default_typography.nil?
      fail "ERROR: Attribute 'Alignment' not specified in line #{text.line} for <Text Id=\"#{text_id}\">" if default_alignment.nil?
      fail "ERROR: Attribute 'Direction' not specified in line #{text.line} for <Text Id=\"#{text_id}\">" if default_direction.nil?
      
      if !text_id.match(/^([0-9a-zA-Z_])*$/)
        fail "ERROR: Illegal characters found in line #{text.line} for <Text Id=\"#{text_id}\">"
      end
      
      fail "ERROR: Translation not specified in line #{text.line} for <Text Id=\"#{text_id}\">" if !text.at("Translation")
      
      { 
        :id => text_id, :typography => default_typography, :alignment => default_alignment, :direction => default_direction, 
        :translations => text.search("Translation").inject({}) do |result, translation|
          
          fail "ERROR: Attribute 'Language' not specified in line #{translation.line} for Translation" if translation.attributes["Language"].nil?
          language = translation.attributes["Language"].value
          
          if translation.text.match(/\n\t|\n /)
            puts "WARNING: Text in line #{translation.line} for <Text Id=\"#{text_id}\"> with Language '#{language}' contains tabs or whitespace indentation"
          end
          
          result[ language.strip ] = { 
            :value => translation.text,
            :typography => get_value_or_nil(translation.attributes["TypographyId"]),
            :alignment => get_value_or_nil(translation.attributes["Alignment"]),
            :direction => get_value_or_nil(translation.attributes["Direction"])
            }; result
        end
      }
    end
      
    languages_with_specific_typographies = texts.map do |text| 
      translations = text[:translations]
      translations.keys.select { |language| translations[language][:typography] }
    end.flatten.uniq
    
    languages_with_specific_alignments = texts.map do |text| 
      translations = text[:translations]
      translations.keys.select { |language| translations[language][:alignment] }
    end.flatten.uniq
    
    languages_with_specific_directions = texts.map do |text| 
      translations = text[:translations]
      translations.keys.select { |language| translations[language][:direction] }
    end.flatten.uniq
    
    languages = texts.map { |text| text[:translations].keys }.flatten.uniq
      
    texts.each do |text|
      text_entry = TextEntry.new(text[:id], text[:typography], text[:alignment], text[:direction])

      languages_with_specific_typographies.each do |language|
        typography = text[:translations][language][:typography]
        text_entry.add_typography(language, typography)
      end
      
      languages_with_specific_alignments.each do |language|
        alignment = text[:translations][language][:alignment]
        text_entry.add_alignment(language, alignment)
      end

      languages_with_specific_directions.each do |language|
        direction = text[:translations][language][:direction]
        text_entry.add_direction(language, direction)
      end

      languages.each do |language|
        translation = text[:translations][language].nil? ? "" : text[:translations][language][:value]
        text_entry.add_translation(language, translation)
      end

      @text_entries.add(text_entry)
    end

    @text_entries
  end
end
