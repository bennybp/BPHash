/*! \file
 *
 * \brief Hashing of std::array
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/types/ContainerHelper.hpp"
#include <array>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::array */
template<typename T, size_t N>
struct ObjectHasher<std::array<T, N>> : public ContainerHasher<std::array<T, N>> { };


} // close namespace detail
} // close namespace bphash

