/*! \file
 *
 * \brief Hashing of std::vector
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <vector>

namespace bphash {

/*! \brief Hashing of std::vector */
template<typename T, typename Alloc>
void hash_object( const std::vector<T, Alloc> & v, Hasher & h)
{
    h(hash_pointer(v.data(), v.size()));
}


} // close namespace bphash

