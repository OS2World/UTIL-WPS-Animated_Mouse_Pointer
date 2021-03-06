/*
    Animated Mouse Pointer
    Copyright (C) 1997 Christian Langanke

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef POINTER_H
#define POINTER_H

#include "info.h"

// ** info structure for pointer  file

typedef struct _POINTERFILEINFO
{                          //    mandantory items marked with !
  BOOL  fMultResolutions;  // !  more than one pointer image (BITMAPARRAYFILEHEADER found)
  BOOL  fBwPointer;        // !  bw or color pointer
  BOOL  f13Header;         // !  OS/2 1.3 Header (OS/2 specific) or OS/2 2.x Header (Win compliant)
  ULONG ulBfhMaskOfs;      // !  offset of mask bitmap file header (BIMATPFILEHEADER or BIMATPFILEHEADER2)
  ULONG ulBfhMaskLen;      // !  length of mask bitmap file header
  ULONG ulBWColorTableOfs; // !  offset of bw color table
  ULONG ulBWColorTableLen; // !  length of bw color table (n * sizeof( RGB or RGB2))
  ULONG ulBfhOfs;          //    offset of 32x32 color image bitmap file header (BIMATPFILEHEADER or BIMATPFILEHEADER2)
  ULONG ulBfhLen;          //    Lenght of color bitmap file header
  ULONG ulColorTableOfs;   //    offset of color table
  ULONG ulColorTableLen;   //    length of color table (n * sizeof( RGB or RGB2))
  ULONG ulMaskDataOfs;     // !  offset of bw mask data
  ULONG ulMaskDataLen;     // !  length of bw mask data
  ULONG ulColorDataOfs;    // !  offset of color data
  ULONG ulColorDataLen;    // !  length of color data
  ULONG ulHotspotX;
  ULONG ulHotspotY;
  ULONG ulPointerColors;
  ULONG ulBitsPerPlane;
  ULONG ulPlanes;
} POINTERFILEINFO, *PPOINTERFILEINFO;

// Prototypes

BOOL LoadPointerFromPointerFile( PSZ pszName, PHPOINTER phptr, PICONINFO pIconInfo, PULONG paulTimeout );
#define LoadFirstPointerFromPointerFile( pszName, phptr, pIconInfo ) LoadPointerFromPointerFile( pszName, phptr, pIconInfo, NULL )
#define LoadFirstAnimationFromPointerFile( pszName, phptr, pulTimeout, pulEntries ) ( FALSE )

APIRET GrayScalePointer( PVOID pvIconData, ULONG ulScaleCount, ULONG ulBwTreshold );
APIRET QueryPointerFileDetails( PSZ pszAnimationFileName, ULONG ulInfoLevel, PVOID pvData, ULONG ulBuflen, PULONG pulSize );
APIRET WritePointerFile( PSZ pszAnimationFileName, ULONG ulTimeout, PICONINFO piconinfo, PSZ pszInfoName, PSZ pszInfoArtist );
APIRET SetPointerFileInfo( PSZ pszAnimationFileName, PSOURCEINFO psourceinfo );
APIRET GetPalette( PICONINFO piconinfo, PBYTE pbData, ULONG ulBuflen );

#endif // POINTER_H
