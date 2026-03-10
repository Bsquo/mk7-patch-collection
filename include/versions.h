#ifndef VERSIONS_H
#define VERSIONS_H

#define VERSION_CHN_DLP       0
#define VERSION_CHN_REV1      1
#define VERSION_EUR_DLP       2
#define VERSION_EUR_KIOSK     3
#define VERSION_EUR_REV0      4
#define VERSION_EUR_REV0_V11  5
#define VERSION_EUR_REV1      6
#define VERSION_EUR_REV2      7
#define VERSION_JPN_DLP       8
#define VERSION_JPN_KIOSK     9
#define VERSION_JPN_REV0      10
#define VERSION_JPN_REV0_V11  11
#define VERSION_JPN_REV1      12
#define VERSION_JPN_REV2      13
#define VERSION_KOR_DLP       14
#define VERSION_KOR_REV1      15
#define VERSION_KOR_REV2      16
#define VERSION_TWN_DLP       17
#define VERSION_TWN_REV1      18
#define VERSION_TWN_REV2      19
#define VERSION_USA_DLP       20
#define VERSION_USA_KIOSK     21
#define VERSION_USA_REV0      22
#define VERSION_USA_REV0_V11  23
#define VERSION_USA_REV1      24
#define VERSION_USA_REV2      25

// All v1.1 versions
#if GAME_VERSION == VERSION_EUR_REV0_V11 || \
    GAME_VERSION == VERSION_EUR_REV1     || \
    GAME_VERSION == VERSION_JPN_REV0_V11 || \
    GAME_VERSION == VERSION_JPN_REV1     || \
    GAME_VERSION == VERSION_USA_REV0_V11 || \
    GAME_VERSION == VERSION_USA_REV1     || \
    GAME_VERSION == VERSION_CHN_REV1     || \
    GAME_VERSION == VERSION_KOR_REV1     || \
    GAME_VERSION == VERSION_TWN_REV1
#define IS_V11 1
#else
#define IS_V11 0
#endif

// All v1.0 versions
#if GAME_VERSION == VERSION_EUR_REV0 || \
    GAME_VERSION == VERSION_JPN_REV0 || \
    GAME_VERSION == VERSION_USA_REV0
#define IS_V10 1
#else
#define IS_V10 0
#endif

#endif