/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__UNORDERED_MAP_HPP_
#define PULSAR_GUARD_TYPES__UNORDERED_MAP_HPP_

#include <unordered_map>
#include "bphash/types/ContainerHelper.hpp"
#include "bphash/types/utility.hpp"

namespace bphash{
namespace detail {

///////////////
// unordered_map
///////////////
template<typename Key, typename T, typename HashT, typename Pred, typename Alloc>
struct ObjectHasher<std::unordered_map<Key, T, HashT, Pred, Alloc>>
    : public ContainerHasher<std::unordered_map<Key, T, HashT, Pred, Alloc>> { };


} // close namespace detail
} // close namespace bphash

#endif
