/*! \file
 *
 * \brief Hashing of std::unordered_set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPHASH_GUARD_UNORDERED_SET_HPP_
#define BPHASH_GUARD_UNORDERED_SET_HPP_

#include "bphash/types/ContainerHelper.hpp"
#include <unordered_set>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::unordered_set */
template<typename Key, typename HashT, typename Pred, typename Alloc>
struct ObjectHasher<std::unordered_set<Key, HashT, Pred, Alloc>> : public ContainerHasher<std::unordered_set<Key, HashT, Pred, Alloc>> { };


} // close namespace detail
} // close namespace bphash

#endif
