/*! \file
 * \brief MurmurHash3_32 hash implementation (source)
 */

#include "bphash/MurmurHash3_32.hpp"

//////////////////////////////////////////
// Some small functions for the hash algo
//////////////////////////////////////////
static inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}


static inline uint32_t fmix32 ( uint32_t k )
{
    k ^= k >> 16;
    k *= 0x85ebca6b;
    k ^= k >> 13;
    k *= 0xc2b2ae35;
    k ^= k >> 16;

    return k;
}



namespace bphash {
namespace detail {



////////////////////////////////
// Public functions
////////////////////////////////

MurmurHash3_32::MurmurHash3_32(void)
{
    reset();
}


void MurmurHash3_32::reset(void)
{
    h_ = 0;
    len_ = 0;
    std::fill(buffer_.begin(), buffer_.end(), 0);
    nbuffer_ = 0;
}


void MurmurHash3_32::update(void const * buffer, size_t nbytes)
{
    if(nbytes == 0)
        return; // got nothing to do

    // cast to an array of bytes
    const uint8_t * data = static_cast<const uint8_t*>(buffer);

    // total number of bytes to do is the amount passed, plus
    // any left over from last time
    size_t ntodo = nbytes + static_cast<size_t>(nbuffer_);


    // If we still don't have a full buffer, just append to the
    // buffer and return
    if(ntodo < 4)
    {
        // buffer_.begin() + nbuffer_ represents where we left off last time
        std::copy(data, data + nbytes, buffer_.begin() + nbuffer_);
        nbuffer_ = static_cast<int>(ntodo);
        return;
    }


    // We actual have to do blocks
    // So copy to the end of the internal buffer
    //
    // dataidx = next place to read from data
    // (which is also where to stop for this initial copy)
    // buffer_.begin() + nbuffer_ represents where we left off last time
    size_t dataidx = 4 - static_cast<size_t>(nbuffer_);
    std::copy(data, data + dataidx, buffer_.begin() + nbuffer_);


    do
    {
        // Hash what is currently in the buffer
        update_block_();

        // How much do we have left to do?
        // Note: We know from above that, on first iteration, ntodo >= 4, so this
        // should be safe
        ntodo -= 4;

        // Copy the next block to the buffer
        // How much of data should we actually copy?
        size_t tocopy = 4;

        // Is here some left in data that won't fill the buffer?
        if((dataidx + 4) >= nbytes)
            tocopy = nbytes - dataidx;

        // Actually copy the data to the internal buffer
        std::copy(data + dataidx, data + dataidx + tocopy, buffer_.begin());

        // update where we are in the input data
        // (it's ok if this goes past the end, because this loop should be terminating then)
        dataidx += tocopy;

        // How much is in the buffer
        nbuffer_ = static_cast<int>(ntodo);

    } while(ntodo >= 4);

    // Any remainder/tail will be left in the buffer for next time
}


HashValue MurmurHash3_32::finalize(void)
{
    // If we have any left over, we have to do that
    if(nbuffer_ > 0)
    {
        const uint8_t * tail = static_cast<const uint8_t*>(buffer_.data());

        uint32_t k = 0;

        switch(nbuffer_ & 3)
        {
            case  3: k ^= (static_cast<uint32_t>(tail[ 2])) << 16;
            case  2: k ^= (static_cast<uint32_t>(tail[ 1])) << 8;
            case  1: k ^= (static_cast<uint32_t>(tail[ 0])) << 0;
                     k *= c1; k  = rotl32(k,15); k *= c2; h_ ^= k;
        };
    }

    // How much we've done altogether
    len_ += static_cast<size_t>(nbuffer_);

    // Last steps of the hash
    h_ ^= len_;
    h_ = fmix32(h_);

    // Create the hash object and return
    return HashValue{ static_cast<uint8_t>(h_),
                      static_cast<uint8_t>(h_ >> 8),
                      static_cast<uint8_t>(h_ >> 16),
                      static_cast<uint8_t>(h_ >> 24) };
}


////////////////////////////////
// Private member functions
////////////////////////////////
void MurmurHash3_32::update_block_(void)
{
    // This function only does an entire 4-byte buffer
    // (that is stored as private member buffer_)
    uint32_t k = *(reinterpret_cast<const uint32_t *>(buffer_.data()));

    k *= c1;
    k  = rotl32(k, 15);
    k *= c2; 

    h_ ^= k;
    h_ = rotl32(h_, 13);
    h_ = h_*5+0xe6546b64;

    // update how much we've actually hashed
    len_ += 4;
}


} // close namespace detail
} // close namespace bphash

