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
        /*! \brief Construct given 32-bit components */
        Hash(const std::vector<uint32_t> & h)
            : hash_(h) { }

        Hash(std::vector<uint32_t> && h)
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


        /*! \brief Truncate the hash to a given type
         * 
         * Useful if using for a specialization of std::hash
         */
        template<typename T>
        T truncate(void) const
        {
            static_assert(std::is_fundamental<T>::value,
                          "Given type must be a fundamental type");

            const size_t wantedbytes = sizeof(T);
            const size_t nbytes = hash_.size() * 4;
            if(nbytes >= wantedbytes)
            {
                const T * retptr = reinterpret_cast<const T *>(hash_.data());
                return *retptr;
            }
            else
            {
                return 0; //! \todo
            }
        }


        /*! \brief Return a string representation of the hash
         *
         * The string representation is the usual hex representation,
         * with lower case letters. The length of the string
         * should be 2*bits/8 characters.
         */
        std::string to_string(void) const;


    private:
        //!< The hash as a number
        std::vector<uint32_t> hash_;

};



} // close namespace bphash


#endif

