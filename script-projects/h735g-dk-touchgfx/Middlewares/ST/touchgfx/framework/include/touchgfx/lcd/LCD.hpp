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
 * @file touchgfx/lcd/LCD.hpp
 *
 * This file contains two super classes, namely LCD and DebugPrinter. The LCD class specifies
 * various functions that allow drawing images, texts and boxes on the display. In general,
 * these functions are available through various widgets which encapsulates these drawing
 * operations and keep track of the state of image/text/etc. Please consult the documentation
 * for the widgets for more information.
 *
 * @see touchgfx::LCD, touchgfx::DebugPrinter
 */
#ifndef TOUCHGFX_LCD_HPP
#define TOUCHGFX_LCD_HPP

// LCD is defined in some CubeFW C header file. We have to undef to be able to create the LCD class
#ifdef LCD
#undef LCD
#endif

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/TextProvider.hpp>
#include <touchgfx/TextureMapTypes.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/lcd/DebugPrinter.hpp>

namespace touchgfx
{
/**
 * This class contains the various low-level drawing routines for drawing bitmaps, texts and
 * rectangles/boxes. Normally, these draw operations are called from widgets, which also
 * keep track of logical states such as visibility etc.
 *
 * The LCD class cannot be instantiated, instead use one of the subclasses which
 * implements the LCD drawing operations for a specific display configuration.
 *
 * @note All coordinates sent to functions in the LCD class are expected to be in absolute
 *       coordinates, i.e. (0, 0) is upper left corner of the display.
 */
class LCD
{
public:
    /** Initializes a new instance of the LCD class. */
    LCD()
        : textureMapperClass(0)
    {
    }

    /** Finalizes an instance of the LCD class. */
    virtual ~LCD()
    {
    }

    /**
     * Draws all (or a part) of a \a bitmap. The coordinates of the corner of the bitmap is
     * given in (\a x, \a y) and \a rect describes which part of the \a bitmap should be
     * drawn. The bitmap can be drawn as it is or more or less transparent depending on the
     * value of \a alpha. The value of \a alpha is independent of the transparency of the
     * individual pixels of the given \a bitmap.
     *
     * @param  bitmap       The bitmap to draw.
     * @param  x            The absolute x coordinate to place (0, 0) of the bitmap on the screen.
     * @param  y            The absolute y coordinate to place (0, 0) of the bitmap on the screen.
     * @param  rect         A rectangle describing what region of the bitmap is to be drawn.
     * @param  alpha        (Optional) Optional alpha value ranging from 0=invisible to
     *                      255=solid. Default is 255 (solid).
     * @param  useOptimized (Optional) if false, do not attempt to substitute (parts of) this
     *                      bitmap with faster fillrects.
     */
    virtual void drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true) = 0;

    /**
     * Blits (directly copies) a block of data to the framebuffer, performing alpha blending
     * (and tranparency keying) as specified. Performs a software blend if HAL does not
     * support BLIT_COPY_WITH_ALPHA and alpha != 255 (solid).
     *
     * @param  sourceData           The source array pointer (points to the beginning of the
     *                              data). The sourceData must be stored in a format
     *                              suitable for the selected display.
     * @param  source               The position and dimensions of the source. The x and y of this
     *                              rect should both be 0.
     * @param  blitRect             A rectangle describing what region of the \a sourceData is to
     *                              be copied to the framebuffer.
     * @param  alpha                The alpha value to use for blending ranging from 0=invisible
     *                              to 255=solid=no blending.
     * @param  hasTransparentPixels If true, this data copy contains transparent pixels and
     *                              require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels) = 0;

    /**
     * Blits (directly copies) a block of data to the framebuffer, performing alpha blending
     * (and tranparency keying) as specified. Performs a software blend if HAL does not
     * support BLIT_COPY_WITH_ALPHA and alpha < 255 (solid).
     *
     * If the display does not support the specified \a sourceFormat, an \a assert will be
     * raised.
     *
     * @param  sourceData           The source array pointer (points to the beginning of the
     *                              data). The sourceData must be stored in a format
     *                              suitable for the selected display.
     * @param  sourceFormat         The bitmap format used in the source data.
     * @param  source               The position and dimensions of the source. The x and y of this
     *                              rect should both be 0.
     * @param  blitRect             A rectangle describing what region of the \a sourceData is to
     *                              be copied to the framebuffer.
     * @param  alpha                The alpha value to use for blending ranging from 0=invisible
     *                              to 255=solid=no blending.
     * @param  hasTransparentPixels If true, this data copy contains transparent pixels and
     *                              require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels) = 0;

    /**
     * Copies part of the framebuffer to the data section of a bitmap. The bitmap must be a
     * dynamic bitmap or animation storage (BITMAP_ANIMATION_STORAGE). Only the part
     * specified with by parameter \a region is copied.
     *
     * If \a region has negative x/y coordinates of if width/height exceeds those of the
     * given bitmap, only the visible and legal part of the framebuffer is copied. The rest
     * of the bitmap image is left untouched.
     *
     * @param  region   The part of the framebuffer to copy.
     * @param  bitmapId (Optional) The bitmap to store the data in. Default is to use
     *                  Animation Storage.
     *
     * @return A pointer to the copy.
     *
     * @see blitCopy
     *
     * @note There is only one instance of animation storage. The content of the bitmap data (or
     *       animation storage) outside the given region is left untouched.
     */
    uint16_t* copyFrameBufferRegionToMemory(const Rect& region, const BitmapId bitmapId = BITMAP_ANIMATION_STORAGE)
    {
        return copyFrameBufferRegionToMemory(region, region, bitmapId);
    }

    /**
     * Copies part of the framebuffer to the data section of a bitmap. The bitmap must be a
     * dynamic bitmap or animation storage (BITMAP_ANIMATION_STORAGE). The two regions given
     * are the visible region and the absolute region on screen. This is used to copy only a
     * part of the framebuffer. This might be the case if a SnapshotWidget is placed inside
     * a Container where parts of the SnapshowWidget is outside the area defined by the
     * Container. The visible region must be completely inside the absolute region.
     *
     * @param  visRegion The visible region.
     * @param  absRegion The absolute region.
     * @param  bitmapId  Identifier for the bitmap.
     *
     * @return Null if it fails, else a pointer to the data in the given bitmap.
     *
     * @see blitCopy
     *
     * @note There is only one instance of animation storage. The content of the bitmap data
     *       /animation storage outside the given region is left untouched.
     */
    virtual uint16_t* copyFrameBufferRegionToMemory(const Rect& visRegion, const Rect& absRegion, const BitmapId bitmapId) = 0;

    /**
     * Draws a filled rectangle in the framebuffer in the specified color and opacity. By
     * default the rectangle will be drawn as a solid box. The rectangle can be drawn with
     * transparency by specifying alpha from 0=invisible to 255=solid.
     *
     * @param  rect  The rectangle to draw in absolute display coordinates.
     * @param  color The rectangle color.
     * @param  alpha (Optional) The rectangle opacity, from 0=invisible to 255=solid.
     *
     * @see fillBuffer
     */
    virtual void fillRect(const Rect& rect, colortype color, uint8_t alpha = 255) = 0;

    /**
     * Draws a filled rectangle in destination memory using the specified color and opacity. The
     * destination memory must have the same format as the display framebuffer. By default the
     * rectangle will be drawn as a solid box. The rectangle can be drawn with transparency by
     * specifying alpha from 0=invisible to 255=solid.
     *
     * @param [in]  destination The start of the memory area to fill.
     * @param       pixelStride The pixel stride, i.e. number of pixels in a row.
     * @param       rect        The rectangle to fill absolute coordinates.
     * @param       color       The rectangle color.
     * @param       alpha       The rectangle opacity, from 0=invisible to 255=solid.
     *
     * @note    The pixelStride is rounded up to nearest whole bytes for displays with more than one
     *          pixel per byte (LCD1bpp, LCD2bpp and LCD4bpp)
     */
    virtual void fillBuffer(uint8_t* const destination, uint16_t pixelStride, const Rect& rect, const colortype color, const uint8_t alpha) = 0;

    /** The visual elements when writing a string. */
    struct StringVisuals
    {
        const Font* font;              ///< The font to use.
        Alignment alignment;           ///< The alignment to use. Default is LEFT.
        TextDirection textDirection;   ///< The direction to use. Default is LTR
        TextRotation rotation;         ///< Orientation (rotation) of the text. Default is TEXT_ROTATE_0.
        colortype color;               ///< RGB color value. Default is 0 (black).
        int16_t linespace;             ///< Line space in pixels for multiline strings. Default is 0.
        uint8_t alpha;                 ///< 8-bit alpha value. Default is 255 (solid).
        uint8_t indentation;           ///< Indentation of text inside rectangle. Text will start this far from the left/right edge
        WideTextAction wideTextAction; ///< What to do with wide text lines

        /** Initializes a new instance of the LCD class. */
        StringVisuals()
            : font(0), alignment(LEFT), textDirection(TEXT_DIRECTION_LTR), rotation(TEXT_ROTATE_0), color(0), linespace(0), alpha(255), indentation(0), wideTextAction(WIDE_TEXT_NONE)
        {
        }

