// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rmos_interfaces:msg/Armor.idl
// generated code does not contain a copyright notice
#include "rmos_interfaces/msg/detail/armor__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `pose`
#include "geometry_msgs/msg/detail/pose__functions.h"

bool
rmos_interfaces__msg__Armor__init(rmos_interfaces__msg__Armor * msg)
{
  if (!msg) {
    return false;
  }
  // num_id
  // pose
  if (!geometry_msgs__msg__Pose__init(&msg->pose)) {
    rmos_interfaces__msg__Armor__fini(msg);
    return false;
  }
  // distance_to_image_center
  // distance
  return true;
}

void
rmos_interfaces__msg__Armor__fini(rmos_interfaces__msg__Armor * msg)
{
  if (!msg) {
    return;
  }
  // num_id
  // pose
  geometry_msgs__msg__Pose__fini(&msg->pose);
  // distance_to_image_center
  // distance
}

bool
rmos_interfaces__msg__Armor__are_equal(const rmos_interfaces__msg__Armor * lhs, const rmos_interfaces__msg__Armor * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // num_id
  if (lhs->num_id != rhs->num_id) {
    return false;
  }
  // pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->pose), &(rhs->pose)))
  {
    return false;
  }
  // distance_to_image_center
  if (lhs->distance_to_image_center != rhs->distance_to_image_center) {
    return false;
  }
  // distance
  if (lhs->distance != rhs->distance) {
    return false;
  }
  return true;
}

bool
rmos_interfaces__msg__Armor__copy(
  const rmos_interfaces__msg__Armor * input,
  rmos_interfaces__msg__Armor * output)
{
  if (!input || !output) {
    return false;
  }
  // num_id
  output->num_id = input->num_id;
  // pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->pose), &(output->pose)))
  {
    return false;
  }
  // distance_to_image_center
  output->distance_to_image_center = input->distance_to_image_center;
  // distance
  output->distance = input->distance;
  return true;
}

rmos_interfaces__msg__Armor *
rmos_interfaces__msg__Armor__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Armor * msg = (rmos_interfaces__msg__Armor *)allocator.allocate(sizeof(rmos_interfaces__msg__Armor), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rmos_interfaces__msg__Armor));
  bool success = rmos_interfaces__msg__Armor__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rmos_interfaces__msg__Armor__destroy(rmos_interfaces__msg__Armor * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rmos_interfaces__msg__Armor__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rmos_interfaces__msg__Armor__Sequence__init(rmos_interfaces__msg__Armor__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Armor * data = NULL;

  if (size) {
    data = (rmos_interfaces__msg__Armor *)allocator.zero_allocate(size, sizeof(rmos_interfaces__msg__Armor), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rmos_interfaces__msg__Armor__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rmos_interfaces__msg__Armor__fini(&data[i - 1]);
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
rmos_interfaces__msg__Armor__Sequence__fini(rmos_interfaces__msg__Armor__Sequence * array)
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
      rmos_interfaces__msg__Armor__fini(&array->data[i]);
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

rmos_interfaces__msg__Armor__Sequence *
rmos_interfaces__msg__Armor__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Armor__Sequence * array = (rmos_interfaces__msg__Armor__Sequence *)allocator.allocate(sizeof(rmos_interfaces__msg__Armor__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rmos_interfaces__msg__Armor__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rmos_interfaces__msg__Armor__Sequence__destroy(rmos_interfaces__msg__Armor__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rmos_interfaces__msg__Armor__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rmos_interfaces__msg__Armor__Sequence__are_equal(const rmos_interfaces__msg__Armor__Sequence * lhs, const rmos_interfaces__msg__Armor__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rmos_interfaces__msg__Armor__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rmos_interfaces__msg__Armor__Sequence__copy(
  const rmos_interfaces__msg__Armor__Sequence * input,
  rmos_interfaces__msg__Armor__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rmos_interfaces__msg__Armor);
    rmos_interfaces__msg__Armor * data =
      (rmos_interfaces__msg__Armor *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rmos_interfaces__msg__Armor__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          rmos_interfaces__msg__Armor__fini(&data[i]);
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
    if (!rmos_interfaces__msg__Armor__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
