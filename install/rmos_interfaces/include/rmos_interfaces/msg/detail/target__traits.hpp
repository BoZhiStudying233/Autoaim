// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rmos_interfaces:msg/Target.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__TARGET__TRAITS_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__TARGET__TRAITS_HPP_

#include "rmos_interfaces/msg/detail/target__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'position'
#include "geometry_msgs/msg/detail/point__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rmos_interfaces::msg::Target>()
{
  return "rmos_interfaces::msg::Target";
}

template<>
inline const char * name<rmos_interfaces::msg::Target>()
{
  return "rmos_interfaces/msg/Target";
}

template<>
struct has_fixed_size<rmos_interfaces::msg::Target>
  : std::integral_constant<bool, has_fixed_size<geometry_msgs::msg::Point>::value && has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<rmos_interfaces::msg::Target>
  : std::integral_constant<bool, has_bounded_size<geometry_msgs::msg::Point>::value && has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<rmos_interfaces::msg::Target>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RMOS_INTERFACES__MSG__DETAIL__TARGET__TRAITS_HPP_
