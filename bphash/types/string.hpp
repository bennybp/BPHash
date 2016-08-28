/*! \file
 *
 * \brief Hashing of std::string
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

//#include <string> // included via ContainerHelper
#include "bphash/types/ContainerHelper.hpp"

namespace bphash {
namespace detail {

/*! \brief Hashing of std::string */
template<typename charT, typename Traits, typename Alloc>
struct ObjectHasher<std::basic_string<charT, Traits, Alloc>> : public ContainerHasher<std::basic_string<charT, Traits, Alloc>> { };


} // close namespace detail
} // close namespace bphash

