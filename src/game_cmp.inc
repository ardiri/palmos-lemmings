/*
 * @(#)game_cmp.inc
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

/**
 * Inflate a RLE compressed data chunk into an uncompressed chunk.
 *
 * @param ptrData       the source data buffer.
 * @param ptrOutput     the output data buffer.
 * @param ptrOutputSize the size of the source data buffer.
 */
static void
GameEncodingInflate(UInt8  *ptrData,
                    UInt8  *ptrOutput,
                    UInt16 ptrOutputSize)
{
  UInt16 ptrDataSize;

  ptrDataSize = MemPtrSize(ptrData) >> 1;

  // lets make sure our "pointers" are valid
  if ((ptrData  != NULL) && (ptrOutput != NULL) && (ptrDataSize != 0))
  {
    UInt8 data, freq;

    // do all RLE chunks
    do
    {
      // extract the block info
      data = *ptrData++;
      freq = *ptrData++;

      if (freq > ptrOutputSize)
      {
        freq = ptrOutputSize;
        if (freq == 0) break;
      }

      ptrOutputSize -= freq;

      // extract XXXX bytes
      do
      {
        *ptrOutput++ = data;
      }
      while (--freq);
    }
    while (--ptrDataSize);
  }
}

/**
 * Deflate an uncompressed data chunk into a RLE compressed chunk.
 *
 * @param ptrData     the source data buffer.
 * @param ptrDataSize the size of the source data buffer.
 * @param ptrOutput   the output data buffer.
 */
static void
GameEncodingDeflate(UInt8  *ptrData,
                    UInt16 ptrDataSize,
                    UInt8  *ptrOutput)
{
  UInt16 ptrOutputSize;

  ptrOutputSize = MemPtrSize(ptrOutput);

  // lets make sure our "pointers" are valid
  if ((ptrData  != NULL) && (ptrOutput != NULL) && (ptrDataSize != 0))
  {
    UInt8  data, freq, data2;

    // encoding!!
    data  = *ptrData++;
    freq  = 1;
    while (--ptrDataSize)
    {
      data2 = *ptrData++;
      // same data, increase RLE entry
      if (data == data2)
      {
        // max RLE = 255
        if (++freq == 0)
        {
          // save the state
          *ptrOutput++ = data;
          *ptrOutput++ = 255;

          freq    = 1;
        }
      }

      // different data, new RLE entry
      else
      {
        // save the state
        *ptrOutput++ = data;
        *ptrOutput++ = freq;

        freq    = 1;
      }

      data = data2;
    }

    *ptrOutput++ = data;
    *ptrOutput++ = freq;
  }
}

/**
 * Calculate the size of the output buffer required when making a call to
 * the function GameEncodingInflate() with a specific pointer of memory.
 *
 * @param ptrData the source data buffer.
 * @return the size of the inflated buffer
 */
static UInt16
GameEncodingInflateLength(UInt8 *ptrData)
{
  UInt16 result = 0;
  UInt16 ptrDataSize;

  ptrDataSize = MemPtrSize(ptrData) >> 1;

  // lets make sure our "pointer" is valid
  if ((ptrData != NULL) && (ptrDataSize != 0))
  {
    // count the bytes
    do {
      ptrData++;
      result += *ptrData++;    // add up all the RLE counts
    } while (--ptrDataSize);
  }

  return result;
}

/**
 * Calculate the size of the output buffer required when making a call to
 * the function GameEncodingDeflate() with a specific pointer of memory.
 *
 * @param ptrData     the source data buffer.
 * @param ptrDataSize the size of the source data buffer.
 * @return the size of the deflated buffer
 */
static UInt16
GameEncodingDeflateLength(UInt8 *ptrData, UInt16 ptrDataSize)
{
  UInt16 result = 0;

  // lets make sure our "pointer" is valid
  if ((ptrData != NULL) && (ptrDataSize > 0))
  {
    UInt8  data, freq, data2;

    // setup time :)
    result = 2;

    // encoding!!
    data  = *ptrData++;
    freq  = 1;
    while (--ptrDataSize)
    {
      data2 = *ptrData++;
      // same data, increase RLE entry
      if (data == data2)
      {
        // max RLE = 255
        if (++freq == 0)
        {
          freq    = 1;
          result += 2; // two more bytes
        }
      }

      // different data, new RLE entry
      else
      {
        freq    = 1;
        result += 2; // two more bytes
      }
      data = data2;
    }
  }

  return result;
}
