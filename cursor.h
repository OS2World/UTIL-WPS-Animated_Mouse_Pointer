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
#ifndef CURSOR_H
#define CURSOR_H

#include "info.h"

// define MASK for ICONINFO.fFormat
#define ICON_CURSOR 0x8000

#pragma pack(1)

// structures
typedef struct _ANIHEADER
{
  ULONG cbSizeof;                  // Num. bytes in aniheader (incl. cbSizeof)
  ULONG cFrames;                   // Number of unique icons in the ani. cursor
  ULONG cSteps;                    // Number of frames in the animation */
  ULONG cx, cy;                    // reserved, must be 0
  ULONG cBitCount;                 // reserved, must be 0
  ULONG cPlanes;                   // reserved, must be 0
  ULONG jifRate;                   // default rate if rate chunk not present
  ULONG fl;                        // flag
} ANIHEADER, *PANIHEADER;

// ** info structure for cursor file

typedef struct _CURSORFILEINFO
{
  BOOL  fMultResolutions;
  BOOL  fBwPointer;
  ULONG ulCurFileHeaderOfs;                    //    offset of CURFILEHEADER
  ULONG ulCurFileHeaderLen;                    //    length of CURFILEHEADER
  ULONG ulCurFileResourceOfs;                  //    offset of CURFILERES
  ULONG ulCurFileResourceLen;                  //    length of CURFILERES
  ULONG ulDIBDataLen;                          //    length of DIB Data
  ULONG ulBfhOfs;                              //    offset of bitmap file header
  ULONG ulBfhLen;                              //    length of bitmap file header
  ULONG ulColorTableOfs;                       //    offset of color table
  ULONG ulColorTableLen;                       //    length of color table (n * sizeof( RGB2))
  ULONG ulBitmapDataOfs;                       // !  offset of bitmap data
  ULONG ulBitmapDataLen;                       // !  length of bitmap data
  ULONG ulMaskDataOfs;                         // !  offset of bw mask data
  ULONG ulMaskDataLen;                         // !  length of bw mask data
  ULONG ulHotspotX;
  ULONG ulHotspotY;
  ULONG ulPointerColors;
  ULONG ulBitsPerPlane;
  ULONG ulPlanes;
} CURSORFILEINFO, *PCURSORFILEINFO;

// ** info structure for animation file
// note: - chunk offsets point to chunk headers
typedef struct _ANIFILEINFO                    //    - chunk length does include the headers
{                                              //    - mandantory items marked with !
  ULONG     ulFrameCount;                      // !  count of frames in animation
  ULONG     ulPhysFrameCount;                  // !  count of phyical frames
  ULONG     ulDefaultTimeout;                  //    default timeout in ms
  ULONG     ulInfoListOfs;                     //    offset of info chunk
  ULONG     ulInfoListLen;                     //    length of info chunk
  ULONG     ulInfoNameOfs;                     //    offset of name chunk
  ULONG     ulInfoNameLen;                     //    length of name chunk
  ULONG     ulInfoArtistOfs;                   //    offset of artist chunk
  ULONG     ulInfoArtistLen;                   //    length of artist chunk
  ULONG     ulDisplayOfs;                      //    offset of display chunk
  ULONG     ulDisplayLen;                      //    offset of display chunk
  ULONG     ulAniheaderOfs;                    // !  offset of aniheader
  ANIHEADER aniheader;                         // !  aniheader data
  ULONG     ulRateinfoOfs;                     //    offset of rate info table
  ULONG     ulRateinfoLen;                     //    length of rate info table
  PULONG    paulTimeout;                       //    pointer to table with timeouts in ms.
  ULONG     ulSequenceinfoOfs;                 //    offset of sequence info table
  ULONG     ulSequenceinfoLen;                 //    length of sequence info table
  PULONG    paulSequence;                      // !  pointer to table with sequence index values
  ULONG     ulFrameListOfs;                    // !  offset to frame list chunk
  ULONG     ulFrameListLen;                    // !  length of frame list chunk
  PULONG    paulFrameOfs;                      // !  pointer to table with file offsets for frames
} ANIFILEINFO, *PANIFILEINFO;

#pragma pack()

// Prototypes

BOOL EXPENTRY LoadPointerFromCursorFile( PSZ pszName, PHPOINTER phptr, PICONINFO pIconInfo, PULONG paulTimeout );
#define LoadFirstPointerFromCursorFile( pszName, phptr, pIconInfo ) LoadPointerFromCursorFile( pszName, phptr, pIconInfo,  NULL )
#define LoadFirstAnimationFromCursorFile( pszName, phptr, pulTimeout, pulEntries ) ( FALSE )

APIRET EXPENTRY WriteCursorFile( PSZ pszAnimationFileName, ULONG ulTimeout, PICONINFO piconinfo, PSZ pszInfoName, PSZ pszInfoArtist );
APIRET EXPENTRY QueryCursorFileDetails( PSZ pszAnimationFileName, ULONG ulInfoLevel, PVOID pvData, ULONG ulBuflen, PULONG pulSize );
APIRET EXPENTRY SetCursorFileInfo( PSZ pszAnimationFileName, PSOURCEINFO psourceinfo );

BOOL EXPENTRY LoadPointerFromWinAnimationFile( PSZ pszName, PHPOINTER pahpointer, PICONINFO paiconinfo, PULONG paulTimeout, PULONG pulEntries );
BOOL EXPENTRY LoadFirstPointerFromWinAnimationFile( PSZ pszAnimationFileName, PHPOINTER phpointer, PICONINFO paiconinfo );
BOOL EXPENTRY LoadFirstAnimationFromWinAnimationFile( PSZ pszAnimationFile, PHPOINTER pahpointer, PULONG pulTimeout, PULONG pulEntries );

APIRET EXPENTRY WriteWinAnimationFile( PSZ pszAnimationFileName, PULONG paulTimeout, PICONINFO paiconinfo, ULONG ulEntries, PSZ pszInfoName, PSZ pszInfoArtist );
APIRET EXPENTRY QueryWinAnimationFileDetails( PSZ pszAnimationFileName, ULONG ulInfoLevel, PVOID pvData, ULONG ulBuflen, PULONG pulSize );
APIRET EXPENTRY SetWinAnimationFileInfo( PSZ pszAnimationFileName, PSOURCEINFO psourceinfo );

#endif // CURSOR_H

