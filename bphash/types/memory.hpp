/*! \file
 *
 * \brief A class that hashes smart pointer objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__MEMORY_HPP_
#define PULSAR_GUARD_TYPES__MEMORY_HPP_

#include <memory>
#include "bphash/Hasher.hpp"


namespace bphash{



template<typename T, typename Deleter>
detail::PointerWrapper<T> HashPointer(const std::unique_ptr<T, Deleter> & ptr, size_t len = 1)
{
    return detail::PointerWrapper<T>{typeid(std::unique_ptr<T, Deleter>).name(),
                                     ptr.get(), len};
}

// if using array syntax, you must specify the length
template<typename T, typename Deleter>
detail::PointerWrapper<T> HashPointer(const std::unique_ptr<T[], Deleter> & ptr, size_t len)
{
    return detail::PointerWrapper<T>{typeid(std::unique_ptr<T[], Deleter>).name(),
                                     ptr.get(), len};
}

template<typename T>
detail::PointerWrapper<T> HashPointer(const std::shared_ptr<T> & ptr, size_t len = 1)
{
    return detail::PointerWrapper<T>{typeid(std::shared_ptr<T>).name(),
                                     ptr.get(), len};
}


/*
template<typename T>
detail::PointerWrapper<T> HashPointer(const std::shared_ptr<T[]> & ptr, size_t len)
{
    return detail::PointerWrapper<T>{typeid(std::shared_ptr<T[]>).name(),
                                     ptr.get(), len};
}
*/

namespace detail {

template<typename T, typename Deleter>
struct ObjectHasher<std::unique_ptr<T, Deleter>> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const std::unique_ptr<T, Deleter> & obj)
    {
        hasher(HashPointer(obj));
    }
};

template<typename T>
struct ObjectHasher<std::shared_ptr<T>> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const std::shared_ptr<T> & obj)
    {
        hasher(HashPointer(obj));
    }
};

} // close namespace detail


} // close namespace bphash

#endif
