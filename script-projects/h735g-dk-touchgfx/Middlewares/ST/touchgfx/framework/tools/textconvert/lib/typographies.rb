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
class Typography < Struct.new(:name, :font_file, :font_size, :bpp, :fallback_character, :ellipsis_character, :wildcard_characters, :widget_wildcard_characters, :wildcard_ranges)
  def cpp_name
    font_file.gsub(/\.ttf$/,"").gsub(/[^0-9a-zA-Z]/, "_")
  end
end
