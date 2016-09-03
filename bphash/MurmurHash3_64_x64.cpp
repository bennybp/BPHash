/*! \file
 * \brief MurmurHash3 64-bit x64 hash (source)
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "bphash/MurmurHash3_64_x64.hpp"

namespace bphash {
namespace detail {

HashValue MurmurHash3_64_x64::finalize(void)
{
    HashValue hv = MurmurHash3_128_x64::finalize();
    return truncate_hash(hv, 8);
}


} // close namespace detail
} // close namespace bphash

