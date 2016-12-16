/*! \file
 * \brief Forward declarations of some of BPHash
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once


namespace bphash {

class Hasher;

template<typename T> struct PointerWrapper;

namespace detail {

template <typename T> class detect_hash_member;

}


/*! \brief Macro for declaring certain functions as friends
 *
 * This can be used for classes where the hashing functions are
 * kept protected or private.
 */
#define BPHASH_DECLARE_HASHING_FRIENDS \
    friend class bphash::Hasher;\
    template<typename T__> friend class bphash::detail::detect_hash_member;



} // close namespace bphash

