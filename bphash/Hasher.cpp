/*! \file
 * \brief A class that hashes objects (source)
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "bphash/Hasher.hpp"

// Different hashing types
#include "MurmurHash3_128_x64.hpp"
#include "MurmurHash3_64_x64.hpp"
#include "MurmurHash3_32_x64.hpp"
#include "MurmurHash3_32_x32.hpp"

namespace bphash {

Hasher::Hasher(HashType type)
{
    switch(type)
    {
        case HashType::Hash128:
        case HashType::Hash128_x32:
        case HashType::Hash128_x64:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_128_x64);
            break;

        case HashType::Hash64:
        case HashType::Hash64_x32:
        case HashType::Hash64_x64:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_64_x64);
            break;

        case HashType::Hash32:
        case HashType::Hash32_x64:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_32_x64);
            break;

        case HashType::Hash32_x32:
            hashimpl_ = std::unique_ptr<detail::HashImpl>(new detail::MurmurHash3_32_x32);
            break;
    }
}


} // close namespace bphash

