/*! \file
 * \brief MurmurHash3 32-bit x64 hash (source)
 */

#include "bphash/MurmurHash3_32_x64.hpp"

namespace bphash {
namespace detail {

HashValue MurmurHash3_32_x64::finalize(void)
{
    HashValue hv = MurmurHash3_128_x64::finalize();
    return truncate_hash(hv, 4);
}


} // close namespace detail
} // close namespace bphash

