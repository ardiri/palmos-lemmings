/*
 * @(#)game_msk.inc
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
// custom written Win* API routines
//

#if !USE_PALMOS_WINAPI
/**
 * Draw a mask to the offscreen 2bpp window - this means "winMask" = erase 
 * 
 * @param maskBuf a pointer to a 16x16 sprite buffer
 * @param x       the x co-ordinate in the offscreen window.
 * @param y       the y co-ordinate in the offscreen window.
 */
static void    
GameDrawMask2bpp(UInt8 *maskBuf, Int16 x, Int16 y)
{
  UInt8  *ptrScreen, *ptrMask;
#if PORTABLE
  UInt8  mask, scrn;
  UInt16 i, j;
#endif

  // get pointers to everything
  ptrScreen  = globals.ptrWinDraw;
  ptrMask    = maskBuf;

//ptrScreen += ((y + SPR_HEIGHT) * 160) + (x / 4);
  ptrScreen += (((y + SPR_HEIGHT) << 5) * 5) + (x >> 2);

  switch (x % 4)
  {
    // aligned - simple mask
    case 0:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do {
             scrn = *ptrScreen;
             *ptrScreen++ = (scrn & ~*(ptrMask++));
           } while (--i);

           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d2/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMask2bpp_0_even
               move.l  #255,     %%d2           | 0x000000ff
  
    SpriteDrawMask2bpp_0_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #8,       %%d1
               not.l   %%d1
               and.b   %%d1,     (%%a0)+
               or.b    %%d2,     %%d1
               and.l   %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask2bpp_0_odd
               bra.s   SpriteDrawMask2bpp_0_exit
  
    SpriteDrawMask2bpp_0_even:

               move.l  (%%a1)+, %%d1
               not.l   %%d1
               and.l   %%d1,    (%%a0)+

               lea     156(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask2bpp_0_even
  
    SpriteDrawMask2bpp_0_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d2/%%a0-%%a1": :);
#endif

         break;

    // mis aligned by 2 bits - need to shift, mask+overlay and mess around
    case 1:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);

             scrn = *ptrScreen;
             scrn = (scrn & ~(mask >> 2));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask << 6));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMask2bpp_1_even
               move.l  #192,     %%d2           | 0x000000c0
               moveq   #63,      %%d3           | 0x0000003f
  
    SpriteDrawMask2bpp_1_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #6,       %%d1
               not.l   %%d1
               move.l  %%d1,     %%d4
               or.b    %%d2,     %%d4
               and.b   %%d4,     (%%a0)+
               or.b    %%d3,     %%d1
               and.l   %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask2bpp_1_odd
               bra.s   SpriteDrawMask2bpp_1_exit
  
    SpriteDrawMask2bpp_1_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #2,       %%d1
               not.l   %%d1
               and.l   %%d1,     (%%a0)+
               lsl.b   #6,       %%d2
               not.b   %%d2
               and.b   %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask2bpp_1_even
  
    SpriteDrawMask2bpp_1_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    // mis aligned by 4 bits - need to shift, mask+overlay and mess around
    case 2:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask >> 4));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask << 4));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMask2bpp_2_even
               move.l  #240,     %%d2           | 0x000000f0
               moveq   #15,      %%d3           | 0x0000000f
  
    SpriteDrawMask2bpp_2_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #4,       %%d1
               not.l   %%d1
               move.l  %%d1,     %%d4
               or.b    %%d2,     %%d4
               and.b   %%d4,     (%%a0)+
               or.b    %%d3,     %%d1
               and.l   %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask2bpp_2_odd
               bra.s   SpriteDrawMask2bpp_2_exit
  
    SpriteDrawMask2bpp_2_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #4,       %%d1
               not.l   %%d1
               and.l   %%d1,     (%%a0)+
               lsl.b   #4,       %%d2
               not.b   %%d2
               and.b   %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask2bpp_2_even
  
    SpriteDrawMask2bpp_2_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    // mis aligned by 6 bits - need to shift, mask+overlay and mess around
    case 3:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask >> 6));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask << 2));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMask2bpp_3_even
               move.l  #252,     %%d2           | 0x000000fc
               moveq   #3,       %%d3           | 0x00000003
  
    SpriteDrawMask2bpp_3_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #2,       %%d1
               not.l   %%d1
               move.l  %%d1,     %%d4
               or.b    %%d2,     %%d4
               and.b   %%d4,     (%%a0)+
               or.b    %%d3,     %%d1
               and.l   %%d1,     (%%a0)+

               lea     155(%%a0), %%a0
  
               dbra    %%d0, SpriteDrawMask2bpp_3_odd
               bra.s   SpriteDrawMask2bpp_3_exit
  
    SpriteDrawMask2bpp_3_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #6,       %%d1
               not.l   %%d1
               and.l   %%d1,     (%%a0)+
               add.b   %%d2,     %%d2
               add.b   %%d2,     %%d2
               not.b   %%d2
               and.b   %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask2bpp_3_even
  
    SpriteDrawMask2bpp_3_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    default:
         break;
  }
}

