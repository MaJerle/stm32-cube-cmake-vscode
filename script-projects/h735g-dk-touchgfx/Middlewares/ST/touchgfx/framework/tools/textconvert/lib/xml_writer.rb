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
require 'fileutils'
require 'lib/text_entries'
require 'lib/typographies'

class XMLWriter
  def write(file_name, content)
    if File.exist?(file_name)
      old_content = File.read(file_name)
      return if content == old_content
    end
    File.write(file_name, content)
  end

  def run(file_name, file_version, text_entries, typographies)
    xml_doc = Nokogiri::XML::Document.new
    xml_doc.encoding = 'utf-8'

    # Create <TextDatabase>
    textdatabase_node = xml_doc.add_child(Nokogiri::XML::Node.new('TextDatabase', xml_doc))
    textdatabase_node['xmlns:xsi'] = 'http://www.w3.org/2001/XMLSchema-instance'
    textdatabase_node['xsi:noNamespaceSchemaLocation'] = 'texts.xsd'
    textdatabase_node['Version'] = file_version

    # Create <Texts> inside <TextDatabase>
    texts_node = textdatabase_node.add_child(Nokogiri::XML::Node.new('Texts', xml_doc))
    text_entries.each do |entry|
      # Add <Text> with required attributes inside <Texts>
      text_node = texts_node.add_child(Nokogiri::XML::Node.new('Text', xml_doc))
      text_node['Id'] = entry.text_id
      text_node['TypographyId'] = entry.typography
      text_node['Alignment'] = entry.alignment.capitalize
      text_node['Direction'] = entry.direction
      text_entries.languages.each do |lang|
        # Add <Translation> with required attribute
        translation_node = text_node.add_child(Nokogiri::XML::Node.new('Translation', xml_doc))
        translation_node['Language'] = lang
        # Add optional attributes
        translation_node['TypographyId'] = entry.typographies[lang] if entry.typographies[lang]
        translation_node['Alignment'] = entry.alignments[lang].capitalize if entry.alignments[lang]
        translation_node['Direction'] = entry.directions[lang] if entry.directions[lang]
        # Add actual text
        translation_node.add_child(Nokogiri::XML::Text.new(entry.translation_in(lang).text, xml_doc))
        # Convert newlines to "&#10;" (disabled as the generated files look more natural)
        translation_node.native_content = translation_node.encode_special_chars(translation_node.text).gsub("\n", '&&#10;')
      end
    end

    # Add <Typographies> inside <TextDatabase>
    typographies_node = textdatabase_node.add_child(Nokogiri::XML::Node.new('Typographies', xml_doc))
    typographies.each do |typo|
      # Add <Typography> with required attributes inside <Typographies>
      typography_node = typographies_node.add_child(Nokogiri::XML::Node.new('Typography', xml_doc))
      typography_node['Id'] = typo.name
      typography_node['Font'] = typo.font_file
      typography_node['Size'] = typo.font_size
      typography_node['Bpp'] = typo.bpp
      # Add optional attributes
      typography_node['FallbackCharacter'] = typo.fallback_character if typo.fallback_character
      typography_node['WildcardCharacters'] = typo.wildcard_characters if typo.wildcard_characters
      typography_node['WidgetWildcardCharacters'] = typo.widget_wildcard_characters if typo.widget_wildcard_characters
      typography_node['WildcardCharacterRanges'] = typo.wildcard_ranges if typo.wildcard_ranges
      typography_node['EllipsisCharacter'] = typo.ellipsis_character if typo.ellipsis_character
    end

    # Create a formatter for nice output and write to file
    xml = xml_doc.to_xml
    xml.gsub!('/>', ' />')
    write(file_name, xml)

    # Create schema
    schema_name = File.join(File.dirname(file_name), 'texts.xsd')
    if !File.exist?(schema_name)
      schema_file = File.join(root_dir, 'schema', 'texts.xsd')
      if File.exist?(schema_file)
        FileUtils.cp(schema_file, schema_name)
      end
    end
  end
end
