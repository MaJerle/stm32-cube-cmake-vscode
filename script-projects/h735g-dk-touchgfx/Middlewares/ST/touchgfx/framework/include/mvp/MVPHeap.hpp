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
 * @file mvp/MVPHeap.hpp
 *
 * Declares the touchgfx::MVPHeap class.
 */
#ifndef TOUCHGFX_MVPHEAP_HPP
#define TOUCHGFX_MVPHEAP_HPP

#include "common/AbstractPartition.hpp"

namespace touchgfx
{
class AbstractPartition;
class MVPApplication;

/**
 * Generic heap class for MVP applications. Serves as a way of obtaining the memory storage
 * areas for presenters, screens, transitions and the concrete application.
 *
 * Subclassed by an application-specific heap which provides the actual storage areas.
 * This generic interface is used only in makeTransition.
 */
class MVPHeap
{
public:
    /**
     * Initializes a new instance of the MVPHeap class.
     *
     * @param [in] pres A memory partition containing enough memory to hold the largest
     *                  presenter.
     * @param [in] scr  A memory partition containing enough memory to hold the largest view.
     * @param [in] tra  A memory partition containing enough memory to hold the largest
     *                  transition.
     * @param [in] app  A reference to the MVPApplication instance.
     */
    MVPHeap(AbstractPartition& pres,
            AbstractPartition& scr,
            AbstractPartition& tra,
            MVPApplication& app)
        : presenterStorage(pres),
          screenStorage(scr),
          transitionStorage(tra),
          frontendApplication(app)
    {
    }

    /** Finalizes an instance of the MVPHeap class. */
    virtual ~MVPHeap()
    {
    }

    AbstractPartition& presenterStorage;  ///< A memory partition containing enough memory to hold the largest presenter.
    AbstractPartition& screenStorage;     ///< A memory partition containing enough memory to hold the largest view.
    AbstractPartition& transitionStorage; ///< A memory partition containing enough memory to hold the largest transition.
    MVPApplication& frontendApplication;  ///< A reference to the MVPApplication instance.
};

} // namespace touchgfx

#endif // TOUCHGFX_MVPHEAP_HPP
