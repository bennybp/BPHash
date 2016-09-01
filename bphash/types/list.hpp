/*! \file
 *
 * \brief Hashing of std::list
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <list>

namespace bphash {

/*! \brief Hashing of std::list */
template<typename T, typename Alloc>
void hash_object( const std::list<T, Alloc> & a, Hasher & h)
{
    detail::hash_container_object(a, h);
}


} // close namespace bphash

