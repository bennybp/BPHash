/*! \file
 *
 * \brief A class holding a hash of data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_BPHASH__HASH_HPP_
#define PULSAR_GUARD_BPHASH__HASH_HPP_

#include <cstdint>
#include <vector>
#include <string>

namespace bphash {
 

/*! \brief The hash of some data
 *
 * This is the main result of hashing.
 *
 * This class also allows for comparison between different
 * hashes.
 */
class Hash
{
    public:
        /*! \brief Constructor
         *
         * \param [in] h 32-bit components of the hash  
         */
        Hash(const std::vector<uint8_t> & h)
            : hash_(h) { }


        /*! \brief Constructor
         *
         * \param [in] h 32-bit components of the hash  
         */
        Hash(std::vector<uint8_t> && h)
            : hash_(std::move(h)) { }


        Hash() = default;
        Hash(const Hash &)              = default;
        Hash & operator=(const Hash &)  = default;
        Hash(Hash &&)                   = default;
        Hash & operator=(Hash &&)       = default;

        bool operator==(const Hash & rhs) const { return hash_ == rhs.hash_; } 
        bool operator!=(const Hash & rhs) const { return hash_ != rhs.hash_; } 
        bool operator>(const Hash & rhs)  const { return hash_ >  rhs.hash_; } 
        bool operator>=(const Hash & rhs) const { return hash_ >= rhs.hash_; } 
        bool operator<(const Hash & rhs)  const { return hash_ <  rhs.hash_; } 
        bool operator<=(const Hash & rhs) const { return hash_ <= rhs.hash_; } 


        /*! \brief Truncate the hash to a given size
         * 
         * If the desired size is larger than the size of the stored hash,
         * it is padded with zero.
         *
         * \param [in] nbytes Desired size of the new hash (in bytes)
         */
        Hash truncate(size_t nbytes) const;


        /*! \brief Convert the hash to a given type
         * 
         * If the type is larger than the stored hash, it is padded
         * with zeros.
         *
         * \tparam The type to truncate to. Must be an arithmetic type.
         */
        template<typename T>
        T convert(void) const
        {
            static_assert(std::is_arithmetic<T>::value,
                          "Type to truncate to must be an arithmetic type");

            const size_t wantedbytes = sizeof(T);
            Hash tmphash = truncate(wantedbytes); // will pad with zero

            T ret = 0;
            for(size_t i = 0; i < wantedbytes; i++)
                ret |= static_cast<T>(tmphash.hash_[i]) << (i*8);

            return ret;
        }


        /*! \brief Return a string representation of the hash
         *
         * The string representation is the usual hex representation,
         * with lower case letters. The length of the string
         * should be 2*number_of_bits/8 characters.
         */
        std::string to_string(void) const;


    private:
        //!< The hash as a series of 8-bit components
        std::vector<uint8_t> hash_;
};



} // close namespace bphash


#endif

