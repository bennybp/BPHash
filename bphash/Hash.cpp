/*! \file
 *
 * \brief A class holding a hash of data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bphash/Hash.hpp"

#include <cstring>


namespace bphash {
 

std::string Hash::to_string(void) const
{
    std::string hashstr;

    char buf[3] = { '\0', '\0', '\0' };

    for(uint8_t v : hash_)
    {
        snprintf(buf, 3, "%02x", v); // max size is 2 + null
        hashstr += buf;
    }

    return hashstr;
}


Hash Hash::truncate(size_t nbytes) const
{
    /* Copy the hash, and then resize it.
       This automatically handles cases where the new size is larger
       or smaller. The second parameter to resize() is zero so
       that if nbytes is larger than the stored hash size, newhash
       will be padded with zeros.
    */
    std::vector<uint8_t> newhash(hash_);
    newhash.resize(nbytes, 0);
    return Hash(std::move(newhash));
}


} // close namespace bphash

