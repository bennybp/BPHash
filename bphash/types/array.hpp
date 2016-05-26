/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__ARRAY_HPP_
#define PULSAR_GUARD_TYPES__ARRAY_HPP_

#include <array>
#include "bphash/types/ContainerHelper.hpp"

namespace bphash{
namespace detail {


///////////////
// array
///////////////
template<typename T, size_t N>
struct ObjectHasher<std::array<T, N>>
    : public ContainerHasher<std::array<T, N>> { };


} // close namespace detail
} // close namespace bphash

#endif
