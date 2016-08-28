#include "test_helpers.hpp"

#include <algorithm>


using namespace bphash;

// A vector containing all the computed hashes
static std::vector<HashValue> found_hashes_;

// In test_integers.cpp
void test_integers(HashType htype,
                   std::vector<HashValue> & found_hashes);


int main(void)
{
    HashType htype = HashType::Hash128;

    test_integers(htype, found_hashes_);

    std::cout << "\n";
    std::cout << "==============================================================================\n";
    std::cout << "Computed " << found_hashes_.size() << " hashes.\n";
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

