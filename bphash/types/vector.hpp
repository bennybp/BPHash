/*! \file
 *
 * \brief Hashing of std::vector
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPHASH_GUARD_VECTOR_HPP_
#define BPHASH_GUARD_VECTOR_HPP_

#include "bphash/types/ContainerHelper.hpp"
#include <vector>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::vector */
template<typename T, typename Alloc>
struct ObjectHasher<std::vector<T, Alloc>> : public ContainerHasher<std::vector<T, Alloc>> { };


} // close namespace detail
} // close namespace bphash

#endif
