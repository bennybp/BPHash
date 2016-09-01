/*! \file
 *
 * \brief Hashing of std::map
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include "bphash/types/utility.hpp"
#include <map>

namespace bphash {

/*! \brief Hashing of std::map */
template<typename Key, typename T, typename Compare, typename Alloc>
void hash_object( const std::map<Key, T, Compare, Alloc> & m, Hasher & h)
{
    detail::hash_container_object(m, h);
}


} // close namespace bphash

