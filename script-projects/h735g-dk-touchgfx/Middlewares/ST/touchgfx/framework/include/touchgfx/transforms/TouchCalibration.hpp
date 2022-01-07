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
 * @file touchgfx/transforms/TouchCalibration.hpp
 *
 * Declares the touchgfx::TouchCalibration class.
 */
#ifndef TOUCHGFX_TOUCHCALIBRATION_HPP
#define TOUCHGFX_TOUCHCALIBRATION_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * Calibrates a touch coordinate.
 *
 * Class TouchCalibraiton is responsible for translating coordinates (Point) based on
 * matrix of calibration values.
 */
class TouchCalibration
{
public:
    TouchCalibration()
    {
        matrix.Divider = 0;
    }

    /**
     * Initializes the calibration matrix based on reference and measured values.
     *
     * @param  ref Pointer to array of three reference points.
     * @param  scr Pointer to array of three measured points.
     */
    static void setCalibrationMatrix(const Point* ref, const Point* scr);

    /**
     * Translates the specified point using the matrix. If matrix has not been initialized,
     * p is not modified.
     *
     * @param [in,out] p The point to translate.
     */
    static void translatePoint(Point& p);

private:
    static int32_t muldiv(int32_t factor1, int32_t clzu1, int32_t factor2, int32_t divisor, int32_t& remainder);
    static uint32_t muldivu(const uint32_t factor1, const uint32_t clzu1, const uint32_t factor2, const uint32_t clzu2, const uint32_t divisor, uint32_t& remainder);

    static int32_t clzu(uint32_t x);

    /**
     * A matrix. See http://www.embedded.com/design/system-integration/4023968/How-To-Calibrate-Touch-Screens
     * for calibration technique by Carlos E. Vidales.
     */
    struct Matrix
    {
        int32_t An, Bn, Cn, Dn, En, Fn, Divider;
        int32_t clzuAn, clzuBn, clzuDn, clzuEn;
    };

    static Matrix matrix;
};

} // namespace touchgfx

#endif // TOUCHGFX_TOUCHCALIBRATION_HPP
