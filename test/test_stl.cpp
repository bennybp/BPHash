/*! \file
 * \brief Testing of std::hash replacement
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "bphash/Hasher.hpp"
#include "bphash/StdHash.hpp"
#include "bphash/types/All.hpp"

#include <iostream>
#include <string>

using namespace bphash;

int main(void)
{
    typedef std::pair<int, double> key_type;
    
    std::unordered_set<key_type, bphash::StdHash<key_type>> us;
    us.emplace(key_type{0, 10.1});
    us.emplace(key_type{1, 10.2});
    us.emplace(key_type{2, 10.3});
    us.emplace(key_type{3, 10.4});
    us.emplace(key_type{4, 10.5});
    us.emplace(key_type{5, 10.5});
    us.emplace(key_type{5, 10.5});

    std::cout << "\n";
    std::cout << "Elements in unordered set: " << us.size() << "\n";
    for(const auto & it : us)
        std::cout << "  ->  (" << it.first << "  " << it.second << ")\n";

    return 0;
}

