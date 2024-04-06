/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Qylann
 * @Date: 2022-04-05 16:03:01
 * @LastEditors: Qylann
 * @LastEditTime: 2022-04-05 16:06:21
 */

#ifndef RMOS_USB_HPP
#define RMOS_USB_HPP

#include "sdk/include/usbcdc_transporter.hpp"

#define RMOS_RECEIVE_ID 0x1
#define RMOS_SEND_ID 0x2

namespace transporter
{


typedef signed char     int8_t;
typedef unsigned char   uint8_t;
typedef signed short    int16_t;
typedef unsigned short  uint16_t;
typedef signed int      int32_t;
typedef unsigned int    uint32_t;


#pragma pack(push, 1)

typedef struct
{
    // 包头
    uint8_t _SOF;
    uint8_t ID;
		// imu数据
		uint32_t TimeStamp;
    float q0;
    float q1;
    float q2;
    float q3;
    // 机器人自身信息
    uint8_t robot_id;
    uint8_t mode;
    // 包尾
    uint8_t _EOF;
} RMOSReceivePackage;

typedef struct
{
    // 包头
    uint8_t _SOF;
    uint8_t ID;
		// 自瞄状态
		uint8_t AimbotState;
    uint8_t AimbotTarget;
		// 自瞄数据
		float PitchRelativeAngle;
    float YawRelativeAngle;
    // 目标角速度
    float TargetPitchSpeed;
    float TargetYawSpeed;
    // 时间戳
    uint32_t SystemTimer;
    // 包尾
    uint8_t _EOF;
} RMOSSendPackage;


#pragma pack(pop)

} // namespace transporter


#endif



