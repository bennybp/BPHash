#include <iostream>
#include <algorithm>

#include "bphash/Hasher.hpp"
#include "bphash/types/All.hpp"

using namespace bphash;

namespace {
    std::vector<HashValue> found_hashes_;
}


template<typename T>
void TestSingle(const char * type, const T & val)
{
    HashType htype = HashType::Hash128;

    // raw value
    Hasher h(htype);
    h(val);

    // Raw pointer
    const T * valptr = new T(val);
    Hasher hptr(htype);
    hptr(valptr);
    delete valptr;

    // unique_ptr
    Hasher huptr(htype);
    std::unique_ptr<T> uptr(new T(val));
    huptr(uptr);

    // shared_ptr
    Hasher hsptr(htype);
    std::shared_ptr<T> sptr(new T(val));
    hsptr(sptr);

    HashValue val_hash = h.finalize();
    HashValue ptr_hash = hptr.finalize();
    HashValue uptr_hash = huptr.finalize();
    HashValue sptr_hash = hsptr.finalize();

    // output the results
    std::cout << "  " << type << "\n";
    std::cout << "            normal: " << hash_to_string(val_hash) 
              << "    Trunc: " << hash_to_string(truncate_hash(val_hash, 8)) << "\n"
              << "                                                      Convert: " << convert_hash<size_t>(val_hash) << "\n";

    std::cout << "           raw ptr: " << hash_to_string(ptr_hash)
              << "    Trunc: " << hash_to_string(truncate_hash(ptr_hash, 8)) << "\n"
              << "                                                      Convert: " << convert_hash<size_t>(ptr_hash) << "\n";

    std::cout << "        unique_ptr: " << hash_to_string(uptr_hash)
              << "    Trunc: " << hash_to_string(truncate_hash(uptr_hash, 8)) << "\n"
              << "                                                      Convert: " << convert_hash<size_t>(uptr_hash) << "\n";

    std::cout << "        shared_ptr: " << hash_to_string(sptr_hash)
              << "    Trunc: " << hash_to_string(truncate_hash(sptr_hash, 8)) << "\n"
              << "                                                      Convert: " << convert_hash<size_t>(sptr_hash) << "\n";

    found_hashes_.push_back(std::move(val_hash));
    found_hashes_.push_back(std::move(ptr_hash));
    found_hashes_.push_back(std::move(uptr_hash));
    found_hashes_.push_back(std::move(sptr_hash));
}

std::string MakeTypeStr(const char * container, const char * type)
{
    std::string s(container);
    s += "<";
    s += type;
    s += ">";
    return s;
}

std::string MakeArrayTypeStr(size_t N, const char * type)
{
    std::string s("std::array<");
    s += type;
    s += ",";
    s += std::to_string(N);
    s += ">";
    return s;
}

template<typename T, typename InputIt>
void TestContainer(const char * type, const InputIt first, const InputIt last)
{
    T cont(first, last);
    TestSingle<T>(type, cont);
}

template<typename T, size_t N, typename InputIt>
void TestArray(const char * type, const InputIt first)
{
    std::array<T, N> arr;
    for(size_t i = 0; i < N; i++)
        arr[i] = *(first + i);

    TestSingle<std::array<T, N>>(MakeArrayTypeStr(N, type).c_str(), arr);    
}



template<typename T, typename InputIt>
void TestContainers(const char * type, const InputIt first, const InputIt last)
{
    TestContainer<std::vector<T>>(MakeTypeStr("std::vector", type).c_str(), first, last);
    TestContainer<std::list<T>>(MakeTypeStr("std::list", type).c_str(), first, last);
    TestContainer<std::forward_list<T>>(MakeTypeStr("std::forward_list", type).c_str(), first, last);
    TestContainer<std::set<T>>(MakeTypeStr("std::set", type).c_str(), first, last);
    TestContainer<std::unordered_set<T>>(MakeTypeStr("std::unordered_set", type).c_str(), first, last);

    // arrays have to be handled slightly differently
    auto d = std::distance(first, last);

    switch(d)
    {
        case 1:
            TestArray<T, 1>(type, first);
            break;
        case 2:
            TestArray<T, 2>(type, first);
            break;
        case 3:
            TestArray<T, 3>(type, first);
            break;
        case 4:
            TestArray<T, 4>(type, first);
            break;
        case 5:
            TestArray<T, 5>(type, first);
            break;
        case 6:
            TestArray<T, 6>(type, first);
            break;
        case 7:
            TestArray<T, 7>(type, first);
            break;
        case 8:
            TestArray<T, 8>(type, first);
            break;
    }
}


enum class MyEnumClass
{
    SomeVal1,
    SomeVal2,
    SomeVal3,
    SomeVal4 = 9001
};



struct MyStruct
{
    std::vector<double> i;
    int j;
    std::complex<double> k;

    void hash(Hasher & hasher) const
    {
        hasher(i, j, k);
    }
};




