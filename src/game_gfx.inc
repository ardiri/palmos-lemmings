/*
 * @(#)game_gfx.inc
 *
 * Copyright 2001-2002, Aaron Ardiri     (mailto:aaron@ardiri.com),
 *                      Charles Kerchner (mailto:chip@ardiri.com)
 * All rights reserved.
 *
 * This file was generated as part of the  "lemmings" program developed 
 * for the Palm Computing Platform designed by Palm: 
 *
 *   http://www.palm.com/ 
 *
 * The contents of this file is confidential and proprietrary in nature 
 * ("Confidential Information"). distribution or modification without 
 * prior consent of the original author is prohibited. 
 */

static 
UInt8 ptrConvertLevel[16]     = { 
                                  1, 1, 2, 3, 2, 3, 2, 1,
                                  1, 2, 2, 2, 3, 3, 3, 0
/*
 * original table
 *
                                  3, 1, 1, 2, 3, 2, 3, 3,
                                  3, 3, 2, 2, 2, 1, 1, 0   */
                                };

#if USE_CHIP_COMPRESS
static 
UInt8 ptrConvertGameBase[16]  = { 
                                  0, 1, 1, 2, 3, 2, 3, 3, 
                                  0, 0, 0, 0, 0, 0, 0, 3   // 8..14 not used
                                };

#if PORTABLE

static 
UInt8 reverseByte[256] = {
                           0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 
                           0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
                           0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 
                           0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
                           0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 
                           0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
                           0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 
                           0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
                           0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 
                           0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
                           0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 
                           0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, 
                           0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 
                           0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
                           0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 
                           0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 
                           0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 
                           0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, 
                           0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 
                           0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
                           0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 
                           0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, 
                           0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 
                           0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, 
                           0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 
                           0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
                           0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
                           0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 
                           0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 
                           0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
                           0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 
                           0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
                         };

#define nextbit                     \
  if ((UInt8)(c >>= 1) <= 1) {      \
    c = 0x100 | *src++;             \
  }

#define DumpOutBits(out, ch)        \
    if (pos++ & 1) {                \
        *out++ |= ch;               \
    } else {                        \
        *out = (ch << 4);           \
    }

#define DumpOutBits2(out, ch)       \
    ch2 = ptrConvertLevel[ch];      \
    num = (pos++ & 3);              \
    if (num == 0) {                 \
        *out = (ch2 << 6);          \
    } else if (num == 1) {          \
        *out |= (ch2 << 4);         \
    } else if (num == 2) {          \
        *out |= (ch2 << 2);         \
    } else {                        \
        *out++ |= ch2;              \
    }

#define DumpMaskBits(out, ch)       \
    if (pos++ & 1) {                \
        *out++ |= ch;               \
    } else {                        \
        *out = (ch << 4);           \
    }

#define DumpMaskBits2(out, ch)      \
    ch2 = ptrConvertGameBase[ch];   \
    num = (pos++ & 3);              \
    if (num == 0) {                 \
        *out = (ch2 << 6);          \
    } else if (num == 1) {          \
        *out |= (ch2 << 4);         \
    } else if (num == 2) {          \
        *out |= (ch2 << 2);         \
    } else {                        \
        *out++ |= ch2;              \
    }

#define COMP_TYPE_HUFF        0x00
#define COMP_TYPE_LZ77        0x01

#define OUTPUT_BIT_COUNT      3
#define OUTPUT_BIT_COUNT_MASK 4
#define INDEX_BIT_COUNT       16
#define LENGTH_BIT_COUNT      9
#define BREAK_EVEN            ((1 + INDEX_BIT_COUNT + LENGTH_BIT_COUNT) / \
                                          (OUTPUT_BIT_COUNT + 1))
#define BREAK_EVEN_MASK       ((1 + INDEX_BIT_COUNT + LENGTH_BIT_COUNT) / \
                                       (OUTPUT_BIT_COUNT_MASK + 1))
#define END_OF_STREAM         0
#endif
#endif

/**
 * Load the grayscale background (convert from 4bpc).
 * 
 * @param memHandle a reference to the 4bpc bitmap resource.
 * @param size      the size of the 4bpc bitmap resource.
 */
static void 
GameLoadBackgroundBitmap2bpp(MemHandle memHandle, UInt32 size)
{
#if !USE_CHIP_COMPRESS
  UInt8  *ptrBuffer, *ptrScreen;
  UInt16 i, count;
  UInt8  hi, lo;

  // get pointers to everything
  ptrScreen = globals.ptrWinDraw + (SPR_HEIGHT * 160);
  ptrBuffer = (UInt8 *)MemHandleLock(memHandle);

  i = 0; hi = 0; lo = 0;
  while (i < size) 
  {
    if (ptrBuffer[i] == 0xff)
    {
      count = ptrBuffer[++i]; i++;

      // got a partial write in progress - complete it
      if (hi != 0) 
      {
        *(ptrScreen++) = ((ptrConvertLevel[hi >> 4]   << 6) | 
                          (ptrConvertLevel[hi & 0x0f] << 4) |  // high byte
                          0x00);                               // low byte
        count--;
      }
      hi = 0xff; lo = 0x0ff; 

      // do the remainding set of pairs
      MemSet(ptrScreen, (count >> 1), 0x00);
      ptrScreen += (count >> 1); count = count % 2;

      // do we have a "remainder" left over? prepare for it.
      if (count == 0) hi = 0; lo = 0; 
    }
    else
    {
      if (hi == 0) hi = ptrBuffer[i++];
      else         lo = ptrBuffer[i++];
    }

    // if (hi | lo) are not 0, need to write data
    if ((hi != 0) && (lo != 0))
    {
      *(ptrScreen++) = ((ptrConvertLevel[hi >> 4]   << 6) |
                        (ptrConvertLevel[hi & 0x0f] << 4) |  // high byte
                        (ptrConvertLevel[lo >> 4]   << 2) |
                        (ptrConvertLevel[lo & 0x0f]));       // low byte
      hi = 0; lo = 0;
    }
  }

  MemHandleUnlock(memHandle);
#else
  UInt8 *dest, *src;
#if PORTABLE
  UInt16 c;
#endif

  // get pointers to everything
  dest = globals.ptrWinDraw + (SPR_HEIGHT * 160);
  src  = (UInt8 *)MemHandleLock(memHandle);

#if PORTABLE
  c = *src++;
  if (c == COMP_TYPE_HUFF) 
  {
    UInt8 *dest2, cur, tog, col, tg2, cur4;
    UInt16 cnt, num;

    cur   = 0;
    cur4  = ptrConvertLevel[cur];
    tog   = 1;
    col   = 1;
    tg2   = 2;
    c     = 0;

    while (1) 
    {
      nextbit;
      if (c & 1)                    // 1...
      {
        nextbit;
        if (c & 1)                  // 11...
        {
          col  = cur;               // Toggle
          cur  = tog;
          tog  = col;
          cur4 = ptrConvertLevel[cur];

          *dest = (*dest << 2) | cur4;
          if ((--size & 3) == 0)
            dest++;

          continue;
        }
        else                        // 10...
        {
          num = (UInt16)(-1);       // Duplication
          do 
          {
            num++;
            nextbit;
          } 
          while (!(c & 1));
          cnt = 1;
          if (num) 
          {
            do 
            {
              nextbit;
              cnt += cnt + (c & 1);
            } 
            while (--num);
          }

          if (col == cur) 
          {
            cur  = tog;
            tog  = col;
            cur4 = ptrConvertLevel[cur];

            cnt++;
          }

          col  = (size & 3);
          size -= cnt;
          if (col)
          {
            do
            {
              cnt--;
              *dest = (*dest << 2) | cur4;
              if (--col == 0)
              {
                dest++;
                break;
              }
            }
            while (cnt);
          }

          col  = (cur4 << 6) | (cur4 << 4) | (cur4 << 2) | cur4;
          num  = cnt >> 2;
          if (num) 
          {
            do 
            {
              *dest++ = col;
            } 
            while (--num);
          }

          cnt &= 3;
          if (cnt) 
            *dest = col >> (8 - (cnt * 2));

          col  = cur;
          continue;
        }
      }
      else                          // 0...
      {
        nextbit;
        if (c & 1)                  // 01...
        {
          nextbit;
          if (c & 1)                // 011...
          {
            col  = tog;             // Toggle2
            tog  = cur;
            cur  = tg2;
            tg2  = col;
            cur4 = ptrConvertLevel[cur];

            *dest = (*dest << 2) | cur4;
            if ((--size & 3) == 0)
              dest++;

            continue;
          }
          else                      // 010...
          {
            tg2  = tog;             // Literal
            col  = tog = cur;

            nextbit;
            cur  = (c & 1);
            nextbit;
            cur += cur + (c & 1);
            nextbit;
            cur += cur + (c & 1);
            nextbit;
            cur += cur + (c & 1);
            cur4 = ptrConvertLevel[cur];

            if (size == 0) break;

            *dest = (*dest << 2) | cur4;
            if ((--size & 3) == 0)
              dest++;

            continue;
          }
        }
        else                        // 00...
        {
          col = (UInt8)(-1);        // Copying
          do 
          {
            col++;
            nextbit;
          } 
          while (!(c & 1));
          cnt = 1;
          if (col) 
          {
            do 
            {
              nextbit;
              cnt += cnt + (c & 1);
            } 
            while (--col);
          }

          dest2 = dest - (OFFSCREEN_WIDTH / 4);
          num   = (size & 3);
          size -= cnt;
          if (num)
          {
            do
            {
              cnt--;
              *dest = (*dest << 2) | ((*dest2 >> (--num * 2)) & 0x3);
              if (num == 0)
              {
                dest2++;
                dest++;
                break;
              }
            }
            while (cnt);
          }

          num = cnt >> 2;
          if (num) 
          {
            do 
            {
              *dest++ = *dest2++;
            } 
            while (--num);
          }

          cnt &= 3;
          if (cnt) 
            *dest = *dest2 >> (8 - (cnt * 2));

          col = (UInt8)(-1);
          continue;
        }
      }
    }
  } 
  else 
  if (c == COMP_TYPE_LZ77) 
  {
    UInt16 pos, ch2, ch3, num;

    c = 1;
    pos = 0;
    while (1) 
    {
      nextbit;
      if (c & 1) 
      {
        ch3 = 0;
        num = OUTPUT_BIT_COUNT + 1;
        do 
        {
          nextbit;
          ch3 += ch3 + (c & 1);
        } 
        while (--num);
        DumpOutBits2(dest, ch3);
      } 
      else 
      {
        UInt16 match_length, match_position;
        UInt8 *match;

        match_position = 0;
        num = INDEX_BIT_COUNT;
        do 
        {
          nextbit;
          if (c & 0x100) break;
          match_position += match_position + (c & 1);
        } 
        while (--num);

        c = reverseByte[c & 0xff] | 0x100;
        match_position += match_position + (c & 1);
        if (--num) 
        {
          do 
          {
            nextbit;
            match_position += match_position + (c & 1);
          } 
          while (--num);
        }

        if (match_position == END_OF_STREAM) break;

        num = (UInt16)(-1);
        do 
        {
          num++;
          nextbit;
        } 
        while (!(c & 1));

        match_length = 1;
        if (num) 
        {
          do 
          {
            nextbit;
            match_length += match_length + (c & 1);
          } 
          while (--num);
        }

        match_length += BREAK_EVEN;

        num = match_length & 3;
        match_length >>= 2;
        ch3 = (match_position & 3);
        ch2 = (pos & 3);
        if (ch2 == 3) 
        {
          match = dest - (match_position >> 2);
          if (ch3 == 3) 
          {
            ch3 = *match++;
            *dest++ |= (ch3 >> 6);
            do 
            {
              *dest++ = ((pos = *match++) >> 6) | (ch3 << 2);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 2) & 0xf0; } 
            else if (num == 2) { *dest = (ch3 << 2) & 0xc0; } 
            else if (num == 0) { *--dest &= 0xfc; }
          } 
          else 
          if (ch3 == 2) 
          {
            ch3 = *match++;
            *dest++ |= ((ch3 >> 4) & 0x03);
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 4); } 
            else if (num == 2) { *dest = (ch3 << 4) & 0xc0; } 
            else if (num == 0) { *--dest &= 0xfc; }
          } 
          else 
          if (ch3 == 1) 
          {
            ch3 = *match++;
            *dest++ |= ((ch3 >> 2) & 0x03);
            do 
            {
              *dest++ = ((pos = *match++) >> 2) | (ch3 << 6);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = ((ch3 << 6) | (*match >> 2)) & 0xf0; } 
            else if (num == 2) { *dest = ((ch3 << 6) | (*match >> 2)) & 0xc0; }
            else if (num == 0) { *--dest &= 0xfc; }
          } 
          else 
          {
            *dest++ |= (*match++ & 0x03);
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);

                 if (num == 3) { *dest = *match & 0xf0; } 
            else if (num == 2) { *dest = *match & 0xc0; } 
            else if (num == 0) { *--dest &= 0xfc; }
          }
          pos = (ch2 + num) & 3;
        } 
        else 
        if (ch2 == 2) 
        {
          match = dest - ((match_position + 1) >> 2);
          if (ch3 == 3) 
          {
            ch3 = *match++;
            pos = *match++;
            *dest++ |= ((ch3 << 2) & 0x0c) | (pos >> 6);
            do 
            {
              ch3 = pos;
              *dest++ = ((pos = *match++) >> 6) | (ch3 << 2);
            } 
            while (--match_length);

                 if (num == 3) { *dest = (pos << 2) & 0xc0; } 
            else if (num == 1) { *--dest &= 0xfc; } 
            else if (num == 0) { *--dest &= 0xf0; }
          } 
          else 
          if (ch3 == 2) 
          {
            ch3 = *match++;
            *dest++ |= (ch3 >> 4);
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 4) & 0xc0; } 
            else if (num == 1) { *--dest &= 0xfc; } 
            else if (num == 0) { *--dest &= 0xf0; }
          } 
          else 
          if (ch3 == 1) 
          {
            ch3 = *match++;
            *dest++ |= ((ch3 >> 2) & 0x0f);
            do 
            {
              *dest++ = ((pos = *match++) >> 2) | (ch3 << 6);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 6); } 
            else if (num == 1) { *--dest &= 0xfc; } 
            else if (num == 0) { *--dest &= 0xf0; }
          } 
          else 
          {
            *dest++ |= (*match++ & 0x0f);
            do 
            {
              *dest++ = *match++;
            } while (--match_length);

                 if (num == 3) { *dest = *match & 0xc0; } 
            else if (num == 1) { *--dest &= 0xfc; } 
            else if (num == 0) { *--dest &= 0xf0; }
          }
          pos = (ch2 + num) & 3;
        } 
        else 
        if (ch2 == 1) 
        {
          match = dest - ((match_position + 2) >> 2);
          if (ch3 == 3)
          {
            ch3 = *match++;
            pos = *match++;
            *dest++ |= ((ch3 << 2) & 0x3c) | (pos >> 6);
            do 
            {
              ch3 = pos;
              *dest++ = ((pos = *match++) >> 6) | (ch3 << 2);
            } 
            while (--match_length);
          } 
          else 
          if (ch3 == 2) 
          {
            ch3 = *match++;
            pos = *match++;
            *dest++ |= ((ch3 << 4) & 0x30) | (pos >> 4);
            do 
            {
              ch3 = pos;
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
            } 
            while (--match_length);
          } 
          else 
          if (ch3 == 1) 
          {
            ch3 = *match++;
            *dest++ |= (ch3 >> 2);
            do 
            {
              *dest++ = ((pos = *match++) >> 2) | (ch3 << 6);
              ch3 = pos;
            } 
            while (--match_length);
          } 
          else 
          {
            *dest++ |= (*match++ & 0x3f);
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);
          }
          pos = (ch2 + num) & 3;

               if (num == 2) { *--dest &= 0xfc; } 
          else if (num == 1) { *--dest &= 0xf0; } 
          else if (num == 0) { *--dest &= 0xc0; }
        } 
        else 
        {
          match = dest - ((match_position + 3) >> 2);
          if (ch3 == 3) 
          {
            ch3 = *match++;
            do 
            {
              *dest++ = ((pos = *match++) >> 6) | (ch3 << 2);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 2); } 
            else if (num == 2) { *dest = (ch3 << 2) & 0xf0; } 
            else if (num == 1) { *dest = (ch3 << 2) & 0xc0; }
          } 
          else 
          if (ch3 == 2) 
          {
            ch3 = *match++;
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = ((ch3 << 4) | (*match >> 4)) & 0xfc; } 
            else if (num == 2) { *dest = (ch3 << 4); } 
            else if (num == 1) { *dest = (ch3 << 4) & 0xc0; }
          } 
          else 
          if (ch3 == 1) 
          {
            ch3 = *match++;
            do 
            {
              *dest++ = ((pos = *match++) >> 2) | (ch3 << 6);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = ((ch3 << 6) | (*match >> 2)) & 0xfc; } 
            else if (num == 2) { *dest = ((ch3 << 6) | (*match >> 2)) & 0xf0; }
            else if (num == 1) { *dest = (ch3 << 6); }
          } 
          else 
          {
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);

                 if (num == 3) { *dest = *match & 0xfc; } 
            else if (num == 2) { *dest = *match & 0xf0; } 
            else if (num == 1) { *dest = *match & 0xc0; }
          }
          pos = num;
        }
      }
    }
  }
