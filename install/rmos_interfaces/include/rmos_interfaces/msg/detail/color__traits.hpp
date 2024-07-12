// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rmos_interfaces:msg/Color.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__COLOR__TRAITS_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__COLOR__TRAITS_HPP_

#include "rmos_interfaces/msg/detail/color__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rmos_interfaces::msg::Color>()
{
  return "rmos_interfaces::msg::Color";
}

template<>
inline const char * name<rmos_interfaces::msg::Color>()
{
  return "rmos_interfaces/msg/Color";
}

template<>
struct has_fixed_size<rmos_interfaces::msg::Color>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rmos_interfaces::msg::Color>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rmos_interfaces::msg::Color>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RMOS_INTERFACES__MSG__DETAIL__COLOR__TRAITS_HPP_