/**
 * Draw a mask to the offscreen 4bpc window - this means "winOverlay" = overlay 
 * 
 * @param maskBuf a pointer to a 16x16 sprite buffer
 * @param x       the x co-ordinate in the offscreen window.
 * @param y       the y co-ordinate in the offscreen window.
 */
static void    
GameDrawMask4bpc(UInt8 *maskBuf, Int16 x, Int16 y)
{
  UInt8  *ptrScreen, *ptrMask;
#if PORTABLE
  UInt8  mask, scrn;
  UInt16 i, j;
#endif

  // get pointers to everything
  ptrScreen  = globals.ptrWinDraw;
  ptrMask    = maskBuf;

//ptrScreen += ((y + SPR_HEIGHT) * 320) + (x / 2);
  ptrScreen += (UInt32)(((UInt16)(y + SPR_HEIGHT) << 6) * 5) + (x >> 1);

  // aligned - simple mask+overlay
  if ((x % 2) == 0) 
  {
#if PORTABLE
    j = SPR_HEIGHT;
    do
    {
      i = (SPR_WIDTH >> 1);
      do {
        scrn = *ptrScreen;
        *ptrScreen++ = (scrn | *(ptrMask++));
      } while (--i);

      ptrScreen += 320 - (SPR_WIDTH >> 1);
    }
    while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d2/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMask4bpc_0_even
  
    SpriteDrawMask4bpc_0_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #8,       %%d1
               or.b    %%d1,     (%%a0)+
               move.l  (%%a1)+,  %%d2
               rol.l   #8,       %%d2
               move.b  %%d2,     %%d1
               or.l    %%d1,     (%%a0)+
               clr.b   %%d2
               or.l    %%d2,     (%%a0)+

               lea     311(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask4bpc_0_odd
               bra.s   SpriteDrawMask4bpc_0_exit
  
    SpriteDrawMask4bpc_0_even:

               move.l  (%%a1)+, %%d1
               or.l    %%d1,    (%%a0)+
               move.l  (%%a1)+, %%d1
               or.l    %%d1,    (%%a0)+

               lea     312(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask4bpc_0_even
  
    SpriteDrawMask4bpc_0_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d2/%%a0-%%a1": :);
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
      do 
      {
        mask = *(ptrMask++);

        scrn = *ptrScreen;
        scrn = (scrn | (mask >> 4));
        *ptrScreen++ = scrn;

        scrn = *ptrScreen;
        scrn = (scrn | (mask << 4));
        *ptrScreen = scrn;
      }
      while (--i);

      ptrScreen += 320 - (SPR_WIDTH >> 1);
    }
    while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d5/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMask4bpc_1_even_start
               move.l  #240,     %%d2           | 0x000000f0
               moveq   #15,      %%d3           | 0x0000000f
  
    SpriteDrawMask4bpc_1_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #4,       %%d1
               move.l  %%d1,     %%d4
               and.b   %%d3,     %%d4
               or.b    %%d4,     (%%a0)+
               move.l  (%%a1)+,  %%d5
               rol.l   #4,       %%d5
               move.l  %%d5,     %%d4
               and.b   %%d2,     %%d1
               and.b   %%d3,     %%d4
               or.b    %%d4,     %%d1
               or.l    %%d1,     (%%a0)+
               and.b   %%d2,     %%d5
               or.l    %%d5,     (%%a0)+

               lea     311(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask4bpc_1_odd
               bra.s   SpriteDrawMask4bpc_1_exit

    SpriteDrawMask4bpc_1_even_start:

               move.l  #4026531840, %%d2        | 0xf0000000
               move.l  #268435455, %%d3         | 0x0fffffff

    SpriteDrawMask4bpc_1_even:

               move.l  (%%a1)+,  %%d1
               ror.l   #4,       %%d1
               move.l  %%d1,     %%d4
               and.l   %%d3,     %%d4
               or.l    %%d4,     (%%a0)+
               move.l  (%%a1)+,  %%d5
               move.l  %%d5,     %%d4
               ror.l   #4,       %%d5
               and.l   %%d2,     %%d1
               and.l   %%d3,     %%d5
               or.l    %%d5,     %%d1
               or.l    %%d1,     (%%a0)+
               lsl.b   #4,       %%d4
               or.b    %%d4,     (%%a0)+

               lea     311(%%a0), %%a0

               dbra    %%d0, SpriteDrawMask4bpc_1_even
  
    SpriteDrawMask4bpc_1_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d5/%%a0-%%a1": :);
#endif
  }
}

#endif

/**
 * Mask a mask from the offscreen mask window - this means erase 
 * 
 * @param maskBuf a pointer to a 16x16 sprite buffer
 * @param x       the x co-ordinate in the offscreen window.
 * @param y       the y co-ordinate in the offscreen window.
 */
static void    
GameDrawMaskMask(UInt8 *maskBuf, Int16 x, Int16 y)
{
  UInt8  *ptrScreen, *ptrMask;
#if PORTABLE
  UInt8  mask, scrn;
  UInt16 i, j;
#endif

  // get pointers to everything
  ptrScreen  = globals.levelMask;
  ptrMask    = maskBuf;

//ptrScreen += ((y + SPR_HEIGHT) * 160) + (x / 4);
  ptrScreen += (((y + SPR_HEIGHT) << 5) * 5) + (x >> 2);

  switch (x % 4)
  {
    // aligned - simple mask
    case 0:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do {
             scrn = *ptrScreen;
             *ptrScreen++ = (scrn & ~*(ptrMask++));
           } while (--i);

           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d2/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMaskMask_0_even
               move.l  #255,     %%d2           | 0x000000ff
  
    SpriteDrawMaskMask_0_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #8,       %%d1
               not.l   %%d1
               and.b   %%d1,     (%%a0)+
               or.b    %%d2,     %%d1
               and.l   %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMaskMask_0_odd
               bra.s   SpriteDrawMaskMask_0_exit
  
    SpriteDrawMaskMask_0_even:

               move.l  (%%a1)+, %%d1
               not.l   %%d1
               and.l   %%d1,    (%%a0)+

               lea     156(%%a0), %%a0

               dbra    %%d0, SpriteDrawMaskMask_0_even
  
    SpriteDrawMaskMask_0_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d2/%%a0-%%a1": :);
#endif

         break;

    // mis aligned by 2 bits - need to shift, mask+overlay and mess around
    case 1:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);

             scrn = *ptrScreen;
             scrn = (scrn & ~(mask >> 2));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask << 6));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMaskMask_1_even
               move.l  #192,     %%d2           | 0x000000c0
               moveq   #63,      %%d3           | 0x0000003f
  
    SpriteDrawMaskMask_1_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #6,       %%d1
               not.l   %%d1
               move.l  %%d1,     %%d4
               or.b    %%d2,     %%d4
               and.b   %%d4,     (%%a0)+
               or.b    %%d3,     %%d1
               and.l   %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMaskMask_1_odd
               bra.s   SpriteDrawMaskMask_1_exit
  
    SpriteDrawMaskMask_1_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #2,       %%d1
               not.l   %%d1
               and.l   %%d1,     (%%a0)+
               lsl.b   #6,       %%d2
               not.b   %%d2
               and.b   %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMaskMask_1_even
  
    SpriteDrawMaskMask_1_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    // mis aligned by 4 bits - need to shift, mask+overlay and mess around
    case 2:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask >> 4));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask << 4));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMaskMask_2_even
               move.l  #240,     %%d2           | 0x000000f0
               moveq   #15,      %%d3           | 0x0000000f
  
    SpriteDrawMaskMask_2_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #4,       %%d1
               not.l   %%d1
               move.l  %%d1,     %%d4
               or.b    %%d2,     %%d4
               and.b   %%d4,     (%%a0)+
               or.b    %%d3,     %%d1
               and.l   %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMaskMask_2_odd
               bra.s   SpriteDrawMaskMask_2_exit
  
    SpriteDrawMaskMask_2_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #4,       %%d1
               not.l   %%d1
               and.l   %%d1,     (%%a0)+
               lsl.b   #4,       %%d2
               not.b   %%d2
               and.b   %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMaskMask_2_even
  
    SpriteDrawMaskMask_2_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    // mis aligned by 6 bits - need to shift, mask+overlay and mess around
    case 3:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask >> 6));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn & ~(mask << 2));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawMaskMask_3_even
               move.l  #252,     %%d2           | 0x000000fc
               moveq   #3,       %%d3           | 0x00000003
  
    SpriteDrawMaskMask_3_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #2,       %%d1
               not.l   %%d1
               move.l  %%d1,     %%d4
               or.b    %%d2,     %%d4
               and.b   %%d4,     (%%a0)+
               or.b    %%d3,     %%d1
               and.l   %%d1,     (%%a0)+

               lea     155(%%a0), %%a0
  
               dbra    %%d0, SpriteDrawMaskMask_3_odd
               bra.s   SpriteDrawMaskMask_3_exit
  
    SpriteDrawMaskMask_3_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #6,       %%d1
               not.l   %%d1
               and.l   %%d1,     (%%a0)+
               add.b   %%d2,     %%d2
               add.b   %%d2,     %%d2
               not.b   %%d2
               and.b   %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawMaskMask_3_even
  
    SpriteDrawMaskMask_3_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    default:
         break;
  }
}

