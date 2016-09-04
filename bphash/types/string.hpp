/*! \file
 * \brief Hashing of std::string
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

//#include <string> // included via ContainerHelper
#include "bphash/types/ContainerHelper.hpp"

namespace bphash {

/*! \brief Hashing of std::string */
template<typename charT, typename Traits, typename Alloc>
typename std::enable_if<is_hashable<charT>::value, void>::type
hash_object( const std::basic_string<charT, Traits, Alloc> & s, Hasher & h)
{
    detail::hash_container_object(s, h);
}


} // close namespace bphash

