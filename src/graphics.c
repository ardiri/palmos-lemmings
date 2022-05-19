/*
 * @(#)graphics.c
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

// interface
static void GraphicsClear_blt();
static void GraphicsRepaint_blt();
static void GraphicsClear_api();
static void GraphicsRepaint_api();
#if HANDERA_NATIVE
static void GraphicsRepaint_handera();
static void GraphicsRepaint_handeraHiRes();
#endif
#if SONY_NATIVE
static void GraphicsRepaint_sony();
static void GraphicsRepaint_sonyHiRes();
#endif

// global variable structure
typedef struct
{
  UInt32       scrDepth;
  Boolean      m68k_device;
  
  WinHandle    drawWindow;
  BitmapType   *drawBitmap;
  BitmapTypeV3 *drawBitmapV3;
  UInt8        *ptrWinDraw;
  UInt8        *ptrWinLCD;

  UInt16       xOffset;             // the xOffset to start blitting from
  Boolean      widescreen;
#if !FULL_SCREEN_BLIT
  UInt16       yStart;              // the start of the y-band to draw 
  UInt16       yEnd;                // the end of the y-band to draw
#endif

#if HANDERA_NATIVE
  struct
  {
    Boolean    device;
    UInt32     trgVersion;
  } handera;
#endif
#if SONY_NATIVE
  struct
  {
    Boolean    device;
    UInt16     libRef;
  } sony;
#endif
#if PALM_HIDENSITY
  struct
  {
    Boolean    device;              // are we running on palm hi-density device?

    UInt32     winVersion;    
    UInt32     width;
    UInt32     height;
    UInt16     density;             // display properties
  } palmHD;
#endif

  void         (*fnClear)(void);
  void         (*fnRepaint)(void);

} GraphicsGlobals;

static GraphicsGlobals globals;

/**
 * Initialize the Graphics engine.
 *
 * @return true if the graphics initialization was successful, false otherwise
 */  
Boolean   
GraphicsInitialize()
{
  Boolean result = true;
  UInt16  err;

  // clear the globals object
  MemSet(&globals, sizeof(GraphicsGlobals), 0);

  // determine the screen depth
  globals.scrDepth   = 1;
  if (DeviceSupportsVersion(romVersion3)) 
    WinScreenMode(winScreenModeGet,NULL,NULL,&globals.scrDepth,NULL);

  // are we running on handera?
#if HANDERA_NATIVE
  globals.handera.device =
    (FtrGet(TRGSysFtrID, TRGVgaFtrNum, 
            &globals.handera.trgVersion) != ftrErrNoSuchFeature);
#endif

  // are we running on sony?
#if SONY_NATIVE
  err = SysLibFind(sonySysLibNameHR, &globals.sony.libRef);
  if (err != errNone)
    err = SysLibLoad('libr', sonySysFileCHRLib, &globals.sony.libRef);
  globals.sony.device = (err == errNone);
#endif

  // are we running on a palm hidensity device?
#if PALM_HIDENSITY
  FtrGet(sysFtrCreator, sysFtrNumWinVersion, &globals.palmHD.winVersion);
  globals.palmHD.device = (globals.palmHD.winVersion >= 4);

  if (globals.palmHD.device)
  {
    // we must NOT allow this
#if HANDERA_NATIVE
    globals.handera.device = false;
#endif
#if SONY_NATIVE
    globals.sony.device    = false;
#endif

    // get the current display information
    WinScreenGetAttribute(winScreenWidth,  &globals.palmHD.width);
    WinScreenGetAttribute(winScreenHeight, &globals.palmHD.height);

    // which depth do we have?
    switch (globals.palmHD.width)
    {
      case 160: globals.palmHD.density = kDensityLow;         break;
      case 320: globals.palmHD.density = kDensityDouble;      break;
      default:  globals.palmHD.width   = 160;
                globals.palmHD.height  = 160; 
                globals.palmHD.density = kDensityLow;         break;
    }
  }
#endif  

  // determine screen size (only >= 3.5)
  globals.m68k_device = true;
  if (DeviceSupportsVersion(romVersion3_5)) 
  {
    UInt32 cpuID;

    // lets make sure we are running on a "m68k chip" :) - blt routines work
    FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
    globals.m68k_device &= ((cpuID == sysFtrNumProcessor328)  ||
                            (cpuID == sysFtrNumProcessorEZ)  ||
                            (cpuID == sysFtrNumProcessorVZ)  ||
                            (cpuID == sysFtrNumProcessorSuperVZ));
  }

  // create the offscreen window [+SPR_HEIGHT*3 = no checking (above+below)]
  if (DeviceSupportsVersion(romVersion3_5)) 
  {
    globals.drawBitmap = 
      BmpCreate(OFFSCREEN_WIDTH, (OFFSCREEN_HEIGHT + ((SPR_HEIGHT*2)+1)), 
                globals.scrDepth, NULL, &err);
    if (err == errNone) 
    {
      // high density device
      if (globals.palmHD.device)
      {
        globals.drawBitmapV3 = 
          BmpCreateBitmapV3(globals.drawBitmap,
                            globals.palmHD.density, 
                            BmpGetBits(globals.drawBitmap), NULL);
        err = (globals.drawBitmapV3 == NULL);
        if (err == errNone)
          globals.drawWindow = 
            WinCreateBitmapWindow((BitmapType *)globals.drawBitmapV3, &err);  	
      }  
      else
        globals.drawWindow = WinCreateBitmapWindow(globals.drawBitmap, &err);  
    }
  }
  else
    globals.drawWindow = 
      WinCreateOffscreenWindow(OFFSCREEN_WIDTH, 
                               (OFFSCREEN_HEIGHT + ((SPR_HEIGHT*2)+1)), 
                               screenFormat, &err);
  err |= (Boolean)(globals.drawWindow == NULL);

  globals.xOffset = (OFFSCREEN_WIDTH - SCREEN_WIDTH) >> 1;
#if !FULL_SCREEN_BLIT
  globals.yStart  = 0;
  globals.yEnd    = SCREEN_HEIGHT-1;
#endif

  // nothing went wrong? configure pointers
  result = (err == errNone);
  if (err == errNone) 
  {
    // standard 160x160 units :)
    globals.fnClear     = (globals.m68k_device) 
                           ? (void *)GraphicsClear_blt
                           : (void *)GraphicsClear_api; 
    globals.fnRepaint   = (globals.m68k_device) 
                           ? (void *)GraphicsRepaint_blt 
                           : (void *)GraphicsRepaint_api; 

    // we use this a lot, lets store it *g* [optimization]
    globals.ptrWinDraw = DeviceWindowGetPointer(globals.drawWindow);
    globals.ptrWinLCD  = DeviceWindowGetPointer(WinGetDisplayWindow());
  }

  return result;
}

/**
 * Adjust widescreen display mode property.
 * 
 * @param widescreen enable widescreen display.
 */ 
void
GraphicsWideScreen(Boolean widescreen)
{
  globals.widescreen = widescreen;
  
  // standard 160x160 units :)
  globals.fnClear     = (globals.m68k_device) 
                         ? (void *)GraphicsClear_blt
                         : (void *)GraphicsClear_api; 
  globals.fnRepaint   = (globals.m68k_device) 
                         ? (void *)GraphicsRepaint_blt 
                         : (void *)GraphicsRepaint_api; 

  // special units [custom blitting]
#if HANDERA_NATIVE
  if (globals.handera.device) 
  {
    if (widescreen)
      globals.fnRepaint = (void *)GraphicsRepaint_handeraHiRes;
    else
      globals.fnRepaint = (void *)GraphicsRepaint_handera;
  }
#endif
#if SONY_NATIVE
  if (globals.sony.device) 
  {
    if (widescreen)
      globals.fnRepaint = (void *)GraphicsRepaint_sonyHiRes;
    else
      globals.fnRepaint = (void *)GraphicsRepaint_sony;
  }
#endif

#if PALM_HIDENSITY
  if (globals.palmHD.device) 
  {  
    // Tungsten|W specifically (we can re-use SONY routines)
    if (globals.m68k_device)
    {
      if (widescreen)
        globals.fnRepaint = (void *)GraphicsRepaint_sonyHiRes;
      else
        globals.fnRepaint = (void *)GraphicsRepaint_sony;
    } 
    
    // set the new density for the drawing bitmap 
    else
    { 	
      BmpSetDensity((BitmapType *)globals.drawBitmapV3, 
                    widescreen ? globals.palmHD.density : kDensityLow);
    }              
  }
#endif
}