#else
  // push all registers on stack
  asm("      movem.l %%d0-%%d7/%%a0-%%a5, -(%%sp)": :);

  asm("      move.l  %0,      %%d7          | size
             move.l  %3,      %%a2          | ptrConvertLevel
             move.l  %1,      %%a0          | src
             move.l  %2,      %%a1          | dest
             moveq   #1,      %%d4          | c
             move.b  (%%a0)+, %%d3
             bne     GameLoadBackgroundBitmap2bpp_lz_start
             moveq   #0,      %%d0          | cur
             move.b  (%%a2, %%d0), %%d6     | cur4
             move.l  #1,      %%d3          | col
             move.l  %%d3,    %%a3          | tog
             move.l  #2,      %%a5          | tg2
             lsr.w   #1,      %%d4
             bcs.b   GameLoadBackgroundBitmap2bpp_bit1_reload

  GameLoadBackgroundBitmap2bpp_again2:

             move.q  #16,     %%d3

  GameLoadBackgroundBitmap2bpp_again:

             lsr.w   #1,      %%d4
             bcc     GameLoadBackgroundBitmap2bpp_bit0
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_skip

  GameLoadBackgroundBitmap2bpp_bit1_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc     GameLoadBackgroundBitmap2bpp_bit0

  GameLoadBackgroundBitmap2bpp_bit1_skip:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit10
             bne.b   GameLoadBackgroundBitmap2bpp_bit11_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit10

  GameLoadBackgroundBitmap2bpp_bit11_skip:

             move.l  %%d0,    %%d3          | col = cur
             exg     %%d0,    %%a3          | cur <-> tog
             subq.l  #1,      %%d7          | Toggle
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             move.b  (%%a2, %%d0), %%d6
             add.b   %%d6,    %%d1
             move.w  %%d7,    %%d2
             and.w   #3,      %%d2
             bne.b   GameLoadBackgroundBitmap2bpp_again
             move.b  %%d1,    (%%a1)+
             bra.b   GameLoadBackgroundBitmap2bpp_again

  GameLoadBackgroundBitmap2bpp_bit10:

             moveq   #0,      %%d2          | num

  GameLoadBackgroundBitmap2bpp_bit10_zero:

             addq.w  #1,      %%d2          | Duplication
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit10_zero
             bne.b   GameLoadBackgroundBitmap2bpp_bit10_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit10_zero

  GameLoadBackgroundBitmap2bpp_bit10_zero_skip:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d2
             bcs.b   GameLoadBackgroundBitmap2bpp_bit10_count_skip
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundBitmap2bpp_bit10_count_again
             bne.b   GameLoadBackgroundBitmap2bpp_bit10_count_skip2
             bra.b   GameLoadBackgroundBitmap2bpp_bit10_count_reload

  GameLoadBackgroundBitmap2bpp_bit10_count_again:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundBitmap2bpp_bit10_count_again
             bne.b   GameLoadBackgroundBitmap2bpp_bit10_count_skip2

  GameLoadBackgroundBitmap2bpp_bit10_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d2,    GameLoadBackgroundBitmap2bpp_bit10_count_again

  GameLoadBackgroundBitmap2bpp_bit10_count_skip2:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundBitmap2bpp_bit10_count_skip:

             cmp.w   %%d3,    %%d0
             bne.b   GameLoadBackgroundBitmap2bpp_bit10_tog_skip
             exg     %%d0,    %%a3          | cur <-> tog
             move.b  (%%a2, %%d0), %%d6
             addq.w  #1,      %%d5

  GameLoadBackgroundBitmap2bpp_bit10_tog_skip:

             move.w  %%d7,    %%d2
             sub.l   %%d5,    %%d7
             move.l  %%d0,    %%d3
             and.w   #3,      %%d2
             beq.b   GameLoadBackgroundBitmap2bpp_bit10_move2
             subq.w  #1,      %%d5

  GameLoadBackgroundBitmap2bpp_bit10_move_again:

             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             add.b   %%d6,    %%d1
             subq.w  #1,      %%d2
             dbeq    %%d5,    GameLoadBackgroundBitmap2bpp_bit10_move_again
             bne     GameLoadBackgroundBitmap2bpp_again
             move.b  %%d1,    (%%a1)+
             tst.w   %%d5
             beq     GameLoadBackgroundBitmap2bpp_again

  GameLoadBackgroundBitmap2bpp_bit10_move2:

             move.b  %%d6,    %%d1
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             add.b   %%d6,    %%d1
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             add.b   %%d6,    %%d1
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             add.b   %%d6,    %%d1
             lsr.w   #2,      %%d5
             beq     GameLoadBackgroundBitmap2bpp_again
             subq.w  #1,      %%d5

  GameLoadBackgroundBitmap2bpp_bit10_move2_again:

             move.b  %%d1,    (%%a1)+
             dbra    %%d5,    GameLoadBackgroundBitmap2bpp_bit10_move2_again
             bra     GameLoadBackgroundBitmap2bpp_again

  GameLoadBackgroundBitmap2bpp_bit0:

             lsr.w   #1,      %%d4
             bcc     GameLoadBackgroundBitmap2bpp_bit00
             bne.b   GameLoadBackgroundBitmap2bpp_bit0_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit00

  GameLoadBackgroundBitmap2bpp_bit0_skip:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit010
             bne.b   GameLoadBackgroundBitmap2bpp_bit01_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit010

  GameLoadBackgroundBitmap2bpp_bit01_skip:

             move.l  %%a3,    %%d3          | col = tog
             exg     %%d0,    %%a5          | cur <-> tg2
             exg     %%a3,    %%a5          | tog <-> tg2
             subq.l  #1,      %%d7          | Toggle2
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             move.b  (%%a2, %%d0), %%d6
             add.b   %%d6,    %%d1
             move.w  %%d7,    %%d2
             and.w   #3,      %%d2
             bne     GameLoadBackgroundBitmap2bpp_again
             move.b  %%d1,    (%%a1)+
             bra     GameLoadBackgroundBitmap2bpp_again

  GameLoadBackgroundBitmap2bpp_bit010:

             move.l  %%a3,    %%a5          | tg2 = tog
             move.l  %%d0,    %%d3          | col = cur
             move.l  %%d0,    %%a3          | tog = cur
             moveq   #0,      %%d0          | cur = 0x00
             lsr.w   #1,      %%d4          | Literal
             bne.b   GameLoadBackgroundBitmap2bpp_bit010_skip1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap2bpp_bit010_skip1:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap2bpp_bit010_skip2
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap2bpp_bit010_skip2:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap2bpp_bit010_skip3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap2bpp_bit010_skip3:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap2bpp_bit010_skip4
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap2bpp_bit010_skip4:

             addx.b  %%d0,    %%d0
             subq.l  #1,      %%d7
             jcs     GameLoadBackgroundBitmap2bpp_exit
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             move.b  (%%a2, %%d0), %%d6
             add.b   %%d6,    %%d1
             move.w  %%d7,    %%d2
             and.w   #3,      %%d2
             bne     GameLoadBackgroundBitmap2bpp_again
             move.b  %%d1,    (%%a1)+
             bra     GameLoadBackgroundBitmap2bpp_again

  GameLoadBackgroundBitmap2bpp_bit00:

             moveq   #0,      %%d3          | col

  GameLoadBackgroundBitmap2bpp_bit00_zero:

             addq.w  #1,      %%d3          | Copying
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit00_zero
             bne.b   GameLoadBackgroundBitmap2bpp_bit00_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_bit00_zero

  GameLoadBackgroundBitmap2bpp_bit00_zero_skip:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d3
             bcs.b   GameLoadBackgroundBitmap2bpp_bit00_count_skip
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundBitmap2bpp_bit00_count_again
             bne.b   GameLoadBackgroundBitmap2bpp_bit00_count_skip2
             bra.b   GameLoadBackgroundBitmap2bpp_bit00_count_reload

  GameLoadBackgroundBitmap2bpp_bit00_count_again:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundBitmap2bpp_bit00_count_again
             bne.b   GameLoadBackgroundBitmap2bpp_bit00_count_skip2

  GameLoadBackgroundBitmap2bpp_bit00_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d3,    GameLoadBackgroundBitmap2bpp_bit00_count_again

  GameLoadBackgroundBitmap2bpp_bit00_count_skip2:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundBitmap2bpp_bit00_count_skip:

             lea     -160(%%a1), %%a4
             move.w  %%d7,    %%d2
             sub.l   %%d5,    %%d7
             and.w   #3,      %%d2
             beq.b   GameLoadBackgroundBitmap2bpp_bit00_move2
             move.b  (%%a4)+, %%d3
             add.b   %%d2,    %%d2
             ror.b   %%d2,    %%d3
             subq.w  #1,      %%d5
             
  GameLoadBackgroundBitmap2bpp_bit00_move_again:

             add.b   %%d3,    %%d3
             addx.b  %%d1,    %%d1
             add.b   %%d3,    %%d3
             addx.b  %%d1,    %%d1
             subq.w  #2,      %%d2
             dbeq    %%d5,    GameLoadBackgroundBitmap2bpp_bit00_move_again
             bne     GameLoadBackgroundBitmap2bpp_again2
             move.b  %%d1,    (%%a1)+
             tst.w   %%d5
             beq     GameLoadBackgroundBitmap2bpp_again2

  GameLoadBackgroundBitmap2bpp_bit00_move2:

             move.w  %%d5,    %%d2
             lsr.w   #2,      %%d2
             beq.b   GameLoadBackgroundBitmap2bpp_bit00_finish
             subq.w  #1,      %%d2

  GameLoadBackgroundBitmap2bpp_bit00_move2_again:

             move.b  (%%a4)+, (%%a1)+
             dbra    %%d2,    GameLoadBackgroundBitmap2bpp_bit00_move2_again

  GameLoadBackgroundBitmap2bpp_bit00_finish:

             and.w   #3,      %%d5
             add.w   %%d5,    %%d5
             move.b  (%%a4),  %%d1
             rol.b   %%d5,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_again

  GameLoadBackgroundBitmap2bpp_lz_start:

             moveq   #0,      %%d2          | pos
             move.l  %%d2,    %%d3

  GameLoadBackgroundBitmap2bpp_lz_again3:

             move.b  (%%a3),  %%d1

  GameLoadBackgroundBitmap2bpp_lz_again2:

             move.l  %%d2,    %%d0
             add.b   %%d0,    %%d0
             rol.b   %%d0,    %%d1

  GameLoadBackgroundBitmap2bpp_lz_again:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_lz_bit1
             bne.b   GameLoadBackgroundBitmap2bpp_lz_bit0
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_lz_bit1

  GameLoadBackgroundBitmap2bpp_lz_bit0:

             moveq   #0,      %%d0          | ch3
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap2bpp_lz_bit0_skip1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit0_skip1:

             addx.w  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap2bpp_lz_bit0_skip2
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit0_skip2:

             addx.w  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap2bpp_lz_bit0_skip3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit0_skip3:

             addx.w  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap2bpp_lz_bit0_skip4
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit0_skip4:

             addx.w  %%d0,    %%d0
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             add.b   (%%a2, %%d0), %%d1
             addq.w  #1,      %%d2
             and.w   #3,      %%d2
             bne.b   GameLoadBackgroundBitmap2bpp_lz_again
             move.b  %%d1,    (%%a1)+
             bra.b   GameLoadBackgroundBitmap2bpp_lz_again

  GameLoadBackgroundBitmap2bpp_lz_bit1:

             moveq   #0,      %%d3          | match_position
             subq.w  #1,      %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip9_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip10_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip11_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip12_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip13_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip14_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip15_skip
             addx.w  %%d3,    %%d3
             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip16

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip9_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip9

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip10_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip10

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip11_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip11

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip12_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip12

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip13_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip13

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip14_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip14

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip15_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_skip15

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip9:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip10:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip11:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip12:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip13:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip14:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip15:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap2bpp_lz_bit1_skip16:

             addx.w  %%d3,    %%d3
             tst.w   %%d3
             beq     GameLoadBackgroundBitmap2bpp_exit
             moveq   #0,      %%d5          | num

  GameLoadBackgroundBitmap2bpp_lz_bit1_zero:

             addq.w  #1,      %%d5
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_lz_bit1_zero
             bne.b   GameLoadBackgroundBitmap2bpp_lz_bit1_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap2bpp_lz_bit1_zero

  GameLoadBackgroundBitmap2bpp_lz_bit1_zero_skip:

             moveq   #1,      %%d6          | match_length
             subq.w  #2,      %%d5
             bcs.b   GameLoadBackgroundBitmap2bpp_lz_bit1_count_skip2
             lsr.w   #1,      %%d4
             dbeq    %%d5,    GameLoadBackgroundBitmap2bpp_lz_bit1_count_again
             bne.b   GameLoadBackgroundBitmap2bpp_lz_bit1_count_skip
             bra.b   GameLoadBackgroundBitmap2bpp_lz_bit1_count_reload

  GameLoadBackgroundBitmap2bpp_lz_bit1_count_again:

             addx.w  %%d6,    %%d6
             lsr.w   #1,      %%d4
             dbeq    %%d5,    GameLoadBackgroundBitmap2bpp_lz_bit1_count_again
             bne.b   GameLoadBackgroundBitmap2bpp_lz_bit1_count_skip

  GameLoadBackgroundBitmap2bpp_lz_bit1_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d5,    GameLoadBackgroundBitmap2bpp_lz_bit1_count_again

  GameLoadBackgroundBitmap2bpp_lz_bit1_count_skip:

             addx.w  %%d6,    %%d6

  GameLoadBackgroundBitmap2bpp_lz_bit1_count_skip2:

             addq.w  #6,      %%d6
             move.l  %%d6,    %%d5
             and.w   #3,      %%d5
             lsr.w   #2,      %%d6
             subq.w  #1,      %%d6
             move.l  %%d3,    %%d7
             move.l  %%a1,    %%a3
             btst.b  #0,      %%d2
             bne     GameLoadBackgroundBitmap2bpp_bit1_lz_pos_low
             btst.b  #1,      %%d2
             bne     GameLoadBackgroundBitmap2bpp_bit1_lz_pos_high
             move.w  %%d5,    %%d2
             addq.w  #3,      %%d7
             lsr.w   #2,      %%d7
             suba.l  %%d7,    %%a3
             btst.b  #0,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_zero_low
             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_zero_high

  GameLoadBackgroundBitmap2bpp_bit1_lz_zero_zero:

             move.b  (%%a3)+, (%%a1)+
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_zero_zero
             bra     GameLoadBackgroundBitmap2bpp_lz_again3

  GameLoadBackgroundBitmap2bpp_bit1_lz_zero_low:

             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_zero_high2
             move.b  (%%a3)+, %%d0
             ror.b   #2,      %%d0
             moveq   #63,     %%d3

  GameLoadBackgroundBitmap2bpp_bit1_lz_zero_one:

             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #192,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_zero_one
             move.b  (%%a3),  %%d1
             lsr.b   #2,      %%d1
             and.b   #192,    %%d0
             or.b    %%d0,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_zero_high:

             move.b  (%%a3)+, %%d0
             ror.b   #4,      %%d0
             moveq   #15,     %%d3

  GameLoadBackgroundBitmap2bpp_bit1_lz_zero_two:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #240,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_zero_two
             move.b  (%%a3),  %%d1
             lsr.b   #4,      %%d1
             and.b   #240,    %%d0
             or.b    %%d0,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_zero_high2:

             move.b  (%%a3)+, %%d0
             add.b   %%d0,    %%d0
             add.b   %%d0,    %%d0
             moveq   #3,      %%d3

  GameLoadBackgroundBitmap2bpp_bit1_lz_zero_three:

             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #252,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_zero_three
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_pos_high:

             add.w   %%d5,    %%d2
             and.w   #3,      %%d2
             addq.w  #1,      %%d7
             lsr.w   #2,      %%d7
             suba.l  %%d7,    %%a3
             lsl.b   #4,      %%d1
             btst.b  #0,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_two_low
             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_two_high
             move.b  (%%a3)+, %%d0
             and.b   #15,     %%d0
             or.b    %%d0,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_two_zero:

             move.b  (%%a3)+, (%%a1)+
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_two_zero
             sub.b   #2,      %%d5
             bcc     GameLoadBackgroundBitmap2bpp_lz_again3
             move.b  -(%%a1), %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_two_low:

             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_two_high2
             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   #15,     %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+
             moveq   #63,     %%d3

  GameLoadBackgroundBitmap2bpp_bit1_lz_two_one:

             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #192,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_two_one
             sub.b   #2,      %%d5
             bcc     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_two_high:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.l  %%d7,    %%d0
             and.b   #15,     %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+
             moveq   #15,     %%d3

  GameLoadBackgroundBitmap2bpp_bit1_lz_two_two:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #240,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_two_two
             sub.b   #2,      %%d5
             bcc     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_two_high2:

             moveq   #3,      %%d3
             move.b  (%%a3)+, %%d7
             and.b   %%d3,    %%d7
             add.b   %%d7,    %%d7
             add.b   %%d7,    %%d7
             or.b    %%d7,    %%d1
             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_two_three:

             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #252,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_two_three
             sub.b   #2,      %%d5
             bcc     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_pos_low:

             btst.b  #1,      %%d2
             bne     GameLoadBackgroundBitmap2bpp_bit1_lz_pos_high2
             add.w   %%d5,    %%d2
             and.w   #3,      %%d2
             addq.w  #2,      %%d7
             lsl.b   #6,      %%d1
             lsr.w   #2,      %%d7
             suba.l  %%d7,    %%a3
             btst.b  #0,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_one_low
             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_one_high
             move.b  (%%a3)+, %%d7
             and.b   #63,     %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_one_zero:

             move.b  (%%a3)+, (%%a1)+
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_one_zero
             sub.b   #3,      %%d5
             bcc     GameLoadBackgroundBitmap2bpp_lz_again2
             move.b  -(%%a1), %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_one_low:

             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_one_high2
             moveq   #63,     %%d3
             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_one_one:

             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #192,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_one_one
             sub.b   #3,      %%d5
             bcc     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_one_high:

             moveq   #15,     %%d3
             move.b  (%%a3)+, %%d7
             and.b   #3,      %%d7
             lsl.b   #4,      %%d7
             or.b    %%d7,    %%d1
             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_one_two:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #240,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_one_two
             sub.b   #3,      %%d5
             bcc     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_one_high2:

             moveq   #3,      %%d3
             move.b  (%%a3)+, %%d7
             and.b   #15,     %%d7
             add.b   %%d7,    %%d7
             add.b   %%d7,    %%d7
             or.b    %%d7,    %%d1
             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_one_three:

             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #252,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_one_three
             sub.b   #3,      %%d5
             bcc     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_pos_high2:

             add.w   %%d5,    %%d2
             and.w   #3,      %%d2
             lsr.w   #2,      %%d7
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             suba.l  %%d7,    %%a3
             btst.b  #0,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_three_low
             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_three_high
             move.b  (%%a3)+, %%d7
             and.b   #3,      %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_three_zero:

             move.b  (%%a3)+, (%%a1)+
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_three_zero
             tst.b   %%d5
             bne     GameLoadBackgroundBitmap2bpp_lz_again3
             move.b  -(%%a1), %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_three_low:

             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundBitmap2bpp_bit1_lz_three_high2
             moveq   #63,     %%d3
             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   #3,      %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_three_one:

             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #192,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_three_one
             move.b  (%%a3),  %%d1
             lsr.b   #2,      %%d1
             and.b   #192,    %%d0
             or.b    %%d0,    %%d1
             tst.b   %%d5
             bne     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_three_high:

             moveq   #15,     %%d3
             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.l  %%d7,    %%d0
             and.b   #3,      %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_three_two:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #240,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_three_two
             tst.b   %%d5
             bne     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_bit1_lz_three_high2:

             moveq   #3,      %%d3
             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap2bpp_bit1_lz_three_three:

             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #252,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundBitmap2bpp_bit1_lz_three_three
             tst.b   %%d5
             bne     GameLoadBackgroundBitmap2bpp_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundBitmap2bpp_lz_again2

  GameLoadBackgroundBitmap2bpp_exit:

  ": : "r" (size),
       "r" (src),
       "r" (dest),
       "r" (ptrConvertLevel));

  // pop all registers off stack
  asm("      movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a5": :);
