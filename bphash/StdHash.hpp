/*! \file
 * \brief A class that can be used in place of std::hash
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hasher.hpp"

namespace bphash {

/*! \brief A class that can be used in place of std::hash in containers
 *
 * This is useful for `unordered_map`, etc, that require hashing of the key
 * type.
 */
template<typename T>
struct StdHash
{
    size_t operator()(const T & obj) const
    {
        static_assert(is_hashable<T>::value,
         "\n\n"
         "  ***  Object is not hashable. Did you remember to include the correct header       ***\n"
         "  ***  (such as <bphash/types/string.hpp>) or to declare a hash member function or  ***\n"
         "  ***  free function?                                                               ***\n");

        auto h = make_hash(HashType::Hash64, obj);
        return convert_hash<size_t>(h);
    }
};

} // close namespace bphash

