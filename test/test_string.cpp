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
    std::vector<const char *> str_test{"",
                                       " ",
                                       "\n",
                                       "String1",
                                       "String 1",
                                       "string1",
                                       "string 1",
                                       "STRING1",
                                       " String1",
                                       "String1!",
                                       "String1!\n"};

    test_all<const char *>(str_test, htype, all_hashes);
    test_all<std::string>(str_test, htype, all_hashes);
}

