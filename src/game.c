/*
 * @(#)game.c
 *
 * Copyright 2001-2002, Aaron Ardiri     (mailto:aaron@ardiri.com),
 *                      Charles Kerchner (mailto:chip@ardiri.com)
 * All rights reserved.
 *
 * This file was generated as part of the  "lemmings" program developed
 * for the Palm Computing Platform designed by Palm:
 *
 *   http://www.palm.com/
 *
 * The contents of this file is confidential and proprietrary in nature
 * ("Confidential Information"). distribution or modification without
 * prior consent of the original author is prohibited.
 */

#include "palm.h"

#ifdef PALM_AUDIO_STREAMING
#include "sfx-callback.h"
#endif

#ifdef PALM_MIDI_STREAMING
#include "midi-callback.h"
#endif

// interface
static void    GameAdjustStylus(PreferencesType *, Coord *, Coord *)  __GAME__;
static void    GameAssignLemmingTask(PreferencesType *)               __GAME__;
static UInt8   GameGetMaskPixel(UInt16, UInt16)                       __GAME__;
static Boolean GameObstacleAtPosition(UInt16, UInt16)                 __GAME__;
static Boolean GameDiggableAtPosition(UInt16, UInt16, Boolean)        __GAME__;
static Boolean GameBlockerAtPosition(PreferencesType *,
                                     UInt16, UInt16, UInt16)          __GAME__;

// game_gfx.inc
static void    GameLoadBackgroundBitmap2bpp(MemHandle, UInt32)        __DEVICE__;
static void    GameLoadBackgroundBitmap4bpc(MemHandle, UInt32)        __DEVICE__;
static void    GameLoadBackgroundMask4bpp(MemHandle, UInt32,
                                          UInt8 *, Boolean)           __DEVICE__;
#if USE_CHIP_COMPRESS
static void    GameLoadBackgroundMask2bpp(MemHandle, UInt32, UInt8 *) __DEVICE__;
#endif

// game_spr.inc
#if USE_PALMOS_WINAPI
static void    GameBackupTile(Int16, Int16, Int16, Int16, WinHandle)  __GAME__;
static void    GameRestoreTile(Int16, Int16, Int16, Int16, WinHandle) __GAME__;
static void    GameGetLemmingRectangle(UInt8, UInt8, RectangleType *) __GAME__;
#else
static void    GameDrawSprite2bpp(UInt8 *,UInt16,UInt16,Int16,Int16)  __GAME__;
static void    GameSpriteRestore2bpp(UInt8 *, Int16, Int16)           __GAME__;
static void    GameDrawSprite4bpc(UInt8 *,UInt16,UInt16,Int16,Int16)  __GAME__;
static void    GameSpriteRestore4bpc(UInt8 *, Int16, Int16)           __GAME__;
#endif

// game_msk.inc
#if !USE_PALMOS_WINAPI
static void    GameDrawMask2bpp(UInt8 *, Int16, Int16)                __GAME__;
static void    GameDrawMask4bpc(UInt8 *, Int16, Int16)                __GAME__;
#endif
static void    GameDrawMaskMask(UInt8 *, Int16, Int16)                __GAME__;
static void    GameDrawDrawMask(UInt8 *, Int16, Int16)                __GAME__;
static void    GameGenerateMask2bpp(UInt16, UInt16, Int16, Int16,
                                    UInt8 *, UInt8 *, Boolean)        __GAME__;
static void    GameGenerateMask4bpc(UInt16, UInt16, Int16, Int16,
                                    UInt8 *, UInt8 *, Boolean)        __GAME__;

// game_cmp.inc
static void    GameEncodingInflate(UInt8 *, UInt8 *, UInt16)          __GAME__;
static void    GameEncodingDeflate(UInt8 *, UInt16, UInt8 *)          __GAME__;
static UInt16  GameEncodingInflateLength(UInt8 *)                     __GAME__;
static UInt16  GameEncodingDeflateLength(UInt8 *, UInt16)             __GAME__;

// global variable structure
typedef struct
{
  WinHandle    winLemmings;                 // the lemmings bitmap
  WinHandle    winLemmingsMask;             // the lemmings mask bitmap
  WinHandle    winGameMasks;                // the game mask images (screen)
#if NEW_MEMORY_MGR
  UInt16       dbCard;
  LocalID      dbID;
  DmOpenRef    dbMemoryStore;               // the memory storage database

  MemHandle    hanLemmings;                 // the lemmings bitmap
  MemHandle    hanLemmingsMask;             // the lemmings mask bitmap
  MemHandle    hanGameMasks;                // the game mask images (screen)
#endif
  WinHandle    winGameMasks2bpp;            // the game mask (2bpp = for mask)
  BitmapType   *bmpGameMasks2bpp;           // the game mask (2bpp = for mask)

  WinHandle    winLemmingsBackup[MAX_LEMMINGS_WIN]; // the lemmings backup
  WinHandle    winStatusBackup[MAX_STATUS]; // the status areas backup bitmap
  WinHandle    winFlameBackup[MAX_FLAMES];  // the flame (exit) backup bitmap
  WinHandle    winPauseBackup[MAX_PAUSE];   // the pause area backup bitmaps
  WinHandle    winCheatBackup[MAX_CHEAT];   // the cheat area backup bitmaps
  WinHandle    winCursorBackup;             // the cursor backup bitmap

  UInt8        *levelMask;                  // the level mask

  UInt8        *ptrWinDraw;                 // reference to offscreen window
  UInt8        *ptrLemmings;                // ptr - lemmings bitmap
  UInt8        *ptrLemmingsMask;            // ptr - lemmings mask bitmap
  UInt8        *ptrLemmingsBackup[MAX_LEMMINGS_WIN]; // ptr - lemmings backup
  UInt8        *ptrStatusBackup[MAX_STATUS];// ptr - status areas backup bitmap
  UInt8        *ptrFlameBackup[MAX_FLAMES]; // ptr - flame (exit) backup bitmap
  UInt8        *ptrPauseBackup[MAX_PAUSE];  // ptr - pause areas backup bitmap
  UInt8        *ptrCheatBackup[MAX_CHEAT];  // ptr - cheat areas backup bitmap
  UInt8        *ptrCursorBackup;            // ptr - cursor backup bitmap

  UInt8        *ptrGameMasks;               // the game masks
  UInt8        *ptrGameMasks2bpp;           // the game masks (2bpp = for mask)

  WinHandle    winMaskTemporary[2];         // the temporary mask bitmaps
  WinHandle    winScratch;                  // temp backup window (scratch)
  UInt8        *ptrMaskMask;                // the mask mask sprite
  UInt8        *ptrDispMask;                // the display mask sprite
  UInt8        *ptrScratch;                 // temp backup window (scratch)

  UInt8        backupWidth;                 // sprite backup width

  UInt32       scrDepth;                    // the depth of the display
  UInt16       millisecsPerFrame;           // the number of milliseconds/frame

  UInt16       levelCount;                  // how many levels are there?

  UInt32       lastKeyPress;                // the last known key press
  UInt32       lastKeyPressCount;           // how many times last key pressed?
  UInt32       lastKeyDelayCount;           // the last known key press counter

  struct
  {
    Coord      oldCursorX;
    Coord      oldCursorY;                  // the old cursor position
    UInt16     oldSpriteID;                 // cursor sprite indicator?

    UInt16     rateCounter;                 // rate counter (== 0, draw tool)

#if !FULL_SCREEN_BLIT
    Boolean    dirty;                       // do we need a complete redraw?
    Int16      screenOffset;                // the last known screen offset
    Coord      drawY1;
    Coord      drawY2;                      // y1-y2 drawing band for update
#endif
  } display;

  struct
  {
    Boolean    device;                      // is the gamepad driver present
    UInt16     libRef;                      // library reference for gamepad
  } gamePad;

  struct
  {
    UInt16     dbCard;
    LocalID    dbID;
    DmOpenRef  dbMemoryStore;               // the memory storage database
    UInt16     musicCount;                  // how many music pieces are there?

    struct
    {
      Boolean  device;

      UInt16   musicSize;                   // the size of the music
      UInt16  *musicData;                   // the music data chunk
    } core;

#ifdef PALM_MIDI_STREAMING
    struct
    {
      Boolean    device;

      MemHandle  armH;                      // midi-callback armlet handle
      MemHandle  midiH;                     // midi handle, for playback

      MemHandle  default_inst_sampleH;
      MemHandle  default_drum_sampleH;
      MemHandle  inst_sampleH[128];
      MemHandle  drum_sampleH[128];         // instrument and drum samples required

      UInt32    *globals;                   // the midi globals
      Boolean    active;
      UInt32     stream;                    // the audio stream reference (SndStreamRef)
    } palm_midi_streaming;
#endif

#ifdef PALM_MIDI_YAMAHAPA1
    struct
    {
      Boolean    device;
      Boolean    sony_enhanced_audio;       // special flag for new ADPCM manager?

      UInt16     libRef;                    // library reference
      MemHandle  midiH;                     // midi handle, for playback

      UInt8      midiHInternal;             // libraries internal 'handle' reference
      UInt32     midiPosition;              // important for the ability to pause playback

      Boolean    active;
    } yamaha_midi;
#endif
  } music;

#if PALM_AUDIO_ANY
  struct
  {
#if PALM_AUDIO_STREAMING
    struct
    {
      Boolean    device;

      MemHandle  armH;                      // sfx-callback armlet handle
      UInt32    *globals;                   // the sfx globals
      UInt32     stream;                    // the audio stream reference (SndStreamRef)

      MemHandle  audioH[MAX_AUDIO];         // handles to wav resources
      void      *audioP[MAX_AUDIO];         // locked pointers to pcm resources
    } palm_sfx_streaming;
#endif

#if PALM_AUDIO_YAMAHAPA1
    struct
    {
      Boolean    device;
      Boolean    sony_enhanced_audio;       // special flag for new ADPCM manager?

      UInt16     libRef;                    // library reference

      MemHandle  audioH[MAX_AUDIO];         // handles to adpcm wav resources
      void      *audioP[MAX_AUDIO];         // locked pointers to adpcm resources

      UInt8      audioHInternal;            // libraries internal 'handle' reference
    } yamaha_sfx;
#endif

  } audio;
#endif

#if !USE_PALMOS_WINAPI
  void         (*fnGameDrawMask)(UInt8 *, Int16, Int16);
  void         (*fnGameDrawSprite)(UInt8 *, UInt16, UInt16, Int16, Int16);
  void         (*fnGameSpriteRestore)(UInt8 *, Int16, Int16);
#endif
  void         (*fnGameGenerateMask)(UInt16, UInt16, Int16, Int16,
                                     UInt8 *,UInt8 *, Boolean);

} GameGlobals;

static GameGlobals globals;

//
// support code:
// - sprite routines
// - gfx decompression/compression routines
//
// -- Aaron Ardiri, 2001

#include "game_gfx.inc"   // game gfx decompression algorithms
#include "game_spr.inc"   // sprite engine
#include "game_msk.inc"   // mask engine (similar to sprite)
#include "game_cmp.inc"   // game save/restore (de)compression routines

/**
 * Initialize the Game.
 *
 * @return true if game is initialized, false otherwise.
 */
