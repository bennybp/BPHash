/*! \file
 * \brief Testing of string types
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "test_helpers.hpp"

using namespace bphash;

void test_string(HashType htype,
                 std::vector<HashValue> & all_hashes)
{
    test_fundamental<const char *>(str_test, htype, all_hashes);
    test_fundamental<std::string>(str_test, htype, all_hashes);
}

