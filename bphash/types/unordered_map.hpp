/*! \file
 *
 * \brief Hashing of std::unordered_map
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include "bphash/types/utility.hpp"
#include <unordered_map>

namespace bphash {

/*! \brief Hashing of std::unordered_map */
template<typename Key, typename T, typename HashT, typename Pred, typename Alloc>
void hash_object( const std::unordered_map<Key, T, HashT, Pred, Alloc> & m, Hasher & h)
{
    detail::hash_container_object(m, h);
}


} // close namespace bphash

