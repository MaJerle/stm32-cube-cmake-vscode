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
#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>
#include <touchgfx/containers/progress_indicators/LineProgress.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/widgets/canvas/CWRUtil.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>

namespace touchgfx
{
LineProgress::LineProgress()
    : AbstractProgressIndicator(), line(), startX(0), startY(0), endX(0), endY(0)
{
    progressIndicatorContainer.add(line);
    line.setPosition(0, 0, getWidth(), getHeight());
}

void LineProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    line.setPosition(0, 0, width, height);

    AbstractProgressIndicator::setProgressIndicatorPosition(x, y, width, height);
}

void LineProgress::setPainter(AbstractPainter& painter)
{
    line.setPainter(painter);
}

void LineProgress::setStart(int x, int y)
{
    startX = CWRUtil::toQ5<int>(x);
    startY = CWRUtil::toQ5<int>(y);
    line.setStart(x, y);
}

void LineProgress::getStart(int& x, int& y) const
{
    x = startX.to<int>();
    y = startY.to<int>();
}

void LineProgress::setEnd(int x, int y)
{
    endX = CWRUtil::toQ5<int>(x);
    endY = CWRUtil::toQ5<int>(y);
}

void LineProgress::getEnd(int& x, int& y) const
{
    x = endX.to<int>();
    y = endY.to<int>();
}

void LineProgress::setLineWidth(int width)
{
    line.setLineWidth(width);
}

int LineProgress::getLineWidth() const
{
    int width;
    line.getLineWidth(width);
    return width;
}

void LineProgress::setLineEndingStyle(Line::LINE_ENDING_STYLE lineEndingStyle)
{
    line.setLineEndingStyle(lineEndingStyle);
}

Line::LINE_ENDING_STYLE LineProgress::getLineEndingStyle() const
{
    return line.getLineEndingStyle();
}

void LineProgress::setAlpha(uint8_t newAlpha)
{
    AbstractProgressIndicator::setAlpha(newAlpha);
    line.setAlpha(newAlpha);
}

void LineProgress::setValue(int value)
{
    if (rangeSteps > 0)
    {
        AbstractProgressIndicator::setValue(value);
        int progress = (int)AbstractProgressIndicator::getProgress(rangeSteps);
        CWRUtil::Q5 r(rangeSteps);
        CWRUtil::Q5 p(progress);
        CWRUtil::Q5 x = startX + (endX - startX) / r * p;
        CWRUtil::Q5 y = startY + (endY - startY) / r * p;
        line.updateEnd(x, y);
    }
}
} // namespace touchgfx
