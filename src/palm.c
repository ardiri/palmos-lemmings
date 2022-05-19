/*
 * @(#)palm.c
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

#include "palm.h"

// globals
Int16 SCREEN_WIDTH_STYLUS;
Int16 SCREEN_START_STYLUS;
Int16 SCREEN_HEIGHT_STYLUS;
Int16 SCREEN_WIDTH;
Int16 SCREEN_START;
Int16 SCREEN_TOOL_SPEED_X;
Int16 SCREEN_TOOL_COUNT_X;
Int16 SCREEN_TOOL_START_X;
Int16 SCREEN_TOOL_START_Y;
Int16 SCREEN_TOOL_WIDTH;
Int16 SCREEN_TOOL_HEIGHT;

#if CHEAT_MODE
Char *cheatStr[] = { "lemmings","psygnosis","yippee","ohno","godmode","nukem" };
#endif

// variables
typedef struct
{
  PreferencesType *prefs;
  Int32           evtTimeOut;
  Int16           timerDiff;
  Int16           ticksPerFrame;
  UInt16          ticksPerSecond;

  UInt32          timerLastFrameUpdate;
  UInt32          timerPoint;
#if SHOW_FPS
  Int16           frameCount;
  UInt32          timerReference;
#endif

  Char            strButton[32];

  struct
  {
    Char          hotSyncUsername[dlkUserNameBufSize];
  } system;

  struct
  {
    Char          **strLevelList;
    UInt16        levelCount;      // used for selecting the level

    Char          **strPackList;
    Char          **strPackFileName;
    UInt16        packCount;       // used for selecting the level pack
  } pack;

} Globals;

static Globals globals;
#if NO_DYNAMIC_RAM
static PreferencesType preferences;
#endif

// interface
static void    GlobalsInitialize();
static void    GlobalsTerminate();
static Boolean mainFormEventHandler(EventType *);
static Boolean gameFormEventHandler(EventType *);
static Boolean infoFormEventHandler(EventType *);
static Boolean dvlpFormEventHandler(EventType *);
#ifndef MDM_DISTRIBUTION
static Boolean demoFormEventHandler(EventType *);
#endif
static Boolean cfigFormEventHandler(EventType *);
static Boolean grayFormEventHandler(EventType *);
static Boolean helpFormEventHandler(EventType *);
static Boolean xmemFormEventHandler(EventType *);
static Boolean prepFormEventHandler(EventType *);
static Boolean verdFormEventHandler(EventType *);
static Boolean regiFormEventHandler(EventType *);
static Boolean packFormEventHandler(EventType *);

static Err     notifyCardMounted(SysNotifyParamType *notifyParamsP);
static Err     notifyCardRemoval(SysNotifyParamType *notifyParamsP);
static Err     notifySleepRequest(SysNotifyParamType *notifyParamsP);

/**
 * Initialize the globals (allocate memory etc)
 */
void
GlobalsInitialize()
{
  Int16  i;

  // clear the globals object
  MemSet(&globals, sizeof(Globals), 0);

  // allocate memory for preferences
#if NO_DYNAMIC_RAM
  globals.prefs = &preferences;
#else
  globals.prefs = (PreferencesType *)MemPtrNew(sizeof(PreferencesType));
#endif
  MemSet(globals.prefs, sizeof(PreferencesType), 0);

  // configure the "level list"
  globals.pack.strLevelList =
    (Char **)MemPtrNew(MAX_LEVELS * sizeof(Char *));
  for (i=0; i<MAX_LEVELS; i++)
  {
    globals.pack.strLevelList[i] = (Char *)MemPtrNew(3 * sizeof(Char));
    StrIToA(globals.pack.strLevelList[i], i+1);
  }

  // configure the "pack list"
  globals.pack.strPackList =
    (Char **)MemPtrNew(MAX_PACKS * sizeof(Char *));
  globals.pack.strPackFileName =
    (Char **)MemPtrNew(MAX_PACKS * sizeof(Char *));
  for (i=0; i<MAX_PACKS; i++)
  {
    globals.pack.strPackList[i]     = (Char *)MemPtrNew(40 * sizeof(Char));
    globals.pack.strPackFileName[i] = (Char *)MemPtrNew(40 * sizeof(Char));
    MemSet(globals.pack.strPackList[i], 40, 0);
    MemSet(globals.pack.strPackFileName[i], 40, 0);
  }
}

/**
 * Terminate the globals (free memory etc)
 */
void
GlobalsTerminate()
{
  Int16  i;

  // clean up the pack list
  for (i=0; i<MAX_PACKS; i++)
  {
    MemPtrFree(globals.pack.strPackList[i]);
    MemPtrFree(globals.pack.strPackFileName[i]);
  }
  MemPtrFree(globals.pack.strPackList);
  MemPtrFree(globals.pack.strPackFileName);

  // clean up the level list
  for (i=0; i<MAX_LEVELS; i++)
    MemPtrFree(globals.pack.strLevelList[i]);
  MemPtrFree(globals.pack.strLevelList);

  // free memory for preferences
#if !NO_DYNAMIC_RAM
  MemPtrFree(globals.prefs);
#endif
}

