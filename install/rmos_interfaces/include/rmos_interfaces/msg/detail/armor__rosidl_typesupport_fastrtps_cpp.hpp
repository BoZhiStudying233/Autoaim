// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from rmos_interfaces:msg/Armor.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__ARMOR__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__ARMOR__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "rmos_interfaces/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "rmos_interfaces/msg/detail/armor__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace rmos_interfaces
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rmos_interfaces
cdr_serialize(
  const rmos_interfaces::msg::Armor & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rmos_interfaces
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  rmos_interfaces::msg::Armor & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rmos_interfaces
get_serialized_size(
  const rmos_interfaces::msg::Armor & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rmos_interfaces
max_serialized_size_Armor(
  bool & full_bounded,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace rmos_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rmos_interfaces
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, rmos_interfaces, msg, Armor)();

#ifdef __cplusplus
}
#endif

#endif  // RMOS_INTERFACES__MSG__DETAIL__ARMOR__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
