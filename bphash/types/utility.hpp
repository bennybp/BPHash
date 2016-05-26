/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__UTILITY_HPP_
#define PULSAR_GUARD_TYPES__UTILITY_HPP_

#include <utility>
#include "bphash/Hasher.hpp"

namespace bphash{
namespace detail {


//////////////////////////////////////////
// Pairs
//////////////////////////////////////////

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
