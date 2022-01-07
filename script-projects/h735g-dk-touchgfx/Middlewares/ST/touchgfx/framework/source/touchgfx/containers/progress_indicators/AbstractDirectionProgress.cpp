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

#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>

namespace touchgfx
{
AbstractDirectionProgress::AbstractDirectionProgress()
    : AbstractProgressIndicator(), progressDirection(RIGHT)
{
    AbstractDirectionProgress::setDirection(RIGHT);
}

void AbstractDirectionProgress::setDirection(DirectionType direction)
{
    if (direction != progressDirection)
    {
        progressDirection = direction;
        progressIndicatorContainer.invalidate();
        setValue(getValue());
    }
}

AbstractDirectionProgress::DirectionType AbstractDirectionProgress::getDirection() const
{
    return progressDirection;
}
} // namespace touchgfx