        /**
         * Construct a StringVisual object for rendering text.
         *
         * @param  font           The Font with which to draw the text.
         * @param  color          The color with which to draw the text.
         * @param  alpha          Alpha blending. Default value is 255 (solid)
         * @param  alignment      How to align the text.
         * @param  linespace      Line space in pixels between each line, in case the text
         *                        contains newline characters.
         * @param  rotation       How to rotate the text.
         * @param  textDirection  The text direction.
         * @param  indentation    The indentation of the text from the left and right of the
         *                        text area rectangle.
         * @param  wideTextAction (Optional) What to do with lines longer than the width of the
         *                        TextArea.
         */
        StringVisuals(const Font* font, colortype color, uint8_t alpha, Alignment alignment, int16_t linespace, TextRotation rotation, TextDirection textDirection, uint8_t indentation, WideTextAction wideTextAction = WIDE_TEXT_NONE)
            : font(font), alignment(alignment), textDirection(textDirection), rotation(rotation), color(color), linespace(linespace), alpha(alpha), indentation(indentation), wideTextAction(wideTextAction)
        {
        }
    };

    /**
     * Draws the specified Unicode string. Breaks line on newline.
     *
     * @param  widgetArea      The area covered by the drawing widget in absolute
     *                         coordinates.
     * @param  invalidatedArea The (sub)region of the widget area to draw, expressed
     *                         relative to the widget area. If the widgetArea is x=10, y=10,
     *                         width=20, height=20 and invalidatedArea is x=5, y=5, width=6,
     *                         height=6 the widgetArea drawn on the LCD is x=15, y=15,
     *                         width=6, height=6.
     * @param  stringVisuals   The string visuals (font, alignment, line space, color)
     *                         with which to draw this string.
     * @param  format          A pointer to a null-terminated text string with optional
     *                         additional wildcard arguments.
     * @param  ...             Variable arguments providing additional information.
     */
    void drawString(Rect widgetArea,
                    const Rect& invalidatedArea,
                    const StringVisuals& stringVisuals,
                    const Unicode::UnicodeChar* format,
                    ...);

    /**
     * Number of bits per pixel used by the display.
     *
     * @return The number of bits per pixel.
     */
    virtual uint8_t bitDepth() const = 0;

    /**
     * Framebuffer format used by the display.
     *
     * @return A Bitmap::BitmapFormat.
     */
    virtual Bitmap::BitmapFormat framebufferFormat() const = 0;

    /**
     * Framebuffer stride in bytes. The distance (in bytes) from the start of one
     * framebuffer row, to the next.
     *
     * @return The number of bytes in one framebuffer row.
     */
    virtual uint16_t framebufferStride() const = 0;

    /**
     * Check if LCD support dynamic bitmap drawing.
     *
     * @param  format The dynamic bitmap format.
     *
     * @return true if dynamic bitmap drawing is supported, false otherwise.
     */
    virtual bool supportDynamicBitmapDrawing(const Bitmap::BitmapFormat format)
    {
        // return true if bitmap format matches framebuffer format
        return (format == framebufferFormat());
    }

    /**
     * Sets default color as used by alpha level only bitmap formats, e.g. A4. The default
     * color, if no color is set, is black.
     *
     * @param  color The color.
     *
     * @see getDefaultColor
     */
    virtual void setDefaultColor(colortype color)
    {
        defaultColor = color;
    }

    /**
     * Gets default color previously set using setDefaultColor.
     *
     * @return The default color.
     *
     * @see setDefaultColor
     */
    colortype getDefaultColor() const
    {
        return defaultColor;
    }

    /**
     * Texture map triangle. Draw a perspective correct texture mapped triangle. The
     * vertices describes the surface, the x,y,z coordinates and the u,v coordinates of the
     * texture. The texture contains the image data to be drawn The triangle line will be
     * placed and clipped using the absolute and dirty rectangles The alpha will determine
     * how the triangle should be alpha blended. The subDivisionSize will determine the size
     * of the piecewise affine texture mapped portions of the triangle.
     *
     * @param  dest              The description of where the texture is drawn - can be used
     *                           to issue a draw off screen.
     * @param  vertices          The vertices of the triangle.
     * @param  texture           The texture.
     * @param  absoluteRect      The containing rectangle in absolute coordinates.
     * @param  dirtyAreaAbsolute The dirty area in absolute coordinates.
     * @param  renderVariant     The render variant - includes the algorithm and the pixel
     *                           format.
     * @param  alpha             (Optional) the alpha. Default is 255 (solid).
     * @param  subDivisionSize   (Optional) the size of the subdivisions of the scan line.
     *                           Default is 12.
     */
    virtual void drawTextureMapTriangle(const DrawingSurface& dest,
                                        const Point3D* vertices,
                                        const TextureSurface& texture,
                                        const Rect& absoluteRect,
                                        const Rect& dirtyAreaAbsolute,
                                        RenderingVariant renderVariant,
                                        uint8_t alpha = 255,
                                        uint16_t subDivisionSize = 12);

