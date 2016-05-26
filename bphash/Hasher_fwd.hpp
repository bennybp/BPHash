/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_BPHASH__HASHER_FWD_HPP_
#define PULSAR_GUARD_BPHASH__HASHER_FWD_HPP_


namespace bphash{

class Hasher;
class Hash;


} // close namespace bphash



#ifndef DECLARE_HASHING_FRIENDS

#define DECLARE_HASHING_FRIENDS \
    friend class bphash::Hasher;

#endif


#endif
