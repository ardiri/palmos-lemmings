/*
 * @(#)palm.h
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
 * ("Confidential Information"). redistribution or modification without
 * prior consent of the original author is prohibited.
 */

#ifndef _PALM_H
#define _PALM_H

// system includes
#include <PalmOS.h>
#include <System/DLServer.h>
#include <VFSMgr.h>

// resource "include" :P
#include "resource.h"

// special includes (additional hardware etc)
#include "hardware/GamePad.h"
#include "hardware/SonyHR.h"
#include "hardware/PalmHDD.h"
#include "hardware/HanderaVGA.h"
#include "hardware/PalmChars.h"
#include "hardware/HanderaChars.h"
#include "hardware/SonyChars.h"

// application constants and structures
#define appCreator             'LEMM'
#define cacheMusicFileName     "LEMM_musc_buffer"
#define cacheGameFileName      "LEMM_game_buffer"
#define cacheHelpFileName      "LEMM_help_buffer"
#define savedGameFileName      "LEMM_!SAVED_GAME!"
#define cacheType              'temp'
#define savedGameType          'save'
#define levlType               'levl'
#define lemmSprites            '_spt'
#define levlCount              'levl'
#define levlTitle              'titl'
#define levlMusic              'musc'
#define levlMidi               'midi'
#define levlAccessCode         'code'
#define levlLemmingsStartX     'xofs'
#define levlLemmingsStartY     'yofs'
#define levlLemmingsExitX      '_exx'
#define levlLemmingsExitY      '_exy'
#define levlTaskEasy           'easy'
#define levlTaskHard           'hard'
#define levlPalette            'PALT'
#define levlMask               'mask'
#define levlGraphics           '_gfx'
#define levlFlame1X            '_f1x'
#define levlFlame1Y            '_f1y'
#define levlFlame2X            '_f2x'
#define levlFlame2Y            '_f2y'

#define __REGISTER__ __attribute__ ((section ("register"))) // code0002.bin
#define __DEVICE__   __attribute__ ((section ("device")))   // code0003.bin
#define __GAME__     __attribute__ ((section ("game")))     // code0004.bin
#define __HELP__     __attribute__ ((section ("help")))     // code0005.bin
#define __PROTECT__  __attribute__ ((section ("protect")))  // code0006.bin

#define ftrGlobalsCfgActiveVol 1000

#define VFS_DIRECTORY          "/Palm/Launcher/"
#define MAX_LEMMINGS           50   // 50 lemmings is ABS max!
#define MAX_LEMMINGS_WIN       100
#define MAX_STATUS             8    // XXXX (info) + X:XX (time)
#define MAX_PAUSE              4    // XXXX (pause)
#define MAX_CHEAT              4    // XXXX (cheat!)
#define MAX_FLAMES             2    // the flames above the exit
#define MAX_PACKS              16   // the maximum number of packs to exist
#define MAX_LEVELS             32   // the maximum number of levels to exist
#define GAME_FPS               12   // 12 FPS for 'second counter'
#define GAME_FPS_x10           125  // 12.5 frames per second
#define TASK_SIZE              44   // @@@: ... :@@@
#define VERSION                0

#ifdef MDM_DISTRIBUTION
#define VFS_MDM_DIRECTORY      "/Palm/Lemmings/"
#endif

#define OFFSCREEN_WIDTH        640
#define OFFSCREEN_HEIGHT       128
#define SCREEN_WIDTH_GENERIC   160
#define SCREEN_START_GENERIC   16
#define SCREEN_WIDTH_HANDERA   240
#define SCREEN_START_HANDERA   56
#define SCREEN_WIDTH_SONY      320
#define SCREEN_START_SONY      96
#define SCREEN_WIDTH_PALMHD    320
#define SCREEN_START_PALMHD    96
#define SCROLL_OFFSET          32

#define SCREEN_HEIGHT          128 // 160x128 visible display area (game)
#define SCREEN_PAUSE_Y1        ((SCREEN_HEIGHT-SPR_HEIGHT) >> 1)
#define SCREEN_PAUSE_Y2        (SCREEN_PAUSE_Y1+SPR_HEIGHT)
#define SCREEN_CHEAT_Y1        4
#define SCREEN_CHEAT_Y2        (SCREEN_CHEAT_Y1+SPR_HEIGHT)
#define SPRITE_HEIGHT          848
#define SPRITE_WIDTH           128

#define LEMMING_MIN_RATE       95
#define LEMMING_MAX_RATE       5
#define LEMMING_INC_RATE       5

enum appEvents
{
  appUpdateEvent = firstUserEvent
};

