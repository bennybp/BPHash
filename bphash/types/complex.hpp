/*! \file
 *
 * \brief Hashing of std::complex
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPHASH_GUARD_COMPLEX_HPP_
#define BPHASH_GUARD_COMPLEX_HPP_

#include "bphash/Hasher.hpp"
#include <complex>

namespace bphash {
namespace detail {

/*! \brief Hashing of std::complex */
template<typename T>
struct ObjectHasher<std::complex<T>> : public std::true_type
{
    static void
    hash(Hasher & hasher, const std::complex<T> & obj)
    {
        hasher(obj.real(), obj.imag());
    }
};


} // close namespace detail
} // close namespace bphash

#endif
