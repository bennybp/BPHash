/*! \file
 * \brief Testing of compile-time detection of hashing ability
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "bphash/Hasher.hpp"
#include "bphash/types/All.hpp"

#include <iostream>
#include <string>

using namespace bphash;

// This class has a hash member function
class HashMember
{ 
    public:
        int i;
        std::string s;

        void hash(Hasher & h) const { h(i, s); }
};


// These classes aren't hashable because of the member function is malformed
class HashMember_BadSig0 { };
class HashMember_BadSig1 { public: void hash(Hasher &) { } };
class HashMember_BadSig2 { public: void hash(const Hasher &) const { } };
class HashMember_BadSig3 { public: void hash(Hasher &&) const { } };
class HashMember_BadSig4 { public: void hash(Hasher) const { } };
class HashMember_BadSig5 { public: void hash(int) const { } };
class HashMember_BadSig6 { public: int  hash(Hasher &) const { return 0; } };


// This class is hashable via a free function
class HashFree
{
    public:
        int i;
        std::string s;
};

static void hash_object(const HashFree & hf, Hasher & h)
{
    h(hf.i, hf.s);
}


// These aren't hashable due to malformed free functions
class HashFree_BadSig0 { };
class HashFree_BadSig1 { };
class HashFree_BadSig2 { };
class HashFree_BadSig3 { };
class HashFree_BadSig4 { };
class HashFree_BadSig5 { };

/////////////////////////////////////////////////////////////////////////////
// Some of the following fail, since the free-function detector is a little
// more lax in checking some types
/////////////////////////////////////////////////////////////////////////////
//static void hash_object(HashFree_BadSig0 &, Hasher &) { }
//static void hash_object(const HashFree_BadSig1 &, const Hasher &) { }
//static void hash_object(const HashFree_BadSig2 &, Hasher) { }
//static void hash_object(const HashFree_BadSig3 &, Hasher &&) { }
//static void hash_object(const HashFree_BadSig4 &, int) { }
//static int hash_object(const HashFree_BadSig5 &, Hasher &) { }



// This class is hashable via a free function
template<typename T>
class HashFreeTemplate
{
    public:
        T data;
};

template<typename T>
void hash_object(const HashFreeTemplate<T> & hft, Hasher & h)
{
    h(hft.data);
}


#define IS_HASHABLE(type)     static_assert(is_hashable<type>::value, "Type " #type  " is not hashable, but should be")
#define IS_NOT_HASHABLE(type) static_assert(!is_hashable<type>::value, "Type " #type " is hashable, but shouldn't be")

int main(void)
{
    HashType htype = HashType::Hash128;

    // Test detection of fundamental types
    IS_HASHABLE(char);
    IS_HASHABLE(unsigned char);
    IS_HASHABLE(signed char);
    IS_HASHABLE(unsigned short);
    IS_HASHABLE(short);
    IS_HASHABLE(unsigned int);
    IS_HASHABLE(int);
    IS_HASHABLE(unsigned long);
    IS_HASHABLE(long);
    IS_HASHABLE(unsigned long long);
    IS_HASHABLE(long long);
    IS_HASHABLE(float);
    IS_HASHABLE(double);
    IS_HASHABLE(long double);

    // Complex numbers
    IS_HASHABLE(std::complex<char>);
    IS_HASHABLE(std::complex<unsigned char>);
    IS_HASHABLE(std::complex<signed char>);
    IS_HASHABLE(std::complex<unsigned short>);
    IS_HASHABLE(std::complex<short>);
    IS_HASHABLE(std::complex<unsigned int>);
    IS_HASHABLE(std::complex<int>);
    IS_HASHABLE(std::complex<unsigned long>);
    IS_HASHABLE(std::complex<long>);
    IS_HASHABLE(std::complex<unsigned long long>);
    IS_HASHABLE(std::complex<long long>);
    IS_HASHABLE(std::complex<float>);
    IS_HASHABLE(std::complex<double>);
    IS_HASHABLE(std::complex<long double>);


    // Pointers to fundamental types
    IS_HASHABLE(char *);
    IS_HASHABLE(unsigned char *);
    IS_HASHABLE(signed char *);
    IS_HASHABLE(unsigned short *);
    IS_HASHABLE(short *);
    IS_HASHABLE(unsigned int *);
    IS_HASHABLE(int *);
    IS_HASHABLE(unsigned long *);
    IS_HASHABLE(long *);
    IS_HASHABLE(unsigned long long *);
    IS_HASHABLE(long long *);
    IS_HASHABLE(float *);
    IS_HASHABLE(double *);
    IS_HASHABLE(long double *);

    // Arrays should not be hashable
    IS_NOT_HASHABLE(char []);
    IS_NOT_HASHABLE(unsigned char []);
    IS_NOT_HASHABLE(signed char []);
    IS_NOT_HASHABLE(unsigned short []);
    IS_NOT_HASHABLE(short []);
    IS_NOT_HASHABLE(unsigned int []);
    IS_NOT_HASHABLE(int []);
    IS_NOT_HASHABLE(unsigned long []);
    IS_NOT_HASHABLE(long []);
    IS_NOT_HASHABLE(unsigned long long []);
    IS_NOT_HASHABLE(long long []);
    IS_NOT_HASHABLE(float []);
    IS_NOT_HASHABLE(double []);
    IS_NOT_HASHABLE(long double []);


    // Test a few pairs
    static_assert(is_hashable<std::pair<int, int>>::value, "Failed test for pair");
    static_assert(is_hashable<std::pair<const char, float>>::value, "Failed test for pair");
    static_assert(is_hashable<std::pair<unsigned long, short>>::value, "Failed test for pair");
    static_assert(!is_hashable<std::pair<HashMember_BadSig0, int>>::value, "Test");


    static_assert(is_hashable<HashMember>::value, "Failed test for Hash Member");

    static_assert(!is_hashable<HashMember_BadSig0>::value, "Failed test for member function bad signature 0");
    static_assert(!is_hashable<HashMember_BadSig1>::value, "Failed test for member function bad signature 1");
    static_assert(!is_hashable<HashMember_BadSig2>::value, "Failed test for member function bad signature 2");
    static_assert(!is_hashable<HashMember_BadSig3>::value, "Failed test for member function bad signature 3");
    static_assert(!is_hashable<HashMember_BadSig4>::value, "Failed test for member function bad signature 4");
    static_assert(!is_hashable<HashMember_BadSig5>::value, "Failed test for member function bad signature 5");
    static_assert(!is_hashable<HashMember_BadSig6>::value, "Failed test for member function bad signature 6");


    static_assert(is_hashable<HashFree>::value, "Failed test for free hash function");

    static_assert(!is_hashable<HashFree_BadSig0>::value, "Failed test for free function bad signature 0");
    //static_assert(!is_hashable<HashFree_BadSig1>::value, "Failed test for free function bad signature 1");
    static_assert(!is_hashable<HashFree_BadSig2>::value, "Failed test for free function bad signature 2");
    static_assert(!is_hashable<HashFree_BadSig3>::value, "Failed test for free function bad signature 3");
    static_assert(!is_hashable<HashFree_BadSig4>::value, "Failed test for free function bad signature 4");
    //static_assert(!is_hashable<HashFree_BadSig5>::value, "Failed test for free function bad signature 5");
    
    static_assert(is_hashable<HashFreeTemplate<int>>::value, "Failed test for free hash function");
    static_assert(is_hashable<HashFreeTemplate<std::string>>::value, "Failed test for free hash function");



    HashMember hm{123, "This is a test string\n"};
    HashFree   hf{123, "This is a test string\n"};
    HashFreeTemplate<int> hfti{123};
    HashFreeTemplate<std::string> hfts{"This is a test string\n"};

    HashValue hm_val = make_hash(htype, hm);
    HashValue hf_val = make_hash(htype, hf);
    HashValue hfti_val = make_hash(htype, hfti);
    HashValue hfts_val = make_hash(htype, hfts);

    std::cout << "\n\n";
    std::cout << "                Hash of HashMember class: " << hash_to_string(hm_val) << "\n";
    std::cout << "                  Hash of HashFree class: " << hash_to_string(hf_val) << "\n";
    std::cout << "     Hash of HashFreeTemplate<int> class: " << hash_to_string(hfti_val) << "\n";
    std::cout << "  Hash of HashFreeTemplate<string> class: " << hash_to_string(hfts_val) << "\n";
    std::cout << "                                          (they aren't supposed to be the same)\n";
    std::cout << "\n\n";



    return 0;
}

