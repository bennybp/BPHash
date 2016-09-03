/*! \file
 * \brief Testing of core hashing algorithms
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

/* This file tests the core MurmurHash algorithms against
 * the reference implementations in smhasher */

#include <iostream>
#include <random>
#include <chrono>
#include <sstream>

#include "bphash/Hasher.hpp"
#include "bphash/MurmurHash3_32_x32.hpp"
#include "bphash/MurmurHash3_32_x64.hpp"
#include "bphash/MurmurHash3_64_x64.hpp"
#include "bphash/MurmurHash3_128_x64.hpp"

#include "MurmurHash3_reference.h" // in this directory

// Size of the test set. Make it an odd number
#define TEST_SIZE 1024*1024 + 7

using namespace bphash;


void random_fill(std::vector<uint8_t> & buffer)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<uint8_t> dist;

    for(auto & it : buffer)
        it = dist(generator);
}


void test_offset(detail::HashImpl & hasher,
                 const std::vector<uint8_t> & testdata,
                 size_t offset, size_t blocksize,
                 const HashValue & reference,
                 int hashsize, int bitness)
{
    std::cout << "Testing " << hashsize << "-bit x" << bitness << " hash,"
              << " offset " << offset
              << " blocksize " << blocksize << " ... ";

    hasher.reset();

    // do the first part
    hasher.update(testdata.data(), offset);

    // now do the rest by blocks
    if(blocksize == 0)
        blocksize = testdata.size() - offset;

    size_t done = offset;
    size_t todo = 0;

    do {
        todo = blocksize;
        if( (done + todo) > testdata.size() )
            todo = testdata.size() - done;

        hasher.update(testdata.data() + done, todo);
        done += todo;

    } while(todo);

    HashValue calc = hasher.finalize();

    if(calc != reference)
    {
        std::cout << "FAILED\n";

        std::stringstream ss;
        ss << "Mismatch: " << hashsize << "-bit hash,"
                           << " offset " << offset
                           << " blocksize " << blocksize;

        throw std::runtime_error(ss.str());
    }
    else
        std::cout << "OK\n";
}



int main(void)
{
    std::vector<uint8_t> testdata(TEST_SIZE);
    random_fill(testdata);

    HashValue ref_32_x32(4);
    HashValue ref_32_x64(4);
    HashValue ref_64_x64(8);
    HashValue ref_128_x64(16);

    const void * testdata_ptr = testdata.data();
    const size_t testdata_size = testdata.size();
    const int testdata_size_int = static_cast<int>(testdata_size);

    // calculate the reference values
    MurmurHash3_x86_32(testdata_ptr, testdata_size_int, 0, ref_32_x32.data());
    MurmurHash3_x64_128(testdata_ptr, testdata_size_int, 0, ref_128_x64.data());
    ref_32_x64 = truncate_hash(ref_128_x64, 4);
    ref_64_x64 = truncate_hash(ref_128_x64, 8);

    detail::MurmurHash3_32_x32 mh32_x32;
    detail::MurmurHash3_32_x64 mh32_x64;
    detail::MurmurHash3_64_x64 mh64_x64;
    detail::MurmurHash3_128_x64 mh128_x64;

    mh32_x32.update(testdata_ptr, testdata_size);
    mh32_x64.update(testdata_ptr, testdata_size);
    mh64_x64.update(testdata_ptr, testdata_size);
    mh128_x64.update(testdata_ptr, testdata_size);

    auto bph_32_x32 = mh32_x32.finalize();
    auto bph_32_x64 = mh32_x64.finalize();
    auto bph_64_x64 = mh64_x64.finalize();
    auto bph_128_x64 = mh128_x64.finalize();


    std::string ref_32_x32_str = hash_to_string(ref_32_x32);
    std::string ref_32_x64_str = hash_to_string(ref_32_x64);
    std::string ref_64_x64_str = hash_to_string(ref_64_x64);
    std::string ref_128_x64_str = hash_to_string(ref_128_x64);

    std::string bph_32_x32_str = hash_to_string(bph_32_x32);
    std::string bph_32_x64_str = hash_to_string(bph_32_x64);
    std::string bph_64_x64_str = hash_to_string(bph_64_x64);
    std::string bph_128_x64_str = hash_to_string(bph_128_x64);

    try {

    std::cout << "\n";
    std::cout << "32-bit hash, x32\n";
    std::cout << "Reference:  " << ref_32_x32_str << "\n";
    std::cout << "   BPHash:  " << bph_32_x32_str << "\n";
    std::cout << "\n";
    if(ref_32_x32 != bph_32_x32)
        throw std::runtime_error("Mismatch on 32-bit x64 hash");

    std::cout << "32-bit hash, x64\n";
    std::cout << "Reference:  " << ref_32_x64_str << "\n";
    std::cout << "   BPHash:  " << bph_32_x64_str << "\n";
    std::cout << "\n";
    if(ref_32_x64 != bph_32_x64)
        throw std::runtime_error("Mismatch on 32-bit x64 hash");

    std::cout << "64-bit hash, x64\n";
    std::cout << "Reference:  " << ref_64_x64_str << "\n";
    std::cout << "   BPHash:  " << bph_64_x64_str << "\n";
    std::cout << "\n";
    if(ref_64_x64 != bph_64_x64)
        throw std::runtime_error("Mismatch on 64-bit x64 hash");

    std::cout << "128-bit hash, x64\n";
    std::cout << "Reference:  " << ref_128_x64_str << "\n";
    std::cout << "   BPHash:  " << bph_128_x64_str << "\n";
    std::cout << "\n";
    if(ref_128_x64 != bph_128_x64)
        throw std::runtime_error("Mismatch on 128-bit x64 hash");


    // try bphash with different offsets (to test progressive hashing
    for(size_t i = 0; i <= 18; i++)
    for(size_t j = 0; j <= 1150; j += 23) // purposely odd numbers
    {
        test_offset(mh32_x32,  testdata, i, j, ref_32_x32,  32, 32);
        test_offset(mh32_x64,  testdata, i, j, ref_32_x64,  32, 64);
        test_offset(mh64_x64,  testdata, i, j, ref_64_x64,  64, 64);
        test_offset(mh128_x64, testdata, i, j, ref_128_x64, 128, 64);
    }

    std::cout << "\n";

    }
    catch(const std::exception & ex)
    {
        std::cout << "!!! Failed test: " << ex.what() << "\n\n";
        return 1;
    }

    return 0;
}

