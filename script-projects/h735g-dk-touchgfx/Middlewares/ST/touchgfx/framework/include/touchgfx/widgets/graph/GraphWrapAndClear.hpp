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
 * @file touchgfx/widgets/graph/GraphWrapAndClear.hpp
 *
 * Declares the touchgfx::DataGraphWrapAndClear and touchgfx::GraphWrapAndClear classes.
 */
#ifndef TOUCHGFX_GRAPHWRAPANDCLEAR_HPP
#define TOUCHGFX_GRAPHWRAPANDCLEAR_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/graph/AbstractDataGraph.hpp>

namespace touchgfx
{
/**
 * The DataGraphWrapAndClear will show new points progressing across the graph. Once the graph is
 * filled, the next point added will cause the graph to be cleared and a new graph will slowly
 * be created as new values are added.
 */
class DataGraphWrapAndClear : public AbstractDataGraphWithY
{
public:
    /**
     * Initializes a new instance of the DataGraphWrapAndOverwrite class.
     *
     * @param      capacity The capacity.
     * @param [in] values   Pointer to memory with room for capacity elements of type T.
     */
    DataGraphWrapAndClear(int16_t capacity, int* values);

    virtual int32_t indexToGlobalIndex(int16_t index) const;

protected:
    virtual void beforeAddValue();

    virtual int16_t addValue(int value);
};

/**
 * The GraphWrapAndClear will show new points progressing across the graph. Once the graph is filled,
 * the next point added will cause the graph to be cleared and a new graph will slowly be
 * created as new values are added.
 */
template <int16_t CAPACITY>
class GraphWrapAndClear : public DataGraphWrapAndClear
{
public:
    GraphWrapAndClear()
        : DataGraphWrapAndClear(CAPACITY, yValues)
    {
    }

private:
    int yValues[CAPACITY];
};

} // namespace touchgfx

#endif // TOUCHGFX_GRAPHWRAPANDCLEAR_HPP
