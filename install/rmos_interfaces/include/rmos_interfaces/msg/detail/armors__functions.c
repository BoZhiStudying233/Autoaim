// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rmos_interfaces:msg/Armors.idl
// generated code does not contain a copyright notice
#include "rmos_interfaces/msg/detail/armors__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `armors`
#include "rmos_interfaces/msg/detail/armor__functions.h"

bool
rmos_interfaces__msg__Armors__init(rmos_interfaces__msg__Armors * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    rmos_interfaces__msg__Armors__fini(msg);
    return false;
  }
  // armors
  if (!rmos_interfaces__msg__Armor__Sequence__init(&msg->armors, 0)) {
    rmos_interfaces__msg__Armors__fini(msg);
    return false;
  }
  // is_rune
  return true;
}

void
rmos_interfaces__msg__Armors__fini(rmos_interfaces__msg__Armors * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // armors
  rmos_interfaces__msg__Armor__Sequence__fini(&msg->armors);
  // is_rune
}

bool
rmos_interfaces__msg__Armors__are_equal(const rmos_interfaces__msg__Armors * lhs, const rmos_interfaces__msg__Armors * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // armors
  if (!rmos_interfaces__msg__Armor__Sequence__are_equal(
      &(lhs->armors), &(rhs->armors)))
  {
    return false;
  }
  // is_rune
  if (lhs->is_rune != rhs->is_rune) {
    return false;
  }
  return true;
}

bool
rmos_interfaces__msg__Armors__copy(
  const rmos_interfaces__msg__Armors * input,
  rmos_interfaces__msg__Armors * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // armors
  if (!rmos_interfaces__msg__Armor__Sequence__copy(
      &(input->armors), &(output->armors)))
  {
    return false;
  }
  // is_rune
  output->is_rune = input->is_rune;
  return true;
}

rmos_interfaces__msg__Armors *
rmos_interfaces__msg__Armors__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Armors * msg = (rmos_interfaces__msg__Armors *)allocator.allocate(sizeof(rmos_interfaces__msg__Armors), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rmos_interfaces__msg__Armors));
  bool success = rmos_interfaces__msg__Armors__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rmos_interfaces__msg__Armors__destroy(rmos_interfaces__msg__Armors * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rmos_interfaces__msg__Armors__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rmos_interfaces__msg__Armors__Sequence__init(rmos_interfaces__msg__Armors__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Armors * data = NULL;

  if (size) {
    data = (rmos_interfaces__msg__Armors *)allocator.zero_allocate(size, sizeof(rmos_interfaces__msg__Armors), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rmos_interfaces__msg__Armors__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rmos_interfaces__msg__Armors__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
rmos_interfaces__msg__Armors__Sequence__fini(rmos_interfaces__msg__Armors__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      rmos_interfaces__msg__Armors__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

rmos_interfaces__msg__Armors__Sequence *
rmos_interfaces__msg__Armors__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Armors__Sequence * array = (rmos_interfaces__msg__Armors__Sequence *)allocator.allocate(sizeof(rmos_interfaces__msg__Armors__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rmos_interfaces__msg__Armors__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rmos_interfaces__msg__Armors__Sequence__destroy(rmos_interfaces__msg__Armors__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rmos_interfaces__msg__Armors__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rmos_interfaces__msg__Armors__Sequence__are_equal(const rmos_interfaces__msg__Armors__Sequence * lhs, const rmos_interfaces__msg__Armors__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rmos_interfaces__msg__Armors__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rmos_interfaces__msg__Armors__Sequence__copy(
  const rmos_interfaces__msg__Armors__Sequence * input,
  rmos_interfaces__msg__Armors__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rmos_interfaces__msg__Armors);
    rmos_interfaces__msg__Armors * data =
      (rmos_interfaces__msg__Armors *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rmos_interfaces__msg__Armors__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          rmos_interfaces__msg__Armors__fini(&data[i]);
        }
        free(data);
        return false;
      }
    }
    output->data = data;
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rmos_interfaces__msg__Armors__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
