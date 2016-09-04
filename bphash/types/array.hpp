/*! \file
 * \brief Hashing of std::array
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <array>

namespace bphash {

/*! \brief Hashing of std::array */
template<typename T, size_t N>
typename std::enable_if<is_hashable<T>::value, void>::type
hash_object( const std::array<T, N> & a, Hasher & h)
{
    h(hash_pointer(a.data(), N));
}


} // close namespace bphash

