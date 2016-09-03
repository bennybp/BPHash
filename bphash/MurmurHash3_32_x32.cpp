/*! \file
 * \brief MurmurHash3 32-bit x32 hash (source)
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#include "bphash/MurmurHash3_32_x32.hpp"

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

MurmurHash3_32_x32::MurmurHash3_32_x32(void)
{
    reset();
}


void MurmurHash3_32_x32::reset(void)
{
    h_ = 0;
    len_ = 0;
    std::fill(buffer_.begin(), buffer_.end(), 0);
    nbuffer_ = 0;
}


void MurmurHash3_32_x32::update(void const * data, size_t nbytes)
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
        size_t nbytes_avail = 4 - nbuffer_;
    
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
        if(nbuffer_ == 4)
        {
            update_block_(buffer_.data(), 1);
            nbuffer_ = 0;
        }
    }

    // now continue hashing the data in place
    size_t nblocks = nbytes / 4;
    update_block_(data_conv, nblocks);  // ok if nblocks == 0

    // advance the pointer and calculate how much is left
    nbytes -= (nblocks * 4);
    data_conv += (nblocks * 4);

    // Leave any remainder in the main buffer
    // (we already know that nbytes < 4, or else
    // the while loop would have kept going)
    if(nbytes != 0)
    {
        std::copy(data_conv, data_conv + nbytes, buffer_.begin());
        nbuffer_ = nbytes;
    }
}


HashValue MurmurHash3_32_x32::finalize(void)
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
void MurmurHash3_32_x32::update_block_(uint8_t const * data, size_t nblocks)
{
    // This function only does entire 4-byte blocks
    // (passed in through the first parameter)

    const uint32_t * block32 = reinterpret_cast<const uint32_t *>(data);

    for(size_t i = 0; i < nblocks; i++)
    {
        uint32_t k = block32[i];

        k *= c1;
        k  = rotl32(k, 15);
        k *= c2; 

        h_ ^= k;
        h_ = rotl32(h_, 13);
        h_ = h_*5+0xe6546b64;
    }

    // update how much we've actually hashed
    len_ += 4 * nblocks;
}


} // close namespace detail
} // close namespace bphash

