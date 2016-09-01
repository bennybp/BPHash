/*! \file
 * \brief A class that can be used in place of std::hash
 */

#pragma once

#include "bphash/Hasher.hpp"

namespace bphash {

/*! \brief A class that can be used in place of std::hash in containers
 *
 * This is useful for `unordered_map`, etc, that require hashing of the key
 * type.
 */
template<typename T>
struct StdHash
{
    size_t operator()(const T & obj) const
    {
        auto h = make_hash(HashType::Hash64, obj);
        return convert_hash<size_t>(h);
    }
};

} // close namespace bphash

