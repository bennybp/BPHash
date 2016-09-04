/*! \file
 * \brief Hashing of std::unique_ptr and std::shared_ptr
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hasher.hpp"
#include <memory>


namespace bphash {

/*! \brief Create a wrapper around a unique_ptr
 *
 * \param [in] ptr The shared pointer to wrap
 * \param [in] len The number of elements that \p ptr points to
 * \return The wrapped pointer that is ready for hashing
 */
template<typename T, typename Deleter>
PointerWrapper<T> hash_pointer(const std::unique_ptr<T, Deleter> & ptr, size_t len = 1)
{
    return PointerWrapper<T> {ptr.get(), len};
}

/*! \brief Create a wrapper around a unique_ptr
 *
 * \note Since this unique_ptr uses the array syntax, you must specify the length
 *
 * \param [in] ptr The shared pointer to wrap
 * \param [in] len The number of elements that \p ptr points to
 * \return The wrapped pointer that is ready for hashing
 */
template<typename T, typename Deleter>
PointerWrapper<T> hash_pointer(const std::unique_ptr<T[], Deleter> & ptr, size_t len)
{
    return PointerWrapper<T> {ptr.get(), len};
}

/*! \brief Create a wrapper around a shared_ptr
 *
 * \param [in] ptr The shared pointer to wrap
 * \param [in] len The number of elements that \p ptr points to
 * \return The wrapped pointer that is ready for hashing
 */
template<typename T>
PointerWrapper<T> hash_pointer(const std::shared_ptr<T> & ptr, size_t len = 1)
{
    return PointerWrapper<T> {ptr.get(), len};
}


/*
template<typename T>
PointerWrapper<T> hash_pointer(const std::shared_ptr<T[]> & ptr, size_t len)
{
    return PointerWrapper<T>{typeid(std::shared_ptr<T[]>).name(),
                                     ptr.get(), len};
}
*/



/*! \brief Hashing of std::unique_ptr
 *
 * It is assumed that the pointer points to a single element.
 * If not, you must wrap the pointer with hash_pointer.
 */
template<typename T, typename Deleter>
typename std::enable_if<is_hashable<T>::value, void>::type
hash_object(const std::unique_ptr<T, Deleter> & p, Hasher & h)
{
    h(hash_pointer(p)); 
}


/*! \brief Hashing of std::shared_ptr
 *
 * It is assumed that the pointer points to a single element.
 * If not, you must wrap the pointer with hash_pointer.
 */
template<typename T>
typename std::enable_if<is_hashable<T>::value, void>::type
hash_object(const std::shared_ptr<T> & p, Hasher & h)
{
    h(hash_pointer(p)); 
}


} // close namespace bphash

