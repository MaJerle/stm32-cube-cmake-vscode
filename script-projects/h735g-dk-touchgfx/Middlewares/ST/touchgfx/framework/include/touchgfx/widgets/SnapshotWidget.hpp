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
 * @file touchgfx/widgets/SnapshotWidget.hpp
 *
 * Declares the touchgfx::SnapshotWidget class.
 */
#ifndef TOUCHGFX_SNAPSHOTWIDGET_HPP
#define TOUCHGFX_SNAPSHOTWIDGET_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
/**
 * A widget that is able to make a snapshot of the area the SnapshotWidget covers into either a
 * Bitmap or into animation storage (if this available). Once the snapshot has been
 * taken using SnapshowWidget::makeSnapshot(), the SnapshotWidget will show the captured
 * snapshot when it is subsequently drawn.
 */
class SnapshotWidget : public Widget
{
public:
    SnapshotWidget();

    virtual void draw(const Rect& invalidatedArea) const;

    virtual Rect getSolidRect() const;

    /**
     * Makes a snapshot of the area the SnapshotWidget currently covers. This area is
     * defined by setting the dimensions and the position of the SnapshotWidget. The
     * snapshot is stored in Animation Storage.
     *
     * @see setPosition
     */
    virtual void makeSnapshot();

    /**
     * Makes a snapshot of the area the SnapshotWidget currently covers. This area is
     * defined by setting the dimensions and the position of the SnapshotWidget. The
     * snapshot is stored in the provided dynamic bitmap. The format of the Bitmap must
     * match the format of the display.
     *
     * @param  bmp The target dynamic bitmap.
     */
    virtual void makeSnapshot(const BitmapId bmp);

    /**
     * @copydoc Image::setAlpha
     */
    void setAlpha(const uint8_t newAlpha)
    {
        alpha = newAlpha;
    }

    /**
     * @copydoc Image::getAlpha
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

protected:
    BitmapId bitmapId; ///< BitmapId where copy is stored s copied to.
    uint8_t alpha;     ///< The alpha with which to draw this snapshot.
};

} // namespace touchgfx

#endif // TOUCHGFX_SNAPSHOTWIDGET_HPP