    /**
     * Texture map quad. Draw a perspective correct texture mapped quad. The
     * vertices describes the surface, the x,y,z coordinates and the u,v coordinates of the
     * texture. The texture contains the image data to be drawn The quad line will be
     * placed and clipped using the absolute and dirty rectangles The alpha will determine
     * how the quad should be alpha blended. The subDivisionSize will determine the size
     * of the piecewise affine texture mapped portions of the quad.
     *
     * @param  dest              The description of where the texture is drawn - can be used
     *                           to issue a draw off screen.
     * @param  vertices          The vertices of the quad.
     * @param  texture           The texture.
     * @param  absoluteRect      The containing rectangle in absolute coordinates.
     * @param  dirtyAreaAbsolute The dirty area in absolute coordinates.
     * @param  renderVariant     The render variant - includes the algorithm and the pixel
     *                           format.
     * @param  alpha             (Optional) the alpha. Default is 255 (solid).
     * @param  subDivisionSize   (Optional) the size of the subdivisions of the scan line.
     *                           Default is 12.
     */
    virtual void drawTextureMapQuad(const DrawingSurface& dest,
                                    const Point3D* vertices,
                                    const TextureSurface& texture,
                                    const Rect& absoluteRect,
                                    const Rect& dirtyAreaAbsolute,
                                    RenderingVariant renderVariant,
                                    uint8_t alpha = 255,
                                    uint16_t subDivisionSize = 12);

    /**
     * Approximates an integer division of a 16bit value by 255. Divides numerator num (e.g.
     * the sum resulting from an alpha-blending operation) by 255.
     *
     * @param [in] num The numerator to divide by 255.
     *
     * @return The result of a division by 255.
     */
    FORCE_INLINE_FUNCTION static uint8_t div255(uint16_t num)
    {
        return (num + 1 + (num >> 8)) >> 8;
    }

    /**
     * Divides the red and blue components of pixelxAlpha by 255.
     *
     * @param [in] pixelxAlpha The red and blue components of a 32bit ARGB pixel multiplied
     *                         by an alpha factor.
     *
     * @return pixelxAlpha with its red and blue components divided by 255.
     */
    FORCE_INLINE_FUNCTION static uint32_t div255rb(uint32_t pixelxAlpha)
    {
        return ((pixelxAlpha + 0x10001 + ((pixelxAlpha >> 8) & 0xFF00FF)) >> 8) & 0xFF00FF;
    }

    /**
     * Divides the green component of pixelxAlpha by 255.
     *
     * @param [in] pixelxAlpha The green component of a 32bit ARGB pixel multiplied by an
     *                         alpha factor.
     *
     * @return pixelxAlpha with its green component divided by 255.
     */
    FORCE_INLINE_FUNCTION static uint32_t div255g(uint32_t pixelxAlpha)
    {
        return ((pixelxAlpha + 0x100 + (pixelxAlpha >> 8)) >> 8) & 0x00FF00;
    }

protected:
    static const uint16_t newLine = 10; ///< NewLine value.

    static colortype defaultColor; ///< Default Color to use when displaying transparency-only elements, e.g. A4 bitmaps

    /** Base class for drawing scanline by the texture mapper. */
    class DrawTextureMapScanLineBase
    {
    public:
        /** Finalizes an instance of the DrawTextureMapScanLineBase class. */
        virtual ~DrawTextureMapScanLineBase()
        {
        }

        /**
         * Draw texture map scan line subdivisions.
         *
         * @param  subdivisions   The number of subdivisions.
         * @param  widthModLength Remainder of length (after subdivisions).
         * @param  pixelsToDraw   The pixels to draw.
         * @param  affineLength   Length of one subdivision.
         * @param  oneOverZRight  1/Z right.
         * @param  UOverZRight    U/Z right.
         * @param  VOverZRight    V/Z right.
         * @param  U              U Coordinate in fixed16_16 notation.
         * @param  V              V Coordinate in fixed16_16 notation.
         * @param  deltaU         U delta to get to next pixel coordinate.
         * @param  deltaV         V delta to get to next pixel coordinate.
         * @param  ULeft          The left U.
         * @param  VLeft          The left V.
         * @param  URight         The right U.
         * @param  VRight         The right V.
         * @param  ZRight         The right Z.
         * @param  dest           Destination drawing surface.
         * @param  destX          Destination x coordinate.
         * @param  destY          Destination y coordinate.
         * @param  texture        The texture.
         * @param  alpha          The global alpha.
         * @param  dOneOverZdXAff 1/ZdX affine.
         * @param  dUOverZdXAff   U/ZdX affine.
         * @param  dVOverZdXAff   V/ZdX affine.
         */
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff) = 0;

