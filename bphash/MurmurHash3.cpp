/*! \file
 *
 * \brief MurmurHash3 hash implementation (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bphash/MurmurHash3.hpp"



/////////////////////////////////////////////////////////////////
// The hash algorithm implemented here is 128-bit MurmurHash3,
// by Austin Appleby. It has been placed in the public domain
// by the author.
//
// The code here is adapted from the smhasher project at
// https://github.com/aappleby/smhasher. Mostly, it has
// been converted to a progressive version
//
// No care has been taken to work with different endianness, etc,
// since that is pretty much beyond the scope of the project.
//
// This is not a cryptographic hash, so if you are using it as
// one, you are very, very wrong.
/////////////////////////////////////////////////////////////////


//////////////////////////////////////////
// Some small functions for the hash algo
//////////////////////////////////////////
inline uint64_t rotl64 ( uint64_t x, int8_t r )
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

MurmurHash3::MurmurHash3(void)
{
    reset();
}


void MurmurHash3::reset(void)
{
    h1 = h2 = 0;
    len_ = 0;
    std::fill(buffer_.begin(), buffer_.end(), 0);
    nbuffer_ = 0;
}


void MurmurHash3::update(void const * buffer, size_t size)
{
    if(size == 0)
        return; // got nothing to do

    // cast to an array of bytes
    const uint8_t * data = static_cast<const uint8_t*>(buffer);

    // total number of bytes to do is the amount passed, plus
    // any left over from last time
    size_t ntodo = size + nbuffer_;


    // If we still don't have a full buffer, just append to the
    // buffer and return
    if(ntodo < 16)
    {
        // buffer_.begin() + nbuffer_ represents where we left off last time
        std::copy(data, data + size, buffer_.begin() + nbuffer_);
        nbuffer_ = ntodo;
        return;
    }


    // We actual have to do blocks
    // So copy to the end of the internal buffer
    //
    // dataidx = next place to read from data
    // (which is also where to stop for this initial copy)
    // buffer_.begin() + nbuffer_ represents where we left off last time
    size_t dataidx = 16-nbuffer_;
    std::copy(data, data + dataidx, buffer_.begin() + nbuffer_);


    do
    {
        // Hash what is currently in the buffer
        update_block_();

        // How much do we have left to do?
        // Note: We know from above that, on first iteration, ntodo >= 16, so this
        // should be safe
        ntodo -= 16;

        // Copy the next block to the buffer
        // How much of data should we actually copy?
        size_t tocopy = 16;

        // Is here some left in data that won't fill the buffer?
        if((dataidx + 16) >= size)
            tocopy = size - dataidx;

        // Actually copy the data to the internal buffer
        std::copy(data + dataidx, data + dataidx + tocopy, buffer_.begin());

        // update where we are in the input data
        // (it's ok if this goes past the end, because this loop should be terminating then)
        dataidx += tocopy;

        // How much is in the buffer
        nbuffer_ = ntodo;

    } while(ntodo >= 16);

    // Any remainder/tail will be left in the buffer for next time
}


HashValue MurmurHash3::finalize(void)
{
    // If we have any left over, we have to do that
    if(nbuffer_ > 0)
    {
        const uint8_t * tail = (const uint8_t*)(buffer_.data());

        uint64_t k1 = 0;
        uint64_t k2 = 0;

        switch(nbuffer_ & 15)
        {
            case 15: k2 ^= ((uint64_t)tail[14]) << 48;
            case 14: k2 ^= ((uint64_t)tail[13]) << 40;
            case 13: k2 ^= ((uint64_t)tail[12]) << 32;
            case 12: k2 ^= ((uint64_t)tail[11]) << 24;
            case 11: k2 ^= ((uint64_t)tail[10]) << 16;
            case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
            case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;
                     k2 *= c2; k2  = rotl64(k2,33); k2 *= c1; h2 ^= k2;

            case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;
            case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
            case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
            case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
            case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
            case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
            case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
            case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
                     k1 *= c1; k1  = rotl64(k1,31); k1 *= c2; h1 ^= k1;
        };
    }

    // How much we've done altogether
    len_ += nbuffer_;

    // Last steps of the hash
    h1 ^= len_; h2 ^= len_;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    // Create the hash object and return
    return HashValue{ static_cast<uint8_t>(h1),
                      static_cast<uint8_t>(h1 >> 8),
                      static_cast<uint8_t>(h1 >> 16),
                      static_cast<uint8_t>(h1 >> 24),
                      static_cast<uint8_t>(h1 >> 32),
                      static_cast<uint8_t>(h1 >> 40),
                      static_cast<uint8_t>(h1 >> 48),
                      static_cast<uint8_t>(h1 >> 56),
                      static_cast<uint8_t>(h2),
                      static_cast<uint8_t>(h2 >> 8),
                      static_cast<uint8_t>(h2 >> 16),
                      static_cast<uint8_t>(h2 >> 24),
                      static_cast<uint8_t>(h2 >> 32),
                      static_cast<uint8_t>(h2 >> 40),
                      static_cast<uint8_t>(h2 >> 48),
                      static_cast<uint8_t>(h2 >> 56) };
}


////////////////////////////////
// Private member functions
////////////////////////////////
void MurmurHash3::update_block_(void)
{
    // This function only does an entire 16-byte buffer
    // (that is stored as private member buffer_)
    const uint64_t * block64 = (const uint64_t *)(buffer_.data());

    uint64_t k1 = block64[0];
    uint64_t k2 = block64[1];

    k1 *= c1; k1  = rotl64(k1,31); k1 *= c2; h1 ^= k1;

    h1 = rotl64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

    k2 *= c2; k2  = rotl64(k2,33); k2 *= c1; h2 ^= k2;

    h2 = rotl64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;

    // update how much we've actually hashed
    len_ += 16;
}


} // close namespace detail
} // close namespace bphash

