/*! \file
 * \brief Testing of integral types
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "test_helpers.hpp"

using namespace bphash;

void test_integers(HashType htype,
                   std::vector<HashValue> & all_hashes)
{
    // create tests for complex values that are just combinations
    // of the vector above
    std::vector<std::complex<long>> cmplx_test;
    for(auto i : int_test)
    for(auto j : int_test)
        cmplx_test.push_back({i,j});

    test_fundamental<signed char        >(int_test, htype, all_hashes);
    test_fundamental<signed short       >(int_test, htype, all_hashes);
    test_fundamental<signed int         >(int_test, htype, all_hashes);
    test_fundamental<signed long        >(int_test, htype, all_hashes);
    test_fundamental<signed long long   >(int_test, htype, all_hashes);
    test_fundamental<unsigned char      >(int_test, htype, all_hashes);
    test_fundamental<unsigned short     >(int_test, htype, all_hashes);
    test_fundamental<unsigned int       >(int_test, htype, all_hashes);
    test_fundamental<unsigned long      >(int_test, htype, all_hashes);
    test_fundamental<unsigned long long >(int_test, htype, all_hashes);
}