enum LevelPackType
{
  PACK_INTERNAL = 0,
  PACK_RAM,
  PACK_VFS,
#ifdef MDM_DISTRIBUTION
  PACK_VFS_MDM  = 16
#endif
};

enum GameStateType
{
  GAME_START = 0,
  GAME_PLAY,
  GAME_END
};

#if CHEAT_MODE
enum GameCheatType
{
  CHEAT_ENABLE = 0,                     // "lemmings"  = enable cheats
  CHEAT_TOOL,                           // "psygnosis" = +10 more tools!
  CHEAT_TIME,                           // "yippee"    = +1 min more time
  CHEAT_RATE,                           // "ohno"      = out rate = 5 (fast)
  CHEAT_INVINCIBLE,                     // "godmode"   = invincibility!
  CHEAT_NUKE,                           // "nukem"     = same as [NUKE]
  CHEAT_COUNT
};

#define MAX_ENTRY      32
extern Char *cheatStr[];

#endif

enum ToolType
{
  TOOL_CLIMBER = 0,
  TOOL_FLOATER,
  TOOL_EXPLODER,
  TOOL_BLOCKER,
  TOOL_BUILDER,
  TOOL_BASHER,
  TOOL_MINER,
  TOOL_DIGGER,
  TOOL_NUKE,
  TOOL_COUNT    // must be last
};

typedef struct
{
  Boolean      alive;                   // is the lemming still alive? :)

  Boolean      visible;                 // is the lemming visual on screen?
  Int16        x;
  Int16        y;                       // the (x,y) co-ordinate of the lemming
  UInt8        spriteType;              // the sprite type to display
  UInt8        spritePos;               // the sprite position to display
  UInt8        animCounter;             // the animation counter

  Boolean      nuke;                    // is this dude being nuked?
  UInt8        nukeInTicks;             // how many ticks until i should nuke?
  UInt8        nukeCounter;             // the nuke counter

  Boolean      facingRight;             // is this dude facing right?
  UInt8        fallDistance;            // the falling distance of a lemming

  struct
  {
    Boolean    climber;                 // lemming can climb up walls
    Boolean    floater;                 // lemming can float if falling too far
  } flags;

//UInt8        padding[0];              // padding to ^2 [help compiler]

} Lemming;

