/*************************************************************************
 *
 * Copyright (c) 2002 MobileWizardry
 * All rights reservered.
 *
 *************************************************************************/

/*
 * @(#)palmHDD.h
 *
 * -- Aaron Ardiri  (mailto:aaron_ardiri@mobilewizardry.com)
 *    Chip Kerchner (mailto:chip_kerchner@mobilewizardry.com)
 *    Andrew Empson (mailto:andrew_empson@mobilewizardry.com)
 */

#ifndef __PALMHDD_H__
#define __PALMHDD_H__

#include <ErrorBase.h>

// bitmap version numbers
#define BitmapVersionThree	3

// constants used by WinSetCoordinateSystem
#define kCoordinatesNative	0
#define kCoordinatesStandard	72
#define kCoordinatesOneAndAHalf	108
#define kCoordinatesDouble	144
#define kCoordinatesTriple	216
#define kCoordinatesQuadruple	288

// pixel format defined with BitmapVersion3
typedef enum PixelFormatTag
{
	pixelFormatIndexed,  // standard for Palm 68k;  standard for BMPs
	pixelFormat565,      // standard for Palm 68k
	pixelFormat565LE,    // standard for BMPs;  popular on ARM hardware
	pixelFormatIndexedLE // popular on ARM hardware
} PixelFormatType;

// constants used by density field
typedef enum DensityTag {
	kDensityLow		= 72,
	kDensityOneAndAHalf	= 108,
	kDensityDouble		= 144,
	kDensityTriple		= 216,
	kDensityQuadruple	= 288
} DensityType;

// selectors for WinScreenGetAttribute
typedef enum WinScreenAttrTag 
{
	winScreenWidth,
	winScreenHeight,
	winScreenRowBytes,
	winScreenDepth,
	winScreenAllDepths,
	winScreenDensity,
	winScreenPixelFormat,
	winScreenResolutionX,
	winScreenResolutionY
} WinScreenAttrType;

// This data structure is the PalmOS 5 version 3 BitmapType.
typedef struct BitmapTypeV3
#ifdef ALLOW_ACCESS_TO_INTERNALS_OF_BITMAPS	
{
	// BitmapType
	Int16  			width;
	Int16  			height;
	UInt16  		rowBytes;
	BitmapFlagsType		flags;			// see BitmapFlagsType
	UInt8			pixelSize;		// bits per pixel
	UInt8			version;		// data structure version 3
	
	// version 3 fields
	UInt8			size;			// size of this structure in bytes (0x16)
	UInt8			pixelFormat;		// format of the pixel data, see pixelFormatType
	UInt8			unused;
	UInt8			compressionType;	// see BitmapCompressionType
	UInt16			density;		// used by the blitter to scale bitmaps
	UInt32			transparentValue;	// the index or RGB value of the transparent color
	UInt32			nextBitmapOffset;	// byte offset to next bitmap in bitmap family

	// if (flags.hasColorTable)
	// {
	//	if (flags.indirectColorTable)
	//		ColorTableType* colorTableP;	// pointer to color table
	//	else
	//  		ColorTableType	colorTable;	// color table, could have 0 entries (2 bytes long)
	// }
	//
	// if (flags.indirect)
	//   	void*	  bitsP;			// pointer to actual bits
	// else
	//   	UInt8	  bits[];			// or actual bits
	//
}
#endif
BitmapTypeV3;
typedef BitmapTypeV3* BitmapPtrV3;

// high density trap selectors
#define HDSelectorBmpGetNextBitmapAnyDensity		0
#define HDSelectorBmpGetVersion				1
#define HDSelectorBmpGetCompressionType			2
#define HDSelectorBmpGetDensity				3
#define HDSelectorBmpSetDensity				4
#define HDSelectorBmpGetTransparentValue		5
#define HDSelectorBmpSetTransparentValue		6
#define HDSelectorBmpCreateBitmapV3			7
#define HDSelectorWinSetCoordinateSystem		8
#define HDSelectorWinGetCoordinateSystem		9
#define HDSelectorWinScalePoint				10
#define HDSelectorWinUnscalePoint			11
#define HDSelectorWinScaleRectangle			12
#define HDSelectorWinUnscaleRectangle			13
#define HDSelectorWinScreenGetAttribute			14
#define HDSelectorWinPaintTiledBitmap			15
#define HDSelectorWinGetSupportedDensity		16
#define HDSelectorEvtGetPenNative			17
#define HDSelectorWinScaleCoord				18
#define HDSelectorWinUnscaleCoord			19
#define HDSelectorWinPaintRoundedRectangleFrame		20
#define HDSelectorInvalid				21  // leave this selector at end

