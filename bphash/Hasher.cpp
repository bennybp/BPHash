/*! \file
 * \brief A class that hashes objects (source)
 */

#include "bphash/Hasher.hpp"

// Different hashing types
#include "MurmurHash3_128_x64.hpp"
#include "MurmurHash3_64_x64.hpp"
#include "MurmurHash3_32_x64.hpp"
#include "MurmurHash3_32_x32.hpp"

namespace bphash {

Hasher::Hasher(HashType type)
{
    switch(type)
    {
        case HashType::Hash128:
        case HashType::Hash128_x32:
        case HashType::Hash128_x64:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_128_x64);
            break;

        case HashType::Hash64:
        case HashType::Hash64_x32:
        case HashType::Hash64_x64:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_64_x64);
            break;

        case HashType::Hash32:
        case HashType::Hash32_x64:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_32_x64);
            break;

        case HashType::Hash32_x32:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_32_x32);
            break;
    }
}


} // close namespace bphash

