// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rmos_interfaces:msg/QuaternionTime.idl
// generated code does not contain a copyright notice
#include "rmos_interfaces/msg/detail/quaternion_time__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `quaternion_stamped`
#include "geometry_msgs/msg/detail/quaternion_stamped__functions.h"

bool
rmos_interfaces__msg__QuaternionTime__init(rmos_interfaces__msg__QuaternionTime * msg)
{
  if (!msg) {
    return false;
  }
  // quaternion_stamped
  if (!geometry_msgs__msg__QuaternionStamped__init(&msg->quaternion_stamped)) {
    rmos_interfaces__msg__QuaternionTime__fini(msg);
    return false;
  }
  // timestamp_recv
  return true;
}

void
rmos_interfaces__msg__QuaternionTime__fini(rmos_interfaces__msg__QuaternionTime * msg)
{
  if (!msg) {
    return;
  }
  // quaternion_stamped
  geometry_msgs__msg__QuaternionStamped__fini(&msg->quaternion_stamped);
  // timestamp_recv
}

bool
rmos_interfaces__msg__QuaternionTime__are_equal(const rmos_interfaces__msg__QuaternionTime * lhs, const rmos_interfaces__msg__QuaternionTime * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // quaternion_stamped
  if (!geometry_msgs__msg__QuaternionStamped__are_equal(
      &(lhs->quaternion_stamped), &(rhs->quaternion_stamped)))
  {
    return false;
  }
  // timestamp_recv
  if (lhs->timestamp_recv != rhs->timestamp_recv) {
    return false;
  }
  return true;
}

bool
rmos_interfaces__msg__QuaternionTime__copy(
  const rmos_interfaces__msg__QuaternionTime * input,
  rmos_interfaces__msg__QuaternionTime * output)
{
  if (!input || !output) {
    return false;
  }
  // quaternion_stamped
  if (!geometry_msgs__msg__QuaternionStamped__copy(
      &(input->quaternion_stamped), &(output->quaternion_stamped)))
  {
    return false;
  }
  // timestamp_recv
  output->timestamp_recv = input->timestamp_recv;
  return true;
}

rmos_interfaces__msg__QuaternionTime *
rmos_interfaces__msg__QuaternionTime__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__QuaternionTime * msg = (rmos_interfaces__msg__QuaternionTime *)allocator.allocate(sizeof(rmos_interfaces__msg__QuaternionTime), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rmos_interfaces__msg__QuaternionTime));
  bool success = rmos_interfaces__msg__QuaternionTime__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rmos_interfaces__msg__QuaternionTime__destroy(rmos_interfaces__msg__QuaternionTime * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rmos_interfaces__msg__QuaternionTime__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rmos_interfaces__msg__QuaternionTime__Sequence__init(rmos_interfaces__msg__QuaternionTime__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__QuaternionTime * data = NULL;

  if (size) {
    data = (rmos_interfaces__msg__QuaternionTime *)allocator.zero_allocate(size, sizeof(rmos_interfaces__msg__QuaternionTime), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rmos_interfaces__msg__QuaternionTime__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rmos_interfaces__msg__QuaternionTime__fini(&data[i - 1]);
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
rmos_interfaces__msg__QuaternionTime__Sequence__fini(rmos_interfaces__msg__QuaternionTime__Sequence * array)
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
      rmos_interfaces__msg__QuaternionTime__fini(&array->data[i]);
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

rmos_interfaces__msg__QuaternionTime__Sequence *
rmos_interfaces__msg__QuaternionTime__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__QuaternionTime__Sequence * array = (rmos_interfaces__msg__QuaternionTime__Sequence *)allocator.allocate(sizeof(rmos_interfaces__msg__QuaternionTime__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rmos_interfaces__msg__QuaternionTime__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rmos_interfaces__msg__QuaternionTime__Sequence__destroy(rmos_interfaces__msg__QuaternionTime__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rmos_interfaces__msg__QuaternionTime__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rmos_interfaces__msg__QuaternionTime__Sequence__are_equal(const rmos_interfaces__msg__QuaternionTime__Sequence * lhs, const rmos_interfaces__msg__QuaternionTime__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rmos_interfaces__msg__QuaternionTime__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rmos_interfaces__msg__QuaternionTime__Sequence__copy(
  const rmos_interfaces__msg__QuaternionTime__Sequence * input,
  rmos_interfaces__msg__QuaternionTime__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rmos_interfaces__msg__QuaternionTime);
    rmos_interfaces__msg__QuaternionTime * data =
      (rmos_interfaces__msg__QuaternionTime *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rmos_interfaces__msg__QuaternionTime__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          rmos_interfaces__msg__QuaternionTime__fini(&data[i]);
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
    if (!rmos_interfaces__msg__QuaternionTime__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
