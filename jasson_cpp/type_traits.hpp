#ifndef JASSON_TYPE_TRAITS_HPP
#define JASSON_TYPE_TRAITS_HPP

#include <type_traits>
#include <variant>
#include <optional>

namespace jasson {

template<typename T, template<typename...> class Template>
struct is_specialization : std::false_type {};

template<template<typename...> class Template, typename... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type {};

template<typename T, template<typename...> class Template>
inline constexpr bool is_specialization_v = is_specialization<T, Template>::value;

template<typename T>
struct always_false : std::false_type {};

template<typename T>
inline constexpr bool always_false_v = always_false<T>::value;

} // namespace jasson

#endif // JASSON_TYPE_TRAITS_HPP
