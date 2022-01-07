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
#include <touchgfx/Drawable.hpp>
#include <touchgfx/Utils.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>

namespace touchgfx
{
ScalableImage::ScalableImage(const Bitmap& bitmap /*= Bitmap() */)
    : Image(bitmap),
      currentScalingAlgorithm(BILINEAR_INTERPOLATION)
{
}

void ScalableImage::setScalingAlgorithm(ScalingAlgorithm algorithm)
{
    currentScalingAlgorithm = algorithm;
}

ScalableImage::ScalingAlgorithm ScalableImage::getScalingAlgorithm()
{
    return currentScalingAlgorithm;
}

void ScalableImage::drawQuad(const Rect& invalidatedArea, uint16_t* fb, const float* triangleXs, const float* triangleYs, const float* triangleZs, const float* triangleUs, const float* triangleVs) const
{
    // Area to redraw. Relative to the scalableImage.
    Rect dirtyArea = Rect(0, 0, getWidth(), getHeight()) & invalidatedArea;

    // Absolute position of the scalableImage.
    Rect dirtyAreaAbsolute = dirtyArea;
    translateRectToAbsolute(dirtyAreaAbsolute);

    Rect absoluteRect = getAbsoluteRect();
    DisplayTransformation::transformDisplayToFrameBuffer(absoluteRect);

    // Transform rects to match framebuffer coordinates
    // This is needed if the display is rotated compared to the framebuffer
    DisplayTransformation::transformDisplayToFrameBuffer(dirtyArea, this->getRect());
    DisplayTransformation::transformDisplayToFrameBuffer(dirtyAreaAbsolute);

    // Get a pointer to the bitmap data, return if no bitmap found
    const uint16_t* textmap = (const uint16_t*)bitmap.getData();
    if (!textmap)
    {
        return;
    }

    float x0 = triangleXs[0];
    float x1 = triangleXs[1];
    float x2 = triangleXs[2];
    float x3 = triangleXs[3];
    float y0 = triangleYs[0];
    float y1 = triangleYs[1];
    float y2 = triangleYs[2];
    float y3 = triangleYs[3];

    DisplayTransformation::transformDisplayToFrameBuffer(x0, y0, this->getRect());
    DisplayTransformation::transformDisplayToFrameBuffer(x1, y1, this->getRect());
    DisplayTransformation::transformDisplayToFrameBuffer(x2, y2, this->getRect());
    DisplayTransformation::transformDisplayToFrameBuffer(x3, y3, this->getRect());

    Point3D vertices[4];
    Point3D point0 = { floatToFixed28_4(x0), floatToFixed28_4(y0), (float)(triangleZs[0]), (float)(triangleUs[0]), (float)(triangleVs[0]) };
    Point3D point1 = { floatToFixed28_4(x1), floatToFixed28_4(y1), (float)(triangleZs[1]), (float)(triangleUs[1]), (float)(triangleVs[1]) };
    Point3D point2 = { floatToFixed28_4(x2), floatToFixed28_4(y2), (float)(triangleZs[2]), (float)(triangleUs[2]), (float)(triangleVs[2]) };
    Point3D point3 = { floatToFixed28_4(x3), floatToFixed28_4(y3), (float)(triangleZs[3]), (float)(triangleUs[3]), (float)(triangleVs[3]) };

    vertices[0] = point0;
    vertices[1] = point1;
    vertices[2] = point2;
    vertices[3] = point3;

    DrawingSurface dest = { fb, HAL::FRAME_BUFFER_WIDTH };
    TextureSurface src = { textmap, bitmap.getExtraData(), bitmap.getWidth(), bitmap.getHeight(), bitmap.getWidth() };

    HAL::lcd().drawTextureMapQuad(dest, vertices, src, absoluteRect, dirtyAreaAbsolute, lookupRenderVariant(), alpha, 0xFFFF);
}

RenderingVariant ScalableImage::lookupRenderVariant() const
{
    RenderingVariant renderVariant;
    if (currentScalingAlgorithm == NEAREST_NEIGHBOR)
    {
        renderVariant = lookupNearestNeighborRenderVariant(bitmap);
    }
    else
    {
        renderVariant = lookupBilinearRenderVariant(bitmap);
    }
    return renderVariant;
}

void ScalableImage::draw(const Rect& invalidatedArea) const
{
    if (!alpha)
    {
        return;
    }
    uint16_t* fb = 0;

    float triangleXs[4];
    float triangleYs[4];
    float triangleZs[4];
    float triangleUs[4];
    float triangleVs[4];

    float imageX0 = 0;
    float imageY0 = 0;
    float imageX1 = imageX0 + getWidth();
    float imageY1 = imageY0;
    float imageX2 = imageX1;
    float imageY2 = imageY0 + getHeight();
    float imageX3 = imageX0;
    float imageY3 = imageY2;

    triangleZs[0] = 100.f;
    triangleZs[1] = 100.f;
    triangleZs[2] = 100.f;
    triangleZs[3] = 100.f;

    // Setup texture coordinates
    float right = (float)(bitmap.getWidth());
    float bottom = (float)(bitmap.getHeight());
    float textureU0 = 0.0f;
    float textureV0 = 0.0f;
    float textureU1 = right;
    float textureV1 = 0.0f;
    float textureU2 = right;
    float textureV2 = bottom;
    float textureU3 = 0.0f;
    float textureV3 = bottom;
    if (HAL::DISPLAY_ROTATION == rotate90)
    {
        textureU0 = 0.0f;
        textureV0 = right;
        textureU1 = 0.0f;
        textureV1 = 0.0f;
        textureU2 = bottom;
        textureV2 = 0.0f;
        textureU3 = bottom;
        textureV3 = right;
    }

    triangleXs[0] = imageX0;
    triangleXs[1] = imageX1;
    triangleXs[2] = imageX2;
    triangleXs[3] = imageX3;
    triangleYs[0] = imageY0;
    triangleYs[1] = imageY1;
    triangleYs[2] = imageY2;
    triangleYs[3] = imageY3;

    triangleUs[0] = textureU0;
    triangleUs[1] = textureU1;
    triangleUs[2] = textureU2;
    triangleUs[3] = textureU3;
    triangleVs[0] = textureV0;
    triangleVs[1] = textureV1;
    triangleVs[2] = textureV2;
    triangleVs[3] = textureV3;

    drawQuad(invalidatedArea, fb, triangleXs, triangleYs, triangleZs, triangleUs, triangleVs);
}

Rect ScalableImage::getSolidRect() const
{
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }

    // If original image is completely solid the scaled image will also be
    if ((bitmap.getSolidRect().width == bitmap.getWidth()) && (bitmap.getSolidRect().height == bitmap.getHeight()))
    {
        return bitmap.getSolidRect();
    }
    return Rect(0, 0, 0, 0);
}
} // namespace touchgfx
