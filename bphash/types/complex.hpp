/*! \file
 * \brief Hashing of std::complex
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <complex>

namespace bphash {

/*! \brief Hashing of std::complex */
template<typename T>
typename std::enable_if<is_hashable<T>::value, void>::type
hash_object( const std::complex<T> & a, Hasher & h)
{
    h(a.real(), a.imag());
}


} // close namespace bphash

