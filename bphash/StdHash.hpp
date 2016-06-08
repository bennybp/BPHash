#ifndef BPHASH_STDHASH_HPP
#define BPHASH_STDHASH_HPP

#include "bphash/Hasher.hpp"

namespace bphash {

/*! \brief A class that can be used in place of std::hash in containers */
template<typename T>
struct StdHash
{
    size_t operator()(const T & obj) const
    {
        //! \todo replace with Hash64 if available?
        Hasher h(HashType::Hash128);
        h(obj);

        HashValue hashval = h.finalize();
        return convert_hash<size_t>(hashval);
    }
};

}

#endif
