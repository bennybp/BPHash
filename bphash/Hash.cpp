/*! \file
 *
 * \brief A class holding a hash of data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bphash/Hash.hpp"

#include <cstring>


namespace bphash {
 

/*! \brief Return a string representation of the hash
 *
 * The string representation is the usual hex representation,
 * with lower case letters. The length of the string
 * should be 2*bits/8 characters.
 */
std::string Hash::to_string(void) const
{
    // 32-bites = 4 bytes, and 2 characters per byte
    const size_t nbytes = hash_.size() * 4;

    std::string hashstr;

    char buf[3] = { '\0', '\0', '\0' };
    const uint8_t * p = reinterpret_cast<const uint8_t *>(hash_.data());

    for(size_t i = 0; i < nbytes; i++)
    {
        snprintf(buf, 3, "%02x", p[i]); // max size is 2 + null
        hashstr += buf;
    }

    return hashstr;
}


} // close namespace bphash

