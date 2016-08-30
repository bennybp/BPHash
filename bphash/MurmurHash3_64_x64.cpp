/*! \file
 * \brief MurmurHash3_64_x64 hash implementation (source)
 */

#include "bphash/MurmurHash3_64_x64.hpp"

namespace bphash {
namespace detail {

HashValue MurmurHash3_64_x64::finalize(void)
{
    HashValue hv = MurmurHash3_128_x64::finalize();
    hv.resize(8, 0);
    return hv;
}


} // close namespace detail
} // close namespace bphash