int main(void)
{
    // integer values to test
    std::vector<long> inttest{0,  1,  10,  100,
                                 -1, -10, -100};


    // floating point values to test
    std::vector<double> floattest{0.0, -1.0, 1.0, -1.000001, 1.000001,
                                  -1.0e30, 1.0e30, 1.0e-20, -1.0e-20};

    // strings to test
    std::vector<const char *> strtest{"Hello", "String1", "String2",
                                      "hello", "string1", "string2",
                                      " hello", "string1 ", "strIng2"};

    std::vector<MyEnumClass> eclasstest{MyEnumClass::SomeVal1,
                                        MyEnumClass::SomeVal2,
                                        MyEnumClass::SomeVal3,
                                        MyEnumClass::SomeVal4};

    for(auto i : inttest)
    {
        std::cout << "\nTesting integer value = " << i << "\n";
        TestSingle<unsigned char     >("unsigned char",      i);
        TestSingle<unsigned short    >("unsigned short",     i);
        TestSingle<unsigned int      >("unsigned int",       i);
        TestSingle<unsigned long     >("unsigned long",      i);
        TestSingle<unsigned long long>("unsigned long long", i);

        if(i < 0)
        {
            std::cout << "\nTesting integer value = " << -i << "\n";
            TestSingle<signed char       >("signed char",      i);
            TestSingle<signed short      >("signed short",     i);
            TestSingle<signed int        >("signed int",       i);
            TestSingle<signed long       >("signed long",      i);
            TestSingle<signed long long  >("signed long long", i);
        }
    }

    for(auto i : inttest)
    for(auto j : inttest)
    {
        std::cout << "\nTesting complex integer value = (" << i << ", " << j << ")\n";
        TestSingle<std::complex<unsigned char     >>("std::complex<unsigned char>",      {i,j});
        TestSingle<std::complex<unsigned short    >>("std::complex<unsigned short>",     {i,j});
        TestSingle<std::complex<unsigned int      >>("std::complex<unsigned int>",       {i,j});
        TestSingle<std::complex<unsigned long     >>("std::complex<unsigned long>",      {i,j});
        TestSingle<std::complex<unsigned long long>>("std::complex<unsigned long long>", {i,j});

        if(i < 0 || j < 0)
        {
            TestSingle<std::complex<signed char       >>("std::complex<signed char>",        {i,j});
            TestSingle<std::complex<signed short      >>("std::complex<signed short>",       {i,j});
            TestSingle<std::complex<signed int        >>("std::complex<signed int>",         {i,j});
            TestSingle<std::complex<signed long       >>("std::complex<signed long>",        {i,j});
            TestSingle<std::complex<signed long long  >>("std::complex<signed long long>",   {i,j});
        }
    }

    for(auto i : floattest)
    {
        std::cout << "\nTesting floating point value = " << i << "\n";
        TestSingle<float>("float",      i);
        TestSingle<double>("double",      i);
    }


    for(auto i : floattest)
    for(auto j : floattest)
    {
        std::cout << "\nTesting complex floating point value = (" << i << ", " << j << ")\n";
        TestSingle<std::complex<float>>("std::complex<float>",      {i,j});
        TestSingle<std::complex<double>>("std::complex<double>",    {i,j});
    }


    for(auto i : strtest)
    {
        std::cout << "\nTesting string value = \"" << i << "\"\n";
        TestSingle<std::string>("std::string", i);
        TestSingle<const char *>("const char *", i);
    }
    

    for(auto i : eclasstest)
    {
        std::cout << "\nTesting enum class value " << static_cast<std::underlying_type<MyEnumClass>::type>(i) << "\n";
        TestSingle<MyEnumClass>("MyEnumClass", i);
    }



    for(size_t i = 0; i < inttest.size(); i++)
    {
        std::cout << "\nTesting integer containers of length " << i+1 << "\n";
        TestContainers<unsigned char>("unsigned char", inttest.begin(), inttest.begin() + i + 1);
        TestContainers<unsigned short>("unsigned short", inttest.begin(), inttest.begin() + i + 1);
        TestContainers<unsigned int>("unsigned int", inttest.begin(), inttest.begin() + i + 1);
        TestContainers<unsigned long>("unsigned long", inttest.begin(), inttest.begin() + i + 1);
        TestContainers<unsigned long long>("unsigned long long", inttest.begin(), inttest.begin() + i + 1);

        TestContainers<signed char>("signed char", inttest.begin(), inttest.begin() + i + 1);
        TestContainers<signed short>("signed short", inttest.begin(), inttest.begin() + i + 1);
        TestContainers<signed int>("signed int", inttest.begin(), inttest.begin() + i + 1);
        TestContainers<signed long>("signed long", inttest.begin(), inttest.begin() + i + 1);
        TestContainers<signed long long>("signed long long", inttest.begin(), inttest.begin() + i + 1);
    }


    for(size_t i = 0; i < floattest.size(); i++)
    {
        std::cout << "\nTesting floating point containers of length " << i+1 << "\n";
        TestContainers<float>("float", floattest.begin(), floattest.begin() + i + 1);
        TestContainers<double>("double", floattest.begin(), floattest.begin() + i + 1);
    }

    for(size_t i = 0; i < strtest.size(); i++)
    {
        std::cout << "\nTesting string containers of length " << i+1 << "\n";
        TestContainers<std::string>("std::string", strtest.begin(), strtest.begin() + i + 1);
        TestContainers<const char *>("const char *", strtest.begin(), strtest.begin() + i + 1);
    }

    std::unordered_map<std::string, std::string> mymap {{"Hi", "4" }};
    TestSingle("Map", mymap);
    std::tuple<int, std::string> mytup{4, "HAJSD"};
    TestSingle("MyTuple", mytup);

    std::cout << "\n";
    std::cout << "==============================================================================\n";
    std::cout << "Found " << found_hashes_.size() << " hashes.\n";
    std::sort(found_hashes_.begin(), found_hashes_.end());

    auto dup = std::adjacent_find(found_hashes_.begin(), found_hashes_.end());
    if(dup != found_hashes_.end())
    {
        std::cout << "DUPLICATE FOUND: " << hash_to_string(*dup) << "\n";
        return 1;
    }
    else
        std::cout << "No duplicates found. Hooray!\n";
    

    return 0;
}

