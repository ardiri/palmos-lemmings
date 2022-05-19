/*
 * @(#)graphics.h
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

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "palm.h"

extern Boolean   GraphicsInitialize()                               __DEVICE__;
extern void      GraphicsWideScreen(Boolean)                        __DEVICE__;
extern WinHandle GraphicsGetDrawWindow()                            __DEVICE__;
extern void      GraphicsClear()                                    __DEVICE__;
extern void      GraphicsRepaint()                                  __DEVICE__;
#if SHOW_Y_UPDATE_BARS
extern void      GraphicsRepaintShowBars()                          __DEVICE__;
#endif
extern UInt16    GraphicsGetOffset()                                __DEVICE__;
extern void      GraphicsSetOffset(Int16)                           __DEVICE__;
#if !FULL_SCREEN_BLIT
extern void      GraphicsSetUpdate(Int16, Int16)                    __DEVICE__;
#endif
extern void      GraphicsTerminate()                                __DEVICE__;

#endif 