/**
 * Get the draw window.
 *
 * @return the draw window.
 */
WinHandle
GraphicsGetDrawWindow()
{
  return globals.drawWindow;
}

/**
 * Clear the offscreen image.
 */
void
GraphicsClear()
{
  // execute the appropriate function
  globals.fnClear();
}

/**
 * Set the x-offset for display blitting.
 *
 * @param x the new x-offset.
 */
void
GraphicsSetOffset(Int16 x)
{
  // dont overrun!
  if (x < 0) 
    x = 0; 
  else 
  if (x > (OFFSCREEN_WIDTH - SCREEN_WIDTH)) 
    x = OFFSCREEN_WIDTH - SCREEN_WIDTH;

  globals.xOffset = x;
}

/**
 * Get the x-offset for display blitting.
 *
 * @return the x-offset.
 */
UInt16
GraphicsGetOffset()
{
  return globals.xOffset;
}

#if !FULL_SCREEN_BLIT
/**
 * Set the top and bottom update lines.
 *
 * @param y1 top update line.
 * @param y2 bottom update line.
 */
void
GraphicsSetUpdate(Int16 y1, Int16 y2)
{
  // dont overrun!
  if (y1 < 0) y1 = 0; else if (y1 >= SCREEN_HEIGHT) y1 = SCREEN_HEIGHT-1;
  if (y2 < 0) y2 = 0; else if (y2 >= SCREEN_HEIGHT) y2 = SCREEN_HEIGHT-1;

  // y1 must be less than y2
  if (y1 <= y2)
  {
    globals.yStart = y1;
    globals.yEnd   = y2;
  }
  else
  {
    globals.yStart = 0;
    globals.yEnd   = 0;   // dont draw anything :)
  }
}
#endif

/**
 * Blit the offscreen image to the screen.
 */
void
GraphicsRepaint()
{
  // execute the appropriate function
  globals.fnRepaint();
}

#if SHOW_Y_UPDATE_BARS
/**
 * Blit the offscreen image to the screen (showing y1-y2 bars)
 */
void
GraphicsRepaintShowBars()
{
  RectangleType rect = {{0,SPR_HEIGHT},{8,SCREEN_HEIGHT}};

  // repaint the screen as per normal
  GraphicsRepaint();
  
  // draw the update bars
  rect.topLeft.x = globals.xOffset;

  // copy the backbuffer to the screen
  WinCopyRectangle(globals.drawWindow, WinGetDisplayWindow(),
                   &rect, 0, SCREEN_START, winPaint);

  if (DeviceSupportsColor())
  {
    WinEraseLine(0, globals.yStart+SCREEN_START, 
                 4, globals.yStart+SCREEN_START);
    WinEraseLine(0, globals.yEnd+SCREEN_START, 
                 4, globals.yEnd+SCREEN_START);
  }
  else
  {
    WinDrawLine(0, globals.yStart+SCREEN_START, 
                4, globals.yStart+SCREEN_START);
    WinDrawLine(0, globals.yEnd+SCREEN_START, 
                4, globals.yEnd+SCREEN_START);
  }
}
#endif

/**
 * Terminate the Graphics engine.
 */
void   
GraphicsTerminate()
{
  // clean up windows/memory
  if (DeviceSupportsVersion(romVersion3_5)) 
  {
    if (globals.drawBitmap   != NULL) BmpDelete(globals.drawBitmap);
    if (globals.drawBitmapV3 != NULL) BmpDelete((BitmapType *)globals.drawBitmapV3);
  }	
  if (globals.drawWindow != NULL) WinDeleteWindow(globals.drawWindow, false);
}

// 
// 160x160 direct screen display routines [speed on 160x160 devices]
//
// -- Aaron Ardiri, 2000
//

/**
 * Clear the offscreen image.
 */
static void
GraphicsClear_blt()
{
  UInt16 ptrSize = 0;
  UInt8  *ptrScreen;
    
  ptrScreen = globals.ptrWinDraw;

  // how much memory is being used?
  if (globals.scrDepth == 2) 
  {
    ptrSize  = ((UInt16)OFFSCREEN_HEIGHT * 160);
    ptrSize += ((SPR_HEIGHT*2)+1) * 160; 
  }
  else
  if (globals.scrDepth == 4) 
  {
    ptrSize  = ((UInt16)OFFSCREEN_HEIGHT * 320);
    ptrSize += ((SPR_HEIGHT*2)+1) * 320; 
  } 
    
  // clear the memory
  MemSet(ptrScreen, ptrSize, 0);
}

/**
 * Blit the offscreen image to the screen.
 */