typedef struct
{
  struct
  {
    UInt8          signatureVersion;    // a preference version number
    Char           signature[16];       // a "signature" for decryption

    UInt8          hotSyncChecksum;     // checksum :) [used later]
#ifndef MDM_DISTRIBUTION
    Boolean        showNotice;          // show notice on startup
#endif
  } system;

  struct
  {
    UInt16         ctlKeySelect;        // key definition for select
    UInt16         ctlKeyTool;          // key definition for tool rotation
    UInt16         ctlKeyUp;            // key definition for move up
    UInt16         ctlKeyDown;          // key definition for move down
    UInt16         ctlKeyLeft;          // key definition for move left
    UInt16         ctlKeyRight;         // key definition for move right

    Boolean        graffitiScroll;      // scroll using graffiti?
    Boolean        widescreenDisplay;   // wide screen display?

    Boolean        sndMute;             // sound is muted?
    Boolean        musicEnabled;        // music is enabled?
    UInt16         sndVolume;           // the volume setting for sound

    UInt8          lgray;               // the light gray configuration setting
    UInt8          dgray;               // the dark gray configuration setting
  } config;

  struct
  {
    UInt8          type;                // level pack type?
    Char           strLevelPack[32];    // the level pack name
    Boolean        useDifficult;        // play "hard" over "easy" :)
  } levelPack;

  struct
  {
    UInt16         gameState;           // the game state?
    Boolean        gamePlaying;         // is there a game in play?
    Boolean        gamePaused;          // is the game currently paused?
    UInt16         gameScore;           // the score of the player

    UInt16         gameLevel;           // the level of the game :)
    UInt16         activeTool;          // which tool is active?

    // level information [for user view]
    Char           levelTitle[32];      // the title of the level
    UInt16         lemmingOut;          // the # of lemmings out
    UInt16         lemmingReq;          // the # of lemmings required
    UInt16         tools[TOOL_COUNT];   // the number of 'tools' available

    UInt8          blockerCount;
    UInt8          blckID[MAX_LEMMINGS];// which lemmings are blockers?

    UInt16         lemmingStartX;
    UInt16         lemmingStartY;       // the (x,y) co-ords of start position
    UInt16         lemmingExitX;
    UInt16         lemmingExitY;        // the (x,y) co-ords of home position
    UInt16         lemmingStartRate;    // the rate in which lemmings come out
    UInt16         lemmingCount;        // the current number of lemmings out
    UInt16         lemmingDead;         // the number of lemmings dead
    UInt16         lemmingSaved;        // the number of lemmings saved
    UInt16         timeRemaining;       // the total time remaining (secs)

    UInt16         lemmingRate;         // the rate in which lemmings come out

    Boolean        flamesVisible;       // are the flames on-screen?
    Coord          flamesX[MAX_FLAMES];
    Coord          flamesY[MAX_FLAMES]; // the flame positions in the level

    UInt16         lemmingLastOutAt;    // the last time lemming was released
    UInt16         animationCounter;    // the animation counter for the game

    Boolean        nukeOccuring;        // is their a nuke in progress?

    struct
    {
      Int16        x;
      Int16        y;                   // the (x,y) position of the cursor
      Int16        screenOffset;        // the scrolling offset

      UInt8        lemming;             // the lemming highlighted :)
      UInt8        spriteID;            // sprite the cursor is over
    } cursor;

    Lemming        lemming[MAX_LEMMINGS];

#if CHEAT_MODE
    struct
    {
      Boolean      active;              // are the cheats active?
      UInt8        dataOffset;          // offset into data array :)
      Char         dataEntry[MAX_ENTRY];// the data that the user has entered

      Boolean      invincible;          // are the lemmings invincible?
    } cheat;
#endif

    struct
    {
      Boolean      playing;             // is the music playing?

      struct
      {
        Boolean    musicInterrupted;    // was the music interupted?
        UInt16     frameCountDown;      // countdown until next note
        UInt16     frameIndex;          // the note to play next
      } core;

    } music;

  } game;

#if HANDERA_NATIVE
  struct
  {
    Boolean           device;           // are we running on handera device?
    VgaScreenModeType scrMode;          // current mode (scale, 1:1 etc)
    VgaRotateModeType scrRotate;        // current display rotation
  } handera;
#endif

#if SONY_NATIVE
  struct
  {
    Boolean        device;              // are we running on sony device?
    UInt16         libRef;              // the sony HR library reference
  } sony;
#endif

#if PALM_HIDENSITY
  struct
  {
    Boolean        device;              // are we running on palm hi-density device?

    UInt32         width;
    UInt32         height;
    UInt16         density;             // display properties
  } palmHD;
#endif

} PreferencesType;

// helper macro's
#define ABS(a) (((a) < 0) ? -(a) : (a))

// this is our 'double check' for decryption - make sure it worked :P
#define CHECK_SIGNATURE(x) (StrCompare(x->system.signature, "|HaCkMe|") == 0)

// local includes
#include "device.h"
#include "graphics.h"
#include "help.h"
#include "game.h"
#include "register.h"
#include "levelpack.h"

// functions
extern UInt32  PilotMain(UInt16, MemPtr, UInt16);
extern void    InitApplication(void);
extern Boolean ApplicationHandleEvent(EventType *);
extern void    ApplicationDisplayDialog(UInt16);
extern void    EventLoop(void);
extern void    EndApplication(void);

// global variables (argh)
extern Int16   SCREEN_WIDTH_STYLUS;
extern Int16   SCREEN_START_STYLUS;
extern Int16   SCREEN_HEIGHT_STYLUS;
extern Int16   SCREEN_WIDTH;
extern Int16   SCREEN_START;
extern Int16   SCREEN_TOOL_SPEED_X;
extern Int16   SCREEN_TOOL_COUNT_X;
extern Int16   SCREEN_TOOL_START_X;
extern Int16   SCREEN_TOOL_START_Y;
extern Int16   SCREEN_TOOL_WIDTH;
extern Int16   SCREEN_TOOL_HEIGHT;

//
// standard 160x160 constant values:
//

// #define SCREEN_START_STYLUS    SCREEN_START_GENERIC
// #define SCREEN_WIDTH_STYLUS    SCREEN_WIDTH_GENERIC
// #define SCREEN_HEIGHT_STYLUS   SCREEN_HEIGHT
// #define SCREEN_START           SCREEN_START_GENERIC
// #define SCREEN_WIDTH           SCREEN_WIDTH_GENERIC
// #define SCREEN_TOOL_SPEED_X    30
// #define SCREEN_TOOL_COUNT_X    44
// #define SCREEN_TOOL_START_X    60
// #define SCREEN_TOOL_START_Y    146
// #define SCREEN_TOOL_WIDTH      11
// #define SCREEN_TOOL_HEIGHT     11

#endif
