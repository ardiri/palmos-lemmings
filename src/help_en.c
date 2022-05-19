/*
 * @(#)help_en.c
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
 *
 * --------------------------------------------------------------------
 *             THIS FILE CONTAINS THE ENGLISH LANGUAGE TEXT
 * --------------------------------------------------------------------
 */

#include "palm.h"

typedef struct
{
#if SET_KEYMASK
  UInt32    keyMask;
#endif
  WinHandle helpWindow;

  Char      strData[256];  // temp data string

#if NEW_MEMORY_MGR
  UInt16    dbCard;
  LocalID   dbID;
  DmOpenRef dbMemoryStore;
  MemHandle hanOffScreen;
#endif
} HelpGlobals;

static HelpGlobals globals;

/**
 * Initialize the instructions screen.
 *
 * @return the height in pixels of the instructions data area.
 */
UInt16
InitInstructions()
{
  const RectangleType     rect  = {{0,0},{142,414}};
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
  UInt16    err;
  UInt16    result = 0;
#if NEW_MEMORY_MGR
  UInt32    scrDepth;
  UInt16    index, rowBytes, binSize;
  WinHandle winHandle;
  UInt16    e;
#endif

  // clear the globals object
  MemSet(&globals, sizeof(HelpGlobals), 0);

#if SET_KEYMASK
  // setup the valid keys available at this point in time
  globals.keyMask = KeySetMask(~(keyBitsAll ^
                              (keyBitPower    |
#if !DISABLE_HOTSYNC
                               keyBitCradle   | keyBitAntenna  |
#endif
                               keyBitPageUp   | keyBitPageDown |
#if HANDERA_NATIVE
                               keyBitJogUp    | keyBitJogDown  |
#endif
                               keyBitContrast | keyBitNavLRS)));
#endif

  // initialize windows
#if NEW_MEMORY_MGR
  // pre PALMOS_5 - there aint enough heap - hack time
  if (!DeviceSupportsVersion(romVersion5))
  {
    globals.hanOffScreen = NULL; globals.helpWindow = NULL;

    // determine the screen depth
    scrDepth = 1;
    if (DeviceSupportsVersion(romVersion3))
      WinScreenMode(winScreenModeGet,NULL,NULL,&scrDepth,NULL);

    err = errNone;

    // create the 'memory' store for the help image [kill previous]
    globals.dbCard = 0;
    globals.dbID   = DmFindDatabase(globals.dbCard, cacheHelpFileName);
    if (globals.dbID != NULL)
      DmDeleteDatabase(globals.dbCard, globals.dbID);

    err |= (DmCreateDatabase(globals.dbCard, cacheHelpFileName,
                             appCreator, cacheType, true) != errNone);
    if (err == errNone)
    {
      globals.dbID = DmFindDatabase(globals.dbCard, cacheHelpFileName);
      globals.dbMemoryStore =
        DmOpenDatabase(globals.dbCard, globals.dbID, dmModeReadWrite);

      rowBytes = (((rect.extent.x * scrDepth) + 15) >> 4) << 1;

      // palmos 3.5+
      if (DeviceSupportsVersion(romVersion3_5))
      {
        BitmapType *bmpPtr;

        // help window
        index = 0;
        binSize = sizeof(BitmapType) + (rowBytes * rect.extent.y);
        globals.hanOffScreen =
          DmNewResource(globals.dbMemoryStore, cacheType, index, binSize);
        err |= (globals.hanOffScreen == NULL);
        if (err != errNone) goto HELP_MEMORY_QUIT;

        MemSemaphoreReserve(true);
        bmpPtr = (BitmapType *)MemHandleLock(globals.hanOffScreen);
        MemSet(bmpPtr, binSize, 0);
        bmpPtr->width     = rect.extent.x;
        bmpPtr->height    = rect.extent.y;
        bmpPtr->rowBytes  = rowBytes;
        bmpPtr->pixelSize = scrDepth;
        bmpPtr->version   = BitmapVersionTwo;
        MemHandleUnlock(globals.hanOffScreen);
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
        bmpPtr = BmpCreate(SPR_WIDTH, SPR_HEIGHT, scrDepth, NULL, &e);
        err |= e;
        globals.helpWindow = WinCreateBitmapWindow(bmpPtr, &e); err |= e;
        err |= (globals.helpWindow == NULL);
        if (err != errNone) goto HELP_MEMORY_QUIT;
        // f*ck you palm :P - get around that darn limitation
        BmpDelete(bmpPtr);
        bmpPtr = (BitmapType *)MemHandleLock(globals.hanOffScreen);
        globals.helpWindow->bitmapP                = bmpPtr;
        globals.helpWindow->windowBounds.extent.x  = bmpPtr->width;
        globals.helpWindow->windowBounds.extent.y  = bmpPtr->height;
        globals.helpWindow->displayWidthV20        = bmpPtr->width;
        globals.helpWindow->displayHeightV20       = bmpPtr->height;
        globals.helpWindow->displayAddrV20         = bmpPtr;
        globals.helpWindow->windowFlags.freeBitmap = false;
        winHandle = WinSetDrawWindow(globals.helpWindow);
        WinResetClip();
        WinSetDrawWindow(winHandle);
      }

      // pre palmos 3.5
      else
      {
        GDeviceType *gDevicePtr;

        // help window
        index = 0;
        binSize = (rowBytes * rect.extent.y);
        globals.hanOffScreen =
          DmNewResource(globals.dbMemoryStore, cacheType, index, binSize);
        err |= (globals.hanOffScreen == NULL);
        if (err != errNone) goto HELP_MEMORY_QUIT;

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
        globals.helpWindow =
          WinCreateOffscreenWindow(SPR_WIDTH, SPR_HEIGHT, screenFormat, &e);
        err |= e; err |= (globals.helpWindow == NULL);
        if (err != errNone) goto HELP_MEMORY_QUIT;
        // f*ck you palm :P - get around that darn limitation
        gDevicePtr = (GDeviceType *)globals.helpWindow->bitmapP;
        MemPtrFree(gDevicePtr->baseAddr);
        gDevicePtr->baseAddr = MemHandleLock(globals.hanOffScreen);
        gDevicePtr->width                          = rect.extent.x;
        gDevicePtr->height                         = rect.extent.y;
        gDevicePtr->rowBytes                       = rowBytes;
        gDevicePtr->version                        = BitmapVersionZero;
        globals.helpWindow->windowBounds.extent.x = gDevicePtr->width;
        globals.helpWindow->windowBounds.extent.y = gDevicePtr->height;
        globals.helpWindow->displayWidthV20       = gDevicePtr->width;
        globals.helpWindow->displayHeightV20      = gDevicePtr->height;
        globals.helpWindow->displayAddrV20        = gDevicePtr->baseAddr;
        globals.helpWindow->windowFlags.offscreen = false;
        winHandle = WinSetDrawWindow(globals.helpWindow);
        WinResetClip();
        WinSetDrawWindow(winHandle);
      }

HELP_MEMORY_QUIT:

      // protect + close the database :)
      DmDatabaseProtect(globals.dbCard, globals.dbID, true);
      DmCloseDatabase(globals.dbMemoryStore);
    }
  }

  // post PALMOS_5 - there may be enough heap :)
  else
  {
    globals.helpWindow =
      WinCreateOffscreenWindow(rect.extent.x,rect.extent.y,screenFormat,&err);
    err |= (globals.helpWindow == NULL);
  }
#else
  globals.helpWindow =
    WinCreateOffscreenWindow(rect.extent.x,rect.extent.y,screenFormat,&err);
  err |= (globals.helpWindow == NULL);
#endif

  // did something go wrong?
  if (err != errNone)
  {
    result = 0;
    ApplicationDisplayDialog(xmemForm);
  }

  // draw the help
  else
  {
    FontID    font;
    WinHandle currWindow;

    currWindow = WinGetDrawWindow();
    font       = FntGetFont();

#if NEW_MEMORY_MGR
    // pre PALMOS_5 - need memory semaphore for writing to storage heap
    if (!DeviceSupportsVersion(romVersion5))
      MemSemaphoreReserve(true);
#endif

    // draw to help window
    WinSetDrawWindow(globals.helpWindow);
    WinSetPattern(&erase);
    WinFillRectangle(&rect,0);

    {
      Char  *ptrStr;
      Coord x, y;

      // initialize
      y   = 2;

      // draw title
      StrCopy(globals.strData, "WHAT IS A LEMMING?");
      x = (rect.extent.x -
           FntCharsWidth(globals.strData, StrLen(globals.strData))) >> 1;

      WinSetUnderlineMode(grayUnderline);
      WinDrawChars(globals.strData,
                   StrLen(globals.strData), x, y); y += FntLineHeight();
      WinSetUnderlineMode(noUnderline);

      // add space (little)
      y += FntLineHeight() >> 1;

      // general text
      x = 4; y += 2;
      WinDrawLine(x, y, rect.extent.x-x, y); y += 2;
      WinDrawLine(x, y, rect.extent.x-x, y); y += 3;

      // general text
      FntSetFont(boldFont);
      StrCopy(globals.strData, "lemming: [lem-ing] (n)");
      x = (rect.extent.x -
           FntCharsWidth(globals.strData, StrLen(globals.strData))) >> 1;
      WinDrawChars(globals.strData,
                   StrLen(globals.strData), x, y); y += FntLineHeight();
      FntSetFont(stdFont);

      x = 4;
      StrCopy(globals.strData,
"adorable yet incredibly stupid furry creature; known for \
walking off cliffs, wandering aimlessly into dangerous situations and \
drowning by the thousands in pools of water.");
      ptrStr = globals.strData;
      while (StrLen(ptrStr) != 0) {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

        x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
        WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;

        ptrStr += count;
      }

      x = 4; y += 3;
      WinDrawLine(x, y, rect.extent.x-x, y); y += 2;
      WinDrawLine(x, y, rect.extent.x-x, y); y += 2;

      // add space (little)
      y += FntLineHeight() >> 1;

      // draw title
      StrCopy(globals.strData, "HOW TO PLAY");
      x = (rect.extent.x -
           FntCharsWidth(globals.strData, StrLen(globals.strData))) >> 1;

      WinSetUnderlineMode(grayUnderline);
      WinDrawChars(globals.strData,
                   StrLen(globals.strData), x, y); y += FntLineHeight();
      WinSetUnderlineMode(noUnderline);

      // add space (little)
      y += FntLineHeight() >> 1;

      // general text
      x = 4;
      StrCopy(globals.strData,
"Lemmings may be mindless, but can be equipped with special skills \
that you assign to some so the remainder can travel safely through \
the dangerous terrain.");
      ptrStr = globals.strData;
      while (StrLen(ptrStr) != 0) {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

        x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
        WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;

        ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      // show the lemmings tasks
      x = 8;
      {
        MemHandle bitmapHandle = DmGetResource('Tbmp', bitmapHelpLemmings);
        WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), x, y);
        MemHandleUnlock(bitmapHandle);
        DmReleaseResource(bitmapHandle);
      }

      // add space (little)
      y += 110 + (FntLineHeight() >> 1);

      // general text
      x = 4;
      StrCopy(globals.strData,
"An experienced player will know precisely where and when to assign \
these skills to the right Lemmings so a safe path towards an exit is \
available for the rest.");
      ptrStr = globals.strData;
      while (StrLen(ptrStr) != 0) {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

        x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
        WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;

        ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      StrCopy(globals.strData, "Need a hint on a level?");
      x = (rect.extent.x -
           FntCharsWidth(globals.strData, StrLen(globals.strData))) >> 1;
      WinDrawChars(globals.strData,
                   StrLen(globals.strData), x, y); y += FntLineHeight(); x = 4;
      StrCopy(globals.strData, "www.ardiri.com");
      FntSetFont(boldFont);
      x = (rect.extent.x -
           FntCharsWidth(globals.strData, StrLen(globals.strData))) >> 1;
      WinDrawChars(globals.strData,
                   StrLen(globals.strData), x, y); y += FntLineHeight(); x = 4;
      FntSetFont(stdFont);
      StrCopy(globals.strData, "Save those Lemmings!");
      x = (rect.extent.x -
           FntCharsWidth(globals.strData, StrLen(globals.strData))) >> 1;
      WinDrawChars(globals.strData,
                   StrLen(globals.strData), x, y); y += FntLineHeight(); x = 4;

      // add space (little)
      y += FntLineHeight() >> 1;

      StrCopy(globals.strData, "GOOD LUCK!");
      FntSetFont(boldFont);
      x = (rect.extent.x -
           FntCharsWidth(globals.strData, StrLen(globals.strData))) >> 1;
      WinDrawChars(globals.strData,
                   StrLen(globals.strData), x, y); y += FntLineHeight();
    }

    FntSetFont(font);
    WinSetDrawWindow(currWindow);

#if NEW_MEMORY_MGR
    // pre PALMOS_5 - release memory semaphore for writing to storage heap
    if (!DeviceSupportsVersion(romVersion5))
      MemSemaphoreRelease(true);
#endif

    result = rect.extent.y - 1;
  }

  return result;
}

