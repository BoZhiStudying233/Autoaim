// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rmos_interfaces:msg/Armors.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__ARMORS__STRUCT_H_
#define RMOS_INTERFACES__MSG__DETAIL__ARMORS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'armors'
#include "rmos_interfaces/msg/detail/armor__struct.h"

// Struct defined in msg/Armors in the package rmos_interfaces.
typedef struct rmos_interfaces__msg__Armors
{
  std_msgs__msg__Header header;
  rmos_interfaces__msg__Armor__Sequence armors;
  bool is_rune;
} rmos_interfaces__msg__Armors;

// Struct for a sequence of rmos_interfaces__msg__Armors.
typedef struct rmos_interfaces__msg__Armors__Sequence
{
  rmos_interfaces__msg__Armors * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rmos_interfaces__msg__Armors__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RMOS_INTERFACES__MSG__DETAIL__ARMORS__STRUCT_H_
