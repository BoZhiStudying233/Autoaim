// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rmos_interfaces:msg/Target.idl
// generated code does not contain a copyright notice
#include "rmos_interfaces/msg/detail/target__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `position`
#include "geometry_msgs/msg/detail/point__functions.h"

bool
rmos_interfaces__msg__Target__init(rmos_interfaces__msg__Target * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    rmos_interfaces__msg__Target__fini(msg);
    return false;
  }
  // id
  // track_state
  // suggest_fire
  // gun_pitch
  // gun_yaw
  // timestamp_recv
  // outpost_direction
  // position
  if (!geometry_msgs__msg__Point__init(&msg->position)) {
    rmos_interfaces__msg__Target__fini(msg);
    return false;
  }
  return true;
}

void
rmos_interfaces__msg__Target__fini(rmos_interfaces__msg__Target * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // id
  // track_state
  // suggest_fire
  // gun_pitch
  // gun_yaw
  // timestamp_recv
  // outpost_direction
  // position
  geometry_msgs__msg__Point__fini(&msg->position);
}

bool
rmos_interfaces__msg__Target__are_equal(const rmos_interfaces__msg__Target * lhs, const rmos_interfaces__msg__Target * rhs)
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
  // id
  if (lhs->id != rhs->id) {
    return false;
  }
  // track_state
  if (lhs->track_state != rhs->track_state) {
    return false;
  }
  // suggest_fire
  if (lhs->suggest_fire != rhs->suggest_fire) {
    return false;
  }
  // gun_pitch
  if (lhs->gun_pitch != rhs->gun_pitch) {
    return false;
  }
  // gun_yaw
  if (lhs->gun_yaw != rhs->gun_yaw) {
    return false;
  }
  // timestamp_recv
  if (lhs->timestamp_recv != rhs->timestamp_recv) {
    return false;
  }
  // outpost_direction
  if (lhs->outpost_direction != rhs->outpost_direction) {
    return false;
  }
  // position
  if (!geometry_msgs__msg__Point__are_equal(
      &(lhs->position), &(rhs->position)))
  {
    return false;
  }
  return true;
}

bool
rmos_interfaces__msg__Target__copy(
  const rmos_interfaces__msg__Target * input,
  rmos_interfaces__msg__Target * output)
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
  // id
  output->id = input->id;
  // track_state
  output->track_state = input->track_state;
  // suggest_fire
  output->suggest_fire = input->suggest_fire;
  // gun_pitch
  output->gun_pitch = input->gun_pitch;
  // gun_yaw
  output->gun_yaw = input->gun_yaw;
  // timestamp_recv
  output->timestamp_recv = input->timestamp_recv;
  // outpost_direction
  output->outpost_direction = input->outpost_direction;
  // position
  if (!geometry_msgs__msg__Point__copy(
      &(input->position), &(output->position)))
  {
    return false;
  }
  return true;
}

rmos_interfaces__msg__Target *
rmos_interfaces__msg__Target__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Target * msg = (rmos_interfaces__msg__Target *)allocator.allocate(sizeof(rmos_interfaces__msg__Target), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rmos_interfaces__msg__Target));
  bool success = rmos_interfaces__msg__Target__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rmos_interfaces__msg__Target__destroy(rmos_interfaces__msg__Target * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rmos_interfaces__msg__Target__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rmos_interfaces__msg__Target__Sequence__init(rmos_interfaces__msg__Target__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Target * data = NULL;

  if (size) {
    data = (rmos_interfaces__msg__Target *)allocator.zero_allocate(size, sizeof(rmos_interfaces__msg__Target), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rmos_interfaces__msg__Target__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rmos_interfaces__msg__Target__fini(&data[i - 1]);
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
rmos_interfaces__msg__Target__Sequence__fini(rmos_interfaces__msg__Target__Sequence * array)
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
      rmos_interfaces__msg__Target__fini(&array->data[i]);
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

rmos_interfaces__msg__Target__Sequence *
rmos_interfaces__msg__Target__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmos_interfaces__msg__Target__Sequence * array = (rmos_interfaces__msg__Target__Sequence *)allocator.allocate(sizeof(rmos_interfaces__msg__Target__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rmos_interfaces__msg__Target__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rmos_interfaces__msg__Target__Sequence__destroy(rmos_interfaces__msg__Target__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rmos_interfaces__msg__Target__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rmos_interfaces__msg__Target__Sequence__are_equal(const rmos_interfaces__msg__Target__Sequence * lhs, const rmos_interfaces__msg__Target__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rmos_interfaces__msg__Target__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rmos_interfaces__msg__Target__Sequence__copy(
  const rmos_interfaces__msg__Target__Sequence * input,
  rmos_interfaces__msg__Target__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rmos_interfaces__msg__Target);
    rmos_interfaces__msg__Target * data =
      (rmos_interfaces__msg__Target *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rmos_interfaces__msg__Target__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          rmos_interfaces__msg__Target__fini(&data[i]);
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
    if (!rmos_interfaces__msg__Target__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
