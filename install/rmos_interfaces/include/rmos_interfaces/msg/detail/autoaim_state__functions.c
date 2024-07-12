// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rmos_interfaces:msg/AutoaimState.idl
// generated code does not contain a copyright notice
#include "rmos_interfaces/msg/detail/autoaim_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
rmos_interfaces__msg__AutoaimState__init(rmos_interfaces__msg__AutoaimState * msg)
{
  if (!msg) {
    return false;
  }
  // autoaim_state
  return true;
}

void
rmos_interfaces__msg__AutoaimState__fini(rmos_interfaces__msg__AutoaimState * msg)
{
  if (!msg) {
    return;
  }
  // autoaim_state
}

bool
rmos_interfaces__msg__AutoaimState__are_equal(const rmos_interfaces__msg__AutoaimState * lhs, const rmos_interfaces__msg__AutoaimState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // autoaim_state
  if (lhs->autoaim_state != rhs->autoaim_state) {
    return false;
  }
  return true;
}

bool
rmos_interfaces__msg__AutoaimState__copy(
  const rmos_interfaces__msg__AutoaimState * input,
  rmos_interfaces__msg__AutoaimState * output)
{
  if (!input || !output) {
    return false;
  }
  // autoaim_state
  output->autoaim_state = input->autoaim_state;
  return true;
}

rmos_interfaces__msg__AutoaimState *
rmos_interfaces__msg__AutoaimState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__AutoaimState * msg = (rmos_interfaces__msg__AutoaimState *)allocator.allocate(sizeof(rmos_interfaces__msg__AutoaimState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rmos_interfaces__msg__AutoaimState));
  bool success = rmos_interfaces__msg__AutoaimState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rmos_interfaces__msg__AutoaimState__destroy(rmos_interfaces__msg__AutoaimState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rmos_interfaces__msg__AutoaimState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rmos_interfaces__msg__AutoaimState__Sequence__init(rmos_interfaces__msg__AutoaimState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__AutoaimState * data = NULL;

  if (size) {
    data = (rmos_interfaces__msg__AutoaimState *)allocator.zero_allocate(size, sizeof(rmos_interfaces__msg__AutoaimState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rmos_interfaces__msg__AutoaimState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rmos_interfaces__msg__AutoaimState__fini(&data[i - 1]);
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
rmos_interfaces__msg__AutoaimState__Sequence__fini(rmos_interfaces__msg__AutoaimState__Sequence * array)
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
      rmos_interfaces__msg__AutoaimState__fini(&array->data[i]);
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

rmos_interfaces__msg__AutoaimState__Sequence *
rmos_interfaces__msg__AutoaimState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__AutoaimState__Sequence * array = (rmos_interfaces__msg__AutoaimState__Sequence *)allocator.allocate(sizeof(rmos_interfaces__msg__AutoaimState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rmos_interfaces__msg__AutoaimState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rmos_interfaces__msg__AutoaimState__Sequence__destroy(rmos_interfaces__msg__AutoaimState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rmos_interfaces__msg__AutoaimState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rmos_interfaces__msg__AutoaimState__Sequence__are_equal(const rmos_interfaces__msg__AutoaimState__Sequence * lhs, const rmos_interfaces__msg__AutoaimState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rmos_interfaces__msg__AutoaimState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rmos_interfaces__msg__AutoaimState__Sequence__copy(
  const rmos_interfaces__msg__AutoaimState__Sequence * input,
  rmos_interfaces__msg__AutoaimState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rmos_interfaces__msg__AutoaimState);
    rmos_interfaces__msg__AutoaimState * data =
      (rmos_interfaces__msg__AutoaimState *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rmos_interfaces__msg__AutoaimState__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          rmos_interfaces__msg__AutoaimState__fini(&data[i]);
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
    if (!rmos_interfaces__msg__AutoaimState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
