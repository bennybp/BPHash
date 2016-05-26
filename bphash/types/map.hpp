/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__MAP_HPP_
#define PULSAR_GUARD_TYPES__MAP_HPP_

#include <map>
#include "bphash/types/ContainerHelper.hpp"
#include "bphash/types/utility.hpp"

namespace bphash{
namespace detail {


///////////////
// map
///////////////
template<typename Key, typename T, typename Compare, typename Alloc>
struct ObjectHasher<std::map<Key, T, Compare, Alloc>>
    : public ContainerHasher<std::map<Key, T, Compare, Alloc>> { };


} // close namespace detail
} // close namespace bphash

#endif