/**
 * Draw a mask chunk to the offscreen mask window - this means draw 
 *
 * @param maskBuf a pointer to a 16x16 sprite buffer
 * @param x           the x co-ordinate in the offscreen window.
 * @param y           the y co-ordinate in the offscreen window.
 */
static void    
GameDrawDrawMask(UInt8 *maskBuf, Int16 x, Int16 y)
{
  UInt8  *ptrScreen, *ptrMask;
#if PORTABLE
  UInt8  mask, scrn;
  UInt16 i, j;
#endif

  // get pointers to everything
  ptrScreen  = globals.levelMask;
  ptrMask    = maskBuf;

//ptrScreen += ((y + SPR_HEIGHT) * 160) + (x / 4);
  ptrScreen += (((y + SPR_HEIGHT) << 5) * 5) + (x >> 2);

  switch (x % 4)
  {
    // aligned - simple mask
    case 0:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do {
             scrn = *ptrScreen;
             *ptrScreen++ = (scrn | *(ptrMask++));
           } while (--i);

           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d1/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawDrawMask_0_even
  
    SpriteDrawDrawMask_0_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #8,       %%d1
               or.b    %%d1,     (%%a0)+
               clr.b   %%d1
               or.l    %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawDrawMask_0_odd
               bra.s   SpriteDrawDrawMask_0_exit
  
    SpriteDrawDrawMask_0_even:

               move.l  (%%a1)+,  %%d1
               or.l    %%d1,    (%%a0)+

               lea     156(%%a0), %%a0

               dbra    %%d0, SpriteDrawDrawMask_0_even
  
    SpriteDrawDrawMask_0_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d1/%%a0-%%a1": :);
#endif

         break;

    // mis aligned by 2 bits - need to shift, mask+overlay and mess around
    case 1:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);

             scrn = *ptrScreen;
             scrn = (scrn | (mask >> 2));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn | (mask << 6));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawDrawMask_1_even
               move.l  #192,     %%d2           | 0x000000c0
               moveq   #63,      %%d3           | 0x0000003f
  
    SpriteDrawDrawMask_1_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #6,       %%d1
               move.l  %%d1,     %%d4
               and.b   %%d3,     %%d4
               or.b    %%d4,     (%%a0)+
               and.b   %%d2,     %%d1
               or.l    %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawDrawMask_1_odd
               bra.s   SpriteDrawDrawMask_1_exit
  
    SpriteDrawDrawMask_1_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #2,       %%d1
               or.l    %%d1,     (%%a0)+
               lsl.b   #6,       %%d2
               or.b    %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawDrawMask_1_even
  
    SpriteDrawDrawMask_1_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    // mis aligned by 4 bits - need to shift, mask+overlay and mess around
    case 2:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);
     
             scrn = *ptrScreen;
             scrn = (scrn | (mask >> 4));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn | (mask << 4));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawDrawMask_2_even
               move.l  #240,     %%d2           | 0x000000f0
               moveq   #15,      %%d3           | 0x0000000f
  
    SpriteDrawDrawMask_2_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #4,       %%d1
               move.l  %%d1,     %%d4
               and.b   %%d3,     %%d4
               or.b    %%d4,     (%%a0)+
               and.b   %%d2,     %%d1
               or.l    %%d1,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawDrawMask_2_odd
               bra.s   SpriteDrawDrawMask_2_exit
  
    SpriteDrawDrawMask_2_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #4,       %%d1
               or.l    %%d1,     (%%a0)+
               lsl.b   #4,       %%d2
               or.b    %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawDrawMask_2_even
  
    SpriteDrawDrawMask_2_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    // mis aligned by 6 bits - need to shift, mask+overlay and mess around
    case 3:

#if PORTABLE
         j = SPR_HEIGHT;
         do
         {
           i = (SPR_WIDTH >> 2);
           do 
           {
             mask = *(ptrMask++);
     
             scrn = *ptrScreen;
             scrn = (scrn | (mask >> 6));
             *ptrScreen++ = scrn;
     
             scrn = *ptrScreen;
             scrn = (scrn | (mask << 2));
             *ptrScreen = scrn;
           }
           while (--i);
     
           ptrScreen += 160 - (SPR_WIDTH >> 2);
         }
         while (--j);
#else
    // push all registers on stack
    asm("      movem.l %%d0-%%d4/%%a0-%%a1, -(%%sp)": :);

    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrMask));

    asm("      moveq   #15,      %%d0
               move.l  %%a0,     %%d1
               btst.b  #0,       %%d1
               beq.s   SpriteDrawDrawMask_3_even
               move.l  #252,     %%d2           | 0x000000fc
               moveq   #3,       %%d3           | 0x00000003
  
    SpriteDrawDrawMask_3_odd:

               move.l  (%%a1)+,  %%d1
               rol.l   #2,       %%d1
               move.l  %%d1,     %%d4
               and.b   %%d3,     %%d4
               or.b    %%d4,     (%%a0)+
               and.b   %%d2,     %%d1
               or.l    %%d1,     (%%a0)+

               lea     155(%%a0), %%a0
  
               dbra    %%d0, SpriteDrawDrawMask_3_odd
               bra.s   SpriteDrawDrawMask_3_exit
  
    SpriteDrawDrawMask_3_even:

               move.l  (%%a1)+,  %%d1
               move.l  %%d1,     %%d2
               lsr.l   #6,       %%d1
               or.l    %%d1,     (%%a0)+
               add.b   %%d2,     %%d2
               add.b   %%d2,     %%d2
               or.b    %%d2,     (%%a0)+

               lea     155(%%a0), %%a0

               dbra    %%d0, SpriteDrawDrawMask_3_even
  
    SpriteDrawDrawMask_3_exit:
    ": :);
  
    // pop all registers off stack
    asm("      movem.l (%%sp)+, %%d0-%%d4/%%a0-%%a1": :);
#endif
 
         break;

    default:
         break;
  }
}

//
// utility masking API routines
//