#endif

  MemHandleUnlock(memHandle);
#endif
}


/**
 * Load a 4bpc (16 color) bitmap into the offscreen buffer.
 * 
 * @param memHandle a reference to the 4bpc bitmap resource.
 * @param size      the size of the 4bpc bitmap resource.
 */
static void
GameLoadBackgroundBitmap4bpc(MemHandle memHandle, UInt32 size)
{
#if !USE_CHIP_COMPRESS
  UInt8  *ptrBuffer, *ptrScreen;
  UInt16 i;

  // get pointers to everything
  ptrScreen = globals.ptrWinDraw + (SPR_HEIGHT * 320);
  ptrBuffer = (UInt8 *)MemHandleLock(memHandle);

  i = 0;
  while (i < size) 
  {
    if (ptrBuffer[i] == 0xff)
    {
      MemSet(ptrScreen, ptrBuffer[++i], 0xff); 
      ptrScreen += ptrBuffer[i++];
    }
    else
      *(ptrScreen++) = ptrBuffer[i++];
  }

  MemHandleUnlock(memHandle);
#else
  UInt8 *dest, *src;
#if PORTABLE
  UInt16 c;
#endif

  // get pointers to everything
  dest = globals.ptrWinDraw + (SPR_HEIGHT * 320);
  src  = (UInt8 *)MemHandleLock(memHandle);

#if PORTABLE
  c = *src++;
  if (c == COMP_TYPE_HUFF) 
  {
    UInt8 *dest2, cur, tog, col, tg2;
    UInt16 cnt, num;

    cur   = 0;
    tog   = 1;
    col   = 1;
    tg2   = 2;
    c     = 0;

    while (1) 
    {
      nextbit;
      if (c & 1)                    // 1...
      {
        nextbit;
        if (c & 1)                  // 11...
        {
          col  = cur;               // Toggle
          cur  = tog;
          tog  = col;

          if (size & 1) 
            *dest++ |= cur;
          else 
            *dest    = cur << 4;

          size--;
          continue;
        }
        else                        // 10...
        {
          num = (UInt16)(-1);       // Duplication
          do 
          {
            num++;
            nextbit;
          } 
          while (!(c & 1));
          cnt = 1;
          if (num) 
          {
            do 
            {
              nextbit;
              cnt += cnt + (c & 1);
            } 
            while (--num);
          }

          if (col == cur) 
          {
            cur  = tog;
            tog  = col;

            cnt++;
          }

          num  = cnt;
          if (size & 1) 
          {
            *dest++ |= cur;
             cnt--;
          }
          size -= num;

          col  = (cur << 4) | cur;
          num = cnt >> 1;
          if (num) 
          {
            do 
            {
              *dest++ = col;
            } 
            while (--num);
          }

          if (cnt & 1) 
            *dest = col & 0xf0;

          col  = cur;
          continue;
        }
      }
      else                          // 0...
      {
        nextbit;
        if (c & 1)                  // 01...
        {
          nextbit;
          if (c & 1)                // 011...
          {
            col  = tog;             // Toggle2
            tog  = cur;
            cur  = tg2;
            tg2  = col;

            if (size & 1)
              *dest++ |= cur;
            else 
              *dest    = cur << 4;

            size--;
            continue;
          }
          else                      // 010...
          {
            tg2  = tog;             // Literal
            col  = tog = cur;

            nextbit;
            cur  = (c & 1);
            nextbit;
            cur += cur + (c & 1);
            nextbit;
            cur += cur + (c & 1);
            nextbit;
            cur += cur + (c & 1);

            if (size & 1) 
              *dest++ |= cur;
            else 
            {
              if (size == 0) break;
              *dest    = cur << 4;
            }
            size--;
            continue;
          }
        }
        else                        // 00...
        {
          col = (UInt8)(-1);        // Copying
          do 
          {
            col++;
            nextbit;
          } 
          while (!(c & 1));
          cnt = 1;
          if (col) 
          {
            do 
            {
              nextbit;
              cnt += cnt + (c & 1);
            } 
            while (--col);
          }

          num  = cnt;
          dest2 = dest - (OFFSCREEN_WIDTH / 2);
          if (size & 1) 
          {
            *dest++ |= (*dest2++ & 0x0f);
            cnt--;
          }
          size -= num;

          num = cnt >> 1;
          if (num) 
          {
            do 
            {
              *dest++ = *dest2++;
            } 
            while (--num);
          }

          if (cnt & 1) 
          {
            *dest = *dest2 & 0xf0;
          }

          col = (UInt8)(-1);
          continue;
        }
      }
    }
  } 
  else 
  if (c == COMP_TYPE_LZ77)  
  {
    UInt16 pos, c, ch3, num;

    c = 1;
    pos = 0;
    while (1) 
    {
      nextbit;
      if (c & 1) 
      {
        ch3 = 0;
        num = OUTPUT_BIT_COUNT + 1;
        do 
        {
          nextbit;
          ch3 += ch3 + (c & 1);
        } 
        while (--num);
        DumpOutBits(dest, ch3);
      } 
      else 
      {
        UInt16 match_length, match_position;
        UInt8 *match;

        match_position = 0;
        num = INDEX_BIT_COUNT;
        do 
        {
          nextbit;
          if (c & 0x100) break; 

          match_position += match_position + (c & 1);
        } 
        while (--num);

        c = reverseByte[c & 0xff] | 0x100;
        match_position += match_position + (c & 1);
        if (--num) 
        {
          do 
          {
            nextbit;
            match_position += match_position + (c & 1);
          } 
          while (--num);
        }

        if (match_position == END_OF_STREAM) break;

        num = (UInt16)(-1);
        do 
        {
          num++;
          nextbit;
        } 
        while (!(c & 1));

        match_length = 1;
        if (num) 
        {
          do  
          {
            nextbit;
            match_length += match_length + (c & 1);
          } 
          while (--num);
        }

        match_length += BREAK_EVEN;

        num = match_length & 1;
        match_length >>= 1;
        if (pos & 1) 
        {
          match = dest - (match_position >> 1);
          if (match_position & 1) 
          {
            ch3 = *match++;
            *dest++ |= (ch3 >> 4);
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);
            pos = num ^ 1;
            if (pos) *--dest &= 0xf0;
          } 
          else 
          {
            *dest++ |= (*match++ & 0x0f);
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);
            pos = num ^ 1;
            if (pos) *--dest &= 0xf0;
          }
        } 
        else 
        {
          match = dest - ((match_position + 1) >> 1);
          if (match_position & 1) 
          {
            ch3 = *match++;
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);
            pos = num;
            if (pos) *dest = (ch3 << 4);
          } 
          else 
          {
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);
            if (num) 
            {
              *dest = (*match & 0xf0);
              pos = 1;
            }
          }
        }
      }
    }
  }
