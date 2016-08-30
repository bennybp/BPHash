/*! \file
 * \brief MurmurHash3_64_x64 hash implementation (header)
 */

#pragma once

#include "bphash/MurmurHash3_128_x64.hpp"

namespace bphash {
namespace detail {

/*! \brief Implementation of MurmurHash3 64-bit hash
 *
 * This hash calculates the 128-bit hash and then returns only
 * the first 64-bits. This is claimed to be faster than a pure
 * 64-bit hash (according to the author).
 *
 * The code here is adapted from the smhasher project at
 * https://github.com/aappleby/smhasher. Mostly, it has
 * been converted to a progressive version.
 *
 * No care has been taken to work with different endianness, etc,
 * since that is pretty much beyond the scope of the project.
 *
 * This is not a cryptographic hash, so if you are using it as
 * one, you are very, very wrong.
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
