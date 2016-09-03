/*! \file
 * \brief Trait classes for detecting if something is hashable
 */

/* Copyright (c) 2016 Benjamin Pritchard <ben@bennyp.org>
 * This file is part of the BPHash project, which is released
 * under the BSD 3-clause license. See the LICENSE file for details
 */

#pragma once

#include "bphash/Hasher_fwd.hpp"

namespace bphash {


/*! \brief Trait class that determines if a type is hashable or not
 *
 * Whether or not a type is hashable can be determined via is_hashable<Type>::value
 *
 * This also works with multiple types. If multiple types are specified, \p value is
 * only true if all are hashable.
 */
template<typename ... Targs>
struct is_hashable;




namespace detail {


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


/*! \brief Detects if a type is a PointerWrapper class (and contains a hashable type */
template<typename T>
struct detect_pointer_wrapper : public std::false_type { };


template<typename T>
struct detect_pointer_wrapper<PointerWrapper<T>>
{
    static const bool value = is_hashable<T>::value;
};


} // close namespace detail


template<typename T>
struct is_hashable<T>
{
    using my_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

    static constexpr bool value = std::is_fundamental<T>::value ||
                                  detail::detect_pointer_wrapper<my_type>::value ||
                                  detail::detect_hash_member<my_type>::value ||
                                  detail::detect_hash_free_function<my_type>::value ||
                                  std::is_enum<my_type>::value;
};


template<typename T>
struct is_hashable<T *>
{
    using my_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

    static constexpr bool value = is_hashable<my_type>::value;
};


template<typename T, typename ... Targs>
struct is_hashable<T, Targs...>
{
    static constexpr bool value = is_hashable<T>::value &&
                                  is_hashable<Targs...>::value;
};


} // close namespace bphash

