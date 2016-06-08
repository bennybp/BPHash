/*! \file
 *
 * \brief A class that can be used for std::hash
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPHASH_GUARD_STDHASH_HPP_
#define BPHASH_GUARD_STDHASH_HPP_

#include "bphash/Hasher.hpp"

namespace bphash {

/*! \brief A class that can be used in place of std::hash in containers
 */
template<typename T>
struct StdHash
{
    size_t operator()(const T & obj) const
    {
        //! \todo replace with Hash64 if available?
        return convert_hash<size_t>(MakeHash(HashType::Hash128, obj));
    }
};

}

#endif