static UInt8 MASK_FLAG[512] =
{
/*
 * table: identifies area being changed (only)
 *
 *  if (mask == 00)                              result = 00;
 *  if ((mask == 11) && (rule == 11))            result = 11;
 *  if ((mask == 11) && (rule == 10))            result = 00;
 *  if ((mask == 11) && (rule == 01) && special) result = 11; else result = 00;
 *
 *  -- this is not exactly what we want, in order to "add" data to the 
 *     mask, we must also have the following rule (ie: builder, blocker)
 *
 *  if ((mask == 11) && (rule == 00)             result = 11;
 *
 * -- Aaron Ardiri, 2001
 *
  // normal
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x30, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x33,  
  0x30, 0x30, 0x30, 0x33, 0x3c, 0x3c, 0x3c, 0x3f,
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x30, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x33, 
  0x30, 0x30, 0x30, 0x33, 0x3c, 0x3c, 0x3c, 0x3f,
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x03, 0x0c, 0x0c, 0x0c, 0x0f,
  0x30, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x33, 
  0x30, 0x30, 0x30, 0x33, 0x3c, 0x3c, 0x3c, 0x3f,
  0xc0, 0xc0, 0xc0, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 
  0xc0, 0xc0, 0xc0, 0xc3, 0xcc, 0xcc, 0xcc, 0xcf,
  0xc0, 0xc0, 0xc0, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 
  0xc0, 0xc0, 0xc0, 0xc3, 0xcc, 0xcc, 0xcc, 0xcf,
  0xc0, 0xc0, 0xc0, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 
  0xc0, 0xc0, 0xc0, 0xc3, 0xcc, 0xcc, 0xcc, 0xcf,
  0xf0, 0xf0, 0xf0, 0xf3, 0xf0, 0xf0, 0xf0, 0xf3, 
  0xf0, 0xf0, 0xf0, 0xf3, 0xfc, 0xfc, 0xfc, 0xff,

  // special
  0x00, 0x03, 0x00, 0x03, 0x0c, 0x0f, 0x0c, 0x0f, 
  0x00, 0x03, 0x00, 0x03, 0x0c, 0x0f, 0x0c, 0x0f,
  0x30, 0x33, 0x30, 0x33, 0x3c, 0x3f, 0x3c, 0x3f, 
  0x30, 0x33, 0x30, 0x33, 0x3c, 0x3f, 0x3c, 0x3f,
  0x00, 0x03, 0x00, 0x03, 0x0c, 0x0f, 0x0c, 0x0f, 
  0x00, 0x03, 0x00, 0x03, 0x0c, 0x0f, 0x0c, 0x0f,
  0x30, 0x33, 0x30, 0x33, 0x3c, 0x3f, 0x3c, 0x3f, 
  0x30, 0x33, 0x30, 0x33, 0x3c, 0x3f, 0x3c, 0x3f,
  0xc0, 0xc3, 0xc0, 0xc3, 0xcc, 0xcf, 0xcc, 0xcf,
  0xc0, 0xc3, 0xc0, 0xc3, 0xcc, 0xcf, 0xcc, 0xcf,
  0xf0, 0xf3, 0xf0, 0xf3, 0xfc, 0xff, 0xfc, 0xff, 
  0xf0, 0xf3, 0xf0, 0xf3, 0xfc, 0xff, 0xfc, 0xff,
  0xc0, 0xc3, 0xc0, 0xc3, 0xcc, 0xcf, 0xcc, 0xcf, 
  0xc0, 0xc3, 0xc0, 0xc3, 0xcc, 0xcf, 0xcc, 0xcf,
  0xf0, 0xf3, 0xf0, 0xf3, 0xfc, 0xff, 0xfc, 0xff, 
  0xf0, 0xf3, 0xf0, 0xf3, 0xfc, 0xff, 0xfc, 0xff,
  0x00, 0x03, 0x00, 0x03, 0x0c, 0x0f, 0x0c, 0x0f, 
  0x00, 0x03, 0x00, 0x03, 0x0c, 0x0f, 0x0c, 0x0f,
  0x30, 0x33, 0x30, 0x33, 0x3c, 0x3f, 0x3c, 0x3f, 
  0x30, 0x33, 0x30, 0x33, 0x3c, 0x3f, 0x3c, 0x3f,
  0x00, 0x03, 0x00, 0x03, 0x0c, 0x0f, 0x0c, 0x0f, 
  0x00, 0x03, 0x00, 0x03, 0x0c, 0x0f, 0x0c, 0x0f,
  0x30, 0x33, 0x30, 0x33, 0x3c, 0x3f, 0x3c, 0x3f, 
  0x30, 0x33, 0x30, 0x33, 0x3c, 0x3f, 0x3c, 0x3f,
  0xc0, 0xc3, 0xc0, 0xc3, 0xcc, 0xcf, 0xcc, 0xcf, 
  0xc0, 0xc3, 0xc0, 0xc3, 0xcc, 0xcf, 0xcc, 0xcf,
  0xf0, 0xf3, 0xf0, 0xf3, 0xfc, 0xff, 0xfc, 0xff, 
  0xf0, 0xf3, 0xf0, 0xf3, 0xfc, 0xff, 0xfc, 0xff,
  0xc0, 0xc3, 0xc0, 0xc3, 0xcc, 0xcf, 0xcc, 0xcf, 
  0xc0, 0xc3, 0xc0, 0xc3, 0xcc, 0xcf, 0xcc, 0xcf,
  0xf0, 0xf3, 0xf0, 0xf3, 0xfc, 0xff, 0xfc, 0xff, 
  0xf0, 0xf3, 0xf0, 0xf3, 0xfc, 0xff, 0xfc, 0xff
 */

  // normal
  0xff, 0xfc, 0xfc, 0xff, 0xf3, 0xf0, 0xf0, 0xf3, 
  0xf3, 0xf0, 0xf0, 0xf3, 0xff, 0xfc, 0xfc, 0xff,
  0xcf, 0xcc, 0xcc, 0xcf, 0xc3, 0xc0, 0xc0, 0xc3, 
  0xc3, 0xc0, 0xc0, 0xc3, 0xcf, 0xcc, 0xcc, 0xcf,
  0xcf, 0xcc, 0xcc, 0xcf, 0xc3, 0xc0, 0xc0, 0xc3, 
  0xc3, 0xc0, 0xc0, 0xc3, 0xcf, 0xcc, 0xcc, 0xcf,
  0xff, 0xfc, 0xfc, 0xff, 0xf3, 0xf0, 0xf0, 0xf3, 
  0xf3, 0xf0, 0xf0, 0xf3, 0xff, 0xfc, 0xfc, 0xff,
  0x3f, 0x3c, 0x3c, 0x3f, 0x33, 0x30, 0x30, 0x33, 
  0x33, 0x30, 0x30, 0x33, 0x3f, 0x3c, 0x3c, 0x3f,
  0x0f, 0x0c, 0x0c, 0x0f, 0x03, 0x00, 0x00, 0x03, 
  0x03, 0x00, 0x00, 0x03, 0x0f, 0x0c, 0x0c, 0x0f,
  0x0f, 0x0c, 0x0c, 0x0f, 0x03, 0x00, 0x00, 0x03, 
  0x03, 0x00, 0x00, 0x03, 0x0f, 0x0c, 0x0c, 0x0f,
  0x3f, 0x3c, 0x3c, 0x3f, 0x33, 0x30, 0x30, 0x33, 
  0x33, 0x30, 0x30, 0x33, 0x3f, 0x3c, 0x3c, 0x3f,
  0x3f, 0x3c, 0x3c, 0x3f, 0x33, 0x30, 0x30, 0x33, 
  0x33, 0x30, 0x30, 0x33, 0x3f, 0x3c, 0x3c, 0x3f,
  0x0f, 0x0c, 0x0c, 0x0f, 0x03, 0x00, 0x00, 0x03, 
  0x03, 0x00, 0x00, 0x03, 0x0f, 0x0c, 0x0c, 0x0f,
  0x0f, 0x0c, 0x0c, 0x0f, 0x03, 0x00, 0x00, 0x03,
  0x03, 0x00, 0x00, 0x03, 0x0f, 0x0c, 0x0c, 0x0f,
  0x3f, 0x3c, 0x3c, 0x3f, 0x33, 0x30, 0x30, 0x33,
  0x33, 0x30, 0x30, 0x33, 0x3f, 0x3c, 0x3c, 0x3f,
  0xff, 0xfc, 0xfc, 0xff, 0xf3, 0xf0, 0xf0, 0xf3,
  0xf3, 0xf0, 0xf0, 0xf3, 0xff, 0xfc, 0xfc, 0xff,
  0xcf, 0xcc, 0xcc, 0xcf, 0xc3, 0xc0, 0xc0, 0xc3,
  0xc3, 0xc0, 0xc0, 0xc3, 0xcf, 0xcc, 0xcc, 0xcf,
  0xcf, 0xcc, 0xcc, 0xcf, 0xc3, 0xc0, 0xc0, 0xc3,
  0xc3, 0xc0, 0xc0, 0xc3, 0xcf, 0xcc, 0xcc, 0xcf,
  0xff, 0xfc, 0xfc, 0xff, 0xf3, 0xf0, 0xf0, 0xf3,
  0xf3, 0xf0, 0xf0, 0xf3, 0xff, 0xfc, 0xfc, 0xff,

  // special
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff, 
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff,
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff,
  0xcf, 0xcf, 0xcc, 0xcf, 0xcf, 0xcf, 0xcc, 0xcf,
  0xc3, 0xc3, 0xc0, 0xc3, 0xcf, 0xcf, 0xcc, 0xcf,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff,
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff,
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff,
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff,
  0xcf, 0xcf, 0xcc, 0xcf, 0xcf, 0xcf, 0xcc, 0xcf,
  0xc3, 0xc3, 0xc0, 0xc3, 0xcf, 0xcf, 0xcc, 0xcf,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff,
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff,
  0x3f, 0x3f, 0x3c, 0x3f, 0x3f, 0x3f, 0x3c, 0x3f,
  0x33, 0x33, 0x30, 0x33, 0x3f, 0x3f, 0x3c, 0x3f,
  0x3f, 0x3f, 0x3c, 0x3f, 0x3f, 0x3f, 0x3c, 0x3f,
  0x33, 0x33, 0x30, 0x33, 0x3f, 0x3f, 0x3c, 0x3f,
  0x0f, 0x0f, 0x0c, 0x0f, 0x0f, 0x0f, 0x0c, 0x0f, 
  0x03, 0x03, 0x00, 0x03, 0x0f, 0x0f, 0x0c, 0x0f,
  0x3f, 0x3f, 0x3c, 0x3f, 0x3f, 0x3f, 0x3c, 0x3f, 
  0x33, 0x33, 0x30, 0x33, 0x3f, 0x3f, 0x3c, 0x3f,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff, 
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff, 
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff,
  0xcf, 0xcf, 0xcc, 0xcf, 0xcf, 0xcf, 0xcc, 0xcf, 
  0xc3, 0xc3, 0xc0, 0xc3, 0xcf, 0xcf, 0xcc, 0xcf,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xff, 
  0xf3, 0xf3, 0xf0, 0xf3, 0xff, 0xff, 0xfc, 0xff
};

