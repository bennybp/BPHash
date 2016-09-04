/*! \file
 * \brief Hashing of std::unordered_set
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <unordered_set>

namespace bphash {

/*! \brief Hashing of std::unordered_set */
template<typename Key, typename HashT, typename Pred, typename Alloc>
typename std::enable_if<is_hashable<Key>::value, void>::type
hash_object( const std::unordered_set<Key, HashT, Pred, Alloc> & s, Hasher & h)
{
    detail::hash_container_object(s, h);
}


} // close namespace bphash