/**
 * The Form:mainForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
mainFormEventHandler(EventType *event)
{
  Boolean     processed = false;
  FormType    *frm;
  EventType   newEvent;
  MemHandle   bitmapHandle, memHandle;
  Char        strTitle[32];
#if PROTECTION_ON
  LocalID     dbID;
  MemHandle   keyH;
  UInt8       strVerification[16];
#endif
  ListType    *lstLevels;
  ControlType *ctlPlayButton, *ctlLevels;

  switch (event->eType)
  {
    case frmOpenEvent:
         frm = FrmGetActiveForm();

         // dynamically adjust the menu :)
         FrmSetMenu(frm,
                    DeviceSupportsGrayscale()
                      ? mainMenu_gray : mainMenu_nogray);
         FrmDrawForm(frm);

         // terminate any music if it is running :)
         GameMusicStop(globals.prefs);

         // post an update event
         MemSet(&newEvent, sizeof(EventType), 0);
         newEvent.eType = appUpdateEvent;
         EvtAddEventToQueue(&newEvent);

         processed = true;
         break;

    case frmUpdateEvent:

         // we ignore these, stupid palmos does funky stuff
         processed = true;
         break;

    case appUpdateEvent:

         frm = FrmGetActiveForm();
         FrmDrawForm(frm);

#if PROTECTION_ON
         // level pack verification (tut tut)
         LevelPackOpen(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack);
         if ((globals.prefs->levelPack.type == PACK_RAM) ||
             (globals.prefs->levelPack.type == PACK_VFS))
         {
           keyH = LevelPackGetResource('_key', 0x0000);
           memHandle = LevelPackGetResource('xxxx', 0x0000);
           MemSet(strVerification, 16, 0);
           MemMove(strVerification, (Char *)MemHandleLock(memHandle), 8);
           MemHandleUnlock(memHandle);
           LevelPackReleaseResource(memHandle);

           RegisterDecryptChunk(strVerification, 8, keyH,
                                globals.prefs->system.hotSyncChecksum);
           LevelPackReleaseResource(keyH);

           // verification [no match!]
           if (StrCompare((Char *)strVerification, "|HaCkMe|") != 0)
           {
             FrmAlert(invalidPackAlert);

             // remove saved game (if any), reset to defaults
             dbID = DmFindDatabase(0, savedGameFileName);
             if (dbID != NULL) DmDeleteDatabase(0, dbID);

             globals.prefs->levelPack.type           = PACK_INTERNAL;
             MemSet(globals.prefs->levelPack.strLevelPack, 32, 0);
             globals.prefs->levelPack.useDifficult   = false;
           }
         }
#ifdef MDM_DISTRIBUTION
         else
         if (globals.prefs->levelPack.type == PACK_VFS_MDM)
         {
           UInt8  i, checksum, MDM_key[dlkUserNameBufSize] = { };
           UInt32 MDM_key_length = 0;

           keyH = LevelPackGetResource('_key', 0x0000);
           memHandle = LevelPackGetResource('xxxx', 0x0000);
           MemSet(strVerification, 16, 0);
           MemMove(strVerification, (Char *)MemHandleLock(memHandle), 8);
           MemHandleUnlock(memHandle);
           LevelPackReleaseResource(memHandle);

           // SPECIAL: hotsync username = 4D:44:4D:2D:63:61:72:64:7D "MDM-card"
           MDM_GetKey(MDM_key, &MDM_key_length);
           MemSet(&MDM_key[MDM_key_length], dlkUserNameBufSize - MDM_key_length, 0);

           checksum = 0;
           for (i=0; i<MAX_IDLENGTH; i++)
             checksum += (UInt8)MDM_key[i];
           checksum &= 0xff;
           if (checksum == 0) checksum = 0x20; // cannot be zero

           RegisterDecryptChunk(strVerification, 8, keyH, checksum);
           LevelPackReleaseResource(keyH);

           // verification [no match!]
           if (StrCompare((Char *)strVerification, "|HaCkMe|") != 0)
           {
             FrmAlert(invalidPackAlert);

             // remove saved game (if any), reset to defaults
             dbID = DmFindDatabase(0, savedGameFileName);
             if (dbID != NULL) DmDeleteDatabase(0, dbID);

             globals.prefs->levelPack.type           = PACK_INTERNAL;
             MemSet(globals.prefs->levelPack.strLevelPack, 32, 0);
             globals.prefs->levelPack.useDifficult   = false;
           }
         }
#endif
         LevelPackClose();
#endif

         // level pack loading..
         LevelPackOpen(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack);

         // has a custom "bitmap" been defined?
         bitmapHandle = LevelPackGetResource('logo', 0);
         if (bitmapHandle != NULL)
         {
           WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), 0, 16);
           MemHandleUnlock(bitmapHandle);
           LevelPackReleaseResource(bitmapHandle);
         }
         else

         // just get the name of the level pack (we'll draw later)
         {
           memHandle = LevelPackGetResource('titl', 0);
           StrCopy(strTitle, (Char *)MemHandleLock(memHandle));
           MemHandleUnlock(memHandle);
           LevelPackReleaseResource(memHandle);
         }

         memHandle = LevelPackGetResource(levlCount, 0);
         globals.pack.levelCount = StrAToI((Char *)MemHandleLock(memHandle));
         MemHandleUnlock(memHandle);
         LevelPackReleaseResource(memHandle);

         // close the database
         LevelPackClose();

         // draw the stuff to screen
         if (bitmapHandle == NULL)
         {
           bitmapHandle = DmGetResource('Tbmp', bitmapLogo);
           WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), 0, 16);
           MemHandleUnlock(bitmapHandle);
           DmReleaseResource(bitmapHandle);

           FntSetFont(largeBoldFont);
           WinEraseChars(strTitle, StrLen(strTitle), 32, 32);
           WinInvertChars(strTitle, StrLen(strTitle), 32, 32);
           FntSetFont(stdFont);
         }

         // configure the list
         lstLevels =
           (ListType *)FrmGetObjectPtr(frm,
              FrmGetObjectIndex(frm, mainFormLevelList));
         ctlLevels =
           (ControlType *)FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, mainFormLevelTrigger));
         LstSetListChoices(lstLevels,
                           globals.pack.strLevelList, globals.pack.levelCount);
         LstSetSelection(lstLevels, 0);                 // show first item (1)
         LstSetHeight(lstLevels, globals.pack.levelCount);
         CtlSetLabel(ctlLevels,
           LstGetSelectionText(lstLevels, LstGetSelection(lstLevels)));

         // redraw the play button
         ctlPlayButton =
           (ControlType *)FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, mainFormPlayButton));
         CtlDrawControl(ctlPlayButton);

         // draw seperators
         WinDrawLine(   0, 145, 159, 145);
         WinDrawLine(   0, 146, 159, 146);

         RegisterShowMessage(globals.prefs);
         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case mainFormPlayButton:

                // regenerate a menu event
                MemSet(event, sizeof(EventType), 0);
                event->eType            = menuEvent;
                event->data.menu.itemID = mainMenuItemPlay;
                EvtAddEventToQueue(event);

                SndPlaySystemSound(sndClick);
                processed = true;
                break;

           case mainFormLevelButton:

                // regenerate a menu event
                MemSet(event, sizeof(EventType), 0);
                event->eType            = menuEvent;
                event->data.menu.itemID = mainMenuItemLevelPacks;
                EvtAddEventToQueue(event);

                SndPlaySystemSound(sndClick);
                processed = true;
                break;

           default:
                break;
         }
         break;

    case menuEvent:

         // what menu?
         switch (event->data.menu.itemID)
         {
           case mainMenuItemPlay:

                frm = FrmGetActiveForm();

                // what level to jump to?
                lstLevels =
                  (ListType *)FrmGetObjectPtr(frm,
                     FrmGetObjectIndex(frm, mainFormLevelList));

                globals.prefs->game.gameLevel = LstGetSelection(lstLevels) + 1;

                LevelPackOpen(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack);
                GameResetPreferences(globals.prefs);
                LevelPackClose();

                FrmGotoForm(gameForm);

                processed = true;
                break;

           case mainMenuItemLevelPacks:

                ApplicationDisplayDialog(packForm);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:gameForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
gameFormEventHandler(EventType *event)
{
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
        RectangleType     rect  = {{0,0},{160,160}};

  Boolean   processed = false;
  MemHandle memHandle;
#if CHEAT_MODE
  Char      ch;
  UInt16    i, index;
#endif
  FormType *frm;
  EventType newEvent;
  UInt32    timeStamp;

  switch (event->eType)
  {
    case frmOpenEvent:
         frm = FrmGetActiveForm();

#if HANDERA_NATIVE
         // force handera into 160 -> 240 stretch and 1to1 for bitmaps
         if (globals.prefs->handera.device)
         {
           VgaFormModify(frm, vgaFormModify160To240);
           VgaSetScreenMode(screenMode1To1, rotateModeNone);

           rect.extent.x = 240;
           rect.extent.y = 240;
         }
#endif

         // clear the LCD screen (dont want palette flash)
         WinSetDrawWindow(WinGetDisplayWindow());
         WinSetPattern(&erase);
         WinFillRectangle(&rect, 0);

         // dynamically adjust the menu :)
         FrmSetMenu(frm,
                    DeviceSupportsGrayscale()
                      ? gameMenu_gray : gameMenu_nogray);

         // load the level
         LevelPackOpen(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack);
         GameLoadLevel(globals.prefs);
         LevelPackClose();

#if CHEAT_MODE
         // reset the search :)
         globals.prefs->game.cheat.dataOffset = 0;
         MemSet(globals.prefs->game.cheat.dataEntry,MAX_ENTRY,0);
#endif

#if SHOW_FPS
         globals.frameCount     = 0;
         globals.timerReference = -1;
#endif

         globals.prefs->game.music.playing = true;

         // post an update event
         if (globals.prefs->game.gameState == GAME_PLAY)
         {
           MemSet(&newEvent, sizeof(EventType), 0);
           newEvent.eType = appUpdateEvent;
           EvtAddEventToQueue(&newEvent);
         }

         processed = true;
         break;

    case frmUpdateEvent:

         // we ignore these, stupid palmos does funky stuff
         processed = true;
         break;

    case appUpdateEvent:
         FrmDrawForm(FrmGetActiveForm());

         // draw seperators
#if HANDERA_NATIVE
         if (globals.prefs->handera.device)
         {
           WinDrawLine(   0, 218, 239, 218);
           WinDrawLine(   0, 219, 239, 219);

           rect.topLeft.x = 0;
           rect.topLeft.y = 24;
           rect.extent.x  = 240;
           rect.extent.y  = 192;

           WinSetPattern(&erase);
           if (DeviceSupportsColor())
             WinDrawRectangle(&rect, 0);
           else
             WinFillRectangle(&rect, 0);

           if (globals.prefs->config.widescreenDisplay)
           {
             Char  str[32];
             Coord x, y;

             FntSetFont(VgaBaseToVgaFont(stdFont));
             if (globals.prefs->levelPack.type == PACK_INTERNAL)
               SysCopyStringResource(str, stringDemo);
             else
               SysCopyStringResource(str, stringRegisteredPack); // special = level pack
             y = 32;
             x = (SCREEN_WIDTH_HANDERA - FntCharsWidth(str, StrLen(str))) >> 1;
             WinInvertChars(str, StrLen(str), x, y);

             SysCopyStringResource(str, stringCopyrightNotice);
             y = 194;
             x = (SCREEN_WIDTH_HANDERA - FntCharsWidth(str, StrLen(str))) >> 1;
             WinInvertChars(str, StrLen(str), x, y);
             FntSetFont(stdFont);
           }
         }
         else
#endif
#if SONY_NATIVE
         if (globals.prefs->sony.device)
         {
           WinDrawLine(   0, 145, 159, 145);
           WinDrawLine(   0, 146, 159, 146);

           rect.topLeft.x = 0;
           rect.topLeft.y = SCREEN_START_GENERIC;
           rect.extent.x  = SCREEN_WIDTH_GENERIC;
           rect.extent.y  = SCREEN_HEIGHT;

           WinSetPattern(&erase);
           if (DeviceSupportsColor())
             WinDrawRectangle(&rect, 0);
           else
             WinFillRectangle(&rect, 0);

           if (globals.prefs->config.widescreenDisplay)
           {
             Char  str[32];
             Coord x, y;

             if (globals.prefs->levelPack.type == PACK_INTERNAL)
               SysCopyStringResource(str, stringDemo);
             else
               SysCopyStringResource(str, stringRegisteredPack); // special = level pack
             y = 26;
             x = (SCREEN_WIDTH_GENERIC - FntCharsWidth(str, StrLen(str))) >> 1;
             WinInvertChars(str, StrLen(str), x, y);

             SysCopyStringResource(str, stringCopyrightNotice);
             y = 122;
             x = (SCREEN_WIDTH_GENERIC - FntCharsWidth(str, StrLen(str))) >> 1;
             WinInvertChars(str, StrLen(str), x, y);
           }
         }
         else
#endif
#if PALM_HIDENSITY
         if (globals.prefs->palmHD.device)
         {
           WinDrawLine(   0, 145, 159, 145);
           WinDrawLine(   0, 146, 159, 146);

           rect.topLeft.x = 0;
           rect.topLeft.y = SCREEN_START_GENERIC;
           rect.extent.x  = SCREEN_WIDTH_GENERIC;
           rect.extent.y  = SCREEN_HEIGHT;

           WinSetPattern(&erase);
           if (DeviceSupportsColor())
             WinDrawRectangle(&rect, 0);
           else
             WinFillRectangle(&rect, 0);

           if (globals.prefs->config.widescreenDisplay)
           {
             Char  str[32];
             Coord x, y;

             if (globals.prefs->levelPack.type == PACK_INTERNAL)
               SysCopyStringResource(str, stringDemo);
             else
               SysCopyStringResource(str, stringRegisteredPack); // special = level pack
             y = 26;
             x = (SCREEN_WIDTH_GENERIC - FntCharsWidth(str, StrLen(str))) >> 1;
             WinInvertChars(str, StrLen(str), x, y);

             SysCopyStringResource(str, stringCopyrightNotice);
             y = 122;
             x = (SCREEN_WIDTH_GENERIC - FntCharsWidth(str, StrLen(str))) >> 1;
             WinInvertChars(str, StrLen(str), x, y);
           }
         }
         else
#endif
         {
           WinDrawLine(   0, 145, 159, 145);
           WinDrawLine(   0, 146, 159, 146);
         }

         // draw the active tool
         GameChangeTool(globals.prefs, globals.prefs->game.activeTool,
           globals.prefs->game.tools[globals.prefs->game.activeTool]);

         // make sure we are viewing the right area
         GraphicsSetOffset(globals.prefs->game.cursor.screenOffset);
#if !FULL_SCREEN_BLIT
         GraphicsSetUpdate(0, SCREEN_HEIGHT-1);
#endif
         GraphicsRepaint();

         // force a repaint
         globals.timerLastFrameUpdate = TimGetTicks() - globals.ticksPerFrame;

         processed = true;
         break;

    case penDownEvent:
    case penMoveEvent:
         {
           Coord x, y;

           x = event->screenX;
           y = event->screenY;

           // within the graffiti area?
           if (
               (globals.prefs->config.graffitiScroll)          &&
               (globals.prefs->game.gamePlaying)               &&
#if HANDERA_NATIVE
               ((
                (globals.prefs->handera.device)                &&
                (x >  38) && (x < 202) && (y > 340) && (y < 388)
               ) ||
               (
                (!globals.prefs->handera.device)               &&
                (x >  32) && (x < 128) && (y > 170) && (y < 218)
               ))
#else
               (x >  32) && (x < 128) && (y > 170) && (y < 218)
#endif
              )
           {
             Int16 pos;

#if HANDERA_NATIVE
             if (globals.prefs->handera.device)
             {
               x   = x - 39;
//             pos = (((UInt32)OFFSCREEN_WIDTH * x) / 164) - (SCREEN_WIDTH>>1);
               pos = ((160 * x) / 41) - 120;                    // optimized :P
             }
#endif
             {
               x   = x - 32;
//             pos = (((UInt32)OFFSCREEN_WIDTH * x) / 96) - (SCREEN_WIDTH >> 1);
               pos = ((20 * x) / 3) - 80;                       // optimized :P
             }
             pos = pos & ~0x07; // bind to 8 pixel boundary

             // make sure we dont go off scale here
             if (pos < 0) pos = 0; else
             if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
               pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);

             // jump to location
             globals.prefs->game.cursor.screenOffset = pos;
             GraphicsSetOffset(globals.prefs->game.cursor.screenOffset);

             // we have handled this event, lets continue
             processed = true;
           }

#if HANDERA_NATIVE
           if ((globals.prefs->handera.device) &&
               (!globals.prefs->config.widescreenDisplay))
           {
             x = (x << 1) / 3;
             y = (y << 1) / 3;  // adjust back to 160x160 resolution
           }
#endif

           // within the game play area?
           if (
               (globals.prefs->game.gamePlaying) &&
               (x > 0) && (x < SCREEN_WIDTH_STYLUS) &&
               (y > SCREEN_START_STYLUS) &&
               (y < (SCREEN_START_STYLUS + SCREEN_HEIGHT_STYLUS))
              )
           {
             // if the player has tapped this area, they wants to move
             GameProcessStylusInput(globals.prefs,
                                    x, y, (event->eType == penMoveEvent));
           }

           // speed control?
           else
           if (
               (globals.prefs->game.gamePlaying) &&
               (event->screenX > SCREEN_TOOL_SPEED_X) &&
               (event->screenX < (SCREEN_TOOL_SPEED_X+SCREEN_TOOL_WIDTH)) &&
               (event->screenY > SCREEN_TOOL_START_Y) &&
               (event->screenY < (SCREEN_TOOL_START_Y+SCREEN_TOOL_HEIGHT))
              )
           {
             UInt16 x, y;

             x = (event->screenX - SCREEN_TOOL_SPEED_X);
             y = (event->screenY - SCREEN_TOOL_START_Y);

             // perform the adjustment :)
             GameAdjustLemmingRate(globals.prefs, (x + y) < SCREEN_TOOL_WIDTH);
           }

           // within the tool bar area? [use stylus values here]
           else
           if (
               (globals.prefs->game.gamePlaying) &&
               (event->screenX > SCREEN_TOOL_START_X) &&
               (event->screenX < SCREEN_WIDTH_STYLUS) &&
               (event->screenY > SCREEN_TOOL_START_Y) &&
               (event->screenY < (SCREEN_TOOL_START_Y+SCREEN_TOOL_HEIGHT))
              )
           {
             UInt16 newTool =
               (event->screenX - SCREEN_TOOL_START_X) / SCREEN_TOOL_WIDTH;

             // set the new tool
             if (newTool >= TOOL_COUNT) newTool = TOOL_COUNT-1;
             GameChangeTool(globals.prefs, newTool,
               globals.prefs->game.tools[globals.prefs->game.activeTool]);
           }
         }
         break;

    case keyDownEvent:

         // ignore hard keys [sometimes they get through]
         if ((event->data.keyDown.modifiers & commandKeyMask) &&
             (TxtCharIsHardKey(event->data.keyDown.modifiers,
                               event->data.keyDown.chr)))
         {
           switch (event->data.keyDown.chr)
           {
             case vchrHard1:
             case vchrHard2:
             case vchrHard3:
             case vchrHard4:
             case vchrPageUp:
             case vchrPageDown:

                  processed = true;
                  break;

             default:
                  break;
           }
         }
         if (processed) goto KEYDOWN_ABORT;

         // palmos 5.0+ [5-way dpad]
         if (EvtKeydownIsVirtual(event) && IsFiveWayNavEvent(event))
         {
           processed = true;
         }
         if (processed) goto KEYDOWN_ABORT;

         switch (event->data.keyDown.chr)
         {
           case 'z':                          // treo90: special keys
           case vchrJogUp:                    // jog dial up: sony
           case vchrPrevField:                // jog dial up: handera
//         case vchrTrgJogUp:                 // jog dial up: handera
                {
                  Int16 pos;

                  pos  = globals.prefs->game.cursor.screenOffset;
                  pos -= SCROLL_OFFSET;
                  pos  = pos & ~0x07; // bind to 8 pixel boundary

                  // make sure we dont go off scale here
                  if (pos < 0) pos = 0; else
                  if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
                    pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);

                  // jump to location
                  globals.prefs->game.cursor.screenOffset = pos;
                  GraphicsSetOffset(globals.prefs->game.cursor.screenOffset);

                  // we have handled this event, lets continue
                  processed = true;
                }
                break;

           case '.':                          // treo90: special keys
           case vchrJogDown:                  // jog dial down: sony
           case vchrNextField:                // jog dial down: handera
//         case vchrTrgJogDown:               // jog dial down: handera
                {
                  Int16 pos;

                  pos  = globals.prefs->game.cursor.screenOffset;
                  pos += SCROLL_OFFSET;
                  pos  = pos & ~0x07; // bind to 8 pixel boundary

                  // make sure we dont go off scale here
                  if (pos < 0) pos = 0; else
                  if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
                    pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);

                  // jump to location
                  globals.prefs->game.cursor.screenOffset = pos;
                  GraphicsSetOffset(globals.prefs->game.cursor.screenOffset);

                  // we have handled this event, lets continue
                  processed = true;
                }
                break;

           case vchrRockerUp:
           case vchrRockerDown:
           case vchrRockerLeft:
           case vchrRockerRight:
           case vchrRockerCenter:
                processed = true;
                break;

           default:

#if CHEAT_MODE
                ch = event->data.keyDown.chr;
                if (((ch >= 'A') && (ch <= 'Z')) ||
                    ((ch >= 'a') && (ch <= 'z')))
                {
                  ch = ch | 0x20; // turn on the shift

                  index = globals.prefs->game.cheat.dataOffset;
                  globals.prefs->game.cheat.dataEntry[index] = ch;
                  index = ++globals.prefs->game.cheat.dataOffset;

                  for (i=0; i<CHEAT_COUNT; i++)
                  {
                    // got a partial match?
                    if (StrNCompare(globals.prefs->game.cheat.dataEntry,
                                    cheatStr[i], index) == 0)
                    {
                      // a complete match?
                      if (StrCompare(globals.prefs->game.cheat.dataEntry,
                                     cheatStr[i]) == 0)
                      {
                        // apply the cheat :)
                        GameCheat(globals.prefs, i);

                        // reset the search :)
                        globals.prefs->game.cheat.dataOffset = 0;
                        MemSet(globals.prefs->game.cheat.dataEntry,MAX_ENTRY,0);
                      }

                      goto FOUND_MATCH;
                    }
                  }

                  // no match, try again buddy :)
                  MemSet(globals.prefs->game.cheat.dataEntry, MAX_ENTRY, 0);
                  globals.prefs->game.cheat.dataOffset = 1;
                  globals.prefs->game.cheat.dataEntry[0] = ch;

FOUND_MATCH:

                  processed = true;
                }

#endif

                break;
         }

KEYDOWN_ABORT:

         break;

    case menuEvent:

         // what menu?
         switch (event->data.menu.itemID)
         {
           case gameMenuItemRestart:

                // restart?
#if MIDI_PAUSE_ON_DIALOG
                GameMusicPause(globals.prefs, true);
#endif
                if (FrmAlert(restartLevelAlert) == 0)
                {
                  LevelPackOpen(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack);
                  GameResetPreferences(globals.prefs);
                  GameLoadLevel(globals.prefs);
                  LevelPackClose();
                }
#if MIDI_PAUSE_ON_DIALOG
                GameMusicPause(globals.prefs, globals.prefs->game.gamePaused);
#endif
                processed = true;
                break;

           case gameMenuItemSkip:

                // skip level?
#if MIDI_PAUSE_ON_DIALOG
                GameMusicPause(globals.prefs, true);
#endif
                if (FrmAlert(skipLevelAlert) == 0)
                {
                  // completed all the levels?
                  if (globals.prefs->game.gameLevel == GameGetLevelCount())
                  {
                    globals.prefs->game.gamePlaying = false;

                    // return to the main screen
                    MemSet(&newEvent, sizeof(EventType), 0);
                    newEvent.eType            = menuEvent;
                    newEvent.data.menu.itemID = gameMenuItemExit;
                    EvtAddEventToQueue(&newEvent);
                  }
                  else
                  {
                    globals.prefs->game.gameLevel =
                      (globals.prefs->game.gameLevel % GameGetLevelCount())+1;

                    LevelPackOpen(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack);
                    GameResetPreferences(globals.prefs);
                    GameLoadLevel(globals.prefs);
                    LevelPackClose();
                  }
                }
#if MIDI_PAUSE_ON_DIALOG
                GameMusicPause(globals.prefs, globals.prefs->game.gamePaused);
#endif
                processed = true;
                break;

           case gameMenuItemLevelPacks:

#if MIDI_PAUSE_ON_DIALOG
                GameMusicPause(globals.prefs, true);
#endif
                FrmAlert(exitGameAlert);
#if MIDI_PAUSE_ON_DIALOG
                GameMusicPause(globals.prefs, globals.prefs->game.gamePaused);
#endif
                processed = true;
                break;

           case gameMenuItemPause:

                // invert the game pause state
                GamePause(globals.prefs, !globals.prefs->game.gamePaused);
                GamePlaySound(globals.prefs, snd_selection);

                processed = true;
                break;

           case gameMenuItemExit:

#if MIDI_PAUSE_ON_DIALOG
                GameMusicPause(globals.prefs, true);
#endif
                if ((!globals.prefs->game.gamePlaying) ||
                    (FrmAlert(quitGameAlert) == 0))
                {
                  globals.prefs->game.gamePaused  = true;
                  globals.prefs->game.gamePlaying = false;
                  FrmGotoForm(mainForm);
                }
#if MIDI_PAUSE_ON_DIALOG
                GameMusicPause(globals.prefs, globals.prefs->game.gamePaused);
#endif
                processed = true;
                break;

           default:
                break;
         }
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case gameFormSoundButton:

                // regenerate menu event
                MemSet(event, sizeof(EventType), 0);
                event->eType            = menuEvent;
                event->data.menu.itemID = menuItemConfig;
                EvtAddEventToQueue(event);

                processed = true;
                break;

           case gameFormPauseButton:

                // regenerate menu event
                MemSet(event, sizeof(EventType), 0);
                event->eType            = menuEvent;
                event->data.menu.itemID = gameMenuItemPause;
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    case nilEvent:

         // display the prepForm
         if (globals.prefs->game.gameState == GAME_START)
           ApplicationDisplayDialog(prepForm);
         else

         // display the verdForm
         if (globals.prefs->game.gameState == GAME_END)
           ApplicationDisplayDialog(verdForm);
         else

         // make sure the active window is the form
         if (WinGetActiveWindow() == (WinHandle)FrmGetActiveForm())
         {
           timeStamp = TimGetTicks();

           // update screen (animation) if possible
           if ((timeStamp - globals.timerLastFrameUpdate) >= globals.ticksPerFrame)
           {
             UInt32 keyState;

             // animation requirement
             globals.timerLastFrameUpdate = timeStamp;

             keyState = KeyCurrentState();

             if (keyState & keyBitNavLeft)      keyState |= globals.prefs->config.ctlKeyLeft;
             if (keyState & keyBitNavRight)     keyState |= globals.prefs->config.ctlKeyRight;
             if (keyState & keyBitNavSelect)    keyState |= globals.prefs->config.ctlKeySelect;
             if (keyState & keyBitRockerLeft)   keyState |= globals.prefs->config.ctlKeyLeft;
             if (keyState & keyBitRockerRight)  keyState |= globals.prefs->config.ctlKeyRight;
             if (keyState & keyBitRockerSelect) keyState |= globals.prefs->config.ctlKeySelect;

             // play the game!
             GameProcessKeyInput(globals.prefs, keyState);
             GameMovement(globals.prefs);

             // draw the game
             GameDraw(globals.prefs);

             // is the pen being held down? if so, lets post event
             {
               Coord   x, y;
               Boolean penDown;

               EvtGetPen(&x, &y, &penDown);

               if (penDown)
               {
                 EventType event;
                 MemSet(&event, sizeof(EventType), 0);
                 event.eType   = penMoveEvent;
                 event.penDown = true;
                 event.screenX = x;
                 event.screenY = y;
                 EvtAddEventToQueue(&event);
               }
             }
           }
         }
         processed = true;
         break;

    case frmCloseEvent:

         // save the level if it is possible
         GamePause(globals.prefs, true);
         GameSaveLevel(globals.prefs);

         if (DeviceSupportsColor())
         {
           // clear the LCD screen (dont want palette flash)
           WinSetPattern(&erase);
           WinFillRectangle(&rect, 0);

           memHandle = DmGetResource('PALT', paletteVGA16);
           WinPalette(winPaletteSet, 0, 16,
                      (RGBColorType *)MemHandleLock(memHandle));
           MemHandleUnlock(memHandle);
           DmReleaseResource(memHandle);
         }

         globals.prefs->game.music.playing = false;

#if HANDERA_NATIVE
         // reset handera display configuration
         if (globals.prefs->handera.device)
           VgaSetScreenMode(screenModeScaleToFit, rotateModeNone);
#endif

         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:cfigForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
cfigFormEventHandler(EventType *event)
{
  Boolean processed = false;

  switch (event->eType)
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         {
           FormType    *frm;
           UInt16      index;
           ControlType *muteCheckbox, *volumeControl;
           ControlType *musicCheckbox, *scrollCheckbox, *scaleCheckbox;
           ListType    *lstHard1, *lstHard2, *lstHard3, *lstHard4;
           ListType    *lstUp, *lstDown;
           ControlType *ctlHard1, *ctlHard2, *ctlHard3, *ctlHard4;
           ControlType *ctlUp, *ctlDown;
           UInt16      *choices[6] = { };

           choices[0] = &(globals.prefs->config.ctlKeyLeft);
           choices[1] = &(globals.prefs->config.ctlKeyRight);
           choices[2] = &(globals.prefs->config.ctlKeyUp);
           choices[3] = &(globals.prefs->config.ctlKeyDown);
           choices[4] = &(globals.prefs->config.ctlKeySelect);
           choices[5] = &(globals.prefs->config.ctlKeyTool);

           frm = FrmGetActiveForm();
           ctlHard1 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey1Trigger));
           ctlHard2 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey2Trigger));
           ctlHard3 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey3Trigger));
           ctlHard4 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey4Trigger));
           ctlUp =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormPageUpTrigger));
           ctlDown =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormPageDownTrigger));

           lstHard1 =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey1List));
           lstHard2 =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey2List));
           lstHard3 =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey3List));
           lstHard4 =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey4List));
           lstUp =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormPageUpList));
           lstDown =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormPageDownList));

           LstSetSelection(lstHard1, 0);
           CtlSetLabel(ctlHard1, LstGetSelectionText(lstHard1, 0));
           LstSetSelection(lstHard2, 0);
           CtlSetLabel(ctlHard2, LstGetSelectionText(lstHard2, 0));
           LstSetSelection(lstHard3, 0);
           CtlSetLabel(ctlHard3, LstGetSelectionText(lstHard3, 0));
           LstSetSelection(lstHard4, 0);
           CtlSetLabel(ctlHard4, LstGetSelectionText(lstHard4, 0));
           LstSetSelection(lstDown, 0);
           CtlSetLabel(ctlDown, LstGetSelectionText(lstDown, 0));
           LstSetSelection(lstUp, 0);
           CtlSetLabel(ctlUp, LstGetSelectionText(lstUp, 0));

           // show the "current" settings
           for (index=0; index<6; index++)
           {
             if ((*(choices[index]) & keyBitHard1) != 0)
             {
               LstSetSelection(lstHard1, index+1);
               CtlSetLabel(ctlHard1, LstGetSelectionText(lstHard1, index+1));
             }

             if ((*(choices[index]) & keyBitHard2) != 0)
             {
               LstSetSelection(lstHard2, index+1);
               CtlSetLabel(ctlHard2, LstGetSelectionText(lstHard2, index+1));
             }

             if ((*(choices[index]) & keyBitHard3) != 0)
             {
               LstSetSelection(lstHard3, index+1);
               CtlSetLabel(ctlHard3, LstGetSelectionText(lstHard3, index+1));
             }

             if ((*(choices[index]) & keyBitHard4) != 0)
             {
               LstSetSelection(lstHard4, index+1);
               CtlSetLabel(ctlHard4, LstGetSelectionText(lstHard4, index+1));
             }

             if ((*(choices[index]) & keyBitPageUp) != 0)
             {
               LstSetSelection(lstUp, index+1);
               CtlSetLabel(ctlUp, LstGetSelectionText(lstUp, index+1));
             }

             if ((*(choices[index]) & keyBitPageDown) != 0)
             {
               LstSetSelection(lstDown, index+1);
               CtlSetLabel(ctlDown, LstGetSelectionText(lstDown, index+1));
             }
           }

           // configure sounds settings
           musicCheckbox =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormMusicCheckbox));
           muteCheckbox =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormMuteCheckbox));
           volumeControl =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormSound0Button+DeviceGetVolume()));

           CtlSetValue(musicCheckbox,
             (globals.prefs->config.musicEnabled) ? 1 : 0);
           CtlSetValue(muteCheckbox, DeviceGetMute() ? 1 : 0);
           CtlSetValue(volumeControl, 1);

           // other settings
           scrollCheckbox =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormScrollCheckbox));
           CtlSetValue(scrollCheckbox,
             (globals.prefs->config.graffitiScroll) ? 1 : 0);
           scaleCheckbox =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormScaleCheckbox));
           CtlSetValue(scaleCheckbox,
             (globals.prefs->config.widescreenDisplay) ? 1 : 0);

           // save this
           FtrSet(appCreator, ftrGlobalsCfgActiveVol, (UInt32)volumeControl);
         }
         processed = true;
         break;

    case ctlEnterEvent:

         switch (event->data.ctlEnter.controlID)
         {
           case cfigFormSound0Button:
           case cfigFormSound1Button:
           case cfigFormSound2Button:
           case cfigFormSound3Button:
           case cfigFormSound4Button:
           case cfigFormSound5Button:
           case cfigFormSound6Button:
           case cfigFormSound7Button:
                {
                  ControlType *newCtl, *oldCtl;

                  newCtl = event->data.ctlEnter.pControl;

                  // we dont want an audible beep from the system
                  FtrGet(appCreator, ftrGlobalsCfgActiveVol, (UInt32 *)&oldCtl);
                  CtlSetValue(oldCtl, 0);
                  CtlSetValue(newCtl, 1);
                  FtrSet(appCreator, ftrGlobalsCfgActiveVol, (UInt32)newCtl);

                  // act as we needed :)
                  DeviceSetVolume(
                     (event->data.ctlEnter.controlID - cfigFormSound0Button));
                  {
                    SndCommandType testBeep =
                      {sndCmdFreqDurationAmp,0,512,32,sndMaxAmp};
                    DevicePlaySound(&testBeep);
                  }
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case cfigFormMuteCheckbox:
                DeviceSetMute(CtlGetValue(event->data.ctlEnter.pControl) != 0);
                processed = true;
                break;

           case cfigFormOkButton:
                {
                  ControlType *musicCheckbox, *scrollCheckbox, *scaleCheckbox;
                  ListType    *lstHard1, *lstHard2, *lstHard3, *lstHard4;
                  ListType    *lstUp, *lstDown;
                  UInt16      index;
                  FormType    *frm;
                  UInt16      keySignature;
                  UInt16      *choices[6] = { };

                  choices[0] = &(globals.prefs->config.ctlKeyLeft);
                  choices[1] = &(globals.prefs->config.ctlKeyRight);
                  choices[2] = &(globals.prefs->config.ctlKeyUp);
                  choices[3] = &(globals.prefs->config.ctlKeyDown);
                  choices[4] = &(globals.prefs->config.ctlKeySelect);
                  choices[5] = &(globals.prefs->config.ctlKeyTool);

                  frm = FrmGetActiveForm();
                  lstHard1 =
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormHardKey1List));
                  lstHard2 =
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormHardKey2List));
                  lstHard3 =
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormHardKey3List));
                  lstHard4 =
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormHardKey4List));
                  lstUp =
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormPageUpList));
                  lstDown =
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormPageDownList));

                  keySignature =
                    (
                      (0x01 << LstGetSelection(lstHard1)) |
                      (0x01 << LstGetSelection(lstHard2)) |
                      (0x01 << LstGetSelection(lstHard3)) |
                      (0x01 << LstGetSelection(lstHard4)) |
                      (0x01 << LstGetSelection(lstUp))    |
                      (0x01 << LstGetSelection(lstDown))
                    );
                  keySignature = keySignature >> 1;  // ignore the '------'

                  // only process if good setting is selected.
                  if (keySignature == 0x3F)
                  {
                    Boolean widescreenMode;
#if HANDERA_NATIVE
                    Int16   pos;
#endif

                    // sounds preferences
                    globals.prefs->config.sndMute   = DeviceGetMute();
                    globals.prefs->config.sndVolume = DeviceGetVolume();

                    // key preferences
                    for (index=0; index<6; index++)
                    {
                      *(choices[index]) = 0;
                    }
                    if (LstGetSelection(lstHard1) != 0)
                      *(choices[LstGetSelection(lstHard1)-1]) |= keyBitHard1;
                    if (LstGetSelection(lstHard2) != 0)
                      *(choices[LstGetSelection(lstHard2)-1]) |= keyBitHard2;
                    if (LstGetSelection(lstHard3) != 0)
                      *(choices[LstGetSelection(lstHard3)-1]) |= keyBitHard3;
                    if (LstGetSelection(lstHard4) != 0)
                      *(choices[LstGetSelection(lstHard4)-1]) |= keyBitHard4;
                    if (LstGetSelection(lstUp) != 0)
                      *(choices[LstGetSelection(lstUp)-1])    |= keyBitPageUp;
                    if (LstGetSelection(lstDown) != 0)
                      *(choices[LstGetSelection(lstDown)-1])  |= keyBitPageDown;

                    musicCheckbox =
                      (ControlType *)FrmGetObjectPtr(frm,
                         FrmGetObjectIndex(frm, cfigFormMusicCheckbox));
                    globals.prefs->config.musicEnabled =
                      (CtlGetValue(musicCheckbox) == 1);

                    scrollCheckbox =
                      (ControlType *)FrmGetObjectPtr(frm,
                         FrmGetObjectIndex(frm, cfigFormScrollCheckbox));
                    globals.prefs->config.graffitiScroll =
                      (CtlGetValue(scrollCheckbox) == 1);
                    scaleCheckbox =
                      (ControlType *)FrmGetObjectPtr(frm,
                         FrmGetObjectIndex(frm, cfigFormScaleCheckbox));
                    widescreenMode = globals.prefs->config.widescreenDisplay;
                    globals.prefs->config.widescreenDisplay =
                      (CtlGetValue(scaleCheckbox) == 1);

                    // lets adjust the graphics display
                    if (widescreenMode !=
                        globals.prefs->config.widescreenDisplay)
                    {
#if HANDERA_NATIVE
                      if (globals.prefs->handera.device)
                      {
                        GameWideScreen(globals.prefs);

                        if (widescreenMode)
                        {
                          globals.prefs->game.cursor.x =
                            (globals.prefs->game.cursor.x << 1) / 3;

                          pos = globals.prefs->game.cursor.screenOffset + 40;
                          if (pos < 0) pos = 0; else
                          if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
                            pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);
                          globals.prefs->game.cursor.screenOffset = pos;
                        }
                        else
                        {
                          globals.prefs->game.cursor.x =
                            (globals.prefs->game.cursor.x * 3) >> 1;

                          pos = globals.prefs->game.cursor.screenOffset - 40;
                          if (pos < 0) pos = 0; else
                          if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
                            pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);
                          globals.prefs->game.cursor.screenOffset = pos;
                        }

                      }
                      else
#endif
#if SONY_NATIVE
                      if (globals.prefs->sony.device)
                      {
                        GameWideScreen(globals.prefs);

                        if (widescreenMode)
                        {
                          globals.prefs->game.cursor.x =
                            globals.prefs->game.cursor.x >> 1;

                          pos = globals.prefs->game.cursor.screenOffset + 80;
                          if (pos < 0) pos = 0; else
                          if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
                            pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);
                          globals.prefs->game.cursor.screenOffset = pos;
                        }
                        else
                        {
                          globals.prefs->game.cursor.x =
                            globals.prefs->game.cursor.x << 1;

                          pos = globals.prefs->game.cursor.screenOffset - 80;
                          if (pos < 0) pos = 0; else
                          if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
                            pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);
                          globals.prefs->game.cursor.screenOffset = pos;
                        }
                      }
#endif
#if PALM_HIDENSITY
                      if (globals.prefs->palmHD.device)
                      {
                        GameWideScreen(globals.prefs);

                        if (widescreenMode)
                        {
                          globals.prefs->game.cursor.x =
                            globals.prefs->game.cursor.x >> 1;

                          pos = globals.prefs->game.cursor.screenOffset + 80;
                          if (pos < 0) pos = 0; else
                          if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
                            pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);
                          globals.prefs->game.cursor.screenOffset = pos;
                        }
                        else
                        {
                          globals.prefs->game.cursor.x =
                            globals.prefs->game.cursor.x << 1;

                          pos = globals.prefs->game.cursor.screenOffset - 80;
                          if (pos < 0) pos = 0; else
                          if (pos > (OFFSCREEN_WIDTH - SCREEN_WIDTH))
                            pos = (OFFSCREEN_WIDTH - SCREEN_WIDTH);
                          globals.prefs->game.cursor.screenOffset = pos;
                        }
                      }
#endif
                    }

                    // send a close event
                    MemSet(event, sizeof(EventType), 0);
                    event->eType = frmCloseEvent;
                    event->data.frmClose.formID = FrmGetActiveFormID();
                    EvtAddEventToQueue(event);
                  }
                  else
                  {
                    SndPlaySystemSound(sndError);
                  }
                }
                processed = true;
                break;

           case cfigFormCancelButton:

                // revert changes
                DeviceSetMute(globals.prefs->config.sndMute);
                DeviceSetVolume(globals.prefs->config.sndVolume);

                // send a close event
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    case frmCloseEvent:

         // clean up
         FtrUnregister(appCreator, ftrGlobalsCfgActiveVol);

         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:grayForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
grayFormEventHandler(EventType *event)
{
  Boolean processed = false;

  switch (event->eType)
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         {
           FormType    *frm;
           ControlType *ctlWhite, *ctlLGray, *ctlDGray, *ctlBlack;

           frm = FrmGetActiveForm();
           ctlWhite =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, grayWhite1Button));
           ctlLGray =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm,
                 grayLightGray1Button + globals.prefs->config.lgray));
           ctlDGray =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm,
                 grayDarkGray1Button + globals.prefs->config.dgray));
           ctlBlack =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, grayBlack7Button));

           CtlSetValue(ctlWhite, 1); CtlDrawControl(ctlWhite);
           CtlSetValue(ctlLGray, 1); CtlDrawControl(ctlLGray);
           CtlSetValue(ctlDGray, 1); CtlDrawControl(ctlDGray);
           CtlSetValue(ctlBlack, 1); CtlDrawControl(ctlBlack);
         }

         // pre 3.5 - we must 'redraw' form to actually display PUSHBUTTONS
         if (!DeviceSupportsVersion(romVersion3_5))
         {
           FrmDrawForm(FrmGetActiveForm());
         }

         processed = true;
         break;

    case ctlEnterEvent:

         switch (event->data.ctlEnter.controlID)
         {
           case grayLightGray1Button:
           case grayLightGray7Button:
           case grayDarkGray1Button:
           case grayDarkGray7Button:

           // stupid user, they must select one of the other options
           SndPlaySystemSound(sndError);
           processed = true;
         }
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case grayLightGray2Button:
           case grayLightGray3Button:
           case grayLightGray4Button:
           case grayLightGray5Button:
           case grayLightGray6Button:

                globals.prefs->config.lgray = event->data.ctlEnter.controlID -
                                               grayLightGray1Button;
                DeviceGrayscale(graySet,
                                &globals.prefs->config.lgray,
                                &globals.prefs->config.dgray);
                processed = true;
                break;

           case grayDarkGray2Button:
           case grayDarkGray3Button:
           case grayDarkGray4Button:
           case grayDarkGray5Button:
           case grayDarkGray6Button:

                globals.prefs->config.dgray = event->data.ctlEnter.controlID -
                                               grayDarkGray1Button;
                DeviceGrayscale(graySet,
                                &globals.prefs->config.lgray,
                                &globals.prefs->config.dgray);
                processed = true;
                break;

           case grayFormOkButton:
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:infoForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
infoFormEventHandler(EventType *event)
{
  Boolean processed = false;

  switch (event->eType)
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case infoFormOkButton:

                // send a close event
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:dvlpForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
dvlpFormEventHandler(EventType *event)
{
  Boolean processed = false;

  switch (event->eType)
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case dvlpFormOkButton:

                // send a close event
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

#ifndef MDM_DISTRIBUTION
/**
 * The Form:demoForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
demoFormEventHandler(EventType *event)
{
  Boolean     processed = false;
  FormType    *frm;
  ControlType *showCheckbox;

  switch (event->eType)
  {
    case frmOpenEvent:
         frm = FrmGetActiveForm();

         // adjust checkbox to preference
         showCheckbox =
           (ControlType *)FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, demoFormDemoCheckbox));

         CtlSetValue(showCheckbox,
           (globals.prefs->system.showNotice) ? 1 : 0);

         FrmDrawForm(frm);

         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case demoFormAcceptButton:

                frm = FrmGetActiveForm();

                showCheckbox =
                  (ControlType *)FrmGetObjectPtr(frm,
                    FrmGetObjectIndex(frm, demoFormDemoCheckbox));

                // do we need to display this message in future?
                globals.prefs->system.showNotice =
                  (CtlGetValue(showCheckbox) != 0);

                // send a close event
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           case demoFormDeclineButton:

                // send a appstop event (dont wanna play)
                MemSet(event, sizeof(EventType), 0);
                event->eType = appStopEvent;
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}
#endif

/**
 * The Form:helpForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
helpFormEventHandler(EventType *event)
{
  Boolean processed = false;

  switch (event->eType)
  {
    case frmOpenEvent:
         {
           UInt16 helpHeight;

           helpHeight = InitInstructions();

           // help exists?
           if (helpHeight != 0)
           {
             FormType      *frm;
             ScrollBarType *sclBar;
             Int16         val, min, max, pge;

             frm    = FrmGetActiveForm();
             FrmDrawForm(frm);

             sclBar =
               (ScrollBarType *)FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, helpFormScrollBar));

             SclGetScrollBar(sclBar, &val, &min, &max, &pge);
             val = helpHeight;
             max = (val > pge) ? (val-(pge+16)) : 0;
             SclSetScrollBar(sclBar, 0, min, max, pge);

             DrawInstructions(0);
           }

           // no help, close form
           else
           {
             EventType newEvent;

             MemSet(&newEvent, sizeof(EventType), 0);
             newEvent.eType = frmCloseEvent;
             newEvent.data.frmClose.formID = FrmGetActiveFormID();
             EvtAddEventToQueue(&newEvent);
           }
         }
         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case helpFormOkButton:

                // send a close event
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    case sclRepeatEvent:
         {
           FormType      *frm;
           ScrollBarType *sclBar;
           Int16         val, min, max, pge;
//         UInt16        val, min, max, pge;

           frm = FrmGetActiveForm();
           sclBar =
             (ScrollBarType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, helpFormScrollBar));

           SclGetScrollBar(sclBar, &val, &min, &max, &pge);
           DrawInstructions(val);
         }
         break;

    case keyDownEvent:

         switch (event->data.keyDown.chr)
         {
           case vchrJogUp:                    // jog dial up: sony
           case vchrPrevField:                // jog dial up: handera
//         case vchrTrgJogUp:                 // jog dial up: handera
           case vchrPageUp:
                {
                  FormType      *frm;
                  ScrollBarType *sclBar;
                  Int16         val, min, max, pge;

                  frm = FrmGetActiveForm();
                  sclBar =
                    (ScrollBarType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, helpFormScrollBar));

                  SclGetScrollBar(sclBar, &val, &min, &max, &pge);
                  val = (pge > val) ? 0 : (val-pge);
                  SclSetScrollBar(sclBar, val, min, max, pge);
                  DrawInstructions(val);
                }
                processed = true;
                break;

           case vchrJogDown:                  // jog dial down: sony
           case vchrNextField:                // jog dial down: handera
//         case vchrTrgJogDown:               // jog dial down: handera
           case vchrPageDown:
                {
                  FormType      *frm;
                  ScrollBarType *sclBar;
                  Int16         val, min, max, pge;

                  frm = FrmGetActiveForm();
                  sclBar =
                    (ScrollBarType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, helpFormScrollBar));

                  SclGetScrollBar(sclBar, &val, &min, &max, &pge);
                  val = (max < (val+pge)) ? max : (val+pge);
                  SclSetScrollBar(sclBar, val, min, max, pge);
                  DrawInstructions(val);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    case frmCloseEvent:

         // clean up
         QuitInstructions();
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:xmemForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
xmemFormEventHandler(EventType *event)
{
  Boolean processed = false;

  switch (event->eType)
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case xmemFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:prepForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
prepFormEventHandler(EventType *event)
{
  Boolean       processed = false;
  Char          str[36];
  UInt16        i, j, count;
  UInt32        depth;
  UInt8         *ptrPrep, *ptrScreen;
  WinHandle     winPrep;
#if HANDERA_NATIVE || SONY_NATIVE || PALM_HIDENSITY
  BitmapType    *bmpPrep   = NULL;
#endif
#if PALM_HIDENSITY
  BitmapTypeV3  *bmpPrepV3 = NULL;
#endif
  RectangleType rect;
  Err           e;
  EventType     newEvent;

  switch (event->eType)
  {
    case frmOpenEvent:

         // post an update event
         MemSet(&newEvent, sizeof(EventType), 0);
         newEvent.eType = appUpdateEvent;
         EvtAddEventToQueue(&newEvent);

         processed = true;
         break;

    case frmUpdateEvent:

         // we ignore these, stupid palmos does funky stuff
         processed = true;
         break;

    case appUpdateEvent:
         FrmDrawForm(FrmGetActiveForm());

         rect.topLeft.x = 0;
         rect.topLeft.y = 0;
         rect.extent.x  = 128;
         rect.extent.y  = 25;

         depth = (DeviceSupportsColor() ? 4 : 2);

#if HANDERA_NATIVE
         if (globals.prefs->handera.device)
         {
           bmpPrep = BmpCreate(rect.extent.x, rect.extent.y, depth, NULL, &e);
           winPrep = WinCreateBitmapWindow(bmpPrep, &e);
         }
         else
#endif
#if SONY_NATIVE
         if (globals.prefs->sony.device)
         {
           rect.extent.x = rect.extent.x << 1;
           rect.extent.y = rect.extent.y << 1;

           bmpPrep = BmpCreate(rect.extent.x, rect.extent.y, depth, NULL, &e);
           winPrep = WinCreateBitmapWindow(bmpPrep, &e);
         }
         else
#endif
#if PALM_HIDENSITY
         if ((globals.prefs->palmHD.device) && (globals.prefs->palmHD.density == kDensityDouble))
         {
           rect.extent.x = rect.extent.x << 1;
           rect.extent.y = rect.extent.y << 1;

           bmpPrep   = BmpCreate(rect.extent.x, rect.extent.y, depth, NULL, &e);
           bmpPrepV3 = BmpCreateBitmapV3(bmpPrep,
                                         globals.prefs->palmHD.density,
                                         BmpGetBits(bmpPrep), NULL);
           winPrep = WinCreateBitmapWindow((BitmapType *)bmpPrepV3, &e);
         }
         else
#endif
           winPrep =
             WinCreateOffscreenWindow(rect.extent.x,
                                      rect.extent.y, screenFormat, &e);

         // no space to do this? skip the preview..
         if (winPrep == NULL) goto SKIP_PREVIEW;

         ptrPrep   = DeviceWindowGetPointer(winPrep);
         ptrScreen = DeviceWindowGetPointer(GraphicsGetDrawWindow());

         // offset to skip over the "buffer area"
         if (depth == 2) ptrScreen += (SPR_HEIGHT * 160);
         else            ptrScreen += (SPR_HEIGHT * 320);

         // generate mini map
         j = rect.extent.y;
         do
         {
           // grayscale
           if (depth == 2)
           {
#if SONY_NATIVE
             if (globals.prefs->sony.device)
             {
               i = 32;
               do
               {
                 *(ptrPrep++) = ((*(ptrScreen)    & 0xC0) |
                                 ((*(ptrScreen)   & 0x03) << 4) |
                                 ((*(ptrScreen+1) & 0xC0) >> 4) |
                                  (*(ptrScreen+1) & 0x03));
                 *(ptrPrep++) = ((*(ptrScreen+3)  & 0xC0) |
                                 ((*(ptrScreen+3) & 0x03) << 4) |
                                 ((*(ptrScreen+4) & 0xC0) >> 4) |
                                 (*(ptrScreen+4)  & 0x03));

                 ptrScreen += 5;
               }
               while (--i);

               ptrScreen += (j % 2) ? 320 : 160;
             }
             else
#endif
#if PALM_HIDENSITY
             if ((globals.prefs->palmHD.device) && (globals.prefs->palmHD.density == kDensityDouble))
             {
               i = 32;
               do
               {
                 *(ptrPrep++) = ((*(ptrScreen)    & 0xC0) |
                                 ((*(ptrScreen)   & 0x03) << 4) |
                                 ((*(ptrScreen+1) & 0xC0) >> 4) |
                                  (*(ptrScreen+1) & 0x03));
                 *(ptrPrep++) = ((*(ptrScreen+3)  & 0xC0) |
                                 ((*(ptrScreen+3) & 0x03) << 4) |
                                 ((*(ptrScreen+4) & 0xC0) >> 4) |
                                 (*(ptrScreen+4)  & 0x03));

                 ptrScreen += 5;
               }
               while (--i);

               ptrScreen += (j % 2) ? 320 : 160;
             }
             else
#endif
             // default mode
             {
               i = 32;
               do
               {
                 *(ptrPrep++) = ((*(ptrScreen)   & 0xC0) |
                                 (*(ptrScreen+1) & 0x30) |
                                 (*(ptrScreen+3) & 0x0C) |
                                 (*(ptrScreen+4) & 0x03));

                 ptrScreen += 5;
               }
               while (--i);
               ptrScreen += 640;
             }
           }

           // 4bpc color
           else
           {
#if SONY_NATIVE
             if (globals.prefs->sony.device)
             {
               i = 64;
               do
               {
                 *(ptrPrep++) = ((*(ptrScreen)   & 0xF0) |
                                 (*(ptrScreen+1) & 0x0F));
                 *(ptrPrep++) = ((*(ptrScreen+3) & 0xF0) |
                                 (*(ptrScreen+3) & 0x0F));

                 ptrScreen += 5;
               }
               while (--i);

               ptrScreen += (j % 2) ? 640 : 320;
             }
             else
#endif
#if PALM_HIDENSITY
             if ((globals.prefs->palmHD.device) && (globals.prefs->palmHD.density == kDensityDouble))
             {
               i = 64;
               do
               {
                 *(ptrPrep++) = ((*(ptrScreen)   & 0xF0) |
                                 (*(ptrScreen+1) & 0x0F));
                 *(ptrPrep++) = ((*(ptrScreen+3) & 0xF0) |
                                 (*(ptrScreen+3) & 0x0F));

                 ptrScreen += 5;
               }
               while (--i);

               ptrScreen += (j % 2) ? 640 : 320;
             }
             else
#endif
             // default mode
             {
               i = 64;
               do
               {
                 *(ptrPrep++) = ((*(ptrScreen)   & 0xF0) |
                                 (*(ptrScreen+3) & 0x0F));

                 ptrScreen += 5;
               }
               while (--i);
               ptrScreen += 1280;
             }
           }
         }
         while (--j);

         // draw mini map
#if HANDERA_NATIVE || SONY_NATIVE || PALM_HIDENSITY
         if (globals.prefs->handera.device)
         {
           WinDrawBitmap(bmpPrep, 14, 18);
           BmpDelete(bmpPrep);
         }
         else
#endif
#if SONY_NATIVE
         if (globals.prefs->sony.device)
         {
           HRWinCopyRectangle(globals.prefs->sony.libRef,
                              winPrep, WinGetDrawWindow(),
                              &rect, 28, 36, winPaint);
           BmpDelete(bmpPrep);
         }
         else
#endif
#if PALM_HIDENSITY
         if ((globals.prefs->palmHD.device) && (globals.prefs->palmHD.density == kDensityDouble))
         {
           WinCopyRectangle(winPrep, WinGetDrawWindow(),
                            &rect, 14, 18, winPaint);
           BmpDelete(bmpPrep);
           BmpDelete((BitmapType *)bmpPrepV3);
         }
         else
#endif
           WinCopyRectangle(winPrep, WinGetDrawWindow(),
                            &rect, 14, 18, winPaint);

         // clean up
         WinDeleteWindow(winPrep, false);

SKIP_PREVIEW:

         // the name of the level?
//       StrCopy(str, globals.prefs->game.levelTitle);
         StrPrintF(str, "%02d: %s",
                   globals.prefs->game.gameLevel, globals.prefs->game.levelTitle);
         WinDrawChars(str, StrLen(str),
                      (156 - FntCharsWidth(str, StrLen(str))) >> 1, 46);

         // how many lemmings are to be released?
         StrIToA(str, globals.prefs->game.lemmingOut);
         FntSetFont(boldFont);
         WinDrawChars(str, StrLen(str),
                      142 - FntCharsWidth(str, StrLen(str)), 64);
         FntSetFont(stdFont);

         // how many lemmings are required?
         StrIToA(str, globals.prefs->game.lemmingReq);
         FntSetFont(boldFont);
         WinDrawChars(str, StrLen(str),
                      142 - FntCharsWidth(str, StrLen(str)), 75);
         FntSetFont(stdFont);

         // time limit?
         StrPrintF(str, "%d:%d%d",
                   (globals.prefs->game.timeRemaining / 60) % 10,
                   (globals.prefs->game.timeRemaining % 60) / 10,
                   (globals.prefs->game.timeRemaining % 60) % 10);
         FntSetFont(boldFont);
         WinDrawChars(str, StrLen(str),
                      142 - FntCharsWidth(str, StrLen(str)), 88);
         FntSetFont(stdFont);

         // whats the 'count' for each tool
         for (i=0; i<TOOL_COUNT-1; i++)
         {
           count = globals.prefs->game.tools[i];

           if (count == 0) StrPrintF(str, "--", count);
           else            StrPrintF(str, "%02d", count);

           WinDrawChars(str, StrLen(str), 24 + (i * 14), 104);
         }

         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case prepFormGoButton:

                // send a close event
                MemSet(&newEvent, sizeof(EventType), 0);
                newEvent.eType                = frmCloseEvent;
                newEvent.data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(&newEvent);

                globals.prefs->game.gameState = GAME_PLAY;

                // 'lets go!' audio playback
                GamePlaySound(globals.prefs, snd_letsgo);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:verdForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
verdFormEventHandler(EventType *event)
{
  Boolean     processed = false;
  Char        str[64];                      // <<-- [32] = one byte too small!
  FormType    *frm;
  ControlType *ctlButton;
  UInt16      idStrButton, idStrMessage;
  EventType   newEvent;
  UInt16      val;

  switch (event->eType)
  {
    case frmOpenEvent:

         // terminate any music if it is running :)
         GameMusicStop(globals.prefs);

         // post an update event
         MemSet(&newEvent, sizeof(EventType), 0);
         newEvent.eType = appUpdateEvent;
         EvtAddEventToQueue(&newEvent);

         processed = true;
         break;

    case frmUpdateEvent:

         // we ignore these, stupid palmos does funky stuff
         processed = true;
         break;

    case appUpdateEvent:

         frm = FrmGetActiveForm();
         FrmDrawForm(frm);

         WinDrawLine(4,36,152,36);
         WinDrawLine(4,38,152,38);
         WinDrawLine(4,72,152,72);
         WinDrawLine(4,74,152,74);

         // how many lemmings were required?
         val = (globals.prefs->game.lemmingReq * 100) /
                   globals.prefs->game.lemmingOut;
         MemSet(str, 64, 0);
         StrPrintF(str, "%d %%", val);
         FntSetFont(boldFont);
         WinDrawChars(str, StrLen(str),
                      142 - FntCharsWidth(str, StrLen(str)), 44);
         FntSetFont(stdFont);

         // how many lemmings were saved?
         val = (globals.prefs->game.lemmingSaved * 100) /
                   globals.prefs->game.lemmingOut;
         MemSet(str, 64, 0);
         StrPrintF(str, "%d %%", val);
         FntSetFont(boldFont);
         WinDrawChars(str, StrLen(str),
                      142 - FntCharsWidth(str, StrLen(str)), 55);
         FntSetFont(stdFont);

         // determine what the button should look like
         if (globals.prefs->game.lemmingSaved >=
             globals.prefs->game.lemmingReq)
           idStrButton  = stringVerdButton1;
         else
           idStrButton  = stringVerdButton0;

         // determine what message should be printed to the user
         if (globals.prefs->game.lemmingSaved == 0)
           idStrMessage = stringVerdLevel0_0;
         else
         if (globals.prefs->game.lemmingSaved < globals.prefs->game.lemmingReq)
           idStrMessage = stringVerdLevel1_0;
         else
         if (globals.prefs->game.lemmingSaved == globals.prefs->game.lemmingOut)
           idStrMessage = stringVerdLevel2_0;
         else
         if (globals.prefs->game.lemmingSaved == globals.prefs->game.lemmingReq)
           idStrMessage = stringVerdLevel3_0;
         else
           idStrMessage = stringVerdLevel4_0;

         FntSetFont(boldFont);
         MemSet(str, 64, 0);
         SysCopyStringResource(str, idStrMessage);
         WinDrawChars(str, StrLen(str),
                      (156 - FntCharsWidth(str, StrLen(str))) >> 1, 80);
         FntSetFont(stdFont);

         MemSet(str, 64, 0);
         SysCopyStringResource(str, idStrMessage+1);
         WinDrawChars(str, StrLen(str),
                      (156 - FntCharsWidth(str, StrLen(str))) >> 1, 80+15);
         MemSet(str, 64, 0);
         SysCopyStringResource(str, idStrMessage+2);
         WinDrawChars(str, StrLen(str),
                      (156 - FntCharsWidth(str, StrLen(str))) >> 1, 80+26);

         FntSetFont(boldFont);
         MemSet(str, 64, 0);
         SysCopyStringResource(str, idStrMessage+3);
         WinDrawChars(str, StrLen(str),
                      (156 - FntCharsWidth(str, StrLen(str))) >> 1, 80+39);
         FntSetFont(stdFont);

         ctlButton =
           (ControlType *)FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, verdFormOkButton));
         SysCopyStringResource(globals.strButton, idStrButton);
         CtlSetLabel(ctlButton, globals.strButton);

         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case verdFormOkButton:

                // send a close event
                MemSet(&newEvent, sizeof(EventType), 0);
                newEvent.eType                = frmCloseEvent;
                newEvent.data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(&newEvent);

                // did not completed the level?
                if (globals.prefs->game.lemmingSaved <
                    globals.prefs->game.lemmingReq)
                {
                  // try it again :)
                  LevelPackOpen(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack);
                  GameResetPreferences(globals.prefs);
                  GameLoadLevel(globals.prefs);
                  LevelPackClose();
                }

                // complete level *g*
                else
                {
                  // completed all the levels?
                  if (globals.prefs->game.gameLevel == GameGetLevelCount())
                  {
                    globals.prefs->game.gamePlaying = false;

                    // return to the main screen
                    MemSet(&newEvent, sizeof(EventType), 0);
                    newEvent.eType            = menuEvent;
                    newEvent.data.menu.itemID = gameMenuItemExit;
                    EvtAddEventToQueue(&newEvent);
                  }
                  else
                  {
                    globals.prefs->game.gameLevel =
                      (globals.prefs->game.gameLevel % GameGetLevelCount())+1;

                    LevelPackOpen(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack);
                    GameResetPreferences(globals.prefs);
                    GameLoadLevel(globals.prefs);
                    LevelPackClose();
                  }
                }

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:regiForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
regiFormEventHandler(EventType *event)
{
  Boolean processed = false;

  switch (event->eType)
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());

         // display the HotSync username on the screen (in HEX)
         {
           Char  ID[40];
           Char  tmp[10], num[3];
           Coord x;
           UInt8 i, checksum;

           FontID font = FntGetFont();

           // initialize
           StrCopy(ID, "");

           // generate strings
           checksum = 0;
           for (i=0; i<MAX_IDLENGTH; i++)
           {
             checksum ^= (UInt8)globals.system.hotSyncUsername[i];
             StrIToH(tmp, (UInt8)globals.system.hotSyncUsername[i]);
             StrNCopy(num, &tmp[StrLen(tmp)-2], 2); num[2] = '\0';
             StrCat(ID, num); StrCat(ID, ":");
           }
           StrIToH(tmp, checksum);
           StrNCopy(num, &tmp[StrLen(tmp)-2], 2); num[2] = '\0';
           StrCat(ID, num);

           FntSetFont(boldFont);
           x = (160 - FntCharsWidth(ID, StrLen(ID))) >> 1;
           WinDrawChars(ID, StrLen(ID), x, 82);
           FntSetFont(font);

           WinDrawLine(   4,  78, 152,  78);
           WinDrawLine(   4,  80, 152,  80);
           WinDrawLine(   4, 106, 152, 106);
           WinDrawLine(   4, 108, 152, 108);
         }
         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case regiFormOkButton:

                // send a close event
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:packForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
packFormEventHandler(EventType *event)
{
  Boolean           processed = false;
  FormType          *frm;
  ListType          *lstPacks;
  ControlType       *difficultCheckbox;
  DmSearchStateType stateInfo;
  Err               error;
  UInt16            card, pos, i;
  LocalID           dbID;                 // database variables
  UInt32            volIterator, dirIterator, expMgrVersion;
  UInt16            volRef;
  FileInfoType      info;
  FileRef           dirRef, fileRef;
  Char              *fullPath, *fileName;
  Char              strDatabaseName[32];
  Char              **strLevelPacks, **strLevelPackFileName;
  Char              *ptr;                 // level pack stuff
  Err               err;
  EventType         newEvent;

  switch (event->eType)
  {
    case frmOpenEvent:

         // the internal pack :)
         StrCopy(globals.pack.strPackList[0],
                 "Lemmings: Original Trainer [std]");

         // post an update event
         MemSet(&newEvent, sizeof(EventType), 0);
         newEvent.eType = appUpdateEvent;
         EvtAddEventToQueue(&newEvent);

         processed = true;
         break;

    case appUpdateEvent:
         frm = FrmGetActiveForm();

         // clean up from last time?
         for (i=1; i<MAX_PACKS; i++)
         {
           MemSet(globals.pack.strPackList[i], 40, 0);
           MemSet(globals.pack.strPackFileName[i], 40, 0);
         }

         // update the "difficulty" checkbox
         difficultCheckbox =
           (ControlType *)FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, packFormHardCheckbox));
         CtlSetValue(difficultCheckbox,
           (globals.prefs->levelPack.useDifficult) ? 1 : 0);

         // search palmos ram
         globals.pack.packCount = 0;
         {
           error =
             DmGetNextDatabaseByTypeCreator(true, &stateInfo,
                                            levlType, appCreator, false,
                                            &card, &dbID);

           strLevelPacks        = globals.pack.strPackList+1;
           strLevelPackFileName = globals.pack.strPackFileName+1;
           while ((error == errNone) &&
                  (globals.pack.packCount < (MAX_PACKS-1)))
           {
             // extract the database information
             DmDatabaseInfo(card, dbID, strDatabaseName,
                            NULL, NULL, NULL, NULL, NULL,
                            NULL, NULL, NULL, NULL, NULL);

             // find out where it should go
             pos = 0;
             while ((pos < globals.pack.packCount) &&
                    (StrCompare(strDatabaseName,
                                strLevelPacks[pos]+2) > 0))
               pos++;

             // already there - abort man!?
             if ((StrLen(strDatabaseName) == StrLen(strLevelPacks[pos]+2)) &&
                 (StrNCompare(strDatabaseName, strLevelPacks[pos]+2, StrLen(strDatabaseName)) == 0))
               goto RAM_ABORT;

             // do we need to shift a few things?
             if (pos < globals.pack.packCount)
             {
               // move em down
               for (i=globals.pack.packCount; i>pos; i--)
                 StrCopy(strLevelPacks[i], strLevelPacks[i-1]);
             }

             // copy it to the list
             StrPrintF(strLevelPacks[pos], "\273 %s", strDatabaseName);

             // next sequence
             error =
               DmGetNextDatabaseByTypeCreator(false, &stateInfo,
                                              levlType, appCreator, false,
                                              &card, &dbID);
             globals.pack.packCount++;

RAM_ABORT:

           }
         }

         // search vfs card
         {
           err = FtrGet(sysFileCExpansionMgr, expFtrIDVersion, &expMgrVersion);
           if (err == errNone)
           {
             volIterator = vfsIteratorStart;
             while (volIterator != vfsIteratorStop)
             {
               err = VFSVolumeEnumerate(&volRef, &volIterator);

               // we've got a volume, now let's look in the specified directory
               if (err == errNone)
               {
                 fullPath = (Char *)MemPtrNew(256);
                 fileName = (Char *)MemPtrNew(256);

                 // open the directory first, to get the directory reference
                 err = VFSFileOpen(volRef, VFS_DIRECTORY, vfsModeRead, &dirRef);
                 if (err == errNone)
                 {
                   info.nameP      = fileName;
                   info.nameBufLen = 256;
                   dirIterator = vfsIteratorStart;
                   while (dirIterator != vfsIteratorStop)
                   {
                     // get the next file
                     err = VFSDirEntryEnumerate(dirRef, &dirIterator, &info);
                     if (err == errNone)
                     {
                       if (((info.attributes & vfsFileAttrDirectory) == 0) &&
                           (globals.pack.packCount < (MAX_PACKS-1)))
                       {
                         UInt32 cr8r, type;

                         // the filename?
                         MemSet(fullPath, 256, 0);
                         StrPrintF(fullPath, "%s%s", VFS_DIRECTORY, fileName);

                         err = VFSFileOpen(volRef, fullPath, vfsModeRead, &fileRef);
                         if (err == errNone)
                         {
                           VFSFileDBInfo(fileRef, strDatabaseName,
                                         NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &type, &cr8r, NULL);
                           VFSFileClose(fileRef);

                           // lemmings database file?
                           if ((type == levlType) && (cr8r == appCreator))
                           {
                             // find out where it should go
                             pos = 0;
                             while ((pos < globals.pack.packCount) &&
                                    (StrCompare(strDatabaseName,
                                                strLevelPacks[pos]+2) > 0))
                               pos++;

                             // already there - abort man!?
                             if ((StrLen(strDatabaseName) == StrLen(strLevelPacks[pos]+2)) &&
                                 (StrNCompare(strDatabaseName, strLevelPacks[pos]+2, StrLen(strDatabaseName)) == 0))
                               goto VFS_ABORT;

                             // do we need to shift a few things?
                             if (pos < globals.pack.packCount)
                             {
                               // move em down
                               for (i=globals.pack.packCount; i>pos; i--)
                               {
                                 StrCopy(strLevelPacks[i], strLevelPacks[i-1]);
                                 StrCopy(strLevelPackFileName[i], strLevelPackFileName[i-1]);
                               }
                             }

                             // copy it to the list
                             StrPrintF(strLevelPacks[pos], "\032 %s", strDatabaseName);
                             StrCopy(strLevelPackFileName[pos], fileName);

                             globals.pack.packCount++;
VFS_ABORT:
                           }
                         }
                       }
                     }
                     else
                       dirIterator = vfsIteratorStop;
                   }

                   VFSFileClose(dirRef);
                 }

                 MemPtrFree(fileName);
                 MemPtrFree(fullPath);
               }
               else
                 volIterator = vfsIteratorStop;  // terminate
             }
           }
         }

#ifdef MDM_DISTRIBUTION
         // search vfs card [MDM specific]
         {
           err = FtrGet(sysFileCExpansionMgr, expFtrIDVersion, &expMgrVersion);
           if (err == errNone)
           {
             volIterator = vfsIteratorStart;
             while (volIterator != vfsIteratorStop)
             {
               err = VFSVolumeEnumerate(&volRef, &volIterator);

               // we've got a volume, now let's look in the specified directory
               if (err == errNone)
               {
                 fullPath = (Char *)MemPtrNew(256);
                 fileName = (Char *)MemPtrNew(256);

                 // open the directory first, to get the directory reference
                 err = VFSFileOpen(volRef, VFS_MDM_DIRECTORY, vfsModeRead, &dirRef);
                 if (err == errNone)
                 {
                   info.nameP      = fileName;
                   info.nameBufLen = 256;
                   dirIterator = vfsIteratorStart;
                   while (dirIterator != vfsIteratorStop)
                   {
                     // get the next file
                     err = VFSDirEntryEnumerate(dirRef, &dirIterator, &info);
                     if (err == errNone)
                     {
                       if (((info.attributes & vfsFileAttrDirectory) == 0) &&
                           (globals.pack.packCount < (MAX_PACKS-1)))
                       {
                         UInt32 cr8r, type;

                         // the filename?
                         MemSet(fullPath, 256, 0);
                         StrPrintF(fullPath, "%s%s", VFS_MDM_DIRECTORY, fileName);

                         err = VFSFileOpen(volRef, fullPath, vfsModeRead, &fileRef);
                         if (err == errNone)
                         {
                           VFSFileDBInfo(fileRef, strDatabaseName,
                                         NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &type, &cr8r, NULL);
                           VFSFileClose(fileRef);

                           // lemmings database file?
                           if ((type == levlType) && (cr8r == appCreator))
                           {
                             // find out where it should go
                             pos = 0;
                             while ((pos < globals.pack.packCount) &&
                                    (StrCompare(strDatabaseName,
                                                strLevelPacks[pos]+2) > 0))
                               pos++;

                             // already there - abort man!?
                             if ((StrLen(strDatabaseName) == StrLen(strLevelPacks[pos]+2)) &&
                                 (StrNCompare(strDatabaseName, strLevelPacks[pos]+2, StrLen(strDatabaseName)) == 0))
                               goto VFS_MDM_ABORT;

                             // do we need to shift a few things?
                             if (pos < globals.pack.packCount)
                             {
                               // move em down
                               for (i=globals.pack.packCount; i>pos; i--)
                               {
                                 StrCopy(strLevelPacks[i], strLevelPacks[i-1]);
                                 StrCopy(strLevelPackFileName[i], strLevelPackFileName[i-1]);
                               }
                             }

                             // copy it to the list
                             StrPrintF(strLevelPacks[pos], "\244 %s", strDatabaseName);
                             StrCopy(strLevelPackFileName[pos], fileName);

                             globals.pack.packCount++;
VFS_MDM_ABORT:
                           }
                         }
                       }
                     }
                     else
                       dirIterator = vfsIteratorStop;
                   }

                   VFSFileClose(dirRef);
                 }

                 MemPtrFree(fileName);
                 MemPtrFree(fullPath);
               }
               else
                 volIterator = vfsIteratorStop;  // terminate
             }
           }
         }
#endif

         globals.pack.packCount++; // gotta include "built in levels"

         // update the level pack list
         lstPacks =
           (ListType *)FrmGetObjectPtr(frm,
              FrmGetObjectIndex(frm, packFormDatabaseList));
         LstSetListChoices(lstPacks,
                           globals.pack.strPackList, globals.pack.packCount);
         LstSetSelection(lstPacks, 0);  // highlight the built in levels

         // redraw the form
         FrmDrawForm(frm);

         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case packFormDeleteButton:

                frm = FrmGetActiveForm();

                lstPacks =
                  (ListType *)FrmGetObjectPtr(frm,
                    FrmGetObjectIndex(frm, packFormDatabaseList));
                if (LstGetSelection(lstPacks) > 0)
                {
                  ptr = globals.pack.strPackList[LstGetSelection(lstPacks)];

                  // ram based level pack? (first char = tells us)
                  if (*ptr == '\273')
                  {
                    StrCopy(strDatabaseName, ptr+2);

                    card = 0;
                    dbID = DmFindDatabase(card, strDatabaseName);
                    if ((dbID != NULL) &&
                        (FrmCustomAlert(removePackAlert,
                                        strDatabaseName, NULL, NULL) == 0))
                    {
                      DmDeleteDatabase(card, dbID);

                      // post an update event
                      MemSet(&newEvent, sizeof(EventType), 0);
                      newEvent.eType = appUpdateEvent;
                      EvtAddEventToQueue(&newEvent);
                    }
                  }
                  else
                    SndPlaySystemSound(sndError);
                }
                else
                  SndPlaySystemSound(sndError);

                processed = true;
                break;

           case packFormOpenButton:

                frm = FrmGetActiveForm();

                // put preference over hard levels first?
                difficultCheckbox =
                  (ControlType *)FrmGetObjectPtr(frm,
                    FrmGetObjectIndex(frm, packFormHardCheckbox));
                globals.prefs->levelPack.useDifficult =
                  (CtlGetValue(difficultCheckbox) != 0);

                // which level pack is selected?
                lstPacks =
                  (ListType *)FrmGetObjectPtr(frm,
                    FrmGetObjectIndex(frm, packFormDatabaseList));

                // didn't select built in? which one then?
                if (LstGetSelection(lstPacks) != 0)
                {
                  ptr = globals.pack.strPackList[LstGetSelection(lstPacks)];

                  // open the level pack database
                  if (*ptr == '\273')  // ram
                  {
                    globals.prefs->levelPack.type = PACK_RAM;
                    StrCopy(globals.prefs->levelPack.strLevelPack, ptr+2);
                  }
                  else
                  if (*ptr == '\032')  // vfs
                  {
                    globals.prefs->levelPack.type = PACK_VFS;
                    StrCopy(globals.prefs->levelPack.strLevelPack,
                            globals.pack.strPackFileName[LstGetSelection(lstPacks)]);
                  }
#ifdef MDM_DISTRIBUTION
                  else
                  if (*ptr == '\244')  // vfs MDM
                  {
                    globals.prefs->levelPack.type = PACK_VFS_MDM;
                    StrCopy(globals.prefs->levelPack.strLevelPack,
                            globals.pack.strPackFileName[LstGetSelection(lstPacks)]);
                  }
#endif
                }
                else
                {
                  globals.prefs->levelPack.type = PACK_INTERNAL;
                  MemSet(globals.prefs->levelPack.strLevelPack, 32, 0);
                }

                // send a close event
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           case packFormCancelButton:

                // send a close event
                MemSet(event, sizeof(EventType), 0);
                event->eType = frmCloseEvent;
                event->data.frmClose.formID = FrmGetActiveFormID();
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The CardMounted notification handling routine.
 *
 * @param notifyParamsP the notification parameters.
 * @return errNone if no errors, error code otherwise.
 */
