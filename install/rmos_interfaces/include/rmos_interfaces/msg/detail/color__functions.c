// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rmos_interfaces:msg/Color.idl
// generated code does not contain a copyright notice
#include "rmos_interfaces/msg/detail/color__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
rmos_interfaces__msg__Color__init(rmos_interfaces__msg__Color * msg)
{
  if (!msg) {
    return false;
  }
  // color
  return true;
}

void
rmos_interfaces__msg__Color__fini(rmos_interfaces__msg__Color * msg)
{
  if (!msg) {
    return;
  }
  // color
}

bool
rmos_interfaces__msg__Color__are_equal(const rmos_interfaces__msg__Color * lhs, const rmos_interfaces__msg__Color * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // color
  if (lhs->color != rhs->color) {
    return false;
  }
  return true;
}

bool
rmos_interfaces__msg__Color__copy(
  const rmos_interfaces__msg__Color * input,
  rmos_interfaces__msg__Color * output)
{
  if (!input || !output) {
    return false;
  }
  // color
  output->color = input->color;
  return true;
}

rmos_interfaces__msg__Color *
rmos_interfaces__msg__Color__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Color * msg = (rmos_interfaces__msg__Color *)allocator.allocate(sizeof(rmos_interfaces__msg__Color), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rmos_interfaces__msg__Color));
  bool success = rmos_interfaces__msg__Color__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rmos_interfaces__msg__Color__destroy(rmos_interfaces__msg__Color * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rmos_interfaces__msg__Color__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rmos_interfaces__msg__Color__Sequence__init(rmos_interfaces__msg__Color__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Color * data = NULL;

  if (size) {
    data = (rmos_interfaces__msg__Color *)allocator.zero_allocate(size, sizeof(rmos_interfaces__msg__Color), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rmos_interfaces__msg__Color__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rmos_interfaces__msg__Color__fini(&data[i - 1]);
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
rmos_interfaces__msg__Color__Sequence__fini(rmos_interfaces__msg__Color__Sequence * array)
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
      rmos_interfaces__msg__Color__fini(&array->data[i]);
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

rmos_interfaces__msg__Color__Sequence *
rmos_interfaces__msg__Color__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Color__Sequence * array = (rmos_interfaces__msg__Color__Sequence *)allocator.allocate(sizeof(rmos_interfaces__msg__Color__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rmos_interfaces__msg__Color__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rmos_interfaces__msg__Color__Sequence__destroy(rmos_interfaces__msg__Color__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rmos_interfaces__msg__Color__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rmos_interfaces__msg__Color__Sequence__are_equal(const rmos_interfaces__msg__Color__Sequence * lhs, const rmos_interfaces__msg__Color__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rmos_interfaces__msg__Color__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rmos_interfaces__msg__Color__Sequence__copy(
  const rmos_interfaces__msg__Color__Sequence * input,
  rmos_interfaces__msg__Color__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rmos_interfaces__msg__Color);
    rmos_interfaces__msg__Color * data =
      (rmos_interfaces__msg__Color *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rmos_interfaces__msg__Color__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          rmos_interfaces__msg__Color__fini(&data[i]);
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
    if (!rmos_interfaces__msg__Color__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
