/*! \file
 *
 * \brief Helpers for hashing STL containers
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__CONTAINERHELPER_HPP_
#define PULSAR_GUARD_TYPES__CONTAINERHELPER_HPP_

#include "bphash/Hasher.hpp"

namespace bphash{
namespace detail {


//////////////////////////////////////////
// Helper for STL containers
//////////////////////////////////////////
template<typename Cont>
struct ContainerHasher : public std::true_type
{
    static void
    Hash(Hasher & hasher, const Cont & cont)
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

#endif
