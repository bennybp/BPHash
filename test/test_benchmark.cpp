/*! \file
 * \brief Benchmarking of hashing algorithms
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
#include <cstdlib>

#include "bphash/Hasher.hpp"
#include "bphash/types/vector.hpp"

#include "bphash/MurmurHash3_32_x64.hpp"
#include "bphash/MurmurHash3_32_x32.hpp"
#include "bphash/MurmurHash3_64_x64.hpp"
#include "bphash/MurmurHash3_128_x64.hpp"

using namespace bphash;
using namespace std::chrono;


void random_fill(std::vector<uint8_t> & buffer)
{
    auto seed = system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<uint8_t> dist;

    for(auto & it : buffer)
        it = dist(generator);
}


int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        std::cout << "\n  Missing number of bytes to test\n";
        std::cout << "\n  usage: bphash_benchmark nbytes\n\n";
        return 1;
    }

    size_t nbytes = static_cast<size_t>(atol(argv[1]));

    std::vector<uint8_t> testdata(nbytes);
    random_fill(testdata);

    const void * testdata_ptr = testdata.data();
    const size_t testdata_size = testdata.size();

    std::cout << "\nTesting hashing of " << nbytes << " bytes\n";
    std::cout << "Times in microseconds\n";

    high_resolution_clock timer_clock;

    // converts bytes/microsecond to GiB/second
    const double conv_fac = 1.0e6/(1024.0 * 1024.0 * 1024.0);

    {
        auto time0 = timer_clock.now();
        detail::MurmurHash3_32_x32 mh32;
        mh32.update(testdata_ptr, testdata_size);
        HashValue bph_32 = mh32.finalize();
        auto time1 = timer_clock.now();
        auto elapsed = duration_cast<microseconds>(time1-time0).count();
        double rate = static_cast<double>(nbytes)/static_cast<double>(elapsed);
        std::cout << "   32-bit x32 hash: " << elapsed
                  << " ( " << conv_fac*rate << " GiB/sec)\n";
    }

    {
        auto time0 = timer_clock.now();
        detail::MurmurHash3_32_x64 mh32;
        mh32.update(testdata_ptr, testdata_size);
        HashValue bph_32 = mh32.finalize();
        auto time1 = timer_clock.now();
        auto elapsed = duration_cast<microseconds>(time1-time0).count();
        double rate = static_cast<double>(nbytes)/static_cast<double>(elapsed);
        std::cout << "   32-bit x64 hash: " << elapsed
                  << " ( " << conv_fac*rate << " GiB/sec)\n";
    }

    {
        auto time0 = timer_clock.now();
        detail::MurmurHash3_64_x64 mh64;
        mh64.update(testdata_ptr, testdata_size);
        HashValue bph_64 = mh64.finalize();
        auto time1 = timer_clock.now();
        auto elapsed = duration_cast<microseconds>(time1-time0).count();
        double rate = static_cast<double>(nbytes)/static_cast<double>(elapsed);
        std::cout << "   64-bit x64 hash: " << elapsed
                  << " ( " << conv_fac*rate << " GiB/sec)\n";
    }

    {
        auto time0 = timer_clock.now();
        detail::MurmurHash3_128_x64 mh128;
        mh128.update(testdata_ptr, testdata_size);
        HashValue bph_128 = mh128.finalize();
        auto time1 = timer_clock.now();
        auto elapsed = duration_cast<microseconds>(time1-time0).count();
        double rate = static_cast<double>(nbytes)/static_cast<double>(elapsed);
        std::cout << "  128-bit x64 hash: " << elapsed
                  << " ( " << conv_fac*rate << " GiB/sec)\n";
    }

    std::cout << "\nHashing of vector via make_hash\n";

    {
        auto time0 = timer_clock.now();
        HashValue bph_32 = make_hash(HashType::Hash32_x32, testdata);
        auto time1 = timer_clock.now();
        auto elapsed = duration_cast<microseconds>(time1-time0).count();
        double rate = static_cast<double>(nbytes)/static_cast<double>(elapsed);
        std::cout << "   32-bit x32 hash: " << elapsed
                  << " ( " << conv_fac*rate << " GiB/sec)\n";
    }

    {
        auto time0 = timer_clock.now();
        HashValue bph_32 = make_hash(HashType::Hash32_x64, testdata);
        auto time1 = timer_clock.now();
        auto elapsed = duration_cast<microseconds>(time1-time0).count();
        double rate = static_cast<double>(nbytes)/static_cast<double>(elapsed);
        std::cout << "   32-bit x64 hash: " << elapsed
                  << " ( " << conv_fac*rate << " GiB/sec)\n";
    }

    {
        auto time0 = timer_clock.now();
        HashValue bph_64 = make_hash(HashType::Hash64_x64, testdata);
        auto time1 = timer_clock.now();
        auto elapsed = duration_cast<microseconds>(time1-time0).count();
        double rate = static_cast<double>(nbytes)/static_cast<double>(elapsed);
        std::cout << "   64-bit x64 hash: " << elapsed
                  << " ( " << conv_fac*rate << " GiB/sec)\n";
    }

    {
        auto time0 = timer_clock.now();
        HashValue bph_128 = make_hash(HashType::Hash128_x64, testdata);
        auto time1 = timer_clock.now();
        auto elapsed = duration_cast<microseconds>(time1-time0).count();
        double rate = static_cast<double>(nbytes)/static_cast<double>(elapsed);
        std::cout << "  128-bit x64 hash: " << elapsed
                  << " ( " << conv_fac*rate << " GiB/sec)\n";
    }

    std::cout << "\n\n";




    return 0;
}

