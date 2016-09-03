/*! \file
 * \brief MurmurHash3 32-bit x32 hash (header)
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include <array>

#include "bphash/HashImpl.hpp"

namespace bphash {
namespace detail {

/*! \brief Implementation of MurmurHash3 32-bit x32 hash
 *
 * This algorithm is the 32-bit MurmurHash3 hash,
 * by Austin Appleby. It is optimized for 32-bit
 * architectures.
 *
 * The code here is adapted from the smhasher project at
 * https://github.com/aappleby/smhasher. It has been placed in
 * the public domain by the author.
 *
 * No care has been taken to work with different endianness, etc,
 * since that is beyond the scope of the project.
 */
class MurmurHash3_32_x32 : public HashImpl
{
    private:
        static const uint32_t c1 = 0xcc9e2d51;
        static const uint32_t c2 = 0x1b873593;

        uint32_t h_;  //!< The 32-bit hash

        std::array<uint8_t, 4> buffer_;   //!< Holds any tail/remainder

        size_t nbuffer_;  //!< Number of elements in the buffer
        size_t len_;      //!< Total amount already hashed


        /*! \brief Hash 4 bytes of data in buffer_
         *
         * This does NOT handle any tail/remainder, so the buffer must contain
         * at least 4 bytes
         */
        void update_block_(uint8_t const * data, size_t nblocks);

    public:
        MurmurHash3_32_x32(void);
        ~MurmurHash3_32_x32(void) = default;

        MurmurHash3_32_x32(const MurmurHash3_32_x32 &) = default;
        MurmurHash3_32_x32 & operator=(const MurmurHash3_32_x32 &) = default;
        MurmurHash3_32_x32(MurmurHash3_32_x32 &&) = default;
        MurmurHash3_32_x32 & operator=(MurmurHash3_32_x32 &&) = default;

        /////////////////////////////////
        // Virtual functions of HashImpl
        /////////////////////////////////

        virtual void update(void const * data, size_t nbytes);

        virtual HashValue finalize(void);

        virtual void reset(void);
};


} // close namespace detail
} // close namespace bphash

