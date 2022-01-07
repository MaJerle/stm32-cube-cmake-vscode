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
 * @file touchgfx/widgets/canvas/Circle.hpp
 *
 * Declares the touchgfx::Circle class.
 */
#ifndef TOUCHGFX_CIRCLE_HPP
#define TOUCHGFX_CIRCLE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/CWRUtil.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>

namespace touchgfx
{
/**
 * Simple widget capable of drawing a circle, or part of a circle (an arc). The Circle can be
 * filled or be drawn as a simple line along the circumference of the circle. Several
 * parameters of the circle can be changed: Center, radius, line width, line cap, start
 * angle and end angle.
 *
 * @note Since the underlying CanwasWidgetRenderer only supports straight lines, the circle is
 *       drawn using many small straight lines segments. The granularity can be adjusted to
 *       match the requirements - large circles need more line segments, small circles need
 *       fewer line segments, to look smooth and round.
 * @note All circle parameters are internally handled as CWRUtil::Q5 which means that floating
 *       point values are rounded down to a fixed number of binary digits, for example:
 *       @code
 *            Circle circle;
 *            circle.setCircle(1.1f, 1.1f, 0.9); // Will use (35/32, 35/32, 28/32) = (1.09375f, 1.09375f, 0.875f)
 *            int x, y, r;
 *            circle.getCenter(&amp;x, &amp;y); // Will return (1, 1)
 *            circle.getRadius(&amp;r); // Will return 0
 *       @endcode.
 */
class Circle : public CanvasWidget
{
public:
    Circle();

    /**
     * Sets the center and radius of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  x The x coordinate of center.
     * @param  y The y coordinate of center.
     * @param  r The radius.
     *
     * @see setCenter, setRadius
     *
     * @note The area containing the Circle is not invalidated.
     */
    template <typename T>
    void setCircle(const T x, const T y, const T r)
    {
        setCenter<T>(x, y);
        setRadius<T>(r);
    }

    /**
     * Sets the center and radius of the Circle.
     *
     * @param  x The x coordinate of center.
     * @param  y The y coordinate of center.
     * @param  r The radius.
     *
     * @see setCenter, setRadius
     *
     * @note The area containing the Circle is not invalidated.
     */
    void setCircle(const int16_t x, const int16_t y, const int16_t r)
    {
        setCircle<int>(x, y, r);
    }

    /**
     * Sets the center of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  x The x coordinate of center.
     * @param  y The y coordinate of center.
     *
     * @see setRadius, setCircle, getCenter
     *
     * @note The area containing the Circle is not invalidated.
     */
    template <typename T>
    void setCenter(const T x, const T y)
    {
        this->circleCenterX = CWRUtil::toQ5(x);
        this->circleCenterY = CWRUtil::toQ5(y);
    }

    /**
     * Sets the center of the Circle.
     *
     * @param  x The x coordinate of center.
     * @param  y The y coordinate of center.
     *
     * @see setRadius, setCircle, getCenter
     *
     * @note The area containing the Circle is not invalidated.
     */
    void setCenter(const int16_t x, const int16_t y)
    {
        setCenter<int>(x, y);
    }

    /**
     * Sets the center of the circle / arc in the middle of a pixel. Normally the coordinate is
     * between pixel number x and x+1 horizontally and between pixel y and y+1 vertically. This
     * function will set the center in the middle of the pixel by adding 0.5 to both x and y.
     *
     * @param  x The x coordinate of the center of the circle.
     * @param  y The y coordinate of the center of the circle.
     */
    void setPixelCenter(int x, int y)
    {
        int32_t half = (int32_t)CWRUtil::toQ5(1) / 2;
        setCenter<CWRUtil::Q5>(CWRUtil::Q5((int32_t)CWRUtil::toQ5(x) + half), CWRUtil::Q5((int32_t)CWRUtil::toQ5(y) + half));
    }

    /**
     * Gets the center coordinates of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] x The x coordinate of the center rounded down to the precision of T.
     * @param [out] y The y coordinate of the center rounded down to the precision of T.
     *
     * @see setCenter
     */
    template <typename T>
    void getCenter(T& x, T& y) const
    {
        x = circleCenterX.to<T>();
        y = circleCenterY.to<T>();
    }

    /**
     * Sets the radius of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  r The radius.
     *
     * @see setCircle, setCenter, getRadius
     *
     * @note The area containing the Circle is not invalidated.
     */
    template <typename T>
    void setRadius(const T r)
    {
        this->circleRadius = CWRUtil::toQ5(r);
    }

    /**
     * Gets the radius of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] r The radius rounded down to the precision of T.
     */
    template <typename T>
    void getRadius(T& r) const
    {
        r = circleRadius.to<T>();
    }

