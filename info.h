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
#ifndef INFO_H
#define INFO_H

#include "mptrptr.h"

#define FI_SOURCEINFO    1
#define FI_DETAILINFO 1000

typedef struct _SOURCEINFO
{
  PSZ pszInfoName;
  PSZ pszInfoArtist;
} SOURCEINFO, *PSOURCEINFO;

typedef struct _PTRSOURCEINFO
{
  SOURCEINFO sourceinfo;
  BOOL       fColor;
  BYTE       XHotspot;
  BYTE       YHotspot;
  ULONG      ulColorCount;
  ULONG      ulSequence;
  ULONG      ulTimeout;
} PTSOURCEINFO, *PPTRSOURCEINFO;

typedef struct _ANIMSOURCEINFO
{
  SOURCEINFO     sourceinfo;
  ULONG          ulPhysFrames;
  ULONG          ulFrames;
  ULONG          ulDefaultTimeout;
  PPTRSOURCEINFO ppsi;
} ANIMSOURCEINFO, *PANIMSOURCEINFO;

typedef struct _ANIMSOURCEINFOLIST
{
  BOOL           fSingleAnimation;
  ANIMSOURCEINFO aasi[NUM_OF_SYSCURSORS];
} ANIMSOURCEINFOLIST, *PANIMSOURCEINFOLIST;

#endif // INFO_H
