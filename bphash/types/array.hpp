/*! \file
 *
 * \brief Hashing of std::array
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <array>

namespace bphash {

/*! \brief Hashing of std::array */
template<typename T, size_t N>
void hash_object( const std::array<T, N> & a, Hasher & h)
{
    h(hash_pointer(a.data(), N));
}


} // close namespace bphash

