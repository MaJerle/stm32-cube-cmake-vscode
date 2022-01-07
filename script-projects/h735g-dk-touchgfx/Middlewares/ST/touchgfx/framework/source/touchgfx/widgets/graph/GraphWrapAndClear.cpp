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
#include <touchgfx/widgets/graph/GraphWrapAndClear.hpp>

namespace touchgfx
{
DataGraphWrapAndClear::DataGraphWrapAndClear(int16_t capacity, int* values)
    : AbstractDataGraphWithY(capacity, values)
{
}

int32_t DataGraphWrapAndClear::indexToGlobalIndex(int16_t index) const
{
    return (this->dataCounter - this->usedCapacity) + index;
}

void DataGraphWrapAndClear::beforeAddValue()
{
    if (usedCapacity >= maxCapacity)
    {
        invalidateAllXAxisPoints();
        clear();
        invalidateGraphArea();
    }
}

int16_t DataGraphWrapAndClear::addValue(int value)
{
    const bool clearGraph = (usedCapacity == 0);
    const int16_t index = usedCapacity;
    usedCapacity++;
    yValues[realIndex(index)] = value;
    if (clearGraph)
    {
        // Label sizes might have grown, also invalidate new sizes
        invalidateAllXAxisPoints();
    }
    invalidateGraphPointAt(index);
    return index;
}

} // namespace touchgfx
