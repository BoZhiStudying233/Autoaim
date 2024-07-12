// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from rmos_interfaces:msg/Color.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__COLOR__FUNCTIONS_H_
#define RMOS_INTERFACES__MSG__DETAIL__COLOR__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "rmos_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "rmos_interfaces/msg/detail/color__struct.h"

/// Initialize msg/Color message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * rmos_interfaces__msg__Color
 * )) before or use
 * rmos_interfaces__msg__Color__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__Color__init(rmos_interfaces__msg__Color * msg);

/// Finalize msg/Color message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
void
rmos_interfaces__msg__Color__fini(rmos_interfaces__msg__Color * msg);

/// Create msg/Color message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * rmos_interfaces__msg__Color__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
rmos_interfaces__msg__Color *
rmos_interfaces__msg__Color__create();

/// Destroy msg/Color message.
/**
 * It calls
 * rmos_interfaces__msg__Color__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
void
rmos_interfaces__msg__Color__destroy(rmos_interfaces__msg__Color * msg);

/// Check for msg/Color message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__Color__are_equal(const rmos_interfaces__msg__Color * lhs, const rmos_interfaces__msg__Color * rhs);

/// Copy a msg/Color message.
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
rmos_interfaces__msg__Color__copy(
  const rmos_interfaces__msg__Color * input,
  rmos_interfaces__msg__Color * output);

/// Initialize array of msg/Color messages.
/**
 * It allocates the memory for the number of elements and calls
 * rmos_interfaces__msg__Color__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__Color__Sequence__init(rmos_interfaces__msg__Color__Sequence * array, size_t size);

/// Finalize array of msg/Color messages.
/**
 * It calls
 * rmos_interfaces__msg__Color__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
void
rmos_interfaces__msg__Color__Sequence__fini(rmos_interfaces__msg__Color__Sequence * array);

/// Create array of msg/Color messages.
/**
 * It allocates the memory for the array and calls
 * rmos_interfaces__msg__Color__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
rmos_interfaces__msg__Color__Sequence *
rmos_interfaces__msg__Color__Sequence__create(size_t size);

/// Destroy array of msg/Color messages.
/**
 * It calls
 * rmos_interfaces__msg__Color__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
void
rmos_interfaces__msg__Color__Sequence__destroy(rmos_interfaces__msg__Color__Sequence * array);

/// Check for msg/Color message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rmos_interfaces
bool
rmos_interfaces__msg__Color__Sequence__are_equal(const rmos_interfaces__msg__Color__Sequence * lhs, const rmos_interfaces__msg__Color__Sequence * rhs);

/// Copy an array of msg/Color messages.
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
rmos_interfaces__msg__Color__Sequence__copy(
  const rmos_interfaces__msg__Color__Sequence * input,
  rmos_interfaces__msg__Color__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // RMOS_INTERFACES__MSG__DETAIL__COLOR__FUNCTIONS_H_
