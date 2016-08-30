/*! \file
 *
 * \brief Hashing of std::unique_ptr and std::shared_ptr
 * \author Benjamin Pritchard (ben@bennyp.org)
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
detail::PointerWrapper<T> hash_pointer(const std::unique_ptr<T, Deleter> & ptr, size_t len = 1)
{
    return detail::PointerWrapper<T> {typeid(std::unique_ptr<T, Deleter>).name(),
                                      ptr.get(), len
                                     };
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
detail::PointerWrapper<T> hash_pointer(const std::unique_ptr<T[], Deleter> & ptr, size_t len)
{
    return detail::PointerWrapper<T> {typeid(std::unique_ptr<T[], Deleter>).name(),
                                      ptr.get(), len
                                     };
}

/*! \brief Create a wrapper around a shared_ptr
 *
 * \param [in] ptr The shared pointer to wrap
 * \param [in] len The number of elements that \p ptr points to
 * \return The wrapped pointer that is ready for hashing
 */
template<typename T>
detail::PointerWrapper<T> hash_pointer(const std::shared_ptr<T> & ptr, size_t len = 1)
{
    return detail::PointerWrapper<T> {typeid(std::shared_ptr<T>).name(),
                                      ptr.get(), len
                                     };
}


/*
template<typename T>
detail::PointerWrapper<T> hash_pointer(const std::shared_ptr<T[]> & ptr, size_t len)
{
    return detail::PointerWrapper<T>{typeid(std::shared_ptr<T[]>).name(),
                                     ptr.get(), len};
}
*/



/*! \brief Hashing of std::unique_ptr
 *
 * It is assumed that the pointer points to a single element.
 * If not, you must wrap the pointer with hash_pointer.
 */
template<typename T, typename Deleter>
void hash_object( const std::unique_ptr<T, Deleter> & p, Hasher & h)
{
    h(hash_pointer(p)); 
}


/*! \brief Hashing of std::shared_ptr
 *
 * It is assumed that the pointer points to a single element.
 * If not, you must wrap the pointer with hash_pointer.
 */
template<typename T>
void hash_object( const std::shared_ptr<T> & p, Hasher & h)
{
    h(hash_pointer(p)); 
}


} // close namespace bphash