static Err
notifyCardMounted(SysNotifyParamType *notifyParamsP)
{
  EventType event;

  // dont switch to the application on the card (we support card)
  notifyParamsP->handled |= vfsHandledUIAppSwitch;

  // in some cases, it may be important to regenerate the display
  MemSet(&event, sizeof(EventType), 0);
  event.eType = appUpdateEvent;
  EvtAddEventToQueue(&event);

  return 0;
}

/**
 * The CardRemoved notification handling routine.
 *
 * @param notifyParamsP the notification parameters.
 * @return errNone if no errors, error code otherwise.
 */
static Err
notifyCardRemoval(SysNotifyParamType *notifyParamsP)
{
  EventType event;

#ifdef MDM_DISTRIBUTION
  MemSet(&event, sizeof(EventType), 0);
  event.eType = appStopEvent;
  EvtAddEventToQueue(&event);
#else
  // exit the application if currently using VFS based level pack
  if (globals.prefs->levelPack.type == PACK_VFS)
  {
    MemSet(&event, sizeof(EventType), 0);
    event.eType = appStopEvent;
    EvtAddEventToQueue(&event);
  }

  // in some cases, it may be important to regenerate the display
  else
  {
    MemSet(&event, sizeof(EventType), 0);
    event.eType = appUpdateEvent;
    EvtAddEventToQueue(&event);
  }
#endif

  return 0;
}

