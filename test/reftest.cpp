/*! \file
 * \brief Testing of core hashing algorithms
 */


/* This file tests the core MurmurHash algorithms against
 * the reference implementations in smhasher */

#include <iostream>
#include <random>
#include <chrono>
#include <sstream>

#include "bphash/Hasher.hpp"
#include "bphash/MurmurHash3_32.hpp"
#include "bphash/MurmurHash3_64.hpp"
#include "bphash/MurmurHash3_128.hpp"

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
                 int hashsize)
{
    std::cout << "Testing " << hashsize << "-bit hash,"
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

    HashValue ref_32(4);
    HashValue ref_64(8);
    HashValue ref_128(16);

    HashValue bph_32;
    HashValue bph_64;
    HashValue bph_128;

    const void * testdata_ptr = testdata.data();
    const size_t testdata_size = testdata.size();
    const int testdata_size_int = static_cast<int>(testdata_size);

    // calculate the reference values
    MurmurHash3_x86_32(testdata_ptr, testdata_size_int, 0, ref_32.data());
    MurmurHash3_x64_128(testdata_ptr, testdata_size_int, 0, ref_128.data());
    ref_64 = truncate_hash(ref_128, 8);


    detail::MurmurHash3_32 mh32;
    detail::MurmurHash3_64 mh64;
    detail::MurmurHash3_128 mh128;

    mh32.update(testdata_ptr, testdata_size);
    mh64.update(testdata_ptr, testdata_size);
    mh128.update(testdata_ptr, testdata_size);

    bph_32 = mh32.finalize();
    bph_64 = mh64.finalize();
    bph_128 = mh128.finalize();


    std::string ref_32_str = hash_to_string(ref_32);
    std::string ref_64_str = hash_to_string(ref_64);
    std::string ref_128_str = hash_to_string(ref_128);

    std::string bph_32_str = hash_to_string(bph_32);
    std::string bph_64_str = hash_to_string(bph_64);
    std::string bph_128_str = hash_to_string(bph_128);

    try {

    std::cout << "\n";
    std::cout << "Reference 32-bit hash:  " << ref_32_str << "\n";
    std::cout << "   BPHash 32-bit hash:  " << bph_32_str << "\n";
    std::cout << "\n";
    if(ref_32 != bph_32)
        throw std::runtime_error("Mismatch on 32-bit hash");

    std::cout << "Reference 64-bit hash:  " << ref_64_str << "\n";
    std::cout << "   BPHash 64-bit hash:  " << bph_64_str << "\n";
    std::cout << "\n";
    if(ref_64 != bph_64)
        throw std::runtime_error("Mismatch on 64-bit hash");


    std::cout << "Reference 128-bit hash: " << ref_128_str << "\n";
    std::cout << "   BPHash 128-bit hash: " << bph_128_str << "\n";
    std::cout << "\n";
    if(ref_128 != bph_128)
        throw std::runtime_error("Mismatch on 128-bit hash");


    // try bphash with different offsets (to test progressive hashing
    for(size_t i = 0; i <= 18; i++)
    for(size_t j = 0; j <= 1150; j += 23) // purposely odd numbers
    {
        test_offset(mh32,  testdata, i, j, ref_32,  32);
        test_offset(mh64,  testdata, i, j, ref_64,  64);
        test_offset(mh128, testdata, i, j, ref_128, 128);
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

