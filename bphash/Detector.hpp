/*! \file
 * \brief Detection of member functions and free functions
 */

#pragma once

#include "bphash/Hasher_fwd.hpp"

namespace bphash {


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
      static const bool value = std::is_same<decltype(test<T>(0)), Yes>::value;
};


/*! \brief Detects there is a free function that will hash an object
 *
 * This check is a little lax with respect to the signature of the
 * free function, nor does the hasher have to be non-const,
 * for example.
 *
 * The first argument to `hash_object` must be const, though.
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
      static const bool value = std::is_same<decltype(test<T>(0)), Yes>::value;
};


/*! \brief Detects if a type is a PointerWrapper class */
template<typename T>
class detect_pointer_wrapper : public std::false_type { };


template<typename T>
class detect_pointer_wrapper<PointerWrapper<T>> : public std::true_type { };

} // close namespace bphash