    /**
     * Sets the start and end angles in degrees of the Circle arc. 0 degrees is straight up
     * (12 o'clock) and 90 degrees is to the left (3 o'clock). Any positive or negative
     * degrees can be used to specify the part of the Circle to draw.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  startAngle The start degrees.
     * @param  endAngle   The end degrees.
     *
     * @see getArc, updateArcStart, updateArcEnd, updateArc
     *
     * @note The area containing the Circle is not invalidated.
     */
    template <typename T>
    void setArc(const T startAngle, const T endAngle)
    {
        circleArcAngleStart = CWRUtil::toQ5(startAngle);
        circleArcAngleEnd = CWRUtil::toQ5(endAngle);
    }

    /**
     * Sets the start and end angles in degrees of the Circle arc. 0 degrees is straight up
     * (12 o'clock) and 90 degrees is to the left (3 o'clock). Any positive or negative
     * degrees can be used to specify the part of the Circle to draw.
     *
     * @param  startAngle The start degrees.
     * @param  endAngle   The end degrees.
     *
     * @see getArc, updateArcStart, updateArcEnd, updateArc
     *
     * @note The area containing the Circle is not invalidated.
     */
    void setArc(const int16_t startAngle, const int16_t endAngle)
    {
        setArc<int>(startAngle, endAngle);
    }

    /**
     * Gets the start and end angles in degrees for the circle arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] startAngle The start angle rounded down to the precision of T.
     * @param [out] endAngle   The end angle rounded down to the precision of T.
     *
     * @see setArc
     */
    template <typename T>
    void getArc(T& startAngle, T& endAngle) const
    {
        startAngle = circleArcAngleStart.to<T>();
        endAngle = circleArcAngleEnd.to<T>();
    }

    /**
     * Gets the start angle in degrees for the arc.
     *
     * @return The starting angle for the arc rounded down to an integer.
     *
     * @see getArc, setArc
     */
    int16_t getArcStart() const
    {
        return circleArcAngleStart.to<int>();
    }

    /**
     * Gets the start angle in degrees for the arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] angle The starting angle rounded down to the precision of T.
     *
     * @see getArc, setArc
     */
    template <typename T>
    void getArcStart(T& angle) const
    {
        angle = circleArcAngleStart.to<T>();
    }

    /**
     * Gets the end angle in degrees for the arc.
     *
     * @return The end angle for the arc rounded down to an integer.
     *
     * @see getArc, setArc
     */
    int16_t getArcEnd() const
    {
        return circleArcAngleEnd.to<int>();
    }

    /**
     * Gets the end angle in degrees for the arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] angle The end angle rounded down to the precision of T.
     */
    template <typename T>
    void getArcEnd(T& angle) const
    {
        angle = circleArcAngleEnd.to<T>();
    }

    /**
     * Updates the start angle in degrees for this Circle arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  startAngle The start angle in degrees.
     *
     * @see setArc, updateArcEnd, updateArc
     *
     * @note The area containing the updated Circle arc is invalidated.
     */
    template <typename T>
    void updateArcStart(const T startAngle)
    {
        CWRUtil::Q5 startAngleQ5 = CWRUtil::toQ5(startAngle);
        if (circleArcAngleStart == startAngleQ5)
        {
            return;
        }

        Rect minimalRect = getMinimalRectForUpdatedStartAngle(startAngleQ5);

        circleArcAngleStart = startAngleQ5;

        invalidateRect(minimalRect);
    }

    /**
     * Updates the end angle in degrees for this Circle arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  endAngle The end angle in degrees.
     *
     * @see setArc, updateArcStart, updateArc
     *
     * @note The area containing the updated Circle arc is invalidated.
     */
    template <typename T>
    void updateArcEnd(const T endAngle)
    {
        CWRUtil::Q5 endAngleQ5 = CWRUtil::toQ5(endAngle);
        if (circleArcAngleEnd == endAngleQ5)
        {
            return;
        }

        Rect minimalRect = getMinimalRectForUpdatedEndAngle(endAngleQ5);

        circleArcAngleEnd = endAngleQ5;

        invalidateRect(minimalRect);
    }

    /**
     * Updates the start and end angle in degrees for this Circle arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  startAngle The new start angle in degrees.
     * @param  endAngle   The new end angle in degrees.
     *
     * @see setArc, getArc, updateArcStart, updateArcEnd
     *
     * @note The areas containing the updated Circle arcs are invalidated. As little as possible
     *       will be invalidated for best performance.
     */
    template <typename T>
    void updateArc(const T startAngle, const T endAngle)
    {
        updateArc(CWRUtil::toQ5(startAngle), CWRUtil::toQ5(endAngle));
    }

