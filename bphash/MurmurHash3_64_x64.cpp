/*! \file
 * \brief MurmurHash3 64-bit x64 hash (source)
 */

#include "bphash/MurmurHash3_64_x64.hpp"

namespace bphash {
namespace detail {

HashValue MurmurHash3_64_x64::finalize(void)
{
    HashValue hv = MurmurHash3_128_x64::finalize();
    return truncate_hash(hv, 8);
}


} // close namespace detail
} // close namespace bphash

