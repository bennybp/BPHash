/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__TUPLE_HPP_
#define PULSAR_GUARD_TYPES__TUPLE_HPP_

#include <tuple>
#include "bphash/Hasher.hpp"

namespace bphash{
namespace detail {


//////////////////////////////////////////
// Tuple
//////////////////////////////////////////

template<typename... Types>
struct ObjectHasher<std::tuple<Types...>> : public std::true_type
{
    template<size_t Idx>
    static
    typename std::enable_if<Idx == sizeof...(Types), void>::type
    HashTupleElement(Hasher &, const std::tuple<Types...> &)
    { }


    template<size_t Idx>
    static
    typename std::enable_if<Idx < sizeof...(Types), void>::type
    HashTupleElement(Hasher & hasher, const std::tuple<Types...> & obj)
    {
        hasher(std::get<Idx>(obj));
        HashTupleElement<Idx+1>(hasher, obj);
    }

    static void
    Hash(Hasher & hasher, const std::tuple<Types...> & obj)
    {
        HashTupleElement<0>(hasher, obj);
    }
};


} // close namespace detail
} // close namespace bphash

#endif
