/*
 * @(#)sfx-callback.c
 *
 * Copyright 2001-2003, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                      Charles Kerchner (mailto:chip@ardiri.com)
 *                      Ivo Jager        (mailto:ivo@ardiri.com)
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

#include "sfx-callback.h"

//
// arm native data types
//

#define NULL           (void *)0
#define FALSE          0
#define TRUE           !FALSE

#define true           TRUE
#define false          FALSE

typedef unsigned int   UInt32;
typedef unsigned short UInt16;
typedef unsigned char  UInt8;
typedef signed   int   Int32;
typedef signed   short Int16;
typedef signed   char  Int8;

typedef UInt16         Err;
typedef UInt32         SndStreamRef;

#define errNone        0

/**
 * SFX callback function (native arm)
 *
 * @param userData   pointer to the SFX data
 * @param stream     the audio stream
 * @param buffer     audio buffer to write into
 * @param frameCount the number of frames to generate.
 * @return errNone if no error, error code otherwise.
 */
Err
callback(void *userData, SndStreamRef stream, void *buffer, UInt32 frameCount)
{
  UInt32 *waveData;
  UInt32  currentFrame;
  Int16  *tempBuff, mixValue;

  // init new callback
  waveData     = userData;
  tempBuff     = buffer;
  currentFrame = frameCount;

  // fill new buffer
  do
  {
    if (*(waveData+waveBasePtr) != 0)
    {
      if ((*(waveData+waveLocalPtr))<(*(waveData+waveSize)))
      {
        mixValue = *(UInt8 *)(*(waveData+waveBasePtr)+*(waveData+waveLocalPtr));
        *(waveData+waveLocalPtr) = *(waveData+waveLocalPtr) + 1;
      }
      else
      {
        mixValue = 0x80;
        *(waveData+waveBasePtr) = 0;   // end of sample, void it.
      }
    }
    else
      mixValue = 0x80;

    // pump the value up to 16-bit, coz we've got a 16-bit stream
    mixValue = ((Int16)(mixValue-0x80)) << 7; // convert from unsigned to signed
    *tempBuff++ = (Int16)(mixValue);
  }
  while (--currentFrame);

  return errNone;
}
