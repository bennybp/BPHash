/*! \file
 * \brief Helper functions for testing
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hasher.hpp"
#include "bphash/types/All.hpp"

#include <iostream>

template<typename T>
void test_single(const T & val,
                 bphash::HashType htype,
                 std::vector<bphash::HashValue> & all_hashes)
{
    using namespace bphash;
    using namespace std;

    const char * type = typeid(T).name();
    HashValue val_hash = make_hash(htype, val);

    // output the results
    cout << "Testing type: " << type << "\n";
    cout << "      normal: " << hash_to_string(val_hash) << "\n"
         << "       trunc: " << hash_to_string(truncate_hash(val_hash, 8)) << "\n"
         << "        conv: " << convert_hash<size_t>(val_hash) << "\n";

    all_hashes.push_back(move(val_hash));
}



template<typename To, typename From>
void test_values(const std::vector<From> & vals,
                 bphash::HashType htype,
                 std::vector<bphash::HashValue> & all_hashes)
{
    for(const auto & it : vals)
        test_single(static_cast<To>(it), htype, all_hashes);
}


template<typename To, typename From>
void test_pointers(const std::vector<From> & vals,
                   bphash::HashType htype,
                   std::vector<bphash::HashValue> & all_hashes)
{
    std::vector<To *> new_rptrs;
    std::vector<std::unique_ptr<To>> new_uptrs;
    std::vector<std::shared_ptr<To>> new_sptrs;

    for(const auto & it : vals)
    {
        To new_val = static_cast<To>(it);
        new_rptrs.push_back(new To(new_val));
        new_uptrs.push_back(std::unique_ptr<To>(new To(new_val)));
        new_sptrs.push_back(std::shared_ptr<To>(new To(new_val)));
    }

    for(const auto & it : new_rptrs)
        test_single(it, htype, all_hashes);

    for(const auto & it : new_uptrs)
        test_single(it, htype, all_hashes);

    for(const auto & it : new_sptrs)
        test_single(it, htype, all_hashes);

    for(const auto & it : new_rptrs)
        delete it;
}


template<typename To, typename From>
void test_vectors(const std::vector<From> & values,
                  bphash::HashType htype,
                  std::vector<bphash::HashValue> & all_hashes)
{
    // tests the entire vector, as well subvectors
    for(size_t i = 0; i < values.size(); i++)
    {
        std::vector<To> new_values{values.begin(), values.begin() + i + 1};
        test_single(new_values, htype, all_hashes);
    }
}



template<typename To, typename From, size_t N>
void test_array_helper(const std::vector<From> & vec,
                       bphash::HashType htype,
                       std::vector<bphash::HashValue> & all_hashes)
{
    std::array<To, N> arr_values;
    To plain_arr[N];

    for(size_t i = 0; i < N; i++)
    {
        arr_values[i] = static_cast<To>(vec.at(i));
        plain_arr[i] = static_cast<To>(vec.at(i));

    }

    test_single(arr_values, htype, all_hashes);
    test_single(bphash::hash_pointer(plain_arr, N),  htype, all_hashes);
}



#define HANDLE_TEST_ARRAY(N) \
     case N: \
         test_array_helper<To, From, N>(values, htype, all_hashes); \
         break;

template<typename To, typename From>
void test_arrays(const std::vector<From> & values,
                 bphash::HashType htype,
                 std::vector<bphash::HashValue> & all_hashes)
{
    // arrays have to be handled slightly differently
    for(size_t i = 0; i < values.size(); i++)
    {
        switch(i)
        {
            HANDLE_TEST_ARRAY( 1)
            HANDLE_TEST_ARRAY( 2)
            HANDLE_TEST_ARRAY( 3)
            HANDLE_TEST_ARRAY( 4)
            HANDLE_TEST_ARRAY( 5)
            HANDLE_TEST_ARRAY( 6)
            HANDLE_TEST_ARRAY( 7)
            HANDLE_TEST_ARRAY( 8)
            HANDLE_TEST_ARRAY( 9)
            HANDLE_TEST_ARRAY(10)
            HANDLE_TEST_ARRAY(11)
            HANDLE_TEST_ARRAY(12)
            HANDLE_TEST_ARRAY(13)
            HANDLE_TEST_ARRAY(14)
            HANDLE_TEST_ARRAY(15)
            HANDLE_TEST_ARRAY(16)
            HANDLE_TEST_ARRAY(17)
            HANDLE_TEST_ARRAY(18)
            HANDLE_TEST_ARRAY(19)
            HANDLE_TEST_ARRAY(20)
            default:
                break;
        }
    }
}

#undef HANDLE_TEST_ARRAY





template<typename To, typename From>
void test_all(const std::vector<From> & values,
              bphash::HashType htype,
              std::vector<bphash::HashValue> & all_hashes)
{
    test_values<To>(values, htype, all_hashes);
    test_pointers<To>(values, htype, all_hashes);
    test_vectors<To>(values, htype, all_hashes);
    test_arrays<To>(values, htype, all_hashes);
}

