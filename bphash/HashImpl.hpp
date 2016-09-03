/*! \file
 * \brief Base class for a hash implementation
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hash.hpp"

namespace bphash {
namespace detail {

/*! \brief Base class for a hash implementation
 *
 * Implementations that actually hash data must derive from this
 * class.
 */
class HashImpl
{
    public:

        /*! \brief Add some data to the hash
         *
         * \param [in] data The raw data to hash
         * \param [in] nbytes Number of bytes pointed to by data
         */
        virtual void update(void const * data, size_t nbytes) = 0;


        /*! \brief Finish hashing and report the hash
         *
         * Finish hashing any remaining data if necessary, and perform
         * and last steps. Then, return the hash.
         *
         * \return The computed hash of all the data that had been added
         */
        virtual HashValue finalize(void) = 0;


        /*! \brief Zero out the hash
         *
         * After this, you can start hashing something else again. May be called
         * before or after finalize.
         */
        virtual void reset(void) = 0;


        virtual ~HashImpl() = default;
};


} // close namespace detail
} // close namespace bphash

