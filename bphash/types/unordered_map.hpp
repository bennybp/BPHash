/*! \file
 *
 * \brief Hashing of std::unordered_map
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPHASH_GUARD_UNORDERED_MAP_HPP_
#define BPHASH_GUARD_UNORDERED_MAP_HPP_

#include "bphash/types/ContainerHelper.hpp"
#include <unordered_map>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::unordered_map */
template<typename Key, typename T, typename HashT, typename Pred, typename Alloc>
struct ObjectHasher<std::unordered_map<Key, T, HashT, Pred, Alloc>> : public ContainerHasher<std::unordered_map<Key, T, HashT, Pred, Alloc>> { };


} // close namespace detail
} // close namespace bphash

#endif