static void
GraphicsRepaint_blt()
{
  UInt8  *ptrLCDScreen, *ptrScreen;
  Int16  loop;
#if PORTABLE
  UInt16 i, j;
#endif

  ptrLCDScreen = globals.ptrWinLCD;
  ptrScreen    = globals.ptrWinDraw;

  // 2bpp
  if (globals.scrDepth == 2)
  {
    ptrLCDScreen += (SCREEN_WIDTH_GENERIC * SCREEN_START_GENERIC) >> 2;

//  ptrScreen    += (globals.xOffset / 4);
    ptrScreen    += (globals.xOffset >> 2);
#if FULL_SCREEN_BLIT
    ptrScreen    += ((SPR_HEIGHT << 5) * 5);
    loop          = SCREEN_HEIGHT-1;
#else
//  ptrScreen    += ((globals.yStart + SPR_HEIGHT) * 160);
//  ptrLCDScreen += (globals.yStart  * 40);
    ptrScreen    += (((globals.yStart + SPR_HEIGHT) << 5) * 5);
    ptrLCDScreen += ((globals.yStart << 3) * 5);
    loop          = (globals.yEnd - globals.yStart);
    if (loop == 0) goto REPAINT_DONE;
#endif

#if PORTABLE
    j = loop + 1;
    do
    {
      i = (SCREEN_WIDTH_GENERIC >> 2);
      do
      {
        *ptrLCDScreen++ = *ptrScreen++;
      }
      while (--i);

      ptrScreen += 160 - (SCREEN_WIDTH_GENERIC >> 2);
    }
    while (--j);
#else
    // push all registers (except a7) on stack
    asm("movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)" : : );

    // copy inner 160x128 from back buffer to screen
    asm("move.l  %0, %%a0" : : "g" (ptrScreen));
    asm("move.l  %0, %%a1" : : "g" (ptrLCDScreen));
    asm("move.l  %0, %%d0" : : "g" (loop));
    asm("
blt_ScrLoop:
         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, (%%a1)

         lea     40(%%a1), %%a1                      
         lea     120(%%a0), %%a0                 | blit one row!!

         dbra    %%d0, blt_ScrLoop
    " : :);

    // pop all registers (except a7) off stack
    asm("movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6" : : );
#endif
  }

  // 4bpc
  else
  if (globals.scrDepth == 4)
  {
    ptrLCDScreen += (SCREEN_WIDTH_GENERIC * SCREEN_START_GENERIC) >> 1;

//  ptrScreen    += (globals.xOffset / 2);
    ptrScreen    += (globals.xOffset >> 1);
#if FULL_SCREEN_BLIT
    ptrScreen    += ((SPR_HEIGHT << 6) * 5);
    loop          = SCREEN_HEIGHT-1;
#else
//  ptrScreen    += ((globals.yStart + SPR_HEIGHT) * 320);
//  ptrLCDScreen += (globals.yStart  * 80);
    ptrScreen    += (((globals.yStart + SPR_HEIGHT) << 6) * 5);
    ptrLCDScreen += ((globals.yStart << 4) * 5);
    loop          = (globals.yEnd - globals.yStart);
    if (loop == 0) goto REPAINT_DONE;
#endif

#if PORTABLE
    j = loop + 1;
    do
    {
      i = (SCREEN_WIDTH_GENERIC >> 1);
      do
        *ptrLCDScreen++ = *ptrScreen++;
      while (--i);

      ptrScreen += 320 - (SCREEN_WIDTH_GENERIC >> 1);
    }
    while (--j);
#else
    // push all registers (except a7) on stack
    asm("movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)" : : );

    // copy inner 160x128 from back buffer to screen
    asm("move.l  %0, %%a0" : : "g" (ptrScreen));
    asm("move.l  %0, %%a1" : : "g" (ptrLCDScreen));
    asm("move.l  %0, %%d0" : : "g" (loop));
    asm("
blt_ScrLoop2:
         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, (%%a1)

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, 40(%%a1)

         lea     80(%%a1), %%a1                      
         lea     240(%%a0), %%a0                 | blit one row!!

         dbra    %%d0, blt_ScrLoop2
    " : :);

    // pop all registers (except a7) off stack
    asm("movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6" : : );
#endif
  }

#if !FULL_SCREEN_BLIT
REPAINT_DONE:
#endif
}

// 
// 160x160 API call display routines [for future compatability]
//
// -- Aaron Ardiri, 2000
//

/**
 * Clear the offscreen image.
 */
static void
GraphicsClear_api()
{
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
  const RectangleType     rect  = 
    {{0,0},{OFFSCREEN_WIDTH,OFFSCREEN_HEIGHT+((SPR_HEIGHT*2)+1)}};
  WinHandle currWindow;
  
  // clear the buffer
  currWindow = WinSetDrawWindow(globals.drawWindow);
  WinSetPattern(&erase);
  WinFillRectangle(&rect, 0);
  WinSetDrawWindow(currWindow);
}

/**
 * Blit the offscreen image to the screen.
 */
static void
GraphicsRepaint_api()
{
  RectangleType rect     = {{0,0},{0,SCREEN_HEIGHT}};
  
  // must set this manually [variable]
  rect.topLeft.x = globals.xOffset;  
  rect.extent.x  = SCREEN_WIDTH;  
#if FULL_SCREEN_BLIT
  rect.topLeft.y = SPR_HEIGHT;
#else    
  rect.topLeft.y = globals.yStart + SPR_HEIGHT;
  rect.extent.y  = (globals.yEnd - (globals.yStart + SPR_HEIGHT)) + 1;
#endif    

#if PALM_HIDENSITY
  if ((globals.palmHD.device) && (globals.widescreen))
  {
    switch (globals.palmHD.density)
    {
      case kDensityDouble:
           rect.topLeft.x >>= 1;
           rect.topLeft.y >>= 1;
           rect.extent.x  >>= 1;
           rect.extent.y  >>= 1;  // remember, 2:1 - we gotta tweak here
           break;

      default:
           break;
    }
  }
#endif

#if FULL_SCREEN_BLIT
  // copy the backbuffer to the screen
  WinCopyRectangle(globals.drawWindow, WinGetDisplayWindow(),
                   &rect, 0, SCREEN_START, winPaint);
#else
  WinCopyRectangle(globals.drawWindow, WinGetDisplayWindow(),
                   &rect, 0, globals.yStart+SCREEN_START, winPaint);
#endif
}

// 
// 240x240 handera call display routines 
//
// -- Aaron Ardiri, 2001
//

#if HANDERA_NATIVE

// pixel scretching tables for 2bpp
static UInt8 handeraTable2bpp_1[256] = 
{
  0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 
  0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 
  0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 
  0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 
  0x18, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x19, 
  0x1A, 0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1B, 
  0x1C, 0x1C, 0x1C, 0x1C, 0x1D, 0x1D, 0x1D, 0x1D,
  0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 
  0x40, 0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x41, 
  0x42, 0x42, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43, 
  0x54, 0x54, 0x54, 0x54, 0x55, 0x55, 0x55, 0x55, 
  0x56, 0x56, 0x56, 0x56, 0x57, 0x57, 0x57, 0x57, 
  0x58, 0x58, 0x58, 0x58, 0x59, 0x59, 0x59, 0x59, 
  0x5A, 0x5A, 0x5A, 0x5A, 0x5B, 0x5B, 0x5B, 0x5B, 
  0x6C, 0x6C, 0x6C, 0x6C, 0x6D, 0x6D, 0x6D, 0x6D, 
  0x6E, 0x6E, 0x6E, 0x6E, 0x6F, 0x6F, 0x6F, 0x6F, 
  0x90, 0x90, 0x90, 0x90, 0x91, 0x91, 0x91, 0x91, 
  0x92, 0x92, 0x92, 0x92, 0x93, 0x93, 0x93, 0x93, 
  0x94, 0x94, 0x94, 0x94, 0x95, 0x95, 0x95, 0x95, 
  0x96, 0x96, 0x96, 0x96, 0x97, 0x97, 0x97, 0x97, 
  0xA8, 0xA8, 0xA8, 0xA8, 0xA9, 0xA9, 0xA9, 0xA9, 
  0xAA, 0xAA, 0xAA, 0xAA, 0xAB, 0xAB, 0xAB, 0xAB, 
  0xAC, 0xAC, 0xAC, 0xAC, 0xAD, 0xAD, 0xAD, 0xAD, 
  0xAE, 0xAE, 0xAE, 0xAE, 0xAF, 0xAF, 0xAF, 0xAF, 
  0xD0, 0xD0, 0xD0, 0xD0, 0xD1, 0xD1, 0xD1, 0xD1, 
  0xD2, 0xD2, 0xD2, 0xD2, 0xD3, 0xD3, 0xD3, 0xD3, 
  0xE4, 0xE4, 0xE4, 0xE4, 0xE5, 0xE5, 0xE5, 0xE5, 
  0xE6, 0xE6, 0xE6, 0xE6, 0xE7, 0xE7, 0xE7, 0xE7, 
  0xE8, 0xE8, 0xE8, 0xE8, 0xE9, 0xE9, 0xE9, 0xE9, 
  0xEA, 0xEA, 0xEA, 0xEA, 0xEB, 0xEB, 0xEB, 0xEB, 
  0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 
  0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF 
};

static UInt8 handeraTable2bpp_2[256] = 
{
  0x00, 0x00, 0x01, 0x01, 0x04, 0x05, 0x05, 0x06, 
  0x09, 0x09, 0x0A, 0x0A, 0x0D, 0x0E, 0x0E, 0x0F, 
  0x10, 0x10, 0x11, 0x11, 0x14, 0x15, 0x15, 0x16, 
  0x19, 0x19, 0x1A, 0x1A, 0x1D, 0x1E, 0x1E, 0x1F, 
  0x60, 0x60, 0x61, 0x61, 0x64, 0x65, 0x65, 0x66, 
  0x69, 0x69, 0x6A, 0x6A, 0x6D, 0x6E, 0x6E, 0x6F, 
  0x70, 0x70, 0x71, 0x71, 0x74, 0x75, 0x75, 0x76, 
  0x79, 0x79, 0x7A, 0x7A, 0x7D, 0x7E, 0x7E, 0x7F, 
  0x00, 0x00, 0x01, 0x01, 0x04, 0x05, 0x05, 0x06, 
  0x09, 0x09, 0x0A, 0x0A, 0x0D, 0x0E, 0x0E, 0x0F, 
  0x50, 0x50, 0x51, 0x51, 0x54, 0x55, 0x55, 0x56, 
  0x59, 0x59, 0x5A, 0x5A, 0x5D, 0x5E, 0x5E, 0x5F, 
  0x60, 0x60, 0x61, 0x61, 0x64, 0x65, 0x65, 0x66, 
  0x69, 0x69, 0x6A, 0x6A, 0x6D, 0x6E, 0x6E, 0x6F, 
  0xB0, 0xB0, 0xB1, 0xB1, 0xB4, 0xB5, 0xB5, 0xB6, 
  0xB9, 0xB9, 0xBA, 0xBA, 0xBD, 0xBE, 0xBE, 0xBF, 
  0x40, 0x40, 0x41, 0x41, 0x44, 0x45, 0x45, 0x46, 
  0x49, 0x49, 0x4A, 0x4A, 0x4D, 0x4E, 0x4E, 0x4F, 
  0x50, 0x50, 0x51, 0x51, 0x54, 0x55, 0x55, 0x56, 
  0x59, 0x59, 0x5A, 0x5A, 0x5D, 0x5E, 0x5E, 0x5F, 
  0xA0, 0xA0, 0xA1, 0xA1, 0xA4, 0xA5, 0xA5, 0xA6, 
  0xA9, 0xA9, 0xAA, 0xAA, 0xAD, 0xAE, 0xAE, 0xAF, 
  0xB0, 0xB0, 0xB1, 0xB1, 0xB4, 0xB5, 0xB5, 0xB6, 
  0xB9, 0xB9, 0xBA, 0xBA, 0xBD, 0xBE, 0xBE, 0xBF, 
  0x40, 0x40, 0x41, 0x41, 0x44, 0x45, 0x45, 0x46, 
  0x49, 0x49, 0x4A, 0x4A, 0x4D, 0x4E, 0x4E, 0x4F, 
  0x90, 0x90, 0x91, 0x91, 0x94, 0x95, 0x95, 0x96, 
  0x99, 0x99, 0x9A, 0x9A, 0x9D, 0x9E, 0x9E, 0x9F, 
  0xA0, 0xA0, 0xA1, 0xA1, 0xA4, 0xA5, 0xA5, 0xA6, 
  0xA9, 0xA9, 0xAA, 0xAA, 0xAD, 0xAE, 0xAE, 0xAF, 
  0xF0, 0xF0, 0xF1, 0xF1, 0xF4, 0xF5, 0xF5, 0xF6, 
  0xF9, 0xF9, 0xFA, 0xFA, 0xFD, 0xFE, 0xFE, 0xFF
};

static UInt8 handeraTable2bpp_3[256] = 
{
  0x00, 0x01, 0x06, 0x07, 0x10, 0x15, 0x16, 0x1B, 
  0x24, 0x25, 0x2A, 0x2B, 0x34, 0x39, 0x3A, 0x3F, 
  0x40, 0x41, 0x46, 0x47, 0x50, 0x55, 0x56, 0x5B, 
  0x64, 0x65, 0x6A, 0x6B, 0x74, 0x79, 0x7A, 0x7F, 
  0x80, 0x81, 0x86, 0x87, 0x90, 0x95, 0x96, 0x9B, 
  0xA4, 0xA5, 0xAA, 0xAB, 0xB4, 0xB9, 0xBA, 0xBF, 
  0xC0, 0xC1, 0xC6, 0xC7, 0xD0, 0xD5, 0xD6, 0xDB, 
  0xE4, 0xE5, 0xEA, 0xEB, 0xF4, 0xF9, 0xFA, 0xFF, 
  0x00, 0x01, 0x06, 0x07, 0x10, 0x15, 0x16, 0x1B, 
  0x24, 0x25, 0x2A, 0x2B, 0x34, 0x39, 0x3A, 0x3F, 
  0x40, 0x41, 0x46, 0x47, 0x50, 0x55, 0x56, 0x5B, 
  0x64, 0x65, 0x6A, 0x6B, 0x74, 0x79, 0x7A, 0x7F, 
  0x80, 0x81, 0x86, 0x87, 0x90, 0x95, 0x96, 0x9B, 
  0xA4, 0xA5, 0xAA, 0xAB, 0xB4, 0xB9, 0xBA, 0xBF, 
  0xC0, 0xC1, 0xC6, 0xC7, 0xD0, 0xD5, 0xD6, 0xDB, 
  0xE4, 0xE5, 0xEA, 0xEB, 0xF4, 0xF9, 0xFA, 0xFF, 
  0x00, 0x01, 0x06, 0x07, 0x10, 0x15, 0x16, 0x1B, 
  0x24, 0x25, 0x2A, 0x2B, 0x34, 0x39, 0x3A, 0x3F, 
  0x40, 0x41, 0x46, 0x47, 0x50, 0x55, 0x56, 0x5B, 
  0x64, 0x65, 0x6A, 0x6B, 0x74, 0x79, 0x7A, 0x7F, 
  0x80, 0x81, 0x86, 0x87, 0x90, 0x95, 0x96, 0x9B, 
  0xA4, 0xA5, 0xAA, 0xAB, 0xB4, 0xB9, 0xBA, 0xBF, 
  0xC0, 0xC1, 0xC6, 0xC7, 0xD0, 0xD5, 0xD6, 0xDB, 
  0xE4, 0xE5, 0xEA, 0xEB, 0xF4, 0xF9, 0xFA, 0xFF, 
  0x00, 0x01, 0x06, 0x07, 0x10, 0x15, 0x16, 0x1B, 
  0x24, 0x25, 0x2A, 0x2B, 0x34, 0x39, 0x3A, 0x3F, 
  0x40, 0x41, 0x46, 0x47, 0x50, 0x55, 0x56, 0x5B, 
  0x64, 0x65, 0x6A, 0x6B, 0x74, 0x79, 0x7A, 0x7F, 
  0x80, 0x81, 0x86, 0x87, 0x90, 0x95, 0x96, 0x9B, 
  0xA4, 0xA5, 0xAA, 0xAB, 0xB4, 0xB9, 0xBA, 0xBF, 
  0xC0, 0xC1, 0xC6, 0xC7, 0xD0, 0xD5, 0xD6, 0xDB, 
  0xE4, 0xE5, 0xEA, 0xEB, 0xF4, 0xF9, 0xFA, 0xFF
};

/**
 * Blit the offscreen image to the screen (stretch 160x160 to 240x240)
 */
static void
GraphicsRepaint_handera()
{
  UInt8 *ptrLCDScreen, *ptrScreen;
  UInt16 loop;
#if PORTABLE || !FULL_SCREEN_BLIT
  UInt16 i, j;
#endif

  ptrLCDScreen = globals.ptrWinLCD;
  ptrScreen    = globals.ptrWinDraw;

  // 2bpp
  if (globals.scrDepth == 2)
  {
#if FULL_SCREEN_BLIT
    ptrLCDScreen += ((SCREEN_WIDTH_HANDERA >> 2) * ((SPR_HEIGHT * 3) >> 1));
    ptrScreen    += ((OFFSCREEN_WIDTH >> 2) * SPR_HEIGHT);
    loop          = SCREEN_HEIGHT >> 2;
#else
    i = globals.yStart & ~(4 - 1);
    j = (((globals.yEnd >> 2) + 1) << 2);

    loop = (j - i) >> 2;
    if (loop == 0) goto REPAINT_DONE;

    ptrLCDScreen += ((SCREEN_WIDTH_HANDERA >> 2) * (((SPR_HEIGHT + i) * 3) >> 1));
    ptrScreen    += ((OFFSCREEN_WIDTH >> 2) * (SPR_HEIGHT + i));
#endif
//  ptrScreen    += (globals.xOffset / 4);
    ptrScreen    += (globals.xOffset >> 2);

#if PORTABLE
    j = loop;
    do
    {
      UInt8 scr1, scr2;

      i = ((SCREEN_WIDTH_HANDERA >> 2) / 3);
      do
      {
        *ptrLCDScreen = scr1 = handeraTable2bpp_1[*ptrScreen];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 2)) = scr2 =
          handeraTable2bpp_1[*(ptrScreen + (OFFSCREEN_WIDTH >> 2))];
        *(ptrLCDScreen + (SCREEN_WIDTH_HANDERA >> 2)) =
          ((scr1 & scr2) & 0x55) + (((scr1 & 0xaa) + (scr2 & 0xaa)) >> 1);

        *(ptrLCDScreen + 1) = scr1 = 
          handeraTable2bpp_2[(*(UInt16 *)(ptrScreen) >> 4) & 0xff];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 2) + 1) = scr2 =
          handeraTable2bpp_2[(*(UInt16 *)(ptrScreen + 
                             (OFFSCREEN_WIDTH >> 2)) >> 4) & 0xff];
        *(ptrLCDScreen + (SCREEN_WIDTH_HANDERA >> 2) + 1) =
          ((scr1 & scr2) & 0x55) + (((scr1 & 0xaa) + (scr2 & 0xaa)) >> 1);

        *(ptrLCDScreen + 2) = scr1 = 
          handeraTable2bpp_3[*(ptrScreen + 1)];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 2) + 2) = scr2 =
          handeraTable2bpp_3[*(ptrScreen + 
                             (OFFSCREEN_WIDTH >> 2) + 1)];
        *(ptrLCDScreen + (SCREEN_WIDTH_HANDERA >> 2) + 2) =
          ((scr1 & scr2) & 0x55) + (((scr1 & 0xaa) + (scr2 & 0xaa)) >> 1);

        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 3)) = scr1 =
          handeraTable2bpp_1[*(ptrScreen + ((OFFSCREEN_WIDTH >> 2) * 2))];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 5)) = scr2 =
          handeraTable2bpp_1[*(ptrScreen + ((OFFSCREEN_WIDTH >> 2) * 3))];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 4)) =
          ((scr1 & scr2) & 0x55) + (((scr1 & 0xaa) + (scr2 & 0xaa)) >> 1);

        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 3) + 1) = scr1 =
          handeraTable2bpp_2[(*(UInt16 *)(ptrScreen + 
                             ((OFFSCREEN_WIDTH >> 2) * 2)) >> 4) & 0xff];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 5) + 1) = scr2 =
          handeraTable2bpp_2[(*(UInt16 *)(ptrScreen + 
                             ((OFFSCREEN_WIDTH >> 2) * 3)) >> 4) & 0xff];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 4) + 1) =
          ((scr1 & scr2) & 0x55) + (((scr1 & 0xaa) + (scr2 & 0xaa)) >> 1);

        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 3) + 2) = scr1 =
          handeraTable2bpp_3[*(ptrScreen + 
                             ((OFFSCREEN_WIDTH >> 2) * 2) + 1)];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 5) + 2) = scr2 =
          handeraTable2bpp_3[*(ptrScreen + 
                             ((OFFSCREEN_WIDTH >> 2) * 3) + 1)];
        *(ptrLCDScreen + ((SCREEN_WIDTH_HANDERA >> 2) * 4) + 2) =
          ((scr1 & scr2) & 0x55) + (((scr1 & 0xaa) + (scr2 & 0xaa)) >> 1);

        ptrLCDScreen += 3;
        ptrScreen += 2;
      }
      while (--i);

      ptrScreen    += (((OFFSCREEN_WIDTH >> 2) * 4) - 
                       (((SCREEN_WIDTH_HANDERA >> 2) / 3) * 2));
      ptrLCDScreen += ((SCREEN_WIDTH_HANDERA >> 2) * 5);
    }
    while (--j);
