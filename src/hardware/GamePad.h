/***********************************************************************
 * GamePadDriver (GPD)                                                 *
 *                                                                     *
 * Copyright © 2000, Worldwide WidgetWorks Inc. Austin, Texas, USA     *
 * All rights reserved.                                                *
 *                                                                     *
 * Special thanks to Jeff Ishaq for his excellent example of a shared  *
 * library implementation (currently on palm.com). ishaq@wwg.com       *
 *                                                                     *
 ***********************************************************************
 * MODIFIED: Aaron Ardiri, 27-Nov-2000                                 *
 *           - support for PRC-TOOLS development environment           *
 *           - adjusting datatypes for compliance to palmos 3.5 header *
 ***********************************************************************/

#ifndef GPDLIB_H
#define GPDLIB_H

// Use these for SysLibFind calls.  
#define GPD_LIB_NAME     "GamePadDriver"
#define GPD_LIB_CREATOR  (UInt32)'WwWA'        

// These are the additional errors that GPD might return:
#define GPDErrBase                  appErrorClass + 0x0800
#define GPDErrGamePadNotPresent     GPDErrBase + 0x01
#define GPDErrParam                 GPDErrBase + 0x02
#define GPDErrNoGlobals             GPDErrBase + 0x03

// GPD's trap identifiers.
#ifdef __GNUC__

#define GPDTrapGetVersion           sysLibTrapCustom        // PRC-TOOLS
#define GPDTrapRead                 sysLibTrapCustom+1
#define GPDTrapReadHeld             sysLibTrapCustom+2
#define GPDTrapStatus               sysLibTrapCustom+3
#define GPDTrapIsButtonPressed      sysLibTrapCustom+4
#define GPDTrapSetLegacySupport     sysLibTrapCustom+5
#define GPDTrapGetLegacySupport     sysLibTrapCustom+6
#define GPDTrapReadInstantKCSformat sysLibTrapCustom+7
#define GPDTrapReadHeldKCSformat    sysLibTrapCustom+8
#define GPDTrapSetLegacyKeyMap      sysLibTrapCustom+9
#define GPDTrapGetLegacyKeyMap      sysLibTrapCustom+10

#else

// GPD's trap identifiers.
typedef enum tagGPDTrapNumEnum                              // CodeWarrior
{
  GPDTrapGetVersion            = sysLibTrapCustom,
  GPDTrapRead,
  GPDTrapReadHeld,
  GPDTrapStatus,
  GPDTrapIsButtonPressed,
  GPDTrapSetLegacySupport,
  GPDTrapGetLegacySupport,
  GPDTrapReadInstantKCSformat,
  GPDTrapReadHeldKCSformat,
  GPDTrapSetLegacyKeyMap,
  GPDTrapGetLegacyKeyMap
} GPDTrapNumEnum;

#endif

// Bit Definitions for the result field's bits. 1's represent a pressed button.
#define GAMEPAD_LEFTFIRE      0x01
#define GAMEPAD_START         0x02
#define GAMEPAD_UP            0x04
#define GAMEPAD_LEFT          0x08
#define GAMEPAD_RIGHTFIRE     0x10
#define GAMEPAD_SELECT        0x20
#define GAMEPAD_DOWN          0x40
#define GAMEPAD_RIGHT         0x80

