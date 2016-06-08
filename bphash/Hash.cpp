/*! \file
 *
 * \brief A class holding a hash of data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bphash/Hash.hpp"
#include <cstring>


namespace bphash {


std::string hash_to_string(const HashValue & hash)
{
    std::string hashstr;

    char buf[3] = { '\0', '\0', '\0' };

    for(uint8_t v : hash)
    {
        snprintf(buf, 3, "%02x", v); // max size is 2 + null
        hashstr += buf;
    }

    return hashstr;
}


HashValue truncate_hash(const HashValue & hash, size_t nbytes)
{
    /* Copy the hash, and then resize it.
       This automatically handles cases where the new size is larger
       or smaller. The second parameter to resize() is zero so
       that if nbytes is larger than the stored hash size, newhash
       will be padded with zeros.
    */
    HashValue newhash(hash);
    newhash.resize(nbytes, 0);
    return newhash;
}


} // close namespace bphash

