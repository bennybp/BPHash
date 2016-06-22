/*! \file
 *
 * \brief A class that hashes objects (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPHASH_GUARD_HASHER_HPP_
#define BPHASH_GUARD_HASHER_HPP_

#include "bphash/HashImpl.hpp"


#include <typeinfo>
#include <cstring>  // for strlen
#include <memory>



namespace bphash {

namespace detail {


/*! \brief Hashing of a particular type of object
 * Object hashers tell how to hash a single type of object (for example, an
 * STL vector).
 *
 * The base structure is empty and derives from std::false_type to signify that,
 * in general, objects can't be hashed. If it is possible to hash a type, the
 * ObjectHasher should be specialized and derived from std::true_type.
 *
 * \tparam T Type of object that can be hashed by this structure
 */
template<typename T> struct ObjectHasher : public std::false_type { };


/*! \brief Wrapper for pointers and arrays
 *
 * This wraps any kind of pointer, including arrays, and stores
 * its type information and length. These values are then hashed
 * by an appropriate ObjectHasher.
 */
template<typename T>
struct PointerWrapper
{
    const char * type;  //!< Type of data stored (from typeinfo)
    const T * ptr;      //!< Pointer to the actual data
    size_t len;         //!< Length of the array (as number of elements of \p T)
};

} // close namespace detail





/*! \brief Type of hash to use */
enum class HashType
{
    Hash64,
    Hash128
};



/*! \brief Class that is used to hash objects
 *
 * Data is added via operator() or directly by
 * add_data. The final hash is obtained via a
 * call to finalize().
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
         * Used to terminate variadic template
         */
        void operator()(void) { }


        /*! \brief Add an object to the hash
         *
         * Objects are progressively hashed until finalize() is called
         */
        template<typename T, typename... Targs>
        void operator()(const T & obj, const Targs &... objs)
        {
            // hash the type of the object
            const char * typestr = typeid(T).name();
            size_t len = strlen(typestr);
            add_data(static_cast<void const *>(&len), sizeof(len));
            add_data(static_cast<void const *>(typestr), len);

            // Now hash the object itself
            hash_single_(obj);

            // and the rest
            (*this)(objs...);
        }


        /*! \brief Add raw data to the hash
         *
         * \param [in] data Pointer to the data to hash
         * \param [in] size Number of bytes of \p data to hash
         */
        void add_data(void const * data, size_t size)
        {
            hashimpl_->update(data, size);
        }


        /*! \brief Perform any remaining steps and return the hash
         */
        HashValue finalize(void)
        {
            return hashimpl_->finalize();
        }


    private:

        /*! \brief Hash a single object
         *
         * Enabled if the type is an enum type
         */
        template<typename T>
        typename std::enable_if<std::is_enum<T>::value, void>::type
        hash_single_(const T & obj)
        {
            hash_single_(static_cast<typename std::underlying_type<T>::type>(obj));
        }


        /*! \brief Hash a single object
         *
         * Enabled only if we have an ObjectHasher structure with the
         * appropriate type
         */
        template<typename T>
        typename std::enable_if<detail::ObjectHasher<T>::value, void>::type
        hash_single_(const T & obj)
        {
            detail::ObjectHasher<T>::hash(*this, obj);
        }


        /*! \brief Hash a single object
         *
         * Enabled if the object has a void hash(Hasher &) const member function
         */
        template<typename T>
        auto hash_single_(const T & obj)
        -> decltype(static_cast<void (T::*)(Hasher &) const>(&T::hash), void())
        {
            obj.hash(*this);
        }


        //! Internal hasher object
        std::unique_ptr<detail::HashImpl> hashimpl_;
};



/*! \brief Convenience function for hashing objects in a single function call
 *
 * This can be used to easily obtain the hash of several objects at once without
 * needing to manually create a Hasher object, etc.
 *
 * \param [in] type The type of hash to use
 * \param [in] args Objects to hash
 */
