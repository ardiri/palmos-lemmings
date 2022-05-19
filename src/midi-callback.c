/*
 * @(#)midi-callback.c
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

#include "midi-callback.h"

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
 * MIDI callback function (native arm)
 *
 * @param userData   pointer to the MIDI data
 * @param stream     the audio stream
 * @param buffer     audio buffer to write into
 * @param frameCount the number of frames to generate.
 * @return errNone if no error, error code otherwise.
 */
Err
callback(void *userData, SndStreamRef stream, void *buffer, UInt32 frameCount)
{
  UInt32 *voiceData;
  UInt32 *engineData;
  UInt32  currentFrame;

  Int16  *tempBuff;
  UInt32  smpLocalPtr;
  UInt32  voiceNumber, voiceDataOffset;
  Int16   mixValue;

  UInt8  *TrackPtr;
  UInt8   currentChannel;
  UInt8   currentByte, currentNote, currentVelocity;
  UInt8   currentMIDICommand, currentMetaCommand;
  UInt32  deltaTime;

  UInt8  *NoteOnPtr;
  UInt32  NoteOnStatus;

#ifdef FILTER_ON
  Int32  *buf0, *buf1, *buf2, *buf3, *buf4;
  Int32   f, q, p;
  Int32   MoogFrequency, MoogResonance;
  Int32   bla1, bla2, bla3;
  MoogFrequency = __read_unaligned32(engineData+cutoffFrequency);
  MoogResonance = __read_unaligned32(engineData+resonance);
  q = 255 - MoogFrequency;
  p = MoogFrequency + ((208 * MoogFrequency * q)>>16);
  f = p + p - 255;
  q = (MoogResonance * (255 + ((128 * q * (255 - q + ((1000 * q * q) >> 16))) >> 16))) >> 8;
#endif

  // init new callback
  engineData =  userData;
  voiceData  =  userData;
  voiceData  += sizeofvariableblock;

  tempBuff     = buffer;
  currentFrame = frameCount;

  do // new sample
  {
    // add one sample worth of microseconds to our MIDI timer
    *(engineData+microSecCounter) = *(engineData+microSecCounter) + microSecPerSample;
    // if it is time to perform a new MIDI command, do it :)
    if ((*(engineData+microSecCounter))>(*(engineData+deltaTimeMicroSec))) goto MIDI_DO_COMMAND;

MIDI_PLAYBACK:

#ifdef FILTER_ON
    buf0 = engineData+filterBuffer0Voice0;
    buf1 = engineData+filterBuffer1Voice0;
    buf2 = engineData+filterBuffer2Voice0;
    buf3 = engineData+filterBuffer3Voice0;
    buf4 = engineData+filterBuffer4Voice0;
#endif

    voiceNumber = polyphony;
    mixValue    = 0;

    voiceDataOffset = voiceNumber*sizeofvoicedata;
    do // for every voice
    {
      smpLocalPtr = *(voiceData+voiceDataOffset+samplePtr);

      // only get new value if not end of sample
      if ((smpLocalPtr>>16) < *(voiceData+voiceDataOffset+sampleSize) && (*(voiceData+voiceDataOffset+noteFrequency)))
      {
#ifdef FILTER_ON
        // EMULATE ADAPTED MOOG FILTER
        Int32 t1, t2;

        in    =((Int32)((*(UInt8 *)(*(voiceData+voiceDataOffset+sampleBasePtr)+(smpLocalPtr>>16))))-128)<<4;
        in   -= ((q * (*buf4))>>8);
        t1    = *buf1;
        *buf1 = (((in + (*buf0)) * p)>>8) - (((*buf1) * f)>>8);
        t2    = *buf2;
        *buf2 = ((((*buf1) + t1) * p)>>8) - (((*buf2) * f)>>8);
        t1    = *buf3;
        *buf3 = ((((*buf2) + t2) * p)>>8) - (((*buf3) * f)>>8);
        *buf4 = ((((*buf3) + t1) * p)>>8) - (((*buf4) * f)>>8);
        *buf0 = in;

        // MOOG CLIPPING EMULATION
        // *buf4 = (*buf4) - (((*buf4) * (*buf4) * (*buf4) * 42)>>24);

        // FILTER OUTPUT BLOCK
        // Lowpass  output:  buf4
        // Highpass output:  in - buf4;
        // Bandpass output:  768 * (buf3 - buf4);

        mixValue += (Int16)(*buf4); // use Lowpass out
#else
        // NO FILTER
        mixValue += (Int16)((*(UInt8 *)(*(voiceData+voiceDataOffset+sampleBasePtr)+(smpLocalPtr>>16))))-128;
#endif
        smpLocalPtr+=*(voiceData+voiceDataOffset+noteFrequency);
      }

      // store incremented local sample pointer
      *(voiceData+voiceDataOffset+samplePtr) = (UInt32)smpLocalPtr;
      voiceDataOffset -= sizeofvoicedata;

#ifdef FILTER_ON
      buf0++; buf1++;
#endif
    }
    while (--voiceNumber);

    // pump the value up to 16-bit, coz we've got a 16-bit stream
    mixValue <<= 4;  // 4 = 50% volume
    // put the final mixed value into the buffer
    *tempBuff++ = (Int16)mixValue;
  }
  while (--currentFrame);

  return errNone;

// ---------------------------------------------------------

MIDI_DO_COMMAND:

  // reset counter
  *(engineData+microSecCounter) = 0;

  // get pointer to track data
  TrackPtr = (UInt8 *)(*(engineData+track0Ptr));

  // get new command
  currentByte = *TrackPtr++;
  currentMIDICommand = currentByte;

  // NOTE ON
  if ((currentMIDICommand & 0xf0) == 0x90)
  {
    currentChannel  = (currentMIDICommand ^ 0x90);
    currentNote     = *TrackPtr++;
    currentVelocity = *TrackPtr++;
    NoteOnPtr = (UInt8 *)(engineData+MIDINoteOnTablePtr);

    // if there was a voice playing the exact same note, kill it (can't strike the same key twice)
    NoteOnStatus = (UInt32)*(UInt8 *)(NoteOnPtr+(currentChannel<<7)+currentNote);
    if (NoteOnStatus && (currentChannel != 9)) // is there a channel playing this note ?
    { // yep
      *(UInt32 *)(voiceData+(NoteOnStatus-1)*sizeofvoicedata+sampleBasePtr) = *(engineData+instrumentSampleTablePtr); // instrument 0, we just need pointer that we know is valid, but we don't actually play it.
      *(UInt32 *)(voiceData+(NoteOnStatus-1)*sizeofvoicedata+samplePtr)     = (UInt32)0;
      *(UInt32 *)(voiceData+(NoteOnStatus-1)*sizeofvoicedata+sampleSize)    = (UInt32)0;
      *(UInt32 *)(voiceData+(NoteOnStatus-1)*sizeofvoicedata+noteFrequency) = (UInt32)0;

      // mark the note as not playing (just in case velocity=0)
      *(UInt8 *)(NoteOnPtr+(currentChannel<<7)+currentNote)= 0;
    }

    if (currentVelocity)
    { // can we hear the "new" note at all ? (actually used by Yamaha synths as NOTE OFF)

      // get next vaild voice
      *(engineData+currentVoice) = *(engineData+currentVoice) + 1;
      if ((*(engineData+currentVoice)) == polyphony) *(engineData+currentVoice) = 0;

      // mark the note belonging to this channel as belonging to this new voice.
      *(UInt8 *)(NoteOnPtr+(currentChannel<<7)+currentNote)= *(engineData+currentVoice)+1;

      // check channel type (10(9)=drum, other=regular instrument)
      if (currentChannel != 9)
      { // regular instrument
        // set up voice structure (sample, sample size, reset local pointer, set frequency)
        *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+sampleBasePtr) = *(engineData+instrumentSampleTablePtr+(*(engineData+MIDIPatchTablePtr+currentChannel)));
        *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+sampleSize)    = *(engineData+instrumentSampleSizeTablePtr+(*(engineData+MIDIPatchTablePtr+currentChannel)));
        *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+samplePtr)     = (UInt32)0;
        *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+noteFrequency) = *(engineData+NoteFrequencyTablePtr+currentNote);
      }
      else
      { // drum instrument
        if (*(engineData+drumSampleTablePtr+currentNote))
        { // sample exists
          // set up voice structure (sample, sample size, reset local pointer, set frequency)
          *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+sampleBasePtr) = *(engineData+drumSampleTablePtr+currentNote);
          *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+sampleSize)    = *(engineData+drumSampleSizeTablePtr+currentNote);
          *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+samplePtr)     = 0;
          *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+noteFrequency) = 32768;
        }
        else
        { // sample does not exist
          // set up bogus voice structure (some sample, 0 size, reset local pointer, 0 frequency)
          *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+sampleBasePtr) = *(engineData+instrumentSampleTablePtr); // instrument 0, we just need pointer that we know is valid, but we don't actually play it.
          *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+sampleSize)    = (UInt32)0;
          *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+samplePtr)     = (UInt32)0;
          *(UInt32 *)(voiceData+(*(engineData+currentVoice))*sizeofvoicedata+noteFrequency) = (UInt32)0;
        }

        // we're not quite finished yet, coz there are some special cases
        // closed hi-hats stop open-hi-hats for example...
        // check for open hi-hats when we start playing closed hi-hats
        if ((currentNote == 43) || (currentNote == 45))
        { // we're closing the hi-hats
          // check for open hi-hats
          if (*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47)))
          { // found hi-hats 1 left open, so close them
            // kill the voice playing the open hi-hats
            *(UInt32 *)(voiceData+(*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47))-1)*sizeofvoicedata+sampleBasePtr) = *(engineData+instrumentSampleTablePtr); // instrument 0, we just need pointer that we know is valid, but we don't actually play it.
            *(UInt32 *)(voiceData+(*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47))-1)*sizeofvoicedata+sampleSize)    = (UInt32)0;
            *(UInt32 *)(voiceData+(*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47))-1)*sizeofvoicedata+samplePtr)     = (UInt32)0;
            *(UInt32 *)(voiceData+(*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47))-1)*sizeofvoicedata+noteFrequency) = (UInt32)0;
            // mark them as not playing anymore
            *(UInt8 *)(NoteOnPtr+(currentChannel<<7)+47)=0;
          }
          else
          if (*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+49)))
          { // found hi-hats 2 left open, so close them
            // kill the voice playing the open hi-hats
            *(UInt32 *)(voiceData+(*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47))-1)*sizeofvoicedata+sampleBasePtr) = *(engineData+instrumentSampleTablePtr); // instrument 0, we just need pointer that we know is valid, but we don't actually play it.
            *(UInt32 *)(voiceData+(*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47))-1)*sizeofvoicedata+sampleSize)    = (UInt32)0;
            *(UInt32 *)(voiceData+(*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47))-1)*sizeofvoicedata+samplePtr)     = (UInt32)0;
            *(UInt32 *)(voiceData+(*((UInt8 *)(NoteOnPtr+(currentChannel<<7)+47))-1)*sizeofvoicedata+noteFrequency) = (UInt32)0;
            // mark them as not playing anymore
            *(UInt8 *)(NoteOnPtr+(currentChannel<<7)+49)=0;
          }
        }
      }
    }

    goto MIDI_DO_COMMAND_END;
  }

  // NOTE OFF
  if ((currentMIDICommand & 0xf0) == 0x80)
  {
    currentChannel  = (currentMIDICommand ^ 0x80);
    currentNote     = *TrackPtr++;
    currentVelocity = *TrackPtr++;
    NoteOnPtr = (UInt8 *)(engineData+MIDINoteOnTablePtr);
    NoteOnStatus = (UInt32)*(UInt8 *)(NoteOnPtr+(currentChannel<<7)+currentNote);

    if (NoteOnStatus) // is there actually a channel playing this note ?
    { // yep
      *(UInt32 *)(voiceData+(NoteOnStatus-1)*sizeofvoicedata+sampleBasePtr) = *(engineData+instrumentSampleTablePtr); // instrument 0, we just need pointer that we know is valid, but we don't actually play it.
      *(UInt32 *)(voiceData+(NoteOnStatus-1)*sizeofvoicedata+samplePtr)     = (UInt32)0;
      *(UInt32 *)(voiceData+(NoteOnStatus-1)*sizeofvoicedata+sampleSize)    = (UInt32)0;
      *(UInt32 *)(voiceData+(NoteOnStatus-1)*sizeofvoicedata+noteFrequency) = (UInt32)0;

      // mark the note as not playing
      *(UInt8 *)(NoteOnPtr+(currentChannel<<7)+currentNote)=0;
    }

    goto MIDI_DO_COMMAND_END;
  }

  // patch change
  if ((currentMIDICommand & 0xf0) == 0xc0)
  {
    currentChannel = (currentMIDICommand ^ 0xc0);
    *(engineData+MIDIPatchTablePtr+currentChannel) = (UInt32)*TrackPtr++;
    goto MIDI_DO_COMMAND_END;
  }

  // controller message
  if ((currentMIDICommand & 0xf0) == 0xb0)
  {
    currentChannel = (currentMIDICommand ^ 0xb0);
    TrackPtr += 2; // not implemented
    goto MIDI_DO_COMMAND_END;
  }

  // aftertouch
  if ((currentMIDICommand & 0xf0) == 0xa0)
  {
    currentChannel = (currentMIDICommand ^ 0xa0);
    TrackPtr += 2; // not implemented
    goto MIDI_DO_COMMAND_END;
  }

  // other commands
  switch (currentMIDICommand)
  {
    case 0xff: // meta event

         currentByte = *TrackPtr++;
         currentMetaCommand = currentByte;
         switch (currentMetaCommand)
         {
           case 0x51: // set tempo
                {
                  UInt32 deltaTimePerQuarterNote;
                  TrackPtr++; // should be 0x03

                  //new deltaTimePQN
                  deltaTimePerQuarterNote = ((UInt32)TrackPtr[0] << 16) +
                                            ((UInt32)TrackPtr[1] <<  8) +
                                             (UInt32)TrackPtr[2];
                  TrackPtr += 3;
                  *(engineData+oneTickMicroSec) =
                     deltaTimePerQuarterNote/(*(engineData+PPQNDivision)); // one tick is this many micro secs
                }
                break;

           case 0x58: // time signature
                TrackPtr++; // should be 0x04
                TrackPtr += 4; // not implemented
                break;

           case 0x59: // key signature
                TrackPtr++; // should be 0x02
                TrackPtr += 2; // not implemented
                break;

           case 0x01: // text
                {
                  UInt8 textLength = *TrackPtr++;
                  TrackPtr += textLength; // not implemented
                }
                break;

           case 0x02: // text
                {
                  UInt8 textLength = *TrackPtr++;
                  TrackPtr += textLength; // not implemented
                }
                break;

           case 0x03: // text
                {
                  UInt8 textLength = *TrackPtr++;
                  TrackPtr += textLength; // not implemented
                }
                break;

           case 0x04: // text
                {
                  UInt8 textLength = *TrackPtr++;
                  TrackPtr += textLength; // not implemented
                }
              break;

           case 0x05: // text
                {
                  UInt8 textLength = *TrackPtr++;
                  TrackPtr += textLength; // not implemented
                }
                break;

           case 0x06: // text
                {
                  UInt8 textLength = *TrackPtr++;
                  TrackPtr += textLength; // not implemented
                }
                break;

           case 0x07: // text
                {
                  UInt8 textLength = *TrackPtr++;
                  TrackPtr += textLength; // not implemented
                }
                break;

           case 0x54: // SMPTE
                TrackPtr++; // should be 0x05
                TrackPtr += 5; // not implemented
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

MIDI_DO_COMMAND_END:

  // check for end of MIDI
  if (((UInt32)TrackPtr-(*(engineData+trackStartPtr)))>=(*(engineData+track0Length)))
    {
     // reset tempo to default 120 BPM
     *(engineData+oneTickMicroSec)=500000/(*(engineData+PPQNDivision));
     // reset track ptr
     TrackPtr = (UInt8 *)(*(engineData+trackStartPtr));
    }

  // get a new delta time to wait for
  deltaTime = 0;
  do
  {
    deltaTime <<= 7;
    currentByte = *TrackPtr++;
    deltaTime += (currentByte & 127);
  }
  while ((currentByte & 128));
  *(engineData+deltaTimeMicroSec) = (UInt32)(deltaTime)*(*(engineData+oneTickMicroSec));
  *(engineData+track0Ptr)         = (UInt32)TrackPtr;

goto MIDI_PLAYBACK;

}
