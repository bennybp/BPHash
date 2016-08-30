/*! \file
 * \brief MurmurHash3_128_x64 hash implementation (header)
 */

#pragma once

#include <cstdint>
#include <array>

#include "bphash/HashImpl.hpp"

namespace bphash {
namespace detail {

/*! \brief Implementation of MurmurHash3 128-bit hash
 * The hash algorithm implemented here is 128-bit MurmurHash3,
 * by Austin Appleby. It has been placed in the public domain
 * by the author.
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
class MurmurHash3_128_x64 : public HashImpl
{
    private:
        static const uint64_t c1 = (0x87c37b91114253d5LLU);
        static const uint64_t c2 = (0x4cf5ad432745937fLLU);

        uint64_t h1_; //!< First part of the 128-bit hash
        uint64_t h2_; //!< Second part of the 128-bit hash

        std::array<uint8_t, 16> buffer_;   //!< Buffer for updating blocks, and any left over

        size_t len_;     //!< Total amount already hashed
        int nbuffer_;    //!< Number of elements in the buffer

        /*! \brief Hash 16 bytes of data in buffer_
         *
         * This does NOT handle any tail/remainder, so buffer_
         * must be full
         */
        void update_block_(void);

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

        virtual void update(void const * buffer, size_t nbytes);

        virtual HashValue finalize(void);

        virtual void reset(void);
};


} // close namespace detail
} // close namespace bphash
