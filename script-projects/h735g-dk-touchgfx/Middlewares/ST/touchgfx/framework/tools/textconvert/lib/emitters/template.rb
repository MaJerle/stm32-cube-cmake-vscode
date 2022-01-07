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
require 'erb'
require 'lib/file_io'

class Template
  attr_accessor :text_entries
  attr_accessor :typographies

  def initialize(text_entries, typographies, output_directory)
    @text_entries = text_entries
    @typographies = typographies
    @output_directory = output_directory
  end
  def run
    result = ERB.new(File.read(input_path).gsub(WINDOWS_LINE_ENDINGS, UNIX_LINE_ENDINGS),0,"<>").
               result(binding).
               gsub(WINDOWS_LINE_ENDINGS, UNIX_LINE_ENDINGS).
               gsub(UNIX_LINE_ENDINGS, LINE_ENDINGS)
    FileIO.write_file(File.join(@output_directory, output_path), result)
  end
end