    protected:
        static const fixed16_16 half = 0x8000; ///< 1/2 in fixed16_16 format

        /**
         * Draw texture map next subdivision.
         *
         * @param [out]    ULeft          U left.
         * @param [out]    VLeft          V left.
         * @param [out]    ZRight         Z right.
         * @param [out]    URight         U right.
         * @param [out]    VRight         V right.
         * @param [in,out] oneOverZRight  1/Z right.
         * @param          dOneOverZdXAff d1/ZdX affine.
         * @param [in,out] UOverZRight    U/Z right.
         * @param          dUOverZdXAff   dU/ZdX affine.
         * @param [in,out] VOverZRight    V/Z right.
         * @param          dVOverZdXAff   dV/ZdX affine.
         * @param          affineLength   Length of the affine.
         * @param [out]    U              Bitmap X in fixed16_16.
         * @param [out]    V              Bitmap Y in fixed16_16.
         * @param [out]    deltaU         U delta.
         * @param [out]    deltaV         V delta.
         */
        FORCE_INLINE_FUNCTION void drawTextureMapNextSubdivision(float& ULeft, float& VLeft, float& ZRight, float& URight, float& VRight, float& oneOverZRight, const float dOneOverZdXAff, float& UOverZRight, const float dUOverZdXAff, float& VOverZRight, const float dVOverZdXAff, const int affineLength, fixed16_16& U, fixed16_16& V, fixed16_16& deltaU, fixed16_16& deltaV)
        {
            ULeft = URight;
            VLeft = VRight;

            oneOverZRight += dOneOverZdXAff;
            UOverZRight += dUOverZdXAff;
            VOverZRight += dVOverZdXAff;

            ZRight = 1 / oneOverZRight;
            URight = ZRight * UOverZRight;
            VRight = ZRight * VOverZRight;

            U = floatToFixed16_16(ULeft);
            V = floatToFixed16_16(VLeft);
            deltaU = floatToFixed16_16(URight - ULeft) / affineLength;
            deltaV = floatToFixed16_16(VRight - VLeft) / affineLength;
        }

        /**
         * Check if value is inside [0..limit[.
         *
         * @param [in] value Value to check.
         * @param [in] limit Upper limit.
         *
         * @return true if value is inside given limit.
         */
        FORCE_INLINE_FUNCTION bool is1Inside(int value, int limit)
        {
            return (value >= 0 && value < limit);
        }

        /**
         * Check if (x,y) is inside ([0..width[, [0..height[)
         *
         * @param [in] x      X coordinate.
         * @param [in] y      Y coordinate.
         * @param [in] width  X limit.
         * @param [in] height Y limit.
         *
         * @return true if (x,y) is inside given limits.
         */
        FORCE_INLINE_FUNCTION bool is1x1Inside(int x, int y, int width, int height)
        {
            return is1Inside(x, width) && is1Inside(y, height);
        }

        /**
         * Check if both value and value+1 are inside [0..limit[.
         *
         * @param [in] value Value to check.
         * @param [in] limit Upper limit.
         *
         * @return true if value and value+1 are inside given limit.
         */
        FORCE_INLINE_FUNCTION bool is2Inside(int value, int limit)
        {
            return is1Inside(value, limit - 1);
        }

        /**
         * Check if both (x,y) and (x+1,y+1) are inside ([0..width[,[0..height[)
         *
         * @param [in] x      X coordinate.
         * @param [in] y      Y coordinate.
         * @param [in] width  X limit.
         * @param [in] height Y limit.
         *
         * @return true if (x,y) and (x+1,y+1) are inside given limits.
         */
        FORCE_INLINE_FUNCTION bool is2x2Inside(int x, int y, int width, int height)
        {
            return is2Inside(x, width) && is2Inside(y, height);
        }

        /**
         * Check if either value or value+1 is inside [0..limit[.
         *
         * @param [in] value Value to check.
         * @param [in] limit Upper limit.
         *
         * @return true if either value or value+1 is inside given limit.
         */
        FORCE_INLINE_FUNCTION bool is2PartiallyInside(int value, int limit)
        {
            return is1Inside(value + 1, limit + 1);
        }

