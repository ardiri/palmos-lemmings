/*
 * @(#)levelpack.h
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

#ifndef _LEVELPACK_H
#define _LEVELPACK_H

#include "palm.h"

extern void      LevelPackInitialize(PreferencesType *prefs)          __DEVICE__;
extern Boolean   LevelPackExists(UInt8 packType, UInt8 *strLevelPack) __DEVICE__;
extern void      LevelPackOpen(UInt8 packType, UInt8 *strLevelPack)   __DEVICE__;
extern MemHandle LevelPackGetResource(UInt32 type, UInt16 id)         __DEVICE__;
extern void      LevelPackReleaseResource(MemHandle memHandle)        __DEVICE__;
extern void      LevelPackClose()                                     __DEVICE__;
extern void      LevelPackTerminate()                                 __DEVICE__;

#endif
