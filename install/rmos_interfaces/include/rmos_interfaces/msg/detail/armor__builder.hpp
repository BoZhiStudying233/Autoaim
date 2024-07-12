// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rmos_interfaces:msg/Armor.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__ARMOR__BUILDER_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__ARMOR__BUILDER_HPP_

#include "rmos_interfaces/msg/detail/armor__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace rmos_interfaces
{

namespace msg
{

namespace builder
{

class Init_Armor_distance
{
public:
  explicit Init_Armor_distance(::rmos_interfaces::msg::Armor & msg)
  : msg_(msg)
  {}
  ::rmos_interfaces::msg::Armor distance(::rmos_interfaces::msg::Armor::_distance_type arg)
  {
    msg_.distance = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rmos_interfaces::msg::Armor msg_;
};

class Init_Armor_distance_to_image_center
{
public:
  explicit Init_Armor_distance_to_image_center(::rmos_interfaces::msg::Armor & msg)
  : msg_(msg)
  {}
  Init_Armor_distance distance_to_image_center(::rmos_interfaces::msg::Armor::_distance_to_image_center_type arg)
  {
    msg_.distance_to_image_center = std::move(arg);
    return Init_Armor_distance(msg_);
  }

private:
  ::rmos_interfaces::msg::Armor msg_;
};

class Init_Armor_pose
{
public:
  explicit Init_Armor_pose(::rmos_interfaces::msg::Armor & msg)
  : msg_(msg)
  {}
  Init_Armor_distance_to_image_center pose(::rmos_interfaces::msg::Armor::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return Init_Armor_distance_to_image_center(msg_);
  }

private:
  ::rmos_interfaces::msg::Armor msg_;
};

class Init_Armor_num_id
{
public:
  Init_Armor_num_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Armor_pose num_id(::rmos_interfaces::msg::Armor::_num_id_type arg)
  {
    msg_.num_id = std::move(arg);
    return Init_Armor_pose(msg_);
  }

private:
  ::rmos_interfaces::msg::Armor msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rmos_interfaces::msg::Armor>()
{
  return rmos_interfaces::msg::builder::Init_Armor_num_id();
}

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__ARMOR__BUILDER_HPP_
