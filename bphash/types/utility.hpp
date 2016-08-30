/*! \file
 *
 * \brief Hashing of std::pair
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <utility>

namespace bphash {

template<typename T1, typename T2>
typename std::enable_if<is_hashable<T1, T2>::value, void>::type
hash_object( const std::pair<T1, T2> & p, Hasher & h)
{
    h(p.first, p.second);
}


} // close namespace bphash

