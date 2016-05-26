/*! \file
 *
 * \brief A class holding a hash of data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_BPHASH__HASH_HPP_
#define PULSAR_GUARD_BPHASH__HASH_HPP_

#include <cstdint>
#include <array>
#include <string>

namespace bphash {
 

// forward declare for friend
class HashImpl;


/*! \brief A hash of some data
 *
 * The length is left unspecified to the end user, but
 * is currently 128-bit.
 *
 * A hash can only be constructed from within a HashImpl object
 */
class Hash
{
    public:
        /// Construct as a hash of zeros
        Hash() noexcept : Hash(0, 0) { };

        Hash(const Hash &) noexcept             = default;
        Hash & operator=(const Hash &) noexcept = default;
        Hash(Hash &&) noexcept                  = default;
        Hash & operator=(Hash &&) noexcept      = default;

        bool operator==(const Hash & rhs) const noexcept { return hash_ == rhs.hash_; } 
        bool operator!=(const Hash & rhs) const noexcept { return hash_ != rhs.hash_; } 
        bool operator>(const Hash & rhs)  const noexcept { return hash_ >  rhs.hash_; } 
        bool operator>=(const Hash & rhs) const noexcept { return hash_ >= rhs.hash_; } 
        bool operator<(const Hash & rhs)  const noexcept { return hash_ <  rhs.hash_; } 
        bool operator<=(const Hash & rhs) const noexcept { return hash_ <= rhs.hash_; } 


        /*! \brief Truncate the hash to a size_t
         * 
         * Useful if using for a specialization of std::hash
         */
        size_t truncate(void) const noexcept { return hash_[0]; }

        /*! \brief Return a string representation of the hash
         *
         * The string representation is the usual hex representation,
         * with lower case letters. The length of the string
         * should be 2*bits/8 characters.
         */
        std::string to_string(void) const
        {
            char buf[2*(128/8) + 1]; // 2 chars per byte + null

            const uint8_t * p = reinterpret_cast<const uint8_t *>(hash_.data());

            for(size_t i = 0; i < 128/8; i++)
                snprintf(buf + 2*i, 3, "%02x", p[i]); // max size is 2 + null

            return std::string(buf);
        }


    private:
        friend class HashImpl;

        //!< The hash as a number
        std::array<uint64_t, 2> hash_;

        /*! \brief Construct given the lower and upper 64-bit parts */
        Hash(uint64_t h1, uint64_t h2) noexcept : hash_{h1, h2} { }
};



} // close namespace bphash


#endif

