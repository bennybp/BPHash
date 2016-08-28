/*! \file
 *
 * \brief Helpers for hashing STL containers
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/Hasher.hpp"

namespace bphash {
namespace detail {

/*! \brief Helper for hashing STL containers */
template<typename Cont>
struct ContainerHasher : public is_hashable<typename Cont::value_type>
{
    template<typename U = Cont>
    static
    typename std::enable_if<is_hashable<typename U::value_type>::value, void>::type
    hash(Hasher & hasher, const Cont & cont)
    {
        // some containers don't have size() (ie, forward_list)
        size_t d = static_cast<size_t>(std::distance(cont.begin(), cont.end()));
        hasher(d);

        for(const auto & it : cont)
            hasher(it);
    }
};


} // close namespace detail
} // close namespace bphash

