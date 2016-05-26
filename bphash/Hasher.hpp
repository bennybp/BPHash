/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_BPHASH__HASHER_HPP_
#define PULSAR_GUARD_BPHASH__HASHER_HPP_

#include <typeinfo>
#include <cstring>

#include "bphash/HashImpl.hpp"
#include "bphash/Hash.hpp"
#include "bphash/Hasher_fwd.hpp" // for DECLARE_HASHING_FRIENDS


namespace bphash {



namespace detail {


// Object hashers tell how to hash a single type of object (for example, an
// STL vector). They derive from std::true_type if it is valid to hash
// that type of object. The base structure is empty and derives
// from false_type to signify a type that can't be hashed.
template<typename T> struct ObjectHasher : public std::false_type { };

// Struct that wraps pointers (particularly arrays)
template<typename T>
struct PointerWrapper
{
    const char * type;
    const T * ptr;
    size_t len;
};

} // close namespace detail



/*! \brief Class that is used to hash objects
 *
 * This prevents the need for serializing the whole
 * object into memory, and then hashing. Instead, hasing
 * is done progressively.
 */
class Hasher
{
    public:
        Hasher() = default;

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
            add_data(reinterpret_cast<void const *>(&len), sizeof(len));
            add_data(reinterpret_cast<void const *>(typestr), len);

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
            hashimpl_.update(data, size);
        }


        /*! \brief Perform any remaining steps and return the hash
         */
        Hash finalize(void) { return hashimpl_.finalize(); }


    private:

        // Hash a single object
        // Enabled if the type is an enum type
        template<typename T>
        typename std::enable_if<std::is_enum<T>::value, void>::type
        hash_single_(const T & obj)
        {
            hash_single_(static_cast<typename std::underlying_type<T>::type>(obj));
        }

        // Hash a single object
        // Enabled only if we have an ObjectHasher structure with the
        // appropriate type
        template<typename T>
        typename std::enable_if<detail::ObjectHasher<T>::value, void>::type
        hash_single_(const T & obj)
        {
            detail::ObjectHasher<T>::Hash(*this, obj);
        }


        // Hash a single object
        // Enabled if the object has a void hash(Hasher &) const member function
        template<typename T>
        auto hash_single_(const T & obj)
             -> decltype(static_cast<void (T::*)(Hasher &) const>(&T::hash), void())
        {
            obj.hash(*this);
        }


        // Internal hasher object
        HashImpl hashimpl_;
};

/*! \brief Convenience function for hashing stuff in a single function call
 */
template<typename ... Targs>
Hash MakeHash(const Targs &... args)
{
    Hasher hasher;
    hasher(args...);
    return hasher.finalize();
}


/*! \brief Convenience function for a range from a container
 */
template<typename InputIterator>
Hash MakeHashRange(InputIterator first, InputIterator last)
{
    Hasher hasher;

    for(auto it = first; it != last; ++it)
        hasher(*it);
    return hasher.finalize();
}


//////////////////////////////////////////
// Helper for pointers
//////////////////////////////////////////


/*! \brief Wrap a raw pointer so that it can be hashed */
template<typename T>
detail::PointerWrapper<T> HashPointer(const T * ptr, size_t len = 1)
{
    return detail::PointerWrapper<T>{typeid(T *).name(), ptr, len};
}



namespace detail {


//////////////////////////////////////////
// Hashing pointers and the pointer wrapper
//////////////////////////////////////////
template<typename T>
struct ObjectHasher<PointerWrapper<T>> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const PointerWrapper<T> & obj)
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

template<typename T>
struct ObjectHasher<T *> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const T * obj)
    {
        hasher(HashPointer(obj, 1));
    }
};


// specialization for const char
template<>
struct ObjectHasher<const char *> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const char * obj)
    {
        hasher(HashPointer(obj, strlen(obj)));
    }
};



//////////////////////////////////////////
// Arithmetic types
//////////////////////////////////////////
template<typename T>
struct FundamentalHasher : public std::true_type
{
    static void
    Hash(Hasher & hasher, const T & obj)
    {
        hasher.add_data(reinterpret_cast<void const *>(&obj),
                        static_cast<size_t>(sizeof(T)));
    }
};

#define DECLARE_FUNDAMENTAL_HASHER(type) \
    template<> struct ObjectHasher<type> : public FundamentalHasher<type> { };

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
