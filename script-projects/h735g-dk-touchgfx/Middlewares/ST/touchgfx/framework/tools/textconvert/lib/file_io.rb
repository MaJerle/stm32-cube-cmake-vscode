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
require 'rubygems'
require 'erb'
require 'fileutils'
require 'pathname'

class FileIO
  def self.write_file(file_name, contents)
    callingPath = Pathname.new($calling_path)
    filePath = Pathname.new(file_name)
    puts "Generating #{filePath.relative_path_from(callingPath)}"
    write_file_silent(file_name, contents)
  end
  def self.write_file_silent(file_name, contents)
    FileUtils.mkdir_p(File.dirname(file_name))
    unless File.exist?(file_name) && contents == File.open(file_name, 'r') { |f| f.read() }
      File.open(file_name, 'w') { |f| f.write(contents) }
    end
  end
end
