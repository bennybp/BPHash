/*! \file
 *
 * \brief A class that hashes std::complex objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__COMPLEX_HPP_
#define PULSAR_GUARD_TYPES__COMPLEX_HPP_

#include <complex>
#include "bphash/Hasher.hpp"

namespace bphash{
namespace detail {


//////////////////////////////////////////
// Complex type
//////////////////////////////////////////
template<typename T>
struct ObjectHasher<std::complex<T>> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const std::complex<T> & obj)
    {
        hasher(obj.real(), obj.imag());
    }
};


} // close namespace detail
} // close namespace bphash

#endif