/**
 * The SleepRequest notification handling routine.
 *
 * @param notifyParamsP the notification parameters.
 * @return errNone if no errors, error code otherwise.
 */
static Err
notifySleepRequest(SysNotifyParamType *notifyParamsP)
{
  // sleep: lets pause the game/music playback as appropriate
  if (globals.prefs->game.gamePlaying)
  {
    EventType event;

    // pause the game
    GamePause(globals.prefs, true);

    // exit the application
    MemSet(&event, sizeof(EventType), 0);
    event.eType = appStopEvent;
    EvtAddEventToQueue(&event);
  }

  return 0;
}

/**
 * The Palm Computing Platform initialization routine.
 */
void
InitApplication()
{
  UInt16  card;
  LocalID dbID;
  UInt32  version;
#if HANDERA_NATIVE
  UInt32  trgVersion;
#endif
#if PALM_HIDENSITY
  UInt32  winVersion;
#endif

  // load preferences
  {
    MemHandle keyH;
    Boolean   reset;
    UInt16    prefSize;
    Int16     flag;
    LocalID   dbID;

    // lets see how large the preference is (if it is there)
    reset    = true;
    prefSize = 0;
    flag     = PrefGetAppPreferences(appCreator, 0, NULL, &prefSize, true);

    // we have some preferences, maybe a match :)
    if ((flag != noPreferenceFound) && (prefSize == sizeof(PreferencesType)))
    {
      // extract all the bytes
      PrefGetAppPreferences(appCreator, 0, globals.prefs, &prefSize, true);

      // decrypt the memory chunk (based on the mesg0000.bin)
      keyH = DmGetResource('mesg', 0x0000);
      RegisterDecryptChunk((UInt8 *)globals.prefs, prefSize, keyH, 0x01);
      DmReleaseResource(keyH);

      // decryption screw up? reset
      reset = !CHECK_SIGNATURE(globals.prefs) ||
              (globals.prefs->system.signatureVersion != VERSION);
    }

    // do we need to reset the preferences?
    if (reset)
    {
      // set default values
      MemSet(globals.prefs, sizeof(PreferencesType), 0);

      globals.prefs->system.signatureVersion = VERSION;
      StrCopy(globals.prefs->system.signature, "|HaCkMe|");

#ifndef MDM_DISTRIBUTION
// PGHQ hack
//    globals.prefs->system.showNotice = false;
      globals.prefs->system.showNotice = true;
#endif

      globals.prefs->config.ctlKeySelect      = keyBitHard4;
      globals.prefs->config.ctlKeyTool        = keyBitHard1;
      globals.prefs->config.ctlKeyUp          = keyBitPageUp;
      globals.prefs->config.ctlKeyDown        = keyBitPageDown;
      globals.prefs->config.ctlKeyLeft        = keyBitHard2;
      globals.prefs->config.ctlKeyRight       = keyBitHard3;

      globals.prefs->config.graffitiScroll    = true;
      globals.prefs->config.widescreenDisplay = false;
      globals.prefs->config.musicEnabled      = true;

      globals.prefs->config.sndMute           = false;
      globals.prefs->config.sndVolume         = 4;     // dont start with such high volume

      globals.prefs->game.gameLevel           = 1;
      globals.prefs->game.gamePlaying         = false;

      if (DeviceSupportsGrayscale())
      {
        DeviceGrayscale(grayGet,
                        &globals.prefs->config.lgray,
                        &globals.prefs->config.dgray);
      }

      // remove saved game (if any), reset to defaults
      dbID = DmFindDatabase(0, savedGameFileName);
      if (dbID != NULL) DmDeleteDatabase(0, dbID);

      globals.prefs->levelPack.type           = PACK_INTERNAL;
      MemSet(globals.prefs->levelPack.strLevelPack, 32, 0);
      globals.prefs->levelPack.useDifficult   = false;
    }
  }

  // are we running on a handera?
#if HANDERA_NATIVE
  globals.prefs->handera.device =
    (FtrGet(TRGSysFtrID, TRGVgaFtrNum, &trgVersion) != ftrErrNoSuchFeature);
  if (globals.prefs->handera.device)
  {
    VgaGetScreenMode(&globals.prefs->handera.scrMode,
                     &globals.prefs->handera.scrRotate);
  }
#endif

  // are we running on sony?
#if SONY_NATIVE
  globals.prefs->sony.device =
    (SysLibFind(sonySysLibNameHR, &globals.prefs->sony.libRef) == errNone);
  if (!globals.prefs->sony.device)
    globals.prefs->sony.device =
      (SysLibLoad('libr', sonySysFileCHRLib,
                  &globals.prefs->sony.libRef) == errNone);
#endif

  // are we running on a palm hidensity device?
#if PALM_HIDENSITY
  FtrGet(sysFtrCreator, sysFtrNumWinVersion, &winVersion);
  globals.prefs->palmHD.device = (winVersion >= 4);

  if (globals.prefs->palmHD.device)
  {
    // we must NOT allow this
#if HANDERA_NATIVE
    globals.prefs->handera.device = false;
#endif
#if SONY_NATIVE
    globals.prefs->sony.device    = false;
#endif

    // get the current display information
    WinScreenGetAttribute(winScreenWidth,  &globals.prefs->palmHD.width);
    WinScreenGetAttribute(winScreenHeight, &globals.prefs->palmHD.height);

    // which depth do we have?
    switch (globals.prefs->palmHD.width)
    {
      case 160: globals.prefs->palmHD.density = kDensityLow;         break;
      case 320: globals.prefs->palmHD.density = kDensityDouble;      break;
      default:  globals.prefs->palmHD.width   = 160;
                globals.prefs->palmHD.height  = 160;
                globals.prefs->palmHD.density = kDensityLow;         break;
    }
  }
#endif

  // lets adjust the widescreen display mode
  GameWideScreen(globals.prefs);

  // get the HotSync user name
  MemSet(&globals.system.hotSyncUsername, dlkUserNameBufSize, 0);
  DlkGetSyncInfo(NULL,NULL,NULL,
                 globals.system.hotSyncUsername,NULL,NULL);
  {
    Char *ptrStr;

    ptrStr = StrChr(globals.system.hotSyncUsername, spaceChr);
    if (ptrStr != NULL)
    {
      // erase everything after the FIRST space
      UInt8 index = ((UInt32)ptrStr -
                     (UInt32)globals.system.hotSyncUsername);
      MemSet(ptrStr, dlkUserNameBufSize - index, 0);
    }

    ptrStr = StrChr(globals.system.hotSyncUsername, '\0');
    if (ptrStr != NULL)
    {
      // erase everything after the FIRST null char
      UInt8 index = ((UInt32)ptrStr -
                     (UInt32)globals.system.hotSyncUsername);
      MemSet(ptrStr, dlkUserNameBufSize - index, 0);
    }
  }

#if PROTECTION_ON
  {
    UInt8 i, checksum;

    checksum = 0;
    for (i=0; i<MAX_IDLENGTH; i++)
      checksum += (UInt8)globals.system.hotSyncUsername[i];
    checksum &= 0xff;
    if (checksum == 0) checksum = 0x20; // cannot be zero

    if (globals.prefs->system.hotSyncChecksum != checksum)
      globals.prefs->game.gamePlaying = false;
    globals.prefs->system.hotSyncChecksum = checksum;
  }
#endif

  // configure grayscale registers
  if (DeviceSupportsGrayscale())
  {
    DeviceGrayscale(graySet,
                    &globals.prefs->config.lgray,
                    &globals.prefs->config.dgray);
  }

  // setup sound config
  DeviceSetMute(globals.prefs->config.sndMute);
  DeviceSetVolume(globals.prefs->config.sndVolume);

#if SET_KEYMASK
  // setup the valid keys available while the game is running
  KeySetMask(~(keyBitsAll ^ (keyBitPower    |
#if !DISABLE_HOTSYNC
                             keyBitCradle   | keyBitAntenna |
#endif
#if HANDERA_NATIVE
                             keyBitJogUp    | keyBitJogDown |
#endif
                             keyBitContrast | keyBitNavLRS)));
#endif

  // initialize the game environemnt
  RegisterInitialize(globals.prefs);
  LevelPackInitialize(globals.prefs);

  globals.evtTimeOut     = evtWaitForever;
  globals.ticksPerSecond = SysTicksPerSecond();
  globals.ticksPerFrame  = (globals.ticksPerSecond * 10) / GAME_FPS_x10;

#ifndef MDM_DISTRIBUTION
  // must we show that this application is for demo purposes
  if (globals.prefs->system.showNotice)
    ApplicationDisplayDialog(demoForm);
#endif

  // notification registration for external media card removal
  if (FtrGet(sysFtrCreator, sysFtrNumNotifyMgrVersion, &version) == errNone)
  {
    SysCurAppDatabase(&card, &dbID);

    // mmc insert/removal notification
    SysNotifyRegister(card, dbID, sysNotifyVolumeMountedEvent,
                      notifyCardMounted, sysNotifyNormalPriority, NULL);
    SysNotifyRegister(card, dbID, sysNotifyCardRemovedEvent,
                      notifyCardRemoval, sysNotifyNormalPriority, NULL);

    // device sleep/wakeup notification
    SysNotifyRegister(card, dbID, sysNotifySleepRequestEvent,
                      notifySleepRequest, sysNotifyNormalPriority, NULL);
  }

  // goto the appropriate form
  if (LevelPackExists(globals.prefs->levelPack.type, globals.prefs->levelPack.strLevelPack))
  {
    if ((globals.prefs->game.gamePlaying) &&
        (DmFindDatabase(0, savedGameFileName) != NULL))
    {
      if (globals.prefs->game.gameState == GAME_PLAY)
        globals.prefs->game.gamePaused = true;
      FrmGotoForm(gameForm);
    }
    else
      FrmGotoForm(mainForm);
  }
  else
  {
    LocalID dbID;

    dbID = DmFindDatabase(0, savedGameFileName);
    if (dbID != NULL) DmDeleteDatabase(0, dbID); // remove saved game

    globals.prefs->game.gameLevel           = 1;
    globals.prefs->game.gamePlaying         = false;

    globals.prefs->levelPack.type = PACK_INTERNAL;
    MemSet(globals.prefs->levelPack.strLevelPack, 32, 0);

    FrmGotoForm(mainForm);
  }
}

