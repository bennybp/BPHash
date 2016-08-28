/*! \file
 *
 * \brief Hashing of std::complex
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <complex>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::complex */
template<typename T>
struct ObjectHasher<std::complex<T>> : public is_hashable<T>
{
    static void
    hash(Hasher & hasher, const std::complex<T> & obj)
    {
        hasher(obj.real(), obj.imag());
    }
};


} // close namespace detail
} // close namespace bphash

