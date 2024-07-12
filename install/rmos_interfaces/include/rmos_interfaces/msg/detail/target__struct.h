// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rmos_interfaces:msg/Target.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__TARGET__STRUCT_H_
#define RMOS_INTERFACES__MSG__DETAIL__TARGET__STRUCT_H_

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
// Member 'position'
#include "geometry_msgs/msg/detail/point__struct.h"

// Struct defined in msg/Target in the package rmos_interfaces.
typedef struct rmos_interfaces__msg__Target
{
  std_msgs__msg__Header header;
  uint8_t id;
  uint8_t track_state;
  bool suggest_fire;
  float gun_pitch;
  float gun_yaw;
  uint32_t timestamp_recv;
  int8_t outpost_direction;
  geometry_msgs__msg__Point position;
} rmos_interfaces__msg__Target;

// Struct for a sequence of rmos_interfaces__msg__Target.
typedef struct rmos_interfaces__msg__Target__Sequence
{
  rmos_interfaces__msg__Target * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rmos_interfaces__msg__Target__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RMOS_INTERFACES__MSG__DETAIL__TARGET__STRUCT_H_
