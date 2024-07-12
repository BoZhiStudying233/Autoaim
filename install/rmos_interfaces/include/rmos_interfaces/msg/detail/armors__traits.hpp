// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rmos_interfaces:msg/Armors.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__ARMORS__TRAITS_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__ARMORS__TRAITS_HPP_

#include "rmos_interfaces/msg/detail/armors__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rmos_interfaces::msg::Armors>()
{
  return "rmos_interfaces::msg::Armors";
}

template<>
inline const char * name<rmos_interfaces::msg::Armors>()
{
  return "rmos_interfaces/msg/Armors";
}

template<>
struct has_fixed_size<rmos_interfaces::msg::Armors>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<rmos_interfaces::msg::Armors>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<rmos_interfaces::msg::Armors>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RMOS_INTERFACES__MSG__DETAIL__ARMORS__TRAITS_HPP_
