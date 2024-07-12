// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rmos_interfaces:msg/BulletSpeed.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__STRUCT_H_
#define RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/BulletSpeed in the package rmos_interfaces.
typedef struct rmos_interfaces__msg__BulletSpeed
{
  uint8_t speed;
} rmos_interfaces__msg__BulletSpeed;

// Struct for a sequence of rmos_interfaces__msg__BulletSpeed.
typedef struct rmos_interfaces__msg__BulletSpeed__Sequence
{
  rmos_interfaces__msg__BulletSpeed * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rmos_interfaces__msg__BulletSpeed__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__STRUCT_H_
