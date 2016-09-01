/*! \file
 * \brief MurmurHash3 64-bit x64 hash (header)
 */

#pragma once

#include "bphash/MurmurHash3_128_x64.hpp"

namespace bphash {
namespace detail {

/*! \brief Implementation of MurmurHash3 64-bit x64 hash
 *
 * This hash calculates the 128-bit using MurmurHash3, and
 * then returns only the first 64-bits. This is claimed to be
 * faster than a pure 64-bit hash algorithm (according to
 * the author).
 */
class MurmurHash3_64_x64 : public MurmurHash3_128_x64
{
    public:
        MurmurHash3_64_x64(void) = default;
        ~MurmurHash3_64_x64(void) = default;

        MurmurHash3_64_x64(const MurmurHash3_64_x64 &) = default;
        MurmurHash3_64_x64 & operator=(const MurmurHash3_64_x64 &) = default;
        MurmurHash3_64_x64(MurmurHash3_64_x64 &&) = default;
        MurmurHash3_64_x64 & operator=(MurmurHash3_64_x64 &&) = default;

        /////////////////////////////////
        // Virtual functions of HashImpl
        /////////////////////////////////

        virtual HashValue finalize(void);
};


} // close namespace detail
} // close namespace bphash

