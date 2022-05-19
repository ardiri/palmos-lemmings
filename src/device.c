/*
 * @(#)device.c
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

// globals variable structure
typedef struct
{
  Boolean   colorState;                    // the screen color status
  UInt32    romVersion;                    // the rom version of the device
  UInt32    depthState;                    // the screen depth state (old)

  UInt16    deviceVolume;                  // the volume of the game on device
  Boolean   deviceMuted;                   // the volume muting status

  struct
  {
    UInt16  dramState;                     // DRAM Control Register
  } cpu;

#if SONY_NATIVE
  struct
  {
    Boolean device;
    UInt16  libRef;
  } sony;
#endif

} DeviceGlobals;

static DeviceGlobals globals;

/**
 * Check the compatability status of the device we are working with.
 *
 * @return true if the device is supported, false otherwise.
 */
Boolean
DeviceCheckCompatability()
{
  Boolean result = true;

  // the device is only compatable if rom 3.1 or higher is available
  result = (
            DeviceSupportsVersion(romVersion3_1) &&
//          ((DeviceGetSupportedDepths() & 0x02) != 0)
            ((DeviceGetSupportedDepths() & 0x0A) != 0)  // 2bpp and 4bpp  // CHANGE: 4.4, 20031110
           );

  // not compatable :(
  if (!result) 
  {
    // display a incompatability dialog
    FormPtr form = FrmInitForm(deviForm);
    FrmDoDialog(form);
    FrmDeleteForm(form);

    // lets exit the application "gracefully" :>
    if (!DeviceSupportsVersion(romVersion2))
      AppLaunchWithCommand(sysFileCDefaultApp,sysAppLaunchCmdNormalLaunch,NULL);
  }

  return result;
}

/**
 * Initialize the device.
 */
void
DeviceInitialize()
{
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
  const RectangleType     rect  = {{0,0},{160,160}};

#if SONY_NATIVE
  Err   err;
#endif

  // clear the globals memory
  MemSet(&globals, sizeof(DeviceGlobals), 0);

  // get the rom version and ram size for this device
  FtrGet(sysFtrCreator, sysFtrNumROMVersion, &globals.romVersion);

  // must have active window
  WinSetDrawWindow(WinGetDisplayWindow());

  // are we running on sony?
#if SONY_NATIVE
  err = SysLibFind(sonySysLibNameHR, &globals.sony.libRef);
  if (err != errNone)
    err = SysLibLoad('libr', sonySysFileCHRLib, &globals.sony.libRef);
  globals.sony.device = (err == errNone);
#endif

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3))
  {
    // save the current display state
    WinScreenMode(winScreenModeGet,NULL,NULL,&globals.depthState,NULL);

    // change into the "highest" possible mode :P
    {
      UInt32 depthsToTry[] = { 8, 2 };
      UInt32 *depthPtr;

      // start off at valid depths..
      depthPtr = &depthsToTry[0];

      // loop until a valid mode is found
      while (WinScreenMode(winScreenModeSet,NULL,NULL,depthPtr,NULL))
      {
        // try the next depth
        depthPtr++;
      }

      // did we make it into 8bpp mode? = color
      globals.colorState = (*depthPtr == 8);
      if (*depthPtr == 8)
      {
        // lets change back to 4bpp grayscale
        *depthPtr = 4;
        WinScreenMode(winScreenModeSet,NULL,NULL,depthPtr,NULL);
      }

#if SONY_NATIVE
      // lets set up the hires mode :)
      if (globals.sony.device)
      {
        UInt32 width, height;

        // 320x320 baby!
        width  = hrWidth;
        height = hrHeight;

        // try to open the hires library
        err = HROpen(globals.sony.libRef);
        if (!err)
          // change into 320x320!
          HRWinScreenMode(globals.sony.libRef, winScreenModeSet,
                          &width, &height, depthPtr, NULL);

        globals.sony.device = (err == errNone);
      }
#endif

      if (globals.colorState)
      {
        MemHandle    memHandle;

        // clear the LCD screen (dont want palette flash)
        WinSetPattern(&erase);
        WinFillRectangle(&rect, 0);

        // and tweak a VGA 16 color palette
        memHandle = DmGetResource('PALT', paletteVGA16);
        WinPalette(winPaletteSet, 0, 16,
                   (RGBColorType *)MemHandleLock(memHandle));
        MemHandleUnlock(memHandle);
        DmReleaseResource(memHandle);
      }
    }
  }