Boolean
GameInitialize()
{
  Err       err, e;
  UInt16    i;
#if NEW_MEMORY_MGR
  UInt16    index, rowBytes, binSize;
  WinHandle winHandle;
#endif

  // clear the globals structure
  MemSet(&globals, sizeof(GameGlobals), 0);

  // load the gamepad driver if available
  {
    Err err;

    // attempt to load the library
    err = SysLibFind(GPD_LIB_NAME,&globals.gamePad.libRef);
    if (err == sysErrLibNotFound)
      err = SysLibLoad('libr',GPD_LIB_CREATOR,&globals.gamePad.libRef);

    // lets determine if it is available
    globals.gamePad.device = (err == errNone);

    // open the library if available
    if (globals.gamePad.device)
      GPDOpen(globals.gamePad.libRef);
  }

  // determine the screen depth
  globals.scrDepth = 1;
  if (DeviceSupportsVersion(romVersion3))
    WinScreenMode(winScreenModeGet,NULL,NULL,&globals.scrDepth,NULL);

  err = errNone;

  // initialize timing routines
  globals.millisecsPerFrame  = (10000) / GAME_FPS_x10;

  // initialize the "bitmap" windows
#if NEW_MEMORY_MGR
  // pre PALMOS_5 - there aint enough heap - hack time
  if (!DeviceSupportsVersion(romVersion5))
  {
    globals.hanLemmings     = NULL; globals.winLemmings     = NULL;
    globals.hanLemmingsMask = NULL; globals.winLemmingsMask = NULL;
    globals.hanGameMasks    = NULL; globals.winGameMasks    = NULL;

    // create the 'memory' store for the static images [kill previous]
    globals.dbCard = 0;
    globals.dbID   = DmFindDatabase(globals.dbCard, cacheGameFileName);
    if (globals.dbID != NULL)
      DmDeleteDatabase(globals.dbCard, globals.dbID);

    err |= (DmCreateDatabase(globals.dbCard, cacheGameFileName,
                             appCreator, cacheType, true) != errNone);
    if (err == errNone)
    {
      globals.dbID = DmFindDatabase(globals.dbCard, cacheGameFileName);
      globals.dbMemoryStore =
        DmOpenDatabase(globals.dbCard, globals.dbID, dmModeReadWrite);

      rowBytes = (((SPRITE_WIDTH * globals.scrDepth) + 15) >> 4) << 1;

      // palmos 3.5+
      if (DeviceSupportsVersion(romVersion3_5))
      {
        BitmapType *bmpPtr;

        // lemmings
        index = 0;
        binSize = sizeof(BitmapType) + (rowBytes * SPRITE_HEIGHT);
        globals.hanLemmings =
          DmNewResource(globals.dbMemoryStore, cacheType, index, binSize);
        err |= (globals.hanLemmings == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;

        MemSemaphoreReserve(true);
        bmpPtr = (BitmapType *)MemHandleLock(globals.hanLemmings);
        MemSet(bmpPtr, binSize, 0);
        bmpPtr->width     = SPRITE_WIDTH;
        bmpPtr->height    = SPRITE_HEIGHT;
        bmpPtr->rowBytes  = rowBytes;
        bmpPtr->pixelSize = globals.scrDepth;
        bmpPtr->version   = BitmapVersionTwo;
        MemHandleUnlock(globals.hanLemmings);
        DmReleaseResource(globals.hanLemmings);
        MemSemaphoreRelease(true);

        // lemmings mask
        index++;
        binSize = sizeof(BitmapType) + (rowBytes * SPRITE_HEIGHT);
        globals.hanLemmingsMask =
          DmNewResource(globals.dbMemoryStore, cacheType, index, binSize);
        err |= (globals.hanLemmingsMask == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;

        MemSemaphoreReserve(true);
        bmpPtr = (BitmapType *)MemHandleLock(globals.hanLemmingsMask);
        MemSet(bmpPtr, binSize, 0);
        bmpPtr->width     = SPRITE_WIDTH;
        bmpPtr->height    = SPRITE_HEIGHT;
        bmpPtr->rowBytes  = rowBytes;
        bmpPtr->pixelSize = globals.scrDepth;
        bmpPtr->version   = BitmapVersionTwo;
        MemHandleUnlock(globals.hanLemmingsMask);
        DmReleaseResource(globals.hanLemmingsMask);
        MemSemaphoreRelease(true);

        // games mask
        index++;
        binSize = sizeof(BitmapType) + (rowBytes * 32);
        globals.hanGameMasks =
          DmNewResource(globals.dbMemoryStore, cacheType, index, binSize);
        err |= (globals.hanGameMasks == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;

        MemSemaphoreReserve(true);
        bmpPtr = (BitmapType *)MemHandleLock(globals.hanGameMasks);
        MemSet(bmpPtr, binSize, 0);
        bmpPtr->width     = SPRITE_WIDTH;
        bmpPtr->height    = 32;
        bmpPtr->rowBytes  = rowBytes;
        bmpPtr->pixelSize = globals.scrDepth;
        bmpPtr->version   = BitmapVersionTwo;
        MemHandleUnlock(globals.hanGameMasks);
        DmReleaseResource(globals.hanGameMasks);
        MemSemaphoreRelease(true);

// the WinCreateBitmapWindow() routine performs some checks on the data
// that is provided when creating a window. for data protection reasons
// the BitmapType* data chunk MUST exist in the dynamic heap - as it is
// normally "written" to by the other API routines - without semaphores
//
// winLemmings, winLemmingsMask and winGameMask are "readonly" graphics
// resources in the game, so, we have to hack around this :) we have to
// fool the Palm OS, by creating a small bitmap, and then replacing the
// bitmap with our "handle" we created earlier.
//
// -- Aaron Ardiri, 2001

        // initialize the bitmap chunks
        bmpPtr = BmpCreate(SPR_WIDTH, SPR_HEIGHT, globals.scrDepth, NULL, &e);
        err |= e;
        globals.winLemmings = WinCreateBitmapWindow(bmpPtr, &e); err |= e;
        err |= (globals.winLemmings == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;
        // f*ck you palm :P - get around that darn limitation
        BmpDelete(bmpPtr);
        bmpPtr = (BitmapType *)MemHandleLock(globals.hanLemmings);
        globals.winLemmings->bitmapP                = bmpPtr;
        globals.winLemmings->windowBounds.extent.x  = bmpPtr->width;
        globals.winLemmings->windowBounds.extent.y  = bmpPtr->height;
        globals.winLemmings->displayWidthV20        = bmpPtr->width;
        globals.winLemmings->displayHeightV20       = bmpPtr->height;
        globals.winLemmings->displayAddrV20         = bmpPtr;
        globals.winLemmings->windowFlags.freeBitmap = false;
        winHandle = WinSetDrawWindow(globals.winLemmings);
        WinResetClip();
        WinSetDrawWindow(winHandle);

        bmpPtr = BmpCreate(SPR_WIDTH, SPR_HEIGHT, globals.scrDepth, NULL, &e);
        err |= e;
        globals.winLemmingsMask = WinCreateBitmapWindow(bmpPtr, &e); err |= e;
        err |= (globals.winLemmingsMask == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;
         // f*ck you palm :P - get around that darn limitation
        BmpDelete(bmpPtr);
        bmpPtr = (BitmapType *)MemHandleLock(globals.hanLemmingsMask);
        globals.winLemmingsMask->bitmapP                = bmpPtr;
        globals.winLemmingsMask->windowBounds.extent.x  = bmpPtr->width;
        globals.winLemmingsMask->windowBounds.extent.y  = bmpPtr->height;
        globals.winLemmingsMask->displayWidthV20        = bmpPtr->width;
        globals.winLemmingsMask->displayHeightV20       = bmpPtr->height;
        globals.winLemmingsMask->displayAddrV20         = bmpPtr;
        globals.winLemmingsMask->windowFlags.freeBitmap = false;
        winHandle = WinSetDrawWindow(globals.winLemmingsMask);
        WinResetClip();
        WinSetDrawWindow(winHandle);

        bmpPtr = BmpCreate(SPR_WIDTH, SPR_HEIGHT, globals.scrDepth, NULL, &e);
        err |= e;
        globals.winGameMasks = WinCreateBitmapWindow(bmpPtr, &e); err |= e;
        err |= (globals.winGameMasks == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;
        // f*ck you palm :P - get around that darn limitation
        BmpDelete(bmpPtr);
        bmpPtr = (BitmapType *)MemHandleLock(globals.hanGameMasks);
        globals.winGameMasks->bitmapP                = bmpPtr;
        globals.winGameMasks->windowBounds.extent.x  = bmpPtr->width;
        globals.winGameMasks->windowBounds.extent.y  = bmpPtr->height;
        globals.winGameMasks->displayWidthV20        = bmpPtr->width;
        globals.winGameMasks->displayHeightV20       = bmpPtr->height;
        globals.winGameMasks->displayAddrV20         = bmpPtr;
        globals.winGameMasks->windowFlags.freeBitmap = false;
        winHandle = WinSetDrawWindow(globals.winGameMasks);
        WinResetClip();
        WinSetDrawWindow(winHandle);
      }

      // pre palmos 3.5
      else
      {
        GDeviceType *gDevicePtr;

        // lemmings
        index = 0;
        binSize = (rowBytes * SPRITE_HEIGHT);
        globals.hanLemmings =
          DmNewResource(globals.dbMemoryStore, cacheType, index, binSize);
        err |= (globals.hanLemmings == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;

        // lemmings mask
        index++;
        binSize = (rowBytes * SPRITE_HEIGHT);
        globals.hanLemmingsMask =
          DmNewResource(globals.dbMemoryStore, cacheType, index, binSize);
        err |= (globals.hanLemmingsMask == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;

        // games mask
        index++;
        binSize = (rowBytes * 32);
        globals.hanGameMasks =
          DmNewResource(globals.dbMemoryStore, cacheType, index, binSize);
        err |= (globals.hanGameMasks == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;

// the WinCreateOffscreenWindow() routine creates a memory buffer in an
// area of dynamic memory (ram) which, is very valuable to us *eek* :P,
// and, it is normally "written" to by the other API routines - without
// semaphores - so we have to simulate that :P
//
// winLemmings, winLemmingsMask and winGameMask are "readonly" graphics
// resources in the game, so, we have to hack around this :) we have to
// fool the Palm OS, by creating a small bitmap, and then replacing the
// bitmap with our "handle" we created earlier.
//
// -- Aaron Ardiri, 2001

        // initialize the bitmap chunks
        globals.winLemmings =
          WinCreateOffscreenWindow(SPR_WIDTH, SPR_HEIGHT, screenFormat, &e);
        err |= e; err |= (globals.winLemmings == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;
        // f*ck you palm :P - get around that darn limitation
        gDevicePtr = (GDeviceType *)globals.winLemmings->bitmapP;
        MemPtrFree(gDevicePtr->baseAddr);
        gDevicePtr->baseAddr = MemHandleLock(globals.hanLemmings);
        gDevicePtr->width                          = SPRITE_WIDTH;
        gDevicePtr->height                         = SPRITE_HEIGHT;
        gDevicePtr->rowBytes                       = rowBytes;
        gDevicePtr->version                        = BitmapVersionZero;
        globals.winLemmings->windowBounds.extent.x = gDevicePtr->width;
        globals.winLemmings->windowBounds.extent.y = gDevicePtr->height;
        globals.winLemmings->displayWidthV20       = gDevicePtr->width;
        globals.winLemmings->displayHeightV20      = gDevicePtr->height;
        globals.winLemmings->displayAddrV20        = gDevicePtr->baseAddr;
        globals.winLemmings->windowFlags.offscreen = false;
        winHandle = WinSetDrawWindow(globals.winLemmings);
        WinResetClip();
        WinSetDrawWindow(winHandle);

        globals.winLemmingsMask =
          WinCreateOffscreenWindow(SPR_WIDTH, SPR_HEIGHT, screenFormat, &e);
        err |= e; err |= (globals.winLemmingsMask == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;
        // f*ck you palm :P - get around that darn limitation
        gDevicePtr = (GDeviceType *)globals.winLemmingsMask->bitmapP;
        MemPtrFree(gDevicePtr->baseAddr);
        gDevicePtr->baseAddr = MemHandleLock(globals.hanLemmingsMask);
        gDevicePtr->width                              = SPRITE_WIDTH;
        gDevicePtr->height                             = SPRITE_HEIGHT;
        gDevicePtr->rowBytes                           = rowBytes;
        gDevicePtr->version                            = BitmapVersionZero;
        globals.winLemmingsMask->windowBounds.extent.x = gDevicePtr->width;
        globals.winLemmingsMask->windowBounds.extent.y = gDevicePtr->height;
        globals.winLemmingsMask->displayWidthV20       = gDevicePtr->width;
        globals.winLemmingsMask->displayHeightV20      = gDevicePtr->height;
        globals.winLemmingsMask->displayAddrV20        = gDevicePtr->baseAddr;
        globals.winLemmingsMask->windowFlags.offscreen = false;
        winHandle = WinSetDrawWindow(globals.winLemmingsMask);
        WinResetClip();
        WinSetDrawWindow(winHandle);

        globals.winGameMasks =
          WinCreateOffscreenWindow(SPR_WIDTH, SPR_HEIGHT, screenFormat, &e);
        err |= e; err |= (globals.winGameMasks == NULL);
        if (err != errNone) goto GFX_MEMORY_QUIT;
        // f*ck you palm :P - get around that darn limitation
        gDevicePtr = (GDeviceType *)globals.winGameMasks->bitmapP;
        MemPtrFree(gDevicePtr->baseAddr);
        gDevicePtr->baseAddr = MemHandleLock(globals.hanGameMasks);
        gDevicePtr->width                           = SPRITE_WIDTH;
        gDevicePtr->height                          = 32;
        gDevicePtr->rowBytes                        = rowBytes;
        gDevicePtr->version                         = BitmapVersionZero;
        globals.winGameMasks->windowBounds.extent.x = gDevicePtr->width;
        globals.winGameMasks->windowBounds.extent.y = gDevicePtr->height;
        globals.winGameMasks->displayWidthV20       = gDevicePtr->width;
        globals.winGameMasks->displayHeightV20      = gDevicePtr->height;
        globals.winGameMasks->displayAddrV20        = gDevicePtr->baseAddr;
        globals.winGameMasks->windowFlags.offscreen = false;
        winHandle = WinSetDrawWindow(globals.winGameMasks);
        WinResetClip();
        WinSetDrawWindow(winHandle);
      }

GFX_MEMORY_QUIT:

      // protect + close the database :)
      DmDatabaseProtect(globals.dbCard, globals.dbID, true);
      DmCloseDatabase(globals.dbMemoryStore);
    }
  }

  // post PALMOS_5 - there may be enough heap :)
  else
  {
    globals.winLemmings =
      WinCreateOffscreenWindow(SPRITE_WIDTH, SPRITE_HEIGHT, screenFormat, &e); err |= e;
    err |= (globals.winLemmings == NULL);
    globals.winLemmingsMask =
      WinCreateOffscreenWindow(SPRITE_WIDTH, SPRITE_HEIGHT, screenFormat, &e); err |= e;
    err |= (globals.winLemmingsMask == NULL);
    globals.winGameMasks =
      WinCreateOffscreenWindow(SPRITE_WIDTH, 32, screenFormat, &e); err |= e;
    err |= (globals.winGameMasks == NULL);
  }
#else
  globals.winLemmings =
    WinCreateOffscreenWindow(SPRITE_WIDTH, SPRITE_HEIGHT, screenFormat, &e); err |= e;
  err |= (globals.winLemmings == NULL);
  globals.winLemmingsMask =
    WinCreateOffscreenWindow(SPRITE_WIDTH, SPRITE_HEIGHT, screenFormat, &e); err |= e;
  err |= (globals.winLemmingsMask == NULL);
  globals.winGameMasks =
    WinCreateOffscreenWindow(SPRITE_WIDTH, 32, screenFormat, &e); err |= e;
  err |= (globals.winGameMasks == NULL);
#endif
  if (DeviceSupportsColor())
  {
    globals.bmpGameMasks2bpp =
      BmpCreate(SPRITE_WIDTH, 32, 2, NULL, &e); err |= e;
    globals.winGameMasks2bpp =
      WinCreateBitmapWindow(globals.bmpGameMasks2bpp, &e); err |= e;
    err |= (globals.winGameMasks2bpp == NULL);
  }

#if USE_PALMOS_WINAPI
  globals.backupWidth = SPR_WIDTH;
#else
#if PORTABLE
  globals.backupWidth = SPR_WIDTH+1;
#else
  globals.backupWidth = DeviceSupportsColor() ?
                         SPR_WIDTH + 4 : SPR_WIDTH + 8;
#endif
#endif
  for (i=0; i < MAX_LEMMINGS_WIN; i++)
  {
    globals.winLemmingsBackup[i] =
      WinCreateOffscreenWindow(globals.backupWidth,SPR_HEIGHT,screenFormat,&e);
    err |= e; err |= (globals.winLemmingsBackup[i] == NULL);
  }
  for (i=0; i < MAX_STATUS; i++)
  {
    globals.winStatusBackup[i] =
      WinCreateOffscreenWindow(globals.backupWidth,SPR_HEIGHT,screenFormat,&e);
    err |= e; err |= (globals.winStatusBackup[i] == NULL);
  }
  for (i=0; i < MAX_FLAMES; i++)
  {
    globals.winFlameBackup[i] =
      WinCreateOffscreenWindow(globals.backupWidth,SPR_HEIGHT,screenFormat,&e);
    err |= e; err |= (globals.winFlameBackup[i] == NULL);
  }
  for (i=0; i < MAX_PAUSE; i++)
  {
    globals.winPauseBackup[i] =
      WinCreateOffscreenWindow(globals.backupWidth,SPR_HEIGHT,screenFormat,&e);
    err |= e; err |= (globals.winPauseBackup[i] == NULL);
  }
  for (i=0; i < MAX_CHEAT; i++)
  {
    globals.winCheatBackup[i] =
      WinCreateOffscreenWindow(globals.backupWidth,SPR_HEIGHT,screenFormat,&e);
    err |= e; err |= (globals.winCheatBackup[i] == NULL);
  }
  globals.winCursorBackup =
    WinCreateOffscreenWindow(globals.backupWidth,SPR_HEIGHT,screenFormat,&e);
  err |= e; err |= (globals.winCursorBackup == NULL);

  for (i=0; i < 2; i++)
  {
    globals.winMaskTemporary[i] =
      WinCreateOffscreenWindow(SPR_WIDTH, SPR_HEIGHT, screenFormat, &e);
    err |= e; err |= (globals.winMaskTemporary[i] == NULL);
  }
  globals.winScratch =
    WinCreateOffscreenWindow(globals.backupWidth,SPR_HEIGHT,screenFormat,&e);
  err |= e; err |= (globals.winScratch == NULL);

  // allocate memory for level mask [+SPR_HEIGHT*2+1 = no check (above+below)]
#if SHOW_MASK_IN_2BPP
  if (DeviceSupportsColor())
#endif
  {
    globals.levelMask =
      MemPtrNew((OFFSCREEN_WIDTH >> 2) * (SCREEN_HEIGHT + ((SPR_HEIGHT*2)+1)));
    err |= (globals.levelMask == NULL);
  }

  // no problems creating buffers? load images.
  if (err == errNone)
  {
    WinHandle currWindow;
    MemHandle bitmapHandle;

    currWindow = WinGetDrawWindow();

    // clear the level mask
#if SHOW_MASK_IN_2BPP
    if (DeviceSupportsColor())
#endif
      MemSet(globals.levelMask, sizeof(globals.levelMask), 0);

    // lets setup some pointers [optimization]
    globals.ptrWinDraw      =
      DeviceWindowGetPointer(GraphicsGetDrawWindow());
    globals.ptrLemmings     =
      DeviceWindowGetPointer(globals.winLemmings);
    globals.ptrLemmingsMask =
      DeviceWindowGetPointer(globals.winLemmingsMask);

    for (i=0; i < MAX_LEMMINGS_WIN; i++)
    {
      globals.ptrLemmingsBackup[i] =
        DeviceWindowGetPointer(globals.winLemmingsBackup[i]);
    }
    for (i=0; i < MAX_STATUS; i++)
    {
      globals.ptrStatusBackup[i] =
        DeviceWindowGetPointer(globals.winStatusBackup[i]);
    }
    for (i=0; i < MAX_FLAMES; i++)
    {
      globals.ptrFlameBackup[i] =
        DeviceWindowGetPointer(globals.winFlameBackup[i]);
    }
    for (i=0; i < MAX_PAUSE; i++)
    {
      globals.ptrPauseBackup[i] =
        DeviceWindowGetPointer(globals.winPauseBackup[i]);
    }
    for (i=0; i < MAX_CHEAT; i++)
    {
      globals.ptrCheatBackup[i] =
        DeviceWindowGetPointer(globals.winCheatBackup[i]);
    }
    globals.ptrCursorBackup =
      DeviceWindowGetPointer(globals.winCursorBackup);

    globals.ptrMaskMask = DeviceWindowGetPointer(globals.winMaskTemporary[0]);
    globals.ptrDispMask = DeviceWindowGetPointer(globals.winMaskTemporary[1]);
    globals.ptrScratch =
      DeviceWindowGetPointer(globals.winScratch);

    globals.ptrGameMasks =
      DeviceWindowGetPointer(globals.winGameMasks);
    if (DeviceSupportsColor())
    {
      globals.ptrGameMasks2bpp =
        DeviceWindowGetPointer(globals.winGameMasks2bpp);
    }
    else
      globals.ptrGameMasks2bpp = globals.ptrGameMasks;

#if SHOW_MASK_IN_2BPP
    if (!DeviceSupportsColor())
      globals.levelMask = globals.ptrWinDraw;
#endif

#if NEW_MEMORY_MGR
    // pre PALMOS_5 - need memory semaphore for writing to storage heap
    if (!DeviceSupportsVersion(romVersion5))
      MemSemaphoreReserve(true);
#endif

    // lemming sprites
#if USE_CHIP_COMPRESS
    {
      UInt8  *maskPtr, *sptPtr;
      UInt8  mask;
      UInt16 i;

      sptPtr  = globals.ptrLemmings;
      maskPtr = globals.ptrLemmingsMask;

      // convert colored bitmap into lemming masks
      bitmapHandle = DmGetResource(lemmSprites, bitmapLemmingsColor);
      if (DeviceSupportsColor())
      {
        GameLoadBackgroundMask4bpp(bitmapHandle,
          ((UInt32)(SPRITE_WIDTH >> 1) * SPRITE_HEIGHT), sptPtr, false);

        i = (UInt16)(SPRITE_WIDTH >> 1) * SPRITE_HEIGHT;

        do
        {
          mask = *sptPtr++;
          if (mask & 0x0f) mask |= 0x0f;
          if (mask & 0xf0) mask |= 0xf0;
          *maskPtr++ = mask;
        }
        while (--i);
      }
      else
      {
        GameLoadBackgroundMask2bpp(bitmapHandle,
          ((UInt32)(SPRITE_WIDTH >> 2) * SPRITE_HEIGHT), sptPtr);

        i = (UInt16)(SPRITE_WIDTH >> 2) * SPRITE_HEIGHT;

        do
        {
          mask = *sptPtr++;
          if (mask & 0x03) mask |= 0x03;
          if (mask & 0x0c) mask |= 0x0c;
          if (mask & 0x30) mask |= 0x30;
          if (mask & 0xc0) mask |= 0xc0;
          *maskPtr++ = mask;
        }
        while (--i);
      }

      DmReleaseResource(bitmapHandle);
    }
#else
    WinSetDrawWindow(globals.winLemmings);
    bitmapHandle = DmGetResource('Tbmp', bitmapLemmings);
    WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), 0, 0);
    MemHandleUnlock(bitmapHandle);
    DmReleaseResource(bitmapHandle);

    WinSetDrawWindow(globals.winLemmingsMask);
    bitmapHandle = DmGetResource('Tbmp', bitmapLemmingsMask);
    WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), 0, 0);
    MemHandleUnlock(bitmapHandle);
    DmReleaseResource(bitmapHandle);
#endif

    // game masks
    WinSetDrawWindow(globals.winGameMasks);
    bitmapHandle = DmGetResource('Tbmp', bitmapGameMasks);
    WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), 0, 0);
    MemHandleUnlock(bitmapHandle);
    DmReleaseResource(bitmapHandle);
    if (DeviceSupportsColor())
    {
      WinSetDrawWindow(globals.winGameMasks2bpp);
      bitmapHandle = DmGetResource('Tbmp', bitmapGameMasks);
      WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), 0, 0);
      MemHandleUnlock(bitmapHandle);
      DmReleaseResource(bitmapHandle);
    }

#if NEW_MEMORY_MGR
    // pre PALMOS_5 - release memory semaphore for writing to storage heap
    if (!DeviceSupportsVersion(romVersion5))
      MemSemaphoreRelease(true);
#endif

    WinSetDrawWindow(currWindow);

#if !USE_PALMOS_WINAPI
    // lets setup the dynamic functions
    globals.fnGameDrawMask      = DeviceSupportsColor()
                                   ? (void *)GameDrawMask4bpc
                                   : (void *)GameDrawMask2bpp;
    globals.fnGameDrawSprite    = DeviceSupportsColor()
                                   ? (void *)GameDrawSprite4bpc
                                   : (void *)GameDrawSprite2bpp;
    globals.fnGameSpriteRestore = DeviceSupportsColor()
                                   ? (void *)GameSpriteRestore4bpc
                                   : (void *)GameSpriteRestore2bpp;
#endif
    globals.fnGameGenerateMask  = DeviceSupportsColor()
                                   ? (void *)GameGenerateMask4bpc
                                   : (void *)GameGenerateMask2bpp;
  }

  // initialize the audio+music engine
#if PALM_AUDIO_ANY
  GameSfxInitialize();
#endif
  GameMusicInitialize();

  return (err == errNone);
}

/**
 * Set the pause state of the game (gotta repaint a small area)
 *
 * @param prefs the global preference data.
 * @param state the pause state.
 */
void
GamePause(PreferencesType *prefs, Boolean state)
{
#if !FULL_SCREEN_BLIT
  // we gotta repaint the pause area (erase the junk)
  if (prefs->game.gamePaused)
  {
    GraphicsSetUpdate(SCREEN_PAUSE_Y1, SCREEN_PAUSE_Y2);
    GraphicsRepaint();

    // may need to force redraw
    globals.display.dirty = true;
  }
#endif

  // set the state
  prefs->game.gamePaused = state;

#if MIDI_PAUSE_ON_DIALOG
  GameMusicPause(prefs, prefs->game.gamePaused);
#endif
}

/**
 * Change the active tool selected.
 *
 * @param prefs   the global preference data.
 * @param newTool the new tool to change to.
 * @param oldVal  the old tool count value.
 */
void
GameChangeTool(PreferencesType *prefs, UInt16 newTool, UInt16 oldVal)
{
  RectangleType rect;
  Char          str[3];

  rect.extent.x  = SCREEN_TOOL_WIDTH - 1;
  rect.extent.y  = SCREEN_TOOL_HEIGHT - 1;
  rect.topLeft.x =
    (prefs->game.activeTool * SCREEN_TOOL_WIDTH) + SCREEN_TOOL_START_X;
  rect.topLeft.y =
    DeviceSupportsColor() ? SCREEN_TOOL_START_Y + 2 : SCREEN_TOOL_START_Y + 3;

  //
  // erase old
  //

  // erase rectangle outline
  if (newTool != prefs->game.activeTool)
  {
    WinInvertRectangleFrame(simpleFrame, &rect);

    prefs->game.activeTool = newTool;
    rect.topLeft.x =
      (prefs->game.activeTool * SCREEN_TOOL_WIDTH) + SCREEN_TOOL_START_X;

    // 'tool change' audio playback
    GamePlaySound(prefs, snd_changetool);
  }

  //
  // draw new
  //

#if HANDERA_NATIVE
  if (prefs->handera.device)
    FntSetFont(VgaBaseToVgaFont(boldFont));
  else
#endif
    FntSetFont(boldFont);

  // need it with leading zero
  StrPrintF(str, "%02d", prefs->game.tools[prefs->game.activeTool]);

  // draw rectangle outline & draw the string
  if (DeviceSupportsColor())
  {
    WinEraseRectangleFrame(simpleFrame, &rect);
    WinDrawInvertedChars(str, 2, SCREEN_TOOL_COUNT_X, rect.topLeft.y-1);
  }
  else
  {
    WinDrawRectangleFrame(simpleFrame, &rect);
    WinDrawChars(str, 2, SCREEN_TOOL_COUNT_X, rect.topLeft.y-1);
  }
  FntSetFont(stdFont);

  // we have to wait 4 frames before using this again
  globals.lastKeyDelayCount = 4;
  globals.lastKeyPress      = prefs->config.ctlKeyTool;
}

/**
 * Play a sound in the game.
 *
 * @param prefs the global preference data.
 * @param sound the sound type to play.
 */
void
GamePlaySound(PreferencesType *prefs, UInt8 sound)
{
  SndCommandType splatSnd = {sndCmdFreqDurationAmp,0,  64,30,sndMaxAmp};
  SndCommandType chinkSnd = {sndCmdFreqDurationAmp,0, 512, 5,sndMaxAmp};
  SndCommandType taskSnd  = {sndCmdFreqDurationAmp,0,1024,5,sndMaxAmp};
  SndCommandType popSnd   = {sndCmdFreqDurationAmp,0,2048,10,sndMaxAmp};

#if PALM_AUDIO_STREAMING
  // wav based audio
  if (globals.audio.palm_sfx_streaming.device)
  {
    GameSfxPlay(prefs, sound);
  }
  else
#endif

#if PALM_AUDIO_YAMAHAPA1
  // adpcm wav based audio
  if (globals.audio.yamaha_sfx.device)
  {
    GameSfxPlay(prefs, sound);
  }

  // old fashioned, beep-style
  else
#endif
  {
    switch (sound)
    {
      case snd_selection:
      case snd_changetool:
           DevicePlaySound(&taskSnd);

           // we have interrupted music!
           GameMusicFlagInterrupted(prefs);
           break;

      // play the explode sound
      case snd_explode:
           DevicePlaySound(&popSnd);

           // we have interrupted music!
           GameMusicFlagInterrupted(prefs);
           break;

      // play the splat sound
      case snd_splat:
           DevicePlaySound(&splatSnd);

           // we have interrupted music!
           GameMusicFlagInterrupted(prefs);
           break;

      // play the chink sound
      case snd_ting:
           DevicePlaySound(&chinkSnd);

           // we have interrupted music!
           GameMusicFlagInterrupted(prefs);
           break;

      // no sounds for these :( sorry
      case snd_letsgo:
      case snd_ohno:
      case snd_yippee:
      default:
           break;
    }
  }
}

/**
 * Adjust widescreen display mode property.
 *
 * @param prefs the global preferences structure.
 */
void
GameWideScreen(PreferencesType *prefs)
{
  // lets adjust the graphics display
  GraphicsWideScreen(prefs->config.widescreenDisplay);

#if HANDERA_NATIVE
  if (prefs->handera.device)
  {
    if ((prefs->config.widescreenDisplay) || (DeviceSupportsColor()))
    {
      SCREEN_WIDTH_STYLUS  = SCREEN_WIDTH_HANDERA;
      SCREEN_START_STYLUS  = SCREEN_START_HANDERA;
      SCREEN_HEIGHT_STYLUS = SCREEN_HEIGHT;
      SCREEN_WIDTH         = SCREEN_WIDTH_HANDERA;
      SCREEN_START         = SCREEN_START_HANDERA;
    }
    else
    {
      SCREEN_WIDTH_STYLUS  = SCREEN_WIDTH_HANDERA;
      SCREEN_START_STYLUS  = SCREEN_START_GENERIC;
      SCREEN_HEIGHT_STYLUS = SCREEN_HEIGHT;
      SCREEN_WIDTH         = SCREEN_WIDTH_GENERIC;
      SCREEN_START         = SCREEN_START_GENERIC;
    }
    SCREEN_TOOL_SPEED_X  = 45;
    SCREEN_TOOL_COUNT_X  = 66;
    SCREEN_TOOL_START_X  = 86;
    SCREEN_TOOL_START_Y  = 220;
    SCREEN_TOOL_WIDTH    = 17;
    SCREEN_TOOL_HEIGHT   = 17;                   // x1.5
  }
  else
#endif
#if SONY_NATIVE
  if ((prefs->sony.device) &&
      (prefs->config.widescreenDisplay))
  {
    SCREEN_WIDTH_STYLUS  = SCREEN_WIDTH_GENERIC;
    SCREEN_START_STYLUS  = SCREEN_START_SONY >> 1;
    SCREEN_HEIGHT_STYLUS = SCREEN_HEIGHT >> 1;
    SCREEN_WIDTH         = SCREEN_WIDTH_SONY;
    SCREEN_START         = SCREEN_START_SONY;
    SCREEN_TOOL_SPEED_X  = 30;
    SCREEN_TOOL_COUNT_X  = 44;
    SCREEN_TOOL_START_X  = 60;
    SCREEN_TOOL_START_Y  = 146;
    SCREEN_TOOL_WIDTH    = 11;
    SCREEN_TOOL_HEIGHT   = 11;                   // sony pixel is still 160x160
  }
  else
#endif
#if PALM_HIDENSITY
  if ((prefs->palmHD.device) &&
      (prefs->config.widescreenDisplay))
  {
    switch (prefs->palmHD.density)
    {
      case kDensityDouble:
           SCREEN_WIDTH_STYLUS  = SCREEN_WIDTH_GENERIC;
           SCREEN_START_STYLUS  = SCREEN_START_PALMHD >> 1;
           SCREEN_HEIGHT_STYLUS = SCREEN_HEIGHT >> 1;
           SCREEN_WIDTH         = SCREEN_WIDTH_PALMHD;
           SCREEN_START         = SCREEN_START_PALMHD >> 1;
           break;

      case kDensityLow:
      default:
           SCREEN_WIDTH_STYLUS  = SCREEN_WIDTH_GENERIC;
           SCREEN_START_STYLUS  = SCREEN_START_GENERIC;
           SCREEN_HEIGHT_STYLUS = SCREEN_HEIGHT;
           SCREEN_WIDTH         = SCREEN_WIDTH_GENERIC;
           SCREEN_START         = SCREEN_START_GENERIC;
           break;
    }
    SCREEN_TOOL_SPEED_X  = 30;
    SCREEN_TOOL_COUNT_X  = 44;
    SCREEN_TOOL_START_X  = 60;
    SCREEN_TOOL_START_Y  = 146;
    SCREEN_TOOL_WIDTH    = 11;
    SCREEN_TOOL_HEIGHT   = 11;                   // palmHD pixel still 160x160
  }
  else
#endif
  {
    SCREEN_WIDTH_STYLUS  = SCREEN_WIDTH_GENERIC;
    SCREEN_START_STYLUS  = SCREEN_START_GENERIC;
    SCREEN_HEIGHT_STYLUS = SCREEN_HEIGHT;
    SCREEN_WIDTH         = SCREEN_WIDTH_GENERIC;
    SCREEN_START         = SCREEN_START_GENERIC;
    SCREEN_TOOL_SPEED_X  = 30;
    SCREEN_TOOL_COUNT_X  = 44;
    SCREEN_TOOL_START_X  = 60;
    SCREEN_TOOL_START_Y  = 146;
    SCREEN_TOOL_WIDTH    = 11;
    SCREEN_TOOL_HEIGHT   = 11;
  }
}

/**
 * How many levels are currently available?
 *
 * @return the amount of levels available.
 */
UInt16
GameGetLevelCount()
{
  return globals.levelCount;
}

/**
 * Adjust the lemming output rate (from the entry point into game)
 *
 * @param prefs the global preferences structure.
 * @param increase true if to speed up output rate, false otherwise
 */
void
GameAdjustLemmingRate(PreferencesType *prefs, Boolean increase)
{
  RectangleType rect;
  Char          str[3];

  // lets make sure there has been some delay in this processing
  if (
      (globals.lastKeyPress != prefs->config.ctlKeyTool) ||
      (globals.lastKeyDelayCount == 0)
     )
  {
    //  5 = 95 frames/lemming  = min
    // 50 = 50 frames/lemming
    // 75 = 25 frames/lemming
    // 95 =  5 frames/lemming  = max

    // increase?
    if ((increase) && (prefs->game.lemmingRate > LEMMING_MAX_RATE))
     prefs->game.lemmingRate -= LEMMING_INC_RATE;

    // decrease?
    else
    if ((!increase) &&
        (prefs->game.lemmingRate < LEMMING_MIN_RATE) &&
        (prefs->game.lemmingRate < prefs->game.lemmingStartRate)) // not lower
     prefs->game.lemmingRate += LEMMING_INC_RATE;

    // draw the rate counter
    rect.extent.x  = SCREEN_TOOL_WIDTH - 1;
    rect.extent.y  = SCREEN_TOOL_HEIGHT - 1;
    rect.topLeft.x =
      (prefs->game.activeTool * SCREEN_TOOL_WIDTH) + SCREEN_TOOL_START_X;
    rect.topLeft.y =
      DeviceSupportsColor() ? SCREEN_TOOL_START_Y + 2 : SCREEN_TOOL_START_Y + 3;

#if HANDERA_NATIVE
    if (prefs->handera.device)
      FntSetFont(VgaBaseToVgaFont(boldFont));
    else
#endif
      FntSetFont(boldFont);

    // need it with leading zero
    StrPrintF(str, "%02d", 100 - prefs->game.lemmingRate);

    // draw the string
    if (DeviceSupportsColor())
      WinDrawInvertedChars(str, 2, SCREEN_TOOL_COUNT_X, rect.topLeft.y-1);
    else
      WinDrawChars(str, 2, SCREEN_TOOL_COUNT_X, rect.topLeft.y-1);

    FntSetFont(stdFont);

    // it should stay highlighted for at least a second
    globals.display.rateCounter = GAME_FPS;

    // we have to wait 4 frames before using this again
    globals.lastKeyDelayCount = 4;
    globals.lastKeyPress      = prefs->config.ctlKeyTool;

    // 'selection' audio playback
    GamePlaySound(prefs, snd_selection);
  }
}

/**
 * Load the currently active level into memory, from database or reset
 *
 * @param prefs the global preference data.
 */
void
GameLoadLevel(PreferencesType *prefs)
{
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
  const RectangleType     rect  = {{0,16},{160,128}};
  MemHandle memHandle;

  // gotta clear the graphics window totally
  GraphicsClear();

  //
  // SAVED GAME
  //

  if ((prefs->game.gamePlaying) &&
      (DmFindDatabase(0, savedGameFileName) != 0))
  {
    LocalID   dbID;
    UInt16    card;
    DmOpenRef dbRef;
    MemHandle binHandle;
    UInt16    index, binSize;
    Boolean   loadOk;

    // open it for reading
    card  = 0;
    dbID  = DmFindDatabase(card, savedGameFileName);
    dbRef = DmOpenDatabase(card, dbID, dmModeReadOnly);

    // lets make sure the "save state is ok"
    loadOk = (DmNumRecords(dbRef) == 2);                // we save 2 things
    if (!loadOk) goto LOAD_FAIL;

    // load the game graphics
    index = 0;
    if (DeviceSupportsColor())
      binSize = ((UInt16)(OFFSCREEN_HEIGHT + ((SPR_HEIGHT*2)+1)) * 320);
    else
      binSize = ((UInt16)(OFFSCREEN_HEIGHT + ((SPR_HEIGHT*2)+1)) * 160);
    binHandle = DmGetRecord(dbRef, index);
    if (binHandle != NULL)
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
      if (GameEncodingInflateLength(buffer) == binSize)
        GameEncodingInflate(buffer, globals.ptrWinDraw, binSize);
      else loadOk &= false;

      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

    // load the mask graphics
    index++;

    binSize = ((UInt16)(OFFSCREEN_HEIGHT + ((SPR_HEIGHT*2)+1)) * 160);
    binHandle = DmGetRecord(dbRef, index);
    if (binHandle != NULL)
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
      if (GameEncodingInflateLength(buffer) == binSize)
        GameEncodingInflate(buffer, globals.levelMask, binSize);
      else loadOk &= false;

      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

LOAD_FAIL:

    // close and delete database
    DmCloseDatabase(dbRef);
    DmDeleteDatabase(card, dbID);              // remove saved game

    // something bad happened, oh well, reset
    if (!loadOk)
    {
      FrmAlert(loadGameAlert);

      // reset the level, everything is screwed up
      GameResetPreferences(prefs);
      goto GAME_RESET;
    }
  }

  //
  // STARTING A NEW GAME
  //

  else
  {
#if SHOW_DECOMPRESSION_TIME
    Char   message[64];
    UInt32 t1, t2;

    t1 = TimGetTicks();
#endif

GAME_RESET:

    // load the graphics for the level
    memHandle = LevelPackGetResource(levlGraphics, prefs->game.gameLevel);
    if (DeviceSupportsColor())
    {
#if USE_CHIP_COMPRESS
      GameLoadBackgroundBitmap4bpc(memHandle,
                                   ((UInt32)(OFFSCREEN_WIDTH) * SCREEN_HEIGHT));
#else
      GameLoadBackgroundBitmap4bpc(memHandle, (UInt32)MemHandleSize(memHandle));
#endif
    }
    else
    {
#if USE_CHIP_COMPRESS
      GameLoadBackgroundBitmap2bpp(memHandle,
                                   ((UInt32)(OFFSCREEN_WIDTH) * SCREEN_HEIGHT));
#else
      GameLoadBackgroundBitmap2bpp(memHandle, (UInt32)MemHandleSize(memHandle));
#endif
    }
    LevelPackReleaseResource(memHandle);

#if SHOW_DECOMPRESSION_TIME
    t1 = TimGetTicks() - t1;
    t2 = TimGetTicks();
#endif

    memHandle = LevelPackGetResource(levlMask, prefs->game.gameLevel);
#if USE_CHIP_COMPRESS
    GameLoadBackgroundMask4bpp(memHandle,
                               ((UInt32)(OFFSCREEN_WIDTH >> 1) * SCREEN_HEIGHT),
                               globals.levelMask, true);
#else
    GameLoadBackgroundMask4bpp(memHandle, (UInt32)MemHandleSize(memHandle),
                               globals.levelMask, true);
#endif
    LevelPackReleaseResource(memHandle);

#if SHOW_DECOMPRESSION_TIME
    t2 = TimGetTicks() - t2;
    StrPrintF(message,
              "Decomp Time: _GFX = %3ld ticks, MASK = %3ld ticks", t1, t2);
    SysFatalAlert(message);
#endif
  }

  // set the palette as appropriate
  if (DeviceSupportsColor())
  {
    // clear the LCD screen (dont want palette flash)
    WinSetDrawWindow(WinGetDisplayWindow());
    WinSetPattern(&erase);
    WinFillRectangle(&rect, 0);

    memHandle = LevelPackGetResource('PALT', prefs->game.gameLevel);
    WinPalette(winPaletteSet, 0, 16,
               (RGBColorType *)MemHandleLock(memHandle));
    MemHandleUnlock(memHandle);
    LevelPackReleaseResource(memHandle);
  }

  // how many levels are available?
  memHandle = LevelPackGetResource(levlCount, 0);
  globals.levelCount = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

  // load the music!
  GameMusicLoad(prefs);
}

/**
 * Save the currently active level into a database for later.
 *
 * @param prefs the global preference data.
 */
void
GameSaveLevel(PreferencesType *prefs)
{
  // save the game if playing
  if (prefs->game.gamePlaying)
  {
    UInt16 card;

    // create and open it for writing
    card  = 0;
    if (DmCreateDatabase(card, savedGameFileName,
                         appCreator, savedGameType, false) == errNone)
    {
      LocalID   dbID;
      DmOpenRef dbRef;
      MemHandle binHandle;
      UInt16    index, binSize, winSize;
      Boolean   saveOk;

      dbID  = DmFindDatabase(card, savedGameFileName);
      dbRef = DmOpenDatabase(card, dbID, dmModeReadWrite);

      saveOk = true;

      // save the game graphics
      index = 0;
      if (DeviceSupportsColor())
        winSize = ((UInt16)(OFFSCREEN_HEIGHT + ((SPR_HEIGHT*2)+1)) * 320);
      else
        winSize = ((UInt16)(OFFSCREEN_HEIGHT + ((SPR_HEIGHT*2)+1)) * 160);
      binSize   = GameEncodingDeflateLength(globals.ptrWinDraw, winSize);
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL)
      {
        // pre PALMOS_5 - there aint enough heap - hack time
        if (!DeviceSupportsVersion(romVersion5))
        {
          UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);

          // compress it (directly to database)
          MemSemaphoreReserve(true);
          GameEncodingDeflate(globals.ptrWinDraw, winSize, buffer);
          MemSemaphoreRelease(true);
        }

        // post PALMOS_5 - there may be enough heap :)
        else
        {
          UInt8 *buffer = (UInt8 *)MemPtrNew(binSize);

          // error check
          if (buffer == NULL) saveOk &= false;
          else
          {
            // compress it (via temporary buffer)
            GameEncodingDeflate(globals.ptrWinDraw, winSize, buffer);
            DmWrite(MemHandleLock(binHandle), 0, buffer, binSize);

            MemPtrFree(buffer);
          }
        }

        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else saveOk &= false;

      // save the mask graphics
      index++;
      winSize   = MemPtrSize(globals.levelMask);
      binSize   = GameEncodingDeflateLength(globals.levelMask, winSize);
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL)
      {
        // pre PALMOS_5 - there aint enough heap - hack time
        if (!DeviceSupportsVersion(romVersion5))
        {
          UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);

          // compress it (directly to database)
          MemSemaphoreReserve(true);
          GameEncodingDeflate(globals.levelMask, winSize, buffer);
          MemSemaphoreRelease(true);
        }

        // post PALMOS_5 - there may be enough heap :)
        else
        {
          UInt8 *buffer = (UInt8 *)MemPtrNew(binSize);

          // error check
          if (buffer == NULL) saveOk &= false;
          else
          {
            // compress it (via temporary buffer)
            GameEncodingDeflate(globals.levelMask, winSize, buffer);
            DmWrite(MemHandleLock(binHandle), 0, buffer, binSize);

            MemPtrFree(buffer);
          }
        }

        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else saveOk &= false;

      // close and delete database
      DmCloseDatabase(dbRef);

      // something went wrong?
      if (!saveOk)
      {
        FrmAlert(saveGameAlert);
        DmDeleteDatabase(card, dbID);
      }
    }
  }
}

/**
 * Reset the Game preferences.
 *
 * @param prefs the global preference data.
 */
void
GameResetPreferences(PreferencesType *prefs)
{
  MemHandle dataHandle, keyH;
  UInt8     *ptr;
  Int16     size;
  UInt8     *ptrCodeChunk;
  void      (*resetPrefs)(PreferencesType *);

  // extract the "encrypted" code
  dataHandle    = DmGetResource('code', 0x0006);  // code0006.bin
  ptr  = (UInt8 *)MemHandleLock(dataHandle);
  size = MemHandleSize(dataHandle);
  ptrCodeChunk  = (UInt8 *)MemPtrNew(size);
  MemMove(ptrCodeChunk, ptr, size);
  MemHandleUnlock(dataHandle);
  DmReleaseResource(dataHandle);

  // decrypt the memory chunk (based on the code0002.bin)
  keyH = DmGetResource('code', 0x0002);
  RegisterDecryptChunk(ptrCodeChunk, size, keyH, 0x00);
  DmReleaseResource(keyH);

  // execute the code chunk
  resetPrefs = (void *)ptrCodeChunk;
  resetPrefs(prefs);

  // we dont need the memory anymore, dispose of it
  MemPtrFree(ptrCodeChunk);
}

/**
 * Reset the Game preferences.
 *
 * @param prefs the global preference data.
 */
void
_GameResetPreferences(PreferencesType *prefs)
{
  MemHandle memHandle;
  UInt16    i;
  Lemming   *lemmingPtr;
  Char      *strTask;
  UInt8     strTaskData[TASK_SIZE];
#if PROTECTION_ON
  MemHandle keyH;
#endif

  // now we are playing
  prefs->game.gameState    = GAME_START;
  prefs->game.gamePlaying  = true;
  prefs->game.nukeOccuring = false;  // no nuking going on.. yet :P

  //
  // load the "properties" of the level
  //

  memHandle = LevelPackGetResource(levlTitle, prefs->game.gameLevel);
  StrCopy(prefs->game.levelTitle, MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

  memHandle = LevelPackGetResource(levlLemmingsStartX, prefs->game.gameLevel);
  prefs->game.lemmingStartX = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);
  memHandle = LevelPackGetResource(levlLemmingsStartY, prefs->game.gameLevel);
  prefs->game.lemmingStartY = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

  memHandle = LevelPackGetResource(levlLemmingsExitX, prefs->game.gameLevel);
  prefs->game.lemmingExitX = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);
  memHandle = LevelPackGetResource(levlLemmingsExitY, prefs->game.gameLevel);
  prefs->game.lemmingExitY = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

  // try to load the 'hard' over the 'easy' config..
  if (prefs->levelPack.useDifficult)
  {
    memHandle = LevelPackGetResource(levlTaskHard, prefs->game.gameLevel);
    if (memHandle == NULL)
      memHandle = LevelPackGetResource(levlTaskEasy, prefs->game.gameLevel);
  }
  else
    memHandle = LevelPackGetResource(levlTaskEasy, prefs->game.gameLevel);
  MemSet(strTaskData, TASK_SIZE, 0);
  MemMove(strTaskData, MemHandleLock(memHandle), TASK_SIZE);
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

#if PROTECTION_ON
  if ((prefs->levelPack.type == PACK_RAM) ||
      (prefs->levelPack.type == PACK_VFS))
  {
    keyH = LevelPackGetResource('_key', 0x0000);
    RegisterDecryptChunk(strTaskData, TASK_SIZE, keyH,
                         prefs->system.hotSyncChecksum);
    LevelPackReleaseResource(keyH);
  }
#ifdef MDM_DISTRIBUTION
  else
  if (prefs->levelPack.type == PACK_VFS_MDM)
  {
    UInt8  i, checksum, MDM_key[dlkUserNameBufSize] = { };
    UInt32 MDM_key_length = 0;

    // SPECIAL: hotsync username = 4D:44:4D:2D:63:61:72:64:70 "MDM-card"
    MDM_GetKey(MDM_key, &MDM_key_length);
    MemSet(&MDM_key[MDM_key_length], dlkUserNameBufSize - MDM_key_length, 0);

    checksum = 0;
    for (i=0; i<MAX_IDLENGTH; i++)
      checksum += (UInt8)MDM_key[i];
    checksum &= 0xff;
    if (checksum == 0) checksum = 0x20; // cannot be zero

    keyH = LevelPackGetResource('_key', 0x0000);
    RegisterDecryptChunk(strTaskData, TASK_SIZE, keyH, checksum);
    LevelPackReleaseResource(keyH);
  }
#endif
#endif

  strTask = (Char *)strTaskData + 4; // skip over @@@:
  prefs->game.lemmingStartRate     = StrAToI(strTask); strTask += 3;
  prefs->game.tools[TOOL_CLIMBER]  = StrAToI(strTask); strTask += 3;
  prefs->game.tools[TOOL_FLOATER]  = StrAToI(strTask); strTask += 3;
  prefs->game.tools[TOOL_EXPLODER] = StrAToI(strTask); strTask += 3;
  prefs->game.tools[TOOL_BLOCKER]  = StrAToI(strTask); strTask += 3;
  prefs->game.tools[TOOL_BUILDER]  = StrAToI(strTask); strTask += 3;
  prefs->game.tools[TOOL_BASHER]   = StrAToI(strTask); strTask += 3;
  prefs->game.tools[TOOL_MINER]    = StrAToI(strTask); strTask += 3;
  prefs->game.tools[TOOL_DIGGER]   = StrAToI(strTask); strTask += 3;
  prefs->game.lemmingOut           = StrAToI(strTask); strTask += 3;
  prefs->game.lemmingReq           = StrAToI(strTask); strTask += 3;
  prefs->game.timeRemaining        = StrAToI(strTask);

  prefs->game.lemmingRate          = prefs->game.lemmingStartRate;

  memHandle = LevelPackGetResource(levlFlame1X, prefs->game.gameLevel);
  prefs->game.flamesX[0] = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);
  memHandle = LevelPackGetResource(levlFlame1Y, prefs->game.gameLevel);
  prefs->game.flamesY[0] = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);
  memHandle = LevelPackGetResource(levlFlame2X, prefs->game.gameLevel);
  prefs->game.flamesX[1] = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);
  memHandle = LevelPackGetResource(levlFlame2Y, prefs->game.gameLevel);
  prefs->game.flamesY[1] = StrAToI(MemHandleLock(memHandle));
  MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

  //
  // screen initialiation
  //

  // initialize the cursor :)
  prefs->game.cursor.x = SCREEN_WIDTH  >> 1;
  prefs->game.cursor.y = SCREEN_HEIGHT >> 1;

  // adjust view position to show start in center
  prefs->game.cursor.spriteID     = spr_undefined;
  if (prefs->game.lemmingStartX <= (SCREEN_WIDTH >> 1))
    prefs->game.cursor.screenOffset = 0;
  else
  if (prefs->game.lemmingStartX >= (OFFSCREEN_WIDTH - (SCREEN_WIDTH >> 1)))
    prefs->game.cursor.screenOffset = OFFSCREEN_WIDTH - SCREEN_WIDTH;
  else
    prefs->game.cursor.screenOffset =
      prefs->game.lemmingStartX - (SCREEN_WIDTH >> 1);

  prefs->game.cursor.screenOffset =
    prefs->game.cursor.screenOffset & ~0x07; // bind to 8 pixel boundary

  GraphicsSetOffset(prefs->game.cursor.screenOffset);

  // screen must have a full repaint
  globals.display.oldCursorX   = -1;
  globals.display.oldCursorY   = -1;
  globals.display.oldSpriteID  = -1;
  globals.display.rateCounter  = 0;

