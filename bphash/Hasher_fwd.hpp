/*! \file
 * \brief Forward declarations of some of BPHash
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
    template<typename T> friend class bphash::detail::detect_hash_member;



} // close namespace bphash

