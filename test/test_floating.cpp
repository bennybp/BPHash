/*! \file
 * \brief Testing of floating point types
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "test_helpers.hpp"

using namespace bphash;

void test_floating(HashType htype,
                   std::vector<HashValue> & all_hashes)
{
    // integer values to test
    // This contains negative integers, and we purposely pass them
    // as unsigned sometimes.
    // Unsigned overflow is defined behavior
    std::vector<double> dbl_test{0,  1.0,  1.000001,  1.12e12,  1.12001e12,  1.12001e-12
                                    -1.0, -1.000001, -1.12e12, -1.12001e12, -1.12001e-12};

    // create tests for complex values that are just combinations
    // of the vector above
    std::vector<std::complex<double>> cmplx_test;
    for(auto i : dbl_test)
    for(auto j : dbl_test)
        cmplx_test.push_back({i,j});

    test_all<float  >(dbl_test, htype, all_hashes);
    test_all<double >(dbl_test, htype, all_hashes);

    test_all<std::complex<float >>(cmplx_test, htype, all_hashes);
    test_all<std::complex<double>>(cmplx_test, htype, all_hashes);
}

