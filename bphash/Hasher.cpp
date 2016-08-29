/*! \file
 * \brief A class that hashes objects (source)
 */

#include "bphash/Hasher.hpp"

// Different hashing types
#include "MurmurHash3_128.hpp"
#include "MurmurHash3_64.hpp"
#include "MurmurHash3_32.hpp"

#include <stdexcept>

namespace bphash {


Hasher::Hasher(HashType type)
{
    switch(type)
    {
        case HashType::Hash128:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_128);
            break;
        case HashType::Hash64:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_64);
            break;
        case HashType::Hash32:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_32);
            break;

        default:
            throw std::logic_error("Unhandled HashType");
    }
}


} // close namespace bphash

