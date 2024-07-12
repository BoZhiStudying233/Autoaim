// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rmos_interfaces:msg/Exp.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__EXP__BUILDER_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__EXP__BUILDER_HPP_

#include "rmos_interfaces/msg/detail/exp__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace rmos_interfaces
{

namespace msg
{

namespace builder
{

class Init_Exp_exp
{
public:
  Init_Exp_exp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rmos_interfaces::msg::Exp exp(::rmos_interfaces::msg::Exp::_exp_type arg)
  {
    msg_.exp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rmos_interfaces::msg::Exp msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rmos_interfaces::msg::Exp>()
{
  return rmos_interfaces::msg::builder::Init_Exp_exp();
}

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__EXP__BUILDER_HPP_
