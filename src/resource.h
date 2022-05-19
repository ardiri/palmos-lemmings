/*
 * @(#)resource.h
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

// COMPILING
//
// compilation configurations, allowing custom builds or testing of
// various components/features that are available in the application.
// To enable a particular portion, uncomment the #define and compile.

// #define MDM_DISTRIBUTION        1   // special build for MDM

   #define HANDERA_NATIVE          1   // support Handera native  240x240
   #define SONY_NATIVE             1   // support Sony native     320x320
   #define PALM_HIDENSITY          1   // support Palm hi-density 320x320

   #define PALM_AUDIO_STREAMING    1   // palmos 5.0+ audio (streaming)
   #define PALM_AUDIO_YAMAHAPA1    1   // yamaha PA1 audio support (beat plus)

   #define PALM_MIDI_STREAMING     1   // palmos 5.0+ midi playback (streaming)
   #define PALM_MIDI_YAMAHAPA1     1   // yamaha PA1 midi support (beatplus)
   #define MIDI_SIMPLE_SAMPLES     1   // simple samples -> PALM_MIDI_STREAMING
// #define MIDI_PAUSE_ON_DIALOG    1   // pause music on dialog pop-up?
// #define MIDI_IN_LEVELPACK       1   // midi support for level packs
   #define MUSIC_ENGINE            1   // we have music :)

   #define USE_CHIP_COMPRESS       1   // use chip's mega compression algo :P
   #define NO_DYNAMIC_RAM          1   // dont use any dynamic ram whatsoever!
   #define FULL_SCREEN_BLIT        1   // just blit the whole screen, no y1-y2
   #define NEW_MEMORY_MGR          1   // use "static" ram (not dynamic)

   #define CHEAT_MODE              1   // are we going to allow cheating :)
// #define PROTECTION_ON           1   // encrypted level info (release ver) :P

// #define USE_PALMOS_WINAPI       1   // use the Win* API for sprites
// #define PORTABLE                1   // do not use any m68k asm
// #define DRAM_WS                 1   // disable wait states on m68k CPU

// the following options are for debugging purposes and should not
// be used when building and shipping the final product to users.

// #define SHOW_Y_UPDATE_BARS      1   // show the y1-y2 update bars
// #define SHOW_MASK_IN_2BPP       1   // instead of showing level, show mask
// #define SHOW_FPS                1   // show the frame/sec info [profiling]
// #define SHOW_DECOMPRESSION_TIME 1   // show how long it takes to decompress

// lemmings is a serious memory hog and does not play well with "palmos"
// in general, for example, reports of locking up when beam receive and
// even performing a hotsync while the game is running :(

// #define DISABLE_HOTSYNC         1   // disable the hotsync event

//
// SANITY
//

#ifdef FULL_SCREEN_BLIT
#undef  SHOW_Y_UPDATE_BARS             // - show the y1-y2 update bars
#endif

#ifdef AVOID_BAD_WINDOW
#undef  USE_PALMOS_WINAPI              // - use the Win* API for sprites
#undef  NEW_MEMORY_MGR
#define NEW_MEMORY_MGR             1   // + use "static" ram (not dynamic)
#endif

#ifdef PALM_MIDI_STREAMING
#define PALM_MIDI_ANY              1
#endif
#ifdef PALM_MIDI_YAMAHAPA1
#ifndef PALM_MIDI_ANY
#define PALM_MIDI_ANY              1
#endif
#endif

#ifdef PALM_AUDIO_STREAMING
#define PALM_AUDIO_ANY             1
#endif
#ifdef PALM_AUDIO_YAMAHAPA1
#ifndef PALM_AUDIO_ANY
#define PALM_AUDIO_ANY             1
#endif
#endif

// bitmaps

#define bitmapIcon               1000
#define bitmapPalm               1001
#define bitmapLogo               1002
#define bitmapPGHQ               1003
#define bitmapPaw                1004
#define bitmapGrayscaleTest      1005
#define bitmapKeyConfig          1006
#define bitmapAbout              1007
#define bitmapHelp               1008
#define bitmapHelpLemmings       1009
#define bitmapLemmingTools       1010

#define bitmapToolBar            1020
#define bitmapLemmings           1021
#define bitmapLemmingsMask       1022
#define bitmapGameMasks          1023
#define bitmapLemmingsColor      1024

// alerts

#define restartLevelAlert        1000
#define skipLevelAlert           1001
#define quitGameAlert            1002
#define toolNotAvailable         1003
#define loadGameAlert            1004
#define saveGameAlert            1005
#define invalidPackAlert         1006
#define removePackAlert          1007
#define exitGameAlert            1008

#define deviForm                 1100
#define demoForm                 1200
#define demoFormDemoCheckbox     1201
#define demoFormAcceptButton     1202
#define demoFormDeclineButton    1203

// dialogs

#define infoForm                 2000
#define infoFormOkButton         2001
#define dvlpForm                 2100
#define dvlpFormOkButton         2101
#define cfigForm                 2200
#define cfigFormHardKey1Trigger  2201
#define cfigFormHardKey1List     2202
#define cfigFormHardKey2Trigger  2203
#define cfigFormHardKey2List     2204
#define cfigFormHardKey3Trigger  2205
#define cfigFormHardKey3List     2206
#define cfigFormHardKey4Trigger  2207
#define cfigFormHardKey4List     2208
#define cfigFormPageUpTrigger    2209
#define cfigFormPageUpList       2210
#define cfigFormPageDownTrigger  2211
#define cfigFormPageDownList     2212
#define cfigFormScrollCheckbox   2213
#define cfigFormScaleCheckbox    2214
#define cfigFormSound0Button     2215
#define cfigFormSound1Button     2216
#define cfigFormSound2Button     2217
#define cfigFormSound3Button     2218
#define cfigFormSound4Button     2219
#define cfigFormSound5Button     2220
#define cfigFormSound6Button     2221
#define cfigFormSound7Button     2222
#define cfigFormMuteCheckbox     2223
#define cfigFormMusicCheckbox    2224
#define cfigFormOkButton         2225
#define cfigFormCancelButton     2226
#define grayForm                 2300
#define grayWhite1Button         2301
#define grayWhite2Button         2302
#define grayWhite3Button         2303
#define grayWhite4Button         2304
#define grayWhite5Button         2305
#define grayWhite6Button         2306
#define grayWhite7Button         2307
#define grayLightGray1Button     2308
#define grayLightGray2Button     2309
#define grayLightGray3Button     2310
#define grayLightGray4Button     2311
#define grayLightGray5Button     2312
#define grayLightGray6Button     2313
#define grayLightGray7Button     2314
#define grayDarkGray1Button      2315
#define grayDarkGray2Button      2316
#define grayDarkGray3Button      2317
#define grayDarkGray4Button      2318
#define grayDarkGray5Button      2319
#define grayDarkGray6Button      2320
#define grayDarkGray7Button      2321
#define grayBlack1Button         2322
#define grayBlack2Button         2323
#define grayBlack3Button         2324
#define grayBlack4Button         2325
#define grayBlack5Button         2326
#define grayBlack6Button         2327
#define grayBlack7Button         2328
#define grayFormOkButton         2329
#define helpForm                 2400
#define helpFormScrollBar        2401
#define helpFormOkButton         2402
#define xmemForm                 2500
#define xmemFormOkButton         2501
#define prepForm                 2600
#define prepFormPreviewGadget    2601
#define prepFormGoButton         2602
#define verdForm                 2700
#define verdFormOkButton         2701
#define regiForm                 2800
#define regiFormOkButton         2801
#define packForm                 2900
#define packFormHardCheckbox     2901
#define packFormDatabaseList     2902
#define packFormOpenButton       2903
#define packFormDeleteButton     2904
#define packFormCancelButton     2905

// forms

#define globalFormHelpButton     3001
#define globalFormAboutButton    3002
#define mainForm                 3100
#define mainFormPlayButton       3101
#define mainFormLevelTrigger     3102
#define mainFormLevelList        3103
#define mainFormLevelButton      3104
#define gameForm                 3200
#define gameFormSoundButton      3201
#define gameFormPauseButton      3202

// menus

#define menuItemGrayscale        3001
#define menuItemConfig           3002
#define menuItemRegister         3003
#define menuItemDemo             3004
#define menuItemHelp             3005
#define menuItemDeveloper        3006
#define menuItemAbout            3007
#define menuItemKeyConfig        3008
#define mainMenu_nogray          3100
#define mainMenu_gray            3101
#define mainMenuItemPlay         3110
#define mainMenuItemLevelPacks   3111
#define gameMenu_nogray          3200
#define gameMenu_gray            3201
#define gameMenuItemRestart      3210
#define gameMenuItemSkip         3211
#define gameMenuItemLevelPacks   3212
#define gameMenuItemPause        3213
#define gameMenuItemExit         3214

// palettes

#define paletteVGA16             4000
#define paletteGameBase          4001

// strings

#define stringDemo               5000
#define stringDemoMainPage       5001
#define stringRegisteredPack     5002
#define stringMainTitle          5003
#define stringCopyrightNotice    5004

#define stringVerdButton0        5100
#define stringVerdButton1        5101
#define stringVerdLevel0_0       5200
#define stringVerdLevel0_1       5201
#define stringVerdLevel0_2       5202
#define stringVerdLevel0_3       5203
#define stringVerdLevel1_0       5300
#define stringVerdLevel1_1       5301
#define stringVerdLevel1_2       5302
#define stringVerdLevel1_3       5303
#define stringVerdLevel2_0       5400
#define stringVerdLevel2_1       5401
#define stringVerdLevel2_2       5402
#define stringVerdLevel2_3       5403
#define stringVerdLevel3_0       5500
#define stringVerdLevel3_1       5501
#define stringVerdLevel3_2       5502
#define stringVerdLevel3_3       5503
#define stringVerdLevel4_0       5600
#define stringVerdLevel4_1       5601
#define stringVerdLevel4_2       5602
#define stringVerdLevel4_3       5603

// audio
#ifdef PALM_AUDIO_STREAMING
#define sfxResource              1000
#endif

#ifdef PALM_AUDIO_ANY
#define audioBase                1000
#define audioLetsGo              audioBase+0
#define audioChangeTool          audioBase+1
#define audioSelection           audioBase+2
#define audioOhNo                audioBase+3
#define audioYippee              audioBase+4
#define audioExplode             audioBase+5
#define audioSplat               audioBase+6
#define audioTing                audioBase+7
#define MAX_AUDIO                (audioTing-audioBase+1)
#endif

// music
#ifdef PALM_MIDI_ANY
#define midiResource             1001
#endif

#ifdef PALM_MIDI_STREAMING
#define midiSampleInstrument     2000
#define midiSampleDrum           3000
#endif