#else
    // push registers on stack
    asm("      movem.l %%d0-%%d7/%%a1-%%a5, -(%%sp)": :);

    asm("      move.l  %2, %%a3
               move.l  %3, %%a4
               move.l  %4, %%a5
               move.l  %0, %%a1
               move.l  %1, %%a2
               move.l  %5, %%d0
               moveq   #0, %%d1
               moveq   #0, %%d2
               moveq   #85, %%d7
               move.w  #170, %%d3

handera_ScrLoop:

               moveq   #19, %%d4

handera_ScrLoop2:

               move.b  (%%a1), %%d1
               move.b  (%%a3, %%d1.w), %%d1
               move.b  %%d1, (%%a2)+

               move.b  160(%%a1), %%d2
               move.b  (%%a3, %%d2.w), %%d2
               move.b  %%d2, 119(%%a2)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 59(%%a2)

               move.w  (%%a1), %%d5
               lsr.w   #4, %%d5
               move.b  %%d5, %%d2
               move.b  (%%a4, %%d2.w), %%d1
               move.b  %%d1, (%%a2)+

               move.w  160(%%a1), %%d5
               lsr.w   #4, %%d5
               move.b  %%d5, %%d2
               move.b  (%%a4, %%d2.w), %%d2
               move.b  %%d2, 119(%%a2)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 59(%%a2)

               move.b  1(%%a1), %%d1
               move.b  (%%a5, %%d1.w), %%d1
               move.b  %%d1, (%%a2)+

               move.b  161(%%a1), %%d2
               move.b  (%%a5, %%d2.w), %%d2
               move.b  %%d2, 119(%%a2)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 59(%%a2)

               move.b  320(%%a1), %%d1
               move.b  (%%a3, %%d1.w), %%d1
               move.b  %%d1, 177(%%a2)

               move.b  480(%%a1), %%d2
               move.b  (%%a3, %%d2.w), %%d2
               move.b  %%d2, 297(%%a2)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 237(%%a2)

               move.w  320(%%a1), %%d5
               lsr.w   #4, %%d5
               move.b  %%d5, %%d2
               move.b  (%%a4, %%d2.w), %%d1
               move.b  %%d1, 178(%%a2)

               move.w  480(%%a1), %%d5
               lsr.w   #4, %%d5
               move.b  %%d5, %%d2
               move.b  (%%a4, %%d2.w), %%d2
               move.b  %%d2, 298(%%a2)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 238(%%a2)

               move.b  321(%%a1), %%d1
               move.b  (%%a5, %%d1.w), %%d1
               move.b  %%d1, 179(%%a2)

               move.b  481(%%a1), %%d2
               move.b  (%%a5, %%d2.w), %%d2
               move.b  %%d2, 299(%%a2)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 239(%%a2)

               lea     2(%%a1), %%a1

               dbra    %%d4, handera_ScrLoop2

               lea     600(%%a1), %%a1
               lea     300(%%a2), %%a2

               dbra    %%d0, handera_ScrLoop

    ": : "r" (ptrScreen),
         "r" (ptrLCDScreen),
         "r" (handeraTable2bpp_1),
         "r" (handeraTable2bpp_2),
         "r" (handeraTable2bpp_3),
         "r" (loop-1));

    // pop registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d7/%%a1-%%a5": :);