#if DRAM_WS
  // adjust wait-states as appropriate
  {
    UInt32 cpuID;

    // get the processor ID
    FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
    switch (cpuID & sysFtrNumProcessorMask)
    {
      case sysFtrNumProcessorEZ:
           {
             UInt16 dram;

#define DRAMCONTROL ((unsigned short *)0xFFFFFC02)

             globals.cpu.dramState = *DRAMCONTROL;
             dram = globals.cpu.dramState;

             //   AA B   CCD E
             // 11001011 00000111

             dram = dram & ~0x3000;  // A: + fast page
             dram = dram & ~0x0400;  // B: - edo
             dram = dram & ~0x00C0;  // C: - wait states
             dram = dram & ~0x0020;  // D: - slow multiplexing
             dram = dram & ~0x0004;  // E: - slow ram

             *DRAMCONTROL = dram;

#undef DRAMCONTROL
           }
           break;

      case sysFtrNumProcessorVZ:
      case sysFtrNumProcessorSuperVZ:
           {
             UInt16 dram;

#define DRAMCONTROL ((unsigned short *)0xFFFFFC02)

             globals.cpu.dramState = *DRAMCONTROL;
             dram = globals.cpu.dramState;

             //   AA B     C D
             // 11001011 xx000111

             dram = dram & ~0x3000;  // A: + fast page
             dram = dram & ~0x0400;  // B: - edo
             dram = dram & ~0x0020;  // C: - slow multiplexing
             dram = dram & ~0x0004;  // D: - slow ram

             *DRAMCONTROL = dram;

#undef DRAMCONTROL
           }
           break;

      // no wait states :(
      default:
      case sysFtrNumProcessor328:
           break;
    }
  }
#endif
}

/**
 * Determine if the "DeviceGrayscale" routine is supported on the device.
 *
 * @return true if supported, false otherwise.
 */
Boolean
DeviceSupportsGrayscale()
{
  Boolean result = false;

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3))
  {
    UInt32 cpuID;
    UInt32 scrDepth;

    // get the processor ID
    FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
    cpuID = cpuID & sysFtrNumProcessorMask;

    // get the current "display" depth
    WinScreenMode(winScreenModeGet, NULL, NULL, &scrDepth, NULL);

    // the "rules" for grayscale support
    result = (
              (cpuID == sysFtrNumProcessor328) ||
              (cpuID == sysFtrNumProcessorEZ)  ||
              (cpuID == sysFtrNumProcessorVZ)  ||
              (cpuID == sysFtrNumProcessorSuperVZ)
             ) &&
             (scrDepth == 2); // we must be running in 2bpp grayscale!
  }

  return result;
}

/**
 * Grayscale routine/settings for the device.
 *
 * @param mode the desired mode of operation.
 * @param lgray the lGray index (0..6) in intensity
 * @param dgray the dGray index (0..6) in intensity
 */
void
DeviceGrayscale(UInt16 mode,
                UInt8  *lgray,
                UInt8  *dgray)
{
  UInt32 cpuID;

#define LGPMR1 ((unsigned char *)0xFFFFFA32)
#define LGPMR2 ((unsigned char *)0xFFFFFA33)

  // get the processor ID
  FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
  switch (cpuID & sysFtrNumProcessorMask)
  {
    case sysFtrNumProcessor328:
         {
           UInt8 gray[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07 };

           switch (mode)
           {
             case grayGet:
                  {
                    UInt8 data;

                    // light gray
                    data = (UInt8)(((*LGPMR1) & 0xF0) >> 4);
                    *lgray = 0;
                    while (gray[*lgray] < data)
                      (*lgray)++;

                    // dark gray
                    data = (UInt8)((*LGPMR2) & 0x0F);
                    *dgray = 0;
                    while (gray[*dgray] < data)
                      (*dgray)++;
                  }
                  break;

             case graySet:
                  *LGPMR1 = (gray[*lgray] << 4);
                  *LGPMR2 = (0x70 | gray[*dgray]);
                  break;

             default:
                  break;
           }
         }
         break;

    case sysFtrNumProcessorEZ:
    case sysFtrNumProcessorVZ:
    case sysFtrNumProcessorSuperVZ:
         {
           UInt8 gray[] = { 0x00, 0x03, 0x04, 0x07, 0x0A, 0x0C, 0x0F };

           switch (mode)
           {
             case grayGet:
                  {
                    UInt8 data;

                    // light gray
                    data = (UInt8)((*LGPMR2) & 0x0F);
                    *lgray = 0;
                    while (gray[*lgray] < data)
                      (*lgray)++;

                    // dark gray
                    data = (UInt8)(((*LGPMR2) & 0xF0) >> 4);
                    *dgray = 0;
                    while (gray[*dgray] < data)
                      (*dgray)++;
                  }
                  break;

             case graySet:
                  *LGPMR2 = ((gray[*dgray] << 4) | gray[*lgray]);
                  break;

             default:
                  break;
           }
         }
         break;

    default:
         break;
  }
}

