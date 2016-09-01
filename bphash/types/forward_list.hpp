/*! \file
 *
 * \brief Hashing of std::forward_list
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <forward_list>

namespace bphash {

/*! \brief Hashing of std::forward_list */
template<typename T, typename Alloc>
void hash_object( const std::forward_list<T, Alloc> & a, Hasher & h)
{
    detail::hash_container_object(a, h);
}


} // close namespace bphash