#if !FULL_SCREEN_BLIT
  globals.display.dirty        = true;
  globals.display.screenOffset = -1;
  globals.display.drawY1       = 0;
  globals.display.drawY2       = SCREEN_HEIGHT-1;
#endif

  //
  // other initializations
  //

  prefs->game.activeTool        = TOOL_CLIMBER;

#if CHEAT_MODE
  prefs->game.cheat.active      = false;
  prefs->game.cheat.dataOffset  = 0;
  MemSet(prefs->game.cheat.dataEntry, MAX_ENTRY * sizeof(Char), 0);

  prefs->game.cheat.invincible  = false;
#endif

  GameMusicReset(prefs);

  // no lemmings are out yet :P
  prefs->game.lemmingCount      = 0;
  prefs->game.lemmingDead       = 0;
  prefs->game.lemmingSaved      = 0;
  prefs->game.animationCounter  = 0;
  prefs->game.lemmingLastOutAt  = 0;

  // no blockers exist either..
  prefs->game.blockerCount      = 0;
  MemSet(prefs->game.blckID, MAX_LEMMINGS * sizeof(UInt8), 0);

  // initialize the lemmings data structures
  for (i=0; i < MAX_LEMMINGS; i++)
  {
    lemmingPtr = &prefs->game.lemming[i];

    lemmingPtr->alive           = false;
    lemmingPtr->visible         = false;
    lemmingPtr->x               = 0;
    lemmingPtr->y               = 0;
    lemmingPtr->spriteType      = spr_undefined;
    lemmingPtr->spritePos       = 0;
    lemmingPtr->animCounter     = 0;
    lemmingPtr->nuke            = false;
    lemmingPtr->nukeInTicks     = 0;
    lemmingPtr->nukeCounter     = 0;
    lemmingPtr->facingRight     = true;
    lemmingPtr->fallDistance    = 0;

    lemmingPtr->flags.climber   = false;
    lemmingPtr->flags.floater   = false;
  }

  // the game is not paused now
  prefs->game.gamePaused        = false;
}

/**
 * Process key input from the user.
 *
 * @param prefs the global preference data.
 * @param keyStatus the current key state.
 */
void
GameProcessKeyInput(PreferencesType *prefs, UInt32 keyStatus)
{
  keyStatus &= (prefs->config.ctlKeySelect |
                prefs->config.ctlKeyTool   |
                prefs->config.ctlKeyUp     |
                prefs->config.ctlKeyDown   |
                prefs->config.ctlKeyLeft   |
                prefs->config.ctlKeyRight);

  // additonal checks here
  if (globals.gamePad.device)
  {
    UInt8 gamePadKeyStatus;
    Err   err;

    // read the state of the gamepad
    err = GPDReadInstant(globals.gamePad.libRef, &gamePadKeyStatus);
    if (err == errNone) {

      // process
      if  ((gamePadKeyStatus & GAMEPAD_DOWN)      != 0)
        keyStatus |= prefs->config.ctlKeyDown;
      if  ((gamePadKeyStatus & GAMEPAD_UP)        != 0)
        keyStatus |= prefs->config.ctlKeyUp;
      if  ((gamePadKeyStatus & GAMEPAD_LEFT)      != 0)
        keyStatus |= prefs->config.ctlKeyLeft;
      if  ((gamePadKeyStatus & GAMEPAD_RIGHT)     != 0)
        keyStatus |= prefs->config.ctlKeyRight;
      if  ((gamePadKeyStatus & GAMEPAD_LEFTFIRE)  != 0)
        keyStatus |= prefs->config.ctlKeySelect;
      if  ((gamePadKeyStatus & GAMEPAD_RIGHTFIRE) != 0)
        keyStatus |= prefs->config.ctlKeyTool;
    }
  }

  // is the cursor highlighting a lemming and we can task him?
  if (prefs->game.cursor.spriteID != spr_undefined)
  {
    Coord   x, y;
    Boolean penDown;

    EvtGetPen(&x, &y, &penDown);

    // adjust for offset
    GameAdjustStylus(prefs, &x, &y);

    // if the pen is down and within the cursor, we have a task!
    if ((penDown) &&
        (ABS(globals.display.oldCursorX - x) < CURSOR_XOFFSET) &&
        (ABS(globals.display.oldCursorY - y) < CURSOR_YOFFSET))
    {
      keyStatus |= prefs->config.ctlKeySelect;
    }
  }

  // adjust the "last" key press counter? [holding down something]
  if ((keyStatus & globals.lastKeyPress) != 0)
    globals.lastKeyPressCount++;
  else
    globals.lastKeyPressCount = 0;

  // did they press at least one of the game keys?
  if ((keyStatus != 0) && (prefs->game.gamePaused))
    GamePause(prefs, false);

  // tool rotation
  if (
      ((keyStatus & prefs->config.ctlKeyTool) != 0) &&
      (
       ((globals.lastKeyPress & prefs->config.ctlKeyTool) == 0) ||
       (globals.lastKeyDelayCount == 0)
      )
     )
  {
    GameChangeTool(prefs, (prefs->game.activeTool + 1) % TOOL_COUNT,
                   prefs->game.tools[prefs->game.activeTool]);

    // we have to remember this :P
    globals.lastKeyPress = prefs->config.ctlKeyTool;
  }

  // tool usage
  if (
      ((keyStatus & prefs->config.ctlKeySelect) != 0) &&
      (
       ((globals.lastKeyPress & prefs->config.ctlKeySelect) == 0) ||
       (globals.lastKeyDelayCount == 0)
      )
     )
  {
    // assign a task to the lemming
    GameAssignLemmingTask(prefs);

    // we have to remember this :P
    globals.lastKeyPress = prefs->config.ctlKeySelect;
  }

  // move up?
  if (
      ((keyStatus & prefs->config.ctlKeyUp) != 0) &&
      (
       ((globals.lastKeyPress & prefs->config.ctlKeyUp) == 0) ||
       (globals.lastKeyDelayCount == 0)
      )
     )
  {
#if SONY_NATIVE
    if ((prefs->sony.device) &&
        (prefs->config.widescreenDisplay))
    {
      switch (globals.lastKeyPressCount)
      {
        case 0: case 1: prefs->game.cursor.y -= 2;  break;
        case 2: case 3: prefs->game.cursor.y -= 4;  break;
        case 4: case 5:
        case 6: case 7: prefs->game.cursor.y -= 8;  break;
        default:        prefs->game.cursor.y -= 16; break;
      }
    }
    else
#endif
    {
      switch (globals.lastKeyPressCount)
      {
        case 0: case 1: prefs->game.cursor.y--;    break;
        case 2: case 3: prefs->game.cursor.y -= 2; break;
        case 4: case 5:
        case 6: case 7: prefs->game.cursor.y -= 4; break;
        default:        prefs->game.cursor.y -= 8; break;
      }
    }

    // we have to remember this :P
    globals.lastKeyPress |= prefs->config.ctlKeyUp;
  }

  // move up?
  if (
      ((keyStatus & prefs->config.ctlKeyDown) != 0) &&
      (
       ((globals.lastKeyPress & prefs->config.ctlKeyDown) == 0) ||
       (globals.lastKeyDelayCount == 0)
      )
     )
  {
#if SONY_NATIVE
    if ((prefs->sony.device) &&
        (prefs->config.widescreenDisplay))
    {
      switch (globals.lastKeyPressCount)
      {
        case 0: case 1: prefs->game.cursor.y += 2;  break;
        case 2: case 3: prefs->game.cursor.y += 4;  break;
        case 4: case 5:
        case 6: case 7: prefs->game.cursor.y += 8;  break;
        default:        prefs->game.cursor.y += 16; break;
      }
    }
    else
#endif
    {
      switch (globals.lastKeyPressCount)
      {
        case 0: case 1: prefs->game.cursor.y++;    break;
        case 2: case 3: prefs->game.cursor.y += 2; break;
        case 4: case 5:
        case 6: case 7: prefs->game.cursor.y += 4; break;
        default:        prefs->game.cursor.y += 8; break;
      }
    }

    // we have to remember this :P
    globals.lastKeyPress |= prefs->config.ctlKeyDown;
  }

  // move left?
  if (
      ((keyStatus & prefs->config.ctlKeyLeft) != 0) &&
      (
       ((globals.lastKeyPress & prefs->config.ctlKeyLeft) == 0) ||
       (globals.lastKeyDelayCount == 0)
      )
     )
  {
#if SONY_NATIVE
    if ((prefs->sony.device) &&
        (prefs->config.widescreenDisplay))
    {
      switch (globals.lastKeyPressCount)
      {
        case 0: case 1: prefs->game.cursor.x -= 2;  break;
        case 2: case 3: prefs->game.cursor.x -= 4;  break;
        case 4: case 5:
        case 6: case 7: prefs->game.cursor.x -= 8;  break;
        default:        prefs->game.cursor.x -= 16; break;
      }
    }
    else
#endif
    {
      switch (globals.lastKeyPressCount)
      {
        case 0: case 1: prefs->game.cursor.x--;    break;
        case 2: case 3: prefs->game.cursor.x -= 2; break;
        case 4: case 5:
        case 6: case 7: prefs->game.cursor.x -= 4; break;
        default:        prefs->game.cursor.x -= 8; break;
      }
    }

    // we have to remember this :P
    globals.lastKeyPress |= prefs->config.ctlKeyLeft;
  }

  // move right?
  if (
      ((keyStatus & prefs->config.ctlKeyRight) != 0) &&
      (
       ((globals.lastKeyPress & prefs->config.ctlKeyRight) == 0) ||
       (globals.lastKeyDelayCount == 0)
      )
     )
  {
#if SONY_NATIVE
    if ((prefs->sony.device) &&
        (prefs->config.widescreenDisplay))
    {
      switch (globals.lastKeyPressCount)
      {
        case 0: case 1: prefs->game.cursor.x += 2;  break;
        case 2: case 3: prefs->game.cursor.x += 4;  break;
        case 4: case 5:
        case 6: case 7: prefs->game.cursor.x += 8;  break;
        default:        prefs->game.cursor.x += 16; break;
      }
    }
    else
#endif
    {
      switch (globals.lastKeyPressCount)
      {
        case 0: case 1: prefs->game.cursor.x++;    break;
        case 2: case 3: prefs->game.cursor.x += 2; break;
        case 4: case 5:
        case 6: case 7: prefs->game.cursor.x += 4; break;
        default:        prefs->game.cursor.x += 8; break;
      }
    }

    // we have to remember this :P
    globals.lastKeyPress |= prefs->config.ctlKeyRight;
  }

  if (globals.lastKeyDelayCount != 0)
    globals.lastKeyDelayCount--;
}

/**
 * Process stylus input from the user.
 *
 * @param prefs the global preference data.
 * @param x the x co-ordinate of the stylus event.
 * @param y the y co-ordinate of the stylus event.
 * @param move was the penevent a move event?
 */
void
GameProcessStylusInput(PreferencesType *prefs, Coord x, Coord y, Boolean move)
{
  // adjust for offset
  GameAdjustStylus(prefs, &x, &y);

  prefs->game.cursor.x = x;
  prefs->game.cursor.y = y;

  if (prefs->game.gamePaused)
    GamePause(prefs, false);

  // nuke action?
  if (prefs->game.activeTool == TOOL_NUKE)
    GameAssignLemmingTask(prefs);
}

#if CHEAT_MODE
/**
 * Apply a cheat *tut tut* :)
 *
 * @param prefs the global preference data.
 * @param cheat the cheat type to apply.
 */
void
GameCheat(PreferencesType *prefs, UInt16 cheat)
{
  UInt16 i, x;

  switch (cheat)
  {
    case CHEAT_ENABLE:
         prefs->game.cheat.active = true;
         break;

    case CHEAT_TOOL:

         // are we allowed to do this?
         if (prefs->game.cheat.active)
         {
           // adjust all tools [except nuke]
           for (i=0; i<TOOL_COUNT-1; i++)
           {
             x = prefs->game.tools[i];
             prefs->game.tools[i] += 10;       // just scored +10 tools
             if (prefs->game.tools[i] > 99)
               prefs->game.tools[i] = 99;      // limit to 99

             if (i == prefs->game.activeTool)
               GameChangeTool(prefs, i, x);
           }
         }
         break;

    case CHEAT_TIME:

         // are we allowed to do this?
         if (prefs->game.cheat.active)
         {
           prefs->game.timeRemaining += 60;    // you just scored 60 seconds!
           if (prefs->game.timeRemaining > 599)
             prefs->game.timeRemaining = 599;  // max of 9:59
         }
         break;

    case CHEAT_RATE:
         // are we allowed to do this?
         if (prefs->game.cheat.active)
         {
           prefs->game.lemmingRate = 5;        // you asked for em!

           globals.display.rateCounter = GAME_FPS;
         }
         break;

    case CHEAT_INVINCIBLE:
         // are we allowed to do this?
         if (prefs->game.cheat.active)
           prefs->game.cheat.invincible = true;
         break;

    case CHEAT_NUKE:
         // are we allowed to do this?
         if (prefs->game.cheat.active)
         {
           // we must not be in "nuke" mode to start it :P
           if (!prefs->game.nukeOccuring)
           {
             // we are in nuke mode! argh!
             prefs->game.nukeOccuring = true;

             x = 0;
             for (i=0; i<prefs->game.lemmingCount; i++)
             {
               Lemming *lemmingPtr;
               lemmingPtr = &prefs->game.lemming[i];

               lemmingPtr->nuke         = true;
               lemmingPtr->nukeInTicks  = x;
               lemmingPtr->nukeCounter  = 0;     // this guy is getting nuked

               if (lemmingPtr->alive) x++;
             }
           }
         }
         break;

    default:
         break;
  }
}
#endif

#if (PALM_AUDIO_STREAMING || PALM_MIDI_STREAMING)

#define sysTrapSndStreamCreate    0xA45B
#define sysTrapSndStreamDelete    0xA45C
#define sysTrapSndStreamStart     0xA45D
#define sysTrapSndStreamStop      0xA45F
#define sysTrapSndStreamSetVolume 0xA460

typedef UInt32 SndStreamRef;
typedef Int8   SndStreamMode;
typedef Int16  SndSampleType;
typedef Int8   SndStreamWidth;

#define sndOutput                 1
#define sndInt16Little            0x12
#define sndMono                   0

typedef Err (*SndStreamBufferCallback)(void *userdata,
		SndStreamRef channel, void *buffer, UInt32 numberofframes);

extern Err SndStreamCreate(SndStreamRef *channel,	SndStreamMode mode,
                           UInt32 samplerate, SndSampleType type, SndStreamWidth width,
                           SndStreamBufferCallback func, void *userdata, UInt32 buffsize, Boolean armNative)
  SYS_TRAP(sysTrapSndStreamCreate);

extern Err SndStreamDelete(SndStreamRef channel)
  SYS_TRAP(sysTrapSndStreamDelete);

extern Err SndStreamStart(SndStreamRef channel)
  SYS_TRAP(sysTrapSndStreamStart);

extern Err SndStreamStop(SndStreamRef channel)
  SYS_TRAP(sysTrapSndStreamStop);

extern Err SndStreamSetVolume(SndStreamRef channel, Int32 volume)
  SYS_TRAP(sysTrapSndStreamSetVolume);

#endif

#if (PALM_AUDIO_YAMAHAPA1 || PALM_MIDI_YAMAHAPA1)

typedef struct YamahaCallbackInfoType
{
  void   (*funcP)(UInt32);
  UInt32 dwUserData;
} YamahaCallbackInfoType;

typedef struct YamahaSmfMgrChanRangeType
{
  UInt8  bFirstChan;
  UInt8  bLastChan;
} YamahaSmfMgrChanRangeType;

typedef struct YamahaSmfMgrPlayTimeType
{
  UInt32 dwStartMilliSec;
  UInt32 dwEndMilliSec;
} YamahaSmfMgrPlayTimeType;

typedef struct YamahaUserVoiceDatabaseType
{
  UInt16 cardNo;
  Char   *name;
} YamahaUserVoiceDatabaseType;

#define YAMAHA_LIBNAME   "Pa1Lib"
#define YAMAHA_LIBCR8R   'yP1L'
#define YAMAHA_EVENT     0                 // wont achieve 100% speed with lot of processing
#define YAMAHA_INTERRUPT 1                 // may not work on all devices :(
#define YAMAHA_PLAYMODE  YAMAHA_INTERRUPT

#define YAMAHA_4Khz      0
#define YAMAHA_8Khz      1

// sony specific callbacks (for Yamaha Library)
#define SonySndKindSp    0 // Speaker volume 
#define SonySndKindHp    2 // HeadPhone volume 
typedef void (*SonyPA1LsndStateOnType)(UInt8 /* kind */, UInt8 /* L volume 0-31 */, UInt8 /* R volume 0-31 */);
typedef void (*SonyPA1LsndStateOffType)(UInt8 /* kind */);

// library standard stuff
Err YamahaPA1LibOpen(UInt16 refNum)
	SYS_TRAP(sysLibTrapOpen);
Err YamahaPA1LibClose(UInt16 refNum, UInt16 *numAppsP)
	SYS_TRAP(sysLibTrapClose);

// midi playback
Err YamahaPA1LsmfOpen(UInt16 refNum, MemPtr pSmf,
                      YamahaUserVoiceDatabaseType *userVoiceDB,
                      UInt8 *hd, UInt32 *duration, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 0);
Err YamahaPA1LsmfClose(UInt16 refNum, UInt8 hd, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 1);
Err YamahaPA1LsmfPlay(UInt16 refNum, UInt8 hd, UInt8 mode,
                      YamahaSmfMgrPlayTimeType  *playTimeP,
                      YamahaSmfMgrChanRangeType *chanRangeP,
                      YamahaCallbackInfoType *callbackInfoP, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 2);
Err YamahaPA1LsmfStop(UInt16 refNum, UInt8 hd, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 3);
Err YamahaPA1LsmfGetCurrentPosition(UInt16 refNum, UInt8 hd,
                                    UInt32 *pos, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 6);

// adpcm playback [old yamaha library]
Err YamahaPA1LsndAdpcmStart(UInt16 refNum, UInt8 fs, UInt8 *data, UInt32 dwSize,
                            YamahaCallbackInfoType *callbackInfoP, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 17);
Err YamahaPA1LsndAdpcmStop(UInt16 refNum, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 18);

// adpcm playback [new yamaha library]
Err YamahaPA1LadpcmOpen(UInt16 refNum, UInt8 fs, UInt8 *data, UInt32 dwSize,
                        YamahaCallbackInfoType *callbackInfoP, UInt8 *hd, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 39);
Err YamahaPA1LadpcmClose(UInt16 refNum, UInt8 hd, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 40);
Err YamahaPA1LadpcmStart(UInt16 refNum, UInt8 hd, UInt8 mode, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 41);
Err YamahaPA1LadpcmStop( UInt16 refNum, UInt8 hd, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 42);

// event based callback
Err YamahaPA1LsndEventProc(UInt16 refNum, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 20);

// volume controls
Err YamahaPA1LsndAdpcmMasterVolume(UInt16 refNum, UInt8 vol, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 19);

