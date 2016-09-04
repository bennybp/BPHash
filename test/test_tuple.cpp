/*! \file
 * \brief Testing of tuple and pair
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "test_helpers.hpp"

using namespace bphash;

void test_tuple(HashType htype,
                std::vector<HashValue> & all_hashes)
{
    test_tuples_2<int,            int          >(int_test, int_test, htype, all_hashes); 
    test_tuples_2<int,            long         >(int_test, int_test, htype, all_hashes); 
    test_tuples_2<unsigned short, unsigned long>(int_test, int_test, htype, all_hashes); 

    test_tuples_2<const char *, const char *>(str_test, str_test, htype, all_hashes); 
    test_tuples_2<std::string,  const char *>(str_test, str_test, htype, all_hashes); 
    test_tuples_2<const char *, std::string >(str_test, str_test, htype, all_hashes); 
    test_tuples_2<std::string,  std::string >(str_test, str_test, htype, all_hashes); 

    test_tuples_2<int,     const char *      >(int_test, str_test, htype, all_hashes); 
    test_tuples_2<int,     std::string       >(int_test, str_test, htype, all_hashes); 
    test_tuples_2<unsigned short, std::string>(int_test, str_test, htype, all_hashes); 

    test_tuples_2<const char *, int           >(str_test, int_test, htype, all_hashes); 
    test_tuples_2<std::string,  int           >(str_test, int_test, htype, all_hashes); 
    test_tuples_2<std::string,  unsigned short>(str_test, int_test, htype, all_hashes); 
}

