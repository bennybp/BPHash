/*! \file
 * \brief Hashing of std::forward_list
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <forward_list>

namespace bphash {

/*! \brief Hashing of std::forward_list */
template<typename T, typename Alloc>
typename std::enable_if<is_hashable<T>::value, void>::type
hash_object( const std::forward_list<T, Alloc> & a, Hasher & h)
{
    detail::hash_container_object(a, h);
}


} // close namespace bphash