Err YamahaPA1LSpVolume(UInt16 refNum, UInt8 vol, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 25);
Err YamahaPA1LHpVolume(UInt16 refNum, UInt8 vol_l, UInt8 vol_r, Boolean *retval)
	SYS_TRAP(sysLibTrapCustom + 27);

// midi callback mechanism
#ifdef PALM_MIDI_YAMAHAPA1
static void YamahaMidiCallback(UInt32 userData) __GAME__;
static void
YamahaMidiCallback(UInt32 userData)
{
  Boolean                state;
  YamahaCallbackInfoType callback;

  callback.funcP      = YamahaMidiCallback;
  callback.dwUserData = globals.music.yamaha_midi.midiHInternal;

  // start the playback again, yes, we want to repeat this midi
  YamahaPA1LsmfPlay(globals.music.yamaha_midi.libRef,
                    globals.music.yamaha_midi.midiHInternal, YAMAHA_PLAYMODE,
                    NULL, NULL, &callback, &state);
}
#endif

#endif

#if PALM_AUDIO_ANY
/**
 * Initialize the sfx playback engine.
 */
void
GameSfxInitialize()
{
#if PALM_AUDIO_YAMAHAPA1
  Boolean result = false;
  Err     err;
#endif

#if PALM_AUDIO_STREAMING
  // do we have streaming audio?
  globals.audio.palm_sfx_streaming.device =
    (SysGetTrapAddress(sysTrapSndStreamCreate) != SysGetTrapAddress(sysTrapSysUnimplemented));
#endif
#if PALM_AUDIO_YAMAHAPA1
  // do we have yamaha audio?
	err = SysLibFind(YAMAHA_LIBNAME, &globals.audio.yamaha_sfx.libRef);
	if (err) err = SysLibLoad('libr', YAMAHA_LIBCR8R, &globals.audio.yamaha_sfx.libRef);
  if (!err) err = YamahaPA1LibOpen(globals.audio.yamaha_sfx.libRef);

  result = (err == errNone);  // do we have the yamaha library?
  if (result)
  {
    globals.audio.yamaha_sfx.device = true;
#ifdef PALM_AUDIO_STREAMING
    globals.audio.palm_sfx_streaming.device = false;
#endif

    // are we running on a sony enhanced audio device (different Yamaha Library)
    globals.audio.yamaha_sfx.sony_enhanced_audio = DeviceSupportsVersion(romVersion4);
                                          // cheat, all handspring units are < palmos4
  }
#endif

#if PALM_AUDIO_STREAMING
  // using sfx engine (OS5+)
  if (globals.audio.palm_sfx_streaming.device)
  {
    int i;

    // 8192 bytes memory chunk required for midi globals
    globals.audio.palm_sfx_streaming.globals = MemPtrNew(16);
    MemSet(globals.audio.palm_sfx_streaming.globals, 16, 0);

    // initialize all audio resources
    for (i=0; i<MAX_AUDIO; i++)
    {
      globals.audio.palm_sfx_streaming.audioH[i] = DmGetResource('_pcm', audioBase+i);
      globals.audio.palm_sfx_streaming.audioP[i] = MemHandleLock(globals.audio.palm_sfx_streaming.audioH[i]);
    }

    // create the sound stream
    globals.audio.palm_sfx_streaming.armH = DmGetResource('_arm', sfxResource);
    SndStreamCreate(&globals.audio.palm_sfx_streaming.stream, sndOutput,
		                11025, sndInt16Little, sndMono,
                    (SndStreamBufferCallback)MemHandleLock(globals.audio.palm_sfx_streaming.armH),
		                globals.audio.palm_sfx_streaming.globals, 1024, true);

    // make sure the volume is calibrated correctly
    {
//    UInt16 volumes[] = { 0, 256, 512, 1024, 2048, 4096, 8192, 16384 };
      SndStreamSetVolume(globals.audio.palm_sfx_streaming.stream, 16384);  // MAX
    }

    // start the stream (will play nothing right now)
    SndStreamStart(globals.audio.palm_sfx_streaming.stream);
  }
#endif

#if PALM_AUDIO_YAMAHAPA1
  // yamaha adpcm sfx engine
  if (globals.audio.yamaha_sfx.device)
  {
    int i;

    // initialize all audio resources
    for (i=0; i<MAX_AUDIO; i++)
    {
      globals.audio.yamaha_sfx.audioH[i] = DmGetResource('_adp', audioBase+i);
      globals.audio.yamaha_sfx.audioP[i] = MemHandleLock(globals.audio.yamaha_sfx.audioH[i]);
    }

    // OLD Yamaha Library: (beatplus springboard)
    if (!globals.audio.yamaha_sfx.sony_enhanced_audio)
    {
      Boolean state;

      // make sure the volume is calibrated correctly
      {
//      UInt16 volumes[] = { 0, 48, 64, 80, 96, 104, 112, 120 };
        YamahaPA1LsndAdpcmMasterVolume(globals.audio.yamaha_sfx.libRef, 120, &state); // MAX
      }
    }

    // NEW Yamaha Library: (sony enhanced audio)
    else
    {
      // make sure the volume is calibrated correctly
      {
        Err   err;
        void *sndOn;

        // NEW sony way
        err = FtrGet('SsYs', 10004, (UInt32 *)&sndOn);
        if ((err == errNone) && (sndOn))
        {
//        UInt16  volumes[] = { 0, 4, 8, 12, 16, 20, 24, 28 };
          ((SonyPA1LsndStateOnType)sndOn)(SonySndKindSp, 28, 28);
          ((SonyPA1LsndStateOnType)sndOn)(SonySndKindHp, 28, 28);  // MAX
        }

        // OLD yamaha way
        else
        {
          Boolean state;
//        UInt16  volumes[] = { 0, 48, 64, 80, 96, 104, 112, 120 };
          YamahaPA1LsndAdpcmMasterVolume(globals.audio.yamaha_sfx.libRef, 120, &state); // MAX
        }
      }
    }
  }
#endif
}

/**
 * Play a sound in the sfx playback engine.
 *
 * @param prefs the global preference data.
 * @param sound the sound to playback.
 */
void
GameSfxPlay(PreferencesType *prefs, UInt8 sound)
{
  // if the device audio is muted, exit, stage left
  if (DeviceGetMute()) return;

#if PALM_AUDIO_STREAMING
  // using sfx engine (OS5+)
  if (globals.audio.palm_sfx_streaming.device)
  {
    UInt32 *engineData = globals.audio.palm_sfx_streaming.globals;

    // turn off existing sound
    __write_byte32(engineData+waveBasePtr, 0);

    // make sure the volume is calibrated correctly
    {
      UInt16 volumes[] = { 0, 256, 512, 1024, 2048, 4096, 8192, 16384 };
      SndStreamSetVolume(globals.audio.palm_sfx_streaming.stream, volumes[prefs->config.sndVolume]);
    }

    // configure new sound
    __write_byte32(engineData+waveBasePtr, (UInt32)globals.audio.palm_sfx_streaming.audioP[sound]);
    __write_byte32(engineData+waveLocalPtr, 0);
    __write_byte32(engineData+waveSize, MemHandleSize(globals.audio.palm_sfx_streaming.audioH[sound]));
  }
#endif

#if PALM_AUDIO_YAMAHAPA1
  // yamaha adpcm sfx engine
  if (globals.audio.yamaha_sfx.device)
  {
    // OLD Yamaha Library: (beatplus springboard)
    if (!globals.audio.yamaha_sfx.sony_enhanced_audio)
    {
      Boolean state;

      // turn off existing sound
      YamahaPA1LsndAdpcmStop(globals.audio.yamaha_sfx.libRef, &state);

      // make sure the volume is calibrated correctly
      {
        UInt16 volumes[] = { 0, 48, 64, 80, 96, 104, 112, 120 };
        YamahaPA1LsndAdpcmMasterVolume(globals.audio.yamaha_sfx.libRef, volumes[prefs->config.sndVolume], &state);
      }

      // configure new sound
  	  YamahaPA1LsndAdpcmStart(globals.audio.yamaha_sfx.libRef, YAMAHA_8Khz,
                              globals.audio.yamaha_sfx.audioP[sound],
                              MemHandleSize(globals.audio.yamaha_sfx.audioH[sound]), NULL, &state);
    }

    // NEW Yamaha Library: (sony enhanced audio)
    else
    {
      Boolean state;

      // turn off existing sound
      YamahaPA1LadpcmStop(globals.audio.yamaha_sfx.libRef, globals.audio.yamaha_sfx.audioHInternal, &state);
      YamahaPA1LadpcmClose(globals.audio.yamaha_sfx.libRef, globals.audio.yamaha_sfx.audioHInternal, &state);

      // make sure the volume is calibrated correctly
      {
        Err   err;
        void *sndOn;

        // NEW sony way
        err = FtrGet('SsYs', 10004, (UInt32 *)&sndOn);
        if ((err == errNone) && (sndOn))
        {
          UInt16  volumes[] = { 0, 4, 8, 12, 16, 20, 24, 28 };
          ((SonyPA1LsndStateOnType)sndOn)(SonySndKindSp, volumes[prefs->config.sndVolume], volumes[prefs->config.sndVolume]);
          ((SonyPA1LsndStateOnType)sndOn)(SonySndKindHp, volumes[prefs->config.sndVolume], volumes[prefs->config.sndVolume]);
        }

        // OLD yamaha way
        else
        {
          UInt16 volumes[] = { 0, 48, 64, 80, 96, 104, 112, 120 };
          YamahaPA1LsndAdpcmMasterVolume(globals.audio.yamaha_sfx.libRef, volumes[prefs->config.sndVolume], &state); // MAX
        }
      }

      // configure new sound
  	  YamahaPA1LadpcmOpen(globals.audio.yamaha_sfx.libRef, YAMAHA_8Khz,
                          globals.audio.yamaha_sfx.audioP[sound],
                          MemHandleSize(globals.audio.yamaha_sfx.audioH[sound]),
                          NULL, &globals.audio.yamaha_sfx.audioHInternal, &state);
      YamahaPA1LadpcmStart(globals.audio.yamaha_sfx.libRef, globals.audio.yamaha_sfx.audioHInternal, 3, &state);
    }
  }
#endif
}

/**
 * Pause the sfx playback engine.
 *
 * @param prefs the global preference data.
 * @param state the pause state.
 */
void
GameSfxPause(PreferencesType *prefs, Boolean state)
{
#ifdef PALM_MIDI_STREAMING
  // using streaming engine (OS5+)
  if (globals.audio.palm_sfx_streaming.device)
  {
    if (state) SndStreamStop(globals.audio.palm_sfx_streaming.stream);
    else       SndStreamStart(globals.audio.palm_sfx_streaming.stream);
  }
#endif
}

/**
 * Terminate the sfx playback engine.
 */
void
GameSfxTerminate()
{
#if PALM_AUDIO_STREAMING
  // using sfx engine (OS5+)
  if (globals.audio.palm_sfx_streaming.device)
  {
    int i;

    // stop and delete the stream
    SndStreamStop(globals.audio.palm_sfx_streaming.stream);
    SndStreamDelete(globals.audio.palm_sfx_streaming.stream);

    // unlock callback armlet
    if (globals.audio.palm_sfx_streaming.armH)
    {
      MemHandleUnlock(globals.audio.palm_sfx_streaming.armH);
      DmReleaseResource(globals.audio.palm_sfx_streaming.armH);
    }
    globals.audio.palm_sfx_streaming.armH  = NULL;

    // clean up all audio resources
    for (i=0; i<MAX_AUDIO; i++)
    {
      MemHandleUnlock(globals.audio.palm_sfx_streaming.audioH[i]);
      DmReleaseResource(globals.audio.palm_sfx_streaming.audioH[i]);

      globals.audio.palm_sfx_streaming.audioH[i] = NULL;
      globals.audio.palm_sfx_streaming.audioP[i] = NULL;
    }

    // free memory used
    if (globals.audio.palm_sfx_streaming.globals)
    {
      MemPtrFree(globals.audio.palm_sfx_streaming.globals);
      globals.audio.palm_sfx_streaming.globals = NULL;
    }
  }
#endif

#if PALM_AUDIO_YAMAHAPA1
  // yamaha adpcm sfx engine
  if (globals.audio.yamaha_sfx.device)
  {
    UInt16  useCount;
    int     i;

    // OLD Yamaha Library: (beatplus springboard)
    if (!globals.audio.yamaha_sfx.sony_enhanced_audio)
    {
      Boolean state;

      // stop and delete the stream
      YamahaPA1LsndAdpcmStop(globals.audio.yamaha_sfx.libRef, &state);
    }

    // NEW Yamaha Library: (sony enhanced audio)
    else
    {
      Boolean  state;

      // stop and delete the stream
      YamahaPA1LadpcmStop(globals.audio.yamaha_sfx.libRef, globals.audio.yamaha_sfx.audioHInternal, &state);
      YamahaPA1LadpcmClose(globals.audio.yamaha_sfx.libRef, globals.audio.yamaha_sfx.audioHInternal, &state);

      // make sure the volume is calibrated correctly
      {
        Err   err;
        void *sndOff;

        // NEW sony way
        err = FtrGet('SsYs', 10005, (UInt32 *)&sndOff);
        if ((err == errNone) && (sndOff))
        {
          ((SonyPA1LsndStateOffType)sndOff)(SonySndKindSp);
          ((SonyPA1LsndStateOffType)sndOff)(SonySndKindHp);
        }

        // OLD yamaha way
        else
        {
          // not required
        }
      }
    }

    // clean up all audio resources
    for (i=0; i<MAX_AUDIO; i++)
    {
      MemHandleUnlock(globals.audio.yamaha_sfx.audioH[i]);
      DmReleaseResource(globals.audio.yamaha_sfx.audioH[i]);

      globals.audio.yamaha_sfx.audioH[i] = NULL;
      globals.audio.yamaha_sfx.audioP[i] = NULL;
    }

    // close the yamaha library
	  YamahaPA1LibClose(globals.audio.yamaha_sfx.libRef, &useCount);
    if (useCount == 0)
      SysLibRemove(globals.audio.yamaha_sfx.libRef);
  }
#endif
}
#endif

#ifdef PALM_MIDI_STREAMING
static UInt32 NoteFreq[128] =
{
  0x000004c1, 0x0000050a, 0x00000556, 0x000005a8,
  0x000005fe, 0x00000659, 0x000006ba, 0x00000720,
  0x0000078d, 0x00000800, 0x00000879, 0x000008fa,
  0x00000983, 0x00000a14, 0x00000aad, 0x00000b50,
  0x00000bfc, 0x00000cb2, 0x00000d74, 0x00000e41,
  0x00000f1a, 0x00001000, 0x000010f3, 0x000011f5,
  0x00001306, 0x00001428, 0x0000155b, 0x000016a0,
  0x000017f9, 0x00001965, 0x00001ae8, 0x00001c82,
  0x00001e34, 0x00002000, 0x000021e7, 0x000023eb,
  0x0000260d, 0x00002851, 0x00002ab7, 0x00002d41,
  0x00002ff2, 0x000032cb, 0x000035d1, 0x00003904,
  0x00003c68, 0x00004000, 0x000043ce, 0x000047d6,
  0x00004c1b, 0x000050a2, 0x0000556e, 0x00005a82,
  0x00005fe4, 0x00006597, 0x00006ba2, 0x00007208,
  0x000078d0, 0x00008000, 0x0000879c, 0x00008fac,
  0x00009837, 0x0000a145, 0x0000aadc, 0x0000b504,
  0x0000bfc8, 0x0000cb2f, 0x0000d744, 0x0000e411,
  0x0000f1a1, 0x00010000, 0x00010f38, 0x00011f59,
  0x0001306f, 0x0001428a, 0x000155b8, 0x00016a09,
  0x00017f91, 0x0001965f, 0x0001ae89, 0x0001c823,
  0x0001e343, 0x00020000, 0x00021e71, 0x00023eb3,
  0x000260df, 0x00028514, 0x0002ab70, 0x0002d413,
  0x0002ff22, 0x00032cbf, 0x00035d13, 0x00039047,
  0x0003c686, 0x00040000, 0x00043ce3, 0x00047d66,
  0x0004c1bf, 0x00050a28, 0x000556e0, 0x0005a827,
  0x0005fe44, 0x0006597f, 0x0006ba27, 0x0007208f,
  0x00078d0d, 0x00080000, 0x000879c7, 0x0008facd,
  0x0009837f, 0x000a1451, 0x000aadc0, 0x000b504f,
  0x000bfc88, 0x000cb2ff, 0x000d744f, 0x000e411f,
  0x000f1a1b, 0x00100000, 0x0010f38f, 0x0011f59a,
  0x001306fe, 0x001428a2, 0x00155b81, 0x0016a09e,
  0x0017f910, 0x001965fe, 0x001ae89f, 0x001c823e
};
#endif

/**
 * Initialize the music playback engine.
 */