#endif
  }
  // 4bpc
  else
  if (globals.scrDepth == 4)
  {
    // sorry Mike_W, aint written this code yet, back to 100% hires for you!
    GraphicsRepaint_handeraHiRes();
  }

#if !FULL_SCREEN_BLIT
REPAINT_DONE:
#endif
}

/**
 * Blit the offscreen image to the screen (HiRes mode).
 */
static void
GraphicsRepaint_handeraHiRes()
{
  UInt8  *ptrLCDScreen, *ptrScreen;
  Int16  loop;
#if PORTABLE
  UInt16 i, j;
#endif

  ptrLCDScreen = globals.ptrWinLCD;
  ptrScreen    = globals.ptrWinDraw;

  // 2bpp
  if (globals.scrDepth == 2)
  {
    ptrLCDScreen += (SCREEN_WIDTH_HANDERA * SCREEN_START_HANDERA) >> 2;

//  ptrScreen    += (globals.xOffset / 4);
    ptrScreen    += (globals.xOffset >> 2);
#if FULL_SCREEN_BLIT
    ptrScreen    += ((SPR_HEIGHT << 5) * 5);
    loop          = SCREEN_HEIGHT-1;
#else
//  ptrScreen    += ((globals.yStart + SPR_HEIGHT) * 160);
//  ptrLCDScreen += (globals.yStart  * 60);
    ptrScreen    += (((globals.yStart + SPR_HEIGHT) << 5) * 5);
    ptrLCDScreen += (globals.yStart  * 60);
    loop          = (globals.yEnd - globals.yStart);
    if (loop == 0) goto REPAINT_DONE;
#endif

#if PORTABLE
    j = loop + 1;
    do
    {
      i = (SCREEN_WIDTH_HANDERA >> 2);
      do
      {
        *ptrLCDScreen++ = *ptrScreen++;
      }
      while (--i);

      ptrScreen += 160 - (SCREEN_WIDTH_HANDERA >> 2);
    }
    while (--j);
#else
    // push all registers (except a7) on stack
    asm("movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)" : : );

    // copy inner 160x128 from back buffer to screen
    asm("move.l  %0, %%a0" : : "g" (ptrScreen));
    asm("move.l  %0, %%a1" : : "g" (ptrLCDScreen));
    asm("move.l  %0, %%d0" : : "g" (loop));
    asm("
handeraHR_ScrLoop:
         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, (%%a1)

         movem.l (%%a0)+, %%d1-%%d5
         movem.l %%d1-%%d5, 40(%%a1)

         lea     60(%%a1), %%a1                      
         lea     100(%%a0), %%a0                 | blit one row!!

         dbra    %%d0, handeraHR_ScrLoop
    " : :);

    // pop all registers (except a7) off stack
    asm("movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6" : : );
#endif
  }

  // 4bpc
  else
  if (globals.scrDepth == 4)
  {
    ptrLCDScreen += (SCREEN_WIDTH_HANDERA * SCREEN_START_HANDERA) >> 1;

//  ptrScreen    += (globals.xOffset / 2);
    ptrScreen    += (globals.xOffset >> 1);
#if FULL_SCREEN_BLIT
    ptrScreen    += ((SPR_HEIGHT << 6) * 5);
    loop          = SCREEN_HEIGHT-1;
#else
//  ptrScreen    += ((globals.yStart + SPR_HEIGHT) * 320);
//  ptrLCDScreen += (globals.yStart  * 120);
    ptrScreen    += (((globals.yStart + SPR_HEIGHT) << 6) * 5);
    ptrLCDScreen += (globals.yStart * 120);
    loop          = (globals.yEnd - globals.yStart);
    if (loop == 0) goto REPAINT_DONE;
#endif

#if PORTABLE
    j = loop + 1;
    do
    {
      i = (SCREEN_WIDTH_HANDERA >> 1);
      do
        *ptrLCDScreen++ = *ptrScreen++;
      while (--i);

      ptrScreen += 320 - (SCREEN_WIDTH_HANDERA >> 1);
    }
    while (--j);
#else
    // push all registers (except a7) on stack
    asm("movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)" : : );

    // copy inner 160x128 from back buffer to screen
    asm("move.l  %0, %%a0" : : "g" (ptrScreen));
    asm("move.l  %0, %%a1" : : "g" (ptrLCDScreen));
    asm("move.l  %0, %%d0" : : "g" (loop));
    asm("
handeraHR_ScrLoop2:
         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, (%%a1)

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, 40(%%a1)

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, 80(%%a1)

         lea     120(%%a1), %%a1                      
         lea     200(%%a0), %%a0                 | blit one row!!

         dbra    %%d0, handeraHR_ScrLoop2
    " : :);

    // pop all registers (except a7) off stack
    asm("movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6" : : );
#endif
  }

#if !FULL_SCREEN_BLIT
REPAINT_DONE:
#endif
}
#endif

// 
// 320x320 sony call display routines 
//
// -- Aaron Ardiri, 2001
//

#if SONY_NATIVE

// pixel double tables for 2bpp
static UInt16 table2bpp[256] = 
{
  0x0000,0x0005,0x000a,0x000f,0x0050,0x0055,0x005a,0x005f,
  0x00a0,0x00a5,0x00aa,0x00af,0x00f0,0x00f5,0x00fa,0x00ff,
  0x0500,0x0505,0x050a,0x050f,0x0550,0x0555,0x055a,0x055f,
  0x05a0,0x05a5,0x05aa,0x05af,0x05f0,0x05f5,0x05fa,0x05ff,
  0x0a00,0x0a05,0x0a0a,0x0a0f,0x0a50,0x0a55,0x0a5a,0x0a5f,
  0x0aa0,0x0aa5,0x0aaa,0x0aaf,0x0af0,0x0af5,0x0afa,0x0aff,
  0x0f00,0x0f05,0x0f0a,0x0f0f,0x0f50,0x0f55,0x0f5a,0x0f5f,
  0x0fa0,0x0fa5,0x0faa,0x0faf,0x0ff0,0x0ff5,0x0ffa,0x0fff,
  0x5000,0x5005,0x500a,0x500f,0x5050,0x5055,0x505a,0x505f,
  0x50a0,0x50a5,0x50aa,0x50af,0x50f0,0x50f5,0x50fa,0x50ff,
  0x5500,0x5505,0x550a,0x550f,0x5550,0x5555,0x555a,0x555f,
  0x55a0,0x55a5,0x55aa,0x55af,0x55f0,0x55f5,0x55fa,0x55ff,
  0x5a00,0x5a05,0x5a0a,0x5a0f,0x5a50,0x5a55,0x5a5a,0x5a5f,
  0x5aa0,0x5aa5,0x5aaa,0x5aaf,0x5af0,0x5af5,0x5afa,0x5aff,
  0x5f00,0x5f05,0x5f0a,0x5f0f,0x5f50,0x5f55,0x5f5a,0x5f5f,
  0x5fa0,0x5fa5,0x5faa,0x5faf,0x5ff0,0x5ff5,0x5ffa,0x5fff,
  0xa000,0xa005,0xa00a,0xa00f,0xa050,0xa055,0xa05a,0xa05f,
  0xa0a0,0xa0a5,0xa0aa,0xa0af,0xa0f0,0xa0f5,0xa0fa,0xa0ff,
  0xa500,0xa505,0xa50a,0xa50f,0xa550,0xa555,0xa55a,0xa55f,
  0xa5a0,0xa5a5,0xa5aa,0xa5af,0xa5f0,0xa5f5,0xa5fa,0xa5ff,
  0xaa00,0xaa05,0xaa0a,0xaa0f,0xaa50,0xaa55,0xaa5a,0xaa5f,
  0xaaa0,0xaaa5,0xaaaa,0xaaaf,0xaaf0,0xaaf5,0xaafa,0xaaff,
  0xaf00,0xaf05,0xaf0a,0xaf0f,0xaf50,0xaf55,0xaf5a,0xaf5f,
  0xafa0,0xafa5,0xafaa,0xafaf,0xaff0,0xaff5,0xaffa,0xafff,
  0xf000,0xf005,0xf00a,0xf00f,0xf050,0xf055,0xf05a,0xf05f,
  0xf0a0,0xf0a5,0xf0aa,0xf0af,0xf0f0,0xf0f5,0xf0fa,0xf0ff,
  0xf500,0xf505,0xf50a,0xf50f,0xf550,0xf555,0xf55a,0xf55f,
  0xf5a0,0xf5a5,0xf5aa,0xf5af,0xf5f0,0xf5f5,0xf5fa,0xf5ff,
  0xfa00,0xfa05,0xfa0a,0xfa0f,0xfa50,0xfa55,0xfa5a,0xfa5f,
  0xfaa0,0xfaa5,0xfaaa,0xfaaf,0xfaf0,0xfaf5,0xfafa,0xfaff,
  0xff00,0xff05,0xff0a,0xff0f,0xff50,0xff55,0xff5a,0xff5f,
  0xffa0,0xffa5,0xffaa,0xffaf,0xfff0,0xfff5,0xfffa,0xffff
};

// pixel double tables for 4bpc
static UInt16 table4bpc[256] = 
{
  0x0000,0x0011,0x0022,0x0033,0x0044,0x0055,0x0066,0x0077,
  0x0088,0x0099,0x00aa,0x00bb,0x00cc,0x00dd,0x00ee,0x00ff,
  0x1100,0x1111,0x1122,0x1133,0x1144,0x1155,0x1166,0x1177,
  0x1188,0x1199,0x11aa,0x11bb,0x11cc,0x11dd,0x11ee,0x11ff,
  0x2200,0x2211,0x2222,0x2233,0x2244,0x2255,0x2266,0x2277,
  0x2288,0x2299,0x22aa,0x22bb,0x22cc,0x22dd,0x22ee,0x22ff,
  0x3300,0x3311,0x3322,0x3333,0x3344,0x3355,0x3366,0x3377,
  0x3388,0x3399,0x33aa,0x33bb,0x33cc,0x33dd,0x33ee,0x33ff,
  0x4400,0x4411,0x4422,0x4433,0x4444,0x4455,0x4466,0x4477,
  0x4488,0x4499,0x44aa,0x44bb,0x44cc,0x44dd,0x44ee,0x44ff,
  0x5500,0x5511,0x5522,0x5533,0x5544,0x5555,0x5566,0x5577,
  0x5588,0x5599,0x55aa,0x55bb,0x55cc,0x55dd,0x55ee,0x55ff,
  0x6600,0x6611,0x6622,0x6633,0x6644,0x6655,0x6666,0x6677,
  0x6688,0x6699,0x66aa,0x66bb,0x66cc,0x66dd,0x66ee,0x66ff,
  0x7700,0x7711,0x7722,0x7733,0x7744,0x7755,0x7766,0x7777,
  0x7788,0x7799,0x77aa,0x77bb,0x77cc,0x77dd,0x77ee,0x77ff,
  0x8800,0x8811,0x8822,0x8833,0x8844,0x8855,0x8866,0x8877,
  0x8888,0x8899,0x88aa,0x88bb,0x88cc,0x88dd,0x88ee,0x88ff,
  0x9900,0x9911,0x9922,0x9933,0x9944,0x9955,0x9966,0x9977,
  0x9988,0x9999,0x99aa,0x99bb,0x99cc,0x99dd,0x99ee,0x99ff,
  0xaa00,0xaa11,0xaa22,0xaa33,0xaa44,0xaa55,0xaa66,0xaa77,
  0xaa88,0xaa99,0xaaaa,0xaabb,0xaacc,0xaadd,0xaaee,0xaaff,
  0xbb00,0xbb11,0xbb22,0xbb33,0xbb44,0xbb55,0xbb66,0xbb77,
  0xbb88,0xbb99,0xbbaa,0xbbbb,0xbbcc,0xbbdd,0xbbee,0xbbff,
  0xcc00,0xcc11,0xcc22,0xcc33,0xcc44,0xcc55,0xcc66,0xcc77,
  0xcc88,0xcc99,0xccaa,0xccbb,0xcccc,0xccdd,0xccee,0xccff,
  0xdd00,0xdd11,0xdd22,0xdd33,0xdd44,0xdd55,0xdd66,0xdd77,
  0xdd88,0xdd99,0xddaa,0xddbb,0xddcc,0xdddd,0xddee,0xddff,
  0xee00,0xee11,0xee22,0xee33,0xee44,0xee55,0xee66,0xee77,
  0xee88,0xee99,0xeeaa,0xeebb,0xeecc,0xeedd,0xeeee,0xeeff,
  0xff00,0xff11,0xff22,0xff33,0xff44,0xff55,0xff66,0xff77,
  0xff88,0xff99,0xffaa,0xffbb,0xffcc,0xffdd,0xffee,0xffff
}; 

/**
 * Blit the offscreen image to the screen.
 */
static void
GraphicsRepaint_sony()
{
  UInt16 *ptrLCDScreen;
  UInt8  *ptrScreen;
  Int16  loop;
#if PORTABLE
  UInt16 i, j;
#endif

  ptrLCDScreen = (UInt16 *)globals.ptrWinLCD;
  ptrScreen    = globals.ptrWinDraw;

  // 2bpp
  if (globals.scrDepth == 2)
  {
    ptrLCDScreen += (SCREEN_WIDTH_SONY * (SCREEN_START_GENERIC*2)) >> 3;

//  ptrScreen    += (globals.xOffset / 4);
    ptrScreen    += (globals.xOffset >> 2);
#if FULL_SCREEN_BLIT
    ptrScreen    += ((SPR_HEIGHT << 5) * 5);
    loop          = SCREEN_HEIGHT-1;
#else
//  ptrScreen    += ((globals.yStart + SPR_HEIGHT) * 160);
//  ptrLCDScreen += (globals.yStart  * 40);
    ptrScreen    += (((globals.yStart + SPR_HEIGHT) << 5) * 5);
    ptrLCDScreen += (globals.yStart  * 40);
    loop          = (globals.yEnd - globals.yStart);
    if (loop == 0) goto REPAINT_DONE;
#endif

#if PORTABLE
    j = loop + 1;
    do
    {
      i = (SCREEN_WIDTH_GENERIC >> 2);
      do
      {
        *ptrLCDScreen        = table2bpp[*ptrScreen++];
        *(ptrLCDScreen + 40) = *ptrLCDScreen++;
      }
      while (--i);

      ptrScreen    += 160 - (SCREEN_WIDTH_GENERIC >> 2);
      ptrLCDScreen += 40;
    }
    while (--j);
#else
    // push all registers used on stack
    asm("movem.l %%d0-%%d3/%%a0-%%a3, -(%%sp)" : : );

    // copy inner 160x128 from back buffer to screen
    asm("move.l  %0, %%a1              | ptrLCDScreen
         move.l  %1, %%a2              | ptrScreen
         move.l  %2, %%a3              | table2bpp
         move.l  %3, %%d0              | loop

         moveq   #0, %%d2

sony_ScrLoop0:

         moveq   #9, %%d1
         lea     80(%%a1), %%a0

sony_ScrLoop1:

         move.b  (%%a2)+, %%d2
         move.l  %%d2, %%d3
         add.w   %%d3, %%d3
         move.w  (%%a3, %%d3), %%d3    | x = table2bpp[v]
         move.w  %%d3, (%%a1)+         | *ptrLCDScreen(y)   = x;
         move.w  %%d3, (%%a0)+         | *ptrLCDScreen(y+1) = x;  (next line)

         move.b  (%%a2)+, %%d2
         move.l  %%d2, %%d3
         add.w   %%d3, %%d3
         move.w  (%%a3, %%d3), %%d3
         move.w  %%d3, (%%a1)+
         move.w  %%d3, (%%a0)+

         move.b  (%%a2)+, %%d2
         move.l  %%d2, %%d3
         add.w   %%d3, %%d3
         move.w  (%%a3, %%d3), %%d3
         move.w  %%d3, (%%a1)+
         move.w  %%d3, (%%a0)+

         move.b  (%%a2)+, %%d2
         move.l  %%d2, %%d3
         add.w   %%d3, %%d3
         move.w  (%%a3, %%d3), %%d3
         move.w  %%d3, (%%a1)+
         move.w  %%d3, (%%a0)+

         dbra    %%d1, sony_ScrLoop1

         lea     120(%%a2), %%a2       | ptrScreen    += 120 bytes
         lea     80(%%a1), %%a1        | ptrLCDScreen += 80 bytes
         dbra    %%d0, sony_ScrLoop0
    ": : "r" (ptrLCDScreen),
         "r" (ptrScreen),
         "r" (table2bpp),
         "r" (loop));

    // pop all registers used off stack
    asm("movem.l (%%sp)+, %%d0-%%d3/%%a0-%%a3" : : );
#endif
  }

  // 4bpc
  else
  if (globals.scrDepth == 4)
  {
    ptrLCDScreen += (SCREEN_WIDTH_SONY * (SCREEN_START_GENERIC*2)) >> 2;

//  ptrScreen    += (globals.xOffset / 2);
    ptrScreen    += (globals.xOffset >> 1);
#if FULL_SCREEN_BLIT
    ptrScreen    += ((SPR_HEIGHT << 6) * 5);
    loop          = SCREEN_HEIGHT-1;
#else
//  ptrScreen    += ((globals.yStart + SPR_HEIGHT) * 320);
//  ptrLCDScreen += (globals.yStart  * 80);
    ptrScreen    += (((globals.yStart + SPR_HEIGHT) << 6) * 5);
    ptrLCDScreen += (globals.yStart * 80);
    loop          = (globals.yEnd - globals.yStart);
    if (loop == 0) goto REPAINT_DONE;
#endif

#if PORTABLE
    j = loop + 1;
    do
    {
      i = (SCREEN_WIDTH_GENERIC >> 1);
      do
      {
        *ptrLCDScreen        = table4bpc[*ptrScreen++];
        *(ptrLCDScreen + 80) = *ptrLCDScreen++;
      }
      while (--i);

      ptrScreen    += 320 - (SCREEN_WIDTH_GENERIC >> 1);
      ptrLCDScreen += 80;
    }
    while (--j);
#else
    // push all registers used on stack
    asm("movem.l %%d0-%%d3/%%a0-%%a3, -(%%sp)" : : );

    // copy inner 160x128 from back buffer to screen
    asm("move.l  %0, %%a1              | ptrLCDScreen
         move.l  %1, %%a2              | ptrScreen
         move.l  %2, %%a3              | table4bpc
         move.l  %3, %%d0              | loop

         moveq   #0, %%d2

sony_ScrLoop2:

         moveq   #19, %%d1
         lea     160(%%a1), %%a0

sony_ScrLoop3:

         move.b  (%%a2)+, %%d2
         move.l  %%d2, %%d3
         add.w   %%d3, %%d3            
         move.w  (%%a3, %%d3), %%d3    | x = table4bpc[v]
         move.w  %%d3, (%%a1)+         | *ptrLCDScreen(y)   = x;
         move.w  %%d3, (%%a0)+         | *ptrLCDScreen(y+1) = x;  (next line)

         move.b  (%%a2)+, %%d2
         move.l  %%d2, %%d3
         add.w   %%d3, %%d3
         move.w  (%%a3, %%d3), %%d3
         move.w  %%d3, (%%a1)+
         move.w  %%d3, (%%a0)+

         move.b  (%%a2)+, %%d2
         move.l  %%d2, %%d3
         add.w   %%d3, %%d3
         move.w  (%%a3, %%d3), %%d3
         move.w  %%d3, (%%a1)+
         move.w  %%d3, (%%a0)+

         move.b  (%%a2)+, %%d2
         move.l  %%d2, %%d3
         add.w   %%d3, %%d3
         move.w  (%%a3, %%d3), %%d3
         move.w  %%d3, (%%a1)+
         move.w  %%d3, (%%a0)+

         dbra    %%d1, sony_ScrLoop3

         lea     240(%%a2), %%a2       | ptrScreen    += 240 bytes
         lea     160(%%a1), %%a1       | ptrLCDScreen += 160 bytes
         dbra    %%d0, sony_ScrLoop2
    ": : "r" (ptrLCDScreen),
         "r" (ptrScreen),
         "r" (table4bpc),
         "r" (loop));

    // pop all registers used off stack
    asm("movem.l (%%sp)+, %%d0-%%d3/%%a0-%%a3" : : );
#endif
  }

#if !FULL_SCREEN_BLIT
REPAINT_DONE:
#endif
}

/**
 * Blit the offscreen image to the screen (HiRes mode).
 */
static void
GraphicsRepaint_sonyHiRes()
{
  UInt8  *ptrLCDScreen, *ptrScreen;
  Int16  loop;
#if PORTABLE
  UInt16 i, j;
#endif

  ptrLCDScreen = globals.ptrWinLCD;
  ptrScreen    = globals.ptrWinDraw;

  // 2bpp
  if (globals.scrDepth == 2)
  {
    ptrLCDScreen += (SCREEN_WIDTH_SONY * SCREEN_START_SONY) >> 2;

//  ptrScreen    += (globals.xOffset / 4);
    ptrScreen    += (globals.xOffset >> 2);
#if FULL_SCREEN_BLIT
    ptrScreen    += ((SPR_HEIGHT << 5) * 5);
    loop          = SCREEN_HEIGHT-1;
#else
//  ptrScreen    += ((globals.yStart + SPR_HEIGHT) * 160);
//  ptrLCDScreen += (globals.yStart  * 80);
    ptrScreen    += (((globals.yStart + SPR_HEIGHT) << 5) * 5);
    ptrLCDScreen += (globals.yStart  * 80);
    loop          = (globals.yEnd - globals.yStart);
    if (loop == 0) goto REPAINT_DONE;
#endif

#if PORTABLE
    j = loop + 1;
    do
    {
      i = (SCREEN_WIDTH_SONY >> 2);
      do
      {
        *ptrLCDScreen++ = *ptrScreen++;
      }
      while (--i);

      ptrScreen += 160 - (SCREEN_WIDTH_SONY >> 2);
    }
    while (--j);
#else
    // push all registers (except a7) on stack
    asm("movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)" : : );

    // copy inner 160x128 from back buffer to screen
    asm("move.l  %0, %%a0" : : "g" (ptrScreen));
    asm("move.l  %0, %%a1" : : "g" (ptrLCDScreen));
    asm("move.l  %0, %%d0" : : "g" (loop));
    asm("

sonyHR_ScrLoop:

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, (%%a1)

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, (%%a1)

         lea     80(%%a1), %%a1                      
         lea     80(%%a0), %%a0                 | blit one row!!

         dbra    %%d0, sonyHR_ScrLoop
    " : :);

    // pop all registers (except a7) off stack
    asm("movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6" : : );
#endif
  }

  // 4bpc
  else
  if (globals.scrDepth == 4)
  {
    ptrLCDScreen += (SCREEN_WIDTH_SONY * SCREEN_START_SONY) >> 1;

//  ptrScreen    += (globals.xOffset / 2);
    ptrScreen    += (globals.xOffset >> 1);
#if FULL_SCREEN_BLIT
    ptrScreen    += ((SPR_HEIGHT << 6) * 5);
    loop          = SCREEN_HEIGHT-1;
#else
//  ptrScreen    += ((globals.yStart + SPR_HEIGHT) * 320);
//  ptrLCDScreen += (globals.yStart  * 160);
    ptrScreen    += (((globals.yStart + SPR_HEIGHT) << 6) * 5);
    ptrLCDScreen += (globals.yStart * 160);
    loop          = (globals.yEnd - globals.yStart);
    if (loop == 0) goto REPAINT_DONE;
#endif

#if PORTABLE
    j = loop + 1;
    do
    {
      i = (SCREEN_WIDTH_SONY >> 1);
      do
        *ptrLCDScreen++ = *ptrScreen++;
      while (--i);

      ptrScreen += 320 - (SCREEN_WIDTH_SONY >> 1);
    }
    while (--j);
#else
    // push all registers (except a7) on stack
    asm("movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)" : : );

    // copy inner 160x128 from back buffer to screen
    asm("move.l  %0, %%a0" : : "g" (ptrScreen));
    asm("move.l  %0, %%a1" : : "g" (ptrLCDScreen));
    asm("move.l  %0, %%d0" : : "g" (loop));
    asm("

sonyHR_ScrLoop2:

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, (%%a1)

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, 40(%%a1)

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, 80(%%a1)

         movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
         movem.l %%d1-%%d7/%%a2-%%a4, 120(%%a1)

         lea     160(%%a1), %%a1                      
         lea     160(%%a0), %%a0                 | blit one row!!

         dbra    %%d0, sonyHR_ScrLoop2
    " : :);

    // pop all registers (except a7) off stack
    asm("movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6" : : );
#endif
  }

#if !FULL_SCREEN_BLIT
REPAINT_DONE:
#endif
}
#endif
