// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from rmos_interfaces:msg/QuaternionTime.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "rmos_interfaces/msg/detail/quaternion_time__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace rmos_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void QuaternionTime_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) rmos_interfaces::msg::QuaternionTime(_init);
}

void QuaternionTime_fini_function(void * message_memory)
{
  auto typed_message = static_cast<rmos_interfaces::msg::QuaternionTime *>(message_memory);
  typed_message->~QuaternionTime();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember QuaternionTime_message_member_array[2] = {
  {
    "quaternion_stamped",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::QuaternionStamped>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rmos_interfaces::msg::QuaternionTime, quaternion_stamped),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "timestamp_recv",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rmos_interfaces::msg::QuaternionTime, timestamp_recv),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers QuaternionTime_message_members = {
  "rmos_interfaces::msg",  // message namespace
  "QuaternionTime",  // message name
  2,  // number of fields
  sizeof(rmos_interfaces::msg::QuaternionTime),
  QuaternionTime_message_member_array,  // message members
  QuaternionTime_init_function,  // function to initialize message memory (memory has to be allocated)
  QuaternionTime_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t QuaternionTime_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &QuaternionTime_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace rmos_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<rmos_interfaces::msg::QuaternionTime>()
{
  return &::rmos_interfaces::msg::rosidl_typesupport_introspection_cpp::QuaternionTime_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, rmos_interfaces, msg, QuaternionTime)() {
  return &::rmos_interfaces::msg::rosidl_typesupport_introspection_cpp::QuaternionTime_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
