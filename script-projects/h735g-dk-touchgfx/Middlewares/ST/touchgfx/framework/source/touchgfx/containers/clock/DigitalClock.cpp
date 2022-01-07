/******************************************************************************
* Copyright (c) 2018(-2021) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.18.1 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/clock/AbstractClock.hpp>
#include <touchgfx/containers/clock/DigitalClock.hpp>

namespace touchgfx
{
DigitalClock::DigitalClock()
    : AbstractClock(),
      displayMode(DISPLAY_24_HOUR),
      useLeadingZeroForHourIndicator(false),
      text()
{
    buffer[0] = '\0';
    text.setXY(0, 0);
    text.setWildcard(buffer);
    Container::add(text);
}

void DigitalClock::setWidth(int16_t width)
{
    Container::setWidth(width);
    text.setWidth(width);
}

void DigitalClock::setHeight(int16_t height)
{
    Container::setHeight(height);
    text.setHeight(height);
}

void DigitalClock::setBaselineY(int16_t baselineY)
{
    if (text.getTypedText().hasValidId())
    {
        moveTo(getX(), baselineY - text.getTypedText().getFont()->getFontHeight());
    }
}

void DigitalClock::displayLeadingZeroForHourIndicator(bool displayLeadingZero)
{
    useLeadingZeroForHourIndicator = displayLeadingZero;
}

void DigitalClock::setAlpha(uint8_t newAlpha)
{
    text.setAlpha(newAlpha);
}

uint8_t DigitalClock::getAlpha() const
{
    return text.getAlpha();
}

void DigitalClock::setTypedText(TypedText typedText)
{
    text.setTypedText(typedText);
    text.invalidate();
}

void DigitalClock::setColor(colortype color)
{
    text.setColor(color);
    text.invalidate();
}

colortype DigitalClock::getColor() const
{
    return text.getColor();
}

void DigitalClock::updateClock()
{
    if (displayMode == DISPLAY_12_HOUR_NO_SECONDS)
    {
        const char* format = useLeadingZeroForHourIndicator ? "%02d:%02d %cM" : "%d:%02d %cM";
        Unicode::snprintf(buffer, BUFFER_SIZE, format, getCurrentHour12(), getCurrentMinute(), getCurrentAM() ? 'A' : 'P');
    }
    else if (displayMode == DISPLAY_24_HOUR_NO_SECONDS)
    {
        const char* format = useLeadingZeroForHourIndicator ? "%02d:%02d" : "%d:%02d";
        Unicode::snprintf(buffer, BUFFER_SIZE, format, getCurrentHour24(), getCurrentMinute());
    }
    else if (displayMode == DISPLAY_12_HOUR)
    {
        const char* format = useLeadingZeroForHourIndicator ? "%02d:%02d:%02d %cM" : "%d:%02d:%02d %cM";
        Unicode::snprintf(buffer, BUFFER_SIZE, format, getCurrentHour12(), getCurrentMinute(), getCurrentSecond(), getCurrentAM() ? 'A' : 'P');
    }
    else if (displayMode == DISPLAY_24_HOUR)
    {
        const char* format = useLeadingZeroForHourIndicator ? "%02d:%02d:%02d" : "%d:%02d:%02d";
        Unicode::snprintf(buffer, BUFFER_SIZE, format, getCurrentHour24(), getCurrentMinute(), getCurrentSecond());
    }
    text.invalidate();
}
} // namespace touchgfx