/**
 * The Palm Computing Platform entry routine (mainline).
 *
 * @param cmd         a word value specifying the launch code.
 * @param cmdPBP      pointer to a structure associated with the launch code.
 * @param launchFlags additional launch flags.
 * @return zero if launch successful, non zero otherwise.
 */
UInt32
PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
  const RectangleType     rect  = {{0,0},{160,160}};
  Boolean   ok;
  UInt32    result = 0;
  MemHandle bitmapHandle;
  Char      str[32];

  // what type of launch was this?
  switch (cmd)
  {
    case sysAppLaunchCmdNormalLaunch:
         {
           // is this device compatable?
           if (DeviceCheckCompatability())
           {
             void *osRAMRequirement;

             // initialize device + globals
             DeviceInitialize();
             GlobalsInitialize();

             WinSetPattern(&erase);
             WinFillRectangle(&rect,0);

             // splash screen *g*
             bitmapHandle = DmGetResource('Tbmp', bitmapLogo);
             WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), 0, 16);
             MemHandleUnlock(bitmapHandle);
             DmReleaseResource(bitmapHandle);

             // draw separators
             WinDrawLine(   0,  13, 159,  13);
             WinDrawLine(   0,  14, 159,  14);
             WinDrawLine(   0, 145, 159, 145);
             WinDrawLine(   0, 146, 159, 146);

             FntSetFont(boldFont);
             StrCopy(str, " -    L O A D I N G    - ");
             WinDrawChars(str, StrLen(str),
                          ((160 - FntCharsWidth(str, StrLen(str))) >> 1), 148);
             FntSetFont(stdFont);

             ok  = GraphicsInitialize();
             ok &= GameInitialize();

             osRAMRequirement = MemPtrNew(1024); // we need at least 1024 bytes
             ok &= (osRAMRequirement != NULL);
             MemPtrFree(osRAMRequirement);

             if (ok)
             {
               // initialize
               InitApplication();

               // run the application :))
               EventLoop();
               EndApplication();
             }

             // terminate the graphics and game engine
             GameTerminate();
             GraphicsTerminate();

             // must tell user no memory left :(
             if (!ok)
               ApplicationDisplayDialog(xmemForm);

             // restore device state
             GlobalsTerminate();
             DeviceTerminate();
           }
         }
         break;

    default:
         break;
  }

  return result;
}

