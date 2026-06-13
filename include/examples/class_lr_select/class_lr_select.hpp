#pragma once

#include "mod_common.hpp"

enum ClassLRSelect_OnOff : u32 {
    OPTION_OFF,
    OPTION_ON
};

// engine_class_select
enum ClassLRSelect_EngineClass : u32 {
    _150_CC,
    _300_CC,
    _500_CC,
    _9999_CC
};

// unused_leaf_type_select
enum ClassLRSelect_LeafType : u32 {
    TYPE_NORMAL,
    TYPE_UNUSED
};

// race_prints
enum ClassLRSelect_RacePrints : u32 {
    RACE_PRINTS_OFF,
    RACE_PRINTS_SPEED,
    RACE_PRINTS_SPEED_XYZ,
    RACE_PRINTS_FULL
};
extern u32 g_race_prints_option;

// ultra_miniturbos
extern bool g_ultra_miniturbo_enabled;

// random_stats
extern bool g_random_stats_enabled;

// input_viewer
enum ClassLRSelect_InputViewer : u32 {
    INPUT_VIEWER_OFF,
    INPUT_VIEWER_NO_BG,
    INPUT_VIEWER_FULL
};
extern u32 g_input_viewer_option;

// input_viewer
enum ClassLRSelect_VariableMiiSize : u32 {
    VARIABLE_MII_SIZE_OFF,
    VARIABLE_MII_SIZE_ON
};
extern u32 g_variable_mii_size;
