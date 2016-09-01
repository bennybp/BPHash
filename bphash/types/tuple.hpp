/*! \file
 *
 * \brief Hashing of std::tuple
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