/**
 * The application event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
Boolean
ApplicationHandleEvent(EventType *event)
{
  Boolean processed = false;

  switch (event->eType)
  {
    case frmLoadEvent:
         {
           UInt16   formID = event->data.frmLoad.formID;
           FormType *frm   = FrmInitForm(formID);

           FrmSetActiveForm(frm);
           switch (formID)
           {
             case mainForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)mainFormEventHandler);

                  processed = true;
                  break;

             case gameForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)gameFormEventHandler);

                  processed = true;
                  break;

             case infoForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)infoFormEventHandler);

                  processed = true;
                  break;

             case dvlpForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)dvlpFormEventHandler);

                  processed = true;
                  break;

#ifndef MDM_DISTRIBUTION
             case demoForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)demoFormEventHandler);

                  processed = true;
                  break;
#endif

             case cfigForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)cfigFormEventHandler);

                  processed = true;
                  break;

             case grayForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)grayFormEventHandler);

                  processed = true;
                  break;

             case helpForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)helpFormEventHandler);

                  processed = true;
                  break;

             case xmemForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)xmemFormEventHandler);

                  processed = true;
                  break;

             case prepForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)prepFormEventHandler);

                  processed = true;
                  break;

             case verdForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)verdFormEventHandler);

                  processed = true;
                  break;

             case regiForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)regiFormEventHandler);

                  processed = true;
                  break;

             case packForm:
                  FrmSetEventHandler(frm,
                    (FormEventHandlerPtr)packFormEventHandler);

                  processed = true;
                  break;

             default:
                  break;
           }
         }
         break;

    case winEnterEvent:
         {
           if (event->data.winEnter.enterWindow ==
                (WinHandle)FrmGetFormPtr(gameForm))
           {
             // when game screen is active, animate
             globals.evtTimeOut           = 1;
             globals.timerLastFrameUpdate = TimGetTicks() - globals.ticksPerFrame;
             processed                    = true;

#if MIDI_PAUSE_ON_DIALOG
             // if game is not paused, restart the music
             if (!globals.prefs->game.gamePaused)
               GameMusicPause(globals.prefs, false);
#endif
           }
         }
         break;

    case winExitEvent:
         {
           if (event->data.winExit.exitWindow ==
                (WinHandle)FrmGetFormPtr(gameForm))
           {
             SndCommandType muteSnd = { sndCmdQuiet, 0, 0, 0, 0 };

             // when game screen is not active, stop animation
             globals.evtTimeOut           = evtWaitForever;
             globals.timerLastFrameUpdate = TimGetTicks() - globals.ticksPerFrame;

             // mute all sounds
#if MIDI_PAUSE_ON_DIALOG
             GameMusicPause(globals.prefs, true);
#endif
             DevicePlaySound(&muteSnd);

             processed           = true;
           }
         }
         break;

    case penUpEvent:
    case penDownEvent:
    case penMoveEvent:
         {
           // we have to handle this case specially for the gameForm :))
           if (WinGetActiveWindow() == (WinHandle)FrmGetFormPtr(gameForm))
           {
             processed = gameFormEventHandler(event);
           }
         }
         break;

    case keyDownEvent:
         {
           // we have to handle this case specially for the gameForm :))
           if (WinGetActiveWindow() == (WinHandle)FrmGetFormPtr(gameForm))
           {
             processed = gameFormEventHandler(event);
           }
         }
         break;

    case menuEvent:

         switch (event->data.menu.itemID)
         {
           case menuItemGrayscale:
                ApplicationDisplayDialog(grayForm);
                processed = true;
                break;

           case menuItemConfig:
                ApplicationDisplayDialog(cfigForm);
                processed = true;
                break;

           case menuItemRegister:
                ApplicationDisplayDialog(regiForm);
                RegisterShowMessage(globals.prefs);
                processed = true;
                break;

#ifndef MDM_DISTRIBUTION
           case menuItemDemo:
                ApplicationDisplayDialog(demoForm);
                processed = true;
                break;
#endif

           case menuItemHelp:
                ApplicationDisplayDialog(helpForm);
                processed = true;
                break;

           case menuItemDeveloper:
                ApplicationDisplayDialog(dvlpForm);
                processed = true;
                break;

           case menuItemAbout:
                ApplicationDisplayDialog(infoForm);
                processed = true;
                break;

           default:
                break;
         }
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case globalFormHelpButton:

                // regenerate menu event
                MemSet(event, sizeof(EventType), 0);
                event->eType            = menuEvent;
                event->data.menu.itemID = menuItemHelp;
                EvtAddEventToQueue(event);

                processed = true;
                break;

           case globalFormAboutButton:

                // regenerate menu event
                MemSet(event, sizeof(EventType), 0);
                event->eType            = menuEvent;
                event->data.menu.itemID = menuItemAbout;
                EvtAddEventToQueue(event);

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * Display a MODAL dialog to the user (this is a modified FrmDoDialog)
 *
 * @param formID the ID of the form to display.
 */