/**
 * Draw the instructions on the screen.
 *
 * @param offset the offset height of the window to start copying from.
 */
void
DrawInstructions(UInt16 offset)
{
  const RectangleType helpArea = {{0,offset},{142,116}};

  // blit the required area
  WinCopyRectangle(globals.helpWindow,
                   WinGetDrawWindow(), &helpArea, 3, 16, winPaint);
}

/**
 * Terminate the instructions screen.
 */
void
QuitInstructions()
{
#if SET_KEYMASK
  // return the state of the key processing
  KeySetMask(globals.keyMask);
#endif

  // clean up memory
  if (globals.helpWindow)
    WinDeleteWindow(globals.helpWindow, false);

#if NEW_MEMORY_MGR
  // pre PALMOS_5 - there aint enough heap - hack time
  if (!DeviceSupportsVersion(romVersion5))
  {
    // free the handles used
    if (globals.hanOffScreen) MemHandleUnlock(globals.hanOffScreen);

    // unprotect and zap the memory cache
    globals.dbID = DmFindDatabase(globals.dbCard, cacheHelpFileName);
    if (globals.dbID != NULL)
    {
      DmDatabaseProtect(globals.dbCard, globals.dbID, false);
      DmDeleteDatabase(globals.dbCard, globals.dbID);
    }
  }
#endif
}
