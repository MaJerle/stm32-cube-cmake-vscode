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
 * @file touchgfx/containers/progress_indicators/BoxProgress.hpp
 *
 * Declares the touchgfx::BoxProgress class.
 */
#ifndef TOUCHGFX_BOXPROGRESS_HPP
#define TOUCHGFX_BOXPROGRESS_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/widgets/Box.hpp>

namespace touchgfx
{
/**
 * A BoxProgress which shows the current progress using a simple Box. It is possible to set the
 * color and the alpha of the box. It is also possible to control in what direction the
 * box will progress (up, down, to the left or to the right).
 */
class BoxProgress : public AbstractDirectionProgress
{
public:
    BoxProgress();

    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * Sets the color of the Box.
     *
     * @param  color The color.
     * @see getColor
     */
    virtual void setColor(colortype color);

    /**
     * Gets the color of the Box.
     *
     * @return The color.
     *
     * @see setColor
     */
    virtual colortype getColor() const;

    /**
     * @copydoc Image::setAlpha
     */
    virtual void setAlpha(uint8_t newAlpha);

    virtual void setValue(int value);

protected:
    Box box; ///< The box
};

} // namespace touchgfx

#endif // TOUCHGFX_BOXPROGRESS_HPP
