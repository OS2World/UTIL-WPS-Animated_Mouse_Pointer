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
#ifndef MPTRCNR_H
#define MPTRCNR_H

#include "mptrppl.h"

#define NEWLINE            "\n"
#define MAX_RES_STRLEN     32
#define MAX_RES_MSGLEN     256

typedef struct _HANDLERDATA
{
  HMODULE       hmodResource;    // input fields
  PVOID         somSelf;         //   "      "
  PRECORDCORE*  ppcnrrec;        //   "      "
  PFNWP         pfnwpOriginal;
  PVOID         pvReserved;      // handler storage data
} HANDLERDATA,  *PHANDLERDATA;

typedef struct _MYRECORDCORE
{
  RECORDCORE   rec;
  ULONG        ulPointerIndex;
  PSZ          pszAnimationName;
  PSZ          pszAnimate;
  PSZ          pszAnimationType;
  PSZ          pszInfoName;
  PSZ          pszInfoArtist;
  PSZ          pszFrameRate;
  ULONG        ulPtrCount;
  PPOINTERLIST ppl;
} MYRECORDCORE, *PMYRECORDCORE;

// Prototypes
VOID EXPENTRY InitStringResources( HAB hab, HMODULE hmodResource );

BOOL InitPtrSetContainer( HWND hwnd, PRECORDCORE* ppvCnrData );
BOOL SetContainerView( HWND hwnd, ULONG ulViewStyle );
BOOL QueryContainerView( HWND hwnd, PULONG pulViewStyle );
BOOL QueryItemSet( PRECORDCORE prec );
BOOL QueryItemLoaded( PRECORDCORE prec );
BOOL QueryItemAnimate( PRECORDCORE prec );
BOOL RefreshCnrItem( HWND hwnd, PRECORDCORE prec, PRECORDCORE pcnrrec, BOOL fResetArrowPtr );

BOOL LoadAnimationResource( HWND hwnd, PSZ pszName, PRECORDCORE prec, PRECORDCORE pcnrrec );
BOOL ToggleAnimate( HWND hwnd, ULONG ulPtrIndex, PRECORDCORE prec,
                    PRECORDCORE pcnrrec, BOOL fChangeAll, BOOL fRefresh, PBOOL pfEnable );

#endif // MPTRCNR_H

