// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rmos_interfaces:msg/Armors.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__ARMORS__BUILDER_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__ARMORS__BUILDER_HPP_

#include "rmos_interfaces/msg/detail/armors__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace rmos_interfaces
{

namespace msg
{

namespace builder
{

class Init_Armors_is_rune
{
public:
  explicit Init_Armors_is_rune(::rmos_interfaces::msg::Armors & msg)
  : msg_(msg)
  {}
  ::rmos_interfaces::msg::Armors is_rune(::rmos_interfaces::msg::Armors::_is_rune_type arg)
  {
    msg_.is_rune = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rmos_interfaces::msg::Armors msg_;
};

class Init_Armors_armors
{
public:
  explicit Init_Armors_armors(::rmos_interfaces::msg::Armors & msg)
  : msg_(msg)
  {}
  Init_Armors_is_rune armors(::rmos_interfaces::msg::Armors::_armors_type arg)
  {
    msg_.armors = std::move(arg);
    return Init_Armors_is_rune(msg_);
  }

private:
  ::rmos_interfaces::msg::Armors msg_;
};

class Init_Armors_header
{
public:
  Init_Armors_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Armors_armors header(::rmos_interfaces::msg::Armors::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Armors_armors(msg_);
  }

private:
  ::rmos_interfaces::msg::Armors msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rmos_interfaces::msg::Armors>()
{
  return rmos_interfaces::msg::builder::Init_Armors_header();
}

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__ARMORS__BUILDER_HPP_
