/*! \file
 * \brief Hashing of std::tuple
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <tuple>

namespace bphash {
namespace detail {


template<size_t Idx, typename... Types>
typename std::enable_if<Idx == sizeof...(Types), void>::type
tuple_element_hasher(Hasher &, const std::tuple<Types...> &)
{ }

template<size_t Idx, typename... Types>
typename std::enable_if<Idx < sizeof...(Types), void>::type
tuple_element_hasher(Hasher & h, const std::tuple<Types...> & tup)
{
    h(std::get<Idx>(tup));
    tuple_element_hasher<Idx+1>(h, tup);
}


} // close namespace detail



template<typename... Types>
typename std::enable_if<is_hashable<Types...>::value, void>::type
hash_object( const std::tuple<Types...> & tup, Hasher & h)
{
    detail::tuple_element_hasher<0>(h, tup);
}


} // close namespace bphash