#ifdef __cplusplus
extern "C" {
#endif

//
// public functions 
//

/*
  Call GPDOpen function when starting your program. This function intializes 
  the library's global variables and re-maps IRQ1 for the gamepad. 
   
  Please use the Palm standard protocol when opening and closing the
  GamePadDriver shared library. Here is an example of how to locate and open 
  the driver library:
   
    GamePadDriverPresent = FALSE;
    err = SysLibFind(GPD_LIB_NAME, (UIntPtr)&uiRefNum);
    if (err == sysErrLibNotFound)
       err = SysLibLoad('libr',GPD_LIB_CREATOR, (UIntPtr)&uiRefNum); 
   
    if (!err)
      GamePadDriverPresent = TRUE;

  End of example. Note that if the GamePadDriver is not found, there must not 
  be a gamepad present. (Bet you could have figure that out on your own? :)
 */
extern Err  GPDOpen(UInt16 uRefNum)
  SYS_TRAP(sysLibTrapOpen);

/*
  Call GPDClose function when shutting down your program. This function
  restores the state of the Palm's processor registers. Once again, please 
  use Palm's standard protocol when closing / unloading the driver library. 
  An example:
  
    err = GPDClose(uiRefNum, &dwNumUsers);
    if (dwNumUsers == 0)
       SysLibRemove(uiRefNum);
    
  End of example.
 */
extern Err  GPDClose(UInt16 uRefNum, UInt32 *dwRefCountP)
  SYS_TRAP(sysLibTrapClose);

/*
  Use GPDGetVersion to query the GamepadDriver for it's version number.
  The version number is in the same format as the ROM version number, and 
  the ROM version macros in SystemMgr.h may be used to parse the returned 
  number. For example, sysGetROMVerMajor(*dwVerP) gets the major version 
  number.
 */
extern Err  GPDGetVersion(UInt16 uRefNum, UInt32 *dwVerP)
  SYS_TRAP(GPDTrapGetVersion);

/*
  Use GPDReadInstant to query the gamepad for the state of its keys.
  If the gamepad is connected, this function will return a zero value and
  put the result of the read at the value pointed to by result. Otherwise 
  it will return an error, such as GPDErrGamePadNotPresent if the GamePad 
  is not pluged in to the Palm.
 */
extern Err  GPDReadInstant(UInt16 uRefNum, UInt8 *resultP)
  SYS_TRAP(GPDTrapRead);

/*
  Use GPDReadHeld to query the gamepad for the state of its
  keys. If the gamepad is connected, this function will return a
  zero value and put the result of the read at the value pointed
  to by result.  Otherwise it will return an error, such as 
  GPDErrGamePadNotPresent if the GamePad is not pluged in to the Palm.
   
  This function is different from GPDReadInstant because it
  is guaranteed to return a '1' at least once if a key is pressed.
   
  Programs that poll slowly and would care about missing a fast button
  press should use GPDReadHeld in place of GPDReadInstant
 */

extern Err  GPDReadHeld(UInt16 uRefNum, UInt8 *resultP)
  SYS_TRAP(GPDTrapReadHeld);
  
/*
  Key Map DWord is a 4 byte (8 nibble) value. IE: 0x55316642
  
  Nibble order from left to right: 
  
    START, LEFTFIRE, LEFT, UP, SELECT, RIGHTFIRE, RIGHT, DOWN

  Nibble values:
    0   keyBitPower    Power key
    1   keyBitPageUp   Page-up
    2   keyBitPageDown  Page-down
    3   keyBitHard1    App #1
    4   keyBitHard2    App #2
    5   keyBitHard3    App #3
    6   keyBitHard4    App #4
    7   keyBitAntenna  Antenna "key" 
 */

extern Err GPDReadInstantKCSformat(UInt16 uRefNum, UInt32 *result, UInt32 keyMap)
  SYS_TRAP(GPDTrapReadInstantKCSformat);
extern Err GPDReadHeldKCSformat(UInt16 uRefNum, UInt32 *result, UInt32 keyMap)
  SYS_TRAP(GPDTrapReadHeldKCSformat);

//
// internal functions (useful for debugging)
//

extern Err  GPDStatus(UInt16 uRefNum, UInt32 *status)
  SYS_TRAP(GPDTrapStatus);
extern Err  GPDIsButtonPressed(UInt16 uRefNum, UInt8 *resultP)
  SYS_TRAP(GPDTrapIsButtonPressed);
extern Err  GPDSetLegacySupport(UInt16 uRefNum, Boolean state, Boolean enable)
  SYS_TRAP(GPDTrapSetLegacySupport);
extern Err  GPDGetLegacySupport(UInt16 uRefNum, Boolean *state, Boolean *enabled)
  SYS_TRAP(GPDTrapGetLegacySupport);

extern Err  GPDSetLegacyKeyMap(UInt16 uRefNum, UInt32 KeyCurrentStateBit, UInt32 GamePadBit)
  SYS_TRAP(GPDTrapSetLegacyKeyMap);
extern Err  GPDGetLegacyKeyMap(UInt16 uRefNum, UInt32 KeyCurrentStateBit, UInt32 *GamePadBit)
  SYS_TRAP(GPDTrapGetLegacyKeyMap);

#ifdef __cplusplus
}
#endif

#endif //GPDLIB_H
