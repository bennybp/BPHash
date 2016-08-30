/*! \file
 *
 * \brief Hashing of std::unordered_set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <unordered_set>

namespace bphash {

/*! \brief Hashing of std::unordered_set */
template<typename Key, typename HashT, typename Pred, typename Alloc>
void hash_object( const std::unordered_set<Key, HashT, Pred, Alloc> & s, Hasher & h)
{
    detail::hash_container_object(s, h);
}


} // close namespace bphash

