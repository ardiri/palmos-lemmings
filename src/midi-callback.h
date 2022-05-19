/*
 * @(#)midi-callback.h
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

#ifndef __MIDI_CALLBACK_H
#define __MIDI_CALLBACK_H

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
// midi parsing constants
//

#define polyphony                    16
#define sizeofvoicedata              4
#define latency                      1024
#define sampleRate                   22050
#define microSecPerCallback          1000000/(sampleRate/latency)
#define microSecPerSample            1000000/(sampleRate)

#define sizeofvariableblock          1536

// engine data offset pointers
#define filterBuffer0Voice0          0
#define filterBuffer0Voice15         15
#define filterBuffer1Voice0          16
#define filterBuffer1Voice15         31
#define filterBuffer2Voice0          32
#define filterBuffer2Voice15         47
#define filterBuffer3Voice0          48
#define filterBuffer3Voice15         63
#define filterBuffer4Voice0          64
#define filterBuffer4Voice15         79
#define cutoffFrequency              80
#define resonance                    81
#define microSecCounter              82
#define track0Length                 83
#define track0Ptr                    84
#define deltaTimeMicroSec            85
#define oneTickMicroSec              86
#define PPQNDivision                 87
#define currentVoice                 88
#define trackStartPtr                89

#define instrumentSampleTablePtr     128
#define instrumentSampleSizeTablePtr 256
#define drumSampleTablePtr           384
#define drumSampleSizeTablePtr       512
#define MIDIPatchTablePtr            640
#define NoteFrequencyTablePtr        656
#define MIDINoteOnTablePtr           1024

#define sampleBasePtr                0
#define samplePtr                    1
#define noteFrequency                2
#define sampleSize                   3

#endif

