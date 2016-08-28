/*! \file
 *
 * \brief Hashing of std::set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <set>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::set */
template<typename Key, typename Compare, typename Alloc>
struct ObjectHasher<std::set<Key, Compare, Alloc>> : public ContainerHasher<std::set<Key, Compare, Alloc>> { };


} // close namespace detail
} // close namespace bphash

