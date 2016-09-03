/*! \file
 * \brief MurmurHash3 128-bit x64 hash (header)
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

/*! \brief Implementation of MurmurHash3 128-bit x64 hash
 *
 * This algorithm is the 128-bit MurmurHash3 hash, 
 * by Austin Appleby.
 *
 *
 * The code here is adapted from the smhasher project at
 * https://github.com/aappleby/smhasher. It has been placed in
 * the public domain by the author.
 *
 * No care has been taken to work with different endianness, etc,
 * since that is beyond the scope of the project.
 */
class MurmurHash3_128_x64 : public HashImpl
{
    private:
        static const uint64_t c1 = (0x87c37b91114253d5LLU);
        static const uint64_t c2 = (0x4cf5ad432745937fLLU);

        uint64_t h1_; //!< First part of the 128-bit hash
        uint64_t h2_; //!< Second part of the 128-bit hash

        std::array<uint8_t, 16> buffer_;   //!< Holds any tail/remainder

        size_t len_;      //!< Total amount already hashed
        size_t nbuffer_;  //!< Number of elements in the buffer


        /*! \brief Hash 16-byte blocks of data
         *
         * This does NOT handle any tail/remainder, so the buffer must contain
         * at least 16 bytes
         */
        void update_block_(uint8_t const * data, size_t nblocks);


    public:
        MurmurHash3_128_x64(void);
        ~MurmurHash3_128_x64(void) = default;

        MurmurHash3_128_x64(const MurmurHash3_128_x64 &) = default;
        MurmurHash3_128_x64 & operator=(const MurmurHash3_128_x64 &) = default;
        MurmurHash3_128_x64(MurmurHash3_128_x64 &&) = default;
        MurmurHash3_128_x64 & operator=(MurmurHash3_128_x64 &&) = default;

        /////////////////////////////////
        // Virtual functions of HashImpl
        /////////////////////////////////

        virtual void update(void const * data, size_t nbytes);

        virtual HashValue finalize(void);

        virtual void reset(void);
};


} // close namespace detail
} // close namespace bphash

