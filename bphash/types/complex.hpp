/*! \file
 *
 * \brief Hashing of std::complex
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <complex>

namespace bphash {

/*! \brief Hashing of std::complex */
template<typename T>
void hash_object( const std::complex<T> & a, Hasher & h)
{
    h(a.real(), a.imag());
}


} // close namespace bphash

