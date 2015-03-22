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
#ifndef MPTRSET_H
#define MPTRSET_H

#include "mptrptr.h"

#define  DEFAULT_OVERRIDETIMEOUT       FALSE
#define  DEFAULT_ACTIVATEONLOAD        TRUE
#define  DEFAULT_ANIMATIONPATH         "?:\\OS2\\POINTERS"
#define  DEFAULT_DEMO                  0
#define  DEFAULT_INITDELAY             0
#define  DEFAULT_USEMOUSESETUP         FALSE
#define  DEFAULT_BLACKWHITE            FALSE
#define  DEFAULT_HIDEPOINTER           FALSE
#define  DEFAULT_HIDEPOINTERDELAY      20
#define  DEFAULT_DRAGPTRTYPE           RESFILETYPE_POINTER
#define  DEFAULT_DRAGSETTYPE           RESFILETYPE_ANIMOUSE

#define INITDELAY_MIN                  0
#define INITDELAY_MAX                  20 * 60

#define HIDEPONTERDELAY_MIN            1
#define HIDEPONTERDELAY_MAX            3600

// prototypes

APIRET ScanSetupString( HWND hwnd, PRECORDCORE pcnrrec, PSZ pszSetup, BOOL fModify, BOOL fCallAsync );
APIRET QueryCurrentSettings( PSZ *ppszSettings );
BOOL   IsSettingsInitialized( void );
BOOL   isnumeric( PSZ pszString );

// access functions for settings
// access to DEMO, ANIMATION and POINTER implemented elsewhere

// ACTIVATEONLOAD=YES|NO
VOID  setActivateOnLoad( BOOL fActivate );
BOOL  getActivateOnLoad( void );

// ANIMATIONINITDELAY=n
ULONG getDefaultAnimationInitDelay( void );
ULONG getAnimationInitDelay( void );
VOID  setAnimationInitDelay( ULONG ulNewInitDelay );

// ANIMATIONPATH=...
BOOL  setAnimationPath( PSZ pszNewPath);
PCSZ  getAnimationPath( void );

// BLACKWHITE=YES|NO
VOID  setBlackWhite( BOOL fNewBlackWhite );
BOOL  getBlackWhite( void );

// DRAGPTRTYPE=PTR,CUR,ANI,AND
// use QueryResFileExt() to convert
VOID  setDragPtrType( ULONG ulResFileType );
ULONG getDragPtrType( void );

// DRAGSETTYPE=PTR,CUR,ANI,AND
// use QueryResFileExt() to convert
VOID  setDragSetType( ULONG ulResFileType);
ULONG getDragSetType( void );

// FRAMELENGTH=xxx,ALL|UNDEFINED
VOID  setDefaultTimeout( ULONG ulTimeout );
ULONG getDefaultTimeout( void );
VOID  setOverrideTimeout( BOOL fOverride );
BOOL  getOverrideTimeout( void );

// HIDEPOINTER=ON|OFF
VOID setHidePointer( BOOL fNewHidePointer );
BOOL getHidePointer( void );
VOID overrideSetHidePointer( BOOL fNewHidePointer ); // use only in case of failure

// HIDEPOINTERDELAY=n
VOID  setHidePointerDelay( ULONG ulNewHidePointerDelay );
ULONG getHidePointerDelay( void );

// USEMOUSESETUP=YES|NO
VOID  setUseMouseSetup( BOOL fNewUseMouseSetup );
BOOL  getUseMouseSetup( void );

#endif // MPTRPPL_H

