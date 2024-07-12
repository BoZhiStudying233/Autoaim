// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rmos_interfaces:msg/Color.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__COLOR__BUILDER_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__COLOR__BUILDER_HPP_

#include "rmos_interfaces/msg/detail/color__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace rmos_interfaces
{

namespace msg
{

namespace builder
{

class Init_Color_color
{
public:
  Init_Color_color()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rmos_interfaces::msg::Color color(::rmos_interfaces::msg::Color::_color_type arg)
  {
    msg_.color = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rmos_interfaces::msg::Color msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rmos_interfaces::msg::Color>()
{
  return rmos_interfaces::msg::builder::Init_Color_color();
}

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__COLOR__BUILDER_HPP_
