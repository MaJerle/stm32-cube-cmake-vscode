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
 * @file mvp/Presenter.hpp
 *
 * Declares the touchgfx::Presenter class.
 */
#ifndef TOUCHGFX_PRESENTER_HPP
#define TOUCHGFX_PRESENTER_HPP

namespace touchgfx
{
/**
 * The Presenter base class that all application-specific presenters should derive from. Only
 * contains activate and deactivate virtual functions which are called automatically
 * during screen transition.
 */
class Presenter
{
public:
    /**
     * Place initialization code for the Presenter here.
     *
     * The activate function is called automatically when a screen transition causes this
     * Presenter to become active. Place initialization code for the Presenter here.
     */
    virtual void activate()
    {
    }

    /**
     * Place cleanup code for the Presenter here.
     *
     * The deactivate function is called automatically when a screen transition causes this
     * Presenter to become inactive. Place cleanup code for the Presenter here.
     */
    virtual void deactivate()
    {
    }

    /** Finalizes an instance of the Presenter class. */
    virtual ~Presenter()
    {
    }

protected:
    /** Initializes a new instance of the Presenter class. */
    Presenter()
    {
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_PRESENTER_HPP
