/*! \file
 * \brief A class that hashes objects (header)
 */

#pragma once

#include "bphash/HashImpl.hpp"

#include <typeinfo>
#include <cstring>  // for strlen
#include <memory>


namespace bphash {


// forward declaration
class Hasher;


namespace detail {


/*! \brief Wrapper for pointers and arrays
 *
 * This wraps any kind of pointer, including arrays, and stores
 * its type information and length. These values are then hashed
 * by an appropriate free function or member function
 */
template<typename T>
struct PointerWrapper
{
    const char * type;  //!< Type of data stored (from typeinfo)
    const T * ptr;      //!< Pointer to the actual data
    size_t len;         //!< Length of the array (as number of elements of \p T)
};



/*! \brief Detects if a class has a hash() member function with the
 *         appropriate signature
 */
template <typename T>
class detect_hash_member
{
    private:
        using Yes = std::true_type;
        using No = std::false_type;

        template<typename C> static auto test(void*)
                -> decltype(static_cast<void (C::*)(Hasher &) const>(&C::hash), Yes{});

  template<typename> static No & test(...);

  public:
      static bool const value = std::is_same<decltype(test<T>(0)), Yes>::value;
};


/*! \brief Detects there is a free function that will hash an object
 */
template <typename T>
class detect_hash_free_function
{
    private:
        using Yes = std::true_type;
        using No = std::false_type;

        template<typename C> static auto test(void*)
                -> decltype( hash_object( std::declval<const C &>(), std::declval<Hasher &>() ), Yes{});

  template<typename> static No & test(...);

  public:
      static bool const value = std::is_same<decltype(test<T>(0)), Yes>::value;
};


} // close namespace detail





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


/*! \brief Trait class that determines if a type is hashable or not
 *
 * Whether or not a type is hashable can be determined via is_hashable<Type>::value
 *
 * This also works with multiple types. If multiple types are specified, \p value is
 * only true if all are hashable.
 */
template<typename ... Targs>
struct is_hashable;


template<typename T>
struct is_hashable<T>
{
    using my_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

    static constexpr bool value = detail::detect_hash_member<my_type>::value ||
                                  detail::detect_hash_free_function<my_type>::value ||
                                  std::is_enum<my_type>::value;
};

template<typename T, typename ... Targs>
struct is_hashable<T, Targs...>
{
    static constexpr bool value = is_hashable<T>::value &&
                                  is_hashable<Targs...>::value;
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
            static_assert(is_hashable<T>::value,
                          "\n\n"
                          "  ***  Object is not hashable. Did you remember to include the correct header       ***\n"
                          "  ***  (such as <bphash/types/string.hpp>) or to declare a hash member function or  ***\n"
                          "  ***  free function?                                                               ***\n");

            // hash the type of the object
            const char * typestr = typeid(T).name();
            size_t len = strlen(typestr);
            add_data(static_cast<void const *>(typestr), len);

            // Now hash the object itself
            hash_single_(obj);

            // and the rest
            (*this)(objs...);
        }


        /*! \brief Add raw data to the hash
         *
         * \param [in] data Pointer to the data to hash
         * \param [in] nbytes Number of bytes of \p data to hash
         */
        void add_data(void const * data, size_t nbytes)
        {
            hashimpl_->update(data, nbytes);
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
         * Enabled if the object has a `void hash(Hasher &) const` member function
         */
        template<typename T>
        auto hash_single_(const T & obj)
        -> decltype(static_cast<void (T::*)(Hasher &) const>(&T::hash), void())
        {
            obj.hash(*this);
        }


        /*! \brief Hash a single object
         *
         * Enabled if there is a free function with a signature
         * `void hash_object(const T &, Hasher &)`
         */
        template<typename T>
        auto hash_single_(const T & obj)
        -> decltype( hash_object( std::declval<const T &>(), std::declval<Hasher &>() ), void())
        {
            hash_object(obj, *this);
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
HashValue make_hash_range(HashType type, InputIterator first, InputIterator last)
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
detail::PointerWrapper<T> hash_pointer(const T * ptr, size_t len = 1)
{
    return detail::PointerWrapper<T> {typeid(T *).name(), ptr, len};
}


/*! \brief Hashing of a PointerWrapper */
template<typename T>
void hash_object(const detail::PointerWrapper<T> & pw, Hasher & h)
{
    if(pw.ptr != nullptr)
    {
        h.add_data(&pw.len, sizeof(pw.len));
        h.add_data(pw.ptr, pw.len);
    }
    else
    {
        size_t n = 0;
        h.add_data(&n, sizeof(size_t));
    }
}


/*! \brief Hashing of a raw pointer
 *
 * It is assumed that the pointer points to a single element.
 * If there is more than one element, you must wrap the pointer
 * with hash_pointer.
 */
template<typename T>
void hash_object(const T * p, Hasher & h)
{
    h(hash_pointer(p, 1));
}


/*! \brief Hashing of a C-style string
 *
 * It is assumed that the pointer points to a null-terminated
 * array of characters. If not, you must wrap the pointer
 * with hash_pointer.
 */
inline void hash_object(const char * p, Hasher & h)
{
    h(hash_pointer(p, strlen(p)));
}



//////////////////////////////////////////
// Hashing of arithmetic types
//////////////////////////////////////////
#define DECLARE_FUNDAMENTAL_HASHER(type) \
    inline void hash_object(const type & a, Hasher & h) \
    {\
        h.add_data(static_cast<void const *>(&a), \
                   static_cast<size_t>(sizeof(type))); \
    }\

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


} // close namespace bphash



/*! \brief Macro for declaring certain functions as friends
 *
 * This can be used for classes where the hashing functions are
 * kept protected or private.
 */
#define DECLARE_HASHING_FRIENDS \
    friend class bphash::Hasher;\
    template<typename T> friend class bphash::detail::detect_hash_member;

