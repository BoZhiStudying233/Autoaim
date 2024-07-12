// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rmos_interfaces:msg/BulletSpeed.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__STRUCT_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__rmos_interfaces__msg__BulletSpeed __attribute__((deprecated))
#else
# define DEPRECATED__rmos_interfaces__msg__BulletSpeed __declspec(deprecated)
#endif

namespace rmos_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct BulletSpeed_
{
  using Type = BulletSpeed_<ContainerAllocator>;

  explicit BulletSpeed_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->speed = 0;
    }
  }

  explicit BulletSpeed_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->speed = 0;
    }
  }

  // field types and members
  using _speed_type =
    uint8_t;
  _speed_type speed;

  // setters for named parameter idiom
  Type & set__speed(
    const uint8_t & _arg)
  {
    this->speed = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rmos_interfaces::msg::BulletSpeed_<ContainerAllocator> *;
  using ConstRawPtr =
    const rmos_interfaces::msg::BulletSpeed_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rmos_interfaces::msg::BulletSpeed_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rmos_interfaces::msg::BulletSpeed_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rmos_interfaces::msg::BulletSpeed_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rmos_interfaces::msg::BulletSpeed_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rmos_interfaces::msg::BulletSpeed_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rmos_interfaces::msg::BulletSpeed_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rmos_interfaces::msg::BulletSpeed_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rmos_interfaces::msg::BulletSpeed_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rmos_interfaces__msg__BulletSpeed
    std::shared_ptr<rmos_interfaces::msg::BulletSpeed_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rmos_interfaces__msg__BulletSpeed
    std::shared_ptr<rmos_interfaces::msg::BulletSpeed_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const BulletSpeed_ & other) const
  {
    if (this->speed != other.speed) {
      return false;
    }
    return true;
  }
  bool operator!=(const BulletSpeed_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct BulletSpeed_

// alias to use template instance with default allocator
using BulletSpeed =
  rmos_interfaces::msg::BulletSpeed_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__BULLET_SPEED__STRUCT_HPP_
