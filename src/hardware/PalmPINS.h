/*
 * @(#)PalmPINS.h
 *
 * Pen Input Manager (PINS) API of Palm OS 5 "HiRes+" devices with a
 * dynamic input area (320x480 / 480x320: Tungsten T5/TX, LifeDrive).
 *
 * The Tungsten T3 (Palm OS 5.2.1) has palmOne's own "Active Input Area"
 * instead and no PINS; it gets this API with palmOne's "Tungsten T3 DIA
 * Compatibility Update" (StatusBarLib + AppSlipRotate).
 *
 * The declarations follow PenInputMgr.h, Form.h, SystemMgr.h, Window.h
 * and NotifyMgr.h of the Palm OS 5 SDK (68K) R3; like PalmHDD.h they are
 * kept here so the application continues to build against the 4.0 SDK.
 */

#ifndef __PALMPINS_H__
#define __PALMPINS_H__

// feature to detect the pen input manager
#define pinCreator                      'pins'
#define pinFtrAPIVersion                1
#define pinAPIVersion1_0                0x01000000
#define pinAPIVersion1_1                0x01103000   // + orientation API

// input area states
#define pinInputAreaOpen                0
#define pinInputAreaClosed              1
#define pinInputAreaNone                2
#define pinInputAreaUser                5

// input area trigger (the "collapse" icon in the status bar)
#define pinInputTriggerEnabled          0
#define pinInputTriggerDisabled         1

// status bar attributes (StatGetAttribute)
#define statAttrBarVisible              0

// form dynamic input area policies
#define frmDIAPolicyStayOpen            0
#define frmDIAPolicyCustom              1

// orientations
#define sysOrientationUser              0
#define sysOrientationPortrait          1
#define sysOrientationLandscape         2
#define sysOrientationReversePortrait   3
#define sysOrientationReverseLandscape  4

// orientation trigger (the "rotate" icon in the status bar)
#define sysOrientationTriggerDisabled   0
#define sysOrientationTriggerEnabled    1

// sent to the active form when the display extent has changed
#define winDisplayChangedEvent          0x4101

// broadcast after the input area or the orientation has changed
#define sysNotifyDisplayResizedEvent    'scrs'

// constraint sizes
#define pinMaxConstraintSize            0x7FFF

// PINS trap selectors
#define pinPINSetInputAreaState         0
#define pinPINGetInputAreaState         1
#define pinPINSetInputTriggerState      2
#define pinPINGetInputTriggerState      3
#define pinWinSetConstraintsSize        13
#define pinFrmSetDIAPolicyAttr          14
#define pinFrmGetDIAPolicyAttr          15
#define pinStatHide                     16
#define pinStatShow                     17
#define pinStatGetAttribute             18
#define pinSysGetOrientation            19
#define pinSysSetOrientation            20
#define pinSysGetOrientationTriggerState 21
#define pinSysSetOrientationTriggerState 22

#define sysTrapPinsDispatch             0xA470

#define PINS_TRAP(selector) \
  _SYSTEM_API(_CALL_WITH_SELECTOR)(_SYSTEM_TABLE, sysTrapPinsDispatch, selector)

#ifdef __cplusplus
extern "C" {
#endif

extern Err    PINSetInputAreaState(UInt16 state)
                PINS_TRAP(pinPINSetInputAreaState);
extern UInt16 PINGetInputAreaState(void)
                PINS_TRAP(pinPINGetInputAreaState);
extern Err    PINSetInputTriggerState(UInt16 state)
                PINS_TRAP(pinPINSetInputTriggerState);
extern UInt16 PINGetInputTriggerState(void)
                PINS_TRAP(pinPINGetInputTriggerState);
extern Err    WinSetConstraintsSize(WinHandle winH,
                                    Coord minH, Coord prefH, Coord maxH,
                                    Coord minW, Coord prefW, Coord maxW)
                PINS_TRAP(pinWinSetConstraintsSize);
extern Err    FrmSetDIAPolicyAttr(FormPtr formP, UInt16 diaPolicy)
                PINS_TRAP(pinFrmSetDIAPolicyAttr);
extern UInt16 FrmGetDIAPolicyAttr(FormPtr formP)
                PINS_TRAP(pinFrmGetDIAPolicyAttr);
extern Err    StatGetAttribute(UInt16 selector, UInt32 *dataP)
                PINS_TRAP(pinStatGetAttribute);
extern Err    StatHide(void)
                PINS_TRAP(pinStatHide);
extern Err    StatShow(void)
                PINS_TRAP(pinStatShow);
extern UInt16 SysGetOrientation(void)
                PINS_TRAP(pinSysGetOrientation);
extern Err    SysSetOrientation(UInt16 orientation)
                PINS_TRAP(pinSysSetOrientation);
extern UInt16 SysGetOrientationTriggerState(void)
                PINS_TRAP(pinSysGetOrientationTriggerState);
extern Err    SysSetOrientationTriggerState(UInt16 triggerState)
                PINS_TRAP(pinSysSetOrientationTriggerState);

#ifdef __cplusplus
}
#endif

#endif
