//
// Created by Wang on 23-6-14.
//

#ifndef RMOS_CONST_HPP
#define RMOS_CONST_HPP
namespace base
{
    enum Color
    {
        RED,
        BLUE,
    };

    enum Mode{
        NORMAL,     // 普通模式
        NORMAL_RUNE, // 小符模式
        RUNE,       // 大符模式
        OUTPOST,    // 前哨站模式
        WAIT,
    };

    enum DebugOption{
        CONTEST,
        SAVE_IMAGE,
        SAVE_DRAW_IMAGE,
        SHOW_ARMOR,  
        SHOW_BIN,
        SHOW_DETECT_COST,
        SHOW_RVIZ,
        SHOW_TOTAL_COST,
    };

    enum TrackState
    {
        LOST,       // 丢失目标
        DETECTING,    // 丢失目标但在寻找目标
        TEMP_LOST,      // 丢失目标
        TRACKING,      // 持续识别目标
    };
}
#endif //RMOS_CONST_HPP
