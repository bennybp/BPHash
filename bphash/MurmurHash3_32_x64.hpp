/*! \file
 * \brief MurmurHash3_32_x64 hash implementation (header)
 */

#pragma once

#include "bphash/MurmurHash3_128_x64.hpp"

namespace bphash {
namespace detail {

/*! \brief Implementation of MurmurHash3 64-bit hash
 *
 * This hash calculates the 128-bit hash and then returns only
 * the first 32-bits. This is found to be faster than running the
 * 32-bit x32 hash on a 64-bit machine.
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
class MurmurHash3_32_x64 : public MurmurHash3_128_x64
{
    public:
        MurmurHash3_32_x64(void) = default;
        ~MurmurHash3_32_x64(void) = default;

        MurmurHash3_32_x64(const MurmurHash3_32_x64 &) = default;
        MurmurHash3_32_x64 & operator=(const MurmurHash3_32_x64 &) = default;
        MurmurHash3_32_x64(MurmurHash3_32_x64 &&) = default;
        MurmurHash3_32_x64 & operator=(MurmurHash3_32_x64 &&) = default;

        /////////////////////////////////
        // Virtual functions of HashImpl
        /////////////////////////////////

        virtual HashValue finalize(void);
};


} // close namespace detail
} // close namespace bphash
