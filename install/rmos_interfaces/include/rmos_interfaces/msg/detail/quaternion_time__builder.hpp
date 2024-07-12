// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rmos_interfaces:msg/QuaternionTime.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__BUILDER_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__BUILDER_HPP_

#include "rmos_interfaces/msg/detail/quaternion_time__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace rmos_interfaces
{

namespace msg
{

namespace builder
{

class Init_QuaternionTime_timestamp_recv
{
public:
  explicit Init_QuaternionTime_timestamp_recv(::rmos_interfaces::msg::QuaternionTime & msg)
  : msg_(msg)
  {}
  ::rmos_interfaces::msg::QuaternionTime timestamp_recv(::rmos_interfaces::msg::QuaternionTime::_timestamp_recv_type arg)
  {
    msg_.timestamp_recv = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rmos_interfaces::msg::QuaternionTime msg_;
};

class Init_QuaternionTime_quaternion_stamped
{
public:
  Init_QuaternionTime_quaternion_stamped()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_QuaternionTime_timestamp_recv quaternion_stamped(::rmos_interfaces::msg::QuaternionTime::_quaternion_stamped_type arg)
  {
    msg_.quaternion_stamped = std::move(arg);
    return Init_QuaternionTime_timestamp_recv(msg_);
  }

private:
  ::rmos_interfaces::msg::QuaternionTime msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rmos_interfaces::msg::QuaternionTime>()
{
  return rmos_interfaces::msg::builder::Init_QuaternionTime_quaternion_stamped();
}

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__BUILDER_HPP_
