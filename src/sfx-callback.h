/*
 * @(#)sfx-callback.h
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

#ifndef __SFX_CALLBACK_H
#define __SFX_CALLBACK_H

//
// arm native helper macros
//

#define __read_unaligned32(addr)  \
        ( ((((unsigned char *)(addr))[0]) << 24) | \
          ((((unsigned char *)(addr))[1]) << 16) | \
          ((((unsigned char *)(addr))[2]) <<  8) | \
          ((((unsigned char *)(addr))[3])) )

#define __write_byte32(addr, value) \
	( ((unsigned char *)(addr))[3] = (unsigned char)((unsigned long)(value) >> 24), \
	  ((unsigned char *)(addr))[2] = (unsigned char)((unsigned long)(value) >> 16), \
	  ((unsigned char *)(addr))[1] = (unsigned char)((unsigned long)(value) >>  8), \
	  ((unsigned char *)(addr))[0] = (unsigned char)((unsigned long)(value)) )

//
// sfx parsing constants
//

// engine data offset pointers
#define waveBasePtr                  0
#define waveLocalPtr                 1
#define waveSize                     2

#endif

