/*! \file
 * \brief A class that hashes objects (header)
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/HashImpl.hpp"
#include "bphash/Detector.hpp"


#include <typeinfo>
#include <cstring>  // for strlen
#include <memory>


namespace bphash {


/*! \brief Type of hash to use */
enum class HashType
{
    Hash32,     //!< Default 32-bit hash
    Hash64,     //!< Default 64-bit hash
    Hash128,    //!< Default 128-bit hash

    Hash32_x32,   //!< Default 32-bit hash for x32
    Hash64_x32,   //!< Default 64-bit hash for x32
    Hash128_x32,  //!< Default 128-bit hash for x32

    Hash32_x64,   //!< Default 32-bit hash for x86-64
    Hash64_x64,   //!< Default 64-bit hash for x86-64
    Hash128_x64,  //!< Default 128-bit hash for x86-64
};



/*! \brief Wrapper for pointers and arrays
 *
 * This wraps any kind of pointer, including arrays, and stores
 * its type information and length. These values are then hashed
 * by an appropriate free function or member function
 */
template<typename T>
struct PointerWrapper
{
    const T * ptr;      //!< Pointer to the actual data
    size_t len;         //!< Length of the array (as number of elements of \p T)
};



/*! \brief Wrap a raw pointer so that it can be hashed
 *
 * Small helper function to help create PointerWrapper objects
 * more cleanly.
 *
 * \param [in] ptr Pointer to be hashed
 * \param [in] len Number of elements of \p T that \p ptr points to
 * \return The pointer information wrapped in PointerWrapper
 */
template<typename T>
PointerWrapper<T> hash_pointer(const T * ptr, size_t len = 1)
{
    return PointerWrapper<T>{ptr, len};
}



/*! \brief Class that is used to hash objects
 *
 * Data is added via operator(). 
 */
class Hasher
{
    public:
        /*! \brief Constructor
         *
         * \param[in] type Type of hasher to use
         */
        Hasher(HashType type);

        // not copyable or assignable
        Hasher(const Hasher &)             = delete;
        Hasher & operator=(const Hasher &) = delete;
        Hasher(Hasher &&)                  = default;
        Hasher & operator=(Hasher &&)      = default;


        /*! \brief Add an object to the hash
         *
         * Objects are progressively hashed until finalize() is called
         *
         * This overload is used to terminate the variadic template
         */
        void operator()(void) { }


        /*! \brief Add an object to the hash
         *
         * Objects are progressively hashed until finalize() is called
         */
        template<typename T, typename... Targs>
        void operator()(const T & obj, const Targs &... objs)
        {
            static_assert(is_hashable<T>::value,
             "\n\n"
             "  ***  Object is not hashable. Did you remember to include the correct header       ***\n"
             "  ***  (such as <bphash/types/string.hpp>) or to declare a hash member function or  ***\n"
             "  ***  free function?                                                               ***\n");


            // Hash the object itself
            hash_single_(obj);

            // Now hash the type of the object
            const char * typestr = typeid(T).name();
            size_t len = strlen(typestr);
            hashimpl_->update(typestr, len);

            // and the rest
            (*this)(objs...);
        }


        /*! \brief Perform any remaining steps and return the hash */
        HashValue finalize(void)
        {
            return hashimpl_->finalize();
        }


    private:

        //! Internal hasher object
        std::unique_ptr<detail::HashImpl> hashimpl_;


        /*! \brief Hash a single fundamental type */
        template<typename T>
        typename std::enable_if<std::is_fundamental<T>::value, void>::type
        hash_single_(const T & obj)
        {
            size_t size = sizeof(T);
            hashimpl_->update(&size, sizeof(size_t));
            hashimpl_->update(&obj, size);
        }

        /*! \brief Hash a single enum object */
        template<typename T>
        typename std::enable_if<std::is_enum<T>::value, void>::type
        hash_single_(const T & obj)
        {
            hash_single_(static_cast<typename std::underlying_type<T>::type>(obj));
        }


        /*! \brief Hash an object with an appropriate `hash()` member function */
        template<typename T>
        auto hash_single_(const T & obj)
        -> decltype(static_cast<void (T::*)(Hasher &) const>(&T::hash), void())
        {
            obj.hash(*this);
        }


        /*! \brief Hash an object with an appropriate `hash_object()` free function */
        template<typename T>
        auto hash_single_(const T & obj)
        -> decltype( hash_object( std::declval<const T &>(), std::declval<Hasher &>() ), void())
        {
            hash_object(obj, *this);
        }


        /*! \brief Hash a raw pointer
         *
         * It is assumed that the pointer only points to one element
         */
        template<typename T>
        void hash_single_(const T * const obj)
        {
            hash_single_(hash_pointer(obj, 1));
        }


        /*! \brief Hash a wrapped pointer  to a fundamental type */
        template<typename T>
        typename std::enable_if<std::is_fundamental<T>::value, void>::type
        hash_single_(const PointerWrapper<T> & pw)
        {
            if(pw.ptr != nullptr)
            {
                // we add the data first, then the size
                hashimpl_->update(pw.ptr, pw.len * sizeof(T));
                hashimpl_->update(&pw.len, sizeof(pw.len));
            }
            else
            {
                size_t n = 0;
                hashimpl_->update(&n, sizeof(size_t));
            }
        }

        /*! \brief Hash a wrapped pointer that does not point to a fundamental type */
        template<typename T>
        typename std::enable_if<!std::is_fundamental<T>::value, void>::type
        hash_single_(const PointerWrapper<T> & pw)
        {
            if(pw.ptr != nullptr)
            {
                // we add the data first, then the size
                for(size_t i = 0; i < pw.len; i++)
                    hash_single_(pw.ptr[i]); 

                hashimpl_->update(&pw.len, sizeof(pw.len));
            }
            else
            {
                size_t n = 0;
                hashimpl_->update(&n, sizeof(size_t));
            }
        }


        /*! \brief Hashing of a C-style string
         *
         * It is assumed that the pointer points to a null-terminated
         * array of characters. If not, you must wrap the pointer
         * with hash_pointer.
         */
        void hash_single_(const char * p)
        {
            (*this)(hash_pointer(p, strlen(p)));
        }
};



/*! \brief Convenience function for hashing objects in a single function call
 *
 * This can be used to easily obtain the hash of several objects at once without
 * needing to manually create a Hasher object, etc.
 *
 * \param [in] type The type of hash to use
 * \param [in] objs Objects to hash
 * \return Hash of the given data
 */
template<typename ... Targs>
HashValue make_hash(HashType type, const Targs &... objs)
{
    Hasher hasher(type);
    hasher(objs...);
    return hasher.finalize();
}


/*! \brief Convenience function hashing selected elements of a container
 *
 * This can be used to easily obtain the hash of a range of objects
 * in a container (given via two iterators).
 *
 * Objects in the range [first, last) are hashed (generally the same convention
 * as the STL)
 *
 * \param [in] type The type of hash to use
 * \param [in] first An iterator of the first object to hash
 * \param [in] last An iterator of the element following the last element to hash
 * \return Hash of the given data
 */
template<typename InputIterator>
HashValue make_hash_range(HashType type, InputIterator first, InputIterator last)
{
    Hasher hasher(type);

    for(auto it = first; it != last; ++it)
        hasher(*it);

    return hasher.finalize();
}


} // close namespace bphash