/**
 * Play a sound on the device.
 *
 * @param sndCmd the sound information.
 */
void
DevicePlaySound(SndCommandType *sndCmd)
{
  // are we muted?
  if (!globals.deviceMuted)
  {
    UInt16 volumes[] = { 0x00,0x04,0x08,0x10,0x18,0x20,0x30,0x40 };

    // adjust to the right volume [variable]
    sndCmd->param3 =
      sndCmd->param3 * (volumes[globals.deviceVolume]) / sndMaxAmp;

    // do it!
    SndDoCmd(0, sndCmd, 0);
  }
}

/**
 * Set the volume on the device.
 *
 * @param volume the volume level for the device.
 */
void
DeviceSetVolume(UInt16 volume)
{
  // save the volume state
  globals.deviceVolume = volume;
}

/**
 * Get the volume on the device.
 *
 * @return the volume level for the device.
 */
UInt16
DeviceGetVolume()
{
  // return the volume state
  return globals.deviceVolume;
}

/**
 * Set the mute status of the sound on the device.
 *
 * @param mute true if no sound wanted, false otherwise.
 */
void
DeviceSetMute(Boolean mute)
{
  // save the mute state
  globals.deviceMuted = mute;
}

/**
 * Get the mute status of the sound on the device.
 *
 * @return true if no sound wanted, false otherwise.
 */
Boolean
DeviceGetMute()
{
  // return the mute state
  return globals.deviceMuted;
}

/**
 * Reset the device to its original state.
 */
void
DeviceTerminate()
{
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
  const RectangleType     rect  = {{0,0},{160,160}};

#if DRAM_WS
  // restore previous wait-states
  {
    UInt32 cpuID;

    // get the processor ID
    FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
    switch (cpuID & sysFtrNumProcessorMask)
    {
      case sysFtrNumProcessorEZ:

#define DRAMCONTROL ((unsigned short *)0xFFFFFC02)

           *DRAMCONTROL = globals.cpu.dramState;

#undef DRAMCONTROL

           break;

      case sysFtrNumProcessorVZ:
      case sysFtrNumProcessorSuperVZ:

#define DRAMCONTROL ((unsigned short *)0xFFFFFC02)

           *DRAMCONTROL = globals.cpu.dramState;

#undef DRAMCONTROL

           break;

      // no wait states :(
      default:
      case sysFtrNumProcessor328:
           break;
    }
  }
#endif

  // are we running on sony?
#if SONY_NATIVE
  if (globals.sony.device)
  {
    HRWinScreenMode(globals.sony.libRef,
                    winScreenModeSetToDefaults, NULL, NULL, NULL, NULL);

    // close the hires library
    HRClose(globals.sony.libRef);
  }
#endif

  // must have active window
  WinSetDrawWindow(WinGetDisplayWindow());

  if (DeviceSupportsColor())
  {
    // clear the LCD screen (dont want palette flash)
    WinSetPattern(&erase);
    WinFillRectangle(&rect, 0);
  }

  // restore the current display state
  if (DeviceSupportsVersion(romVersion3))
    WinScreenMode(winScreenModeSet,NULL,NULL,&globals.depthState,NULL);
}

/**
 * Get the supported depths the device can handle.
 *
 * @return the depths supported (1011b = 2^3 | 2^1 | 2^0 = 4,2,1 bpp).
 */
UInt32
DeviceGetSupportedDepths()
{
  UInt32 result = 0x00000001;

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3))
    WinScreenMode(winScreenModeGetSupportedDepths,NULL,NULL,&result,NULL);

  return result;
}

/**
 * Determine the pointer to the bitmap data chunk for a specific window.
 *
 * @param win the window.
 * @return a pointer to the bitmap data chunk.
 */
void *
DeviceWindowGetPointer(WinHandle win)
{
  void *result = NULL;

  // palmos 3.5        - use BmpGetBits()
  if (DeviceSupportsVersion(romVersion3_5))
    result = BmpGetBits(WinGetBitmap(win));

  // palmos pre 3.5    - use standard technique
  else
    result = (void *)win->displayAddrV20;

  return result;
}

/**
 * Check if the device is compatable with a particular ROM version.
 *
 * @param version the ROM version to compare against.
 * @return true if it is compatable, false otherwise.
 */
Boolean
DeviceSupportsVersion(UInt32 version)
{
  UInt32 romVersion;

  // get the rom version for this device
  FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);

  return (romVersion >= version);
}

/**
 * Check if the device is color capable.
 *
 * @return true if it is capable, false otherwise..
 */
Boolean
DeviceSupportsColor()
{
  // return the color state
  return globals.colorState;
}
