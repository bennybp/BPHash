/*! \file
 * \brief MurmurHash3 32-bit x64 hash (header)
 */

#pragma once

#include "bphash/MurmurHash3_128_x64.hpp"

namespace bphash {
namespace detail {

/*! \brief Implementation of MurmurHash3 32-bit x64 hash
 *
 * This hash calculates the 128-bit using MurmurHash3, and
 * then returns only the first 32-bits. This is found to be
 * faster than running the 32-bit x32 hash on a 64-bit machine.
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

