/*
 * @(#)game.h
 *
 * Copyright 2001-2002, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                      Charles Kerchner (mailto:chip@ardiri.com)
 * All rights reserved.
 *
 * This file was generated as part of the  "lemmings" program developed
 * for the Palm Computing Platform designed by Palm:
 *
 *   http://www.palm.com/
 *
 * The contents of this file is confidential and proprietrary in nature
 * ("Confidential Information"). Redistribution or modification without
 * prior consent of the original author is prohibited.
 */

#ifndef _GAME_H
#define _GAME_H

#include "palm.h"

#define SPR_WIDTH       16
#define SPR_HEIGHT      16
#define LEMMING_XOFFSET 8
#define LEMMING_YOFFSET 16
#define LEMMING_FOOTX   8
#define LEMMING_FOOTY   14    // feet located at line 14 in sprite
#define LEMMING_HEADX   8
#define LEMMING_HEADY   5     // head located at line 5 in sprite
#define LEMMING_CLIMBY  10    // the 'climb' detection value
#define LEMMING_BLOCKX  2
#define LEMMING_BLOCKY  4
#define BLOCK_WIDTH     (SPR_WIDTH - (LEMMING_BLOCKX << 1))
#define BLOCK_HEIGHT    SPR_HEIGHT
#define CURSOR_XOFFSET  7
#define CURSOR_YOFFSET  7

enum SpriteType
{
  spr_undefined = 0,
  spr_walkRight,
  spr_walkLeft,
  spr_fallRight,
  spr_fallLeft,
  spr_digger,
  spr_bashRight,
  spr_bashLeft,
  spr_floatRight,
  spr_floatLeft,
  spr_blocker,
  spr_exploder,
  spr_splatter,
  spr_homer,
  spr_climbRight,
  spr_climbLeft,
  spr_hopupRight,
  spr_hopupLeft,
  spr_buildRight,
  spr_buildLeft,
  spr_waveRight,
  spr_waveLeft,
  spr_mineRight,
  spr_mineLeft,
  spr_drowner
};

enum MaskType
{
  msk_undefined = 0
};

enum SoundType
{
  snd_letsgo = 0,
  snd_changetool,
  snd_selection,
  snd_ohno,
  snd_yippee,
  snd_explode,
  snd_splat,
  snd_ting
};

#define SPR_WALK_CNT   8
#define SPR_FALL_CNT   4
#define SPR_DIGGER_CNT 16
#define SPR_BASH_CNT   32
#define SPR_FLOAT_CNT  8
#define SPR_BLOCK_CNT  16
#define SPR_XPLODE_CNT 32
#define SPR_SPLAT_CNT  16
#define SPR_HOMER_CNT  8
#define SPR_CLIMB_CNT  8
#define SPR_HOPUP_CNT  8
#define SPR_BUILD_CNT  16
#define SPR_WAVE_CNT   8
#define SPR_MINE_CNT   24
#define SPR_DROWN_CNT  16

#define MSK_BASH_CNT   4

extern Boolean GameInitialize()                                       __GAME__;
extern void    GamePause(PreferencesType *, Boolean)                  __GAME__;
extern void    GameChangeTool(PreferencesType *, UInt16, UInt16)      __GAME__;
extern void    GamePlaySound(PreferencesType *, UInt8 sound)          __GAME__;
extern void    GameWideScreen(PreferencesType *)                      __GAME__;
extern UInt16  GameGetLevelCount()                                    __GAME__;
extern void    GameAdjustLemmingRate(PreferencesType *, Boolean)      __GAME__;
extern void    GameLoadLevel(PreferencesType *)                       __GAME__;
extern void    GameSaveLevel(PreferencesType *)                       __GAME__;
extern void    GameResetPreferences(PreferencesType *)                __GAME__;
extern void    _GameResetPreferences(PreferencesType *)            __PROTECT__;
extern void    GameProcessKeyInput(PreferencesType *, UInt32)         __GAME__;
extern void    GameProcessStylusInput(PreferencesType *,
                                      Coord, Coord, Boolean)          __GAME__;
#if CHEAT_MODE
extern void    GameCheat(PreferencesType *, UInt16)                   __GAME__;
#endif

#if PALM_AUDIO_ANY
extern void    GameSfxInitialize()                                    __GAME__;
extern void    GameSfxPlay(PreferencesType *, UInt8)                  __GAME__;
extern void    GameSfxPause(PreferencesType *, Boolean)               __GAME__;
extern void    GameSfxTerminate()                                     __GAME__;
#endif

extern void    GameMusicInitialize()                                  __GAME__;
extern void    GameMusicLoad(PreferencesType *)                       __GAME__;
extern void    GameMusicReset(PreferencesType *)                      __GAME__;
extern void    GameMusicFlagInterrupted(PreferencesType *)            __GAME__;
extern void    GameMusicPlayback(PreferencesType *)                   __GAME__;
extern void    GameMusicEventCallback()                               __GAME__;
extern void    GameMusicPause(PreferencesType *, Boolean)             __GAME__;
extern void    GameMusicStop(PreferencesType *)                       __GAME__;
extern void    GameMusicTerminate()                                   __GAME__;
extern void    GameMovement(PreferencesType *)                        __GAME__;
extern void    GameDraw(PreferencesType *)                            __GAME__;
extern void    GameTerminate()                                        __GAME__;

#endif
