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
 * @file common/Partition.hpp
 *
 * Declares the touchgfx::Partition class.
 */
#ifndef TOUCHGFX_PARTITION_HPP
#define TOUCHGFX_PARTITION_HPP

#include <touchgfx/hal/Types.hpp>
#include <common/AbstractPartition.hpp>
#include <common/Meta.hpp>

namespace touchgfx
{
/**
 * This type provides a concrete Partition of memory-slots capable of holding any of the
 * specified list of types.
 *
 * The Partition is not aware of the types stored in the Partition memory, hence it
 * provides no mechanism for deleting C++ objects when the Partition is clear()'ed.
 *
 * This class implements AbstractPartition.
 *
 * @tparam ListOfTypes        Type of the list of types.
 * @tparam NUMBER_OF_ELEMENTS Type of the number of elements.
 *
 * @see AbstractPartition
 */
template <typename ListOfTypes, uint16_t NUMBER_OF_ELEMENTS>
class Partition : public AbstractPartition
{
public:
    /** Provides a generic public type containing the list of supported types. */
    typedef ListOfTypes SupportedTypesList;

    /**
     * Compile-time generated constants specifying the "element" or "slot" size used by this
     * partition.
     */
    enum
    {
        INTS_PR_ELEMENT = (sizeof(typename meta::select_type_maxsize<SupportedTypesList>::type) + sizeof(int) - 1) / sizeof(int),
        SIZE_OF_ELEMENT = INTS_PR_ELEMENT * sizeof(int)
    };

    virtual uint16_t capacity() const
    {
        return NUMBER_OF_ELEMENTS;
    }

    virtual uint32_t element_size()
    {
        return sizeof(stBlocks[0]);
    }

protected:
    virtual void* element(uint16_t index)
    {
        return &stBlocks[index];
    }

    virtual const void* element(uint16_t index) const
    {
        return &stBlocks[index];
    }

private:
    /** Internal type used for storage, in order to ensure "natural" alignment of elements. */
    struct Block
    {
        int filler[INTS_PR_ELEMENT];
    };

    Block stBlocks[NUMBER_OF_ELEMENTS]; ///< Actual memory storage
};

} // namespace touchgfx

#endif // TOUCHGFX_PARTITION_HPP