void
ApplicationDisplayDialog(UInt16 formID)
{
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
  const RectangleType     rect  = {{0,0},{160,160}};
  FormActiveStateType frmCurrState;
  FormType            *frmActive      = NULL;
  WinHandle           winDrawWindow   = NULL;
  WinHandle           winActiveWindow = NULL;
  MemHandle           memHandle;
  RGBColorType        currPalette[16];
  Boolean             gameActive;
  UInt16              activeFormID;

  EventType           event;
  UInt16              err;
  Boolean             keepFormOpen;

  activeFormID = FrmGetActiveFormID();
  gameActive   = (activeFormID == gameForm);

#if HANDERA_NATIVE
  // force handera into 160 -> 240 stretch and 1to1 for bitmaps
  if ((formID != xmemForm) && (globals.prefs->handera.device))
    VgaSetScreenMode(screenModeScaleToFit, rotateModeNone);
#endif

  // save the active form/window
  if (DeviceSupportsVersion(romVersion3))
    FrmSaveActiveState(&frmCurrState);
  else
  {
    frmActive       = FrmGetActiveForm();
    winDrawWindow   = WinGetDrawWindow();
    winActiveWindow = WinGetActiveWindow();  // < palmos3.0, manual work
  }

  // clear the LCD screen (dont want palette flash)
  WinSetDrawWindow(WinGetDisplayWindow());
  WinSetPattern(&erase);
  WinFillRectangle(&rect, 0);

  // and tweak a VGA 16 color palette
  if ((DeviceSupportsColor()) &&
      ((formID != prepForm) && (formID != verdForm)))
  {
    // save the current palette
    WinPalette(winPaletteGet, 0, 16, currPalette);

    // helpForm: we need to ensure it is in the gamebase palette
    if (formID == helpForm)
    {
      memHandle = DmGetResource('PALT', paletteGameBase);
      WinPalette(winPaletteSet, 0, 16,
                 (RGBColorType *)MemHandleLock(memHandle));
      MemHandleUnlock(memHandle);
      DmReleaseResource(memHandle);
    }

    // all others? revert back to 16 default
    else
    {
      memHandle = DmGetResource('PALT', paletteVGA16);
      WinPalette(winPaletteSet, 0, 16,
                 (RGBColorType *)MemHandleLock(memHandle));
      MemHandleUnlock(memHandle);
      DmReleaseResource(memHandle);
    }
  }

  // send a load form event
  MemSet(&event, sizeof(EventType), 0);
  event.eType = frmLoadEvent;
  event.data.frmLoad.formID = formID;
  EvtAddEventToQueue(&event);

  // send a open form event
  MemSet(&event, sizeof(EventType), 0);
  event.eType = frmOpenEvent;
  event.data.frmLoad.formID = formID;
  EvtAddEventToQueue(&event);

  // handle all events here (trap them before the OS does) :)
  keepFormOpen = true;
  while (keepFormOpen)
  {
    EvtGetEvent(&event, globals.evtTimeOut);

    // these are our exit conditions! :)
    keepFormOpen = (event.eType != frmCloseEvent);
    if (!keepFormOpen)
    {
      // restore the old palette
      if ((DeviceSupportsColor()) &&
          ((formID != prepForm) && (formID != verdForm)))
      {
        // clear the LCD screen (dont want palette flash)
        WinSetDrawWindow(WinGetDisplayWindow());
        WinSetPattern(&erase);
        WinFillRectangle(&rect, 0);

        // restore the original palette
        WinPalette(winPaletteSet, 0, 16, currPalette);
      }
    }

    if (!SysHandleEvent(&event))
      if (!MenuHandleEvent(0, &event, &err))
        if (!ApplicationHandleEvent(&event))
          FrmDispatchEvent(&event);

    if (event.eType == appStopEvent)
    {
      keepFormOpen = false;
      EvtAddEventToQueue(&event);  // tap "applications", need to exit
    }
  }

  // restore the active form/window
  if (DeviceSupportsVersion(romVersion3))
    FrmRestoreActiveState(&frmCurrState);
  else
  {
    FrmSetActiveForm(frmActive);
    WinSetDrawWindow(winDrawWindow);
    WinSetActiveWindow(winActiveWindow);     // < palmos3.0, manual work
  }

#if HANDERA_NATIVE
  // force handera into 160 -> 240 stretch and 1to1 for bitmaps
  if ((formID != xmemForm) &&
      (globals.prefs->handera.device) && (gameActive))
    VgaSetScreenMode(screenMode1To1, rotateModeNone);
#endif

  // post an "update" event for the currently active form
  if (formID != verdForm)
  {
    MemSet(&event, sizeof(EventType), 0);
    event.eType = appUpdateEvent;
    EvtAddEventToQueue(&event);
  }
}

