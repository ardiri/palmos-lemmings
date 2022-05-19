/*
 * @(#)game_spr.inc
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

//
// PalmOS Win* API routines
//

#if USE_PALMOS_WINAPI
/**
 * Backup a tile from the screen to the background buffer.
 *
 * @param x         the absolute x-coordinate in the background buffer
 * @param y         the absolute y-coordinate in the background buffer
 * @param width     the width of the area to backup
 * @param height    the height of the area to backup
 * @param winBackup where to place the background buffer contents at x,y
 */
static void 
GameBackupTile(Int16 x, Int16 y, Int16 width, Int16 height, WinHandle winBackup)
{
  RectangleType scrRect, rect;

  rect.topLeft.x    = 0;
  rect.topLeft.y    = 0;
  rect.extent.x     = width;
  rect.extent.y     = height;
  scrRect.topLeft.x = x;
  scrRect.topLeft.y = y + SPR_HEIGHT;
  scrRect.extent.x  = width;
  scrRect.extent.y  = height;

  // take a backup copy!
  WinCopyRectangle(GraphicsGetDrawWindow(), winBackup,
                   &scrRect, rect.topLeft.x, rect.topLeft.y, winPaint);
}

/**
 * Restore a tile from the background buffer to the screen.
 *
 * @param x         the absolute x-coordinate in the background buffer
 * @param y         the absolute y-coordinate in the background buffer
 * @param width     the width of the area to backup
 * @param height    the height of the area to backup
 * @param winBackup where to place the background buffer contents at x,y
 */
static void 
GameRestoreTile(Int16 x, Int16 y, Int16 width, Int16 height, WinHandle winBackup)
{
  RectangleType scrRect, rect;

  rect.topLeft.x    = 0;
  rect.topLeft.y    = 0;
  rect.extent.x     = width;
  rect.extent.y     = height;
  scrRect.topLeft.x = x;
  scrRect.topLeft.y = y + SPR_HEIGHT;
  scrRect.extent.x  = width;
  scrRect.extent.y  = height;

  // restore the backup copy!
  WinCopyRectangle(winBackup, GraphicsGetDrawWindow(),
                   &rect, scrRect.topLeft.x, scrRect.topLeft.y, winPaint);
}

/**
 * Get the bounding rectangle for a particular sprite in the bitmap/masks
 *
 * @param sprite the sprite to deal with
 * @param index  the sprite index 
 * @param rect   the rectangle to store the bounding area.
 */
static void    
GameGetLemmingRectangle(UInt8 sprite, UInt8 index, RectangleType *rect)
{
  rect->extent.x  = SPR_WIDTH;
  rect->extent.y  = SPR_HEIGHT;

  switch (sprite)
  {
    case spr_undefined:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (index >> 3);
         break;

    case spr_walkRight:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 10;
         break;

    case spr_walkLeft:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 11;
         break;

    case spr_fallRight:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 12;
         break;

    case spr_fallLeft:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 13;
         break;

    case spr_digger:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (14 + (index >> 3));
         break;

    case spr_bashRight:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (16 + (index >> 3));
         break;

    case spr_bashLeft:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (20 + (index >> 3));
         break;

    case spr_floatRight:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 24;
         break;

    case spr_floatLeft:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 25;
         break;

    case spr_blocker:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (26 + (index >> 3));
         break;

    case spr_exploder:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (28 + (index >> 3));
         break;

    case spr_splatter:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (32 + (index >> 3));
         break;

    case spr_homer:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 34;
         break;

    case spr_climbRight:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 35;
         break;

    case spr_climbLeft:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 36;
         break;

    case spr_hopupRight:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 37;
         break;

    case spr_hopupLeft:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 38;
         break;

    case spr_buildRight:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (39 + (index >> 3));
         break;

    case spr_buildLeft:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (41 + (index >> 3));
         break;

    case spr_waveRight:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 43;
         break;

    case spr_waveLeft:
         rect->topLeft.x = SPR_WIDTH  * index;
         rect->topLeft.y = SPR_HEIGHT * 44;
         break;

    case spr_mineRight:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (45 + (index >> 3));
         break;

    case spr_mineLeft:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (48 + (index >> 3));
         break;

    case spr_drowner:
         rect->topLeft.x = SPR_WIDTH  * (index & 0x07);
         rect->topLeft.y = SPR_HEIGHT * (51 + (index >> 3));
         break;

    default:
         break;
  }
}

#else

//
// custom written Win* API routines
//

/**
 * Draw a lemming sprite to the offscreen 2bpp window (inc. backup of area).
 * 
 * @param buff        the buffer to copy into.
 * @param spriteType  the sprite to deal with
 * @param spriteIndex the sprite index 
 * @param x           the x co-ordinate in the offscreen window.
 * @param y           the y co-ordinate in the offscreen window.
 */
