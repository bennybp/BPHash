/*! \file
 * \brief Hashing of std::pair
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <utility>

namespace bphash {

/*! \brief Hashing of std::pair */
template<typename T1, typename T2>
typename std::enable_if<is_hashable<T1, T2>::value, void>::type
hash_object( const std::pair<T1, T2> & p, Hasher & h)
{
    h(p.first, p.second);
}


} // close namespace bphash