static UInt16 MASK_2to4bpp[256] =
{
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0f00, 0x0f00, 0x0f00, 0x0f0f, 0x0f00, 0x0f00, 0x0f00, 0x0f00, 
  0x0f00, 0x0f00, 0x0f00, 0x0f00, 0x0ff0, 0x0f00, 0x0f00, 0x0fff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0xf000, 0xf000, 0xf000, 0xf00f, 0xf000, 0xf000, 0xf000, 0xf000, 
  0xf000, 0xf000, 0xf000, 0xf000, 0xf0f0, 0xf000, 0xf000, 0xf0ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x00f0, 0x0000, 0x0000, 0x00ff,
  0xff00, 0xff00, 0xff00, 0xff0f, 0xff00, 0xff00, 0xff00, 0xff00, 
  0xff00, 0xff00, 0xff00, 0xff00, 0xfff0, 0xff00, 0xff00, 0xffff
};

/**
 * Generate the mask bitmaps to update the screen/mask background
 * so that the rules of "digging" removal are followed as they
 * should be :) (sand, rock, special, air). as per 2bpp display.
 *
 * @param spriteType  the sprite to deal with
 * @param sprite_index the sprite index 
 * @param x           the x co-ordinate in the offscreen window.
 * @param y           the y co-ordinate in the offscreen window.
 * @param maskMaskPtr a pointer to the 16x16 bitmap for 2bpp mask
 * @param dispMaskPtr a pointer to the 16x16 bitmap for 2bpc/4bpc display mask
 * @param special     should 01 be converted to 11 or 00?
 */
