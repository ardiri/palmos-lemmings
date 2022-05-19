/*
 * @(#)device.h
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

#ifndef _DEVICE_H
#define _DEVICE_H

#include "palm.h"

#define romVersion1   sysMakeROMVersion(1,0,0,sysROMStageDevelopment,0)
#define romVersion2   sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)
#define romVersion3   sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define romVersion3_1 sysMakeROMVersion(3,1,0,sysROMStageDevelopment,0)
#define romVersion3_2 sysMakeROMVersion(3,2,0,sysROMStageDevelopment,0)
#define romVersion3_5 sysMakeROMVersion(3,5,0,sysROMStageDevelopment,0)
#define romVersion4   sysMakeROMVersion(4,0,0,sysROMStageDevelopment,0)
#define romVersion4_1 sysMakeROMVersion(4,1,0,sysROMStageDevelopment,0)
#define romVersion5   sysMakeROMVersion(5,0,0,sysROMStageDevelopment,0)

#define sysFtrNumProcessorVZ      0x00030000
#define sysFtrNumProcessorSuperVZ 0x00040000

enum
{
  graySet = 0,
  grayGet
};

//
// "gDevice" type structure used within window memory management, which
// was replaced with the BitmapType structure in palmos 3.5 - this is a
// serious hack to get around the SDK lack of info (*grin*)
//
//   winPtr->bitmapP == winPtr->gDeviceP;           [same memory space]
//
// these structures change between OS version *eek* nasty stuff to hack
// around, but, its possible *grin*
//
// -- Aaron Ardiri, 2001
//

typedef struct
{
  void           *baseAddr;     // window base address
  UInt16         width;         // width in pixels
  UInt16         height;        // height in pixels
  UInt16         rowBytes;      // rowBytes of display
  UInt8          pixelSize;     // bits/pixel
  UInt8          version;       // version

  UInt16         forDriver:1;
  UInt16         dynamic:1;
  UInt16         compressed:1;
  UInt16         flags:13;      // gDevice flags

  ColorTableType *cTableP;      // color table, if NIL, default
} GDeviceType;

extern Boolean DeviceCheckCompatability()                           __DEVICE__;
extern void    DeviceInitialize()                                   __DEVICE__;
extern Boolean DeviceSupportsGrayscale()                            __DEVICE__;
extern void    DeviceGrayscale(UInt16, UInt8 *, UInt8 *)            __DEVICE__;
extern void    DevicePlaySound(SndCommandType *)                    __DEVICE__;
extern void    DeviceSetVolume(UInt16)                              __DEVICE__;
extern UInt16  DeviceGetVolume()                                    __DEVICE__;
extern void    DeviceSetMute(Boolean)                               __DEVICE__;
extern Boolean DeviceGetMute()                                      __DEVICE__;
extern UInt32  DeviceGetSupportedDepths()                           __DEVICE__;
extern Boolean DeviceSupportsVersion(UInt32)                        __DEVICE__;
extern void    *DeviceWindowGetPointer(WinHandle)                   __DEVICE__;
extern Boolean DeviceSupportsColor()                                __DEVICE__;
extern void    DeviceTerminate()                                    __DEVICE__;

#endif
