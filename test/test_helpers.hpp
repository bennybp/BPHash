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

//////////////////////////////////
// This is the data we test with
//////////////////////////////////
// integer values to test
// This contains negative integers, and we purposely pass them
// as unsigned sometimes.
// Unsigned overflow is defined behavior
static const std::vector<long>
int_test{ 0,  1,  10,  100,  241,
             -1, -10, -100, -241};

static const std::vector<double>
dbl_test{ 0,  1.0,  1.000001,  1.12e12,  1.12001e12,  1.12001e-12
             -1.0, -1.000001, -1.12e12, -1.12001e12, -1.12001e-12};

static std::vector<const char *>
str_test{"",
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
void test_containers(const std::vector<From> & values,
                     bphash::HashType htype,
                     std::vector<bphash::HashValue> & all_hashes)
{
    // tests the entire vector, as well subvectors
    for(size_t i = 0; i <= values.size(); i++)
    {
        std::vector<To> new_vec{values.begin(), values.begin() + i};
        std::list<To> new_list{values.begin(), values.begin() + i};
        std::unordered_set<To> new_uset{values.begin(), values.begin() + i};
        std::forward_list<To> new_flist{values.begin(), values.begin() + i};
        std::set<To> new_set{values.begin(), values.begin() + i};

        test_single(new_vec, htype, all_hashes);
        test_single(new_list, htype, all_hashes);
        test_single(new_uset, htype, all_hashes);
        test_single(new_flist, htype, all_hashes);
        test_single(new_set, htype, all_hashes);
    }
}



template<typename To, typename From>
void test_fundamental(const std::vector<From> & values,
                      bphash::HashType htype,
                      std::vector<bphash::HashValue> & all_hashes)
{
    test_values<To>(values, htype, all_hashes);
    test_pointers<To>(values, htype, all_hashes);
    test_arrays<To>(values, htype, all_hashes);
    test_containers<To>(values, htype, all_hashes);
}



template<typename To1, typename To2,
         typename From1, typename From2>
void test_tuples_2(const std::vector<From1> & values1,
                   const std::vector<From2> & values2,
                   bphash::HashType htype,
                   std::vector<bphash::HashValue> & all_hashes)
{
    typedef std::tuple<To1, To2> To12;
    typedef std::pair<To1, To2> ToPair12;

    std::vector<To12> tup12;
    std::vector<ToPair12> pair12;

    for(size_t i = 0; i < values1.size(); i++)
    for(size_t j = 0; j < values2.size(); j++)
    {
        tup12.push_back(std::make_tuple<To1, To2>(
                            static_cast<To1>(values1.at(i)),
                            static_cast<To2>(values2.at(j))));

        pair12.push_back(std::make_pair<To1, To2>(
                            static_cast<To1>(values1.at(i)),
                            static_cast<To2>(values2.at(j))));
    }


    test_values  <To12>(tup12, htype, all_hashes);
    test_pointers<To12>(tup12, htype, all_hashes);
    test_arrays  <To12>(tup12, htype, all_hashes);
    test_values  <ToPair12>(pair12, htype, all_hashes);
    test_pointers<ToPair12>(pair12, htype, all_hashes);
    test_arrays  <ToPair12>(pair12, htype, all_hashes);

    // test some containers
    // tuple's don't have comparisons or std::hash
    for(size_t i = 0; i <= tup12.size(); i++)
    {
        std::vector<To12> new_vec{tup12.begin(), tup12.begin() + i};
        std::list<To12>   new_list{tup12.begin(), tup12.begin() + i};
        test_single(new_vec, htype, all_hashes);
        test_single(new_list, htype, all_hashes);
    }
    for(size_t i = 0; i <= pair12.size(); i++)
    {
        std::vector<ToPair12> new_vec{pair12.begin(), pair12.begin() + i};
        std::list<ToPair12>   new_list{pair12.begin(), pair12.begin() + i};
        test_single(new_vec, htype, all_hashes);
        test_single(new_list, htype, all_hashes);
    }
}




