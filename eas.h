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
#ifndef EAS_H
#define EAS_H

#define EANAME_TIMEFRAME   ".TIMEFRAME"
#define EANAME_INFONAME    ".INFONAME"
#define EANAME_INFOARTIST  ".INFOARTIST"

#pragma pack(1)

typedef struct _EAMVMT
{
  USHORT usType;
  USHORT usCodepage;
  USHORT usEntries;
  USHORT usEntryType;
  USHORT usEntryLen;
  CHAR   chEntry[1];
} EAMVMT, *PEAMVMT;

typedef struct _EASVST
{
  USHORT usType;
  USHORT usEntryLen;
  CHAR   chEntry[1];
} EASVST, *PEASVST;

#pragma pack()

// prototypes
APIRET WriteStringEa( HFILE hfile, PSZ pszEaName, PSZ pszEaValue );
APIRET ReadStringEa( PSZ pszFileName, PSZ pszEaName, PSZ pszBuffer, PULONG ulBuflen );

#endif // EAS_H

