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
 * @file mvp/View.hpp
 *
 * Declares the touchgfx::View class.
 */
#ifndef TOUCHGFX_VIEW_HPP
#define TOUCHGFX_VIEW_HPP

#include <touchgfx/Screen.hpp>

namespace touchgfx
{
/**
 * This is a generic touchgfx::Screen specialization for normal applications. It provides a link
 * to the Presenter class.
 *
 * @tparam T The type of Presenter associated with this view.
 *
 * @see Screen
 *
 * @note All views in the application must be a subclass of this type.
 */
template <class T>
class View : public Screen
{
public:
    View()
        : presenter(0)
    {
    }

    /**
     * Binds an instance of a specific Presenter type (subclass) to the View instance. This
     * function is called automatically when a new presenter/view pair is activated.
     *
     * @param [in] presenter The specific Presenter to be associated with the View.
     */
    void bind(T& presenter)
    {
        this->presenter = &presenter;
    }

protected:
    T* presenter; ///< Pointer to the Presenter associated with this view.
};

} // namespace touchgfx

#endif // TOUCHGFX_VIEW_HPP