#else
  // push all registers on stack
  asm("      movem.l %%d0-%%d7/%%a0-%%a4, -(%%sp)": :);

  asm("      move.l  %0,      %%d7          | size
             move.l  %1,      %%a0          | src
             move.l  %2,      %%a1          | dest
             moveq   #1,      %%d4          | c
             move.b  (%%a0)+, %%d3
             bne     GameLoadBackgroundBitmap4bpc_lz_start
             moveq   #0,      %%d0          | cur
             moveq   #1,      %%d3          | col
             move.l  %%d3,    %%a3          | tog
             moveq   #2,      %%d6          | tg2
             lsr.w   #1,      %%d4
             bcs.b   GameLoadBackgroundBitmap4bpc_bit1_reload

  GameLoadBackgroundBitmap4bpc_odd:

             or.w    %%d0,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap4bpc_odd_again:

             lsr.w   #1,      %%d4
             bcc     GameLoadBackgroundBitmap4bpc_bit0
             bne.b   GameLoadBackgroundBitmap4bpc_bit1_skip

  GameLoadBackgroundBitmap4bpc_bit1_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc     GameLoadBackgroundBitmap4bpc_bit0

  GameLoadBackgroundBitmap4bpc_bit1_skip:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit10
             bne.b   GameLoadBackgroundBitmap4bpc_bit11_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit10

  GameLoadBackgroundBitmap4bpc_bit11_skip:

             move.l  %%d0,    %%d3          | col = cur
             exg     %%d0,    %%a3          | cur <-> tog
             subq.l  #1,      %%d7          | Toggle
             bra     GameLoadBackgroundBitmap4bpc_even

  GameLoadBackgroundBitmap4bpc_bit10:

             moveq   #0,      %%d2          | num

  GameLoadBackgroundBitmap4bpc_bit10_zero:

             addq.w  #1,      %%d2          | Duplication
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit10_zero
             bne.b   GameLoadBackgroundBitmap4bpc_bit10_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit10_zero

  GameLoadBackgroundBitmap4bpc_bit10_zero_skip:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d2
             bcs.b   GameLoadBackgroundBitmap4bpc_bit10_count_skip
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundBitmap4bpc_bit10_count_again
             bne.b   GameLoadBackgroundBitmap4bpc_bit10_count_skip2
             bra.b   GameLoadBackgroundBitmap4bpc_bit10_count_reload

  GameLoadBackgroundBitmap4bpc_bit10_count_again:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundBitmap4bpc_bit10_count_again
             bne.b   GameLoadBackgroundBitmap4bpc_bit10_count_skip2

  GameLoadBackgroundBitmap4bpc_bit10_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d2,    GameLoadBackgroundBitmap4bpc_bit10_count_again

  GameLoadBackgroundBitmap4bpc_bit10_count_skip2:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundBitmap4bpc_bit10_count_skip:

             cmp.w   %%d3,    %%d0
             bne.b   GameLoadBackgroundBitmap4bpc_bit10_tog_skip
             exg     %%d0,    %%a3          | cur <-> tog
             addq.w  #1,      %%d5

  GameLoadBackgroundBitmap4bpc_bit10_tog_skip:

             sub.l   %%d5,    %%d7
             move.w  %%d0,    %%d1
             lsl.b   #4,      %%d1
             move.w  %%d0,    %%d3
             subq.w  #2,      %%d5
             bcs     GameLoadBackgroundBitmap4bpc_check
             lsr.w   #1,      %%d5
             or.w    %%d0,    %%d1

  GameLoadBackgroundBitmap4bpc_bit10_move_again:

             move.b  %%d1,    (%%a1)+
             dbra    %%d5,    GameLoadBackgroundBitmap4bpc_bit10_move_again
             sub.w   %%d0,    %%d1
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundBitmap4bpc_odd_again
             bra     GameLoadBackgroundBitmap4bpc_even_again

  GameLoadBackgroundBitmap4bpc_bit0:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit00
             bne.b   GameLoadBackgroundBitmap4bpc_bit0_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit00

  GameLoadBackgroundBitmap4bpc_bit0_skip:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit010
             bne.b   GameLoadBackgroundBitmap4bpc_bit01_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit010

  GameLoadBackgroundBitmap4bpc_bit01_skip:

             move.l  %%a3,    %%d3          | col = tog
             exg     %%d0,    %%d6          | cur <-> tg2
             exg     %%a3,    %%d6          | tog <-> tg2
             subq.l  #1,      %%d7          | Toggle2
             bra     GameLoadBackgroundBitmap4bpc_even

  GameLoadBackgroundBitmap4bpc_bit010:

             move.l  %%a3,    %%d6          | tg2 = tog
             move.l  %%d0,    %%d3          | col = cur
             move.l  %%d0,    %%a3          | tog = cur
             moveq   #0,      %%d0          | cur = 0x00
             lsr.w   #1,      %%d4          | Literal
             bne.b   GameLoadBackgroundBitmap4bpc_bit010_skip1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_bit010_skip1:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_bit010_skip2
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_bit010_skip2:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_bit010_skip3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_bit010_skip3:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_bit010_skip4
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_bit010_skip4:

             addx.b  %%d0,    %%d0
             subq.l  #1,      %%d7
             bcc.b   GameLoadBackgroundBitmap4bpc_even
             bra     GameLoadBackgroundBitmap4bpc_exit

  GameLoadBackgroundBitmap4bpc_bit00:

             moveq   #0,      %%d3

  GameLoadBackgroundBitmap4bpc_bit00_zero:

             addq.w  #1,      %%d3          | Copying
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit00_zero
             bne.b   GameLoadBackgroundBitmap4bpc_bit00_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit00_zero

  GameLoadBackgroundBitmap4bpc_bit00_zero_skip:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d3
             bcs.b   GameLoadBackgroundBitmap4bpc_bit00_count_skip
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundBitmap4bpc_bit00_count_again
             bne.b   GameLoadBackgroundBitmap4bpc_bit00_count_skip2
             bra.b   GameLoadBackgroundBitmap4bpc_bit00_count_reload

  GameLoadBackgroundBitmap4bpc_bit00_count_again:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundBitmap4bpc_bit00_count_again
             bne.b   GameLoadBackgroundBitmap4bpc_bit00_count_skip2

  GameLoadBackgroundBitmap4bpc_bit00_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d3,    GameLoadBackgroundBitmap4bpc_bit00_count_again

  GameLoadBackgroundBitmap4bpc_bit00_count_skip2:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundBitmap4bpc_bit00_count_skip:

             lea     -320(%%a1), %%a4
             sub.l   %%d5,    %%d7
             subq.w  #2,      %%d5
             bcs.b   GameLoadBackgroundBitmap4bpc_bit00_move_skip
             lsr.w   #1,      %%d5

  GameLoadBackgroundBitmap4bpc_bit00_move_again:

             move.b  (%%a4)+, (%%a1)+
             dbra    %%d5,    GameLoadBackgroundBitmap4bpc_bit00_move_again

  GameLoadBackgroundBitmap4bpc_bit00_move_skip:

             move.b  (%%a4),  %%d1
             and.w   #240,    %%d1

  GameLoadBackgroundBitmap4bpc_check:

             btst.b  #0,      %%d7
             beq     GameLoadBackgroundBitmap4bpc_odd_again
             bra.b   GameLoadBackgroundBitmap4bpc_even_again

  GameLoadBackgroundBitmap4bpc_even:

             move.l  %%d0,    %%d1          | cur2
             lsl.b   #4,      %%d1

  GameLoadBackgroundBitmap4bpc_even_again:

             lsr.w   #1,      %%d4
             bcc     GameLoadBackgroundBitmap4bpc_bit0_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit1_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit0_even

  GameLoadBackgroundBitmap4bpc_bit1_skip_even:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit10_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit11_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit10_even

  GameLoadBackgroundBitmap4bpc_bit11_skip_even:

             move.l  %%d0,    %%d3          | col = cur
             exg     %%d0,    %%a3          | cur <-> tog
             subq.l  #1,      %%d7          | Toggle
             bra     GameLoadBackgroundBitmap4bpc_odd

  GameLoadBackgroundBitmap4bpc_bit10_even:

             moveq   #0,      %%d2          | num

  GameLoadBackgroundBitmap4bpc_bit10_zero_even:

             addq.w  #1,      %%d2          | Duplication
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit10_zero_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit10_zero_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit10_zero_even

  GameLoadBackgroundBitmap4bpc_bit10_zero_skip_even:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d2
             bcs.b   GameLoadBackgroundBitmap4bpc_bit10_count_skip_even
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundBitmap4bpc_bit10_count_again_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit10_count_skip2_even
             bra.b   GameLoadBackgroundBitmap4bpc_bit10_count_reload_even

  GameLoadBackgroundBitmap4bpc_bit10_count_again_even:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundBitmap4bpc_bit10_count_again_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit10_count_skip2_even

  GameLoadBackgroundBitmap4bpc_bit10_count_reload_even:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d2,    GameLoadBackgroundBitmap4bpc_bit10_count_again_even

  GameLoadBackgroundBitmap4bpc_bit10_count_skip2_even:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundBitmap4bpc_bit10_count_skip_even:

             cmp.w   %%d3,    %%d0
             bne.b   GameLoadBackgroundBitmap4bpc_bit10_tog_skip_even
             exg     %%d0,    %%a3          | cur <-> tog
             addq.w  #1,      %%d5

  GameLoadBackgroundBitmap4bpc_bit10_tog_skip_even:

             or.w    %%d0,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundBitmap4bpc_bit10_even_even:

             sub.l   %%d5,    %%d7
             move.w  %%d0,    %%d1
             lsl.b   #4,      %%d1
             move.w  %%d0,    %%d3
             subq.w  #3,      %%d5
             bcs     GameLoadBackgroundBitmap4bpc_check_even
             lsr.w   #1,      %%d5
             or.w    %%d0,    %%d1

  GameLoadBackgroundBitmap4bpc_bit10_move_again_even:

             move.b  %%d1,    (%%a1)+
             dbra    %%d5,    GameLoadBackgroundBitmap4bpc_bit10_move_again_even
             sub.w   %%d0,    %%d1
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundBitmap4bpc_odd_again
             bra     GameLoadBackgroundBitmap4bpc_even_again

  GameLoadBackgroundBitmap4bpc_bit0_even:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit00_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit0_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit00_even

  GameLoadBackgroundBitmap4bpc_bit0_skip_even:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit010_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit01_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit010_even

  GameLoadBackgroundBitmap4bpc_bit01_skip_even:

             move.l  %%a3,    %%d3          | col = tog
             exg     %%d0,    %%d6          | cur <-> tg2
             exg     %%a3,    %%d6          | tog <-> tg2
             subq.l  #1,      %%d7          | Toggle2
             bra     GameLoadBackgroundBitmap4bpc_odd

  GameLoadBackgroundBitmap4bpc_bit010_even:

             move.l  %%a3,    %%d6          | tg2 = tog
             move.l  %%d0,    %%d3          | col = cur
             move.l  %%d0,    %%a3          | tog = cur
             moveq   #0,      %%d0          | cur = 0x00
             lsr.w   #1,      %%d4          | Literal
             bne.b   GameLoadBackgroundBitmap4bpc_bit010_skip1_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_bit010_skip1_even:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_bit010_skip2_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_bit010_skip2_even:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_bit010_skip3_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_bit010_skip3_even:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_bit010_skip4_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_bit010_skip4_even:

             addx.b  %%d0,    %%d0
             subq.l  #1,      %%d7
             bra     GameLoadBackgroundBitmap4bpc_odd

  GameLoadBackgroundBitmap4bpc_bit00_even:

             moveq   #0,      %%d3          | num

  GameLoadBackgroundBitmap4bpc_bit00_zero_even:

             addq.w  #1,      %%d3          | Copying
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit00_zero_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit00_zero_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_bit00_zero_even

  GameLoadBackgroundBitmap4bpc_bit00_zero_skip_even:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d3
             bcs.b   GameLoadBackgroundBitmap4bpc_bit00_count_skip_even
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundBitmap4bpc_bit00_count_again_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit00_count_skip2_even
             bra.b   GameLoadBackgroundBitmap4bpc_bit00_count_reload_even

  GameLoadBackgroundBitmap4bpc_bit00_count_again_even:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundBitmap4bpc_bit00_count_again_even
             bne.b   GameLoadBackgroundBitmap4bpc_bit00_count_skip2_even

  GameLoadBackgroundBitmap4bpc_bit00_count_reload_even:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d3,    GameLoadBackgroundBitmap4bpc_bit00_count_again_even

  GameLoadBackgroundBitmap4bpc_bit00_count_skip2_even:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundBitmap4bpc_bit00_count_skip_even:

             lea     -320(%%a1), %%a4
             move.b  (%%a4)+, %%d2
             and.w   #15,     %%d2
             or.w    %%d2,    %%d1
             move.b  %%d1,    (%%a1)+
             sub.l   %%d5,    %%d7
             subq.w  #3,      %%d5
             bcs.b   GameLoadBackgroundBitmap4bpc_bit00_move_skip_even
             lsr.w   #1,      %%d5

  GameLoadBackgroundBitmap4bpc_bit00_move_again_even:

             move.b  (%%a4)+, (%%a1)+
             dbra    %%d5,    GameLoadBackgroundBitmap4bpc_bit00_move_again_even

  GameLoadBackgroundBitmap4bpc_bit00_move_skip_even:

             move.b  (%%a4),  %%d1
             and.w   #240,    %%d1

  GameLoadBackgroundBitmap4bpc_check_even:

             btst.b  #0,      %%d7
             beq     GameLoadBackgroundBitmap4bpc_odd_again
             bra     GameLoadBackgroundBitmap4bpc_even_again


  GameLoadBackgroundBitmap4bpc_lz_start:

             moveq   #0,      %%d2          | pos
             moveq   #1,      %%d5
             move.l  %%d5,    %%a4
             move.w  #240,    %%d0

  GameLoadBackgroundBitmap4bpc_lz_bit0_dump_odd:

             lsl.b   #4,      %%d1
             move.b  %%d1,    (%%a1)

  GameLoadBackgroundBitmap4bpc_lz_again:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_lz_bit1
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit0
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_lz_bit1

  GameLoadBackgroundBitmap4bpc_lz_bit0:

             moveq   #0,      %%d1          | ch3
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit0_skip1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit0_skip1:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit0_skip2
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit0_skip2:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit0_skip3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit0_skip3:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit0_skip4
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit0_skip4:

             addx.w  %%d1,    %%d1
             eor.b   %%d5,    %%d2
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit0_dump_odd
             or.b    %%d1,    (%%a1)+
             bra.b   GameLoadBackgroundBitmap4bpc_lz_again

  GameLoadBackgroundBitmap4bpc_lz_bit1:

             moveq   #0,      %%d1          | match_position
             subq.w  #1,      %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip9_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip10_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip11_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip12_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip13_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip14_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip15_skip
             addx.w  %%d1,    %%d1
             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip16

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip9_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip9

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip10_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip10

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip11_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip11

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip12_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip12

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip13_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip13

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip14_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip14

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip15_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_skip15

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip9:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip10:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip11:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip12:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip13:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip14:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip15:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundBitmap4bpc_lz_bit1_skip16:

             addx.w  %%d1,    %%d1
             tst.w   %%d1
             beq     GameLoadBackgroundBitmap4bpc_exit
             moveq   #0,      %%d3          | num

  GameLoadBackgroundBitmap4bpc_lz_bit1_zero:

             addq.w  #1,      %%d3
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_lz_bit1_zero
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit1_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundBitmap4bpc_lz_bit1_zero

  GameLoadBackgroundBitmap4bpc_lz_bit1_zero_skip:

             moveq   #1,      %%d6          | match_length
             subq.w  #2,      %%d3
             bcs.b   GameLoadBackgroundBitmap4bpc_lz_bit1_count_skip2
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundBitmap4bpc_lz_bit1_count_again
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit1_count_skip
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_count_reload

  GameLoadBackgroundBitmap4bpc_lz_bit1_count_again:

             addx.w  %%d6,    %%d6
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundBitmap4bpc_lz_bit1_count_again
             bne.b   GameLoadBackgroundBitmap4bpc_lz_bit1_count_skip

  GameLoadBackgroundBitmap4bpc_lz_bit1_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d3,    GameLoadBackgroundBitmap4bpc_lz_bit1_count_again
             bra.b   GameLoadBackgroundBitmap4bpc_lz_bit1_count_skip

  GameLoadBackgroundBitmap4bpc_lz_bit1_count_skip2:

             moveq   #0,      %%d6

  GameLoadBackgroundBitmap4bpc_lz_bit1_count_skip:

             move.l  %%d6,    %%d7
             addx.w  %%d7,    %%d7
             move.l  %%a1,    %%a2
             tst.b   %%d2
             beq     GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even
             lsr.w   #1,      %%d1
             bcc.b   GameLoadBackgroundBitmap4bpc_bit1_lz_pos_odd_even
             suba.l  %%d1,    %%a2
             addq.w  #2,      %%d6
             moveq   #15,     %%d2
             move.b  (%%a2)+, %%d1
             rol.b   #4,      %%d1
             move.w  %%d1,    %%d3
             and.b   %%d2,    %%d1
             and.b   %%d0,    %%d3
             or.b    %%d1,    (%%a1)+

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_odd_odd:

             move.b  (%%a2)+, %%d1
             rol.b   #4,      %%d1
             move.w  %%d1,    %%d5
             and.b   %%d2,    %%d1
             and.b   %%d0,    %%d5
             or.b    %%d3,    %%d1
             move.b  %%d1,    (%%a1)+
             dbra    %%d6, GameLoadBackgroundBitmap4bpc_bit1_lz_pos_odd_odd_2
             move.l  %%a4,    %%d5
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             bne     GameLoadBackgroundBitmap4bpc_lz_again
             and.b   %%d0,    -(%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundBitmap4bpc_lz_again

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_odd_odd_2:

             move.b  (%%a2)+, %%d1
             rol.b   #4,      %%d1
             move.w  %%d1,    %%d3
             and.b   %%d2,    %%d1
             and.b   %%d0,    %%d3
             or.b    %%d5,    %%d1
             move.b  %%d1,    (%%a1)+
             dbra    %%d6, GameLoadBackgroundBitmap4bpc_bit1_lz_pos_odd_odd
             move.l  %%a4,    %%d5
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             bne     GameLoadBackgroundBitmap4bpc_lz_again
             and.b   %%d0,    -(%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundBitmap4bpc_lz_again

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_odd_even:

             suba.l  %%d1,    %%a2
             move.b  (%%a2)+, %%d3
             and.b   #15,     %%d3
             or.b    %%d3,    (%%a1)+
             move.b  (%%a2)+, (%%a1)+
             move.b  (%%a2)+, (%%a1)+

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_odd_even_again:

             move.b  (%%a2)+, (%%a1)+
             dbra    %%d6, GameLoadBackgroundBitmap4bpc_bit1_lz_pos_odd_even_again
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             bne     GameLoadBackgroundBitmap4bpc_lz_again
             move.w  %%d5,    %%d2
             and.b   %%d0,    -(%%a1)
             bra     GameLoadBackgroundBitmap4bpc_lz_again

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even:

             move.l  %%d1,    %%d3
             addq.w  #1,      %%d3
             lsr.w   #1,      %%d3
             suba.l  %%d3,    %%a2
             btst.b  #0,      %%d1
             beq.b   GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even_even_start
             addq.w  #2,      %%d6
             moveq   #15,     %%d2
             move.b  (%%a2)+, %%d1
             rol.b   #4,      %%d1

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even_odd:

             move.b  (%%a2)+, %%d5
             rol.b   #4,      %%d5
             move.w  %%d5,    %%d3
             and.b   %%d2,    %%d5
             and.b   %%d0,    %%d1
             or.b    %%d1,    %%d5
             move.b  %%d5,    (%%a1)+
             dbra    %%d6, GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even_odd2
             move.l  %%a4,    %%d5
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundBitmap4bpc_lz_again
             and.b   %%d0,    %%d3
             move.b  %%d3,    (%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundBitmap4bpc_lz_again

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even_odd2:

             move.b  (%%a2)+, %%d5
             rol.b   #4,      %%d5
             move.w  %%d5,    %%d1
             and.b   %%d2,    %%d5
             and.b   %%d0,    %%d3
             or.b    %%d3,    %%d5
             move.b  %%d5,    (%%a1)+
             dbra    %%d6, GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even_odd
             move.l  %%a4,    %%d5
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundBitmap4bpc_lz_again
             and.b   %%d0,    %%d1
             move.b  %%d1,    (%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundBitmap4bpc_lz_again

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even_even_start:

             move.b  (%%a2)+, (%%a1)+
             move.b  (%%a2)+, (%%a1)+

  GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even_even:

             move.b  (%%a2)+, (%%a1)+
             dbra    %%d6, GameLoadBackgroundBitmap4bpc_bit1_lz_pos_even_even
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundBitmap4bpc_lz_again
             move.b  (%%a2),  %%d3
             and.b   %%d0,    %%d3
             move.b  %%d3,    (%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundBitmap4bpc_lz_again

  GameLoadBackgroundBitmap4bpc_exit:

  ": :
    "r"(size),
    "r"(src),
    "r"(dest));

  // pop all registers off stack
  asm("      movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a4": :);
#endif

  MemHandleUnlock(memHandle);
#endif
}

/**
 * Load a mask 4bpp bitmap into the level mask memory area.
 * 
 * @param memHandle a reference to the mask bitmap resource.
 * @param size      the size of the mask bitmap resource.
 * @param dest      the destination pointer
 * @param flag      flag to add borders
 */
static void
GameLoadBackgroundMask4bpp(MemHandle memHandle, UInt32 size, 
                           UInt8 *dest, Boolean flag)
{
#if !USE_CHIP_COMPRESS
  UInt8  *ptrBuffer, *ptrScreen;
  UInt16 i;

  // get pointers to everything
  ptrScreen = dest;
  ptrBuffer = (UInt8 *)MemHandleLock(memHandle);

  if (flag)
  {
    // set the SPR_HEIGHT area above the bitmap data
    MemSet(ptrScreen, (SPR_HEIGHT * 160), 255);

    // load the resource
    ptrScreen = dest + (SPR_HEIGHT * 160);
  }
  i = 0;
  while (i < size) 
  {
    if (ptrBuffer[i] == 0x00)
    {
      MemSet(ptrScreen, ptrBuffer[++i], 0x00); 
      ptrScreen += ptrBuffer[i++];
    }
    else
      *(ptrScreen++) = ptrBuffer[i++];
  }

  if (flag)
  {
    // clear the (SPR_HEIGHT*1) area below the bitmap data
    ptrScreen = dest + ((SCREEN_HEIGHT + SPR_HEIGHT) * 160);
    MemSet(ptrScreen, ((SPR_HEIGHT+1) * 160), 0);
  }

  MemHandleUnlock(memHandle);
#else
  UInt8 *src;
#if PORTABLE
  UInt16 c;
#endif

  // get pointers to everything
  src  = (UInt8 *)MemHandleLock(memHandle);

  if (flag)
  {
    // set the SPR_HEIGHT area above the bitmap data
    MemSet(dest, (SPR_HEIGHT * 160), 255);

    // load the resource
    dest += (SPR_HEIGHT * 160);
  }
#if PORTABLE
  c = *src++;
  if (c == COMP_TYPE_HUFF) 
  {
    UInt8 *dest2, cur, tog, col, tg2;
    UInt16 cnt, num;

    cur   = 0;
    tog   = 1;
    col   = 1;
    tg2   = 2;
    c     = 0;

    while (1) 
    {
      nextbit;
      if (c & 1)                    // 1...
      {
        nextbit;
        if (c & 1)                  // 11...
        {
          col  = cur;               // Toggle
          cur  = tog;
          tog  = col;

          if (size & 1) 
            *dest++ |= cur;
          else 
            *dest    = cur << 4;

          size--;
          continue;
        }
        else                        // 10...
        {
          num = (UInt16)(-1);       // Duplication
          do 
          {
            num++;
            nextbit;
          } 
          while (!(c & 1));
          cnt = 1;
          if (num) 
          {
            do 
            {
              nextbit;
              cnt += cnt + (c & 1);
            } 
            while (--num);
          }

          if (col == cur) 
          {
            cur  = tog;
            tog  = col;

            cnt++;
          }

          num  = cnt;
          if (size & 1) 
          {
            *dest++ |= cur;
             cnt--;
          }
          size -= num;

          col  = (cur << 4) | cur;
          num = cnt >> 1;
          if (num) 
          {
            do 
            {
              *dest++ = col;
            } 
            while (--num);
          }

          if (cnt & 1) 
            *dest = col & 0xf0;

          col  = cur;
          continue;
        }
      }
      else                          // 0...
      {
        nextbit;
        if (c & 1)                  // 01...
        {
          nextbit;
          if (c & 1)                // 011...
          {
            col  = tog;             // Toggle2
            tog  = cur;
            cur  = tg2;
            tg2  = col;

            if (size & 1)
              *dest++ |= cur;
            else 
              *dest    = cur << 4;

            size--;
            continue;
          }
          else                      // 010...
          {
            tg2  = tog;             // Literal
            col  = tog = cur;

            nextbit;
            cur  = (c & 1);
            nextbit;
            cur += cur + (c & 1);
            nextbit;
            cur += cur + (c & 1);
            nextbit;
            cur += cur + (c & 1);

            if (size & 1) 
              *dest++ |= cur;
            else 
            {
              if (size == 0) break;
              *dest    = cur << 4;
            }
            size--;
            continue;
          }
        }
        else                        // 00...
        {
          col = (UInt8)(-1);        // Copying
          do 
          {
            col++;
            nextbit;
          } 
          while (!(c & 1));
          cnt = 1;
          if (col) 
          {
            do 
            {
              nextbit;
              cnt += cnt + (c & 1);
            } 
            while (--col);
          }

          num  = cnt;
          dest2 = dest - (OFFSCREEN_WIDTH / 4);
          if (size & 1) 
          {
            *dest++ |= (*dest2++ & 0x0f);
            cnt--;
          }
          size -= num;

          num = cnt >> 1;
          if (num) 
          {
            do 
            {
              *dest++ = *dest2++;
            } 
            while (--num);
          }

          if (cnt & 1) 
          {
            *dest = *dest2 & 0xf0;
          }

          col = (UInt8)(-1);
          continue;
        }
      }
    }
  } 
  else 
  if (c == COMP_TYPE_LZ77) 
  {
    UInt16 pos, c, ch3, num;

    c = 1;
    pos = 0;
    while (1) 
    {
      nextbit;
      if (c & 1) 
      {
        ch3 = 0;
        num = OUTPUT_BIT_COUNT_MASK;
        do 
        {
          nextbit;
          ch3 += ch3 + (c & 1);
        } 
        while (--num);

        DumpMaskBits(dest, ch3);
      } 
      else 
      {
        UInt16 match_length, match_position;
        UInt8 *match;

        match_position = 0;
        num = INDEX_BIT_COUNT;
        do 
        {
          nextbit;
          if (c & 0x100) break;

          match_position += match_position + (c & 1);
        } 
        while (--num);

        c = reverseByte[c & 0xff] | 0x100;
        match_position += match_position + (c & 1);
        if (--num) 
        {
          do 
          {
            nextbit;
            match_position += match_position + (c & 1);
          } 
          while (--num);
        }

        if (match_position == END_OF_STREAM) break;

        num = (UInt16)(-1);
        do 
        {
          num++;
          nextbit;
        } 
        while (!(c & 1));

        match_length = 1;
        if (num) 
        {
          do 
          {
            nextbit;
            match_length += match_length + (c & 1);
          } 
          while (--num);
        }

        match_length += BREAK_EVEN_MASK;

        num = match_length & 1;
        match_length >>= 1;
        if (pos & 1) 
        {
          match = dest - (match_position >> 1);
          if (match_position & 1) 
          {
            ch3 = *match++;
            *dest++ |= (ch3 >> 4);
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);
            pos = num ^ 1;
            if (pos) 
            {
              *--dest &= 0xf0;
            }
          } 
          else 
          {
            *dest++ |= (*match++ & 0x0f);
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);
            pos = num ^ 1;
            if (pos) *--dest &= 0xf0;
          }
        } 
        else 
        {
          match = dest - ((match_position + 1) >> 1);
          if (match_position & 1) 
          {
            ch3 = *match++;
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);
            pos = num;
            if (pos) *dest = (ch3 << 4);
          } 
          else 
          {
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);

            if (num) 
            {
              *dest = (*match & 0xf0);
              pos = 1;
            }
          }
        }
      }
    }
  }
#else
  // push all registers on stack
  asm("      movem.l %%d0-%%d7/%%a0-%%a4, -(%%sp)": :);

  asm("      move.l  %0,      %%d7          | size
             move.l  %1,      %%a0          | src
             move.l  %2,      %%a1          | dest
             moveq   #1,      %%d4          | c
             move.b  (%%a0)+, %%d3
             bne     GameLoadBackgroundMask_lz_start
             moveq   #0,      %%d0          | cur
             moveq   #1,      %%d3          | col
             move.l  %%d3,    %%a3          | tog
             moveq   #2,      %%d6          | tg2
             lsr.w   #1,      %%d4
             bcs.b   GameLoadBackgroundMask_bit1_reload

  GameLoadBackgroundMask_odd:

             or.w    %%d0,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask_odd_again:

             lsr.w   #1,      %%d4
             bcc     GameLoadBackgroundMask_bit0
             bne.b   GameLoadBackgroundMask_bit1_skip

  GameLoadBackgroundMask_bit1_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit0

  GameLoadBackgroundMask_bit1_skip:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit10
             bne.b   GameLoadBackgroundMask_bit11_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit10

  GameLoadBackgroundMask_bit11_skip:

             move.l  %%d0,    %%d3          | col = cur
             exg     %%d0,    %%a3          | cur <-> tog
             subq.l  #1,      %%d7          | Toggle
             bra     GameLoadBackgroundMask_even

  GameLoadBackgroundMask_bit10:

             moveq   #0,      %%d2          | num

  GameLoadBackgroundMask_bit10_zero:

             addq.w  #1,      %%d2          | Duplication
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit10_zero
             bne.b   GameLoadBackgroundMask_bit10_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit10_zero

  GameLoadBackgroundMask_bit10_zero_skip:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d2
             bcs.b   GameLoadBackgroundMask_bit10_count_skip
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundMask_bit10_count_again
             bne.b   GameLoadBackgroundMask_bit10_count_skip2
             bra.b   GameLoadBackgroundMask_bit10_count_reload

  GameLoadBackgroundMask_bit10_count_again:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundMask_bit10_count_again
             bne.b   GameLoadBackgroundMask_bit10_count_skip2

  GameLoadBackgroundMask_bit10_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d2,    GameLoadBackgroundMask_bit10_count_again

  GameLoadBackgroundMask_bit10_count_skip2:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundMask_bit10_count_skip:

             cmp.w   %%d3,    %%d0
             bne.b   GameLoadBackgroundMask_bit10_tog_skip
             exg     %%d0,    %%a3          | cur <-> tog
             addq.w  #1,      %%d5

  GameLoadBackgroundMask_bit10_tog_skip:

             sub.l   %%d5,    %%d7
             move.w  %%d0,    %%d1
             lsl.b   #4,      %%d1
             move.w  %%d0,    %%d3
             subq.w  #2,      %%d5
             bcs     GameLoadBackgroundMask_check
             lsr.w   #1,      %%d5
             or.w    %%d0,    %%d1

  GameLoadBackgroundMask_bit10_move_again:

             move.b  %%d1,    (%%a1)+
             dbra    %%d5,    GameLoadBackgroundMask_bit10_move_again
             sub.w   %%d0,    %%d1
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundMask_odd_again
             bra     GameLoadBackgroundMask_even_again

  GameLoadBackgroundMask_bit0:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit00
             bne.b   GameLoadBackgroundMask_bit0_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit00

  GameLoadBackgroundMask_bit0_skip:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit010
             bne.b   GameLoadBackgroundMask_bit01_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit010

  GameLoadBackgroundMask_bit01_skip:

             move.l  %%a3,    %%d3          | col = tog
             exg     %%d0,    %%d6          | cur <-> tg2
             exg     %%a3,    %%d6          | tog <-> tg2
             subq.l  #1,      %%d7          | Toggle2
             bra     GameLoadBackgroundMask_even

  GameLoadBackgroundMask_bit010:

             move.l  %%a3,    %%d6          | tg2 = tog
             move.l  %%d0,    %%d3          | col = cur
             move.l  %%d0,    %%a3          | tog = cur
             moveq   #0,      %%d0          | cur = 0x00
             lsr.w   #1,      %%d4          | Literal
             bne.b   GameLoadBackgroundMask_bit010_skip1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_bit010_skip1:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_bit010_skip2
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_bit010_skip2:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_bit010_skip3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_bit010_skip3:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_bit010_skip4
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_bit010_skip4:

             addx.b  %%d0,    %%d0
             subq.l  #1,      %%d7
             bcc.b   GameLoadBackgroundMask_even
             bra     GameLoadBackgroundMask_exit

  GameLoadBackgroundMask_bit00:

             moveq   #0,      %%d3

  GameLoadBackgroundMask_bit00_zero:

             addq.w  #1,      %%d3          | Copying
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit00_zero
             bne.b   GameLoadBackgroundMask_bit00_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit00_zero

  GameLoadBackgroundMask_bit00_zero_skip:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d3
             bcs.b   GameLoadBackgroundMask_bit00_count_skip
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundMask_bit00_count_again
             bne.b   GameLoadBackgroundMask_bit00_count_skip2
             bra.b   GameLoadBackgroundMask_bit00_count_reload

  GameLoadBackgroundMask_bit00_count_again:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundMask_bit00_count_again
             bne.b   GameLoadBackgroundMask_bit00_count_skip2

  GameLoadBackgroundMask_bit00_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d3,    GameLoadBackgroundMask_bit00_count_again

  GameLoadBackgroundMask_bit00_count_skip2:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundMask_bit00_count_skip:

             lea     -160(%%a1), %%a4
             sub.l   %%d5,    %%d7
             subq.w  #2,      %%d5
             bcs.b   GameLoadBackgroundMask_bit00_move_skip
             lsr.w   #1,      %%d5

  GameLoadBackgroundMask_bit00_move_again:

             move.b  (%%a4)+, (%%a1)+
             dbra    %%d5,    GameLoadBackgroundMask_bit00_move_again

  GameLoadBackgroundMask_bit00_move_skip:

             move.b  (%%a4),  %%d1
             and.w   #240,    %%d1

  GameLoadBackgroundMask_check:

             btst.b  #0,      %%d7
             beq     GameLoadBackgroundMask_odd_again
             bra.b   GameLoadBackgroundMask_even_again

  GameLoadBackgroundMask_even:

             move.l  %%d0,    %%d1          | cur2
             lsl.b   #4,      %%d1

  GameLoadBackgroundMask_even_again:

             lsr.w   #1,      %%d4
             bcc     GameLoadBackgroundMask_bit0_even
             bne.b   GameLoadBackgroundMask_bit1_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc     GameLoadBackgroundMask_bit0_even

  GameLoadBackgroundMask_bit1_skip_even:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit10_even
             bne.b   GameLoadBackgroundMask_bit11_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit10_even

  GameLoadBackgroundMask_bit11_skip_even:

             move.l  %%d0,    %%d3          | col = cur
             exg     %%d0,    %%a3          | cur <-> tog
             subq.l  #1,      %%d7          | Toggle
             bra     GameLoadBackgroundMask_odd

  GameLoadBackgroundMask_bit10_even:

             moveq   #0,      %%d2          | num

  GameLoadBackgroundMask_bit10_zero_even:

             addq.w  #1,      %%d2          | Duplication
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit10_zero_even
             bne.b   GameLoadBackgroundMask_bit10_zero_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit10_zero_even

  GameLoadBackgroundMask_bit10_zero_skip_even:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d2
             bcs.b   GameLoadBackgroundMask_bit10_count_skip_even
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundMask_bit10_count_again_even
             bne.b   GameLoadBackgroundMask_bit10_count_skip2_even
             bra.b   GameLoadBackgroundMask_bit10_count_reload_even

  GameLoadBackgroundMask_bit10_count_again_even:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d2,    GameLoadBackgroundMask_bit10_count_again_even
             bne.b   GameLoadBackgroundMask_bit10_count_skip2_even

  GameLoadBackgroundMask_bit10_count_reload_even:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d2,    GameLoadBackgroundMask_bit10_count_again_even

  GameLoadBackgroundMask_bit10_count_skip2_even:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundMask_bit10_count_skip_even:

             cmp.w   %%d3,    %%d0
             bne.b   GameLoadBackgroundMask_bit10_tog_skip_even
             exg     %%d0,    %%a3          | cur <-> tog
             addq.w  #1,      %%d5

  GameLoadBackgroundMask_bit10_tog_skip_even:

             or.w    %%d0,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask_bit10_even_even:

             sub.l   %%d5,    %%d7
             move.w  %%d0,    %%d1
             lsl.b   #4,      %%d1
             move.w  %%d0,    %%d3
             subq.w  #3,      %%d5
             bcs     GameLoadBackgroundMask_check_even
             lsr.w   #1,      %%d5
             or.w    %%d0,    %%d1

  GameLoadBackgroundMask_bit10_move_again_even:

             move.b  %%d1,    (%%a1)+
             dbra    %%d5,    GameLoadBackgroundMask_bit10_move_again_even
             sub.w   %%d0,    %%d1
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundMask_odd_again
             bra     GameLoadBackgroundMask_even_again

  GameLoadBackgroundMask_bit0_even:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit00_even
             bne.b   GameLoadBackgroundMask_bit0_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit00_even

  GameLoadBackgroundMask_bit0_skip_even:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit010_even
             bne.b   GameLoadBackgroundMask_bit01_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit010_even

  GameLoadBackgroundMask_bit01_skip_even:

             move.l  %%a3,    %%d3          | col = tog
             exg     %%d0,    %%d6          | cur <-> tg2
             exg     %%a3,    %%d6          | tog <-> tg2
             subq.l  #1,      %%d7          | Toggle2
             bra     GameLoadBackgroundMask_odd

  GameLoadBackgroundMask_bit010_even:

             move.l  %%a3,    %%d6          | tg2 = tog
             move.l  %%d0,    %%d3          | col = cur
             move.l  %%d0,    %%a3          | tog = cur
             moveq   #0,      %%d0          | cur = 0x01
             lsr.w   #1,      %%d4          | Literal
             bne.b   GameLoadBackgroundMask_bit010_skip1_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_bit010_skip1_even:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_bit010_skip2_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_bit010_skip2_even:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_bit010_skip3_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_bit010_skip3_even:

             addx.b  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_bit010_skip4_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_bit010_skip4_even:

             addx.b  %%d0,    %%d0
             subq.l  #1,      %%d7
             bra     GameLoadBackgroundMask_odd

  GameLoadBackgroundMask_bit00_even:

             moveq   #0,      %%d3          | num

  GameLoadBackgroundMask_bit00_zero_even:

             addq.w  #1,      %%d3          | Copying
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit00_zero_even
             bne.b   GameLoadBackgroundMask_bit00_zero_skip_even
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_bit00_zero_even

  GameLoadBackgroundMask_bit00_zero_skip_even:

             moveq   #1,      %%d5          | cnt
             subq.w  #2,      %%d3
             bcs.b   GameLoadBackgroundMask_bit00_count_skip_even
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundMask_bit00_count_again_even
             bne.b   GameLoadBackgroundMask_bit00_count_skip2_even
             bra.b   GameLoadBackgroundMask_bit00_count_reload_even

  GameLoadBackgroundMask_bit00_count_again_even:

             addx.w  %%d5,    %%d5
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundMask_bit00_count_again_even
             bne.b   GameLoadBackgroundMask_bit00_count_skip2_even

  GameLoadBackgroundMask_bit00_count_reload_even:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d3,    GameLoadBackgroundMask_bit00_count_again_even

  GameLoadBackgroundMask_bit00_count_skip2_even:

             addx.w  %%d5,    %%d5

  GameLoadBackgroundMask_bit00_count_skip_even:

             lea     -160(%%a1), %%a4
             move.b  (%%a4)+, %%d2
             and.w   #15,     %%d2
             or.w    %%d2,    %%d1
             move.b  %%d1,    (%%a1)+
             sub.l   %%d5,    %%d7
             subq.w  #3,      %%d5
             bcs.b   GameLoadBackgroundMask_bit00_move_skip_even
             lsr.w   #1,      %%d5

  GameLoadBackgroundMask_bit00_move_again_even:

             move.b  (%%a4)+, (%%a1)+
             dbra    %%d5,    GameLoadBackgroundMask_bit00_move_again_even

  GameLoadBackgroundMask_bit00_move_skip_even:

             move.b  (%%a4),  %%d1
             and.w   #240,    %%d1

  GameLoadBackgroundMask_check_even:

             btst.b  #0,      %%d7
             beq     GameLoadBackgroundMask_odd_again
             bra     GameLoadBackgroundMask_even_again

  GameLoadBackgroundMask_lz_start:

             moveq   #0,      %%d2          | pos
             moveq   #15,     %%d5
             move.l  %%d5,    %%a4
             move.w  #240,    %%d0

  GameLoadBackgroundMask_lz_bit0_dump_odd:

             lsl.b   #4,      %%d1
             move.b  %%d1,    (%%a1)

  GameLoadBackgroundMask_lz_again:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_lz_bit1
             bne.b   GameLoadBackgroundMask_lz_bit0
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_lz_bit1

  GameLoadBackgroundMask_lz_bit0:

             moveq   #0,      %%d1          | ch3
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_lz_bit0_skip1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_lz_bit0_skip1:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_lz_bit0_skip2
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_lz_bit0_skip2:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_lz_bit0_skip3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_lz_bit0_skip3:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask_lz_bit0_skip4
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask_lz_bit0_skip4:

             addx.w  %%d1,    %%d1
             eor.b   %%d5,    %%d2
             bne.b   GameLoadBackgroundMask_lz_bit0_dump_odd
             or.b    %%d1,    (%%a1)+
             bra.b   GameLoadBackgroundMask_lz_again

  GameLoadBackgroundMask_lz_bit1:

             moveq   #0,      %%d1          | match_position
             subq.w  #1,      %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask_lz_bit1_skip9_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask_lz_bit1_skip10_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask_lz_bit1_skip11_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask_lz_bit1_skip12_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask_lz_bit1_skip13_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask_lz_bit1_skip14_skip
             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask_lz_bit1_skip15_skip
             addx.w  %%d1,    %%d1
             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask_lz_bit1_skip16

  GameLoadBackgroundMask_lz_bit1_skip9_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask_lz_bit1_skip9

  GameLoadBackgroundMask_lz_bit1_skip10_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask_lz_bit1_skip10

  GameLoadBackgroundMask_lz_bit1_skip11_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask_lz_bit1_skip11

  GameLoadBackgroundMask_lz_bit1_skip12_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask_lz_bit1_skip12

  GameLoadBackgroundMask_lz_bit1_skip13_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask_lz_bit1_skip13

  GameLoadBackgroundMask_lz_bit1_skip14_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask_lz_bit1_skip14

  GameLoadBackgroundMask_lz_bit1_skip15_skip:

             lsl.w   #8,      %%d1
             move.b  (%%a0)+, %%d1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask_lz_bit1_skip15

  GameLoadBackgroundMask_lz_bit1_skip9:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask_lz_bit1_skip10:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask_lz_bit1_skip11:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask_lz_bit1_skip12:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask_lz_bit1_skip13:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask_lz_bit1_skip14:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask_lz_bit1_skip15:

             addx.w  %%d1,    %%d1
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask_lz_bit1_skip16:

             addx.w  %%d1,    %%d1
             tst.w   %%d1
             beq     GameLoadBackgroundMask_exit
             moveq   #0,      %%d3          | num

  GameLoadBackgroundMask_lz_bit1_zero:

             addq.w  #1,      %%d3
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask_lz_bit1_zero
             bne.b   GameLoadBackgroundMask_lz_bit1_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask_lz_bit1_zero

  GameLoadBackgroundMask_lz_bit1_zero_skip:

             moveq   #1,      %%d6          | match_length
             subq.w  #2,      %%d3
             bcs.b   GameLoadBackgroundMask_lz_bit1_count_skip2
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundMask_lz_bit1_count_again
             bne.b   GameLoadBackgroundMask_lz_bit1_count_skip
             bra.b   GameLoadBackgroundMask_lz_bit1_count_reload

  GameLoadBackgroundMask_lz_bit1_count_again:

             addx.w  %%d6,    %%d6
             lsr.w   #1,      %%d4
             dbeq    %%d3,    GameLoadBackgroundMask_lz_bit1_count_again
             bne.b   GameLoadBackgroundMask_lz_bit1_count_skip

  GameLoadBackgroundMask_lz_bit1_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d3,    GameLoadBackgroundMask_lz_bit1_count_again

  GameLoadBackgroundMask_lz_bit1_count_skip:

             addx.w  %%d6,    %%d6

  GameLoadBackgroundMask_lz_bit1_count_skip2:

             addq.w  #3,      %%d6
             move.l  %%d6,    %%d7
             lsr.w   #1,      %%d6
             move.l  %%a1,    %%a2
             tst.b   %%d2
             beq     GameLoadBackgroundMask_bit1_lz_pos_even
             lsr.w   #1,      %%d1
             bcc.b   GameLoadBackgroundMask_bit1_lz_pos_odd_even
             suba.l  %%d1,    %%a2
             move.b  (%%a2)+, %%d1
             rol.b   #4,      %%d1
             move.w  %%d1,    %%d3
             and.b   %%d5,    %%d1
             and.b   %%d0,    %%d3
             or.b    %%d1,    (%%a1)+

  GameLoadBackgroundMask_bit1_lz_pos_odd_odd:

             move.b  (%%a2)+, %%d1
             rol.b   #4,      %%d1
             move.w  %%d1,    %%d5
             and.b   %%d2,    %%d1
             and.b   %%d0,    %%d5
             or.b    %%d3,    %%d1
             move.b  %%d1,    (%%a1)+
             dbra    %%d6, GameLoadBackgroundMask_bit1_lz_pos_odd_odd_2
             move.l  %%a4,    %%d5
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             bne     GameLoadBackgroundMask_lz_again
             and.b   %%d0,    -(%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundMask_lz_again

  GameLoadBackgroundMask_bit1_lz_pos_odd_odd_2:

             move.b  (%%a2)+, %%d1
             rol.b   #4,      %%d1
             move.w  %%d1,    %%d3
             and.b   %%d2,    %%d1
             and.b   %%d0,    %%d3
             or.b    %%d5,    %%d1
             move.b  %%d1,    (%%a1)+
             dbra    %%d6, GameLoadBackgroundMask_bit1_lz_pos_odd_odd
             move.l  %%a4,    %%d5
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             bne     GameLoadBackgroundMask_lz_again
             and.b   %%d0,    -(%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundMask_lz_again

  GameLoadBackgroundMask_bit1_lz_pos_odd_even:

             suba.l  %%d1,    %%a2
             move.b  (%%a2)+, %%d3
             and.b   %%d5,    %%d3
             or.b    %%d3,    (%%a1)+

  GameLoadBackgroundMask_bit1_lz_pos_odd_even_again:

             move.b  (%%a2)+, (%%a1)+
             dbra    %%d6, GameLoadBackgroundMask_bit1_lz_pos_odd_even_again
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             bne     GameLoadBackgroundMask_lz_again
             move.w  %%d5,    %%d2
             and.b   %%d0,    -(%%a1)
             bra     GameLoadBackgroundMask_lz_again

  GameLoadBackgroundMask_bit1_lz_pos_even:

             move.l  %%d1,    %%d3
             addq.w  #1,      %%d3
             lsr.w   #1,      %%d3
             suba.l  %%d3,    %%a2
             btst.b  #0,      %%d1
             beq.b   GameLoadBackgroundMask_bit1_lz_pos_even_even
             move.w  %%d5,    %%d2
             move.b  (%%a2)+, %%d1
             rol.b   #4,      %%d1

  GameLoadBackgroundMask_bit1_lz_pos_even_odd:

             move.b  (%%a2)+, %%d5
             rol.b   #4,      %%d5
             move.w  %%d5,    %%d3
             and.b   %%d2,    %%d5
             and.b   %%d0,    %%d1
             or.b    %%d1,    %%d5
             move.b  %%d5,    (%%a1)+
             dbra    %%d6, GameLoadBackgroundMask_bit1_lz_pos_even_odd2
             move.l  %%a4,    %%d5
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundMask_lz_again
             and.b   %%d0,    %%d3
             move.b  %%d3,    (%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundMask_lz_again

  GameLoadBackgroundMask_bit1_lz_pos_even_odd2:

             move.b  (%%a2)+, %%d5
             rol.b   #4,      %%d5
             move.w  %%d5,    %%d1
             and.b   %%d2,    %%d5
             and.b   %%d0,    %%d3
             or.b    %%d3,    %%d5
             move.b  %%d5,    (%%a1)+
             dbra    %%d6, GameLoadBackgroundMask_bit1_lz_pos_even_odd
             move.l  %%a4,    %%d5
             moveq   #0,      %%d2
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundMask_lz_again
             and.b   %%d0,    %%d1
             move.b  %%d1,    (%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundMask_lz_again

  GameLoadBackgroundMask_bit1_lz_pos_even_even:

             move.b  (%%a2)+, (%%a1)+
             dbra    %%d6, GameLoadBackgroundMask_bit1_lz_pos_even_even
             btst.b  #0,      %%d7
             beq     GameLoadBackgroundMask_lz_again
             move.b  (%%a2),  %%d3
             and.b   %%d0,    %%d3
             move.b  %%d3,    (%%a1)
             move.w  %%d5,    %%d2
             bra     GameLoadBackgroundMask_lz_again

  GameLoadBackgroundMask_exit:

  ": :
    "r"(size),
    "r"(src),
    "r"(dest));

  // pop all registers off stack
  asm("      movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a4": :);
#endif

  if (flag)
  {
    // clear the (SPR_HEIGHT*1) area below the bitmap data
#if !PORTABLE
    dest += (SCREEN_HEIGHT * 160);
#endif
    MemSet(dest, ((SPR_HEIGHT+1) * 160), 0);
  }

  MemHandleUnlock(memHandle);
#endif
}

#if USE_CHIP_COMPRESS
/**
 * Load a mask 2bpp bitmap into the level mask memory area.
 * 
 * @param memHandle a reference to the mask bitmap resource.
 * @param size      the size of the mask bitmap resource.
 * @param dest      the destination pointer
 */
static void
GameLoadBackgroundMask2bpp(MemHandle memHandle, UInt32 size, UInt8 *dest)
{
  UInt8 *src;
#if PORTABLE
  UInt16 c;
#endif

  // get pointers to everything
  src  = (UInt8 *)MemHandleLock(memHandle);

#if PORTABLE
  c = *src++;
  if (c == COMP_TYPE_LZ77) 
  {
    UInt16 pos, ch2, ch3, num;

    c = 1;
    pos = 0;
    while (1) 
    {
      nextbit;
      if (c & 1) 
      {
        ch3 = 0;
        num = OUTPUT_BIT_COUNT_MASK;
        do 
        {
          nextbit;
          ch3 += ch3 + (c & 1);
        } 
        while (--num);
        DumpMaskBits2(dest, ch3);
      } 
      else 
      {
        UInt16 match_length, match_position;
        UInt8 *match;

        match_position = 0;
        num = INDEX_BIT_COUNT;
        do 
        {
          nextbit;
          if (c & 0x100) break;
          match_position += match_position + (c & 1);
        } 
        while (--num);

        c = reverseByte[c & 0xff] | 0x100;
        match_position += match_position + (c & 1);
        if (--num) 
        {
          do 
          {
            nextbit;
            match_position += match_position + (c & 1);
          } 
          while (--num);
        }

        if (match_position == END_OF_STREAM) break;

        num = (UInt16)(-1);
        do 
        {
          num++;
          nextbit;
        } 
        while (!(c & 1));

        match_length = 1;
        if (num) 
        {
          do 
          {
            nextbit;
            match_length += match_length + (c & 1);
          } 
          while (--num);
        }

        match_length += BREAK_EVEN_MASK;

        num = match_length & 3;
        match_length >>= 2;
        ch3 = (match_position & 3);
        ch2 = (pos & 3);
        if (ch2 == 3) 
        {
          match = dest - (match_position >> 2);
          if (ch3 == 3) 
          {
            ch3 = *match++;
            *dest++ |= (ch3 >> 6);
            do 
            {
              *dest++ = ((pos = *match++) >> 6) | (ch3 << 2);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 2) & 0xf0; } 
            else if (num == 2) { *dest = (ch3 << 2) & 0xc0; } 
            else if (num == 0) { *--dest &= 0xfc; }
          } 
          else 
          if (ch3 == 2) 
          {
            ch3 = *match++;
            *dest++ |= ((ch3 >> 4) & 0x03);
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 4); } 
            else if (num == 2) { *dest = (ch3 << 4) & 0xc0; } 
            else if (num == 0) { *--dest &= 0xfc; }
          } 
          else 
          if (ch3 == 1) 
          {
            ch3 = *match++;
            *dest++ |= ((ch3 >> 2) & 0x03);
            do 
            {
              *dest++ = ((pos = *match++) >> 2) | (ch3 << 6);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = ((ch3 << 6) | (*match >> 2)) & 0xf0; } 
            else if (num == 2) { *dest = ((ch3 << 6) | (*match >> 2)) & 0xc0; }
            else if (num == 0) { *--dest &= 0xfc; }
          } 
          else 
          {
            *dest++ |= (*match++ & 0x03);
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);

                 if (num == 3) { *dest = *match & 0xf0; } 
            else if (num == 2) { *dest = *match & 0xc0; } 
            else if (num == 0) { *--dest &= 0xfc; }
          }
          pos = (ch2 + num) & 3;
        } 
        else 
        if (ch2 == 2) 
        {
          match = dest - ((match_position + 1) >> 2);
          if (ch3 == 3) 
          {
            ch3 = *match++;
            pos = *match++;
            *dest++ |= ((ch3 << 2) & 0x0c) | (pos >> 6);
            do 
            {
              ch3 = pos;
              *dest++ = ((pos = *match++) >> 6) | (ch3 << 2);
            } 
            while (--match_length);

                 if (num == 3) { *dest = (pos << 2) & 0xc0; } 
            else if (num == 1) { *--dest &= 0xfc; } 
            else if (num == 0) { *--dest &= 0xf0; }
          } 
          else 
          if (ch3 == 2) 
          {
            ch3 = *match++;
            *dest++ |= (ch3 >> 4);
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 4) & 0xc0; } 
            else if (num == 1) { *--dest &= 0xfc; } 
            else if (num == 0) { *--dest &= 0xf0; }
          } 
          else 
          if (ch3 == 1) 
          {
            ch3 = *match++;
            *dest++ |= ((ch3 >> 2) & 0x0f);
            do 
            {
              *dest++ = ((pos = *match++) >> 2) | (ch3 << 6);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 6); } 
            else if (num == 1) { *--dest &= 0xfc; } 
            else if (num == 0) { *--dest &= 0xf0; }
          } 
          else 
          {
            *dest++ |= (*match++ & 0x0f);
            do 
            {
              *dest++ = *match++;
            } while (--match_length);

                 if (num == 3) { *dest = *match & 0xc0; } 
            else if (num == 1) { *--dest &= 0xfc; } 
            else if (num == 0) { *--dest &= 0xf0; }
          }
          pos = (ch2 + num) & 3;
        } 
        else 
        if (ch2 == 1) 
        {
          match = dest - ((match_position + 2) >> 2);
          if (ch3 == 3)
          {
            ch3 = *match++;
            pos = *match++;
            *dest++ |= ((ch3 << 2) & 0x3c) | (pos >> 6);
            do 
            {
              ch3 = pos;
              *dest++ = ((pos = *match++) >> 6) | (ch3 << 2);
            } 
            while (--match_length);
          } 
          else 
          if (ch3 == 2) 
          {
            ch3 = *match++;
            pos = *match++;
            *dest++ |= ((ch3 << 4) & 0x30) | (pos >> 4);
            do 
            {
              ch3 = pos;
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
            } 
            while (--match_length);
          } 
          else 
          if (ch3 == 1) 
          {
            ch3 = *match++;
            *dest++ |= (ch3 >> 2);
            do 
            {
              *dest++ = ((pos = *match++) >> 2) | (ch3 << 6);
              ch3 = pos;
            } 
            while (--match_length);
          } 
          else 
          {
            *dest++ |= (*match++ & 0x3f);
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);
          }
          pos = (ch2 + num) & 3;

               if (num == 2) { *--dest &= 0xfc; } 
          else if (num == 1) { *--dest &= 0xf0; } 
          else if (num == 0) { *--dest &= 0xc0; }
        } 
        else 
        {
          match = dest - ((match_position + 3) >> 2);
          if (ch3 == 3) 
          {
            ch3 = *match++;
            do 
            {
              *dest++ = ((pos = *match++) >> 6) | (ch3 << 2);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = (ch3 << 2); } 
            else if (num == 2) { *dest = (ch3 << 2) & 0xf0; } 
            else if (num == 1) { *dest = (ch3 << 2) & 0xc0; }
          } 
          else 
          if (ch3 == 2) 
          {
            ch3 = *match++;
            do 
            {
              *dest++ = ((pos = *match++) >> 4) | (ch3 << 4);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = ((ch3 << 4) | (*match >> 4)) & 0xfc; } 
            else if (num == 2) { *dest = (ch3 << 4); } 
            else if (num == 1) { *dest = (ch3 << 4) & 0xc0; }
          } 
          else 
          if (ch3 == 1) 
          {
            ch3 = *match++;
            do 
            {
              *dest++ = ((pos = *match++) >> 2) | (ch3 << 6);
              ch3 = pos;
            } 
            while (--match_length);

                 if (num == 3) { *dest = ((ch3 << 6) | (*match >> 2)) & 0xfc; } 
            else if (num == 2) { *dest = ((ch3 << 6) | (*match >> 2)) & 0xf0; }
            else if (num == 1) { *dest = (ch3 << 6); }
          } 
          else 
          {
            do 
            {
              *dest++ = *match++;
            } 
            while (--match_length);

                 if (num == 3) { *dest = *match & 0xfc; } 
            else if (num == 2) { *dest = *match & 0xf0; } 
            else if (num == 1) { *dest = *match & 0xc0; }
          }
          pos = num;
        }
      }
    }
  }