void
GameMusicInitialize()
{
#if (PALM_MIDI_STREAMING || PALM_MIDI_YAMAHAPA1)
  Boolean result = false;
#endif
#ifdef PALM_MIDI_YAMAHAPA1
  Err     err;
#endif

  // worst case = there is the ability to use the core music engine
  globals.music.core.device = true;
#ifdef PALM_MIDI_STREAMING
  result = (SysGetTrapAddress(sysTrapSndStreamCreate) != SysGetTrapAddress(sysTrapSysUnimplemented));
  if (result)
  {
    globals.music.palm_midi_streaming.device = true;
    globals.music.core.device = false;    // midi engine gets precedence!
  }
#endif
#ifdef PALM_MIDI_YAMAHAPA1
	err = SysLibFind(YAMAHA_LIBNAME, &globals.music.yamaha_midi.libRef);
	if (err) err = SysLibLoad('libr', YAMAHA_LIBCR8R, &globals.music.yamaha_midi.libRef);
  if (!err) err = YamahaPA1LibOpen(globals.music.yamaha_midi.libRef);

  result = (err == errNone);
  if (result)
  {
    globals.music.yamaha_midi.device = true;
#ifdef PALM_MIDI_STREAMING
    globals.music.palm_midi_streaming.device = false;
#endif
    globals.music.core.device = false;    // midi engine gets precedence!

    // are we running on a sony enhanced audio device (different Yamaha Library)
    globals.audio.yamaha_sfx.sony_enhanced_audio = DeviceSupportsVersion(romVersion4);
                                          // cheat, all handspring units are < palmos4
  }
#endif

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  if (globals.music.palm_midi_streaming.device)
  {
    UInt32 *engineData, *voiceData;
    UInt32 defaultSample, defaultSampleSize, defaultDrumSample, defaultDrumSampleSize;
#ifndef MIDI_SIMPLE_SAMPLES
    UInt32 currentSample, currentSampleSize;
#endif
    int    i;

    // 8192 bytes memory chunk required for midi globals
    globals.music.palm_midi_streaming.globals = MemPtrNew(8192);
    MemSet(globals.music.palm_midi_streaming.globals, 8192, 0);

    engineData = globals.music.palm_midi_streaming.globals;
    voiceData  = globals.music.palm_midi_streaming.globals + sizeofvariableblock;

    // initialize instrument/drum samples
    globals.music.palm_midi_streaming.default_inst_sampleH = DmGetResource('_pcm', midiSampleInstrument);
    defaultSampleSize = (UInt32)MemHandleSize(globals.music.palm_midi_streaming.default_inst_sampleH);
    defaultSample     = (UInt32)MemHandleLock(globals.music.palm_midi_streaming.default_inst_sampleH);
    globals.music.palm_midi_streaming.default_drum_sampleH = DmGetResource('_pcm', midiSampleDrum);
    defaultDrumSampleSize = (UInt32)MemHandleSize(globals.music.palm_midi_streaming.default_drum_sampleH);
    defaultDrumSample     = (UInt32)MemHandleLock(globals.music.palm_midi_streaming.default_drum_sampleH);

    for (i=0; i<128; i++)
    {
      // notes
      __write_byte32(engineData+NoteFrequencyTablePtr+i, NoteFreq[i]);

      // instrument sample
#ifndef MIDI_SIMPLE_SAMPLES
      globals.music.palm_midi_streaming.inst_sampleH[i] = DmGetResource('_pcm', midiSampleInstrument+1+i);
      if (globals.music.palm_midi_streaming.inst_sampleH[i])
      {
        currentSampleSize = (UInt32)MemHandleSize(globals.music.palm_midi_streaming.inst_sampleH[i]);
        currentSample     = (UInt32)MemHandleLock(globals.music.palm_midi_streaming.inst_sampleH[i]);

        __write_byte32((UInt32 *)(engineData+instrumentSampleTablePtr+i), currentSample);
        __write_byte32((UInt32 *)(engineData+instrumentSampleSizeTablePtr+i), currentSampleSize);
      }

      // drum sample
      globals.music.palm_midi_streaming.drum_sampleH[i] = DmGetResource('_pcm', midiSampleDrum+1+i);
      if (globals.music.palm_midi_streaming.drum_sampleH[i])
      {
        currentSampleSize = (UInt32)MemHandleSize(globals.music.palm_midi_streaming.drum_sampleH[i]);
        currentSample     = (UInt32)MemHandleLock(globals.music.palm_midi_streaming.drum_sampleH[i]);

        __write_byte32((UInt32 *)(engineData+drumSampleTablePtr+i), currentSample);
        __write_byte32((UInt32 *)(engineData+drumSampleSizeTablePtr+i), currentSampleSize);
      }
#endif
    }

    // ensure there is a sample for every instrument possible
    for (i=0; i<128; i++)
    {
      if (globals.music.palm_midi_streaming.inst_sampleH[i] == NULL)
      {
        __write_byte32((UInt32 *)(engineData+instrumentSampleTablePtr+i), defaultSample);
        __write_byte32((UInt32 *)(engineData+instrumentSampleSizeTablePtr+i), defaultSampleSize);
      }

      if (globals.music.palm_midi_streaming.drum_sampleH[i] == NULL)
      {
        __write_byte32((UInt32 *)(engineData+drumSampleTablePtr+i), defaultDrumSample);
        __write_byte32((UInt32 *)(engineData+drumSampleSizeTablePtr+i), defaultDrumSampleSize);
      }
    }

    // initialize voices
    for (i=0; i<16; i++)
    {
      __write_byte32((UInt32 *)(voiceData+i*sizeofvoicedata+sampleBasePtr), (UInt32)defaultSample);
      __write_byte32((UInt32 *)(voiceData+i*sizeofvoicedata+samplePtr), 0);
      __write_byte32((UInt32 *)(voiceData+i*sizeofvoicedata+sampleSize), 0);
      __write_byte32((UInt32 *)(voiceData+i*sizeofvoicedata+noteFrequency), 0);
	  }

    // create the sound stream
    globals.music.palm_midi_streaming.armH = DmGetResource('_arm', midiResource);
    SndStreamCreate(&globals.music.palm_midi_streaming.stream, sndOutput,
		                22050, sndInt16Little, sndMono,
                    (SndStreamBufferCallback)MemHandleLock(globals.music.palm_midi_streaming.armH),
		                globals.music.palm_midi_streaming.globals, 1024, true);

    // make sure the volume is calibrated correctly
    {
//    UInt16 volumes[] = { 0, 256, 512, 1024, 2048, 4096, 8192, 16384 };
      SndStreamSetVolume(globals.music.palm_midi_streaming.stream, 16384); // MAX
    }

    // the stream has NOT been started yet
    globals.music.palm_midi_streaming.active = false;
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  if (globals.music.yamaha_midi.device)
  {
    // OLD Yamaha Library: (beatplus springboard)
    if (!globals.audio.yamaha_sfx.sony_enhanced_audio)
    {
      Boolean state;

      // make sure the volume is calibrated correctly
      {
//      UInt16 volumes[] = { 0, 4, 8, 12, 16, 20, 24, 28 };
        YamahaPA1LSpVolume(globals.music.yamaha_midi.libRef, 28, &state);
        YamahaPA1LHpVolume(globals.music.yamaha_midi.libRef, 28, 28, &state); // MAX
      }
    }

    // NEW Yamaha Library: (sony enhanced audio)
    else
    {
      // make sure the volume is calibrated correctly
      {
        Err   err;
        void *sndOn;

        // NEW sony way
        err = FtrGet('SsYs', 10004, (UInt32 *)&sndOn);
        if ((err == errNone) && (sndOn))
        {
//        UInt16  volumes[] = { 0, 4, 8, 12, 16, 20, 24, 28 };
          ((SonyPA1LsndStateOnType)sndOn)(SonySndKindSp, 28, 28);
          ((SonyPA1LsndStateOnType)sndOn)(SonySndKindHp, 28, 28);  // MAX
        }

        // OLD yamaha way
        else
        {
          Boolean state;
//        UInt16  volumes[] = { 0, 4, 8, 12, 16, 20, 24, 28 };
          YamahaPA1LSpVolume(globals.music.yamaha_midi.libRef, 28, &state);
          YamahaPA1LHpVolume(globals.music.yamaha_midi.libRef, 28, 28, &state); // MAX
        }
      }
    }

    // configure stuff
    globals.music.yamaha_midi.midiPosition = 0;

    // the playback has NOT been started yet
    globals.music.yamaha_midi.active = false;
  }
#endif
}

/**
 * Initialize the music playback engine.
 *
 * @param prefs the global preference data.
 */
void
GameMusicLoad(PreferencesType *prefs)
{
  // using core midi/playback engine (simple freq+duration engine)
  if (globals.music.core.device)
  {
    MemHandle memHandle;
    UInt16    *dataP, i;

    // release any memory previously used
    if (globals.music.core.musicData)
    {
      MemPtrFree(globals.music.core.musicData);
      globals.music.core.musicData = NULL;
    }

    // how many music songs do we have?
    memHandle = LevelPackGetResource(levlMusic, 0);
    globals.music.musicCount = StrAToI(MemHandleLock(memHandle));
    MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

    // load the music data for the level
    memHandle =
      LevelPackGetResource(levlMusic,
        ((prefs->game.gameLevel-1) % globals.music.musicCount)+1);
    dataP = (UInt16 *)MemHandleLock(memHandle);

    globals.music.core.musicSize = dataP[0];
    globals.music.core.musicData =
      (UInt16 *)MemPtrNew(globals.music.core.musicSize * sizeof(UInt16) * 2);
    if (globals.music.core.musicData)
    {
      // copy data over :)
      for (i=0; i<globals.music.core.musicSize; i++)
      {
        globals.music.core.musicData[(i*2)+0] = dataP[(i*2)+1];
        globals.music.core.musicData[(i*2)+1] = dataP[(i*2)+2]-1;
      }
    }

    MemHandleUnlock(memHandle);
    LevelPackReleaseResource(memHandle);

    // must trigger the playback [yes, we have been interrupted] :)
    prefs->game.music.core.musicInterrupted = true;
  }

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  else
  if (globals.music.palm_midi_streaming.device)
  {
#ifdef MIDI_IN_LEVELPACK
    MemHandle  memHandle;
#endif
    UInt32    *engineData;

    // do we have an active stream?
    if (globals.music.palm_midi_streaming.active)
      SndStreamStop(globals.music.palm_midi_streaming.stream);

    // release the previous handle (if we have set it)
    if (globals.music.palm_midi_streaming.midiH)
    {
      MemHandleUnlock(globals.music.palm_midi_streaming.midiH);
      DmReleaseResource(globals.music.palm_midi_streaming.midiH);
    }
    globals.music.palm_midi_streaming.midiH = NULL;

    // initialize midi specific information
    engineData = globals.music.palm_midi_streaming.globals;

    // how many music songs do we have?
#ifdef MIDI_IN_LEVELPACK
    memHandle = LevelPackGetResource(levlMidi, 0);
    if (memHandle == NULL)
#endif
      globals.music.palm_midi_streaming.midiH = DmGetResource(levlMidi, midiResource);
#ifdef MIDI_IN_LEVELPACK
    else
    {
      UInt16 size;

      globals.music.musicCount = StrAToI(MemHandleLock(memHandle));
      MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

      globals.music.palm_midi_streaming.midiH =
        LevelPackGetResource(levlMidi,
          ((prefs->game.gameLevel-1) % globals.music.musicCount)+1);

      if (globals.music.palm_midi_streaming.midiH)
      {
        // create the 'memory' store for the 'midi' resource (need to copy locally)
        globals.music.dbCard = 0;
        globals.music.dbID   = DmFindDatabase(globals.music.dbCard, cacheMusicFileName);
        if (globals.music.dbID != NULL)
        {
          DmDatabaseProtect(globals.music.dbCard, globals.music.dbID, false);
          DmDeleteDatabase(globals.music.dbCard, globals.music.dbID);
        }
        DmCreateDatabase(globals.music.dbCard, cacheMusicFileName, appCreator, cacheType, true);
        globals.music.dbID = DmFindDatabase(globals.music.dbCard, cacheMusicFileName);
        globals.music.dbMemoryStore =
          DmOpenDatabase(globals.music.dbCard, globals.music.dbID, dmModeReadWrite);

        size = MemHandleSize(globals.music.palm_midi_streaming.midiH);
        memHandle = DmNewResource(globals.music.dbMemoryStore, levlMidi, midiResource+1, size);
        if (memHandle)
        {
          DmWrite(MemHandleLock(memHandle), 0, MemHandleLock(globals.music.palm_midi_streaming.midiH), size);
          MemHandleUnlock(globals.music.palm_midi_streaming.midiH);
          MemHandleUnlock(memHandle);
        }
        LevelPackReleaseResource(globals.music.palm_midi_streaming.midiH);

        // use the local copy!
        globals.music.palm_midi_streaming.midiH = memHandle;

        // protect and close memory store
        DmDatabaseProtect(globals.music.dbCard, globals.music.dbID, true);
        DmCloseDatabase(globals.music.dbMemoryStore);
      }

      // fallback? resource not existant, default
      if (globals.music.palm_midi_streaming.midiH == NULL)
        globals.music.palm_midi_streaming.midiH = DmGetResource(levlMidi, midiResource);
    }
#endif

    {
      UInt32  divisionPPQN, oneTick, deltaTime;
      void   *midiData;
      UInt8  *trackPtr;

      midiData = MemHandleLock(globals.music.palm_midi_streaming.midiH);

      // timing information
      divisionPPQN = (UInt32)(*(UInt16 *)(midiData + 12));
      oneTick      = (UInt32)(500000 / divisionPPQN);

      __write_byte32((UInt32 *)(engineData+PPQNDivision), divisionPPQN);
      __write_byte32((UInt32 *)(engineData+oneTickMicroSec), oneTick);

      // initialize track data
      trackPtr = (midiData + 14);
      __write_byte32((UInt32 *)(engineData+track0Length), *((UInt32 *)(trackPtr+4)));
      trackPtr += 8;
      __write_byte32((UInt32 *)(engineData+trackStartPtr), (UInt32)trackPtr);

      // get initial delta time
      deltaTime = 0;
      do
      {
        deltaTime <<= 7;
        deltaTime += (*trackPtr & 127);
      }
      while ((*trackPtr++ & 128));

      __write_byte32((UInt32 *)(engineData+track0Ptr), (UInt32)trackPtr);
      __write_byte32((UInt32 *)(engineData+deltaTimeMicroSec), deltaTime*oneTick);
    }

    // ok, now we are ready to start
    globals.music.palm_midi_streaming.active = false;
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  else
  if (globals.music.yamaha_midi.device)
  {
#ifdef MIDI_IN_LEVELPACK
    MemHandle memHandle;
#endif
    Boolean   state;
    UInt32    duration;

    // do we have an active stream?
    if (globals.music.yamaha_midi.active)
      YamahaPA1LsmfStop(globals.music.yamaha_midi.libRef,
                        globals.music.yamaha_midi.midiHInternal, &state);

    // release the previous handle (if we have set it)
    if (globals.music.yamaha_midi.midiH)
    {
      YamahaPA1LsmfClose(globals.music.yamaha_midi.libRef,
                         globals.music.yamaha_midi.midiHInternal, &state);

      MemHandleUnlock(globals.music.yamaha_midi.midiH);
      DmReleaseResource(globals.music.yamaha_midi.midiH);
    }
    globals.music.yamaha_midi.midiH = NULL;

#ifdef MIDI_IN_LEVELPACK
    // how many music songs do we have?
    memHandle = LevelPackGetResource(levlMidi, 0);
    if (memHandle == NULL)
#endif
      globals.music.yamaha_midi.midiH = DmGetResource(levlMidi, midiResource);
#ifdef MIDI_IN_LEVELPACK
    else
    {
      UInt16 size;

      globals.music.musicCount = StrAToI(MemHandleLock(memHandle));
      MemHandleUnlock(memHandle); LevelPackReleaseResource(memHandle);

      // initialize midi specific information
      globals.music.yamaha_midi.midiH =
        LevelPackGetResource(levlMidi,
                             ((prefs->game.gameLevel-1) % globals.music.musicCount)+1);

      if (globals.music.yamaha_midi.midiH)
      {
        // create the 'memory' store for the 'midi' resource (need to copy locally)
        globals.music.dbCard = 0;
        globals.music.dbID   = DmFindDatabase(globals.music.dbCard, cacheMusicFileName);
        if (globals.music.dbID != NULL)
        {
          DmDatabaseProtect(globals.music.dbCard, globals.music.dbID, false);
          DmDeleteDatabase(globals.music.dbCard, globals.music.dbID);
        }
        DmCreateDatabase(globals.music.dbCard, cacheMusicFileName, appCreator, cacheType, true);
        globals.music.dbID = DmFindDatabase(globals.music.dbCard, cacheMusicFileName);
        globals.music.dbMemoryStore =
          DmOpenDatabase(globals.music.dbCard, globals.music.dbID, dmModeReadWrite);

        size = MemHandleSize(globals.music.yamaha_midi.midiH);
        memHandle = DmNewResource(globals.music.dbMemoryStore, levlMidi, midiResource+1, size);
        if (memHandle)
        {
          DmWrite(MemHandleLock(memHandle), 0, MemHandleLock(globals.music.yamaha_midi.midiH), size);
          MemHandleUnlock(globals.music.yamaha_midi.midiH);
          MemHandleUnlock(memHandle);
        }
        LevelPackReleaseResource(globals.music.yamaha_midi.midiH);

        // use the local copy!
        globals.music.yamaha_midi.midiH = memHandle;

        // protect and close memory store
        DmDatabaseProtect(globals.music.dbCard, globals.music.dbID, true);
        DmCloseDatabase(globals.music.dbMemoryStore);
      }

      // fallback? resource not existant, default
      if (globals.music.yamaha_midi.midiH == NULL)
        globals.music.yamaha_midi.midiH = DmGetResource(levlMidi, midiResource);
    }
#endif

    YamahaPA1LsmfOpen(globals.music.yamaha_midi.libRef,
                      MemHandleLock(globals.music.yamaha_midi.midiH), NULL,
                      &globals.music.yamaha_midi.midiHInternal, &duration, &state);

    // ok, now we are ready to start
    globals.music.yamaha_midi.active = false;
  }
#endif
}

/**
 * Reset the music engine (start music playing from start) :)
 *
 * @param prefs the global preference data.
 */
void
GameMusicReset(PreferencesType *prefs)
{
  // using core midi/playback engine (simple freq+duration engine)
  if (globals.music.core.device)
  {
    prefs->game.music.core.frameCountDown = 0;
    prefs->game.music.core.frameIndex     = 0;
  }

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  else
  if (globals.music.palm_midi_streaming.device)
  {
    // do we have an active stream?
    if (globals.music.palm_midi_streaming.active)
      SndStreamStop(globals.music.palm_midi_streaming.stream);

    // reset the track index pointer to start at beginning
    *(globals.music.palm_midi_streaming.globals+track0Ptr) = *(globals.music.palm_midi_streaming.globals+trackStartPtr);
    globals.music.palm_midi_streaming.active = false;
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  else
  if (globals.music.yamaha_midi.device)
  {
    Boolean state;

    // do we have an active stream?
    if (globals.music.yamaha_midi.active)
      YamahaPA1LsmfStop(globals.music.yamaha_midi.libRef,
                        globals.music.yamaha_midi.midiHInternal, &state);

    globals.music.yamaha_midi.active = false;
  }
#endif

  // callback for music (need to call it often)
  GameMusicEventCallback();
}

void
GameMusicFlagInterrupted(PreferencesType *prefs)
{
  // using core midi/playback engine (simple freq+duration engine)
  if (globals.music.core.device)
  {
    prefs->game.music.core.musicInterrupted = true;
  }

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  else
  if (globals.music.palm_midi_streaming.device)
  {
    // midi playback is asynchronous, doesn't interfere with normal sounds
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  else
  if (globals.music.yamaha_midi.device)
  {
    // midi playback is asynchronous, doesn't interfere with normal sounds
  }
#endif
}

/**
 * Play music :)
 *
 * @param prefs the global preference data.
 */
void
GameMusicPlayback(PreferencesType *prefs)
{
  // if the device audio is muted, exit, stage left
  if (DeviceGetMute()) return;

  // using core midi/playback engine (simple freq+duration engine)
  if (globals.music.core.device)
  {
    // is the music actually playing and music exists?
    if ((prefs->game.music.playing) && (globals.music.core.musicData))
    {
      SndCommandType musicNote = {sndCmdFrqOn,0,0,0,sndMaxAmp};
      UInt16         index;

      index = prefs->game.music.core.frameIndex * 2;

      // do we need to play a new note?
      if (prefs->game.music.core.frameCountDown == 0)
      {
        // prepare for the next frame
        prefs->game.music.core.frameCountDown = globals.music.core.musicData[index+1];
        prefs->game.music.core.frameIndex =
          (prefs->game.music.core.frameIndex + 1) % globals.music.core.musicSize;

        // must trigger the playback :)
        prefs->game.music.core.musicInterrupted = true;
      }

      // not ready yet, count down..
      else
        prefs->game.music.core.frameCountDown--;

      // if music was interrupted, we need to restart the note
      if (prefs->game.music.core.musicInterrupted)
      {
        // what freq/duration to play?
        musicNote.param1 = globals.music.core.musicData[index];
        musicNote.param2 =
         ((prefs->game.music.core.frameCountDown) * globals.millisecsPerFrame) - 2;
        prefs->game.music.core.musicInterrupted = false;

        // play the sound [if music is enabled]
        if (prefs->config.musicEnabled)
          DevicePlaySound(&musicNote);
      }
    }
  }

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  else
  if (globals.music.palm_midi_streaming.device)
  {
    if (prefs->config.musicEnabled)
    {
      // stream hasn't started yet?
      if (!globals.music.palm_midi_streaming.active)
        SndStreamStart(globals.music.palm_midi_streaming.stream);

      globals.music.palm_midi_streaming.active = true;
    }
    else
    {
      // do we have an active stream?
      if (globals.music.palm_midi_streaming.active)
        SndStreamStop(globals.music.palm_midi_streaming.stream);

      globals.music.palm_midi_streaming.active = false;
    }

    // make sure the volume is calibrated correctly
    {
      UInt16 volumes[] = { 0, 256, 512, 1024, 2048, 4096, 8192, 16384 };
      SndStreamSetVolume(globals.music.palm_midi_streaming.stream, volumes[prefs->config.sndVolume]);
    }
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  else
  if (globals.music.yamaha_midi.device)
  {
    Boolean state;

    if (prefs->config.musicEnabled)
    {
      // stream hasn't started yet?
      if (!globals.music.yamaha_midi.active)
      {
        YamahaCallbackInfoType callback;

        callback.funcP      = YamahaMidiCallback;
        callback.dwUserData = globals.music.yamaha_midi.midiHInternal;

        YamahaPA1LsmfPlay(globals.music.yamaha_midi.libRef,
                          globals.music.yamaha_midi.midiHInternal, YAMAHA_PLAYMODE,
                          NULL, NULL, &callback, &state);
      }

      globals.music.yamaha_midi.active = true;
    }
    else
    {
      // do we have an active stream?
      if (globals.music.yamaha_midi.active)
        YamahaPA1LsmfStop(globals.music.yamaha_midi.libRef,
                          globals.music.yamaha_midi.midiHInternal, &state);

      globals.music.yamaha_midi.active = false;
    }

    // OLD Yamaha Library: (beatplus springboard)
    if (!globals.audio.yamaha_sfx.sony_enhanced_audio)
    {
      // make sure the volume is calibrated correctly
      {
        UInt16 volumes[] = { 0, 4, 8, 12, 16, 20, 24, 28 };
        YamahaPA1LSpVolume(globals.music.yamaha_midi.libRef, volumes[prefs->config.sndVolume], &state);
        YamahaPA1LHpVolume(globals.music.yamaha_midi.libRef, volumes[prefs->config.sndVolume], volumes[prefs->config.sndVolume], &state);
      }
    }

    // NEW Yamaha Library: (sony enhanced audio)
    else
    {
      // make sure the volume is calibrated correctly
      {
        Err   err;
        void *sndOn;

        // NEW sony way
        err = FtrGet('SsYs', 10004, (UInt32 *)&sndOn);
        if ((err == errNone) && (sndOn))
        {
          UInt16  volumes[] = { 0, 4, 8, 12, 16, 20, 24, 28 };
          ((SonyPA1LsndStateOnType)sndOn)(SonySndKindSp, volumes[prefs->config.sndVolume], volumes[prefs->config.sndVolume]);
          ((SonyPA1LsndStateOnType)sndOn)(SonySndKindHp, volumes[prefs->config.sndVolume], volumes[prefs->config.sndVolume]);
        }

        // OLD yamaha way
        else
        {
          UInt16 volumes[] = { 0, 4, 8, 12, 16, 20, 24, 28 };
          YamahaPA1LSpVolume(globals.music.yamaha_midi.libRef, volumes[prefs->config.sndVolume], &state);
          YamahaPA1LHpVolume(globals.music.yamaha_midi.libRef, volumes[prefs->config.sndVolume], volumes[prefs->config.sndVolume], &state);
        }
      }
    }
  }
#endif
}

void
GameMusicEventCallback()
{
  // using core midi/playback engine (simple freq+duration engine)
  if (globals.music.core.device)
  {
    // nothing happening here
  }

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  else
  if (globals.music.palm_midi_streaming.device)
  {
    // nothing happening here
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  else
  if (globals.music.yamaha_midi.device)
  {
#if (YAMAHA_PLAYMODE == YAMAHA_EVENT)
    Boolean state;

    // SPECIAL: event based playback?
    YamahaPA1LsndEventProc(globals.music.yamaha_midi.libRef, &state);
#endif
  }
#endif
}

/**
 * Pause the music playback engine.
 *
 * @param prefs the global preference data.
 * @param state the pause state.
 */
void
GameMusicPause(PreferencesType *prefs, Boolean state)
{
  // using core midi/playback engine (simple freq+duration engine)
  if (globals.music.core.device)
  {
    if (state)
    {
      SndCommandType musicNote = {sndCmdFrqOn,0,0,0,sndMaxAmp};

      // turn off the sound [if music is enabled]
      if (prefs->config.musicEnabled)
        DevicePlaySound(&musicNote);

      prefs->game.music.core.musicInterrupted = true;
    }
  }

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  else
  if (globals.music.palm_midi_streaming.device)
  {
    if (globals.music.palm_midi_streaming.active)
    {
      if (state) SndStreamStop(globals.music.palm_midi_streaming.stream);
      else       SndStreamStart(globals.music.palm_midi_streaming.stream);
    }
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  else
  if (globals.music.yamaha_midi.device)
  {
    if (globals.music.yamaha_midi.active)
    {
/**
 ** BUG: smfPlay() with cfg is BUGGY - does not work as it should :(
 **/
      if (state)
      {
        YamahaPA1LsmfGetCurrentPosition(globals.music.yamaha_midi.libRef,
                                        globals.music.yamaha_midi.midiHInternal,
                                        &globals.music.yamaha_midi.midiPosition, &state);
        YamahaPA1LsmfStop(globals.music.yamaha_midi.libRef,
                          globals.music.yamaha_midi.midiHInternal, &state);
      }
      else
      {
        YamahaSmfMgrPlayTimeType cfg;
        YamahaCallbackInfoType   callback;

        cfg.dwStartMilliSec = globals.music.yamaha_midi.midiPosition;
        cfg.dwEndMilliSec   = 0xFFFFFFFF;
        callback.funcP      = YamahaMidiCallback;
        callback.dwUserData = globals.music.yamaha_midi.midiHInternal;

        YamahaPA1LsmfPlay(globals.music.yamaha_midi.libRef,
                          globals.music.yamaha_midi.midiHInternal, YAMAHA_PLAYMODE,
//                        &cfg, NULL, &callback, &state);             // doesn't work?
                          NULL, NULL, &callback, &state);
      }
    }
  }
#endif
}

/**
 * Stop the music playback engine.
 *
 * @param prefs the global preference data.
 */
void
GameMusicStop(PreferencesType *prefs)
{
  // using core midi/playback engine (simple freq+duration engine)
  if (globals.music.core.device)
  {
    SndCommandType musicNote = {sndCmdFrqOn,0,0,0,sndMaxAmp};

    // turn off the sound [if music is enabled]
    if (prefs->config.musicEnabled)
      DevicePlaySound(&musicNote);

    prefs->game.music.core.musicInterrupted = true;
  }

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  else
  if (globals.music.palm_midi_streaming.device)
  {
    if (globals.music.palm_midi_streaming.active)
    {
      SndStreamStop(globals.music.palm_midi_streaming.stream);
    }
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  else
  if (globals.music.yamaha_midi.device)
  {
    Boolean state;

    if (globals.music.yamaha_midi.active)
    {
      YamahaPA1LsmfStop(globals.music.yamaha_midi.libRef,
                        globals.music.yamaha_midi.midiHInternal, &state);
    }
  }
#endif
}

/**
 * Terminate the music playback engine.
 */
void
GameMusicTerminate()
{
  // using core midi/playback engine (simple freq+duration engine)
  if (globals.music.core.device)
  {
    if (globals.music.core.musicData)
    {
      MemPtrFree(globals.music.core.musicData);
      globals.music.core.musicData = NULL;
    }
  }

#ifdef PALM_MIDI_STREAMING
  // using midi engine (OS5+)
  else
  if (globals.music.palm_midi_streaming.device)
  {
    int i;

    // do we have an active stream?
    if (globals.music.palm_midi_streaming.active)
      SndStreamStop(globals.music.palm_midi_streaming.stream);
    SndStreamDelete(globals.music.palm_midi_streaming.stream);

    // unlock midi and callback armlet
    if (globals.music.palm_midi_streaming.armH)
    {
      MemHandleUnlock(globals.music.palm_midi_streaming.armH);
      DmReleaseResource(globals.music.palm_midi_streaming.armH);
    }
    globals.music.palm_midi_streaming.armH  = NULL;

    if (globals.music.palm_midi_streaming.midiH)
    {
      MemHandleUnlock(globals.music.palm_midi_streaming.midiH);
      DmReleaseResource(globals.music.palm_midi_streaming.midiH);
    }
    globals.music.palm_midi_streaming.midiH = NULL;

    // unlock instrument/drum samples
    for (i=0; i<128; i++)
    {
      if (globals.music.palm_midi_streaming.inst_sampleH[i])
      {
        MemHandleUnlock(globals.music.palm_midi_streaming.inst_sampleH[i]);
        DmReleaseResource(globals.music.palm_midi_streaming.inst_sampleH[i]);
      }
      if (globals.music.palm_midi_streaming.drum_sampleH[i])
      {
        MemHandleUnlock(globals.music.palm_midi_streaming.drum_sampleH[i]);
        DmReleaseResource(globals.music.palm_midi_streaming.drum_sampleH[i]);
      }

      globals.music.palm_midi_streaming.inst_sampleH[i] = NULL;
      globals.music.palm_midi_streaming.drum_sampleH[i] = NULL;
    }
    MemHandleUnlock(globals.music.palm_midi_streaming.default_inst_sampleH);
    DmReleaseResource(globals.music.palm_midi_streaming.default_inst_sampleH);
    MemHandleUnlock(globals.music.palm_midi_streaming.default_drum_sampleH);
    DmReleaseResource(globals.music.palm_midi_streaming.default_drum_sampleH);

    // free memory used
    if (globals.music.palm_midi_streaming.globals)
    {
      MemPtrFree(globals.music.palm_midi_streaming.globals);
      globals.music.palm_midi_streaming.globals = NULL;
    }

    // the stream is no longer active, mark as so.
    globals.music.palm_midi_streaming.active = false;

    // unprotect and zap the memory cache
    globals.music.dbID = DmFindDatabase(globals.music.dbCard, cacheMusicFileName);
    if (globals.music.dbID != NULL)
    {
      DmDatabaseProtect(globals.music.dbCard, globals.music.dbID, false);
      DmDeleteDatabase(globals.music.dbCard, globals.music.dbID);
    }
  }
#endif

#ifdef PALM_MIDI_YAMAHAPA1
  // using yamaha midi engine
  else
  if (globals.music.yamaha_midi.device)
  {
    Boolean state;
    UInt16  useCount;

    // do we have an active stream?
    if (globals.music.yamaha_midi.active)
      YamahaPA1LsmfStop(globals.music.yamaha_midi.libRef,
                        globals.music.yamaha_midi.midiHInternal, &state);

    // NEW Yamaha Library: (sony enhanced audio)
    if (globals.audio.yamaha_sfx.sony_enhanced_audio)
    {
      // make sure the volume is calibrated correctly
      {
        Err   err;
        void *sndOff;

        // NEW sony way
        err = FtrGet('SsYs', 10005, (UInt32 *)&sndOff);
        if ((err == errNone) && (sndOff))
        {
          ((SonyPA1LsndStateOffType)sndOff)(SonySndKindSp);
          ((SonyPA1LsndStateOffType)sndOff)(SonySndKindHp);
        }

        // OLD yamaha way
        else
        {
          // not required
        }
      }
    }

    // unlock midi handle
    if (globals.music.yamaha_midi.midiH)
    {
      YamahaPA1LsmfClose(globals.music.yamaha_midi.libRef,
                         globals.music.yamaha_midi.midiHInternal, &state);

      MemHandleUnlock(globals.music.yamaha_midi.midiH);
      DmReleaseResource(globals.music.yamaha_midi.midiH);
    }
    globals.music.yamaha_midi.midiH = NULL;

    // close the yamaha library
	  YamahaPA1LibClose(globals.music.yamaha_midi.libRef, &useCount);
    if (useCount == 0)
      SysLibRemove(globals.music.yamaha_midi.libRef);

    // the playback is no longer active, mark as so.
    globals.music.yamaha_midi.active = false;

    // unprotect and zap the memory cache
    globals.music.dbID = DmFindDatabase(globals.music.dbCard, cacheMusicFileName);
    if (globals.music.dbID != NULL)
    {
      DmDatabaseProtect(globals.music.dbCard, globals.music.dbID, false);
      DmDeleteDatabase(globals.music.dbCard, globals.music.dbID);
    }
  }
#endif
}

/**
 * Process the object movement in the game.
 *
 * @param prefs the global preference data.
 */
void
GameMovement(PreferencesType *prefs)
{
#if USE_PALMOS_WINAPI
  RectangleType rect    = { {   0,   0 }, {   0,   0 } };
  RectangleType scrRect = { {   0,   0 }, {   0,   0 } };
#endif
  UInt16   i, j, count;
  Coord    cx = 0;
  Coord    cy = 0;
  Lemming *lemmingPtr;

  //
  // ADJUST CURSOR
  //

  if (prefs->game.cursor.y < CURSOR_YOFFSET)
    prefs->game.cursor.y = CURSOR_YOFFSET;
  if (prefs->game.cursor.y > SCREEN_HEIGHT - CURSOR_YOFFSET)
    prefs->game.cursor.y = SCREEN_HEIGHT - CURSOR_YOFFSET;

  // cursor trying to move <-- direction?
  if (prefs->game.cursor.x < CURSOR_XOFFSET)
  {
    // pushing right on edge?
    if (prefs->game.cursor.x < (CURSOR_XOFFSET - 1))
    {
      Int16 pos;

      // try to shift the whole screen
      pos  = prefs->game.cursor.screenOffset;
      pos -= SCROLL_OFFSET;
      pos  = pos & ~0x07; // bind to 8 pixel boundary

      // make sure we dont go off scale here
      if (pos < 0) pos = 0; else
      if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
        pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);

      // jump to location
      prefs->game.cursor.screenOffset = pos;
      GraphicsSetOffset(prefs->game.cursor.screenOffset);
    }

    // stop there buddy :)
    prefs->game.cursor.x = CURSOR_XOFFSET;
  }

  // cursor trying to move --> direction?
  if (prefs->game.cursor.x > (SCREEN_WIDTH - CURSOR_XOFFSET))
  {
    // pushing right on edge?
    if (prefs->game.cursor.x > (SCREEN_WIDTH - (CURSOR_XOFFSET - 1)))
    {
      Int16 pos;

      // try to shift the whole screen
      pos  = prefs->game.cursor.screenOffset;
      pos += SCROLL_OFFSET;
      pos  = pos & ~0x07; // bind to 8 pixel boundary

      // make sure we dont go off scale here
      if (pos < 0) pos = 0; else
      if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
        pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);

      // jump to location
      prefs->game.cursor.screenOffset = pos;
      GraphicsSetOffset(prefs->game.cursor.screenOffset);
    }

    // stop there buddy :)
    prefs->game.cursor.x = (SCREEN_WIDTH - CURSOR_XOFFSET);
  }

  //
  // ADJUST LEMMINGS
  //

  cx = prefs->game.cursor.x + prefs->game.cursor.screenOffset;
  cy = prefs->game.cursor.y + 7;  // adjust for cursor/sprite diff
  prefs->game.cursor.spriteID = spr_undefined;

  // if the game is paused, no point moving anyone :P
  if (prefs->game.gamePaused) goto MOVEMENT_DONE;

  // playback music! :)
  GameMusicPlayback(prefs);

  // move the lemmings
  count = 0;
  for (i = 0; i < prefs->game.lemmingCount; i++)
  {
    lemmingPtr = &prefs->game.lemming[i];

    // is this lemming alive?
    if (lemmingPtr->alive)
    {
      switch (lemmingPtr->spriteType)
      {
        case spr_walkLeft:
        case spr_walkRight:
             {
               Int16 _x, x, y;

               // move the little dude
               if (lemmingPtr->facingRight)
                 lemmingPtr->x++;
               else
                 lemmingPtr->x--;

               // update animation
               lemmingPtr->spritePos =
                 (lemmingPtr->spritePos + 1) % SPR_WALK_CNT;
               lemmingPtr->animCounter++;

               x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_FOOTX;
               y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;
               if (lemmingPtr->facingRight) _x = x-1; else _x = x+1;

               // have we just walked into the exit?
               if ((prefs->game.lemmingExitX == x) &&
                   (prefs->game.lemmingExitY == y))
               {
                 lemmingPtr->spriteType   = spr_homer;
                 lemmingPtr->facingRight  = false;
                 lemmingPtr->spritePos    = 0;
                 lemmingPtr->animCounter  = 0;
                 lemmingPtr->fallDistance = 0;

                 // yay, saved a lemming
                 prefs->game.lemmingSaved++;
               }

               // is there a blocker in the way?
               else
               if (GameBlockerAtPosition(prefs, _x, x, y))
               {
                 // turn around
                 if (lemmingPtr->facingRight)
                   lemmingPtr->spriteType = spr_walkLeft;
                 else
                   lemmingPtr->spriteType = spr_walkRight;
                 lemmingPtr->facingRight = !lemmingPtr->facingRight;
               }

               // walking up hill or walked into a wall
               else
               if (GameObstacleAtPosition(x, y))
               {
                 y--;
                 if (GameObstacleAtPosition(x, y))
                 {
                   y--;
                   if (GameObstacleAtPosition(x, y))
                   {
                     y--;
                     if (GameObstacleAtPosition(x, y))
                     {
                       y--;
                       if (GameObstacleAtPosition(x, y))
                       {
                         y--;
                         if (GameObstacleAtPosition(x, y))
                         {
                           // we hit a wall, climb?
                           if (lemmingPtr->flags.climber)
                           {
                             if (lemmingPtr->facingRight)
                               lemmingPtr->spriteType = spr_climbRight;
                             else
                               lemmingPtr->spriteType = spr_climbLeft;

                             lemmingPtr->y--;  // adjust animation offsets
                           }

                           // we hit a wall, turn around
                           else
                           {
                             if (lemmingPtr->facingRight)
                               lemmingPtr->spriteType = spr_walkLeft;
                             else
                               lemmingPtr->spriteType = spr_walkRight;
                             lemmingPtr->facingRight = !lemmingPtr->facingRight;
                           }

                           lemmingPtr->spritePos    = 0;
                           lemmingPtr->animCounter  = 0;
                           lemmingPtr->fallDistance = 0;
                         }
                         else
                           lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
                       }
                       else
                         lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
                     }
                     else
                       lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
                   }
                   else
                     lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
                 }
                 else
                   lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
               }

               // walking down hill or fall off the edge
               else
               {
                 // lets see how we have to "walk" down
                 if (!GameObstacleAtPosition(x, y+1))
                 {
                   y++;
                   if (!GameObstacleAtPosition(x, y+1))
                   {
                     y++;
                     if (!GameObstacleAtPosition(x, y+1))
                     {
                       y++;
                       if (!GameObstacleAtPosition(x, y+1))
                       {
                         y++;
                         if (!GameObstacleAtPosition(x, y+1))
                         {
                           // nothing! argh - fall!
                           if (lemmingPtr->facingRight)
                             lemmingPtr->spriteType = spr_fallRight;
                           else
                             lemmingPtr->spriteType = spr_fallLeft;

                           lemmingPtr->spritePos    = 0;
                           lemmingPtr->animCounter  = 0;
                           lemmingPtr->fallDistance = 0;
                         }
                         else
                           lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
                       }
                       else
                         lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
                     }
                     else
                       lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
                   }
                   else
                     lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
                 }
                 else
                   lemmingPtr->y = y + (SPR_HEIGHT - LEMMING_FOOTY) + 1;
               }
             }

             break;

        case spr_fallLeft:
        case spr_fallRight:
             {
               Int16   x, y;
               Boolean hitFloor;

               // move the little dude
               lemmingPtr->y++;

               x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_FOOTX;
               y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;

               hitFloor = false;
               if (GameObstacleAtPosition(x, y))
                 hitFloor = true;
               else
               {
                 lemmingPtr->y++;
                 lemmingPtr->fallDistance++;
                 hitFloor = GameObstacleAtPosition(x, y+1);
               }

               // ran into the ground?
               if (hitFloor)
               {
                 // distance = ok, convert to a walker
#if CHEAT_MODE
                 if ((lemmingPtr->fallDistance < (SPR_HEIGHT * 3.5)) ||
                     (prefs->game.cheat.invincible))
#else
                 if (lemmingPtr->fallDistance < (SPR_HEIGHT * 3.5))
#endif
                 {
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_walkRight;
                   else
                     lemmingPtr->spriteType = spr_walkLeft;
                 }

                 // oh-no! *splat*
                 else
                 {
                   lemmingPtr->spriteType   = spr_splatter;

                   // 'splat!' audio playback
                   GamePlaySound(prefs, snd_splat);
                 }

                 lemmingPtr->spritePos    = 0;
                 lemmingPtr->animCounter  = 0;
                 lemmingPtr->fallDistance = 0;
               }

               // update animation
               else
               {
                 lemmingPtr->spritePos =
                   (lemmingPtr->spritePos + 1) % SPR_FALL_CNT;
                 lemmingPtr->animCounter++;

                 lemmingPtr->fallDistance++;

                 // is this lemming a lucky guy?
                 if (lemmingPtr->flags.floater &&
                     (lemmingPtr->fallDistance > SPR_HEIGHT))
                 {
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_floatRight;
                   else
                     lemmingPtr->spriteType = spr_floatLeft;

                   lemmingPtr->spritePos    = 0;
                   lemmingPtr->animCounter  = 0;
                   lemmingPtr->fallDistance = 0;
                 }
               }
             }

             break;

        case spr_digger:
             {
               Int16   x, y;
               Boolean special, diggable;

               // update animation
               lemmingPtr->spritePos =
                 (lemmingPtr->spritePos + 1) % SPR_DIGGER_CNT;

               // operation: special?
               special = !lemmingPtr->facingRight;

               if ((lemmingPtr->spritePos == ((SPR_DIGGER_CNT >> 1) - 1)) ||
                   (lemmingPtr->spritePos == (SPR_DIGGER_CNT - 1)))
               {
                 x = lemmingPtr->x - LEMMING_XOFFSET + 2;
                 y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;

                 // lets assume we cannot dig
                 diggable = false;
                 j=9;
                 do
                 {
                   // can dig? great! flag it
                   if (GameDiggableAtPosition(x,y,true))
                     diggable = true;
                   else

                   // if we run into a rock, bail out.. sorry
                   if (GameGetMaskPixel(x,y) == 2)
                   {
                     diggable = false; break;
                   }
                   x++;
                 }
                 while (--j);

                 // oh-oh, get out of here :P
                 if (!diggable)
                 {
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_walkRight;
                   else
                     lemmingPtr->spriteType = spr_walkLeft;
                   lemmingPtr->spritePos    = 0;
                   lemmingPtr->animCounter  = 0;
                 }

                 // move the little dude
                 else
                 {
                   lemmingPtr->y++;

                   {
                     x = lemmingPtr->x - LEMMING_XOFFSET;
                     y = lemmingPtr->y - LEMMING_YOFFSET;

                     // determine the mask we need to blit
                     globals.fnGameGenerateMask(msk_undefined, 2, x, y,
                                                 globals.ptrMaskMask,
                                                 globals.ptrDispMask, special);

#if USE_PALMOS_WINAPI
                     scrRect.topLeft.x = x;
                     scrRect.topLeft.y = y + SPR_HEIGHT;
                     scrRect.extent.x  = SPR_WIDTH;
                     scrRect.extent.y  = SPR_HEIGHT;

                     rect.topLeft.x    = 0;
                     rect.topLeft.y    = 0;
                     rect.extent.x     = SPR_WIDTH;
                     rect.extent.y     = SPR_HEIGHT;

                     WinCopyRectangle(globals.winMaskTemporary[1],
                       GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y,
                       DeviceSupportsColor() ? winOverlay : winMask);
#else
                     // dig out the bit on the screen
                     globals.fnGameDrawMask(globals.ptrDispMask, x, y);
#endif

                     // make sure the mask is removed from the background
                     GameDrawMaskMask(globals.ptrMaskMask, x, y);
                   }
                 }
               }
             }

             break;

        case spr_bashRight:
        case spr_bashLeft:
             {
               Int16   x, y;
               Boolean special, diggable;
               UInt8   index = 0;

               // update animation
               lemmingPtr->spritePos =
                 (lemmingPtr->spritePos + 1) % SPR_BASH_CNT;
               lemmingPtr->animCounter++;

               // operation: special?
               special = !lemmingPtr->facingRight;

               // need to shift over a pixel?
               if ((lemmingPtr->spritePos ==  0) ||
                   (lemmingPtr->spritePos ==  5) ||
                   (lemmingPtr->spritePos == 22) ||
                   (lemmingPtr->spritePos == 28))
               {
                 // lets check the 'validity' of our bash
                 if (lemmingPtr->spritePos == 5)      // our first real move
                 {
                   if (lemmingPtr->facingRight)
                     x = lemmingPtr->x - LEMMING_XOFFSET + 13;
                   else
                     x = lemmingPtr->x - LEMMING_XOFFSET + 2;
                   y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;

                   // lets assume we cannot dig
                   diggable = false;
                   j=12;
                   do
                   {
                     y--;

                     // can dig? great! flag it
                     if (GameDiggableAtPosition(x,y,special))
                       diggable = true;
                     else

                     // if we run into a rock, bail out.. sorry
                     if (GameGetMaskPixel(x,y) == 2)
                     {
                       diggable = false; break;
                     }
                   }
                   while (--j);

                   // oh-oh, get out of here :P
                   if (!diggable)
                   {
                     if (lemmingPtr->facingRight)
                       lemmingPtr->spriteType = spr_walkRight;
                     else
                       lemmingPtr->spriteType = spr_walkLeft;
                     lemmingPtr->spritePos    = 0;
                     lemmingPtr->animCounter  = 0;

                     goto BASH_ABORT;
                   }
                 }

                 // move forward
                 if (lemmingPtr->facingRight)
                   lemmingPtr->x++;
                 else
                   lemmingPtr->x--;
               }

               index = 0;
               if (lemmingPtr->spritePos ==  3) index = 1; else
               if (lemmingPtr->spritePos ==  4) index = 2; else
               if (lemmingPtr->spritePos ==  5) index = 3; else
               if (lemmingPtr->spritePos ==  7) index = 4; else
               if (lemmingPtr->spritePos == 19) index = 1; else
               if (lemmingPtr->spritePos == 20) index = 2; else
               if (lemmingPtr->spritePos == 21) index = 3; else
               if (lemmingPtr->spritePos == 23) index = 4;

               // we have just bashed a bit out.. clean up :P
               if ((index != 0) && (lemmingPtr->animCounter >= 5))
               {
                 x = lemmingPtr->x - LEMMING_XOFFSET;
                 y = lemmingPtr->y - LEMMING_YOFFSET;

                 index--;
                 if (lemmingPtr->facingRight) x++; else { x--; index += 4; }

                 // determine the mask we need to blit
                 globals.fnGameGenerateMask(msk_undefined, 8 + index, x, y,
                                             globals.ptrMaskMask,
                                             globals.ptrDispMask, special);
#if USE_PALMOS_WINAPI
                 scrRect.topLeft.x = x;
                 scrRect.topLeft.y = y + SPR_HEIGHT;
                 scrRect.extent.x  = SPR_WIDTH;
                 scrRect.extent.y  = SPR_HEIGHT;

                 rect.topLeft.x    = 0;
                 rect.topLeft.y    = 0;
                 rect.extent.x     = SPR_WIDTH;
                 rect.extent.y     = SPR_HEIGHT;

                 WinCopyRectangle(globals.winMaskTemporary[1],
                   GraphicsGetDrawWindow(),
                   &rect, scrRect.topLeft.x, scrRect.topLeft.y,
                   DeviceSupportsColor() ? winOverlay : winMask);
#else
                 // dig out the bit on the screen
                 globals.fnGameDrawMask(globals.ptrDispMask, x, y);
#endif

                 // make sure the mask is removed from the background
                 GameDrawMaskMask(globals.ptrMaskMask, x, y);
               }
             }

BASH_ABORT:

             break;

        case spr_floatLeft:
        case spr_floatRight:

             // update animation
             if (lemmingPtr->spritePos < 4)
             {
               // move the little dude every second frame
               if (((lemmingPtr->animCounter % 2) == 0) ||
                   (lemmingPtr->spritePos < 2))
               {
                 Int16 x, y;

                 lemmingPtr->y++;

                 x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_FOOTX;
                 y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;

                 // ran into the ground?
                 if (GameObstacleAtPosition(x, y))
                 {
                   // convert to a walker
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_walkRight;
                   else
                     lemmingPtr->spriteType = spr_walkLeft;
                   lemmingPtr->spritePos   = 0;
                   lemmingPtr->animCounter = 0;
                 }
               }

               // update animation [if still ok]
               if ((lemmingPtr->spriteType == spr_floatRight) ||
                   (lemmingPtr->spriteType == spr_floatLeft))
               {
                 lemmingPtr->spritePos++;
                 lemmingPtr->animCounter++;
               }

               // finisihed opening umbrella?
               if (lemmingPtr->spritePos == 4)
                 lemmingPtr->animCounter = 0;
             }
             else
             {
               Int16 x, y;

               // move the little dude
               lemmingPtr->y++;

               x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_FOOTX;
               y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;

               // ran into the ground?
               if (GameObstacleAtPosition(x, y))
               {
                 // convert to a walker
                 if (lemmingPtr->facingRight)
                   lemmingPtr->spriteType = spr_walkRight;
                 else
                   lemmingPtr->spriteType = spr_walkLeft;
                 lemmingPtr->spritePos   = 0;
                 lemmingPtr->animCounter = 0;
               }

               // update animation [if still ok]
               if ((lemmingPtr->spriteType == spr_floatRight) ||
                   (lemmingPtr->spriteType == spr_floatLeft))
               {
                 UInt8 animations[8] = { 4, 5, 6, 7, 7, 6, 5, 4 };

                 lemmingPtr->spritePos =
                   animations[lemmingPtr->animCounter % 8];
                 lemmingPtr->animCounter++;
               }
             }
             break;

        case spr_blocker:

             // update animation
             lemmingPtr->spritePos =
               (lemmingPtr->spritePos + 1) % SPR_BLOCK_CNT;
             lemmingPtr->animCounter++;

             break;

        case spr_exploder:

             // do we take a chunk out of the level mask?
             if (lemmingPtr->spritePos == 16)  // point of "bang"
             {
               Int16 x, y;

               // 'pop!' audio playback
               GamePlaySound(prefs, snd_explode);

               for (j=0; j < 2; j++)
               {
                 x = lemmingPtr->x - LEMMING_XOFFSET;
                 y = lemmingPtr->y + (j * SPR_HEIGHT) -
                     LEMMING_YOFFSET - (SPR_HEIGHT >> 1);

#if !FULL_SCREEN_BLIT
                 // adjust the drawing band [its different here]
                 if (y < globals.display.drawY1) globals.display.drawY1 = y;
                 if ((y + SPR_HEIGHT) > globals.display.drawY2)
                   globals.display.drawY2 = y + SPR_HEIGHT;
#endif

                 // determine the mask we need to blit
                 globals.fnGameGenerateMask(msk_undefined, j, x, y,
                                             globals.ptrMaskMask,
                                             globals.ptrDispMask, true);
#if USE_PALMOS_WINAPI
                 scrRect.topLeft.x = x;
                 scrRect.topLeft.y = y + SPR_HEIGHT;
                 scrRect.extent.x  = SPR_WIDTH;
                 scrRect.extent.y  = SPR_HEIGHT;

                 rect.topLeft.x    = 0;
                 rect.topLeft.y    = 0;
                 rect.extent.x     = SPR_WIDTH;
                 rect.extent.y     = SPR_HEIGHT;

                 WinCopyRectangle(globals.winMaskTemporary[1],
                   GraphicsGetDrawWindow(),
                   &rect, scrRect.topLeft.x, scrRect.topLeft.y,
                   DeviceSupportsColor() ? winOverlay : winMask);
#else
                 // dig out the bit on the screen
                 globals.fnGameDrawMask(globals.ptrDispMask, x, y);
#endif

                 // make sure the mask is removed from the background
                 GameDrawMaskMask(globals.ptrMaskMask, x, y);
               }
             }

             // update animation
             lemmingPtr->spritePos =
               (lemmingPtr->spritePos + 1) % SPR_XPLODE_CNT;
             lemmingPtr->animCounter++;

             // animation over? lemming dead :P
             if (lemmingPtr->animCounter >= SPR_XPLODE_CNT)
             {
               lemmingPtr->alive = false;
               prefs->game.lemmingDead++;
             }

             break;

        case spr_splatter:

             // update animation
             lemmingPtr->spritePos =
               (lemmingPtr->spritePos + 1) % SPR_SPLAT_CNT;
             lemmingPtr->animCounter++;

             // animation over? lemming dead :P
             if (lemmingPtr->animCounter >= SPR_SPLAT_CNT)
             {
               lemmingPtr->alive = false;
               prefs->game.lemmingDead++;
             }

             break;

        case spr_homer:

             // made it home 'yippee' audio playback
             if (lemmingPtr->animCounter == 0)
             {
               GamePlaySound(prefs, snd_yippee);
             }

             // update animation
             lemmingPtr->spritePos =
               (lemmingPtr->spritePos + 1) % SPR_HOMER_CNT;
             lemmingPtr->animCounter++;

             // animation over? lemming made it home :P
             if (lemmingPtr->animCounter >= SPR_HOMER_CNT)
               lemmingPtr->alive = false;
             break;

        case spr_climbRight:
        case spr_climbLeft:

             // move the little dude every second frame
             if ((lemmingPtr->animCounter % 2) == 0)
             {
               Int16 x, _x, y;

               x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_FOOTX;
               y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_CLIMBY;
               if (lemmingPtr->facingRight) _x = x-1; else _x = x+1;

               // is there nothing up the wall?
               if (GameObstacleAtPosition(x, y-1) &&
                   !GameObstacleAtPosition(_x, y-1))
               {
                 lemmingPtr->y--;
               }

               // convert to a hop-up/walker sprite
               else
               {
                 if (lemmingPtr->facingRight) _x = x+1; else _x = x-1;

                 // obstacle ahead, - convert to faller
                 if (GameObstacleAtPosition(x, y-1))
                 {
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_fallLeft;
                   else
                     lemmingPtr->spriteType = spr_fallRight;
                   lemmingPtr->facingRight = !lemmingPtr->facingRight;
                   lemmingPtr->spritePos   = 0;
                   lemmingPtr->animCounter = 0;
                 }

                 // no obstacle, climb up (if possible)
                 else
                 if (!GameObstacleAtPosition(_x, y-1))
                 {
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_hopupRight;
                   else
                     lemmingPtr->spriteType = spr_hopupLeft;
                   lemmingPtr->spritePos   = 0;
                   lemmingPtr->animCounter = 0;
                 }

                 // there is a ledge, allow lemming to follow it.
                 else
                 {
                   lemmingPtr->x = _x;
                   lemmingPtr->y--;

                   // special case, maybe sloped edge, must climb up [can walk]
                   if (!GameObstacleAtPosition(_x, y-2) &&
                       !GameObstacleAtPosition(_x, y-3) &&
                       !GameObstacleAtPosition(_x, y-4))
                   {
                     if (lemmingPtr->facingRight)
                       lemmingPtr->spriteType = spr_hopupRight;
                     else
                       lemmingPtr->spriteType = spr_hopupLeft;
                     lemmingPtr->spritePos   = 0;
                     lemmingPtr->animCounter = 0;
                   }
                 }
               }
             }

             // update animation [if still ok]
             if ((lemmingPtr->spriteType == spr_climbRight) ||
                 (lemmingPtr->spriteType == spr_climbLeft))
             {
               lemmingPtr->spritePos =
                 (lemmingPtr->spritePos + 1) % SPR_CLIMB_CNT;
               lemmingPtr->animCounter++;
             }
             break;

        case spr_hopupRight:
        case spr_hopupLeft:

             // have we finished hopping up?
             if (lemmingPtr->animCounter == (SPR_HOPUP_CNT-1))
             {
               if (lemmingPtr->facingRight)
                 lemmingPtr->x++;
               else
                 lemmingPtr->x--;

               if (lemmingPtr->facingRight)
                 lemmingPtr->spriteType = spr_walkRight;
               else
                 lemmingPtr->spriteType = spr_walkLeft;
               lemmingPtr->spritePos   = 0;
               lemmingPtr->animCounter = 0;

               lemmingPtr->y -= 2;  // for the animation *g*
             }

             // update animation [if still ok]
             if ((lemmingPtr->spriteType == spr_hopupRight) ||
                 (lemmingPtr->spriteType == spr_hopupLeft))
             {
               lemmingPtr->spritePos =
                 (lemmingPtr->spritePos + 1) % SPR_HOPUP_CNT;
               lemmingPtr->animCounter++;
             }

             break;

        case spr_buildRight:
        case spr_buildLeft:
             {
               Int16 _x, x, y;
               UInt8 index = 0;

               // update animation
               lemmingPtr->spritePos =
                 (lemmingPtr->spritePos + 1) % SPR_BUILD_CNT;
               lemmingPtr->animCounter++;

               // have we finished the "build" loop?
               if ((lemmingPtr->animCounter % SPR_BUILD_CNT) == 0)
               {
                 // lets check if we can move forward with the building
                 if (lemmingPtr->facingRight)
                   x = lemmingPtr->x - LEMMING_XOFFSET + 10;
                 else
                   x = lemmingPtr->x - LEMMING_XOFFSET + 5;
                 y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;
                 if (lemmingPtr->facingRight) _x = x-2; else _x = x+1;

                 // is there a blocker in the way?
                 if (GameBlockerAtPosition(prefs, _x, x, y))
                 {
                   // turn around (keep building)
                   if (lemmingPtr->facingRight)
                   {
                     lemmingPtr->x += 4;
                     lemmingPtr->spriteType = spr_buildLeft;
                   }
                   else
                   {
                     lemmingPtr->x -= 4;
                     lemmingPtr->spriteType = spr_buildRight;
                   }
                   lemmingPtr->facingRight = !lemmingPtr->facingRight;
                 }

                 // there is an obstacle there, argh.. walker mode
                 else
                 if (GameObstacleAtPosition(x, y-3))
                 {
                   // turn around and walk
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_walkLeft;
                   else
                     lemmingPtr->spriteType = spr_walkRight;
                   lemmingPtr->facingRight  = !lemmingPtr->facingRight;

                   lemmingPtr->spritePos    = 0;
                   lemmingPtr->animCounter  = 0;

                   goto BUILD_ABORT;
                 }

                 // move forward
                 if (lemmingPtr->facingRight)
                   lemmingPtr->x += 2;
                 else
                   lemmingPtr->x -= 2;
                 lemmingPtr->y--;

                 // has the lemming hit his head? stop and turn around
                 x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_HEADX;
                 y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_HEADY;

                 if (GameObstacleAtPosition(x, y))
                 {
                   // turn around (walk)
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_walkLeft;
                   else
                     lemmingPtr->spriteType = spr_walkRight;
                   lemmingPtr->facingRight  = !lemmingPtr->facingRight;
                   lemmingPtr->spritePos   = 0;
                   lemmingPtr->animCounter = 0;
                 }

BUILD_ABORT:

               }

               // time to insert the brick to the screen/mask?
               if (lemmingPtr->spritePos == 9)
               {
                 if (!lemmingPtr->facingRight) index++;

                 {
                   x = lemmingPtr->x - LEMMING_XOFFSET;
                   y = lemmingPtr->y - LEMMING_YOFFSET;

#if USE_PALMOS_WINAPI
                   scrRect.topLeft.x = x;
                   scrRect.topLeft.y = y + SPR_HEIGHT;
                   scrRect.extent.x  = SPR_WIDTH;
                   scrRect.extent.y  = SPR_HEIGHT;

                   // draw the brick on screen!
                   GameGetLemmingRectangle(spr_undefined, 13 + index, &rect);
                   WinCopyRectangle(globals.winLemmingsMask,
                     GraphicsGetDrawWindow(), &rect,
                     scrRect.topLeft.x, scrRect.topLeft.y, winMask);

                   WinCopyRectangle(globals.winLemmings,
                     GraphicsGetDrawWindow(),
                     &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
#else
                   // draw the brick on screen!
                   globals.fnGameDrawSprite(globals.ptrScratch,
                                             spr_undefined, 13 + index, x, y);
#endif

                   // determine the mask we need to blit
                   globals.fnGameGenerateMask(msk_undefined, 5 + index, x, y,
                                               globals.ptrMaskMask,
                                               globals.ptrDispMask, false);

                   // make sure the mask is placed in the background
                   GameDrawDrawMask(globals.ptrMaskMask, x, y);
                 }

                 // time to play "chink"?
                 if (lemmingPtr->animCounter > (SPR_BUILD_CNT * 9))
                 {
                   // 'ting' audio playback
                   GamePlaySound(prefs, snd_ting);
                 }
               }

               // have we finished our "bag of bricks"?
               if (lemmingPtr->animCounter == (SPR_BUILD_CNT * 12))
               {
                 // stop the building, wave for the users attention!
                 if (lemmingPtr->facingRight)
                   lemmingPtr->spriteType = spr_waveRight;
                 else
                   lemmingPtr->spriteType = spr_waveLeft;
                 lemmingPtr->spritePos   = 0;
                 lemmingPtr->animCounter = 0;
               }
             }

             break;

        case spr_waveRight:
        case spr_waveLeft:

             // update animation
             lemmingPtr->spritePos =
               (lemmingPtr->spritePos + 1) % SPR_WAVE_CNT;
             lemmingPtr->animCounter++;

             // animation over? bad luck? walker now
             if (lemmingPtr->animCounter >= SPR_WAVE_CNT)
             {
               // stop the building.
               if (lemmingPtr->facingRight)
                 lemmingPtr->spriteType = spr_walkRight;
               else
                 lemmingPtr->spriteType = spr_walkLeft;
               lemmingPtr->spritePos   = 0;
               lemmingPtr->animCounter = 0;
             }

             break;

        case spr_mineRight:
        case spr_mineLeft:
             {
               Int16   x, y;
               Boolean special, diggable;
               UInt8   index = 0;

               // update animation
               lemmingPtr->spritePos =
                 (lemmingPtr->spritePos + 1) % SPR_MINE_CNT;
               lemmingPtr->animCounter++;

               // operation: special?
               special = !lemmingPtr->facingRight;

               // time to move forward?
               if (lemmingPtr->spritePos == 15)
               {
                 x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_FOOTX;
                 y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;
                 if (!lemmingPtr->facingRight) x -= 8;

                 // lets assume we cannot dig
                 diggable = false;
                 j=8;
                 do
                 {
                   // can dig? great! flag it
                   if (GameDiggableAtPosition(x,y,special))
                     diggable = true;
                   else

                   // if we run into a rock, bail out.. sorry
                   if (GameGetMaskPixel(x,y) == 2)
                   {
                     diggable = false; break;
                   }
                   x++;
                 }
                 while (--j);

                 // oh-oh, get out of here :P
                 if (!diggable)
                 {
                   if (lemmingPtr->facingRight)
                     lemmingPtr->spriteType = spr_walkRight;
                   else
                     lemmingPtr->spriteType = spr_walkLeft;
                   lemmingPtr->spritePos   = 0;
                   lemmingPtr->animCounter = 0;
                 }

                 // move the little dude
                 else
                 {
                   if (lemmingPtr->facingRight)
                     lemmingPtr->x += 3;
                   else
                     lemmingPtr->x -= 3;
                   lemmingPtr->y++;
                 }
               }

               // time to take a chunk out of the dirt?
               else
               if (lemmingPtr->spritePos == 2)
               {
                 if (!lemmingPtr->facingRight) index++;

                 {
                   x = lemmingPtr->x - LEMMING_XOFFSET;
                   y = lemmingPtr->y - LEMMING_YOFFSET + 1;
                   if (lemmingPtr->facingRight) x += 2; else x -= 2;

                   // determine the mask we need to blit
                   globals.fnGameGenerateMask(msk_undefined, 3 + index, x, y,
                                               globals.ptrMaskMask,
                                               globals.ptrDispMask, special);

#if USE_PALMOS_WINAPI
                   scrRect.topLeft.x = x;
                   scrRect.topLeft.y = y + SPR_HEIGHT;
                   scrRect.extent.x  = SPR_WIDTH;
                   scrRect.extent.y  = SPR_HEIGHT;

                   rect.topLeft.x    = 0;
                   rect.topLeft.y    = 0;
                   rect.extent.x     = SPR_WIDTH;
                   rect.extent.y     = SPR_HEIGHT;

                   WinCopyRectangle(globals.winMaskTemporary[1],
                     GraphicsGetDrawWindow(),
                     &rect, scrRect.topLeft.x, scrRect.topLeft.y,
                     DeviceSupportsColor() ? winOverlay : winMask);
#else
                   // dig out the bit on the screen
                   globals.fnGameDrawMask(globals.ptrDispMask, x, y);
#endif

                   // make sure the mask is removed from the background
                   GameDrawMaskMask(globals.ptrMaskMask, x, y);
                 }
               }
             }

             break;

        case spr_drowner:

             // update animation
             lemmingPtr->spritePos =
               (lemmingPtr->spritePos + 1) % SPR_DROWN_CNT;
             lemmingPtr->animCounter++;

             // animation over? lemming dead :)
             if (lemmingPtr->animCounter >= SPR_DROWN_CNT)
             {
               lemmingPtr->alive = false;
               prefs->game.lemmingDead++;
             }

             break;

        default:
             break;
      }

      // do gravity rules apply?
      if ((lemmingPtr->spriteType == spr_bashRight) ||
          (lemmingPtr->spriteType == spr_bashLeft)  ||
          (lemmingPtr->spriteType == spr_blocker)   ||
          (lemmingPtr->spriteType == spr_exploder))
      {
        Int16 x, y;

        x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_FOOTX;
        y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY + 1;

        // no ground at feet?
        if (!GameObstacleAtPosition(x, y))
        {
          // if it was a blocker, better remove it from our "list"
          if (lemmingPtr->spriteType == spr_blocker)
          {
            // find it..
            j = prefs->game.blockerCount-1;
            while (prefs->game.blckID[j] != i) { j--; }

            // not from end? (gotta shift the elements down)
            if (j != prefs->game.blockerCount-1)
            {
              MemMove(&prefs->game.blckID[j],   // dest
                      &prefs->game.blckID[j+1], // source
                      (prefs->game.blockerCount-j) * sizeof(UInt8));
            }
            prefs->game.blockerCount--;
          }

          // if it was an exploder, he should just fall (move down screen)
          if (lemmingPtr->spriteType == spr_exploder)
          {
            // only fall if shaking...
            if (lemmingPtr->spritePos < 16)  // point of "bang"
              lemmingPtr->y += 2;
          }

          // convert to a walker
          else
          {
            if (lemmingPtr->facingRight)
              lemmingPtr->spriteType = spr_walkRight;
            else
              lemmingPtr->spriteType = spr_walkLeft;
            lemmingPtr->spritePos   = 0;
            lemmingPtr->animCounter = 0;
          }
        }
      }

#define LEMMING_XTHRESHOLD (LEMMING_XOFFSET >> 1)

      // has the lemming just moved to far along screen (to edge etc)
      if ((lemmingPtr->x <= LEMMING_XTHRESHOLD) ||
          (lemmingPtr->x >= OFFSCREEN_WIDTH-SPR_WIDTH+LEMMING_XTHRESHOLD))
      {
      	// turn lemming around, and, cancel activity
        if (lemmingPtr->facingRight)
          lemmingPtr->spriteType = spr_walkLeft;
        else
          lemmingPtr->spriteType = spr_walkRight;

        lemmingPtr->facingRight = !lemmingPtr->facingRight;
        lemmingPtr->spritePos   = 0;
        lemmingPtr->animCounter = 0;

        // force lemming within screen boundaries
        if (lemmingPtr->x <= LEMMING_XTHRESHOLD)
          lemmingPtr->x = LEMMING_XTHRESHOLD + 1;
        if (lemmingPtr->x >= OFFSCREEN_WIDTH-SPR_WIDTH+LEMMING_XTHRESHOLD)
          lemmingPtr->x = OFFSCREEN_WIDTH-SPR_WIDTH+LEMMING_XTHRESHOLD - 1;
      }

      // has the lemming fallen off screen?
      if (lemmingPtr->y >= (SCREEN_HEIGHT + LEMMING_YOFFSET))
      {
        lemmingPtr->alive = false;
        prefs->game.lemmingDead++;
      }

      // we need to keep track of the alive ones :P
      if (lemmingPtr->alive) count++;
    }
  }

  // move on with animations
  prefs->game.animationCounter++;
  if ((prefs->game.animationCounter % GAME_FPS) == 0)
  {
    prefs->game.timeRemaining--;

    //
    // STAGE EXIT: ran out of time?
    //

    if (prefs->game.timeRemaining == 0)
    {
      // we are now at the end of the level
      prefs->game.gameState = GAME_END;

      // need to get out now
      goto MOVEMENT_DONE;
    }
  }

  // have we let all the lemmings out yet?
  if ((prefs->game.lemmingCount < prefs->game.lemmingOut) &&
      (!prefs->game.nukeOccuring))
  {
    // is it time?
    if (((prefs->game.lemmingLastOutAt == 0) &&
         (prefs->game.animationCounter == (GAME_FPS << 1))  // 2 sec delay
        ) ||
        ((prefs->game.lemmingLastOutAt != 0) &&
          (prefs->game.animationCounter -
           prefs->game.lemmingLastOutAt) >= prefs->game.lemmingRate)
        )
    {
      Lemming *lemmingPtr;

      // yay, another lemming
      i = prefs->game.lemmingCount++;

      lemmingPtr = &prefs->game.lemming[i];
      // a lemming is born!
      lemmingPtr->alive         = true;
      lemmingPtr->visible       = true;
      lemmingPtr->x             = prefs->game.lemmingStartX;
      lemmingPtr->y             = prefs->game.lemmingStartY;
      lemmingPtr->spriteType    = spr_fallRight;
      lemmingPtr->spritePos     = 0;
      lemmingPtr->animCounter   = 0;
      lemmingPtr->fallDistance  = 0;
      lemmingPtr->facingRight   = true;
      lemmingPtr->nuke          = false;
      lemmingPtr->nukeInTicks   = 0;
      lemmingPtr->nukeCounter   = 0;
      lemmingPtr->flags.climber = false;
      lemmingPtr->flags.floater = false;

      // update counters
      prefs->game.lemmingLastOutAt = prefs->game.animationCounter;
    }
  }
  else
  {
    //
    // STAGE EXIT: all lemmings are out and none are on screen anymore :)
    //

    if (count == 0)
    {
      // we are now at the end of the level
      prefs->game.gameState = GAME_END;
    }
  }

MOVEMENT_DONE:

  //
  // ADJUST VISUAL SETTINGS
  //

  // determine which lemmings are visible on screen.
  for (i = 0; i < prefs->game.lemmingCount; i++)
  {
    lemmingPtr = &prefs->game.lemming[i];

    // is it possible the lemming is visible on the display area?
    lemmingPtr->visible =
     (((lemmingPtr->x +
         LEMMING_XOFFSET) > prefs->game.cursor.screenOffset) &&
      ((lemmingPtr->x - LEMMING_XOFFSET) <
        (prefs->game.cursor.screenOffset + SCREEN_WIDTH))
     ) && lemmingPtr->alive;

    // cursor not defined and is over lemming?
    if (((prefs->game.cursor.spriteID == spr_undefined)  ||
         (prefs->game.cursor.spriteID == spr_blocker)    ||
         (prefs->game.cursor.spriteID == spr_splatter)   ||
         (prefs->game.cursor.spriteID == spr_exploder)   ||
         (prefs->game.cursor.spriteID == spr_climbLeft)  ||
         (prefs->game.cursor.spriteID == spr_climbRight) ||
         (prefs->game.cursor.spriteID == spr_hopupLeft)  ||
         (prefs->game.cursor.spriteID == spr_hopupRight) ||
         (prefs->game.cursor.spriteID == spr_floatLeft)  ||
         (prefs->game.cursor.spriteID == spr_floatRight) ||
         (prefs->game.cursor.spriteID == spr_fallLeft)   ||
         (prefs->game.cursor.spriteID == spr_fallRight)) &&
        (lemmingPtr->alive) &&  // must be alive :P
        (ABS(lemmingPtr->x - cx) < CURSOR_XOFFSET) &&
        (ABS(lemmingPtr->y - cy) < CURSOR_YOFFSET)
       )
    {
      prefs->game.cursor.lemming  = i;
      prefs->game.cursor.spriteID = lemmingPtr->spriteType;
    }
  }

  // is it possible the flames are visible on screen?
  prefs->game.flamesVisible =
   (
    ((prefs->game.flamesX[0] > prefs->game.cursor.screenOffset) &&
     (prefs->game.flamesX[0] <
      (prefs->game.cursor.screenOffset + SCREEN_WIDTH)))
   ) ||
   (
     ((prefs->game.flamesX[1] > prefs->game.cursor.screenOffset) &&
      (prefs->game.flamesX[1] <
       (prefs->game.cursor.screenOffset + SCREEN_WIDTH)))
   );
}

/**
 * Draw the game on the screen.
 *
 * @param prefs the global preference data.
 */
void
GameDraw(PreferencesType *prefs)
{
#if USE_PALMOS_WINAPI
  RectangleType rect    = { {   0,   0 }, {   0,   0 } };
  RectangleType scrRect = { {   0,   0 }, {   0,   0 } };
#endif
  RectangleType clockRect;
  Lemming       *lemmingPtr;
  Boolean       clockShown, statusShown;
  Coord         x, y;
  Int16         i, j;
#if !FULL_SCREEN_BLIT
  Coord         y1, y2;
#endif

  // assume status and clock is shown
  clockShown  = true;
  statusShown = true;

  clockRect.extent.x  = (SPR_WIDTH * 2) + (SPR_WIDTH >> 1);
  clockRect.extent.y  = SPR_HEIGHT;
  clockRect.topLeft.x = prefs->game.cursor.screenOffset + SCREEN_WIDTH - clockRect.extent.x;
  clockRect.topLeft.y = SCREEN_HEIGHT - clockRect.extent.y;

#if !FULL_SCREEN_BLIT
  // assume some bogus y1-y2 area for update
  y1      = SCREEN_HEIGHT - 1;
  y2      = 0;
#endif

  // need to switch back from rate counter?
  if (globals.display.rateCounter != 0)
  {
    globals.display.rateCounter--;

    // switch back to normal counter
    if (globals.display.rateCounter == 0)
      GameChangeTool(prefs, prefs->game.activeTool,
                     prefs->game.tools[prefs->game.activeTool]);
  }

  //
  // DRAWING
  //

  // draw the "flames" on the background bitmap
  if (prefs->game.flamesVisible)
  {
    for (i=0; i < MAX_FLAMES; i++)
    {
      x = prefs->game.flamesX[i];
      y = prefs->game.flamesY[i];

#if USE_PALMOS_WINAPI
      // backup the area behind the lemming
      GameBackupTile(x, y, globals.backupWidth, SPR_HEIGHT,
                     globals.winFlameBackup[i]);

      scrRect.topLeft.x = x;
      scrRect.topLeft.y = y + SPR_HEIGHT;
      scrRect.extent.x  = SPR_WIDTH;
      scrRect.extent.y  = SPR_HEIGHT;

      // draw the flame!
      GameGetLemmingRectangle(spr_undefined,
        60 + ((prefs->game.animationCounter + (i * 2)) & 3), &rect);
      WinCopyRectangle(globals.winLemmingsMask, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winMask);

      WinCopyRectangle(globals.winLemmings, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
#else
      // backup the area behind the flame & draw the flame
      globals.fnGameDrawSprite(globals.ptrFlameBackup[i], spr_undefined,
        60 + ((prefs->game.animationCounter + (i * 2)) & 3), x, y);
#endif
    }
  }

  // draw the "lemming" at its current position
  for (i=0; i < prefs->game.lemmingCount; i++)
  {
    lemmingPtr = &prefs->game.lemming[i];
    if (lemmingPtr->visible)
    {
      // special checks (hide clock)?
      if (clockShown)
        clockShown &= !(RctPtInRectangle(lemmingPtr->x, lemmingPtr->y, &clockRect));

      x = lemmingPtr->x - LEMMING_XOFFSET;
      y = lemmingPtr->y - LEMMING_YOFFSET;  // lemming is 16x16

#if !FULL_SCREEN_BLIT
      // adjust the drawing band
      if (y < y1) y1 = y;
      if (y > y2) y2 = y;
#endif

      // not being nuked [and, can view 5..4..3..2..1]?
      if ((!lemmingPtr->nuke) ||
          (lemmingPtr->nukeInTicks != 0) || (y < 1)) goto DRAW_LEMMING;

      y -= SPR_HEIGHT;
#if !FULL_SCREEN_BLIT
      // adjust the drawing band
      if (y < y1) y1 = y;
      if (y > y2) y2 = y;
#endif

#if USE_PALMOS_WINAPI
      // backup the area behind the lemming
      GameBackupTile(x, y, globals.backupWidth, SPR_HEIGHT,
                     globals.winLemmingsBackup[i+MAX_LEMMINGS]);

      scrRect.topLeft.x = x;
      scrRect.topLeft.y = y + SPR_HEIGHT;
      scrRect.extent.x  = SPR_WIDTH;
      scrRect.extent.y  = SPR_HEIGHT;

      // draw the lemming!
      GameGetLemmingRectangle(spr_undefined,
                              72 + (4 - (lemmingPtr->nukeCounter / GAME_FPS)),
                              &rect);
      WinCopyRectangle(globals.winLemmingsMask, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winMask);

      WinCopyRectangle(globals.winLemmings, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
#else
      // backup the area behind the lemming & draw the lemming
      globals.fnGameDrawSprite(
        globals.ptrLemmingsBackup[i+MAX_LEMMINGS],
        spr_undefined, 72 + (4 - (lemmingPtr->nukeCounter / GAME_FPS)), x, y);
#endif
      y += SPR_HEIGHT;

DRAW_LEMMING:

#if USE_PALMOS_WINAPI
      // backup the area behind the lemming
      GameBackupTile(x, y, globals.backupWidth, SPR_HEIGHT,
                     globals.winLemmingsBackup[i]);

      scrRect.topLeft.x = x;
      scrRect.topLeft.y = y + SPR_HEIGHT;
      scrRect.extent.x  = SPR_WIDTH;
      scrRect.extent.y  = SPR_HEIGHT;

      // draw the lemming!
      GameGetLemmingRectangle(lemmingPtr->spriteType,
                              lemmingPtr->spritePos, &rect);
      WinCopyRectangle(globals.winLemmingsMask, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winMask);

      WinCopyRectangle(globals.winLemmings, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
#else
      // backup the area behind the lemming & draw the lemming
      globals.fnGameDrawSprite(globals.ptrLemmingsBackup[i],
                                lemmingPtr->spriteType, lemmingPtr->spritePos,
                                x, y);
#endif
    }
  }

  // draw the "status" information at onto the offscreen window
  {
    UInt16 z[4];

    // highlighted lemming info :)
    switch (prefs->game.cursor.spriteID)
    {
      case spr_walkRight:
      case spr_walkLeft:
           z[0] = 16;
           break;

      case spr_fallRight:
      case spr_fallLeft:
           z[0] = 20;
           break;

      case spr_climbRight:
      case spr_climbLeft:
           z[0] = 24;
           break;

      case spr_floatRight:
      case spr_floatLeft:
           z[0] = 28;
           break;

      case spr_blocker:
           z[0] = 32;
           break;

      case spr_waveRight:
      case spr_waveLeft:
      case spr_buildRight:
      case spr_buildLeft:
           z[0] = 36;
           break;

      case spr_bashRight:
      case spr_bashLeft:
           z[0] = 40;
           break;

      case spr_mineRight:
      case spr_mineLeft:
           z[0] = 44;
           break;

      case spr_digger:
           z[0] = 48;
           break;

      case spr_homer:
           z[0] = 64;
           break;

      case spr_drowner:
      case spr_exploder:
      case spr_splatter:
           z[0] = 68;
           break;

      default:
      case spr_undefined:

           // we are not to show anything
           statusShown = false;
           break;
    }

    // only do this if required.
    if (statusShown)
    {
      z[1] = z[0] + 1;
      z[2] = z[1] + 1;
      z[3] = z[2] + 1;

      x = prefs->game.cursor.screenOffset - SPR_WIDTH + 4;
      y = SCREEN_HEIGHT - SPR_HEIGHT;
      for (i=0; i<4; i++)
      {
        x += SPR_WIDTH;

#if USE_PALMOS_WINAPI
        // backup the area behind the status area
        GameBackupTile(x, y, globals.backupWidth, SPR_HEIGHT,
                       globals.winStatusBackup[i]);

        scrRect.topLeft.x = x;
        scrRect.topLeft.y = y + SPR_HEIGHT;
        scrRect.extent.x  = SPR_WIDTH;
        scrRect.extent.y  = SPR_HEIGHT;

        // draw the status area!
        GameGetLemmingRectangle(spr_undefined, z[i], &rect);
        WinCopyRectangle(globals.winLemmingsMask, GraphicsGetDrawWindow(),
                         &rect, scrRect.topLeft.x, scrRect.topLeft.y, winMask);

        WinCopyRectangle(globals.winLemmings, GraphicsGetDrawWindow(),
                         &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
#else
        // backup the area behind the status area & draw the status area
        globals.fnGameDrawSprite(globals.ptrStatusBackup[i],
                                  spr_undefined, z[i], x, y);
#endif
      }
    }

    // only do this if required.
    if (clockShown)
    {
      // X:XX (time)
      z[0] = (prefs->game.timeRemaining / 60) % 10;  // min
      z[1] = 10;                                     // :
      z[2] = (prefs->game.timeRemaining % 60) / 10;  // sec
      z[3] = (prefs->game.timeRemaining % 60) % 10;  // sec

      x = SCREEN_WIDTH - (SPR_WIDTH * 2) - (SPR_WIDTH >> 1) + // 2.5 from edge
          prefs->game.cursor.screenOffset;
      y = SCREEN_HEIGHT - SPR_HEIGHT;
      for (i=4; i<8; i++)
      {
  #if USE_PALMOS_WINAPI
        // backup the area behind the status area
        GameBackupTile(x, y, globals.backupWidth, SPR_HEIGHT,
                       globals.winStatusBackup[i]);

        scrRect.topLeft.x = x;
        scrRect.topLeft.y = y + SPR_HEIGHT;
        scrRect.extent.x  = SPR_WIDTH;
        scrRect.extent.y  = SPR_HEIGHT;

        // draw the status area!
        GameGetLemmingRectangle(spr_undefined, z[i-4], &rect);
        WinCopyRectangle(globals.winLemmingsMask, GraphicsGetDrawWindow(),
                         &rect, scrRect.topLeft.x, scrRect.topLeft.y, winMask);

        WinCopyRectangle(globals.winLemmings, GraphicsGetDrawWindow(),
                         &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
  #else
        // backup the area behind the status area & draw the status area
        globals.fnGameDrawSprite(globals.ptrStatusBackup[i],
                                  spr_undefined, z[i-4], x, y);
  #endif

        x += (SPR_WIDTH >> 1);
      }
    }
  }

  // if the game is paused, show the 'PAUSED' bitmap
  if (prefs->game.gamePaused)
  {
    x = (SCREEN_WIDTH >> 1) - (SPR_WIDTH << 1) +   // center the paused bitmap
        prefs->game.cursor.screenOffset;
    y = SCREEN_PAUSE_Y1;

    for (i=0; i<4; i++)
    {
#if USE_PALMOS_WINAPI
      // backup the area behind the status area
      GameBackupTile(x, y, globals.backupWidth, SPR_HEIGHT,
                     globals.winPauseBackup[i]);

      scrRect.topLeft.x = x;
      scrRect.topLeft.y = y + SPR_HEIGHT;
      scrRect.extent.x  = SPR_WIDTH;
      scrRect.extent.y  = SPR_HEIGHT;

      // draw the pause area!
      GameGetLemmingRectangle(spr_undefined, 56 + i, &rect);
      WinCopyRectangle(globals.winLemmingsMask, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winMask);

      WinCopyRectangle(globals.winLemmings, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
#else
      // backup the area behind the status area & draw the status area
      globals.fnGameDrawSprite(globals.ptrPauseBackup[i],
                                spr_undefined, 56 + i, x, y);
#endif

      x += SPR_WIDTH;
    }
  }

#if CHEAT_MODE
  // is the user cheating? show the 'CHEAT!' bitmap
  if (prefs->game.cheat.active)
  {
    x = (SCREEN_WIDTH >> 1) - (SPR_WIDTH << 1) +   // center the cheat bitmap
        prefs->game.cursor.screenOffset;
    y = SCREEN_CHEAT_Y1;

    for (i=0; i<4; i++)
    {
#if USE_PALMOS_WINAPI
      // backup the area behind the status area
      GameBackupTile(x, y, globals.backupWidth, SPR_HEIGHT,
                     globals.winCheatBackup[i]);

      scrRect.topLeft.x = x;
      scrRect.topLeft.y = y + SPR_HEIGHT;
      scrRect.extent.x  = SPR_WIDTH;
      scrRect.extent.y  = SPR_HEIGHT;

      // draw the pause area!
      GameGetLemmingRectangle(spr_undefined, 52 + i, &rect);
      WinCopyRectangle(globals.winLemmingsMask, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winMask);

      WinCopyRectangle(globals.winLemmings, GraphicsGetDrawWindow(),
                       &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
#else
      // backup the area behind the status area & draw the status area
      globals.fnGameDrawSprite(globals.ptrCheatBackup[i],
                                spr_undefined, 52 + i, x, y);
#endif

      x += SPR_WIDTH;
    }
  }
#endif

  // draw the "cursor" at its current position
  {
    x = prefs->game.cursor.x - CURSOR_XOFFSET + prefs->game.cursor.screenOffset;
    y = prefs->game.cursor.y - CURSOR_YOFFSET;

#if USE_PALMOS_WINAPI
    // backup the area behind the arrow
    GameBackupTile(x, y,
                   globals.backupWidth, SPR_HEIGHT, globals.winCursorBackup);

    scrRect.topLeft.x = x;
    scrRect.topLeft.y = y + SPR_HEIGHT;
    scrRect.extent.x  = SPR_WIDTH;
    scrRect.extent.y  = SPR_HEIGHT;

    // draw the cursor!
    GameGetLemmingRectangle(spr_undefined,
      (prefs->game.cursor.spriteID == spr_undefined) ? 11 : 12, &rect);
    WinCopyRectangle(globals.winLemmingsMask, GraphicsGetDrawWindow(),
                     &rect, scrRect.topLeft.x, scrRect.topLeft.y, winMask);

    WinCopyRectangle(globals.winLemmings, GraphicsGetDrawWindow(),
                     &rect, scrRect.topLeft.x, scrRect.topLeft.y, winOverlay);
#else
    // backup the area behind the lemming & draw the lemming
    globals.fnGameDrawSprite(globals.ptrCursorBackup, spr_undefined,
      (prefs->game.cursor.spriteID == spr_undefined) ? 11 : 12, x, y);
#endif
  }

#if FULL_SCREEN_BLIT
  // blit the graphics to the screen
  GraphicsRepaint();
#else
  // blit the graphics to the screen
  if (globals.display.screenOffset != prefs->game.cursor.screenOffset)
  {
    // full screen update
    GraphicsSetUpdate(0,SCREEN_HEIGHT-1);
    GraphicsRepaint();
  }
  else
  {
    Coord finalY1, finalY2;

    // merge the remove/addition of lemmings areas
    finalY1 = (y1 < globals.display.drawY1) ? y1 : globals.display.drawY1;
    finalY2 = (y2 > globals.display.drawY2) ? y2 : globals.display.drawY2;

    // update the lemmings
    GraphicsSetUpdate(finalY1, finalY2);
#if SHOW_Y_UPDATE_BARS
    GraphicsRepaintShowBars();
#else
    GraphicsRepaint();
#endif

    // update the fire animations
    if (prefs->game.flamesVisible)
    {
      GraphicsSetUpdate(prefs->game.flamesY[0] + 5,
                        prefs->game.flamesY[0] + 10); // small portion here *g*
      GraphicsRepaint();
    }

    // update the status band
    GraphicsSetUpdate(SCREEN_HEIGHT-SPR_HEIGHT, SCREEN_HEIGHT-1);
    GraphicsRepaint();

    // if paused, update the PAUSED area
    if (prefs->game.gamePaused)
    {
      GraphicsSetUpdate(SCREEN_PAUSE_Y1, SCREEN_PAUSE_Y2);
      GraphicsRepaint();
    }

#if CHEAT_MODE
    // if cheating, update the CHEAT area
    if (prefs->game.cheat.active)
    {
      GraphicsSetUpdate(SCREEN_CHEAT_Y1, SCREEN_CHEAT_Y2);
      GraphicsRepaint();
    }
#endif

    // need to update the cursor bands?
    if ((globals.display.dirty) ||
        (globals.display.oldCursorX  != prefs->game.cursor.x) ||
        (globals.display.oldCursorY  != prefs->game.cursor.y) ||
        (globals.display.oldSpriteID != prefs->game.cursor.spriteID))
    {
      // erase old
      GraphicsSetUpdate(globals.display.oldCursorY - CURSOR_YOFFSET,
                        globals.display.oldCursorY + CURSOR_YOFFSET + 1);
      GraphicsRepaint();

      // paint new
      GraphicsSetUpdate(prefs->game.cursor.y - CURSOR_YOFFSET,
                        prefs->game.cursor.y + CURSOR_YOFFSET + 1);
      GraphicsRepaint();
    }

    // ok, dont need to force a redraw of all :)
    globals.display.dirty = false;
  }

  // adjust "everything" as needed
  globals.display.screenOffset = prefs->game.cursor.screenOffset;
  globals.display.drawY1       = y1;
  globals.display.drawY2       = y2 + SPR_HEIGHT;  // for next time!
#endif

  globals.display.oldCursorX   = prefs->game.cursor.x;
  globals.display.oldCursorY   = prefs->game.cursor.y;
  globals.display.oldSpriteID  = prefs->game.cursor.spriteID;

  // draw the xx/xx xx% figures
  if (prefs->game.lemmingSaved != prefs->game.lemmingOut)
  {
    UInt8  str[64] = {};

/**
 ** xx/xx xx%
 **
    // how many lemmings were saved?
    UInt16 val;
    val = (prefs->game.lemmingSaved * 100) / prefs->game.lemmingOut;
    StrPrintF(str, "%02d/%02d - %02d%%",
                   prefs->game.lemmingCount, prefs->game.lemmingOut, val);
    if (prefs->game.lemmingSaved >= prefs->game.lemmingReq) str[6] = '+';    // flag it
 **/

/**
 ** out/pos(tot) x
 **/
    StrPrintF(str, "%02d/%02d(%02d) ",
                   prefs->game.lemmingSaved, prefs->game.lemmingOut - prefs->game.lemmingDead, prefs->game.lemmingOut);
         if (prefs->game.lemmingSaved == prefs->game.lemmingReq) StrCat(str, "! ");
    else if (prefs->game.lemmingSaved >  prefs->game.lemmingReq) StrCat(str, "+ ");
    else StrCat(str, "- ");

    FntSetFont(boldFont);
    WinDrawChars(str, StrLen(str), 64, 1);
    FntSetFont(stdFont);
  }

  //
  // ERASING
  //

  // erase the "cursor" at its current position
  {
    x = prefs->game.cursor.x - CURSOR_XOFFSET + GraphicsGetOffset();
    y = prefs->game.cursor.y - CURSOR_YOFFSET;

#if USE_PALMOS_WINAPI
    // restore the area behind the arrow
    GameRestoreTile(x, y, globals.backupWidth, SPR_HEIGHT,
                    globals.winCursorBackup);
#else
    // restore the area behind the arrow
    globals.fnGameSpriteRestore(globals.ptrCursorBackup, x, y);
#endif
  }

#if CHEAT_MODE
  // is the user cheating? erase the 'CHEAT!' bitmap
  if (prefs->game.cheat.active)
  {
    x = (SCREEN_WIDTH >> 1) + (SPR_WIDTH << 1) +   // center the cheat bitmap
        prefs->game.cursor.screenOffset;
    y = SCREEN_CHEAT_Y1;

    for (i = 4; i--; )
    {
      x -= SPR_WIDTH;

#if USE_PALMOS_WINAPI
      // restore the area behind the status area
      GameRestoreTile(x, y, globals.backupWidth, SPR_HEIGHT,
                      globals.winCheatBackup[i]);
#else
      // restore the area behind the status area
      globals.fnGameSpriteRestore(globals.ptrCheatBackup[i], x, y);
#endif
    }
  }
#endif

  // if the game is paused, erase the 'PAUSED' bitmap
  if (prefs->game.gamePaused)
  {
    x = (SCREEN_WIDTH >> 1) + (SPR_WIDTH << 1) +   // center the paused bitmap
        prefs->game.cursor.screenOffset;
    y = SCREEN_PAUSE_Y1;

    for (i = 4; i--; )
    {
      x -= SPR_WIDTH;

#if USE_PALMOS_WINAPI
      // restore the area behind the status area
      GameRestoreTile(x, y, globals.backupWidth, SPR_HEIGHT,
                      globals.winPauseBackup[i]);
#else
      // restore the area behind the status area
      globals.fnGameSpriteRestore(globals.ptrPauseBackup[i], x, y);
#endif
    }
  }

  // erase the "status" information at onto the offscreen window
  {
    // only do this if required.
    if (clockShown)
    {
      // X:XX (time)
      x = SCREEN_WIDTH - (SPR_WIDTH >> 1) + prefs->game.cursor.screenOffset;
      y = SCREEN_HEIGHT - SPR_HEIGHT;
      for (i = 4; i--; )
      {
        x -= (SPR_WIDTH >> 1);

  #if USE_PALMOS_WINAPI
        // restore the area behind the status area
        GameRestoreTile(x, y, globals.backupWidth, SPR_HEIGHT,
                        globals.winStatusBackup[4+i]);
  #else
        // restore the area behind the status area
        globals.fnGameSpriteRestore(globals.ptrStatusBackup[4+i], x, y);
  #endif
      }
    }

    // only do this if required.
    if (statusShown)
    {
      // highlighted lemming info :)
      x = prefs->game.cursor.screenOffset + (4 * SPR_WIDTH) + 4;
      y = SCREEN_HEIGHT - SPR_HEIGHT;
      for (i = 4; i--; )
      {
        x -= SPR_WIDTH;

#if USE_PALMOS_WINAPI
        // restore the area behind the status area
        GameRestoreTile(x, y, globals.backupWidth, SPR_HEIGHT,
                        globals.winStatusBackup[i]);
#else
        // restore the area behind the status area
        globals.fnGameSpriteRestore(globals.ptrStatusBackup[i], x, y);
#endif
      }
    }
  }

  // erase the "lemming" at its current position
  for (i = prefs->game.lemmingCount; i--;)
  {
    Lemming *lemmingPtr;

    lemmingPtr = &prefs->game.lemming[i];
    if (lemmingPtr->visible)
    {
      x = lemmingPtr->x - LEMMING_XOFFSET;
      y = lemmingPtr->y - LEMMING_YOFFSET;  // lemming is 16x16

      // not being nuked [and, can view 5..4..3..2..1]?
      if ((!lemmingPtr->nuke) ||
          (lemmingPtr->nukeInTicks != 0) || (y < 1)) goto ERASE_LEMMING;

      y -= SPR_HEIGHT;
#if USE_PALMOS_WINAPI
      // restore the area behind the lemming
      GameRestoreTile(x, y, globals.backupWidth, SPR_HEIGHT,
                      globals.winLemmingsBackup[i+MAX_LEMMINGS]);
#else
      // restore the area behind the lemming
      globals.fnGameSpriteRestore(
        globals.ptrLemmingsBackup[i+MAX_LEMMINGS], x, y);
#endif
      y += SPR_HEIGHT;

ERASE_LEMMING:

#if USE_PALMOS_WINAPI
      // restore the area behind the lemming
      GameRestoreTile(x, y, globals.backupWidth, SPR_HEIGHT,
                      globals.winLemmingsBackup[i]);
#else
      // restore the area behind the lemming
      globals.fnGameSpriteRestore(globals.ptrLemmingsBackup[i], x, y);
#endif
    }

    // lemming being nuked?
    if ((lemmingPtr->nuke) && (!prefs->game.gamePaused))
    {
      if (lemmingPtr->nukeInTicks != 0)
        lemmingPtr->nukeInTicks--;
      else
        lemmingPtr->nukeCounter++;

      // timer has expired? time to convert to "exploder"
      if (lemmingPtr->nukeCounter == (5 * GAME_FPS))
      {
        if ((lemmingPtr->spriteType != spr_exploder) &&
            (lemmingPtr->spriteType != spr_splatter))
        {
          // did we just nuke a blocker?
          if (lemmingPtr->spriteType == spr_blocker)
          {
            // find it..
            j = prefs->game.blockerCount-1;
            while (prefs->game.blckID[j] != i) { j--; }

            // not from end? (gotta shift the elements down)
            if (j != prefs->game.blockerCount-1)
            {
              MemMove(&prefs->game.blckID[j],   // dest
                      &prefs->game.blckID[j+1], // source
                      (prefs->game.blockerCount-j) * sizeof(UInt8));
            }
            prefs->game.blockerCount--;
          }

          if ((lemmingPtr->spriteType == spr_homer)      ||
              (lemmingPtr->spriteType == spr_climbLeft)  ||
              (lemmingPtr->spriteType == spr_climbRight) ||
              (lemmingPtr->spriteType == spr_fallLeft)   ||
              (lemmingPtr->spriteType == spr_fallRight)  ||
              (lemmingPtr->spriteType == spr_floatLeft)  ||
              (lemmingPtr->spriteType == spr_floatRight))
            lemmingPtr->spritePos  = 16;
          else
            lemmingPtr->spritePos  = 0;

          lemmingPtr->spriteType   = spr_exploder;
          lemmingPtr->animCounter  = lemmingPtr->spritePos;
          lemmingPtr->fallDistance = 0;

          // 'oh no!' audio playback
          GamePlaySound(prefs, snd_ohno);
        }

        // no longer being nuked *g*
        lemmingPtr->nuke         = false;
        lemmingPtr->nukeCounter  = 0;
      }
    }
  }

  // erase the "flames" from the background bitmap
  if (prefs->game.flamesVisible)
  {
    for (i=MAX_FLAMES; i--; )
    {
      x = prefs->game.flamesX[i];
      y = prefs->game.flamesY[i];

#if USE_PALMOS_WINAPI
      // restore the area behind the lemming
      GameRestoreTile(x, y, globals.backupWidth, SPR_HEIGHT,
                      globals.winFlameBackup[i]);
#else
      // restore the area behind the lemming
      globals.fnGameSpriteRestore(globals.ptrFlameBackup[i], x, y);
#endif
    }
  }
}

/**
 * Terminate the game.
 */
void GameTerminate(PreferencesType *prefs)
{
  UInt16 i;

  // shutdown the audio+music engines
  GameMusicTerminate();
#if PALM_AUDIO_ANY
  GameSfxTerminate();
#endif

  // unlock the gamepad driver (if available)
  if (globals.gamePad.device)
  {
    Err    err;
    UInt32 gamePadUserCount;

    err = GPDClose(globals.gamePad.libRef, &gamePadUserCount);
    if (gamePadUserCount == 0)
      SysLibRemove(globals.gamePad.libRef);
  }

  // clean up windows/memory
  if (globals.winLemmings)
    WinDeleteWindow(globals.winLemmings,             false);
  if (globals.winLemmingsMask)
    WinDeleteWindow(globals.winLemmingsMask,         false);
  if (globals.winGameMasks)
    WinDeleteWindow(globals.winGameMasks,            false);

#if NEW_MEMORY_MGR
  // pre PALMOS_5 - there aint enough heap - hack time
  if (!DeviceSupportsVersion(romVersion5))
  {
    // free the handles used
    if (globals.hanLemmings)
    {
      MemHandleUnlock(globals.hanLemmings);
      DmReleaseResource(globals.hanLemmings);
    }
    if (globals.hanLemmingsMask)
    {
      MemHandleUnlock(globals.hanLemmingsMask);
      DmReleaseResource(globals.hanLemmingsMask);
    }
    if (globals.hanGameMasks)
    {
      MemHandleUnlock(globals.hanGameMasks);
      DmReleaseResource(globals.hanGameMasks);
    }

    // unprotect and zap the memory cache
    globals.dbID = DmFindDatabase(globals.dbCard, cacheGameFileName);
    if (globals.dbID != NULL)
    {
      DmDatabaseProtect(globals.dbCard, globals.dbID, false);
      DmDeleteDatabase(globals.dbCard, globals.dbID);
    }
  }
#endif

  if (DeviceSupportsColor())
  {
    BmpDelete(globals.bmpGameMasks2bpp);
    if (globals.winGameMasks2bpp)
      WinDeleteWindow(globals.winGameMasks2bpp,      false);
  }

  for (i=0; i < MAX_LEMMINGS_WIN; i++)
  {
    if (globals.winLemmingsBackup[i])
      WinDeleteWindow(globals.winLemmingsBackup[i],  false);
  }
  for (i=0; i < MAX_STATUS; i++)
  {
    if (globals.winStatusBackup[i])
      WinDeleteWindow(globals.winStatusBackup[i],    false);
  }
  for (i=0; i < MAX_FLAMES; i++)
  {
    if (globals.winFlameBackup[i])
      WinDeleteWindow(globals.winFlameBackup[i],     false);
  }
  for (i=0; i < MAX_PAUSE; i++)
  {
    if (globals.winPauseBackup[i])
      WinDeleteWindow(globals.winPauseBackup[i],     false);
  }
  for (i=0; i < MAX_CHEAT; i++)
  {
    if (globals.winCheatBackup[i])
      WinDeleteWindow(globals.winCheatBackup[i],     false);
  }
  if (globals.winCursorBackup)
    WinDeleteWindow(globals.winCursorBackup,         false);
  for (i=0; i < 2; i++)
  {
    if (globals.winMaskTemporary[i])
      WinDeleteWindow(globals.winMaskTemporary[i],   false);
  }
  if (globals.winScratch)
    WinDeleteWindow(globals.winScratch,              false);

#if SHOW_MASK_IN_2BPP
  if (DeviceSupportsColor())
#endif
    if (globals.levelMask)
    {
      MemPtrFree(globals.levelMask);
      globals.levelMask = NULL;
    }
}

/**
 * Make any adjustments necessary for the stylus.
 *
 * @param prefs the global preferences data.
 * @param x the x-coordinate
 * @param y the y-coordinate
 */
static void
GameAdjustStylus(PreferencesType *prefs, Coord *x, Coord *y)
{
#if SONY_NATIVE
  if ((prefs->sony.device) && (prefs->config.widescreenDisplay))
  {
    // we must adjust to 320x320 pixel rates here..
    *x = *x << 1;
    *y = SCREEN_START + ((*y - SCREEN_START_STYLUS) << 1);
  }
#endif
#if PALM_HIDENSITY
  if ((prefs->palmHD.device) && (prefs->config.widescreenDisplay))
  {
    switch (prefs->palmHD.density)
    {
      case kDensityDouble:
           *x = *x << 1;
           *y = SCREEN_START + ((*y - SCREEN_START_STYLUS) << 1);
           break;

      default:
           break;
    }
  }
#endif

  // adjust for offset
  *y = *y - SCREEN_START;
}

/**
 * Assign a task to the currently highlighted lemming.
 *
 * @param prefs the global preference data.
 */
static void
GameAssignLemmingTask(PreferencesType *prefs)
{
  UInt16  i, x, y;
  Boolean taskOk;

  // lets assume the task was not ok
  taskOk = false;

  // which tool is active?
  switch (prefs->game.activeTool)
  {
    case TOOL_CLIMBER:

         // have we got a lemming selected?
         if (prefs->game.cursor.spriteID != spr_undefined)
         {
           Lemming *lemmingPtr;
           lemmingPtr = &prefs->game.lemming[prefs->game.cursor.lemming];

           // can we do it?
           if (!lemmingPtr->flags.climber)
           {
             if (prefs->game.tools[TOOL_CLIMBER] > 0)
             {
               lemmingPtr->flags.climber = true;
               prefs->game.tools[TOOL_CLIMBER]--;
               GameChangeTool(prefs,
                              TOOL_CLIMBER, prefs->game.tools[TOOL_CLIMBER]+1);

               taskOk = true;
               globals.lastKeyDelayCount = 4;  // wait frames before using again
             }
             else
             if (prefs->levelPack.type == PACK_INTERNAL)
             {
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, true);
#endif
               FrmAlert(toolNotAvailable);
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, prefs->game.gamePaused);
#endif
             }
           }
         }
         break;

    case TOOL_FLOATER:

         // have we got a lemming selected?
         if (prefs->game.cursor.spriteID != spr_undefined)
         {
           Lemming *lemmingPtr;
           lemmingPtr = &prefs->game.lemming[prefs->game.cursor.lemming];

           // can we do it?
           if (!lemmingPtr->flags.floater)
           {
             if (prefs->game.tools[TOOL_FLOATER] > 0)
             {
               lemmingPtr->flags.floater = true;
               prefs->game.tools[TOOL_FLOATER]--;
               GameChangeTool(prefs,
                              TOOL_FLOATER, prefs->game.tools[TOOL_FLOATER]+1);

               taskOk = true;
               globals.lastKeyDelayCount = 4;  // wait frames before using again
             }
             else
             if (prefs->levelPack.type == PACK_INTERNAL)
             {
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, true);
#endif
               FrmAlert(toolNotAvailable);
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, prefs->game.gamePaused);
#endif
             }
           }
         }
         break;

    case TOOL_EXPLODER:

         // got a lemming selected?
         if ((prefs->game.cursor.spriteID != spr_undefined) &&
             (prefs->game.cursor.spriteID != spr_homer)     &&
             (prefs->game.cursor.spriteID != spr_exploder))
         {
           Lemming *lemmingPtr;
           lemmingPtr = &prefs->game.lemming[prefs->game.cursor.lemming];

           // can we do it?
           if ((lemmingPtr->spriteType != spr_exploder) &&
               (!lemmingPtr->nuke))
           {
             if (prefs->game.tools[TOOL_EXPLODER] > 0)
             {
               lemmingPtr->nuke         = true;
               lemmingPtr->nukeCounter  = 0;     // this guy is getting nuked
               prefs->game.tools[TOOL_EXPLODER]--;
               GameChangeTool(prefs,
                              TOOL_EXPLODER, prefs->game.tools[TOOL_EXPLODER]+1);

               taskOk = true;
               globals.lastKeyDelayCount = 4;  // wait frames before using again
             }
             else
             if (prefs->levelPack.type == PACK_INTERNAL)
             {
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, true);
#endif
               FrmAlert(toolNotAvailable);
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, prefs->game.gamePaused);
#endif
             }
           }
         }
         break;

    case TOOL_BLOCKER:

         // have we selected a lemming we can mess with?
         if ((prefs->game.cursor.spriteID != spr_undefined)  &&
             (prefs->game.cursor.spriteID != spr_homer)      &&
             (prefs->game.cursor.spriteID != spr_fallRight)  &&
             (prefs->game.cursor.spriteID != spr_fallLeft)   &&
             (prefs->game.cursor.spriteID != spr_floatRight) &&
             (prefs->game.cursor.spriteID != spr_floatLeft)  &&
             (prefs->game.cursor.spriteID != spr_climbRight) &&
             (prefs->game.cursor.spriteID != spr_climbLeft)  &&
             (prefs->game.cursor.spriteID != spr_blocker)    &&
             (prefs->game.cursor.spriteID != spr_exploder)   &&
             (prefs->game.cursor.spriteID != spr_splatter))
         {
           Lemming *lemmingPtr;
           lemmingPtr = &prefs->game.lemming[prefs->game.cursor.lemming];

           // can we do it?
           if (lemmingPtr->spriteType != spr_blocker)
           {
             if (prefs->game.tools[TOOL_BLOCKER] > 0)
             {
               lemmingPtr->spriteType   = spr_blocker;
               lemmingPtr->spritePos    = 0;
               lemmingPtr->animCounter  = 0;
               lemmingPtr->fallDistance = 0;
               prefs->game.tools[TOOL_BLOCKER]--;
               GameChangeTool(prefs,
                              TOOL_BLOCKER, prefs->game.tools[TOOL_BLOCKER]+1);

               // add it to the "block" data section :)
               x = prefs->game.blockerCount++;
               prefs->game.blckID[x] = prefs->game.cursor.lemming;

               taskOk = true;
               globals.lastKeyDelayCount = 4;  // wait frames before using again
             }
             else
             if (prefs->levelPack.type == PACK_INTERNAL)
             {
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, true);
#endif
               FrmAlert(toolNotAvailable);
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, prefs->game.gamePaused);
#endif
             }
           }
         }
         break;

    case TOOL_BUILDER:

         // have we selected a lemming we can mess with?
         if ((prefs->game.cursor.spriteID != spr_undefined)  &&
             (prefs->game.cursor.spriteID != spr_homer)      &&
             (prefs->game.cursor.spriteID != spr_fallRight)  &&
             (prefs->game.cursor.spriteID != spr_fallLeft)   &&
             (prefs->game.cursor.spriteID != spr_floatRight) &&
             (prefs->game.cursor.spriteID != spr_floatLeft)  &&
             (prefs->game.cursor.spriteID != spr_climbRight) &&
             (prefs->game.cursor.spriteID != spr_climbLeft)  &&
             (prefs->game.cursor.spriteID != spr_buildRight) &&
             (prefs->game.cursor.spriteID != spr_buildLeft)  &&
             (prefs->game.cursor.spriteID != spr_blocker)    &&
             (prefs->game.cursor.spriteID != spr_exploder)   &&
             (prefs->game.cursor.spriteID != spr_splatter))
         {
           Lemming *lemmingPtr;
           lemmingPtr = &prefs->game.lemming[prefs->game.cursor.lemming];

           // can we do it?
           if ((lemmingPtr->spriteType != spr_buildRight) &&
               (lemmingPtr->spriteType != spr_buildLeft))
           {
             if (prefs->game.tools[TOOL_BUILDER] > 0)
             {
               if (lemmingPtr->facingRight)
                 lemmingPtr->spriteType   = spr_buildRight;
               else
                 lemmingPtr->spriteType   = spr_buildLeft;

               x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_FOOTX;
               y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_FOOTY;
               if ((!GameObstacleAtPosition(x-1, y)) ||
                   (!GameObstacleAtPosition(x,   y)) ||
                   (!GameObstacleAtPosition(x+1, y))) // crazy case.. it happens
                 lemmingPtr->y++;

               lemmingPtr->spritePos    = 0;
               lemmingPtr->animCounter  = 0;
               lemmingPtr->fallDistance = 0;
               prefs->game.tools[TOOL_BUILDER]--;
               GameChangeTool(prefs,
                              TOOL_BUILDER, prefs->game.tools[TOOL_BUILDER]+1);

               taskOk = true;
               globals.lastKeyDelayCount = 4;  // wait frames before using again
             }
             else
             if (prefs->levelPack.type == PACK_INTERNAL)
             {
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, true);
#endif
               FrmAlert(toolNotAvailable);
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, prefs->game.gamePaused);
#endif
             }
           }
         }
         break;

    case TOOL_BASHER:

         // have we selected a lemming we can mess with?
         if ((prefs->game.cursor.spriteID != spr_undefined)  &&
             (prefs->game.cursor.spriteID != spr_homer)      &&
             (prefs->game.cursor.spriteID != spr_fallRight)  &&
             (prefs->game.cursor.spriteID != spr_fallLeft)   &&
             (prefs->game.cursor.spriteID != spr_floatRight) &&
             (prefs->game.cursor.spriteID != spr_floatLeft)  &&
             (prefs->game.cursor.spriteID != spr_climbRight) &&
             (prefs->game.cursor.spriteID != spr_climbLeft)  &&
             (prefs->game.cursor.spriteID != spr_bashRight)  &&
             (prefs->game.cursor.spriteID != spr_bashLeft)   &&
             (prefs->game.cursor.spriteID != spr_blocker)    &&
             (prefs->game.cursor.spriteID != spr_exploder)   &&
             (prefs->game.cursor.spriteID != spr_splatter))
         {
           Lemming *lemmingPtr;
           lemmingPtr = &prefs->game.lemming[prefs->game.cursor.lemming];

           // can we do it?
           if ((lemmingPtr->spriteType != spr_bashLeft)  &&
               (lemmingPtr->spriteType != spr_bashRight))
           {
             if (prefs->game.tools[TOOL_BASHER] > 0)
             {
               if (lemmingPtr->facingRight)
                 lemmingPtr->spriteType   = spr_bashRight;
               else
                 lemmingPtr->spriteType   = spr_bashLeft;
               lemmingPtr->spritePos    = 0;
               lemmingPtr->animCounter  = 0;
               lemmingPtr->fallDistance = 0;
               prefs->game.tools[TOOL_BASHER]--;
               GameChangeTool(prefs,
                              TOOL_BASHER, prefs->game.tools[TOOL_BASHER]+1);

               taskOk = true;
               globals.lastKeyDelayCount = 4;  // wait frames before using again
             }
             else
             if (prefs->levelPack.type == PACK_INTERNAL)
             {
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, true);
#endif
               FrmAlert(toolNotAvailable);
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, prefs->game.gamePaused);
#endif
             }
           }
         }
         break;

    case TOOL_MINER:

         // have we selected a lemming we can mess with?
         if ((prefs->game.cursor.spriteID != spr_undefined)  &&
             (prefs->game.cursor.spriteID != spr_homer)      &&
             (prefs->game.cursor.spriteID != spr_fallRight)  &&
             (prefs->game.cursor.spriteID != spr_fallLeft)   &&
             (prefs->game.cursor.spriteID != spr_floatRight) &&
             (prefs->game.cursor.spriteID != spr_floatLeft)  &&
             (prefs->game.cursor.spriteID != spr_climbRight) &&
             (prefs->game.cursor.spriteID != spr_climbLeft)  &&
             (prefs->game.cursor.spriteID != spr_mineRight)  &&
             (prefs->game.cursor.spriteID != spr_mineLeft)   &&
             (prefs->game.cursor.spriteID != spr_blocker)    &&
             (prefs->game.cursor.spriteID != spr_exploder)   &&
             (prefs->game.cursor.spriteID != spr_splatter))
         {
           Lemming *lemmingPtr;
           lemmingPtr = &prefs->game.lemming[prefs->game.cursor.lemming];

           // can we do it?
           if ((lemmingPtr->spriteType != spr_mineRight) &&
               (lemmingPtr->spriteType != spr_mineLeft))
           {
             if (prefs->game.tools[TOOL_MINER] > 0)
             {
               if (lemmingPtr->facingRight)
                 lemmingPtr->spriteType   = spr_mineRight;
               else
                 lemmingPtr->spriteType   = spr_mineLeft;

               lemmingPtr->spritePos    = 0;
               lemmingPtr->animCounter  = 0;
               lemmingPtr->fallDistance = 0;
               prefs->game.tools[TOOL_MINER]--;
               GameChangeTool(prefs,
                              TOOL_MINER, prefs->game.tools[TOOL_MINER]+1);

               taskOk = true;
               globals.lastKeyDelayCount = 4;  // wait frames before using again
             }
             else
             if (prefs->levelPack.type == PACK_INTERNAL)
             {
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, true);
#endif
               FrmAlert(toolNotAvailable);
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, prefs->game.gamePaused);
#endif
             }
           }
         }
         break;

    case TOOL_DIGGER:

         // have we selected a lemming we can mess with?
         if ((prefs->game.cursor.spriteID != spr_undefined)  &&
             (prefs->game.cursor.spriteID != spr_homer)      &&
             (prefs->game.cursor.spriteID != spr_fallRight)  &&
             (prefs->game.cursor.spriteID != spr_fallLeft)   &&
             (prefs->game.cursor.spriteID != spr_floatRight) &&
             (prefs->game.cursor.spriteID != spr_floatLeft)  &&
             (prefs->game.cursor.spriteID != spr_climbRight) &&
             (prefs->game.cursor.spriteID != spr_climbLeft)  &&
             (prefs->game.cursor.spriteID != spr_digger)     &&
             (prefs->game.cursor.spriteID != spr_blocker)    &&
             (prefs->game.cursor.spriteID != spr_exploder)   &&
             (prefs->game.cursor.spriteID != spr_splatter))
         {
           Lemming *lemmingPtr;
           lemmingPtr = &prefs->game.lemming[prefs->game.cursor.lemming];

           // can we do it?
           if (lemmingPtr->spriteType != spr_digger)
           {
             if (prefs->game.tools[TOOL_DIGGER] > 0)
             {
               lemmingPtr->spriteType   = spr_digger;
               lemmingPtr->spritePos    = 0;
               lemmingPtr->animCounter  = 0;
               lemmingPtr->fallDistance = 0;
               prefs->game.tools[TOOL_DIGGER]--;
               GameChangeTool(prefs,
                              TOOL_DIGGER, prefs->game.tools[TOOL_DIGGER]+1);

               taskOk = true;
               globals.lastKeyDelayCount = 4;  // wait frames before using again
             }
             else
             if (prefs->levelPack.type == PACK_INTERNAL)
             {
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, true);
#endif
               FrmAlert(toolNotAvailable);
#if MIDI_PAUSE_ON_DIALOG
               GameMusicPause(prefs, prefs->game.gamePaused);
#endif
             }
           }
         }
         break;

    case TOOL_NUKE:

         // we must not be in "nuke" mode to start it :P
         if (!prefs->game.nukeOccuring)
         {
           // we are in nuke mode! argh!
           prefs->game.nukeOccuring = true;

           x = 0;
           for (i=0; i<prefs->game.lemmingCount; i++)
           {
             Lemming *lemmingPtr;
             lemmingPtr = &prefs->game.lemming[i];

             lemmingPtr->nuke         = true;
             lemmingPtr->nukeInTicks  = x;
             lemmingPtr->nukeCounter  = 0;     // this guy is getting nuked

             if (lemmingPtr->alive) x++;
           }
         }

         break;

    default:
         break;
  }

  // if the task was ok, we should probably do some audible feedback
  if (taskOk)
  {
    // 'selection' audio playback
    GamePlaySound(prefs, snd_selection);
  }
}

