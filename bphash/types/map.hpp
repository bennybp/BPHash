/*! \file
 *
 * \brief Hashing of std::map
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPHASH_GUARD_MAP_HPP_
#define BPHASH_GUARD_MAP_HPP_

#include "bphash/types/ContainerHelper.hpp"
#include "bphash/types/utility.hpp"
#include <map>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::map */
template<typename Key, typename T, typename Compare, typename Alloc>
struct ObjectHasher<std::map<Key, T, Compare, Alloc>>
            : public ContainerHasher<std::map<Key, T, Compare, Alloc>> { };


} // close namespace detail
} // close namespace bphash

#endif
