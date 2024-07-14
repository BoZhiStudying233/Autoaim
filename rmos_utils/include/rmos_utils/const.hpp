//
// Created by Wang on 23-6-14.
//

#ifndef RMOS_CONST_HPP
#define RMOS_CONST_HPP
namespace base
{
    enum Color
    {
        RED=0,
        BLUE=1,
    };

    enum Mode{
        NORMAL=0,     // 普通模式
        NORMAL_RUNE=1, // 小符模式
        RUNE=2,       // 大符模式
        OUTPOST=3,    // 前哨站模式
        WAIT=4,
    };

    enum ArmorType
    {
        WRONG,
        SMALL,
        BIG,
        RUNE_ARMOR,
        GRAY_BIG_ARMOR,
        GRAY_SMALL_ARMOR,
        DEAD_ARMOR,
    };

    enum TrackState
    {
        LOST,       // 丢失目标
        DETECTING,    // 丢失目标但在寻找目标
        TEMP_LOST,      // 丢失目标
        TRACKING,      // 持续识别目标
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

}
#endif //RMOS_CONST_HPP