static void    
GameDrawSprite2bpp(UInt8 *buff, 
                   UInt16 spriteType, UInt16 spriteIndex, Int16 x, Int16 y)
{
  UInt8  *ptrScreen, *ptrMask, *ptrData;
#if PORTABLE
  UInt8  mask, data, scrn, scrn2;
  UInt16 i, j;
#endif
  UInt16 offset, sx, sy;

  switch (spriteType)
  {
    case spr_undefined:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (spriteIndex >> 3);
         break;

    case spr_walkRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 10;
         break;

    case spr_walkLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 11;
         break;

    case spr_fallRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 12;
         break;

    case spr_fallLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 13;
         break;

    case spr_digger:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (14 + (spriteIndex >> 3));
         break;

    case spr_bashRight:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (16 + (spriteIndex >> 3));
         break;

    case spr_bashLeft:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (20 + (spriteIndex >> 3));
         break;

    case spr_floatRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 24;
         break;

    case spr_floatLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 25;
         break;

    case spr_blocker:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (26 + (spriteIndex >> 3));
         break;

    case spr_exploder:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (28 + (spriteIndex >> 3));
         break;

    case spr_splatter:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (32 + (spriteIndex >> 3));
         break;

    case spr_homer:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 34;
         break;

    case spr_climbRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 35;
         break;

    case spr_climbLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 36;
         break;

    case spr_hopupRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 37;
         break;

    case spr_hopupLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 38;
         break;

    case spr_buildRight:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (39 + (spriteIndex >> 3));
         break;

    case spr_buildLeft:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (41 + (spriteIndex >> 3));
         break;

    case spr_waveRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 43;
         break;

    case spr_waveLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 44;
         break;

    case spr_mineRight:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (45 + (spriteIndex >> 3));
         break;

    case spr_mineLeft:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (48 + (spriteIndex >> 3));
         break;

    case spr_drowner:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (51 + (spriteIndex >> 3));
         break;

    default:
         sx = 0;
         sy = 0;  // should never happen :)
         break;
  }

  // get pointers to everything
  ptrScreen  = globals.ptrWinDraw;
  ptrData    = globals.ptrLemmings;
  ptrMask    = globals.ptrLemmingsMask;

//ptrScreen += ((y + SPR_HEIGHT)  * 160) + (x / 4);
//offset     = (sy * 32) + (sx / 4);
  ptrScreen += (UInt32)(((UInt16)(y + SPR_HEIGHT) << 5) * 5) + (x >> 2);
  offset     = (sy << 5) + (sx >> 2);
  ptrData   += offset;
  ptrMask   += offset;

  switch (x % 4)
  {
    // aligned - simple mask+overlay
    case 0:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do {
             *buff++ = scrn = *ptrScreen; 

             *ptrScreen++ = (scrn & ~*(ptrMask++)) | *(ptrData++); 
           } while (--i);

           buff++; // skip over the extra byte we aint using :)
           ptrScreen += 160 - (SPR_WIDTH >> 2);
           offset     = 32  - (SPR_WIDTH >> 2);
           ptrMask   += offset;
           ptrData   += offset;
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d3/%%a0-%%a4, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));
    asm("      move.l  %0, %%a2": :"g"(ptrData));
    asm("      move.l  %0, %%a3": :"g"(buff));

    asm("      moveq   #7,   %%d0
               move.l  %%a0, %%d1
               btst.b  #0,   %%d1
               beq.s   SpriteDraw2bpp_0_even
               movea.l %%a0, %%a4
               subq.l  #1,   %%a4
  
    SpriteDraw2bpp_0_odd:

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+,  %%d1
               rol.l   #8,       %%d1
               not.b   %%d1
               and.b   (%%a0),   %%d1
               move.l  (%%a2)+,  %%d2
               rol.l   #8,       %%d2
               or.b    %%d2,     %%d1
               move.b  %%d1,     (%%a0)+

               not.l   %%d1
               move.l  (%%a0),   %%d3
               and.l   %%d3,     %%d1
               or.l    %%d2,     %%d1
               move.b  %%d3,     %%d1
               move.l  %%d1,     (%%a0)+

               lea     155(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+,  %%d1
               rol.l   #8,       %%d1
               not.b   %%d1
               and.b   (%%a0),   %%d1
               move.l  (%%a2)+,  %%d2
               rol.l   #8,       %%d2
               or.b    %%d2,     %%d1
               move.b  %%d1,     (%%a0)+

               not.l   %%d1
               move.l  (%%a0),   %%d3
               and.l   %%d3,     %%d1
               or.l    %%d2,     %%d1
               move.b  %%d3,     %%d1
               move.l  %%d1,     (%%a0)+

               lea     155(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               dbra    %%d0, SpriteDraw2bpp_0_odd
               bra.s   SpriteDraw2bpp_0_exit
  
    SpriteDraw2bpp_0_even:

               move.l  (%%a1)+, %%d1
               not.l   %%d1
               move.l  (%%a0),  %%d2
               move.l  %%d2,    (%%a3)+
               and.l   %%d2,    %%d1
               or.l    (%%a2)+, %%d1
               move.l  %%d1,    (%%a0)+

               lea     156(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               move.l  (%%a1)+, %%d1
               not.l   %%d1
               move.l  (%%a0),  %%d2
               move.l  %%d2,    (%%a3)+
               and.l   %%d2,    %%d1
               or.l    (%%a2)+, %%d1
               move.l  %%d1,    (%%a0)+

               lea     156(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               dbra    %%d0, SpriteDraw2bpp_0_even
  
    SpriteDraw2bpp_0_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d3/%%a0-%%a4": :);
#endif

         break;

    // mis aligned by 2 bits - need to shift, mask+overlay and mess around
    case 1:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           scrn2 = *ptrScreen;
           do 
           {
             mask = *(ptrMask++);
             data = *(ptrData++);
     
             *buff++ = scrn2;
     
             scrn = *ptrScreen;
             scrn = ((scrn & ~(mask >> 2)) | (data >> 2));
             *ptrScreen++ = scrn;
     
             scrn2 = scrn = *ptrScreen;
             scrn = ((scrn & ~(mask << 6)) | (data << 6));
             *ptrScreen = scrn;
           }
           while (--i);
     
           *buff++ = scrn2;

           ptrScreen += 160 - (SPR_WIDTH >> 2);
           offset     = 32  - (SPR_WIDTH >> 2);
           ptrMask   += offset;
           ptrData   += offset;
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d6/%%a0-%%a4, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));
    asm("      move.l  %0, %%a2": :"g"(ptrData));
    asm("      move.l  %0, %%a3": :"g"(buff));

    asm("      moveq   #7,   %%d0
               movea.l %%a0, %%a4
               move.l  %%a0, %%d1
               btst.b  #0,   %%d1
               beq     SpriteDraw2bpp_1_even
               subq.l  #1,   %%a4
               move.l  #4294967232, %%d5    | 0xffffffc0
               moveq   #63,  %%d6           | 0x0000003f
  
    SpriteDraw2bpp_1_odd:

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d1
               rol.l   #6,      %%d1
               move.l  %%d1,    %%d3
               and.b   %%d6,    %%d3
               not.b   %%d3
               and.b   (%%a0),  %%d3
               move.l  (%%a2)+, %%d2
               rol.l   #6,      %%d2
               move.l  %%d2,    %%d4
               and.b   %%d6,    %%d4
               or.b    %%d4,    %%d3
               move.b  %%d3,    (%%a0)+

               and.b   %%d5,    %%d1
               not.l   %%d1
               and.l   (%%a0),  %%d1
               and.b   %%d5,    %%d2
               or.l    %%d2,    %%d1
               move.l  %%d1,    (%%a0)+

               lea     155(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d1
               rol.l   #6,      %%d1
               move.l  %%d1,    %%d3
               and.b   %%d6,    %%d3
               not.b   %%d3
               and.b   (%%a0),  %%d3
               move.l  (%%a2)+, %%d2
               rol.l   #6,      %%d2
               move.l  %%d2,    %%d4
               and.b   %%d6,    %%d4
               or.b    %%d4,    %%d3
               move.b  %%d3,    (%%a0)+

               and.b   %%d5,    %%d1
               not.l   %%d1
               and.l   (%%a0),  %%d1
               and.b   %%d5,    %%d2
               or.l    %%d2,    %%d1
               move.l  %%d1,    (%%a0)+

               lea     155(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               dbra    %%d0, SpriteDraw2bpp_1_odd
               bra     SpriteDraw2bpp_1_exit
  
    SpriteDraw2bpp_1_even:

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #2,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #2,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+

               lsl.b   #6,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #6,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               lea     156(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #2,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #2,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+

               lsl.b   #6,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #6,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               lea     156(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               dbra    %%d0, SpriteDraw2bpp_1_even

    SpriteDraw2bpp_1_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d6/%%a0-%%a4": :);
#endif
 
         break;

    // mis aligned by 4 bits - need to shift, mask+overlay and mess around
    case 2:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           scrn2 = *ptrScreen;
           do 
           {
             mask = *(ptrMask++);
             data = *(ptrData++);
     
             *buff++ = scrn2;
     
             scrn = *ptrScreen;
             scrn = ((scrn & ~(mask >> 4)) | (data >> 4));
             *ptrScreen++ = scrn;
     
             scrn2 = scrn = *ptrScreen;
             scrn = ((scrn & ~(mask << 4)) | (data << 4));
             *ptrScreen = scrn;
           }
           while (--i);
     
           *buff++ = scrn2;

           ptrScreen += 160 - (SPR_WIDTH >> 2);
           offset     = 32  - (SPR_WIDTH >> 2);
           ptrMask   += offset;
           ptrData   += offset;
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d6/%%a0-%%a4, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));
    asm("      move.l  %0, %%a2": :"g"(ptrData));
    asm("      move.l  %0, %%a3": :"g"(buff));

    asm("      moveq   #7,   %%d0
               movea.l %%a0, %%a4
               move.l  %%a0, %%d1
               btst.b  #0,   %%d1
               beq     SpriteDraw2bpp_2_even
               subq.l  #1,   %%a4
               move.l  #4294967280, %%d5    | 0xfffffff0
               moveq   #15,  %%d6           | 0x0000000f
  
    SpriteDraw2bpp_2_odd:

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d1
               rol.l   #4,      %%d1
               move.l  %%d1,    %%d3
               and.b   %%d6,    %%d3
               not.b   %%d3
               and.b   (%%a0),  %%d3
               move.l  (%%a2)+, %%d2
               rol.l   #4,      %%d2
               move.l  %%d2,    %%d4
               and.b   %%d6,    %%d4
               or.b    %%d4,    %%d3
               move.b  %%d3,    (%%a0)+

               and.b   %%d5,    %%d1
               not.l   %%d1
               and.l   (%%a0),  %%d1
               and.b   %%d5,    %%d2
               or.l    %%d2,    %%d1
               move.l  %%d1,    (%%a0)+

               lea     155(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d1
               rol.l   #4,      %%d1
               move.l  %%d1,    %%d3
               and.b   %%d6,    %%d3
               not.b   %%d3
               and.b   (%%a0),  %%d3
               move.l  (%%a2)+, %%d2
               rol.l   #4,      %%d2
               move.l  %%d2,    %%d4
               and.b   %%d6,    %%d4
               or.b    %%d4,    %%d3
               move.b  %%d3,    (%%a0)+

               and.b   %%d5,    %%d1
               not.l   %%d1
               and.l   (%%a0),  %%d1
               and.b   %%d5,    %%d2
               or.l    %%d2,    %%d1
               move.l  %%d1,    (%%a0)+

               lea     155(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               dbra    %%d0, SpriteDraw2bpp_2_odd
               bra     SpriteDraw2bpp_2_exit
  
    SpriteDraw2bpp_2_even:

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #4,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #4,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+

               lsl.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #4,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               lea     156(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #4,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #4,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+

               lsl.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #4,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               lea     156(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               dbra    %%d0, SpriteDraw2bpp_2_even

    SpriteDraw2bpp_2_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d6/%%a0-%%a4": :);
#endif
 
         break;

    // mis aligned by 6 bits - need to shift, mask+overlay and mess around
    case 3:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           scrn2 = *ptrScreen;
           do 
           {
             mask = *(ptrMask++);
             data = *(ptrData++);
     
             *buff++ = scrn2;
     
             scrn = *ptrScreen;
             scrn = ((scrn & ~(mask >> 6)) | (data >> 6));
             *ptrScreen++ = scrn;
     
             scrn2 = scrn = *ptrScreen;
             scrn = ((scrn & ~(mask << 2)) | (data << 2));
             *ptrScreen = scrn;
           }
           while (--i);
     
           *buff++ = scrn2;

           ptrScreen += 160 - (SPR_WIDTH >> 2);
           offset     = 32  - (SPR_WIDTH >> 2);
           ptrMask   += offset;
           ptrData   += offset;
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d6/%%a0-%%a4, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));
    asm("      move.l  %0, %%a2": :"g"(ptrData));
    asm("      move.l  %0, %%a3": :"g"(buff));

    asm("      moveq   #7,   %%d0
               movea.l %%a0, %%a4
               move.l  %%a0, %%d1
               btst.b  #0,   %%d1
               beq     SpriteDraw2bpp_3_even
               subq.l  #1,   %%a4
               move.l  #4294967292, %%d5    | 0xfffffffc
               moveq   #3,   %%d6           | 0x00000003
  
    SpriteDraw2bpp_3_odd:

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d1
               rol.l   #2,      %%d1
               move.l  %%d1,    %%d3
               and.b   %%d6,    %%d3
               not.b   %%d3
               and.b   (%%a0),  %%d3
               move.l  (%%a2)+, %%d2
               rol.l   #2,      %%d2
               move.l  %%d2,    %%d4
               and.b   %%d6,    %%d4
               or.b    %%d4,    %%d3
               move.b  %%d3,    (%%a0)+

               and.b   %%d5,    %%d1
               not.l   %%d1
               and.l   (%%a0),  %%d1
               and.b   %%d5,    %%d2
               or.l    %%d2,    %%d1
               move.l  %%d1,    (%%a0)+

               lea     155(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d1
               rol.l   #2,      %%d1
               move.l  %%d1,    %%d3
               and.b   %%d6,    %%d3
               not.b   %%d3
               and.b   (%%a0),  %%d3
               move.l  (%%a2)+, %%d2
               rol.l   #2,      %%d2
               move.l  %%d2,    %%d4
               and.b   %%d6,    %%d4
               or.b    %%d4,    %%d3
               move.b  %%d3,    (%%a0)+

               and.b   %%d5,    %%d1
               not.l   %%d1
               and.l   (%%a0),  %%d1
               and.b   %%d5,    %%d2
               or.l    %%d2,    %%d1
               move.l  %%d1,    (%%a0)+

               lea     155(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               dbra    %%d0, SpriteDraw2bpp_3_odd
               bra     SpriteDraw2bpp_3_exit
  
    SpriteDraw2bpp_3_even:

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #6,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #6,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+

               lsl.b   #2,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #2,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               lea     156(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     154(%%a4), %%a4

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #6,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #6,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+

               lsl.b   #2,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #2,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               lea     156(%%a0), %%a0
               lea     28(%%a1),  %%a1
               lea     28(%%a2),  %%a2

               dbra    %%d0, SpriteDraw2bpp_3_even

    SpriteDraw2bpp_3_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d6/%%a0-%%a4": :);
#endif
 
         break;

    default:
         break;
  }
}

/**
 * Restore a 16x16 sprite area from backup buffer to the offscreen 2bpp window.
 * 
 * @param buff the buffer to copy from.
 * @param x    the x co-ordinate in the offscreen window.
 * @param y    the y co-ordinate in the offscreen window.
 */
static void    
GameSpriteRestore2bpp(UInt8 *buff, Int16 x, Int16 y)
{
  UInt8  *ptrScreen;
#if PORTABLE
  UInt16 i, j;
#endif

  // get pointers to everything
  ptrScreen  = globals.ptrWinDraw;
//ptrScreen += ((y + SPR_HEIGHT) * 160) + (x / 4);
  ptrScreen += (UInt32)(((UInt16)(y + SPR_HEIGHT) << 5) * 5) + (x >> 2);

  // aligned - simple blit
  if ((x % 4) == 0)
  {
#if PORTABLE
    j = SPR_HEIGHT;
    do
    {
      i = (SPR_WIDTH >> 2);
      do 
        *ptrScreen++ = *buff++;
      while (--i);

      buff++; // skip over the extra byte we aint using :)
      ptrScreen += 160 - (SPR_WIDTH >> 2);
    }
    while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(buff));

    asm("      move.l  %%a0, %%d0
               btst.b  #0,   %%d0
               beq.s   SpriteRestore2bpp_0_even_start
               subq.l  #1,   %%a0
               moveq   #7,   %%d0
  
    SpriteRestore2bpp_0_odd:

               move.l  (%%a1)+, (%%a0)+
               move.w  (%%a1)+, (%%a0)+

               lea     154(%%a0), %%a0

               move.l  (%%a1)+, (%%a0)+
               move.w  (%%a1)+, (%%a0)+

               lea     154(%%a0), %%a0

               dbra    %%d0, SpriteRestore2bpp_0_odd
               bra.s   SpriteRestore2bpp_0_exit
  
    SpriteRestore2bpp_0_even_start:

               moveq   #7, %%d0
  
    SpriteRestore2bpp_0_even:

               move.l  (%%a1)+, (%%a0)+

               lea     156(%%a0), %%a0

               move.l  (%%a1)+, (%%a0)+

               lea     156(%%a0), %%a0

               dbra    %%d0, SpriteRestore2bpp_0_even
  
    SpriteRestore2bpp_0_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0/%%a0-%%a1": :);
#endif
  }

  // mis-aligned by 2/4/6 bits
  else
  {
#if PORTABLE
    j = SPR_HEIGHT;
    do
    {
      i = (SPR_WIDTH >> 2) + 1;
      do 
        *ptrScreen++ = *buff++;
      while (--i);

      ptrScreen += 160 - ((SPR_WIDTH >> 2) + 1);
    }
    while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(buff));

    asm("      move.l  %%a0, %%d0
               btst.b  #0,   %%d0
               beq.s   SpriteRestore2bpp_1_even_start
               subq.l  #1,   %%a0
               moveq   #7,   %%d0
  
    SpriteRestore2bpp_1_odd:

               move.l (%%a1)+, (%%a0)+
               move.w (%%a1)+, (%%a0)+

               lea    154(%%a0), %%a0

               move.l (%%a1)+, (%%a0)+
               move.w (%%a1)+, (%%a0)+

               lea    154(%%a0), %%a0

               dbra   %%d0, SpriteRestore2bpp_1_odd
               bra.s  SpriteRestore2bpp_1_exit
  
    SpriteRestore2bpp_1_even_start:

               moveq  #7, %%d0
  
    SpriteRestore2bpp_1_even:

               move.l (%%a1)+, (%%a0)+
               move.w (%%a1)+, (%%a0)+

               lea    154(%%a0), %%a0

               move.l (%%a1)+, (%%a0)+
               move.w (%%a1)+, (%%a0)+

               lea    154(%%a0), %%a0

               dbra   %%d0, SpriteRestore2bpp_1_even
  
    SpriteRestore2bpp_1_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0/%%a0-%%a1": :);
#endif
  }
}

/**
 * Draw a lemming sprite to the offscreen 4bpc window (inc. backup of area).
 * 
 * @param buff        the buffer to copy into.
 * @param spriteType  the sprite to deal with
 * @param spriteIndex the sprite index 
 * @param x           the x co-ordinate in the offscreen window.
 * @param y           the y co-ordinate in the offscreen window.
 */
static void    
GameDrawSprite4bpc(UInt8 *buff, 
                   UInt16 spriteType, UInt16 spriteIndex, Int16 x, Int16 y)
{
  UInt8  *ptrScreen, *ptrMask, *ptrData;
#if PORTABLE
  UInt8  mask, data, scrn, scrn2;
  UInt16 i, j;
#endif
  UInt16 offset, sx, sy;

  switch (spriteType)
  {
    case spr_undefined:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (spriteIndex >> 3);
         break;

    case spr_walkRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 10;
         break;

    case spr_walkLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 11;
         break;

    case spr_fallRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 12;
         break;

    case spr_fallLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 13;
         break;

    case spr_digger:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (14 + (spriteIndex >> 3));
         break;

    case spr_bashRight:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (16 + (spriteIndex >> 3));
         break;

    case spr_bashLeft:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (20 + (spriteIndex >> 3));
         break;

    case spr_floatRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 24;
         break;

    case spr_floatLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 25;
         break;

    case spr_blocker:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (26 + (spriteIndex >> 3));
         break;

    case spr_exploder:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (28 + (spriteIndex >> 3));
         break;

    case spr_splatter:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (32 + (spriteIndex >> 3));
         break;

    case spr_homer:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 34;
         break;

    case spr_climbRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 35;
         break;

    case spr_climbLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 36;
         break;

    case spr_hopupRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 37;
         break;

    case spr_hopupLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 38;
         break;

    case spr_buildRight:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (39 + (spriteIndex >> 3));
         break;

    case spr_buildLeft:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (41 + (spriteIndex >> 3));
         break;

    case spr_waveRight:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 43;
         break;

    case spr_waveLeft:
         sx = SPR_WIDTH  * spriteIndex;
         sy = SPR_HEIGHT * 44;
         break;

    case spr_mineRight:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (45 + (spriteIndex >> 3));
         break;

    case spr_mineLeft:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (48 + (spriteIndex >> 3));
         break;

    case spr_drowner:
         sx = SPR_WIDTH  * (spriteIndex & 0x07);
         sy = SPR_HEIGHT * (51 + (spriteIndex >> 3));
         break;

    default:
         sx = 0;
         sy = 0;  // should never happen :)
         break;
  }

  // get pointers to everything
  ptrScreen  = globals.ptrWinDraw;
  ptrData    = globals.ptrLemmings;
  ptrMask    = globals.ptrLemmingsMask;

//ptrScreen += ((y + SPR_HEIGHT) * 320) + (x / 2);
//offset     = (sy * 64) + (sx / 2);
  ptrScreen += (UInt32)(((UInt16)(y + SPR_HEIGHT) << 6) * 5) + (x >> 1);
  offset     = ((sy << 6) + (sx >> 1));
  ptrData   += offset;
  ptrMask   += offset;

  // aligned - simple mask+overlay
  if ((x % 2) == 0) 
  {
#if PORTABLE
    j = SPR_HEIGHT;
    do
    {
      i = (SPR_WIDTH >> 1);
      do {
        *buff++ = scrn = *ptrScreen; 

        *ptrScreen++ = (scrn & ~*(ptrMask++)) | *(ptrData++); 
      } while (--i);

      buff++; // skip over the extra byte we aint using :)
      ptrScreen += 320 - (SPR_WIDTH >> 1);
      offset     = 64  - (SPR_WIDTH >> 1);
      ptrMask   += offset;
      ptrData   += offset;
    }
    while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a4, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));
    asm("      move.l  %0, %%a2": :"g"(ptrData));
    asm("      move.l  %0, %%a3": :"g"(buff));

    asm("      moveq   #7,   %%d0
               move.l  %%a0, %%d1
               btst.b  #0,   %%d1
               beq     SpriteDraw4bpp_0_even
               movea.l %%a0, %%a4
               subq.l  #1,   %%a4
  
    SpriteDraw4bpp_0_odd:

               move.l  (%%a4)+, (%%a3)+
               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     310(%%a4), %%a4

               move.l  (%%a1)+,  %%d1
               rol.l   #8,       %%d1
               not.b   %%d1
               and.b   (%%a0),   %%d1
               move.l  (%%a2)+,  %%d2
               rol.l   #8,       %%d2
               or.b    %%d2,     %%d1
               move.b  %%d1,     (%%a0)+

               move.l  (%%a1)+,  %%d4
               rol.l   #8,       %%d4
               move.b  %%d4,     %%d1
               not.l   %%d1
               and.l   (%%a0),   %%d1
               move.l  (%%a2)+,  %%d3
               rol.l   #8,       %%d3
               move.b  %%d3,     %%d2
               or.l    %%d2,     %%d1
               move.l  %%d1,     (%%a0)+

               not.l   %%d4
               move.l  (%%a0),   %%d1
               and.l   %%d1,     %%d4
               or.l    %%d3,     %%d4
               move.b  %%d1,     %%d4
               move.l  %%d4,     (%%a0)+

               lea     311(%%a0), %%a0
               lea     56(%%a1),  %%a1
               lea     56(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     310(%%a4), %%a4

               move.l  (%%a1)+,  %%d1
               rol.l   #8,       %%d1
               not.b   %%d1
               and.b   (%%a0),   %%d1
               move.l  (%%a2)+,  %%d2
               rol.l   #8,       %%d2
               or.b    %%d2,     %%d1
               move.b  %%d1,     (%%a0)+

               move.l  (%%a1)+,  %%d4
               rol.l   #8,       %%d4
               move.b  %%d4,     %%d1
               not.l   %%d1
               and.l   (%%a0),   %%d1
               move.l  (%%a2)+,  %%d3
               rol.l   #8,       %%d3
               move.b  %%d3,     %%d2
               or.l    %%d2,     %%d1
               move.l  %%d1,     (%%a0)+

               not.l   %%d4
               move.l  (%%a0),   %%d1
               and.l   %%d1,     %%d4
               or.l    %%d3,     %%d4
               move.b  %%d1,     %%d4
               move.l  %%d4,     (%%a0)+

               lea     311(%%a0), %%a0
               lea     56(%%a1),  %%a1
               lea     56(%%a2),  %%a2

               dbra    %%d0, SpriteDraw4bpp_0_odd
               bra.s   SpriteDraw4bpp_0_exit
  
    SpriteDraw4bpp_0_even:

               move.l  (%%a1)+, %%d1
               not.l   %%d1
               move.l  (%%a0),  %%d2
               move.l  %%d2,    (%%a3)+
               and.l   %%d2,    %%d1
               or.l    (%%a2)+, %%d1
               move.l  %%d1,    (%%a0)+

               move.l  (%%a1)+, %%d1
               not.l   %%d1
               move.l  (%%a0),  %%d2
               move.l  %%d2,    (%%a3)+
               and.l   %%d2,    %%d1
               or.l    (%%a2)+, %%d1
               move.l  %%d1,    (%%a0)+

               lea     312(%%a0), %%a0
               lea     56(%%a1),  %%a1
               lea     56(%%a2),  %%a2

               move.l  (%%a1)+, %%d1
               not.l   %%d1
               move.l  (%%a0),  %%d2
               move.l  %%d2,    (%%a3)+
               and.l   %%d2,    %%d1
               or.l    (%%a2)+, %%d1
               move.l  %%d1,    (%%a0)+

               move.l  (%%a1)+, %%d1
               not.l   %%d1
               move.l  (%%a0),  %%d2
               move.l  %%d2,    (%%a3)+
               and.l   %%d2,    %%d1
               or.l    (%%a2)+, %%d1
               move.l  %%d1,    (%%a0)+

               lea     312(%%a0), %%a0
               lea     56(%%a1),  %%a1
               lea     56(%%a2),  %%a2

               dbra    %%d0, SpriteDraw4bpp_0_even
  
    SpriteDraw4bpp_0_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a4": :);
#endif
  } 

  // not aligned - need to shift, mask+overlay and mess around
  else 
  {
#if PORTABLE
    j = SPR_HEIGHT;
    do
    {
      i = (SPR_WIDTH >> 1);
      scrn2 = *ptrScreen;
      do 
      {
        mask = *(ptrMask++);
        data = *(ptrData++);

        *buff++ = scrn2;

        scrn = *ptrScreen;
        scrn = ((scrn & ~(mask >> 4)) | (data >> 4));
        *ptrScreen++ = scrn;

        scrn2 = scrn = *ptrScreen;
        scrn = ((scrn & ~(mask << 4)) | (data << 4));
        *ptrScreen = scrn;
      }
      while (--i);

      *buff++ = scrn2;

      ptrScreen += 320 - (SPR_WIDTH >> 1);
      offset     = 64  - (SPR_WIDTH >> 1);
      ptrMask   += offset;
      ptrData   += offset;
    }
    while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a4, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));
    asm("      move.l  %0, %%a2": :"g"(ptrData));
    asm("      move.l  %0, %%a3": :"g"(buff));

    asm("      moveq   #7,   %%d0
               movea.l %%a0, %%a4
               move.l  %%a0, %%d1
               btst.b  #0,   %%d1
               beq     SpriteDraw4bpp_1_even
               subq.l  #1,   %%a4
  
    SpriteDraw4bpp_1_odd:

               move.l  (%%a4)+, (%%a3)+
               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     310(%%a4), %%a4

               move.l  (%%a1)+, %%d1
               rol.l   #8,      %%d1
               move.b  %%d1,    %%d3
               lsr.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               move.l  (%%a2)+, %%d2
               rol.l   #8,      %%d2
               move.b  %%d2,    %%d4
               lsr.b   #4,      %%d4
               or.b    %%d4,    %%d1
               move.b  %%d1,    (%%a0)+
               lsl.b   #4,      %%d3
               not.b   %%d3
               and.b   (%%a0),  %%d3
               lsl.b   #4,      %%d2
               or.b    %%d2,    %%d3
               move.b  %%d3,    (%%a0)

               move.b  (%%a1),  %%d1
               move.l  %%d1,    %%d3
               lsr.l   #4,      %%d3
               not.l   %%d3
               and.l   (%%a0),  %%d3
               move.b  (%%a2),  %%d2
               move.l  %%d2,    %%d4
               lsr.l   #4,      %%d4
               or.l    %%d4,    %%d3
               move.l  %%d3,    (%%a0)+
               lsl.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #4,      %%d2
               or.b    %%d2,    %%d1
               move.b  %%d1,    (%%a0)

               move.l  (%%a1)+, %%d1
               lsl.l   #4,      %%d1
               not.l   %%d1
               and.l   (%%a0),  %%d1
               move.l  (%%a2)+, %%d2
               lsl.l   #4,      %%d2
               or.l    %%d2,    %%d1
               move.l  %%d1,    (%%a0)+

               lea     311(%%a0), %%a0
               lea     56(%%a1),  %%a1
               lea     56(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     310(%%a4), %%a4

               move.l  (%%a1)+, %%d1
               rol.l   #8,      %%d1
               move.b  %%d1,    %%d3
               lsr.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               move.l  (%%a2)+, %%d2
               rol.l   #8,      %%d2
               move.b  %%d2,    %%d4
               lsr.b   #4,      %%d4
               or.b    %%d4,    %%d1
               move.b  %%d1,    (%%a0)+
               lsl.b   #4,      %%d3
               not.b   %%d3
               and.b   (%%a0),  %%d3
               lsl.b   #4,      %%d2
               or.b    %%d2,    %%d3
               move.b  %%d3,    (%%a0)

               move.b  (%%a1),  %%d1
               move.l  %%d1,    %%d3
               lsr.l   #4,      %%d3
               not.l   %%d3
               and.l   (%%a0),  %%d3
               move.b  (%%a2),  %%d2
               move.l  %%d2,    %%d4
               lsr.l   #4,      %%d4
               or.l    %%d4,    %%d3
               move.l  %%d3,    (%%a0)+
               lsl.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #4,      %%d2
               or.b    %%d2,    %%d1
               move.b  %%d1,    (%%a0)

               move.l  (%%a1)+, %%d1
               lsl.l   #4,      %%d1
               not.l   %%d1
               and.l   (%%a0),  %%d1
               move.l  (%%a2)+, %%d2
               lsl.l   #4,      %%d2
               or.l    %%d2,    %%d1
               move.l  %%d1,    (%%a0)+

               lea     311(%%a0), %%a0
               lea     56(%%a1),  %%a1
               lea     56(%%a2),  %%a2

               dbra    %%d0, SpriteDraw4bpp_1_odd
               bra     SpriteDraw4bpp_1_exit
  
    SpriteDraw4bpp_1_even:

               move.l  (%%a4)+, (%%a3)+
               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     310(%%a4), %%a4

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #4,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #4,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+
               lsl.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #4,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #4,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #4,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+
               lsl.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #4,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               lea     312(%%a0), %%a0
               lea     56(%%a1),  %%a1
               lea     56(%%a2),  %%a2

               move.l  (%%a4)+, (%%a3)+
               move.l  (%%a4)+, (%%a3)+
               move.w  (%%a4)+, (%%a3)+

               lea     310(%%a4), %%a4

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #4,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #4,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+
               lsl.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #4,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               move.l  (%%a1)+, %%d2
               move.b  %%d2,    %%d1
               lsr.l   #4,      %%d2
               not.l   %%d2
               and.l   (%%a0),  %%d2
               move.l  (%%a2)+, %%d4
               move.b  %%d4,    %%d3
               lsr.l   #4,      %%d4
               or.l    %%d4,    %%d2
               move.l  %%d2,    (%%a0)+
               lsl.b   #4,      %%d1
               not.b   %%d1
               and.b   (%%a0),  %%d1
               lsl.b   #4,      %%d3
               or.b    %%d3,    %%d1
               move.b  %%d1,    (%%a0)

               lea     312(%%a0), %%a0
               lea     56(%%a1),  %%a1
               lea     56(%%a2),  %%a2

               dbra    %%d0, SpriteDraw4bpp_1_even

    SpriteDraw4bpp_1_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a4": :);
#endif
  }
}

/**
 * Restore a 16x16 sprite area from backup buffer to the offscreen 4bpc window.
 * 
 * @param buff the buffer to copy from.
 * @param x    the x co-ordinate in the offscreen window.
 * @param y    the y co-ordinate in the offscreen window.
 */
static void    
GameSpriteRestore4bpc(UInt8 *buff, Int16 x, Int16 y)
{
  UInt8  *ptrScreen;
#if PORTABLE
  UInt16 i, j;
#endif

  // get pointers to everything
  ptrScreen  = globals.ptrWinDraw;
//ptrScreen += ((y + SPR_HEIGHT) * 320) + (x / 2);
  ptrScreen += (UInt32)(((UInt16)(y + SPR_HEIGHT) << 6) * 5) + (x >> 1);

  // aligned - simple blit
  if ((x % 2) == 0) 
  {
#if PORTABLE
    j = SPR_HEIGHT;
    do
    {
      i = (SPR_WIDTH >> 1);
      do 
        *ptrScreen++ = *buff++;
      while (--i);

      buff++; // skip over the extra byte we aint using :)
      ptrScreen += 320 - (SPR_WIDTH >> 1);
    }
    while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(buff));

    asm("      move.l  %%a0, %%d0
               btst.b  #0,   %%d0
               beq.s   SpriteRestore4bpp_0_even_start
               subq.l  #1,   %%a0
               moveq   #7,   %%d0
  
    SpriteRestore4bpp_0_odd:

               move.l  (%%a1)+, (%%a0)+
               move.l  (%%a1)+, (%%a0)+
               move.w  (%%a1)+, (%%a0)+

               lea     310(%%a0), %%a0

               move.l  (%%a1)+, (%%a0)+
               move.l  (%%a1)+, (%%a0)+
               move.w  (%%a1)+, (%%a0)+

               lea     310(%%a0), %%a0

               dbra    %%d0, SpriteRestore4bpp_0_odd
               bra.s   SpriteRestore4bpp_0_exit
  
    SpriteRestore4bpp_0_even_start:

               moveq   #7, %%d0
  
    SpriteRestore4bpp_0_even:

               move.l  (%%a1)+, (%%a0)+
               move.l  (%%a1)+, (%%a0)+

               lea     312(%%a0), %%a0

               move.l  (%%a1)+, (%%a0)+
               move.l  (%%a1)+, (%%a0)+

               lea     312(%%a0), %%a0

               dbra    %%d0, SpriteRestore4bpp_0_even
  
    SpriteRestore4bpp_0_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0/%%a0-%%a1": :);
#endif
  } 

  // not aligned - need to shift and copy
  else 
  {
#if PORTABLE
    j = SPR_HEIGHT;
    do
    {
      i = (SPR_WIDTH >> 1) + 1;
      do 
        *ptrScreen++ = *buff++;
      while (--i);

      ptrScreen += 320 - ((SPR_WIDTH >> 1) + 1);
    }
    while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(buff));

    asm("      move.l  %%a0, %%d0
               btst.b  #0,   %%d0
               beq.s   SpriteRestore4bpp_1_even_start
               subq.l  #1,   %%a0
               moveq   #7,   %%d0
  
    SpriteRestore4bpp_1_odd:

               move.l (%%a1)+, (%%a0)+
               move.l (%%a1)+, (%%a0)+
               move.w (%%a1)+, (%%a0)+

               lea    310(%%a0), %%a0

               move.l (%%a1)+, (%%a0)+
               move.l (%%a1)+, (%%a0)+
               move.w (%%a1)+, (%%a0)+

               lea    310(%%a0), %%a0

               dbra   %%d0, SpriteRestore4bpp_1_odd
               bra.s  SpriteRestore4bpp_1_exit
  
    SpriteRestore4bpp_1_even_start:

               moveq  #7, %%d0
  
    SpriteRestore4bpp_1_even:

               move.l (%%a1)+, (%%a0)+
               move.l (%%a1)+, (%%a0)+
               move.w (%%a1)+, (%%a0)+

               lea    310(%%a0), %%a0

               move.l (%%a1)+, (%%a0)+
               move.l (%%a1)+, (%%a0)+
               move.w (%%a1)+, (%%a0)+

               lea    310(%%a0), %%a0

               dbra   %%d0, SpriteRestore4bpp_1_even
  
    SpriteRestore4bpp_1_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0/%%a0-%%a1": :);
#endif
  }
}

#endif
