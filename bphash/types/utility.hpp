/*! \file
 *
 * \brief Hashing of std::pair
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <utility>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::pair */
template<typename T1, typename T2>
struct ObjectHasher<std::pair<T1, T2>> : public is_hashable<T1, T2>
{
    template<typename U1 = T1, typename U2 = T2>
    static
    typename std::enable_if<is_hashable<U1>::value &&
                            is_hashable<U2>::value, void>::type
    hash(Hasher & hasher, const std::pair<U1, U2> & obj)
    {
        hasher(obj.first, obj.second);
    }
};


} // close namespace detail
} // close namespace bphash

