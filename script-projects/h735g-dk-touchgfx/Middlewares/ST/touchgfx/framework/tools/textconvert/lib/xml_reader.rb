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

class XMLReader
  def read(file_name)
    xml_doc = Nokogiri::XML::Document.parse(File.new(file_name))
    if !xml_doc.errors.none?
      fail (["ERROR: Malformed xml in #{file_name}"] + xml_doc.errors.map { |err| "Line #{err.line}: #{err.message}" }) * "\n"
    end
    xml_doc
  end
end

