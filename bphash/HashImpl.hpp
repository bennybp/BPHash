/*! \file
 *
 * \brief A class for hashing data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_BPHASH__HASHIMPL_HPP_
#define PULSAR_GUARD_BPHASH__HASHIMPL_HPP_

#include <cstdint>
#include <memory>
#include <istream>

namespace bphash{


// forward declare
class Hash;


/*! \brief A class that hashes arbitrary data
 *
 * This class can be used to progressively hash
 * arbitrary data (via the update function). The
 * results are obtained via the finalize function,
 * which returns a Hash object.
 *
 * Currently, the hash algorithm generates 128-bit
 * hashes.
 */
class HashImpl
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
         * This does NOT handle any tail/remainder
         */
        void update_block_(void);

    public:
        HashImpl(void);
        ~HashImpl(void) = default;

        HashImpl(const HashImpl &) = default;
        HashImpl & operator=(const HashImpl &) = default;
        HashImpl(HashImpl &&) = default;
        HashImpl & operator=(HashImpl &&) = default;

        /*! \brief Add some data to the hash
         * 
         * Any remaining from the previous call will be done, and
         * any that doesn't fill out a whole block will be stored
         * until next time
         */
        void update(void const * buffer, size_t size);

        /*! \brief Finish hashing and report the hash
         *
         * Any remaining data (that doesn't fill out a block) will be done.
         * Then, any finalization steps will be done and the hash returned. 
         */
        Hash finalize(void);

        /*! \brief Zero out the hash
         * 
         * After this, you can start hashing something else again
         */
        void reset(void);
};



} // close namespace bphash

#endif
