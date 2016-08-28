/*! \file
 *
 * \brief Hashing of std::tuple
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <tuple>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::tuple */
template<typename... Types>
struct ObjectHasher<std::tuple<Types...>> : public is_hashable<Types...>
{
    private:
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

    public:
        static void
        hash(Hasher & hasher, const std::tuple<Types...> & obj)
        {
            HashTupleElement<0>(hasher, obj);
        }
};


} // close namespace detail
} // close namespace bphash

