/*! \file
 * \brief MurmurHash3_32_x32 hash implementation (header)
 */

#pragma once

#include <cstdint>
#include <array>

#include "bphash/HashImpl.hpp"

namespace bphash {
namespace detail {

/*! \brief Implementation of MurmurHash3 32-bit hash
 * The hash algorithm implemented here is 32-bit MurmurHash3,
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
class MurmurHash3_32_x32 : public HashImpl
{
    private:
        static const uint32_t c1 = 0xcc9e2d51;
        static const uint32_t c2 = 0x1b873593;

        uint32_t h_;  //!< The 32-bit hash

        std::array<uint8_t, 4> buffer_;   //!< Buffer for updating blocks, and any left over

        int nbuffer_;    //!< Number of elements in the buffer
        size_t len_;     //!< Total amount already hashed


        /*! \brief Hash 4 bytes of data in buffer_
         *
         * This does NOT handle any tail/remainder, so buffer_
         * must be full
         */
        void update_block_(void);

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

        virtual void update(void const * buffer, size_t nbytes);

        virtual void pad_out(void);

        virtual HashValue finalize(void);

        virtual void reset(void);
};


} // close namespace detail
} // close namespace bphash
