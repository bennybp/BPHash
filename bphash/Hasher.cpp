/*! \file
 *
 * \brief A class that hashes objects (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bphash/Hasher.hpp"

// Different hashing types
#include "MurmurHash3.hpp"

#include <stdexcept>

namespace bphash {


Hasher::Hasher(HashType type)
{
    switch(type)
    {
        case HashType::Hash128:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3);
            break;

        default:
            throw std::logic_error("Unhandled HashType");
    }
}


} // close namespace bphash

