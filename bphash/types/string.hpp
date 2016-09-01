/*! \file
 *
 * \brief Hashing of std::string
 */

#pragma once

//#include <string> // included via ContainerHelper
#include "bphash/types/ContainerHelper.hpp"

namespace bphash {

/*! \brief Hashing of std::string */
template<typename charT, typename Traits, typename Alloc>
void hash_object( const std::basic_string<charT, Traits, Alloc> & s, Hasher & h)
{
    detail::hash_container_object(s, h);
}


} // close namespace bphash

