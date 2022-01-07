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
#include <touchgfx/widgets/graph/AbstractDataGraph.hpp>
#include <touchgfx/widgets/graph/GraphWrapAndOverwrite.hpp>

namespace touchgfx
{
DataGraphWrapAndOverwrite::DataGraphWrapAndOverwrite(int16_t capacity, int* values)
    : AbstractDataGraphWithY(capacity, values), current(0)
{
}

void DataGraphWrapAndOverwrite::clear()
{
    AbstractDataGraphWithY::clear();
    current = 0;
}

int32_t DataGraphWrapAndOverwrite::indexToGlobalIndex(int16_t index) const
{
    if (this->usedCapacity < this->maxCapacity)
    {
        return index;
    }
    const int16_t gapIndex = this->getGapBeforeIndex();
    if (index < gapIndex)
    {
        return (this->dataCounter - gapIndex) + index;
    }
    return ((this->dataCounter - gapIndex) - this->maxCapacity) + index;
}

void DataGraphWrapAndOverwrite::beforeAddValue()
{
    if (current == 0 && usedCapacity >= maxCapacity)
    {
        int xMin = getGraphRangeXMin();
        int xMax = getGraphRangeXMax();
        for (int i = xMin; i < 0; i++)
        {
            invalidateXAxisPointAt(i);
        }
        for (int i = maxCapacity; i <= xMax; i++)
        {
            invalidateXAxisPointAt(i);
        }
    }
    if (usedCapacity >= maxCapacity)
    {
        invalidateGraphPointAt(current);
        invalidateXAxisPointAt(current);
    }
}

int16_t DataGraphWrapAndOverwrite::addValue(int value)
{
    const int16_t index = current++;
    current %= maxCapacity;
    if (index == usedCapacity)
    {
        usedCapacity++;
    }
    yValues[realIndex(index)] = value;
    setGapBeforeIndex(index + 1);
    invalidateGraphPointAt(index);
    if (usedCapacity >= maxCapacity)
    {
        invalidateXAxisPointAt(index);
    }
    if (index == 0 && usedCapacity >= maxCapacity)
    {
        int xMin = getGraphRangeXMin();
        int xMax = getGraphRangeXMax();
        for (int i = xMin; i < 0; i++)
        {
            invalidateXAxisPointAt(i);
        }
        for (int i = maxCapacity; i <= xMax; i++)
        {
            invalidateXAxisPointAt(i);
        }
    }
    return index;
}

} // namespace touchgfx
