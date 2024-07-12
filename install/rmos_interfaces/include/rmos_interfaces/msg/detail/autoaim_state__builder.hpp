// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rmos_interfaces:msg/AutoaimState.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__AUTOAIM_STATE__BUILDER_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__AUTOAIM_STATE__BUILDER_HPP_

#include "rmos_interfaces/msg/detail/autoaim_state__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace rmos_interfaces
{

namespace msg
{

namespace builder
{

class Init_AutoaimState_autoaim_state
{
public:
  Init_AutoaimState_autoaim_state()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rmos_interfaces::msg::AutoaimState autoaim_state(::rmos_interfaces::msg::AutoaimState::_autoaim_state_type arg)
  {
    msg_.autoaim_state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rmos_interfaces::msg::AutoaimState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rmos_interfaces::msg::AutoaimState>()
{
  return rmos_interfaces::msg::builder::Init_AutoaimState_autoaim_state();
}

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__AUTOAIM_STATE__BUILDER_HPP_
