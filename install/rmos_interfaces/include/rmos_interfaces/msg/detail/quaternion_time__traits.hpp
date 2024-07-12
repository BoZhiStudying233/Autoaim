// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rmos_interfaces:msg/QuaternionTime.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__TRAITS_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__TRAITS_HPP_

#include "rmos_interfaces/msg/detail/quaternion_time__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'quaternion_stamped'
#include "geometry_msgs/msg/detail/quaternion_stamped__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rmos_interfaces::msg::QuaternionTime>()
{
  return "rmos_interfaces::msg::QuaternionTime";
}

template<>
inline const char * name<rmos_interfaces::msg::QuaternionTime>()
{
  return "rmos_interfaces/msg/QuaternionTime";
}

template<>
struct has_fixed_size<rmos_interfaces::msg::QuaternionTime>
  : std::integral_constant<bool, has_fixed_size<geometry_msgs::msg::QuaternionStamped>::value> {};

template<>
struct has_bounded_size<rmos_interfaces::msg::QuaternionTime>
  : std::integral_constant<bool, has_bounded_size<geometry_msgs::msg::QuaternionStamped>::value> {};

template<>
struct is_message<rmos_interfaces::msg::QuaternionTime>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__TRAITS_HPP_
