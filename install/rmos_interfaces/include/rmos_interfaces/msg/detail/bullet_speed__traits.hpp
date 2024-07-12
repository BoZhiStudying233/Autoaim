// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rmos_interfaces:msg/BulletSpeed.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__TRAITS_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__TRAITS_HPP_

#include "rmos_interfaces/msg/detail/bullet_speed__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rmos_interfaces::msg::BulletSpeed>()
{
  return "rmos_interfaces::msg::BulletSpeed";
}

template<>
inline const char * name<rmos_interfaces::msg::BulletSpeed>()
{
  return "rmos_interfaces/msg/BulletSpeed";
}

template<>
struct has_fixed_size<rmos_interfaces::msg::BulletSpeed>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rmos_interfaces::msg::BulletSpeed>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rmos_interfaces::msg::BulletSpeed>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__TRAITS_HPP_
