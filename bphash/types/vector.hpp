/*! \file
 * \brief Hashing of std::vector
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
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

