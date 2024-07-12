// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rmos_interfaces:msg/BulletSpeed.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__BUILDER_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__BUILDER_HPP_

#include "rmos_interfaces/msg/detail/bullet_speed__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace rmos_interfaces
{

namespace msg
{

namespace builder
{

class Init_BulletSpeed_speed
{
public:
  Init_BulletSpeed_speed()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rmos_interfaces::msg::BulletSpeed speed(::rmos_interfaces::msg::BulletSpeed::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rmos_interfaces::msg::BulletSpeed msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rmos_interfaces::msg::BulletSpeed>()
{
  return rmos_interfaces::msg::builder::Init_BulletSpeed_speed();
}

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__BUILDER_HPP_