#else
  // push all registers on stack
  asm("      movem.l %%d0-%%d7/%%a0-%%a5, -(%%sp)": :);

  asm("      move.l  %0,      %%d7          | size
             move.l  %3,      %%a2          | ptrConvertGameBase
             move.l  %1,      %%a0          | src
             move.l  %2,      %%a1          | dest
             moveq   #1,      %%d4          | c
             move.b  (%%a0)+, %%d3
             beq     GameLoadBackgroundMask2_exit
             moveq   #0,      %%d2          | pos
             move.l  %%d2,    %%d3

  GameLoadBackgroundMask2_lz_again3:

             move.b  (%%a3),  %%d1

  GameLoadBackgroundMask2_lz_again2:

             move.l  %%d2,    %%d0
             add.b   %%d0,    %%d0
             rol.b   %%d0,    %%d1

  GameLoadBackgroundMask2_lz_again:

             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask2_lz_bit1
             bne.b   GameLoadBackgroundMask2_lz_bit0
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask2_lz_bit1

  GameLoadBackgroundMask2_lz_bit0:

             moveq   #0,      %%d0          | ch3
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask2_lz_bit0_skip1
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask2_lz_bit0_skip1:

             addx.w  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask2_lz_bit0_skip2
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask2_lz_bit0_skip2:

             addx.w  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask2_lz_bit0_skip3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask2_lz_bit0_skip3:

             addx.w  %%d0,    %%d0
             lsr.w   #1,      %%d4
             bne.b   GameLoadBackgroundMask2_lz_bit0_skip4
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4

  GameLoadBackgroundMask2_lz_bit0_skip4:

             addx.w  %%d0,    %%d0
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             add.b   (%%a2, %%d0), %%d1
             addq.w  #1,      %%d2
             and.w   #3,      %%d2
             bne.b   GameLoadBackgroundMask2_lz_again
             move.b  %%d1,    (%%a1)+
             bra.b   GameLoadBackgroundMask2_lz_again

  GameLoadBackgroundMask2_lz_bit1:

             moveq   #0,      %%d3          | match_position
             subq.w  #1,      %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask2_lz_bit1_skip9_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask2_lz_bit1_skip10_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask2_lz_bit1_skip11_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask2_lz_bit1_skip12_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask2_lz_bit1_skip13_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask2_lz_bit1_skip14_skip
             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4
             beq.b   GameLoadBackgroundMask2_lz_bit1_skip15_skip
             addx.w  %%d3,    %%d3
             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask2_lz_bit1_skip16

  GameLoadBackgroundMask2_lz_bit1_skip9_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask2_lz_bit1_skip9

  GameLoadBackgroundMask2_lz_bit1_skip10_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask2_lz_bit1_skip10

  GameLoadBackgroundMask2_lz_bit1_skip11_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask2_lz_bit1_skip11

  GameLoadBackgroundMask2_lz_bit1_skip12_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask2_lz_bit1_skip12

  GameLoadBackgroundMask2_lz_bit1_skip13_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask2_lz_bit1_skip13

  GameLoadBackgroundMask2_lz_bit1_skip14_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask2_lz_bit1_skip14

  GameLoadBackgroundMask2_lz_bit1_skip15_skip:

             lsl.w   #8,      %%d3
             move.b  (%%a0)+, %%d3
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bra.b   GameLoadBackgroundMask2_lz_bit1_skip15

  GameLoadBackgroundMask2_lz_bit1_skip9:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask2_lz_bit1_skip10:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask2_lz_bit1_skip11:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask2_lz_bit1_skip12:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask2_lz_bit1_skip13:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask2_lz_bit1_skip14:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask2_lz_bit1_skip15:

             addx.w  %%d3,    %%d3
             lsr.w   #1,      %%d4

  GameLoadBackgroundMask2_lz_bit1_skip16:

             addx.w  %%d3,    %%d3
             tst.w   %%d3
             beq     GameLoadBackgroundMask2_exit
             moveq   #0,      %%d5          | num

  GameLoadBackgroundMask2_lz_bit1_zero:

             addq.w  #1,      %%d5
             lsr.w   #1,      %%d4
             bcc.b   GameLoadBackgroundMask2_lz_bit1_zero
             bne.b   GameLoadBackgroundMask2_lz_bit1_zero_skip
             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             bcc.b   GameLoadBackgroundMask2_lz_bit1_zero

  GameLoadBackgroundMask2_lz_bit1_zero_skip:

             moveq   #1,      %%d6          | match_length
             subq.w  #2,      %%d5
             bcs.b   GameLoadBackgroundMask2_lz_bit1_count_skip2
             lsr.w   #1,      %%d4
             dbeq    %%d5,    GameLoadBackgroundMask2_lz_bit1_count_again
             bne.b   GameLoadBackgroundMask2_lz_bit1_count_skip
             bra.b   GameLoadBackgroundMask2_lz_bit1_count_reload

  GameLoadBackgroundMask2_lz_bit1_count_again:

             addx.w  %%d6,    %%d6
             lsr.w   #1,      %%d4
             dbeq    %%d5,    GameLoadBackgroundMask2_lz_bit1_count_again
             bne.b   GameLoadBackgroundMask2_lz_bit1_count_skip

  GameLoadBackgroundMask2_lz_bit1_count_reload:

             move.b  (%%a0)+, %%d4
             roxr.b  #1,      %%d4
             dbra    %%d5,    GameLoadBackgroundMask2_lz_bit1_count_again

  GameLoadBackgroundMask2_lz_bit1_count_skip:

             addx.w  %%d6,    %%d6

  GameLoadBackgroundMask2_lz_bit1_count_skip2:

             addq.w  #5,      %%d6
             move.l  %%d6,    %%d5
             and.w   #3,      %%d5
             lsr.w   #2,      %%d6
             subq.w  #1,      %%d6
             move.l  %%d3,    %%d7
             move.l  %%a1,    %%a3
             btst.b  #0,      %%d2
             bne     GameLoadBackgroundMask2_bit1_lz_pos_low
             btst.b  #1,      %%d2
             bne     GameLoadBackgroundMask2_bit1_lz_pos_high
             move.w  %%d5,    %%d2
             addq.w  #3,      %%d7
             lsr.w   #2,      %%d7
             suba.l  %%d7,    %%a3
             btst.b  #0,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_zero_low
             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_zero_high

  GameLoadBackgroundMask2_bit1_lz_zero_zero:

             move.b  (%%a3)+, (%%a1)+
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_zero_zero
             bra     GameLoadBackgroundMask2_lz_again3

  GameLoadBackgroundMask2_bit1_lz_zero_low:

             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_zero_high2
             move.b  (%%a3)+, %%d0
             ror.b   #2,      %%d0
             moveq   #63,     %%d3

  GameLoadBackgroundMask2_bit1_lz_zero_one:

             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #192,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_zero_one
             move.b  (%%a3),  %%d1
             lsr.b   #2,      %%d1
             and.b   #192,    %%d0
             or.b    %%d0,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_zero_high:

             move.b  (%%a3)+, %%d0
             ror.b   #4,      %%d0
             moveq   #15,     %%d3

  GameLoadBackgroundMask2_bit1_lz_zero_two:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #240,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_zero_two
             move.b  (%%a3),  %%d1
             lsr.b   #4,      %%d1
             and.b   #240,    %%d0
             or.b    %%d0,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_zero_high2:

             move.b  (%%a3)+, %%d0
             add.b   %%d0,    %%d0
             add.b   %%d0,    %%d0
             moveq   #3,      %%d3

  GameLoadBackgroundMask2_bit1_lz_zero_three:

             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #252,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_zero_three
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_pos_high:

             add.w   %%d5,    %%d2
             and.w   #3,      %%d2
             addq.w  #1,      %%d7
             lsr.w   #2,      %%d7
             suba.l  %%d7,    %%a3
             lsl.b   #4,      %%d1
             btst.b  #0,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_two_low
             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_two_high
             move.b  (%%a3)+, %%d0
             and.b   #15,     %%d0
             or.b    %%d0,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_two_zero:

             move.b  (%%a3)+, (%%a1)+
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_two_zero
             sub.b   #2,      %%d5
             bcc     GameLoadBackgroundMask2_lz_again3
             move.b  -(%%a1), %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_two_low:

             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_two_high2
             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   #15,     %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+
             moveq   #63,     %%d3

  GameLoadBackgroundMask2_bit1_lz_two_one:

             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #192,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_two_one
             sub.b   #2,      %%d5
             bcc     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_two_high:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.l  %%d7,    %%d0
             and.b   #15,     %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+
             moveq   #15,     %%d3

  GameLoadBackgroundMask2_bit1_lz_two_two:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #240,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_two_two
             sub.b   #2,      %%d5
             bcc     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_two_high2:

             moveq   #3,      %%d3
             move.b  (%%a3)+, %%d7
             and.b   %%d3,    %%d7
             add.b   %%d7,    %%d7
             add.b   %%d7,    %%d7
             or.b    %%d7,    %%d1
             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_two_three:

             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #252,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_two_three
             sub.b   #2,      %%d5
             bcc     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_pos_low:

             btst.b  #1,      %%d2
             bne     GameLoadBackgroundMask2_bit1_lz_pos_high2
             add.w   %%d5,    %%d2
             and.w   #3,      %%d2
             addq.w  #2,      %%d7
             lsl.b   #6,      %%d1
             lsr.w   #2,      %%d7
             suba.l  %%d7,    %%a3
             btst.b  #0,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_one_low
             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_one_high
             move.b  (%%a3)+, %%d7
             and.b   #63,     %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_one_zero:

             move.b  (%%a3)+, (%%a1)+
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_one_zero
             sub.b   #3,      %%d5
             bcc     GameLoadBackgroundMask2_lz_again2
             move.b  -(%%a1), %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_one_low:

             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_one_high2
             moveq   #63,     %%d3
             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_one_one:

             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #192,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_one_one
             sub.b   #3,      %%d5
             bcc     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_one_high:

             moveq   #15,     %%d3
             move.b  (%%a3)+, %%d7
             and.b   #3,      %%d7
             lsl.b   #4,      %%d7
             or.b    %%d7,    %%d1
             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_one_two:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #240,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_one_two
             sub.b   #3,      %%d5
             bcc     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_one_high2:

             moveq   #3,      %%d3
             move.b  (%%a3)+, %%d7
             and.b   #15,     %%d7
             add.b   %%d7,    %%d7
             add.b   %%d7,    %%d7
             or.b    %%d7,    %%d1
             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_one_three:

             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #252,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_one_three
             sub.b   #3,      %%d5
             bcc     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_pos_high2:

             add.w   %%d5,    %%d2
             and.w   #3,      %%d2
             lsr.w   #2,      %%d7
             add.b   %%d1,    %%d1
             add.b   %%d1,    %%d1
             suba.l  %%d7,    %%a3
             btst.b  #0,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_three_low
             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_three_high
             move.b  (%%a3)+, %%d7
             and.b   #3,      %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_three_zero:

             move.b  (%%a3)+, (%%a1)+
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_three_zero
             tst.b   %%d5
             bne     GameLoadBackgroundMask2_lz_again3
             move.b  -(%%a1), %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_three_low:

             btst.b  #1,      %%d3
             bne.b   GameLoadBackgroundMask2_bit1_lz_three_high2
             moveq   #63,     %%d3
             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   #3,      %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_three_one:

             move.b  (%%a3)+, %%d7
             ror.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #192,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_three_one
             move.b  (%%a3),  %%d1
             lsr.b   #2,      %%d1
             and.b   #192,    %%d0
             or.b    %%d0,    %%d1
             tst.b   %%d5
             bne     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_three_high:

             moveq   #15,     %%d3
             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.l  %%d7,    %%d0
             and.b   #3,      %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_three_two:

             move.b  (%%a3)+, %%d7
             ror.b   #4,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #240,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_three_two
             tst.b   %%d5
             bne     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_bit1_lz_three_high2:

             moveq   #3,      %%d3
             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.l  %%d7,    %%d0
             and.b   %%d3,    %%d7
             or.b    %%d7,    %%d1
             move.b  %%d1,    (%%a1)+

  GameLoadBackgroundMask2_bit1_lz_three_three:

             move.b  (%%a3)+, %%d7
             rol.b   #2,      %%d7
             move.w  %%d7,    %%d1
             and.b   %%d3,    %%d7
             and.b   #252,    %%d0
             or.b    %%d0,    %%d7
             move.b  %%d7,    (%%a1)+
             move.w  %%d1,    %%d0
             dbra    %%d6,    GameLoadBackgroundMask2_bit1_lz_three_three
             tst.b   %%d5
             bne     GameLoadBackgroundMask2_lz_again2
             subq.l  #1,      %%a1
             move.b  %%d7,    %%d1
             bra     GameLoadBackgroundMask2_lz_again2

  GameLoadBackgroundMask2_exit:

  ": : "r" (size),
       "r" (src),
       "r" (dest),
       "r" (ptrConvertGameBase));

  // pop all registers off stack
  asm("      movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a5": :);
#endif

  MemHandleUnlock(memHandle);
}
#endif