        /**
         * Check if either (x,y) or (x+1,y+1) is inside ([0..width[,[0..height[)
         *
         * @param [in] x      X coordinate.
         * @param [in] y      Y coordinate.
         * @param [in] width  X limit.
         * @param [in] height Y limit.
         *
         * @return true if either (x,y) or (x+1,y+1) is inside given limits.
         */
        FORCE_INLINE_FUNCTION bool is2x2PartiallyInside(int x, int y, int width, int height)
        {
            return is2PartiallyInside(x, width) && is2PartiallyInside(y, height);
        }
    };

    /**
     * Gets pointer to object that can draw a scan line which allows for highly specialized
     * and optimized implementation.
     *
     * @param  texture       The texture Surface.
     * @param  renderVariant The render variant.
     * @param  alpha         The global alpha.
     *
     * @return Null if it fails, else the pointer to the texture mapper draw scan line
     *         object.
     */
    virtual DrawTextureMapScanLineBase* getTextureMapperDrawScanLine(const TextureSurface& texture, RenderingVariant renderVariant, uint8_t alpha);

    /**
     * Draw scan line. Draw one horizontal line of the texture map on screen. The scan line
     * will be drawn using perspective correct texture mapping. The appearance of the line
     * is determined by the left and right edge and the gradients structure. The edges
     * contain the information about the x,y,z coordinates of the left and right side
     * respectively and also information about the u,v coordinates of the texture map used.
     * The gradients structure contains information about how to interpolate all the values
     * across the scan line. The data drawn should be present in the texture argument.
     *
     * The scan line will be drawn using the additional arguments. The scan line will be
     * placed and clipped using the absolute and dirty rectangles The alpha will determine
     * how the scan line should be alpha blended. The subDivisionSize will determine the
     * size of the piecewise affine texture mapped lines.
     *
     * @param  dest              The description of where the texture is drawn - can be used
     *                           to issue a draw off screen.
     * @param  gradients         The gradients using in interpolation across the scan line.
     * @param  leftEdge          The left edge of the scan line.
     * @param  rightEdge         The right edge of the scan line.
     * @param  texture           The texture.
     * @param  absoluteRect      The containing rectangle in absolute coordinates.
     * @param  dirtyAreaAbsolute The dirty area in absolute coordinates.
     * @param  renderVariant     The render variant - includes the algorithm and the pixel
     *                           format.
     * @param  alpha             The alpha.
     * @param  subDivisionSize   The size of the subdivisions of the scan line. A value of 1
     *                           will give a completely perspective correct texture mapped
     *                           scan line. A large value will give an affine texture mapped
     *                           scan line.
     */
    virtual void drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionSize);

    /**
     * Private version of draw-glyph with explicit destination buffer pointer argument. For
     * all parameters (except the buffer pointer) see the public function drawString().
     *
     * @param [out] wbuf16          The destination (frame) buffer to draw to.
     * @param       widgetArea      The canvas to draw the glyph inside.
     * @param       x               Horizontal offset to start drawing the glyph.
     * @param       y               Vertical offset to start drawing the glyph.
     * @param       offsetX         Horizontal offset in the glyph to start drawing from.
     * @param       offsetY         Vertical offset in the glyph to start drawing from.
     * @param       invalidatedArea The area to draw inside.
     * @param       glyph           Specifications of the glyph to draw.
     * @param       glyphData       Data containing the actual glyph (dense format)
     * @param       byteAlignRow    Each row of glyph data starts in a new byte.
     * @param       color           The color of the glyph.
     * @param       bitsPerPixel    Bit depth of the glyph.
     * @param       alpha           The transparency of the glyph.
     * @param       rotation        Rotation to do before drawing the glyph.
     */
    virtual void drawGlyph(uint16_t* wbuf16, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, uint8_t byteAlignRow, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation) = 0;

    /**
     * Rotate a rectangle inside another rectangle.
     *
     * @param [in,out] rect     The rectangle to rotate.
     * @param          canvas   The rectangle containing the rect to rotate.
     * @param          rotation Rotation to perform on rect.
     */
    static void rotateRect(Rect& rect, const Rect& canvas, const TextRotation rotation);

    /**
     * Find the real, absolute x coordinate of a point inside a widget with regards to
     * rotation.
     *
     * @param [in] widgetArea The widget containing the point.
     * @param      x          The x coordinate.
     * @param      y          The y coordinate.
     * @param      rotation   Rotation to perform.
     *
     * @return The absolute x coordinate after applying appropriate rotation.
     */
    static int realX(const Rect& widgetArea, int16_t x, int16_t y, TextRotation rotation);

    /**
     * Find the real, absolute y coordinate of a point inside a widget with regards to
     * rotation.
     *
     * @param [in] widgetArea The widget containing the point.
     * @param      x          The x coordinate.
     * @param      y          The y coordinate.
     * @param      rotation   Rotation to perform.
     *
     * @return The absolute y coordinate after applying appropriate rotation.
     */
    static int realY(const Rect& widgetArea, int16_t x, int16_t y, TextRotation rotation);

    /**
     * Draws the specified Unicode string. Breaks line on newline. The string is assumed to
     * contain only Latin characters written left-to-right.
     *
     * @param      widgetArea      The area covered by the drawing widget in absolute
     *                             coordinates.
     * @param      invalidatedArea The (sub)region of the widget area to draw, expressed
     *                             relative to the widget area. If the widgetArea is
     *                             x=10, y=10, width=20, height=20 and invalidatedArea
     *                             is x=5, y=5, width=6, height=6 the widgetArea drawn
     *                             on the LCD is x=15, y=15, width=6, height=6.
     * @param [in] visuals         The string visuals (font, alignment, line space, color)
     *                             with which to draw this string.
     * @param      format          A pointer to a null-terminated text string with optional
     *                             additional wildcard arguments.
     * @param      pArg            Variable arguments providing additional information.
     *
     * @see drawString
     */
    void drawStringLTR(const Rect& widgetArea,
                       const Rect& invalidatedArea,
                       const StringVisuals& visuals,
                       const Unicode::UnicodeChar* format,
                       va_list pArg);

    /**
     * Draws the specified Unicode string. Breaks line on newline. The string can be either
     * right-to-left or left-to-right and may contain sequences of Arabic / Hebrew and Latin
     * characters.
     *
     * @param      widgetArea      The area covered by the drawing widget in absolute
     *                             coordinates.
     * @param      invalidatedArea The (sub)region of the widget area to draw, expressed
     *                             relative to the widget area. If the widgetArea is
     *                             x=10, y=10, width=20, height=20 and invalidatedArea
     *                             is x=5, y=5, width=6, height=6 the widgetArea drawn
     *                             on the LCD is x=15, y=15, width=6, height=6.
     * @param [in] visuals         The string visuals (font, alignment, line space, color)
     *                             with which to draw this string.
     * @param      format          A pointer to a null-terminated text string with optional
     *                             additional wildcard arguments.
     * @param      pArg            Variable arguments providing additional information.
     *
     * @see drawString
     */
    void drawStringRTL(const Rect& widgetArea,
                       const Rect& invalidatedArea,
                       const StringVisuals& visuals,
                       const Unicode::UnicodeChar* format,
                       va_list pArg);

    /**
     * Find string width of the given number of ligatures read from the given TextProvider.
     * After the introduction of Arabic, Thai, Hindi and other languages, ligatures are
     * counted instead of characters. For Latin languages, number of characters equal number
     * of ligatures.
     *
     * @param [in] textProvider  The text provider.
     * @param      font          The font.
     * @param      numChars      Number of characters (ligatures).
     * @param      textDirection The text direction.
     *
     * @return An int16_t.
     */
    static uint16_t stringWidth(TextProvider& textProvider, const Font& font, const int numChars, TextDirection textDirection);

    /**
     * Gets number of lines for a given text taking word wrap into consideration. The font
     * and width are required to find the number of lines in case word wrap is true.
     *
     * @param [in] textProvider   The text provider.
     * @param      wideTextAction The wide text action in case lines are longer than the width
     *                            of the text area.
     * @param      textDirection  The text direction (LTR or RTL).
     * @param      font           The font.
     * @param      width          The width.
     *
     * @return The number lines.
     */
    static uint16_t getNumLines(TextProvider& textProvider, WideTextAction wideTextAction, TextDirection textDirection, const Font* font, int16_t width);

    friend class Font;
    friend class TextArea;
    friend class TextAreaWithWildcardBase;

    /**
     * Gets alpha from A4 image at given offset. The value is scaled up from range 0-15 to 0-
     * 255.
     *
     * @param  data   A pointer to the start of the A4 data.
     * @param  offset The offset into the A4 image.
     *
     * @return The alpha from A4 (0-255).
     */
    FORCE_INLINE_FUNCTION static uint8_t getAlphaFromA4(const uint16_t* data, uint32_t offset)
    {
        uint8_t byte = reinterpret_cast<const uint8_t*>(data)[offset / 2];
        return ((offset & 1) == 0 ? byte & 0xF : byte >> 4) * 0x11;
    }

