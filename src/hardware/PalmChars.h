/******************************************************************************
 *
 * Copyright (c) 1994-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: PalmChars.h
 *
 * Release:
 *
 * Description:
 *      Header file for all Palm Devices
 *      Contains Palm-specific vchrs
 *
 * History:
 *
 *****************************************************************************/

#ifndef __PALMCHARS_H__
#define __PALMCHARS_H__

#include <Chars.h>

#undef vchrPalmMin
#undef vchrPalmMax
#define vchrPalmMin		0x0500
#define vchrPalmMax		0x05FF

#define PalmFtrCreator          'PALM'
#define PalmFtrHwrBitsGroup1    0
#define HwrBitsSlider           0x0001
#define HwrBitsVoiceButton      0x0002
#define HwrBitsBuiltInKeyboard  0x0004
#define vchrHard5		0x0214
#define keyBitHard5		0x2000

#define vchrSilkClock           (vchrPalmMin + 0)
#define vchrClock               (vchrPalmMin + 1)
#define vchrPopupBrightness     (vchrPalmMin + 2)
#define vchrNavChange           (vchrPalmMin + 3)
#define vchrNavReserved0        (vchrPalmMin + 4)
#define vchrNavReserved1        (vchrPalmMin + 5)
#define vchrNavReserved2        (vchrPalmMin + 6)
#define vchrNavReserved3        (vchrPalmMin + 7)

#define keyBitNavLeft           0x01000000
#define keyBitNavRight          0x02000000
#define keyBitNavSelect         0x04000000
#define keyBitNavLRS            0x07000000

#define navBitUp                0x0001
#define navBitDown              0x0002
#define navBitLeft              0x0004
#define navBitRight             0x0008
#define navBitSelect            0x0010
#define navBitsAll              0x001F

#define navChangeUp             0x0100
#define navChangeDown           0x0200
#define navChangeLeft           0x0400
#define navChangeRight          0x0800
#define navChangeSelect         0x1000
#define navChangeBitsAll        0x1F00

#define navFtrCreator           'fway'
#define navFtrVersion           0

#define navVersion              0x00010000

#define IsFiveWayNavEvent(eventP) \
( \
  (((eventP)->data.keyDown.chr == vchrPageUp)   || \
   ((eventP)->data.keyDown.chr == vchrPageDown) || \
   ((eventP)->data.keyDown.chr == vchrNavChange))  \
  && \
  (((eventP)->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll)) != 0) \
)

#define NavSelectPressed(eventP) \
( \
  IsFiveWayNavEvent(eventP) && \
  (((eventP)->data.keyDown.modifiers & autoRepeatKeyMask) == 0) && \
  (((eventP)->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll)) == navChangeSelect) \
)

#define NavDirectionPressed(eventP, nav) \
( \
  IsFiveWayNavEvent(eventP) \
    ? (((eventP)->data.keyDown.modifiers & autoRepeatKeyMask) \
         ? (((eventP)->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll)) == (navBit ## nav)) \
         : (((eventP)->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll)) == (navBit ## nav | navChange ## nav))) \
    : (((eventP)->data.keyDown.chr == vchrPageUp && navBit ## nav == navBitUp) || \
       ((eventP)->data.keyDown.chr == vchrPageDown && navBit ## nav == navBitDown)) \
)

#define NavKeyPressed(eventP, nav) \
( \
  (navBit ## nav == navBitSelect) \
    ? NavSelectPressed(eventP) \
    : NavDirectionPressed(eventP, nav) \
)

#define densityFtrCreator       'dnsT'
#define densityFtrVersion       0

// ADDED WITH PALMOS 5.0 DR2
#define vchrThumbWheelUp        0x012E  // optional thumb-wheel up
#define vchrThumbWheelDown      0x012F  // optional thumb-wheel down
#define vchrThumbWheelPush      0x0130  // optional thumb-wheel press/center
#define vchrThumbWheelBack      0x0131  // optional thumb-wheel cluster back

#define vchrRockerUp            0x0132  // 5-way rocker up
#define vchrRockerDown          0x0133  // 5-way rocker down
#define vchrRockerLeft          0x0134  // 5-way rocker left
#define vchrRockerRight         0x0135  // 5-way rocker right
#define vchrRockerCenter        0x0136  // 5-way rocker center/press

#define keyBitRockerUp          0x00010000
#define keyBitRockerDown        0x00020000
#define keyBitRockerLeft        0x00040000
#define keyBitRockerRight       0x00080000
#define keyBitRockerSelect      0x00100000

#endif
