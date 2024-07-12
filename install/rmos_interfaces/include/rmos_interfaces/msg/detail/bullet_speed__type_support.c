// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rmos_interfaces:msg/BulletSpeed.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rmos_interfaces/msg/detail/bullet_speed__rosidl_typesupport_introspection_c.h"
#include "rmos_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rmos_interfaces/msg/detail/bullet_speed__functions.h"
#include "rmos_interfaces/msg/detail/bullet_speed__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rmos_interfaces__msg__BulletSpeed__init(message_memory);
}

void BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_fini_function(void * message_memory)
{
  rmos_interfaces__msg__BulletSpeed__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_message_member_array[1] = {
  {
    "speed",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rmos_interfaces__msg__BulletSpeed, speed),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_message_members = {
  "rmos_interfaces__msg",  // message namespace
  "BulletSpeed",  // message name
  1,  // number of fields
  sizeof(rmos_interfaces__msg__BulletSpeed),
  BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_message_member_array,  // message members
  BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_init_function,  // function to initialize message memory (memory has to be allocated)
  BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_message_type_support_handle = {
  0,
  &BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rmos_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rmos_interfaces, msg, BulletSpeed)() {
  if (!BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_message_type_support_handle.typesupport_identifier) {
    BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &BulletSpeed__rosidl_typesupport_introspection_c__BulletSpeed_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
