// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rmos_interfaces:msg/Armor.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__ARMOR__TRAITS_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__ARMOR__TRAITS_HPP_

#include "rmos_interfaces/msg/detail/armor__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'pose'
#include "geometry_msgs/msg/detail/pose__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rmos_interfaces::msg::Armor>()
{
  return "rmos_interfaces::msg::Armor";
}

template<>
inline const char * name<rmos_interfaces::msg::Armor>()
{
  return "rmos_interfaces/msg/Armor";
}

template<>
struct has_fixed_size<rmos_interfaces::msg::Armor>
  : std::integral_constant<bool, has_fixed_size<geometry_msgs::msg::Pose>::value> {};

template<>
struct has_bounded_size<rmos_interfaces::msg::Armor>
  : std::integral_constant<bool, has_bounded_size<geometry_msgs::msg::Pose>::value> {};

template<>
struct is_message<rmos_interfaces::msg::Armor>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RMOS_INTERFACES__MSG__DETAIL__ARMOR__TRAITS_HPP_
