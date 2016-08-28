#include "test_helpers.hpp"

using namespace bphash;

void test_integers(HashType htype,
                   std::vector<HashValue> & found_hashes)
{
    // integer values to test
    // This contains negative integers, and we purposely pass them
    // as unsigned sometimes.
    // Unsigned overflow is defined behavior
    std::vector<long> int_test{0,  1,  10,  100,  241,
                                  -1, -10, -100, -241};

    // create tests for complex values that are just combinations
    // of the vector above
    std::vector<std::complex<long>> cmplx_test;
    for(auto i : int_test)
    for(auto j : int_test)
        cmplx_test.push_back({i,j});

    test_all<signed char        >(int_test, htype, found_hashes);
    test_all<signed short       >(int_test, htype, found_hashes);
    test_all<signed int         >(int_test, htype, found_hashes);
    test_all<signed long        >(int_test, htype, found_hashes);
    test_all<signed long long   >(int_test, htype, found_hashes);
    test_all<unsigned char      >(int_test, htype, found_hashes);
    test_all<unsigned short     >(int_test, htype, found_hashes);
    test_all<unsigned int       >(int_test, htype, found_hashes);
    test_all<unsigned long      >(int_test, htype, found_hashes);
    test_all<unsigned long long >(int_test, htype, found_hashes);

    test_all<std::complex<signed char        >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<signed short       >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<signed int         >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<signed long        >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<signed long long   >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<unsigned char      >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<unsigned short     >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<unsigned int       >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<unsigned long      >>(cmplx_test, htype, found_hashes);
    test_all<std::complex<unsigned long long >>(cmplx_test, htype, found_hashes);
}

