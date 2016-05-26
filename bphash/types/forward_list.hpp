/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__FORWARD_LIST_HPP_
#define PULSAR_GUARD_TYPES__FORWARD_LIST_HPP_

#include <forward_list>
#include "bphash/types/ContainerHelper.hpp"

namespace bphash{
namespace detail {


///////////////
// list
///////////////
template<typename T, typename Alloc>
struct ObjectHasher<std::forward_list<T, Alloc>>
    : public ContainerHasher<std::forward_list<T, Alloc>> { };


} // close namespace detail
} // close namespace bphash

#endif
