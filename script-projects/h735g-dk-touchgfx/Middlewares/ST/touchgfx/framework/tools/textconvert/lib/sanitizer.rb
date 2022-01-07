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
TextEntrySanitizer = Struct.new(:text_entries, :typographies, :framebuffer_bpp)

class Sanitizer < TextEntrySanitizer

  def run
    [ RemoveDuplicateKeys,
      #      RemoveIncompleteLanguages,
      RemoveKeysWithMoreThanTwoSubstitutions,
      RemoveKeysWithDifferentNumberOfSubstitutions,
      RemoveTextEntriesWithInvalidTypography,
      RemoveTextEntriesWithInvalidAlignment,
      RemoveTextEntriesWithInvalidDirection,
      CheckSizeAndBpp,
      DowngradeFontsBitDepth
    ].each do |sanitizer|
      sanitizer.new(text_entries, typographies, framebuffer_bpp).run
    end
  end
end

class RemoveDuplicateKeys < TextEntrySanitizer
  def run
    counts = Hash.new(0)
    counts = text_entries.inject(Hash.new(0)) do |h, entry|
      h[entry.cpp_text_id.upcase] = h[entry.cpp_text_id.upcase] + 1
      h
    end

    text_entries.each do |text_entry|
      if counts[text_entry.cpp_text_id.upcase] > 1
        fail "ERROR: Duplicate key removed: #{text_entry.text_id}, yields cpp identifier #{text_entry.cpp_text_id.upcase}"
        text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveIncompleteLanguages < TextEntrySanitizer
  def run
    languages = text_entries.languages
    languages.each do |language|
      text_entries_with_missing_translations = text_entries.select do |text_entry|
        text_entry.translation_in(language).empty?
      end
      text_entries_with_missing_translations.each do |text_entry|
        fail "ERROR: Language #{language} is missing translation for #{text_entry.text_id}"
      end
      if text_entries_with_missing_translations.any?
        text_entries.remove_language(language)
      end
    end
  end
end

class RemoveKeysWithMoreThanTwoSubstitutions < TextEntrySanitizer
  def run
    text_entries.languages.each do |language|
      text_entries_with_more_than_two_substitutions = text_entries.select do |text_entry|
        text_entry.number_of_substitutions_in(language) > 2
      end
      text_entries_with_more_than_two_substitutions.each do |text_entry|
        fail "ERROR: Text Id #{text_entry.text_id} has #{text_entry.number_of_substitutions_in(language)} substitutions"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveKeysWithDifferentNumberOfSubstitutions < TextEntrySanitizer
  def run
    text_entries.each do |text_entry|
      translations = text_entry.translations
      number_of_substitutions_per_translation = translations.collect { |translation| translation.number_of_substitutions }
      if number_of_substitutions_per_translation.uniq.count > 1
        fail "ERROR: Text Id #{text_entry.text_id} has different number of substitutions for some languages"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveTextEntriesWithInvalidTypography < TextEntrySanitizer
  def run
    text_entries.each do |text_entry|
      non_existing_typographies = (text_entry.get_all_typographies - typographies.map( &:name )).compact;

      if non_existing_typographies.any?
        fail "ERROR: Text Id #{text_entry.text_id} uses unknown typographies #{non_existing_typographies}"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveTextEntriesWithInvalidAlignment < TextEntrySanitizer
  def run
    text_entries.each do |text_entry|
      alignments = text_entry.get_all_alignments_as_string
      illegal_alignments = alignments.select { |a| !['LEFT', 'RIGHT', 'CENTER'].include?(a) }
      if illegal_alignments.any?
        fail "ERROR: Text Id #{text_entry.text_id} uses unknown alignments #{illegal_alignments}"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveTextEntriesWithInvalidDirection < TextEntrySanitizer
  def run
    text_entries.each do |text_entry|
      directions = text_entry.get_all_directions_as_string
      illegal_directions = directions.select { |d| !['LTR', 'RTL'].include?(d) }
      if illegal_directions.any?
        fail "ERROR: Text Id #{text_entry.text_id} uses unknown directions #{illegal_directions}"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class CheckSizeAndBpp < TextEntrySanitizer
  def run
    typographies.each do |typography|
      if ![1, 2, 4, 8].include?(typography.bpp)
        fail "ERROR: Typography named '#{typography.name}' has bpp value '#{typography.bpp}', which is not a valid value"
      end

      if !typography.font_size.integer? || typography.font_size < 1
        fail "ERROR: Typography named '#{typography.name}' has font size value '#{typography.font_size}', which is not a valid value"
      end
    end
  end
end

class DowngradeFontsBitDepth < TextEntrySanitizer
  def run
    if !framebuffer_bpp.nil?
      m = framebuffer_bpp.match(/BPP(\d+)/)
      bpp = m.nil? ? 24 : m[1].to_i
      typographies.each do |typography|
        case bpp
        when 8
          if typography.bpp > 2
            puts "Downgrading typography #{typography.name} from #{typography.bpp.to_s}bpp to 2bpp"
            typography.bpp = 2
          end
        when 4
          if typography.bpp > 4
            puts "Downgrading typography #{typography.name} from #{typography.bpp.to_s}bpp to 4bpp"
            typography.bpp = 4
          end
        when 2
          if typography.bpp > 2
            puts "Downgrading typography #{typography.name} from #{typography.bpp.to_s}bpp to 2bpp"
            typography.bpp = 2
          end
        when 1
          if typography.bpp > 1
            puts "Downgrading typography #{typography.name} from #{typography.bpp.to_s}bpp to 1bpp"
            typography.bpp = 1
          end
        end
      end
    end
  end
end
