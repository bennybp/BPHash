/*! \file
 *
 * \brief Hashing of std::forward_list
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <forward_list>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::forward_list */
template<typename T, typename Alloc>
struct ObjectHasher<std::forward_list<T, Alloc>> : public ContainerHasher<std::forward_list<T, Alloc>> { };


} // close namespace detail
} // close namespace bphash