/**
 * The Palm Computing Platform event processing loop.
 */
void
EventLoop()
{
  EventType event;
  FormType  *frm;
  UInt16    err;

  // reset the timer (just in case)
  globals.timerLastFrameUpdate = TimGetTicks() - globals.ticksPerFrame;

  do
  {
#ifdef PALM_MIDI_YAMAHAPA1
    EvtGetEvent(&event, (globals.evtTimeOut == evtWaitForever) ? evtWaitForever : 0);
    GameMusicEventCallback();
#else
    EvtGetEvent(&event, globals.evtTimeOut);
#endif
    frm = FrmGetActiveForm();

    //
    // EVENT HANDLING:
    //

    if (!ApplicationHandleEvent(&event))
      if (!SysHandleEvent(&event))
        if (!MenuHandleEvent(0, &event, &err))
          FrmDispatchEvent(&event);

    //
    // ANIMATION HANDLING:
    //

    // on a form that requires animations, calc time since last nilEvent
    if (
        (WinGetActiveWindow() == (WinHandle)frm) &&
        (frm == FrmGetFormPtr(gameForm))
       )
    {
      globals.timerPoint = TimGetTicks();

      // calculate the delay required
#if SHOW_FPS
      globals.frameCount++;
      if ((globals.timerReference == -1) ||
          ((globals.timerPoint -
            globals.timerReference) > globals.ticksPerSecond))
      {
        Char str[32];
        StrPrintF(str, "  %d FPS  ", globals.frameCount);
        WinSetDrawWindow(WinGetDisplayWindow());
        WinDrawChars(str, StrLen(str), 80, 1);

        globals.timerReference = globals.timerPoint;
        globals.frameCount     = 0;
      }
      globals.evtTimeOut = 1;
#else
      globals.timerDiff = (globals.timerPoint - globals.timerLastFrameUpdate);
      globals.evtTimeOut = (globals.timerDiff > globals.ticksPerFrame) ?
        1 : (globals.ticksPerFrame - globals.timerDiff);
#endif

      // manually add nilEvent if needed (only when pen held down)
      if ((globals.evtTimeOut <= 1) && (event.eType == penMoveEvent))
      {
        EventType event;

        // lets flush the pen events from the queue (jebus, holding it down)
        EvtFlushPenQueue();

        // insert a nilEvent for renderering :)
        MemSet(&event, sizeof(EventType), 0);
        event.eType = nilEvent;
        EvtAddEventToQueue(&event);
      }
    }

  } while (event.eType != appStopEvent);
}

/**
 * The Palm Computing Platform termination routine.
 */
void
EndApplication()
{
  MemHandle keyH;
  UInt16    prefSize;
  UInt16    card;
  LocalID   dbID;
  UInt32    version;

#if SET_KEYMASK
  // restore the key state
  KeySetMask(keyBitsAll);
#endif

  // terminate the game environemnt
  LevelPackTerminate();
  RegisterTerminate();

  // ensure all forms are closed
  FrmCloseAllForms();

  // notification unregistration
  if (FtrGet(sysFtrCreator, sysFtrNumNotifyMgrVersion, &version) == errNone)
  {
    SysCurAppDatabase(&card, &dbID);

    // mmc insert/removal notification
    SysNotifyUnregister(card, dbID, sysNotifyCardInsertedEvent, sysNotifyNormalPriority);
    SysNotifyUnregister(card, dbID, sysNotifyCardRemovedEvent, sysNotifyNormalPriority);

    // device sleep/wakeup notification
    SysNotifyUnregister(card, dbID, sysNotifySleepRequestEvent, sysNotifyNormalPriority);
  }

  // lets add our 'check' data chunk
  prefSize = sizeof(PreferencesType);
  StrCopy(globals.prefs->system.signature, "|HaCkMe|");

  // decrypt the memory chunk (based on the mesg0000.bin)
  keyH = DmGetResource('mesg', 0x0000);
  RegisterDecryptChunk((UInt8 *)globals.prefs, prefSize, keyH, 0x01);
  DmReleaseResource(keyH);
  PrefSetAppPreferences(appCreator, 0, 1, globals.prefs, prefSize, true);
}
