// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rmos_interfaces:msg/QuaternionTime.idl
// generated code does not contain a copyright notice

#ifndef RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__STRUCT_HPP_
#define RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


// Include directives for member types
// Member 'quaternion_stamped'
#include "geometry_msgs/msg/detail/quaternion_stamped__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__rmos_interfaces__msg__QuaternionTime __attribute__((deprecated))
#else
# define DEPRECATED__rmos_interfaces__msg__QuaternionTime __declspec(deprecated)
#endif

namespace rmos_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct QuaternionTime_
{
  using Type = QuaternionTime_<ContainerAllocator>;

  explicit QuaternionTime_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : quaternion_stamped(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->timestamp_recv = 0ul;
    }
  }

  explicit QuaternionTime_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : quaternion_stamped(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->timestamp_recv = 0ul;
    }
  }

  // field types and members
  using _quaternion_stamped_type =
    geometry_msgs::msg::QuaternionStamped_<ContainerAllocator>;
  _quaternion_stamped_type quaternion_stamped;
  using _timestamp_recv_type =
    uint32_t;
  _timestamp_recv_type timestamp_recv;

  // setters for named parameter idiom
  Type & set__quaternion_stamped(
    const geometry_msgs::msg::QuaternionStamped_<ContainerAllocator> & _arg)
  {
    this->quaternion_stamped = _arg;
    return *this;
  }
  Type & set__timestamp_recv(
    const uint32_t & _arg)
  {
    this->timestamp_recv = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rmos_interfaces::msg::QuaternionTime_<ContainerAllocator> *;
  using ConstRawPtr =
    const rmos_interfaces::msg::QuaternionTime_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rmos_interfaces::msg::QuaternionTime_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rmos_interfaces::msg::QuaternionTime_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rmos_interfaces::msg::QuaternionTime_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rmos_interfaces::msg::QuaternionTime_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rmos_interfaces::msg::QuaternionTime_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rmos_interfaces::msg::QuaternionTime_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rmos_interfaces::msg::QuaternionTime_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rmos_interfaces::msg::QuaternionTime_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rmos_interfaces__msg__QuaternionTime
    std::shared_ptr<rmos_interfaces::msg::QuaternionTime_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rmos_interfaces__msg__QuaternionTime
    std::shared_ptr<rmos_interfaces::msg::QuaternionTime_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const QuaternionTime_ & other) const
  {
    if (this->quaternion_stamped != other.quaternion_stamped) {
      return false;
    }
    if (this->timestamp_recv != other.timestamp_recv) {
      return false;
    }
    return true;
  }
  bool operator!=(const QuaternionTime_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct QuaternionTime_

// alias to use template instance with default allocator
using QuaternionTime =
  rmos_interfaces::msg::QuaternionTime_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rmos_interfaces

#endif  // RMOS_INTERFACES__MSG__DETAIL__QUATERNION_TIME__STRUCT_HPP_
