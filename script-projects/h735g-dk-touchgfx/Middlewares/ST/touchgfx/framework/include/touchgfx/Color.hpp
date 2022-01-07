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

/**
 * @file touchgfx/Color.hpp
 *
 * Declares the touchgfx::Color class
 */
#ifndef TOUCHGFX_COLOR_HPP
#define TOUCHGFX_COLOR_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/** Contains functionality for color conversion. */
class Color
{
public:
    /**
     * Generates a color representation to be used on the LCD, based on 24 bit RGB values.
     *
     * @param  red   Value of the red part (0-255).
     * @param  green Value of the green part (0-255).
     * @param  blue  Value of the blue part (0-255).
     *
     * @return The color representation depending on LCD color format.
     *
     * @deprecated Please use getColorFromRGB(uint8_t,uint8_t,uint8_t)
     */
    FORCE_INLINE_FUNCTION static colortype getColorFrom24BitRGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        return getColorFromRGB(red, green, blue);
    }

    /**
     * Gets the red color part of a color.
     *
     * @param  color The color value.
     *
     * @return The red part of the color.
     *
     * @deprecated Use getRed(colortype)
     */
    FORCE_INLINE_FUNCTION static uint8_t getRedColor(colortype color)
    {
        return getRed(color);
    }

    /**
     * Gets the green color part of a color.
     *
     * @param  color The color value.
     *
     * @return The green part of the color.
     *
     * @deprecated Use getGreen(colortype)
     */
    FORCE_INLINE_FUNCTION static uint8_t getGreenColor(colortype color)
    {
        return getGreen(color);
    }

    /**
     * Gets the blue color part of a color.
     *
     * @param  color The color value.
     *
     * @return The blue part of the color.
     *
     * @deprecated Use getBlue(colortype)
     */
    FORCE_INLINE_FUNCTION static uint8_t getBlueColor(colortype color)
    {
        return getBlue(color);
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to RGB (Red, Green, Blue).
     *
     * @param       hue        The input Hue (0 to 255).
     * @param       saturation The input Saturation (0 to 255).
     * @param       value      The input Value (0 to 255).
     * @param [out] red        The output Red (0 to 255).
     * @param [out] green      The output Green (0 to 255).
     * @param [out] blue       The output Blue (0 to 255).
     *
     * @note The conversion is an approximation.
     *
     * @deprecated Use getRGBFromHSV(uint8_t,uint8_t,uint8_t,uint8_t&,uint8_t&,uint8_t&)
     */
    FORCE_INLINE_FUNCTION static void getRGBFrom24BitHSV(uint8_t hue, uint8_t saturation, uint8_t value, uint8_t& red, uint8_t& green, uint8_t& blue)
    {
        getRGBFromHSV(hue, saturation, value, red, green, blue);
    }

    /**
     * Convert a given color from RGB (Red, Green, Blue) to HSV (Hue, Saturation, Value).
     *
     * @param       red        The input Red.
     * @param       green      The input Green.
     * @param       blue       The input Blue.
     * @param [out] hue        The output Hue.
     * @param [out] saturation The output Saturation.
     * @param [out] value      The output Value.
     *
     * @note The conversion is an approximation.
     *
     * @deprecated Use getHSVFromRGB(uint8_t,uint8_t,uint8_t,uint8_t&,uint8_t&,uint8_t&)
     */
    FORCE_INLINE_FUNCTION static void getHSVFrom24BitRGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t& hue, uint8_t& saturation, uint8_t& value)
    {
        getHSVFromRGB(red, green, blue, hue, saturation, value);
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to RGB (Red, Green, Blue).
     *
     * @param       hue        The input Hue (0 to 255).
     * @param       saturation The input Saturation (0 to 255).
     * @param       luminance  The input Value (0 to 255).
     * @param [out] red        The output Red (0 to 255).
     * @param [out] green      The output Green (0 to 255).
     * @param [out] blue       The output Blue (0 to 255).
     *
     * @note The conversion is an approximation.
     * @deprecated Use getRGBFromHSL(uint8_t,uint8_t,uint8_t,uint8_t&,uint8_t&,uint8_t&)
     */
    FORCE_INLINE_FUNCTION static void getRGBFrom24BitHSL(uint8_t hue, uint8_t saturation, uint8_t luminance, uint8_t& red, uint8_t& green, uint8_t& blue)
    {
        getRGBFromHSL(hue, saturation, luminance, red, green, blue);
    }

    /**
     * Convert a given color from RGB (Red, Green, Blue) to HSV (Hue, Saturation, Value).
     *
     * @param       red        The input Red (0 to 255).
     * @param       green      The input Green (0 to 255).
     * @param       blue       The input Blue (0 to 255).
     * @param [out] hue        The output Hue (0 to 255).
     * @param [out] saturation The output Saturation (0 to 255).
     * @param [out] luminance  The output Value (0 to 255).
     *
     * @note The conversion is an approximation.
     *
     * @deprecated Use getHSLFrom24BitRGB(uint8_t,uint8_t,uint8_t,uint8_t&,uint8_t&,uint8_t&)
     */
    FORCE_INLINE_FUNCTION static void getHSLFrom24BitRGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t& hue, uint8_t& saturation, uint8_t& luminance)
    {
        getHSLFromRGB(red, green, blue, hue, saturation, luminance);
    }

    /**
     * Convert HSL (Hue, Saturation, Luminance) to HSV (Hue, Saturation, Value). The Hue is
     * unaltered, the Saturation is changed and the Value is calculated.
     *
     * @param          hue        The hue (0 to 255).
     * @param [in,out] saturation The saturation (0 to 255).
     * @param          luminance  The luminance (0 to 255).
     * @param [out]    value      The value (0 to 255).
     *
     * @deprecated Use getHSVFromHSL(uint8_t,uint8_t,int8_t,uint8_t&,uint8_t&,uint8_t&)
     */
    FORCE_INLINE_FUNCTION static void getHSVFromHSL(uint8_t hue, uint8_t& saturation, uint8_t luminance, uint8_t& value)
    {
        value = luminance + LCD::div255(saturation * MIN(luminance, 255 - luminance));
        saturation = value == 0 ? 0 : 510 - ((luminance * 510) / value);
    }

    /**
     * Convert HSV (Hue, Saturation, Value) to HSL (Hue, Saturation, Luminance). The Hue is
     * unaltered, the Saturation is changed and the Luminance is calculated.
     *
     * @param          hue        The hue (0 to 255).
     * @param [in,out] saturation The saturation (0 to 255).
     * @param          value      The value (0 to 255).
     * @param [out]    luminance  The luminance (0 to 255).
     *
     * @deprecated Use getHSLFromHSV(uint8_t,uint8_t,int8_t,uint8_t&,uint8_t&,uint8_t&)
     */
    FORCE_INLINE_FUNCTION static void getHSLFromHSV(uint8_t hue, uint8_t& saturation, uint8_t value, uint8_t& luminance)
    {
        uint16_t luminance2 = (value * (510 - saturation)) / 255;
        luminance = luminance2 >> 1;
        saturation = (luminance == 0 || luminance == 255) ? 0 : (uint8_t)(((value * 2 - luminance2) * 255) / MIN(luminance2, 510 - luminance2));
    }

    /**
     * Generates a color representation to be used on the LCD, based on 24 bit RGB values. The
     * embedded alpha value is set to 255.
     *
     * @param  red   Value of the red part (0-255).
     * @param  green Value of the green part (0-255).
     * @param  blue  Value of the blue part (0-255).
     *
     * @return The color representation depending on LCD color format.
     */
    FORCE_INLINE_FUNCTION static colortype getColorFromRGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        return 0xFF000000 | (red << 16) | (green << 8) | (blue);
    }

    /**
     * Gets the red color part of a color.
     *
     * @param  color The color value.
     *
     * @return The red part of the color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getRed(colortype color)
    {
        return color >> 16;
    }

    /**
     * Gets the green color part of a color.
     *
     * @param  color The color value.
     *
     * @return The green part of the color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getGreen(colortype color)
    {
        return color >> 8;
    }

    /**
     * Gets the blue color part of a color.
     *
     * @param  color The color value.
     *
     * @return The blue part of the color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getBlue(colortype color)
    {
        return color;
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to RGB (Red, Green, Blue).
     *
     * @param       hue        The input Hue (0 to 255).
     * @param       saturation The input Saturation (0 to 255).
     * @param       value      The input Value (0 to 255).
     * @param [out] red        The output Red (0 to 255).
     * @param [out] green      The output Green (0 to 255).
     * @param [out] blue       The output Blue (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    static void getRGBFromHSV(uint8_t hue, uint8_t saturation, uint8_t value, uint8_t& red, uint8_t& green, uint8_t& blue)
    {
        if (saturation == 0)
        {
            red = green = blue = value;
            return;
        }

        const uint8_t region = (hue * 6) >> 8;
        const int next_region_start = ((((region + 1) << 8) + 5) / 6); // Can go up to 256, uint8_t not enough
        const uint8_t region_size = next_region_start - (((region << 8) + 5) / 6);
        const uint8_t remainder = 255 - (next_region_start - hue) * 255 / region_size;

        const uint8_t p = LCD::div255(value * (255 - saturation));
        const uint8_t q = LCD::div255(value * (255 - LCD::div255(saturation * remainder)));
        const uint8_t t = LCD::div255(value * (255 - LCD::div255(saturation * (255 - remainder))));

        switch (region)
        {
        case 0:
            red = value, green = t, blue = p;
            break;
        case 1:
            red = q, green = value, blue = p;
            break;
        case 2:
            red = p, green = value, blue = t;
            break;
        case 3:
            red = p, green = q, blue = value;
            break;
        case 4:
            red = t, green = p, blue = value;
            break;
        default:
            red = value, green = p, blue = q;
            break;
        }
    }

    /**
     * Convert a given color from RGB (Red, Green, Blue) to HSV (Hue, Saturation, Value).
     *
     * @param       red            The input Red.
     * @param       green          The input Green.
     * @param       blue           The input Blue.
     * @param [out] hue            The output Hue.
     * @param [out] saturation The output Saturation.
     * @param [out] value          The output Value.
     *
     * @note The conversion is an approximation.
     */
    static void getHSVFromRGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t& hue, uint8_t& saturation, uint8_t& value)
    {
        const uint8_t rgbMin = MIN(MIN(red, green), blue);
        const uint8_t rgbMax = MAX(MAX(red, green), blue);
        const uint8_t rgbRange = rgbMax - rgbMin;

        value = rgbMax;
        if (value == 0)
        {
            hue = 0;
            saturation = 0;
            return;
        }

        saturation = 255 * rgbRange / value;
        if (saturation == 0)
        {
            hue = 0;
        }
        else if (rgbMax == red)
        {
            if (green < blue)
            {
                hue = 0 + (42 * (green - blue) + rgbRange / 2) / rgbRange; // [0-42; 0] = [214; 0]
            }
            else
            {
                hue = 0 + (43 * (green - blue) + rgbRange / 2) / rgbRange; // [0; 0+43] = [0; 43]
            }
        }
        else if (rgbMax == green)
        {
            if (blue < red)
            {
                hue = 86 + (43 * (blue - red) + rgbRange / 2) / rgbRange; // [86-43; 86] = [43; 86]
            }
            else
            {
                hue = 86 + (42 * (blue - red) + rgbRange / 2) / rgbRange; // [86; 86+42] = [86; 128]
            }
        }
        else
        {
            hue = 171 + (43 * (red - green) + rgbRange / 2) / rgbRange; // [171-43; 171+43] = [128; 214]
        }
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to colortype.
     *
     * @param       hue        The input Hue (0 to 255).
     * @param       saturation The input Saturation (0 to 255).
     * @param       value      The input Value (0 to 255).
     *
     * @return The colortype color.
     * @note The conversion is an approximation.
     */
    FORCE_INLINE_FUNCTION static colortype getColorFromHSV(uint8_t hue, uint8_t saturation, uint8_t value)
    {
        uint8_t red, green, blue;
        getRGBFromHSV(hue, saturation, value, red, green, blue);
        return getColorFrom24BitRGB(red, green, blue);
    }

    /**
     * Convert a given colortype color to HSV (Hue, Saturation, Value).
     *
     * @param       color      The input color.
     * @param [out] hue        The output Hue (0 to 255).
     * @param [out] saturation The output Saturation (0 to 255).
     * @param [out] value      The output Value (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    FORCE_INLINE_FUNCTION static void getHSVFromColor(colortype color, uint8_t& hue, uint8_t& saturation, uint8_t& value)
    {
        getHSVFromRGB(getRed(color), getGreen(color), getBlue(color), hue, saturation, value);
    }

    /**
     * Convert HSL (Hue, Saturation, Luminance) to HSV (Hue, Saturation, Value).
     *
     * @param       hsl_hue        The input HSL hue (0 to 255).
     * @param       hsl_saturation The input HSL saturation (0 to 255).
     * @param       hsl_luminance  The input HSL luminance (0 to 255).
     * @param [out] hsv_hue        The output HSV hue (0 to 255).
     * @param [out] hsv_saturation The output HSV saturation (0 to 255).
     * @param [out] hsv_value      The output HSV value (0 to 255).
     */
    FORCE_INLINE_FUNCTION static void getHSVFromHSL(uint8_t hsl_hue, uint8_t hsl_saturation, uint8_t hsl_luminance, uint8_t& hsv_hue, uint8_t& hsv_saturation, uint8_t& hsv_value)
    {
        hsv_hue = hsl_hue;
        hsv_value = hsl_luminance + LCD::div255(hsl_saturation * MIN(hsl_luminance, 255 - hsl_luminance));
        hsv_saturation = hsv_value == 0 ? 0 : 510 - ((hsl_luminance * 510) / hsv_value);
    }

    /**
     * Convert HSV (Hue, Saturation, Value) to HSL (Hue, Saturation, Luminance).
     *
     * @param       hsv_hue        The input HSV hue (0 to 255).
     * @param       hsv_saturation The input HSV saturation (0 to 255).
     * @param       hsv_value      The input HSV value (0 to 255).
     * @param [out] hsl_hue        The output HSL hue (0 to 255).
     * @param [out] hsl_saturation The output HSL saturation (0 to 255).
     * @param [out] hsl_luminance  The output HSL luminance (0 to 255).
     */
    FORCE_INLINE_FUNCTION static void getHSLFromHSV(uint8_t hsv_hue, uint8_t hsv_saturation, uint8_t hsv_value, uint8_t& hsl_hue, uint8_t& hsl_saturation, uint8_t& hsl_luminance)
    {
        hsl_hue = hsv_hue;
        uint16_t luminance2 = (hsv_value * (510 - hsv_saturation)) / 255;
        hsl_luminance = luminance2 >> 1;
        hsl_saturation = (hsl_luminance == 0 || hsl_luminance == 255) ? 0 : (uint8_t)(((hsv_value * 2 - luminance2) * 255) / MIN(luminance2, 510 - luminance2));
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to RGB (Red, Green, Blue).
     *
     * @param       hue        The input Hue (0 to 255).
     * @param       saturation The input Saturation (0 to 255).
     * @param       luminance  The input Value (0 to 255).
     * @param [out] red        The output Red (0 to 255).
     * @param [out] green      The output Green (0 to 255).
     * @param [out] blue       The output Blue (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    FORCE_INLINE_FUNCTION static void getRGBFromHSL(uint8_t hue, uint8_t saturation, uint8_t luminance, uint8_t& red, uint8_t& green, uint8_t& blue)
    {
        uint8_t hsv_hue, hsv_saturation, hsv_value;
        getHSVFromHSL(hue, saturation, luminance, hsv_hue, hsv_saturation, hsv_value);
        getRGBFromHSV(hsv_hue, hsv_saturation, hsv_value, red, green, blue);
    }

    /**
     * Convert a given color from RGB (Red, Green, Blue) to HSV (Hue, Saturation, Value).
     *
     * @param       red        The input Red (0 to 255).
     * @param       green      The input Green (0 to 255).
     * @param       blue       The input Blue (0 to 255).
     * @param [out] hue        The output Hue (0 to 255).
     * @param [out] saturation The output Saturation (0 to 255).
     * @param [out] luminance  The output Value (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    FORCE_INLINE_FUNCTION static void getHSLFromRGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t& hue, uint8_t& saturation, uint8_t& luminance)
    {
        uint8_t hsv_hue, hsv_saturation, hsv_value;
        getHSVFromRGB(red, green, blue, hsv_hue, hsv_saturation, hsv_value);
        getHSLFromHSV(hsv_hue, hsv_saturation, hsv_value, hue, saturation, luminance);
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to colortype.
     *
     * @param  hue        The input Hue (0 to 255).
     * @param  saturation The input Saturation (0 to 255).
     * @param  luminance  The input Value (0 to 255).
     *
     * @return The colortype color.
     *
     * @note The conversion is an approximation.
     */
    FORCE_INLINE_FUNCTION static colortype getColorFromHSL(uint8_t hue, uint8_t saturation, uint8_t luminance)
    {
        uint8_t red, green, blue;
        getRGBFromHSL(hue, saturation, luminance, red, green, blue);
        return getColorFrom24BitRGB(red, green, blue);
    }

    /**
     * Convert a given colortype color to HSV (Hue, Saturation, Value).
     *
     * @param       color      The input color.
     * @param [out] hue        The output Hue (0 to 255).
     * @param [out] saturation The output Saturation (0 to 255).
     * @param [out] luminance  The output Value (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    FORCE_INLINE_FUNCTION static void getHSLFromColor(colortype color, uint8_t& hue, uint8_t& saturation, uint8_t& luminance)
    {
        getHSLFrom24BitRGB(getRed(color), getGreen(color), getBlue(color), hue, saturation, luminance);
    }

    /**
     * Gets the red color part of a 16bpp color also known as
     * RGB565. The red part (5 bits) is extracted and scaled to the
     * full 0-255 byte range.
     *
     * @param  color The color value in RGB565.
     *
     * @return The red part of the color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getRedFromRGB565(uint16_t color)
    {
        const uint8_t red = (color >> 8) & 0xF8;
        return red | (red >> 5);
    }

    /**
     * Gets the green color part of a 16bpp color also known as
     * RGB565. The green part (6 bits) is extracted and scaled to the
     * full 0-255 byte range.
     *
     * @param  color The color value in RGB565.
     *
     * @return The green part of the color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getGreenFromRGB565(uint16_t color)
    {
        const uint8_t green = (color >> 3) & 0xFC;
        return green | (green >> 6);
    }

    /**
     * Gets the blue color part of a 16bpp color also known as
     * RGB565. The blue part (5 bits) is extracted and scaled to the
     * full 0-255 byte range.
     *
     * @param  color The color value in RGB565.
     *
     * @return The blue part of the color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getBlueFromRGB565(uint16_t color)
    {
        const uint8_t blue = color << 3;
        return blue | (blue >> 5);
    }

    /**
     * Convert 16bit RGB565 color to 32bit ARGB8888 color by expanding the 5,6,5 colors to full 8,8,
     * 8 colors. The alpha value is set to zero.
     *
     * @param  rgb565 The 16bit RGB565 color.
     *
     * @return The 32bit color value.
     *
     * @see getRedFromRGB565, getGreenFromRGB565, getBlueFromRGB565
     */
    FORCE_INLINE_FUNCTION static uint32_t rgb565toXrgb8888(uint16_t rgb565)
    {
        return (getRedFromRGB565(rgb565) << 16) | (getGreenFromRGB565(rgb565) << 8) | getBlueFromRGB565(rgb565);
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_COLOR_HPP
