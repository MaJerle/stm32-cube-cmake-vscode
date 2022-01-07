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
 * @file touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp
 *
 * Declares the touchgfx::AbstractDirectionProgress class.
 */
#ifndef TOUCHGFX_ABSTRACTDIRECTIONPROGRESS_HPP
#define TOUCHGFX_ABSTRACTDIRECTIONPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>

namespace touchgfx
{
/**
 * An abstract class for progress indicators that need a horizontal or vertical direction to be
 * specified.
 */
class AbstractDirectionProgress : public AbstractProgressIndicator
{
public:
    /** Values that represent directions. */
    enum DirectionType
    {
        RIGHT, ///< Progress should be from left to right
        LEFT,  ///< Progress should be from right to left
        DOWN,  ///< Progress should be down (top to bottom)
        UP     ///< Progress should be up (bottom to top)
    };

    AbstractDirectionProgress();

    /**
     * Sets a direction for the progress indicator. This will re-calculate the current value
     * according to the new direction.
     *
     * @param  direction The direction.
     *
     * @see getDirection
     */
    virtual void setDirection(DirectionType direction);

    /**
     * Gets the current direction for the progress indicator.
     *
     * @return The direction.
     *
     * @see setDirection
     */
    virtual DirectionType getDirection() const;

protected:
    DirectionType progressDirection; ///< The progress direction
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTDIRECTIONPROGRESS_HPP
