/*! \file
 * \brief Hashing of std::map
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include "bphash/types/utility.hpp"
#include <map>

namespace bphash {

/*! \brief Hashing of std::map */
template<typename Key, typename T, typename Compare, typename Alloc>
typename std::enable_if<is_hashable<Key, T>::value, void>::type
hash_object( const std::map<Key, T, Compare, Alloc> & m, Hasher & h)
{
    detail::hash_container_object(m, h);
}


} // close namespace bphash

