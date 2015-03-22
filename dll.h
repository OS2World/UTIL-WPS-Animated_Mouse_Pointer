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
#ifndef DLL_H
#define DLL_H

#include "mptrppl.h"
#include "mptrptr.h"
#include "info.h"

#pragma pack(1)

typedef struct _FRAMEINFO
{
  USHORT usResId;            // Resource Id
  ULONG  ulResSize;          // Resource size
  ULONG  ulPtrIndex;         // index to system pointer
  ULONG  ulResOfs;           // Offset within file
  ULONG  ulTimeout;          // timeout for this frame
  PVOID  pvDuplicate;        // pointer to duplicate frame
} FRAMEINFO, *PFRAMEINFO;

typedef struct _ANDFILEINFO
{
  ULONG      aulFrameCount[ NUM_OF_SYSCURSORS];      // !  count of frames in animation
  PFRAMEINFO apframeinfoStatic[ NUM_OF_SYSCURSORS];  // !  pointer to first frame for syspointer
  PFRAMEINFO apframeinfo1st[ NUM_OF_SYSCURSORS];     // !  pointer to first frame for syspointer
  ULONG      ulPointerCount;                         // !  count of all pointer images
  PFRAMEINFO apframeinfo;                            // !  pointer to frame table
} ANDFILEINFO, *PANDFILEINFO;

#pragma pack()

// Prototypes
BOOL EXPENTRY LoadPointerFromAnimouseFile( HMODULE hmod, ULONG ulPointerIndex, PHPOINTER pahptr, PICONINFO paiconinfo, PHPOINTER phptrStatic, PICONINFO piconinfoStatic, PULONG paulTimeout, PULONG pulEntries );
BOOL EXPENTRY LoadFirstPointerFromAnimouseFile( PSZ pszAnimationFile, PHPOINTER phptr, PICONINFO piconinfo );
BOOL EXPENTRY LoadFirstAnimationFromAnimouseFile( PSZ pszAnimationFile, PHPOINTER pahpointer, PULONG pulTimeout, PULONG pulEntries );

APIRET EXPENTRY WriteAnimouseDllFile( PSZ pszAnimationFileName, BOOL fSaveAll, PPOINTERLIST papl, PSZ pszInfoName, PSZ pszInfoArtist );
APIRET EXPENTRY QueryAnimouseFileDetails( PSZ pszAnimationFileName, ULONG ulInfoLevel, PVOID pvData, ULONG ulBuflen, PULONG pulSize );
APIRET EXPENTRY SetAnimouseFileInfo( PSZ pszAnimationFileName, PSOURCEINFO psourceinfo );

ULONG GetAnimouseBasePointerId( ULONG ulPointerIndex );

#endif // DLL_H

