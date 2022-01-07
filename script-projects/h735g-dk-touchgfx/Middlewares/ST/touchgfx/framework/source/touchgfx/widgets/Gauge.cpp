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
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>
#include <touchgfx/widgets/Gauge.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>

namespace touchgfx
{
Gauge::Gauge()
    : AbstractProgressIndicator(),
      needle(),
      algorithmMoving(TextureMapper::BILINEAR_INTERPOLATION),
      algorithmSteady(TextureMapper::BILINEAR_INTERPOLATION),
      needleStartAngle(0),
      needleEndAngle(0),
      gaugeCenterX(0),
      gaugeCenterY(0),
      needleCenterX(0),
      needleCenterY(0),
      arc()
{
    remove(progressIndicatorContainer);
    add(arc);
    add(needle);
    arc.setVisible(false);
}

void Gauge::setWidth(int16_t width)
{
    AbstractProgressIndicator::setWidth(width);
    needle.setWidth(width);
    arc.setWidth(width);
}

void Gauge::setHeight(int16_t height)
{
    AbstractProgressIndicator::setHeight(height);
    needle.setHeight(height);
    arc.setHeight(height);
}

void Gauge::setBackgroundOffset(int16_t offsetX, int16_t offsetY)
{
    background.setXY(offsetX, offsetY);
}

void Gauge::setCenter(int x, int y)
{
    gaugeCenterX = x;
    gaugeCenterY = y;
    setupNeedleTextureMapper();
    arc.setPixelCenter(x - arc.getX(), y - arc.getY());
}

void Gauge::setArcPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    arc.setPosition(x, y, width, height);
    arc.setPixelCenter(gaugeCenterX - x, gaugeCenterY - y);
}

void Gauge::setNeedle(const BitmapId bitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    needle.setBitmap(Bitmap(bitmapId));
    needleCenterX = rotationCenterX;
    needleCenterY = rotationCenterY;
    setupNeedleTextureMapper();
}

void Gauge::setMovingNeedleRenderingAlgorithm(TextureMapper::RenderingAlgorithm algorithm)
{
    algorithmMoving = algorithm;
}

void Gauge::setSteadyNeedleRenderingAlgorithm(TextureMapper::RenderingAlgorithm algorithm)
{
    algorithmSteady = algorithm;
}

void Gauge::setStartEndAngle(int startAngle, int endAngle)
{
    assert(startAngle != endAngle);
    needleStartAngle = startAngle;
    needleEndAngle = endAngle;
    arc.setArc(startAngle, endAngle);
    Gauge::setValue(Gauge::getValue());
}

int Gauge::getStartAngle() const
{
    return needleStartAngle;
}

int Gauge::getEndAngle() const
{
    return needleEndAngle;
}

void Gauge::setArcVisible(bool show /*= true*/)
{
    arc.setVisible(show);
}

void Gauge::putArcOnTop(bool onTop /*= true*/)
{
    if (onTop)
    {
        remove(arc);
        add(arc);
    }
    else
    {
        remove(needle);
        add(needle);
    }
}

Circle& Gauge::getArc()
{
    return arc;
}

void Gauge::setValue(int value)
{
    AbstractProgressIndicator::setValue(value);
    if (animationStep >= animationDuration)
    {
        needle.setRenderingAlgorithm(algorithmSteady);
    }
    else
    {
        needle.setRenderingAlgorithm(algorithmMoving);
    }
    uint16_t progress = AbstractProgressIndicator::getProgress(abs(needleEndAngle - needleStartAngle));
    if (needleEndAngle < needleStartAngle)
    {
        needle.updateZAngle((needleStartAngle - progress) / 180.0f * PI);
        arc.updateArcEnd(needleStartAngle - progress);
    }
    else
    {
        needle.updateZAngle((needleStartAngle + progress) / 180.0f * PI);
        arc.updateArcEnd(needleStartAngle + progress);
    }
}

void Gauge::setAlpha(uint8_t newAlpha)
{
    AbstractProgressIndicator::setAlpha(newAlpha);
    needle.setAlpha(newAlpha);
    arc.setAlpha(newAlpha);
}

void Gauge::setupNeedleTextureMapper()
{
    needle.setWidthHeight(*this);
    needle.setXY(0, 0);
    needle.setBitmapPosition(gaugeCenterX - needleCenterX, gaugeCenterY - needleCenterY);
    needle.setCameraDistance(300.0f);
    needle.setOrigo((float)gaugeCenterX, (float)gaugeCenterY, needle.getCameraDistance());
    needle.setCamera(needle.getOrigoX(), needle.getOrigoY());
    needle.setRenderingAlgorithm(TextureMapper::BILINEAR_INTERPOLATION);
}

void Gauge::setProgressIndicatorPosition(int16_t /*x*/, int16_t /*y*/, int16_t /*width*/, int16_t /*height*/)
{
}

} // namespace touchgfx