    /**
     * Sets the line width for this Circle. If the line width is set to zero, the circle
     * will be filled.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  width The width of the line measured in pixels.
     *
     * @note The area containing the Circle is not invalidated.
     * @note if the new line with is smaller than the old width, the circle should be invalidated
     *       before updating the width to ensure that the old circle is completely erased.
     */
    template <typename T>
    void setLineWidth(const T width)
    {
        this->circleLineWidth = CWRUtil::toQ5(width);
    }

    /**
     * Gets line width.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] width The line width rounded down to the precision of T.
     *
     * @see setLineWidth
     */
    template <typename T>
    void getLineWidth(T& width) const
    {
        width = circleLineWidth.to<T>();
    }

    /**
     * Sets precision of the Circle drawing function. The number given as precision is the
     * number of degrees used as step counter when drawing the line fragments around the
     * circumference of the circle, five being a reasonable value. Higher values results in
     * less nice circles but faster rendering and possibly sufficient for very small
     * circles. Large circles might require a precision smaller than five to make the edge
     * of the circle look nice and smooth.
     *
     * @param  precision The precision measured in degrees.
     *
     * @note The circle is not invalidated.
     */
    void setPrecision(const int precision);

    /**
     * Gets the precision of the circle drawing function. The precision is the number of
     * degrees used as step counter when drawing smaller line fragments around the
     * circumference of the circle, the default being 5.
     *
     * @return The precision.
     *
     * @see setPrecision
     */
    int getPrecision() const;

    /**
     * Sets the precision of the ends of the Circle arc. The precision is given in degrees
     * where 180 is the default which results in a square ended arc (aka "butt cap"). 90
     * will draw "an arrow head" and smaller values gives a round cap. Larger values of
     * precision results in faster rendering of the circle.
     *
     * @param  precision The new cap precision.
     *
     * @note The circle is not invalidated.
     * @note The cap precision is not used if the circle is filled (if line width is zero) or when
     *       a full circle is drawn.
     */
    void setCapPrecision(const int precision);

    /**
     * Gets the precision of the ends of the Circle arc.
     *
     * @return The cap precision in degrees.
     *
     * @see getCapPrecision
     */
    int getCapPrecision() const;

    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const;

    virtual Rect getMinimalRect() const;

    /**
     * Gets minimal rectangle containing a given circle arc using the set line width.
     *
     * @param  arcStart The arc start.
     * @param  arcEnd   The arc end.
     *
     * @return The minimal rectangle.
     */
    Rect getMinimalRect(int16_t arcStart, int16_t arcEnd) const;

    /**
     * Gets minimal rectangle containing a given circle arc using the set line width.
     *
     * @param  arcStart The arc start.
     * @param  arcEnd   The arc end.
     *
     * @return The minimal rectangle.
     */
    Rect getMinimalRect(CWRUtil::Q5 arcStart, CWRUtil::Q5 arcEnd) const;

protected:
    /**
     * Updates the start and end angle in degrees for this Circle arc.
     *
     * @param  setStartAngleQ5 The new start angle in degrees.
     * @param  setEndAngleQ5   The new end angle in degrees.
     *
     * @see setArc, getArc, updateArcStart, updateArcEnd
     *
     * @note The areas containing the updated Circle arcs are invalidated. As little as possible
     *       will be invalidated for best performance.
     */
    void updateArc(const CWRUtil::Q5 setStartAngleQ5, const CWRUtil::Q5 setEndAngleQ5);

private:
    CWRUtil::Q5 circleCenterX;
    CWRUtil::Q5 circleCenterY;
    CWRUtil::Q5 circleRadius;
    CWRUtil::Q5 circleArcAngleStart;
    CWRUtil::Q5 circleArcAngleEnd;
    CWRUtil::Q5 circleLineWidth;
    uint8_t circleArcIncrement;
    uint8_t circleCapArcIncrement;

    void moveToAR2(Canvas& canvas, const CWRUtil::Q5& angle, const CWRUtil::Q5& r2) const;
    void lineToAR2(Canvas& canvas, const CWRUtil::Q5& angle, const CWRUtil::Q5& r2) const;
    void lineToXYAR2(Canvas& canvas, const CWRUtil::Q5& x, const CWRUtil::Q5& y, const CWRUtil::Q5& angle, const CWRUtil::Q5& r2) const;
    void updateMinMaxAR(const CWRUtil::Q5& a, const CWRUtil::Q5& r2, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const;
    void updateMinMaxXY(const CWRUtil::Q5& xNew, const CWRUtil::Q5& yNew, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const;
    void calculateMinimalRect(CWRUtil::Q5 arcStart, CWRUtil::Q5 arcEnd, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const;
    Rect getMinimalRectForUpdatedStartAngle(const CWRUtil::Q5& startAngleQ5) const;
    Rect getMinimalRectForUpdatedEndAngle(const CWRUtil::Q5& endAngleQ5) const;
};

} // namespace touchgfx

#endif // TOUCHGFX_CIRCLE_HPP
