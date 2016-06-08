/*! \file
 *
 * \brief A class for hashing data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_BPHASH__HASHIMPL_HPP_
#define PULSAR_GUARD_BPHASH__HASHIMPL_HPP_

#include "bphash/Hash.hpp"

#include <cstdlib>

namespace bphash{
namespace detail {


class HashImpl
{
    public:
        /*! \brief Add some data to the hash
         * 
         * Any remaining from the previous call will be done, and
         * any that doesn't fill out a whole block will be stored
         * until next time
         */
        virtual void update(void const * buffer, size_t size) = 0;

        /*! \brief Finish hashing and report the hash
         *
         * Any remaining data (that doesn't fill out a block) will be done.
         * Then, any finalization steps will be done and the hash returned. 
         */
        virtual HashValue finalize(void) = 0;

        /*! \brief Zero out the hash
         * 
         * After this, you can start hashing something else again
         */
        virtual void reset(void) = 0;
};


} // close namespace detail
} // close namespace bphash

#endif
