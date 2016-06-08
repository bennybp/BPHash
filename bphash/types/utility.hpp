/*! \file
 *
 * \brief Hashing of std::pair
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPHASH_GUARD_UTILITY_HPP_
#define BPHASH_GUARD_UTILITY_HPP_

#include "bphash/Hasher.hpp"
#include <utility>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::pair */
template<typename T1, typename T2>
struct ObjectHasher<std::pair<T1, T2>> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const std::pair<T1, T2> & obj)
    {
        hasher(obj.first, obj.second);
    }
};


} // close namespace detail
} // close namespace bphash

#endif
