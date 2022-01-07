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
#include <touchgfx/widgets/graph/GraphScroll.hpp>

namespace touchgfx
{
DataGraphScroll::DataGraphScroll(int16_t capacity, int* values)
    : AbstractDataGraphWithY(capacity, values), current(0)
{
}

void DataGraphScroll::clear()
{
    AbstractDataGraphWithY::clear();
    current = 0;
}

int32_t DataGraphScroll::indexToGlobalIndex(int16_t index) const
{
    if (usedCapacity < maxCapacity)
    {
        return realIndex(index);
    }
    return (dataCounter - maxCapacity) + index;
}

void DataGraphScroll::beforeAddValue()
{
    if (usedCapacity == maxCapacity)
    {
        invalidateAllXAxisPoints();
    }
}

int16_t DataGraphScroll::addValue(int value)
{
    const bool graphFull = usedCapacity == maxCapacity;
    const int16_t index = current++;
    current %= maxCapacity;
    if (index == usedCapacity)
    {
        usedCapacity++;
    }
    yValues[index] = value;
    if (graphFull)
    {
        invalidateGraphArea();
        invalidateAllXAxisPoints();
    }
    else
    {
        invalidateGraphPointAt(index);
    }
    return index;
}

int16_t DataGraphScroll::realIndex(int16_t index) const
{
    return usedCapacity < maxCapacity ? index : (index + current) % maxCapacity;
}

CWRUtil::Q5 DataGraphScroll::indexToXQ5(int16_t index) const
{
    return CWRUtil::toQ5(index);
}

} // namespace touchgfx
