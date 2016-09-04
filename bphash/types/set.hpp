/*! \file
 * \brief Hashing of std::set
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <set>

namespace bphash {

/*! \brief Hashing of std::set */
template<typename Key, typename Compare, typename Alloc>
typename std::enable_if<is_hashable<Key>::value, void>::type
hash_object( const std::set<Key, Compare, Alloc> & s, Hasher & h)
{
    detail::hash_container_object(s, h);
}


} // close namespace bphash

