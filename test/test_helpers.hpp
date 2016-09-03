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
                 std::vector<bphash::HashValue> & found_hashes)
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

    found_hashes.push_back(move(val_hash));
}


template<typename T, size_t N, typename InputIt>
void test_array(const InputIt first,
                bphash::HashType htype,
                std::vector<bphash::HashValue> & found_hashes)
{
    std::array<T, N> arr;
    for(size_t i = 0; i < N; i++)
        arr[i] = *(first + i);

    test_single<>(arr, htype, found_hashes);    
}



#define HANDLE_TEST_ARRAY(N) \
     case N: \
         test_array<T, 1>(first, htype, found_hashes); \
         break;

template<typename T>
void test_containers(const std::vector<T> & values,
                     size_t length,
                     bphash::HashType htype,
                     std::vector<bphash::HashValue> & found_hashes)
{
    const auto first = values.begin();
    const auto last = values.begin() + length + 1;
    test_single<std::vector<T>        >({first, last}, htype, found_hashes);
    test_single<std::list<T>          >({first, last}, htype, found_hashes);
    test_single<std::forward_list<T>  >({first, last}, htype, found_hashes);
    test_single<std::set<T>           >({first, last}, htype, found_hashes);
    test_single<std::unordered_set<T> >({first, last}, htype, found_hashes);

    // arrays have to be handled slightly differently
    auto d = std::distance(first, last);

    switch(d)
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

#undef HANDLE_TEST_ARRAY

template<typename T>
void test_vectors(const std::vector<T> & values,
                  bphash::HashType htype,
                  std::vector<bphash::HashValue> & found_hashes)
{
    for(size_t i = 0; i < values.size(); i++)
    {
        std::vector<T> new_values{values.begin(), values.begin() + i + 1};
        std::vector<T *> new_rptrs;
        std::vector<std::unique_ptr<T>> new_uptrs;
        std::vector<std::shared_ptr<T>> new_sptrs;

        for(auto & it : new_values)
        {
            new_rptrs.push_back(&it);
            new_uptrs.push_back(std::unique_ptr<T>(new T(it)));
            new_sptrs.push_back(std::shared_ptr<T>(new T(it)));
        }

        test_single(new_values, htype, found_hashes);
        test_single(new_rptrs,  htype, found_hashes);
        test_single(new_uptrs,  htype, found_hashes);
        test_single(new_sptrs,  htype, found_hashes);
    }
}


template<typename To, typename From>
void test_all(const std::vector<From> & values,
                 bphash::HashType htype,
                 std::vector<bphash::HashValue> & found_hashes)
{
    // Create a new vector of a different type
    // Also, vectors of pointers, etc
    std::vector<To> new_values;
    std::vector<To *> new_rptrs;
    std::vector<std::unique_ptr<To>> new_uptrs;
    std::vector<std::shared_ptr<To>> new_sptrs;

    for(const auto & it : values)
    {
        To new_val = static_cast<To>(it);
        new_values.push_back(new_val);
        new_rptrs.push_back(new To(new_val));
        new_uptrs.push_back(std::unique_ptr<To>(new To(new_val)));
        new_sptrs.push_back(std::shared_ptr<To>(new To(new_val)));
    }


    for(size_t i = 0; i < new_values.size(); i++)
    {
        test_single(new_values.at(i), htype, found_hashes);
        test_single(new_rptrs.at(i),  htype, found_hashes);
        test_single(new_uptrs.at(i),  htype, found_hashes);
        test_single(new_sptrs.at(i),  htype, found_hashes);
    }

    for(const auto & it : new_rptrs)
        delete it;


    test_vectors(new_values, htype, found_hashes);
}