static void 
GameGenerateMask2bpp(UInt16 spriteType, UInt16 sprite_index, Int16 x, Int16 y, 
                     UInt8 *maskMaskPtr, UInt8 *dispMaskPtr, Boolean special)

{
  UInt8  *ptrMask, *ptrRule, *ptrTable;
  UInt8  scrn, rule;
  UInt16 j;
  UInt16 sx, sy;

  switch (spriteType)
  {
    case msk_undefined:
         sx = SPR_WIDTH  * (sprite_index & 0x07);
         sy = SPR_HEIGHT * (sprite_index >> 3);
         break;

    default:
         sx = 0;
         sy = 0;  // should never happen :)
         break;
  }

  // get pointers to everything
  ptrRule    = globals.levelMask;
  ptrMask    = globals.ptrGameMasks2bpp;

//ptrRule   += ((y + SPR_HEIGHT) * 160) + (x / 4);
//ptrMask   += (sy * 32) + (sx / 4);
  ptrRule   += (((y + SPR_HEIGHT) << 5) * 5) + (x >> 2);
  ptrMask   += (sy << 5) + (sx >> 2);

  ptrTable   = (special) ? (MASK_FLAG + 256) : MASK_FLAG;

  switch (x % 4)
  {
    // aligned - simple mask
    case 0:

         j = SPR_HEIGHT;
         do
         {
           scrn = ptrTable[*ptrRule++];

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = ptrTable[*ptrRule++];

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = ptrTable[*ptrRule++];

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = ptrTable[*ptrRule++];

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           ptrRule     += 160 - (SPR_WIDTH >> 2);
           ptrMask     += 32  - (SPR_WIDTH >> 2);
         }
         while (--j);
         break;

    // mis aligned by 2 bits - need to shift, mask and mess around
    case 1:

         j = SPR_HEIGHT;
         do
         {
           rule = ptrTable[*ptrRule++];

           scrn = rule << 2;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 6);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 2;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 6);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 2;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 6);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 2;
           rule = ptrTable[*ptrRule];
           scrn |= (rule >> 6);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           ptrRule     += 160 - (SPR_WIDTH >> 2);
           ptrMask     += 32  - (SPR_WIDTH >> 2);
         }
         while (--j);
         break;

    // mis aligned by 4 bits - need to shift, mask and mess around
    case 2:

         j = SPR_HEIGHT;
         do
         {
           rule = ptrTable[*ptrRule++];

           scrn = rule << 4;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 4);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 4;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 4);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 4;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 4);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 4;
           rule = ptrTable[*ptrRule];
           scrn |= (rule >> 4);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           ptrRule     += 160 - (SPR_WIDTH >> 2);
           ptrMask     += 32  - (SPR_WIDTH >> 2);
         }
         while (--j);
         break;

    // mis aligned by 6 bits - need to shift, mask and mess around
    case 3:

         j = SPR_HEIGHT;
         do
         {
           rule = ptrTable[*ptrRule++];

           scrn = rule << 6;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 2);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 6;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 2);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 6;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 2);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           scrn = rule << 6;
           rule = ptrTable[*ptrRule];
           scrn |= (rule >> 2);

           *maskMaskPtr++ = *dispMaskPtr++ = *ptrMask++ & scrn;

           ptrRule     += 160 - (SPR_WIDTH >> 2);
           ptrMask     += 32  - (SPR_WIDTH >> 2);
         }
         while (--j);
         break;

    default:
         break;
  }
}