/**
 * Determine if the pixel at (x,y) in the mask bitmap is set.
 *
 * @param x the x-coordinate to test
 * @param y the y-coordinate to test
 * @return the pixel value at (x,y)
 */
static UInt8
GameGetMaskPixel(UInt16 x, UInt16 y)
{
  UInt8 result = globals.levelMask[(((y + SPR_HEIGHT) << 5) * 5) + (x >> 2)];
  result = (result >> ((~x & 3) << 1)) & 0x03;
  return result;
}

/**
 * Determine if there is an obstacle at position (x,y)
 *
 * @param x    the x-coordinate to test
 * @param y    the y-coordinate to test
 * @return true if obstacle at position (x,y), false otherwise
 */
static Boolean
GameObstacleAtPosition(UInt16 x, UInt16 y)
{
  return (GameGetMaskPixel(x, y) != 0);
}

/**
 * Determine if the position (x,y) is diggable (removable)
 *
 * @param x       the x-coordinate to test
 * @param y       the y-coordinate to test
 * @param special should special be taking into account?
 * @return true if obstacle at position (x,y), false otherwise
 */
static Boolean
GameDiggableAtPosition(UInt16 x, UInt16 y, Boolean special)
{
  Boolean result;
  UInt8   pixel;

  pixel  = GameGetMaskPixel(x, y);
  result = (pixel == 3);
  if (special) result |= (pixel == 1);

  return result;
}

