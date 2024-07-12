// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from rmos_interfaces:msg/QuaternionTime.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__FUNCTIONS_H_
#define RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "rmos_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "rmos_interfaces/msg/detail/quaternion_time__struct.h"

/// Initialize msg/QuaternionTime message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * rmos_interfaces__msg__QuaternionTime
 * )) before or use
 * rmos_interfaces__msg__QuaternionTime__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__QuaternionTime__init(rmos_interfaces__msg__QuaternionTime * msg);

/// Finalize msg/QuaternionTime message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
void
rmos_interfaces__msg__QuaternionTime__fini(rmos_interfaces__msg__QuaternionTime * msg);

/// Create msg/QuaternionTime message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * rmos_interfaces__msg__QuaternionTime__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
rmos_interfaces__msg__QuaternionTime *
rmos_interfaces__msg__QuaternionTime__create();

/// Destroy msg/QuaternionTime message.
/**
 * It calls
 * rmos_interfaces__msg__QuaternionTime__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
void
rmos_interfaces__msg__QuaternionTime__destroy(rmos_interfaces__msg__QuaternionTime * msg);

/// Check for msg/QuaternionTime message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__QuaternionTime__are_equal(const rmos_interfaces__msg__QuaternionTime * lhs, const rmos_interfaces__msg__QuaternionTime * rhs);

/// Copy a msg/QuaternionTime message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__QuaternionTime__copy(
  const rmos_interfaces__msg__QuaternionTime * input,
  rmos_interfaces__msg__QuaternionTime * output);

/// Initialize array of msg/QuaternionTime messages.
/**
 * It allocates the memory for the number of elements and calls
 * rmos_interfaces__msg__QuaternionTime__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__QuaternionTime__Sequence__init(rmos_interfaces__msg__QuaternionTime__Sequence * array, size_t size);

/// Finalize array of msg/QuaternionTime messages.
/**
 * It calls
 * rmos_interfaces__msg__QuaternionTime__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
void
rmos_interfaces__msg__QuaternionTime__Sequence__fini(rmos_interfaces__msg__QuaternionTime__Sequence * array);

/// Create array of msg/QuaternionTime messages.
/**
 * It allocates the memory for the array and calls
 * rmos_interfaces__msg__QuaternionTime__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
rmos_interfaces__msg__QuaternionTime__Sequence *
rmos_interfaces__msg__QuaternionTime__Sequence__create(size_t size);

/// Destroy array of msg/QuaternionTime messages.
/**
 * It calls
 * rmos_interfaces__msg__QuaternionTime__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
void
rmos_interfaces__msg__QuaternionTime__Sequence__destroy(rmos_interfaces__msg__QuaternionTime__Sequence * array);

/// Check for msg/QuaternionTime message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__QuaternionTime__Sequence__are_equal(const rmos_interfaces__msg__QuaternionTime__Sequence * lhs, const rmos_interfaces__msg__QuaternionTime__Sequence * rhs);

/// Copy an array of msg/QuaternionTime messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__QuaternionTime__Sequence__copy(
  const rmos_interfaces__msg__QuaternionTime__Sequence * input,
  rmos_interfaces__msg__QuaternionTime__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__FUNCTIONS_H_