template<typename ... Targs>
HashValue make_hash(HashType type, const Targs &... args)
{
    Hasher hasher(type);
    hasher(args...);
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
 */
template<typename InputIterator>
HashValue MakeHashRange(HashType type, InputIterator first, InputIterator last)
{
    Hasher hasher(type);

    for(auto it = first; it != last; ++it)
        hasher(*it);

    return hasher.finalize();
}




//////////////////////////////////////////
// Helper for pointers
//////////////////////////////////////////

/*! \brief Wrap a raw pointer so that it can be hashed
 *
 * \param [in] ptr Pointer to be hashed
 * \param [in] len Number of elements of \p T that \p ptr points to
 * \return The pointer information wrapped in PointerWrapper
 */
template<typename T>
detail::PointerWrapper<T> HashPointer(const T * ptr, size_t len = 1)
{
    return detail::PointerWrapper<T> {typeid(T *).name(), ptr, len};
}



namespace detail {

/*! \brief Hashing of a PointerWrapper */
template<typename T>
struct ObjectHasher<PointerWrapper<T>> : public std::true_type
{
    static void
    hash(Hasher & hasher, const PointerWrapper<T> & obj)
    {
        if(obj.ptr != nullptr)
        {
            hasher(obj.len);

            for(size_t i = 0; i < obj.len; i++)
                hasher(obj.ptr[i]);
        }
        else
            hasher(0);
    }
};


/*! \brief Hashing of a raw pointer
 *
 * It is assumed that the pointer points to a single element.
 * If there is more than one element, you must wrap the pointer in
 * a PointerWrapper manually (via HashPointer).
 */
template<typename T>
struct ObjectHasher<T *> : public std::true_type
{
    static void
    hash(Hasher & hasher, const T * obj)
    {
        hasher(HashPointer(obj, 1));
    }
};


/*! \brief Hashing of a C-style string
 *
 * It is assumed that the pointer points to a null-terminated
 * array of characters. If not, you must wrap the pointer in
 * a PointerWrapper manually (via HashPointer).
 */
template<>
struct ObjectHasher<const char *> : public std::true_type
{
    static void
    hash(Hasher & hasher, const char * obj)
    {
        hasher(HashPointer(obj, strlen(obj)));
    }
};



//////////////////////////////////////////
// Hashing of arithmetic types
//////////////////////////////////////////
#define DECLARE_FUNDAMENTAL_HASHER(type) \
    template<> struct ObjectHasher<type> : public std::true_type \
    {\
        static void hash(Hasher & hasher, const type & obj) \
        {\
            hasher.add_data(static_cast<void const *>(&obj), \
                            static_cast<size_t>(sizeof(type))); \
        }\
    };

DECLARE_FUNDAMENTAL_HASHER(bool)
DECLARE_FUNDAMENTAL_HASHER(char)
DECLARE_FUNDAMENTAL_HASHER(unsigned char)
DECLARE_FUNDAMENTAL_HASHER(signed char)
DECLARE_FUNDAMENTAL_HASHER(unsigned short)
DECLARE_FUNDAMENTAL_HASHER(signed short)
DECLARE_FUNDAMENTAL_HASHER(unsigned int)
DECLARE_FUNDAMENTAL_HASHER(signed int)
DECLARE_FUNDAMENTAL_HASHER(unsigned long)
DECLARE_FUNDAMENTAL_HASHER(signed long)
DECLARE_FUNDAMENTAL_HASHER(unsigned long long)
DECLARE_FUNDAMENTAL_HASHER(signed long long)

DECLARE_FUNDAMENTAL_HASHER(float)
DECLARE_FUNDAMENTAL_HASHER(double)
DECLARE_FUNDAMENTAL_HASHER(long double)

#undef DECLARE_FUNDAMENTAL_HASHER

} // close namespace detail
} // close namespace bphash



#endif
