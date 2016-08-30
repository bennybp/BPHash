/*! \file
 *
 * \brief Hashing of std::set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <set>

namespace bphash {

/*! \brief Hashing of std::set */
template<typename Key, typename Compare, typename Alloc>
void hash_object( const std::set<Key, Compare, Alloc> & s, Hasher & h)
{
    detail::hash_container_object(s, h);
}


} // close namespace bphash

