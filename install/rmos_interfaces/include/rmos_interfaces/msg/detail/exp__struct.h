// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rmos_interfaces:msg/Exp.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__EXP__STRUCT_H_
#define RMOS_INTERFACES__MSG__DETAIL__EXP__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/Exp in the package rmos_interfaces.
typedef struct rmos_interfaces__msg__Exp
{
  uint32_t exp;
} rmos_interfaces__msg__Exp;

// Struct for a sequence of rmos_interfaces__msg__Exp.
typedef struct rmos_interfaces__msg__Exp__Sequence
{
  rmos_interfaces__msg__Exp * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rmos_interfaces__msg__Exp__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RMOS_INTERFACES__MSG__DETAIL__EXP__STRUCT_H_
