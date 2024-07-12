// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rmos_interfaces:msg/Armors.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rmos_interfaces/msg/detail/armors__rosidl_typesupport_introspection_c.h"
#include "rmos_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rmos_interfaces/msg/detail/armors__functions.h"
#include "rmos_interfaces/msg/detail/armors__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `armors`
#include "rmos_interfaces/msg/armor.h"
// Member `armors`
#include "rmos_interfaces/msg/detail/armor__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void Armors__rosidl_typesupport_introspection_c__Armors_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rmos_interfaces__msg__Armors__init(message_memory);
}

void Armors__rosidl_typesupport_introspection_c__Armors_fini_function(void * message_memory)
{
  rmos_interfaces__msg__Armors__fini(message_memory);
}

size_t Armors__rosidl_typesupport_introspection_c__size_function__Armor__armors(
  const void * untyped_member)
{
  const rmos_interfaces__msg__Armor__Sequence * member =
    (const rmos_interfaces__msg__Armor__Sequence *)(untyped_member);
  return member->size;
}

const void * Armors__rosidl_typesupport_introspection_c__get_const_function__Armor__armors(
  const void * untyped_member, size_t index)
{
  const rmos_interfaces__msg__Armor__Sequence * member =
    (const rmos_interfaces__msg__Armor__Sequence *)(untyped_member);
  return &member->data[index];
}

void * Armors__rosidl_typesupport_introspection_c__get_function__Armor__armors(
  void * untyped_member, size_t index)
{
  rmos_interfaces__msg__Armor__Sequence * member =
    (rmos_interfaces__msg__Armor__Sequence *)(untyped_member);
  return &member->data[index];
}

bool Armors__rosidl_typesupport_introspection_c__resize_function__Armor__armors(
  void * untyped_member, size_t size)
{
  rmos_interfaces__msg__Armor__Sequence * member =
    (rmos_interfaces__msg__Armor__Sequence *)(untyped_member);
  rmos_interfaces__msg__Armor__Sequence__fini(member);
  return rmos_interfaces__msg__Armor__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember Armors__rosidl_typesupport_introspection_c__Armors_message_member_array[3] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rmos_interfaces__msg__Armors, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "armors",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rmos_interfaces__msg__Armors, armors),  // bytes offset in struct
    NULL,  // default value
    Armors__rosidl_typesupport_introspection_c__size_function__Armor__armors,  // size() function pointer
    Armors__rosidl_typesupport_introspection_c__get_const_function__Armor__armors,  // get_const(index) function pointer
    Armors__rosidl_typesupport_introspection_c__get_function__Armor__armors,  // get(index) function pointer
    Armors__rosidl_typesupport_introspection_c__resize_function__Armor__armors  // resize(index) function pointer
  },
  {
    "is_rune",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rmos_interfaces__msg__Armors, is_rune),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers Armors__rosidl_typesupport_introspection_c__Armors_message_members = {
  "rmos_interfaces__msg",  // message namespace
  "Armors",  // message name
  3,  // number of fields
  sizeof(rmos_interfaces__msg__Armors),
  Armors__rosidl_typesupport_introspection_c__Armors_message_member_array,  // message members
  Armors__rosidl_typesupport_introspection_c__Armors_init_function,  // function to initialize message memory (memory has to be allocated)
  Armors__rosidl_typesupport_introspection_c__Armors_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t Armors__rosidl_typesupport_introspection_c__Armors_message_type_support_handle = {
  0,
  &Armors__rosidl_typesupport_introspection_c__Armors_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rmos_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rmos_interfaces, msg, Armors)() {
  Armors__rosidl_typesupport_introspection_c__Armors_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  Armors__rosidl_typesupport_introspection_c__Armors_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rmos_interfaces, msg, Armor)();
  if (!Armors__rosidl_typesupport_introspection_c__Armors_message_type_support_handle.typesupport_identifier) {
    Armors__rosidl_typesupport_introspection_c__Armors_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &Armors__rosidl_typesupport_introspection_c__Armors_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
