/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__UNORDERED_SET_HPP_
#define PULSAR_GUARD_TYPES__UNORDERED_SET_HPP_

#include <unordered_set>
#include "bphash/types/ContainerHelper.hpp"

namespace bphash{
namespace detail {

///////////////
// unordered_set
///////////////
template<typename Key, typename HashT, typename Pred, typename Alloc>
struct ObjectHasher<std::unordered_set<Key, HashT, Pred, Alloc>>
    : public ContainerHasher<std::unordered_set<Key, HashT, Pred, Alloc>> { };


} // close namespace detail
} // close namespace bphash

#endif