/**
 * Generate the mask bitmaps to update the screen/mask background
 * so that the rules of "digging" removal are followed as they
 * should be :) (sand, rock, special, air). as per 4bpp display.
 *
 * @param spriteType  the sprite to deal with
 * @param sprite_index the sprite index 
 * @param x           the x co-ordinate in the offscreen window.
 * @param y           the y co-ordinate in the offscreen window.
 * @param maskMaskPtr a pointer to the 16x16 bitmap for 2bpp mask
 * @param dispMaskPtr a pointer to the 16x16 bitmap for 4bpc display mask
 * @param special     should 01 be converted to 11 or 00?
 */
static void 
GameGenerateMask4bpc(UInt16 spriteType, UInt16 sprite_index, Int16 x, Int16 y, 
                     UInt8 *maskMaskPtr, UInt8 *dispMaskPtr, Boolean special)
{
  UInt8  *ptrMask, *ptrRule, *ptrTable;
  UInt8  scrn, rule;
  UInt16 j;
  UInt16 sx, sy;

  switch (spriteType)
  {
    case msk_undefined:
         sx = SPR_WIDTH  * (sprite_index & 0x07);
         sy = SPR_HEIGHT * (sprite_index >> 3);
         break;

    default:
         sx = 0;
         sy = 0;  // should never happen :)
         break;
  }

  // get pointers to everything
  ptrRule    = globals.levelMask;
  ptrMask    = globals.ptrGameMasks2bpp;

//ptrRule   += ((y + SPR_HEIGHT) * 160) + (x / 4);
//ptrMask   += (sy * 32) + (sx / 4);
  ptrRule   += (((y + SPR_HEIGHT) << 5) * 5) + (x >> 2);
  ptrMask   += (sy << 5) + (sx >> 2);

  ptrTable   = (special) ? (MASK_FLAG + 256) : MASK_FLAG;

  switch (x % 4)
  {
    // aligned - simple mask
    case 0:

         j = SPR_HEIGHT;
         do
         {
           scrn = ptrTable[*ptrRule++];
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = ptrTable[*ptrRule++];
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = ptrTable[*ptrRule++];
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = ptrTable[*ptrRule++];
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           ptrRule     += 160 - (SPR_WIDTH >> 2);
           ptrMask     += 32  - (SPR_WIDTH >> 2);
         }
         while (--j);
         break;

    // mis aligned by 2 bits - need to shift, mask and mess around
    case 1:

         j = SPR_HEIGHT;
         do
         {
           rule = ptrTable[*ptrRule++];

           scrn = rule << 2;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 6);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 2;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 6);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 2;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 6);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 2;
           rule = ptrTable[*ptrRule];
           scrn |= (rule >> 6);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           ptrRule     += 160 - (SPR_WIDTH >> 2);
           ptrMask     += 32  - (SPR_WIDTH >> 2);
         }
         while (--j);
         break;

    // mis aligned by 4 bits - need to shift, mask and mess around
    case 2:

         j = SPR_HEIGHT;
         do
         {
           rule = ptrTable[*ptrRule++];

           scrn = rule << 4;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 4);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 4;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 4);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 4;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 4);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 4;
           rule = ptrTable[*ptrRule];
           scrn |= (rule >> 4);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           ptrRule     += 160 - (SPR_WIDTH >> 2);
           ptrMask     += 32  - (SPR_WIDTH >> 2);
         }
         while (--j);
         break;

    // mis aligned by 6 bits - need to shift, mask and mess around
    case 3:

         j = SPR_HEIGHT;
         do
         {
           rule = ptrTable[*ptrRule++];

           scrn = rule << 6;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 2);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 6;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 2);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 6;
           rule = ptrTable[*ptrRule++];
           scrn |= (rule >> 2);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           scrn = rule << 6;
           rule = ptrTable[*ptrRule];
           scrn |= (rule >> 2);
           scrn &= *ptrMask++;

           *maskMaskPtr++ = scrn;
           *((UInt16 *)(dispMaskPtr))++ = MASK_2to4bpp[scrn];

           ptrRule     += 160 - (SPR_WIDTH >> 2);
           ptrMask     += 32  - (SPR_WIDTH >> 2);
         }
         while (--j);
         break;

    default:
         break;
  }
}
