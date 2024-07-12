// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rmos_interfaces:msg/Target.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__TARGET__BUILDER_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__TARGET__BUILDER_HPP_

#include "rmos_interfaces/msg/detail/target__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace rmos_interfaces
{

namespace msg
{

namespace builder
{

class Init_Target_position
{
public:
  explicit Init_Target_position(::rmos_interfaces::msg::Target & msg)
  : msg_(msg)
  {}
  ::rmos_interfaces::msg::Target position(::rmos_interfaces::msg::Target::_position_type arg)
  {
    msg_.position = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

class Init_Target_outpost_direction
{
public:
  explicit Init_Target_outpost_direction(::rmos_interfaces::msg::Target & msg)
  : msg_(msg)
  {}
  Init_Target_position outpost_direction(::rmos_interfaces::msg::Target::_outpost_direction_type arg)
  {
    msg_.outpost_direction = std::move(arg);
    return Init_Target_position(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

class Init_Target_timestamp_recv
{
public:
  explicit Init_Target_timestamp_recv(::rmos_interfaces::msg::Target & msg)
  : msg_(msg)
  {}
  Init_Target_outpost_direction timestamp_recv(::rmos_interfaces::msg::Target::_timestamp_recv_type arg)
  {
    msg_.timestamp_recv = std::move(arg);
    return Init_Target_outpost_direction(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

class Init_Target_gun_yaw
{
public:
  explicit Init_Target_gun_yaw(::rmos_interfaces::msg::Target & msg)
  : msg_(msg)
  {}
  Init_Target_timestamp_recv gun_yaw(::rmos_interfaces::msg::Target::_gun_yaw_type arg)
  {
    msg_.gun_yaw = std::move(arg);
    return Init_Target_timestamp_recv(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

class Init_Target_gun_pitch
{
public:
  explicit Init_Target_gun_pitch(::rmos_interfaces::msg::Target & msg)
  : msg_(msg)
  {}
  Init_Target_gun_yaw gun_pitch(::rmos_interfaces::msg::Target::_gun_pitch_type arg)
  {
    msg_.gun_pitch = std::move(arg);
    return Init_Target_gun_yaw(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

class Init_Target_suggest_fire
{
public:
  explicit Init_Target_suggest_fire(::rmos_interfaces::msg::Target & msg)
  : msg_(msg)
  {}
  Init_Target_gun_pitch suggest_fire(::rmos_interfaces::msg::Target::_suggest_fire_type arg)
  {
    msg_.suggest_fire = std::move(arg);
    return Init_Target_gun_pitch(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

class Init_Target_track_state
{
public:
  explicit Init_Target_track_state(::rmos_interfaces::msg::Target & msg)
  : msg_(msg)
  {}
  Init_Target_suggest_fire track_state(::rmos_interfaces::msg::Target::_track_state_type arg)
  {
    msg_.track_state = std::move(arg);
    return Init_Target_suggest_fire(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

class Init_Target_id
{
public:
  explicit Init_Target_id(::rmos_interfaces::msg::Target & msg)
  : msg_(msg)
  {}
  Init_Target_track_state id(::rmos_interfaces::msg::Target::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_Target_track_state(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

class Init_Target_header
{
public:
  Init_Target_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Target_id header(::rmos_interfaces::msg::Target::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Target_id(msg_);
  }

private:
  ::rmos_interfaces::msg::Target msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rmos_interfaces::msg::Target>()
{
  return rmos_interfaces::msg::builder::Init_Target_header();
}

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__TARGET__BUILDER_HPP_
