#include "test_helpers.hpp"

#include <algorithm>


using namespace bphash;


// In test_integers.cpp
void test_integers(HashType htype, std::vector<HashValue> & all_hashes);

// In test_floating.cpp
void test_floating(HashType htype, std::vector<HashValue> & all_hashes);

// In test_string.cpp
void test_string(HashType htype, std::vector<HashValue> & all_hashes);

// In test_tuple.cpp
void test_tuple(HashType htype, std::vector<HashValue> & all_hashes);


int main(void)
{
    // A vector containing all the computed hashes
    std::vector<HashValue> all_hashes;

    test_integers(HashType::Hash128, all_hashes);
    test_floating(HashType::Hash128, all_hashes);
    test_string(HashType::Hash128, all_hashes);
    test_tuple(HashType::Hash128, all_hashes);

    std::cout << "\n";
    std::cout << "==============================================================================\n";
    std::cout << "Created " << all_hashes.size() << " hashes.\n";
    std::sort(all_hashes.begin(), all_hashes.end());

    auto dup = std::adjacent_find(all_hashes.begin(), all_hashes.end());
    if(dup != all_hashes.end())
    {
        std::cout << "DUPLICATE FOUND: " << hash_to_string(*dup) << "\n";
        return 1;
    }
    else
        std::cout << "No duplicates found. Hooray!\n";
    

    return 0;
}