#define sysTrapSysHighDensitySelector                   0xA3EC
#define HIGH_DENSITY_TRAP(selector) _SYSTEM_API(_CALL_WITH_SELECTOR)(_SYSTEM_TABLE, sysTrapSysHighDensitySelector, selector)

#ifdef __cplusplus
extern          "C"
{
#endif

//-----------------------------------------------
// High Density Bitmap support functions           
//-----------------------------------------------
extern BitmapType* BmpGetNextBitmapAnyDensity(BitmapType* bitmapP)
 	HIGH_DENSITY_TRAP(HDSelectorBmpGetNextBitmapAnyDensity);

extern UInt8 BmpGetVersion(const BitmapType* bitmapP)
	HIGH_DENSITY_TRAP(HDSelectorBmpGetVersion);

extern BitmapCompressionType BmpGetCompressionType(const BitmapType* bitmapP)
	HIGH_DENSITY_TRAP(HDSelectorBmpGetCompressionType);

extern UInt16 BmpGetDensity(const BitmapType* bitmapP)
	HIGH_DENSITY_TRAP(HDSelectorBmpGetDensity);

extern Err BmpSetDensity(BitmapType* bitmapP, UInt16 density)
	HIGH_DENSITY_TRAP(HDSelectorBmpSetDensity);
							
extern Boolean BmpGetTransparentValue(const BitmapType* bitmapP, UInt32* transparentValueP)
	HIGH_DENSITY_TRAP(HDSelectorBmpGetTransparentValue);
							
extern void BmpSetTransparentValue(BitmapType* bitmapP, UInt32 transparentValue)
	HIGH_DENSITY_TRAP(HDSelectorBmpSetTransparentValue);
							
extern BitmapTypeV3* BmpCreateBitmapV3(const BitmapType* bitmapP, UInt16 density, const void* bitsP, const ColorTableType* colorTableP)
	HIGH_DENSITY_TRAP(HDSelectorBmpCreateBitmapV3);

extern UInt16 WinSetCoordinateSystem(UInt16 coordSys)
	HIGH_DENSITY_TRAP(HDSelectorWinSetCoordinateSystem);
							
extern UInt16 WinGetCoordinateSystem(void)
	HIGH_DENSITY_TRAP(HDSelectorWinGetCoordinateSystem);
							
extern Coord WinScaleCoord(Coord coord, Boolean ceiling)
	HIGH_DENSITY_TRAP(HDSelectorWinScaleCoord);

extern Coord WinUnscaleCoord(Coord coord, Boolean ceiling)
	HIGH_DENSITY_TRAP(HDSelectorWinUnscaleCoord);

extern void WinScalePoint(PointType* pointP, Boolean ceiling)
	HIGH_DENSITY_TRAP(HDSelectorWinScalePoint);

extern void WinUnscalePoint(PointType* pointP, Boolean ceiling)
	HIGH_DENSITY_TRAP(HDSelectorWinUnscalePoint);

extern void WinScaleRectangle(RectangleType* rectP)
	HIGH_DENSITY_TRAP(HDSelectorWinScaleRectangle);

extern void WinUnscaleRectangle(RectangleType* rectP)
	HIGH_DENSITY_TRAP(HDSelectorWinUnscaleRectangle);

extern Err WinScreenGetAttribute(WinScreenAttrType selector, UInt32* attrP)
	HIGH_DENSITY_TRAP(HDSelectorWinScreenGetAttribute);

extern void WinPaintTiledBitmap(BitmapType* bitmapP, RectangleType* rectP)
	HIGH_DENSITY_TRAP(HDSelectorWinPaintTiledBitmap);
							
extern Err WinGetSupportedDensity(UInt16* densityP)
	HIGH_DENSITY_TRAP(HDSelectorWinGetSupportedDensity);
							
extern void EvtGetPenNative(WinHandle winH, Int16* pScreenX, Int16* pScreenY, Boolean* pPenDown)
	HIGH_DENSITY_TRAP(HDSelectorEvtGetPenNative);

#ifdef __cplusplus
}
#endif

#endif /* __PALMHDD_H__ */
