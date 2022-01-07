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
#include <touchgfx/Application.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/Utils.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>

namespace touchgfx
{
AbstractProgressIndicator::AbstractProgressIndicator()
    : Container(), background(), progressIndicatorContainer(), rangeMin(0), rangeMax(100), currentValue(0), rangeSteps(100), rangeStepsMin(0),
      equation(&EasingEquations::linearEaseNone), animationRunning(false), animationStartValue(0), animationEndValue(0), animationDuration(0), animationStep(0),
      valueSetCallback(0), valueUpdatedCallback(0)
{
    background.setXY(0, 0);
    AbstractProgressIndicator::add(background);
    AbstractProgressIndicator::add(progressIndicatorContainer);
}

void AbstractProgressIndicator::setBackground(const Bitmap& bitmapBackground)
{
    background.setBitmap(bitmapBackground);
    setWidthHeight(background);
}

void AbstractProgressIndicator::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    progressIndicatorContainer.setPosition(x, y, width, height);

    if (getWidth() < x + width)
    {
        AbstractProgressIndicator::setWidth(x + width);
    }

    if (getHeight() < y + height)
    {
        AbstractProgressIndicator::setHeight(y + height);
    }
}

int16_t AbstractProgressIndicator::getProgressIndicatorX() const
{
    return progressIndicatorContainer.getX();
}

int16_t AbstractProgressIndicator::getProgressIndicatorY() const
{
    return progressIndicatorContainer.getY();
}

int16_t AbstractProgressIndicator::getProgressIndicatorWidth() const
{
    return progressIndicatorContainer.getWidth();
}

int16_t AbstractProgressIndicator::getProgressIndicatorHeight() const
{
    return progressIndicatorContainer.getHeight();
}

void AbstractProgressIndicator::setRange(int min, int max, uint16_t steps /*= 0*/, uint16_t minStep /*= 0*/)
{
    assert(min < max);
    rangeMin = min;
    rangeMax = max;
    setValue(currentValue);
    if (steps == 0)
    {
        rangeSteps = max - min;
    }
    else
    {
        rangeSteps = steps;
    }
    rangeStepsMin = minStep;
    assert(rangeStepsMin < rangeSteps);
}

void AbstractProgressIndicator::getRange(int& min, int& max, uint16_t& steps, uint16_t& minStep) const
{
    min = rangeMin;
    max = rangeMax;
    steps = rangeSteps;
    minStep = rangeStepsMin;
}

void AbstractProgressIndicator::getRange(int& min, int& max, uint16_t& steps) const
{
    min = rangeMin;
    max = rangeMax;
    steps = rangeSteps;
}

void AbstractProgressIndicator::getRange(int& min, int& max) const
{
    min = rangeMin;
    max = rangeMax;
}

void AbstractProgressIndicator::setValue(int value)
{
    value = MAX(value, rangeMin);
    value = MIN(value, rangeMax);
    if (value != currentValue)
    {
        currentValue = value;
        if (valueSetCallback && valueSetCallback->isValid())
        {
            valueSetCallback->execute(*this);
        }
    }
}

void AbstractProgressIndicator::setEasingEquation(EasingEquation easingEquation)
{
    equation = easingEquation;
}

void AbstractProgressIndicator::updateValue(int value, uint16_t duration)
{
    value = MAX(value, rangeMin);
    value = MIN(value, rangeMax);
    if (duration == 0)
    {
        setValue(value);
        if (valueUpdatedCallback && valueUpdatedCallback->isValid())
        {
            valueUpdatedCallback->execute(*this);
        }
        return;
    }
    if (animationDuration > 0)
    {
        // Old animation is running, stop it first
        Application::getInstance()->unregisterTimerWidget(this);
        animationRunning = false;
    }
    animationStartValue = getValue();
    animationEndValue = value;
    animationDuration = duration;
    animationStep = 0;
    Application::getInstance()->registerTimerWidget(this);
    animationRunning = true;
}

int AbstractProgressIndicator::getValue() const
{
    return currentValue;
}

uint16_t AbstractProgressIndicator::getProgress(uint16_t range /*= 100*/) const
{
    if (range == 0)
    {
        return 0;
    }
    int32_t remainder; // Not used here
    // Find out at what step the current value is.
    int32_t step = rangeStepsMin + muldiv(currentValue - rangeMin, rangeSteps - rangeStepsMin, rangeMax - rangeMin, remainder);
    // Scale the step up to [0..range]
    int32_t prog = muldiv(step, range, rangeSteps, remainder);
    return (uint16_t)prog;
}

void AbstractProgressIndicator::setValueSetAction(GenericCallback<const AbstractProgressIndicator&>& callback)
{
    valueSetCallback = &callback;
}

void AbstractProgressIndicator::handleTickEvent()
{
    if (!animationRunning)
    {
        return;
    }
    animationStep++;
    int16_t delta = (int16_t)equation(animationStep, 0, animationEndValue - animationStartValue, animationDuration);
    setValue(animationStartValue + delta);
    if (animationStep >= animationDuration)
    {
        animationDuration = 0;
        animationStep = 0;
        Application::getInstance()->unregisterTimerWidget(this);
        animationRunning = false;
        if (valueUpdatedCallback && valueUpdatedCallback->isValid())
        {
            valueUpdatedCallback->execute(*this);
        }
    }
}

void AbstractProgressIndicator::setValueUpdatedAction(GenericCallback<const AbstractProgressIndicator&>& callback)
{
    valueUpdatedCallback = &callback;
}

void AbstractProgressIndicator::setAlpha(uint8_t newAlpha)
{
    background.setAlpha(newAlpha);
}

uint8_t AbstractProgressIndicator::getAlpha() const
{
    return background.getAlpha();
}

} // namespace touchgfx
