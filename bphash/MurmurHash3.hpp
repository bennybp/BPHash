/*! \file
 *
 * \brief MurmurHash3 hash implementation (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPHASH_GUARD_MURMURHASH3_HPP_
#define BPHASH_GUARD_MURMURHASH3_HPP_

#include <cstdint>
#include <array>

#include "bphash/HashImpl.hpp"

namespace bphash {
namespace detail {

/*! \brief Implementation of MurmurHash3 */
class MurmurHash3 : public HashImpl
{
    private:
        static const uint64_t c1 = (0x87c37b91114253d5LLU);
        static const uint64_t c2 = (0x4cf5ad432745937fLLU);

        uint64_t h1; //!< First part of the 128-bit hash
        uint64_t h2; //!< Second part of the 128-bit hash

        std::array<uint8_t, 16> buffer_;   //!< Buffer for updating blocks, and any left over

        unsigned short nbuffer_;           //!< Number of elements in the buffer
        size_t len_;                       //!< Total amount already hashed

        /*! \brief Hash 16 bytes of data in buffer_
         *
         * This does NOT handle any tail/remainder, so buffer_
         * must be full
         */
        void update_block_(void);

    public:
        MurmurHash3(void);
        ~MurmurHash3(void) = default;

        MurmurHash3(const MurmurHash3 &) = default;
        MurmurHash3 & operator=(const MurmurHash3 &) = default;
        MurmurHash3(MurmurHash3 &&) = default;
        MurmurHash3 & operator=(MurmurHash3 &&) = default;

        virtual void update(void const * buffer, size_t size);

        virtual HashValue finalize(void);

        virtual void reset(void);
};


} // close namespace detail
} // close namespace bphash

#endif
