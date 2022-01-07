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
 * @file touchgfx/widgets/Gauge.hpp
 *
 * Declares the gauge class.
 */
#ifndef TOUCHGFX_GAUGE_HPP
#define TOUCHGFX_GAUGE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>

namespace touchgfx
{
/**
 * A gauge is a graphical element that shows a needle on a dial, often a speedometer or similar.
 * Much like a progress indicator, the minimum and maximum value of the Gauge, as well as steps
 * can be set. For more information on this, consult the documentation on ProgressIndicators.
 *
 * A Gauge has a needle and optionally an arc that follows the needle.
 */
class Gauge : public AbstractProgressIndicator
{
public:
    /** Initializes a new instance of the Gauge class. */
    Gauge();

    virtual void setWidth(int16_t width);

    virtual void setHeight(int16_t height);

    /**
     * Sets background offset inside the Gauge. If the dial is smaller than the size needed for
     * the Gauge to show the needle, the background image can be moved inside the Gauge.
     *
     * @param  offsetX The offset x coordinate.
     * @param  offsetY The offset y coordinate.
     *
     * @see setBackground
     */
    void setBackgroundOffset(int16_t offsetX, int16_t offsetY);

    /**
     * Sets the center of the texture mapper and the arc inside the Gauge.
     *
     * @param  x The x coordinate of the center of the texture mapper.
     * @param  y The y coordinate of the center of the texture mapper.
     *
     * @see getCenter
     */
    virtual void setCenter(int x, int y);

    /**
     * Gets the texture mapper center coordinates.
     *
     * @param [out] x The x coordinate of the center of the texture mapper.
     * @param [out] y The y coordinate of the center of the texture mapper.
     *
     * @see setCenter
     */
    virtual void getCenter(int& x, int& y) const
    {
        x = gaugeCenterX;
        y = gaugeCenterY;
    }

    /**
     * Sets arc position inside the Gauge. This is especially useful if the arc is using a
     * bitmap painter. If the center has previously been set, the arc center will be updated
     * to be at the same offset relative to the top left corner of the Gauge.
     *
     * @param  x      The x coordinate.
     * @param  y      The y coordinate.
     * @param  width  The width.
     * @param  height The height.
     *
     * @see setCenter, getArc
     */
    void setArcPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * Sets a bitmap for the needle and the rotation point in the needle bitmap.
     *
     * @param  bitmapId        Identifier for the bitmap.
     * @param  rotationCenterX The rotation center x coordinate.
     * @param  rotationCenterY The rotation center y coordinate.
     */
    void setNeedle(const BitmapId bitmapId, int16_t rotationCenterX, int16_t rotationCenterY);

    /**
     * Sets rendering algorithm used when the needle is moving during an animation. For better
     * performance, this can be set to TextureMapper::NEAREST_NEIGHBOR. For nicer graphics, it
     * should be set to TextureMapper::BILINEAR_INTERPOLATION (this is the default behavior).
     *
     * @param  algorithm The algorithm.
     *
     * @see updateValue, setSteadyNeedleRenderingAlgorithm
     */
    void setMovingNeedleRenderingAlgorithm(TextureMapper::RenderingAlgorithm algorithm);

    /**
     * Sets rendering algorithm used when the needle is steady (after an animation). For better
     * performance, this can be set to TextureMapper::NEAREST_NEIGHBOR. For nicer graphics, it
     * should be set to TextureMapper::BILINEAR_INTERPOLATION (this is the default behavior).
     *
     * @param  algorithm The algorithm.
     *
     * @see updateValue, setMovingNeedleRenderingAlgorithm
     */
    void setSteadyNeedleRenderingAlgorithm(TextureMapper::RenderingAlgorithm algorithm);

    /**
     * Sets start and end angle for the needle and arc. By swapping end and start angles, these
     * can progress backwards.
     *
     * @param  startAngle The start angle.
     * @param  endAngle   The end angle.
     */
    virtual void setStartEndAngle(int startAngle, int endAngle);

    /**
     * Gets start angle for the needle (and arc).
     *
     * @return The start angle.
     *
     * @see setStartEndAngle, getEndAngle
     */
    virtual int getStartAngle() const;

    /**
     * Gets end angle. Beware that the value returned is not related to the current progress of
     * the texture mapper but rather the end point of the Gauge when it is at max value.
     *
     * @return The end angle.
     *
     * @see setStartEndAngle
     */
    virtual int getEndAngle() const;

    /**
     * Allow the arc to be shown or hidden.
     *
     * @param  show (Optional) True to show, false to hide. Default is to show the arc.
     */
    void setArcVisible(bool show = true);

    /**
     * Shows the arc on top of the needle. By default the needle is drawn on top of the arc.
     *
     * @param  arcOnTop (Optional) True to put the arc on top of the needle (default), false to
     *                  put the needle on top of the arc.
     */
    void putArcOnTop(bool arcOnTop = true);

    /**
     * Gets a reference to the arc (Circle). This allows for setting radius, line width, painter,
     * etc. on the arc (Circle).
     *
     * @return The arc (Circle).
     */
    Circle& getArc();

    virtual void setValue(int value);

    /**
     * @copydoc Image::setAlpha(uint8_t)
     */
    virtual void setAlpha(uint8_t newAlpha);

protected:
    TextureMapper needle;                              ///< The textureMapper
    TextureMapper::RenderingAlgorithm algorithmMoving; ///< The algorithm used when the needle is moving
    TextureMapper::RenderingAlgorithm algorithmSteady; ///< The algorithm used when the needle is steady
    int needleStartAngle;                              ///< The start angle
    int needleEndAngle;                                ///< The end angle
    int16_t gaugeCenterX;                              ///< The x coordinate of the rotation point of the hands
    int16_t gaugeCenterY;                              ///< The y coordinate of the rotation point of the hands
    int16_t needleCenterX;                             ///< The x coordinate of the rotation point of the hands
    int16_t needleCenterY;                             ///< The y coordinate of the rotation point of the hands
    Circle arc;                                        ///< The arc

    /** Sets up the needle texture mapper. */
    void setupNeedleTextureMapper();

    /**
     * This function has no effect on a Gauge.
     *
     * @param  x      unused
     * @param  y      unused
     * @param  width  unused
     * @param  height unused
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
};

} // namespace touchgfx

#endif // TOUCHGFX_GAUGE_HPP