private:
    DrawTextureMapScanLineBase* textureMapperClass; ///< Used during faster TextureMapper rendering

    /** A draw string internal structure. */
    class DrawStringInternalStruct
    {
    public:
        uint16_t* frameBuffer;
        const Rect* widgetArea;
        int16_t widgetRectY;
        const Rect* toDraw;
        const StringVisuals* stringVisuals;

        /** Initializes a new instance of the DrawStringInternalStruct class. */
        DrawStringInternalStruct()
            : frameBuffer(0), widgetArea(0), widgetRectY(0), toDraw(0), stringVisuals(0)
        {
        }
    };
    void drawStringRTLLine(int16_t& offset, const Font* font, TextDirection textDirection, TextProvider& textProvider, const int numChars, const bool useEllipsis, DrawStringInternalStruct const* data);
    void drawStringRTLInternal(int16_t& offset, const Font* font, const TextDirection textDirection, TextProvider& drawTextProvider, const int numChars, const uint16_t widthOfNumChars, DrawStringInternalStruct const* data);
    bool drawStringInternal(uint16_t* frameBuffer, Rect const* widgetArea, int16_t widgetRectY, int16_t& offset, const Rect& invalidatedArea, StringVisuals const* stringVisuals, const TextDirection textDirection, TextProvider& textProvider, const int numChars, bool useEllipsis);

    /** A wide text internal structure. */
    class WideTextInternalStruct
    {
    public:
        /**
         * Initializes a new instance of the WideTextInternalStruct class.
         *
         * @param [in] _textProvider  The text provider.
         * @param      _maxWidth      The maximum width.
         * @param      _textDirection The text direction.
         * @param      _font          The font.
         * @param      action         The action.
         */
        WideTextInternalStruct(TextProvider& _textProvider, uint16_t _maxWidth, TextDirection _textDirection, const Font* _font, WideTextAction action)
            : currChar(0), textProvider(_textProvider), textDirection(_textDirection), wideTextAction(action), font(_font), maxWidth(_maxWidth), charsRead(0), width(0), charsReadAhead(0), widthAhead(0), widthWithoutWhiteSpaceAtEnd(0), ellipsisGlyphWidth(0), useEllipsis(false)
        {
            Unicode::UnicodeChar ellipsisChar = font->getEllipsisChar();
            if (ellipsisChar != 0)
            {
                const GlyphNode* ellipsisGlyph = font->getGlyph(ellipsisChar);
                ellipsisGlyphWidth = ellipsisGlyph->advance();
                if (wideTextAction == WIDE_TEXT_CHARWRAP_DOUBLE_ELLIPSIS)
                {
                    ellipsisGlyphWidth += font->getKerning(ellipsisChar, ellipsisGlyph) + ellipsisGlyph->advance();
                }
            }
        }

        /**
         * Adds a word.
         *
         * @param  widthBeforeCurrChar        The width before curr character.
         * @param  widthBeforeWhiteSpaceAtEnd The width before white space at end.
         * @param  charsReadTooMany           The characters read too many.
         */
        void addWord(uint16_t widthBeforeCurrChar, uint16_t widthBeforeWhiteSpaceAtEnd, uint16_t charsReadTooMany);

        /**
         * Gets string length for line.
         *
         * @param  useWideTextEllipsisFlag True to use wide text ellipsis flag.
         */
        void getStringLengthForLine(bool useWideTextEllipsisFlag);

        /**
         * Query if 'ch' is space.
         *
         * @param  ch The ch.
         *
         * @return True if space, false if not.
         */
        bool isSpace(Unicode::UnicodeChar ch)
        {
            return ch == ' ' || ch == 0x200B;
        }

        /**
         * Gets curr character.
         *
         * @return The curr character.
         */
        Unicode::UnicodeChar getCurrChar() const
        {
            return currChar;
        }

        /**
         * Gets line width.
         *
         * @return The line width.
         */
        uint16_t getLineWidth() const
        {
            return widthWithoutWhiteSpaceAtEnd;
        }

        /**
         * Gets characters read.
         *
         * @return The characters read.
         */
        uint16_t getCharsRead() const
        {
            return charsRead;
        }

        /**
         * Gets use ellipsis.
         *
         * @return True if it succeeds, false if it fails.
         */
        bool getUseEllipsis() const
        {
            return useEllipsis;
        }

    private:
        Unicode::UnicodeChar currChar;
        TextProvider& textProvider;
        TextDirection textDirection;
        WideTextAction wideTextAction;
        const Font* font;
        uint16_t maxWidth;
        uint16_t charsRead;
        uint16_t width;
        uint16_t charsReadAhead;
        uint16_t widthAhead;
        uint16_t widthWithoutWhiteSpaceAtEnd;
        uint16_t ellipsisGlyphWidth;
        bool useEllipsis;
    };
};

} // namespace touchgfx

#endif // TOUCHGFX_LCD_HPP
