/*! \file
 * \brief Base class for a hash implementation
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
         * Any remaining from the previous call will be done, and
         * any that doesn't fill out a whole block will be stored
         * until next time
         *
         * \param [in] buffer The raw data to hash
         * \param [in] nbytes Number of bytes in the buffer
         */
        virtual void update(void const * buffer, size_t nbytes) = 0;


        /*! \brief Pad out the hashing data
         *
         * After this, the next call to update should start at an appropriate
         * boundary
         */
        virtual void pad_out(void) = 0;


        /*! \brief Finish hashing and report the hash
         *
         * Any remaining data (that doesn't fill out a block) will be hashed.
         * Then, any finalization steps will be done and the hash returned.
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

