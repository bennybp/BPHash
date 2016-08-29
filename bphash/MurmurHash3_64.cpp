/*! \file
 * \brief MurmurHash3_64 hash implementation (source)
 */

#include "bphash/MurmurHash3_64.hpp"

namespace bphash {
namespace detail {

HashValue MurmurHash3_64::finalize(void)
{
    HashValue hv = MurmurHash3_128::finalize();
    hv.resize(8, 0);
    return hv;
}


} // close namespace detail
} // close namespace bphash

