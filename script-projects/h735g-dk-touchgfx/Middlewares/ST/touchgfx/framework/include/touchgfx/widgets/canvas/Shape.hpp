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
 * @file touchgfx/widgets/canvas/Shape.hpp
 *
 * Declares the touchgfx::Shape class.
 */
#ifndef TOUCHGFX_SHAPE_HPP
#define TOUCHGFX_SHAPE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/AbstractShape.hpp>
#include <touchgfx/widgets/canvas/CWRUtil.hpp>

namespace touchgfx
{
/**
 * Simple widget capable of drawing a fully filled shape. The shape can be scaled and rotated.
 * The Shape class allows the user to draw any shape and allows the defined shape to be
 * scaled, rotated and moved freely. Example uses could be the hands of a clock.
 *
 * @see AbstractShape
 */
template <uint16_t POINTS>
class Shape : public AbstractShape
{
public:
    virtual int getNumPoints() const
    {
        return POINTS;
    }

    virtual void setCorner(int i, CWRUtil::Q5 x, CWRUtil::Q5 y)
    {
        if (i >= 0 && i < POINTS)
        {
            xCorner[i] = x, yCorner[i] = y;
        }
    }

    virtual CWRUtil::Q5 getCornerX(int i) const
    {
        if (i >= 0 && i < POINTS)
        {
            return xCorner[i];
        }
        return CWRUtil::toQ5<int>(0);
    }

    virtual CWRUtil::Q5 getCornerY(int i) const
    {
        if (i >= 0 && i < POINTS)
        {
            return yCorner[i];
        }
        return CWRUtil::toQ5<int>(0);
    }

protected:
    virtual void setCache(int i, CWRUtil::Q5 x, CWRUtil::Q5 y)
    {
        if (i >= 0 && i < POINTS)
        {
            xCache[i] = x, yCache[i] = y;
        }
    }

    virtual CWRUtil::Q5 getCacheX(int i) const
    {
        if (i >= 0 && i < POINTS)
        {
            return xCache[i];
        }
        return CWRUtil::toQ5<int>(0);
    }

    virtual CWRUtil::Q5 getCacheY(int i) const
    {
        if (i >= 0 && i < POINTS)
        {
            return yCache[i];
        }
        return CWRUtil::toQ5<int>(0);
    }

private:
    CWRUtil::Q5 xCorner[POINTS];
    CWRUtil::Q5 yCorner[POINTS];
    CWRUtil::Q5 xCache[POINTS];
    CWRUtil::Q5 yCache[POINTS];
};

} // namespace touchgfx

#endif // TOUCHGFX_SHAPE_HPP