/**
 * Determine is there is a blocker at the (x,y) position.
 *
 * @param prefs the global preference data.
 * @param oldX  the current x, [required] may be possible walk through if inside
 * @param chkX  the x-coordinate to test
 * @param chkY  the y-coordinate to test
 * @return true if a blocker (noticable) is at position (x,y), false otherwise
 */
static Boolean
GameBlockerAtPosition(PreferencesType *prefs,
                      UInt16 oldX, UInt16 chkX, UInt16 chkY)
{
  Boolean result;
  UInt16  i, x, y;

  // assume we didn't find one :)
  result = false;

  // scan through!
  for (i=0; i<prefs->game.blockerCount; i++)
  {
    Lemming *lemmingPtr;

    // access the first lemming
    lemmingPtr = &prefs->game.lemming[prefs->game.blckID[i]];
    x = lemmingPtr->x - LEMMING_XOFFSET + LEMMING_BLOCKX;
    y = lemmingPtr->y - LEMMING_YOFFSET + LEMMING_BLOCKY;

    // does (chkX, chkY) fit in?
    if ((chkX >= x) && (chkX <= (x + BLOCK_WIDTH))  &&
        (chkY >= y) && (chkY <= (y + BLOCK_HEIGHT)) && // within bounds?
       ((oldX <  x) || (oldX  > (x + BLOCK_WIDTH))))   // oldX is out :P
    {
      result = true;
      goto SEARCH_ABORT;
    }
  }

SEARCH_ABORT:

  return result;
}
