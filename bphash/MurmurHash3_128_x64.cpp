/*! \file
 * \brief MurmurHash3 128-bit x64 hash (source)
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "bphash/MurmurHash3_128_x64.hpp"

//////////////////////////////////////////
// Some small functions for the hash algo
//////////////////////////////////////////
static inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}


static inline uint64_t fmix64 ( uint64_t k )
{
  k ^= k >> 33;
  k *= (0xff51afd7ed558ccdLLU);
  k ^= k >> 33;
  k *= (0xc4ceb9fe1a85ec53LLU);
  k ^= k >> 33;

  return k;
}



namespace bphash {
namespace detail {



////////////////////////////////
// Public functions
////////////////////////////////

MurmurHash3_128_x64::MurmurHash3_128_x64(void)
{
    reset();
}


void MurmurHash3_128_x64::reset(void)
{
    h1_ = h2_ = 0;
    len_ = 0;
    std::fill(buffer_.begin(), buffer_.end(), 0);
    nbuffer_ = 0;
}


void MurmurHash3_128_x64::update(void const * data, size_t nbytes)
{
    if(nbytes == 0)
        return; // got nothing to do

    // cast to an array of bytes
    const uint8_t * data_conv = static_cast<const uint8_t*>(data);

    if(nbuffer_ != 0)
    {
        // we have some leftover data. Add some from the
        // new data and hash the temporary buffer

        // Amount of space left in the buffer?
        size_t nbytes_avail = 16 - nbuffer_;
    
        // How much of data should we actually copy?
        size_t tocopy = std::min(nbytes, nbytes_avail);
    
        // copy it to the end of the buffer
        std::copy(data_conv,
                  data_conv + tocopy,
                  buffer_.begin() + nbuffer_);
    
        // how much do we have in the buffer now?
        nbuffer_ += tocopy;

        // The new number of bytes to do in data
        // This should not underflow since
        // tocopy = std::min(nbytes, ...)
        nbytes -= tocopy;

        // Also advance the data pointer
        data_conv += tocopy;

        // hash the buffer if it is full
        if(nbuffer_ == 16)
        {
            update_block_(buffer_.data(), 1);
            nbuffer_ = 0;
        }
    }

    // now continue hashing the data in place
    size_t nblocks = nbytes / 16;
    update_block_(data_conv, nblocks);  // ok if nblocks == 0

    // advance the pointer and calculate how much is left
    nbytes -= (nblocks * 16);
    data_conv += (nblocks * 16);

    // Leave any remainder in the main buffer
    // (we already know that nbytes < 16, or else
    // the while loop would have kept going)
    if(nbytes != 0)
    {
        std::copy(data_conv, data_conv + nbytes, buffer_.begin());
        nbuffer_ = nbytes;
    }
}


HashValue MurmurHash3_128_x64::finalize(void)
{
    // If we have any left over, we have to do that
    if(nbuffer_ > 0)
    {
        const uint8_t * tail = static_cast<const uint8_t*>(buffer_.data());

        uint64_t k1 = 0;
        uint64_t k2 = 0;

        switch(nbuffer_ & 15)
        {
            case 15: k2 ^= (static_cast<uint64_t>(tail[14])) << 48;
            case 14: k2 ^= (static_cast<uint64_t>(tail[13])) << 40;
            case 13: k2 ^= (static_cast<uint64_t>(tail[12])) << 32;
            case 12: k2 ^= (static_cast<uint64_t>(tail[11])) << 24;
            case 11: k2 ^= (static_cast<uint64_t>(tail[10])) << 16;
            case 10: k2 ^= (static_cast<uint64_t>(tail[ 9])) << 8;
            case  9: k2 ^= (static_cast<uint64_t>(tail[ 8])) << 0;
                     k2 *= c2; k2  = rotl64(k2,33); k2 *= c1; h2_ ^= k2;

            case  8: k1 ^= (static_cast<uint64_t>(tail[ 7])) << 56;
            case  7: k1 ^= (static_cast<uint64_t>(tail[ 6])) << 48;
            case  6: k1 ^= (static_cast<uint64_t>(tail[ 5])) << 40;
            case  5: k1 ^= (static_cast<uint64_t>(tail[ 4])) << 32;
            case  4: k1 ^= (static_cast<uint64_t>(tail[ 3])) << 24;
            case  3: k1 ^= (static_cast<uint64_t>(tail[ 2])) << 16;
            case  2: k1 ^= (static_cast<uint64_t>(tail[ 1])) << 8;
            case  1: k1 ^= (static_cast<uint64_t>(tail[ 0])) << 0;
                     k1 *= c1; k1  = rotl64(k1,31); k1 *= c2; h1_ ^= k1;
        };
    }

    // How much we've done altogether
    len_ += static_cast<size_t>(nbuffer_);

    // Last steps of the hash
    h1_ ^= len_; h2_ ^= len_;

    h1_ += h2_;
    h2_ += h1_;

    h1_ = fmix64(h1_);
    h2_ = fmix64(h2_);

    h1_ += h2_;
    h2_ += h1_;

    // Create the hash object and return
    return HashValue{ static_cast<uint8_t>(h1_),
                      static_cast<uint8_t>(h1_ >> 8),
                      static_cast<uint8_t>(h1_ >> 16),
                      static_cast<uint8_t>(h1_ >> 24),
                      static_cast<uint8_t>(h1_ >> 32),
                      static_cast<uint8_t>(h1_ >> 40),
                      static_cast<uint8_t>(h1_ >> 48),
                      static_cast<uint8_t>(h1_ >> 56),
                      static_cast<uint8_t>(h2_),
                      static_cast<uint8_t>(h2_ >> 8),
                      static_cast<uint8_t>(h2_ >> 16),
                      static_cast<uint8_t>(h2_ >> 24),
                      static_cast<uint8_t>(h2_ >> 32),
                      static_cast<uint8_t>(h2_ >> 40),
                      static_cast<uint8_t>(h2_ >> 48),
                      static_cast<uint8_t>(h2_ >> 56) };
}




////////////////////////////////
// Private member functions
////////////////////////////////
void MurmurHash3_128_x64::update_block_(uint8_t const * data, size_t nblocks)
{
    // This function only does entire 16-byte blocks
    // (passed in through the first parameter)
    const uint64_t * block64 = reinterpret_cast<const uint64_t *>(data);

    for(size_t i = 0; i < nblocks; i++)
    {
        uint64_t k1 = block64[0];
        uint64_t k2 = block64[1];

        k1 *= c1;
        k1  = rotl64(k1, 31);
        k1 *= c2;

        h1_ ^= k1;
        h1_ = rotl64(h1_, 27);
        h1_ += h2_;
        h1_ = h1_*5+0x52dce729;

        k2 *= c2;
        k2  = rotl64(k2, 33);
        k2 *= c1;
        h2_ ^= k2;

        h2_ = rotl64(h2_, 31);
        h2_ += h1_;
        h2_ = h2_*5+0x38495ab5;

        block64 += 2;
    }

    // update how much we've actually hashed
    len_ += nblocks * 16;
}

} // close namespace detail
} // close namespace bphash

