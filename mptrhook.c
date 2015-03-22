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
// C Runtime
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// OS/2 Toolkit
#define  INCL_ERRORS
#define  INCL_PM
#define  INCL_WIN
#define  INCL_DOS
#define  INCL_DOSDEVIOCTL
#define  INCL_DOSMISC
#include <os2.h>

#include "mptrhook.h"
#include "title.h"
#include "debug.h"

// globale variablen
static HOOKDATA hookdataGlobal;
static HMODULE  hmodule = NULLHANDLE;

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : InputHook                                                  ³
 *³ Comment   :                                                            ³
 *³ Author    : C.Langanke                                                 ³
 *³ Date      : 06.12.1995                                                 ³
 *³ Update    : 06.12.1995                                                 ³
 *³ called by : PM message queue                                           ³
 *³ calls     : Win*, Dos*                                                 ³
 *³ Input     : HAB, PQMSG, ULONG - parms of input hook                    ³
 *³ Tasks     : - intercepts trigger message and performs specified action ³
 *³               on specified pushbutton.                                 ³
 *³ returns   : BOOL - remove flag                                         ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL EXPENTRY
InputHook( HAB hab, PQMSG pqmsg, ULONG fs )
{
  BOOL   fRemove = FALSE;
  USHORT x, y;
  static USHORT xold = 0, yold = 0;
  BOOL   fMouseMoved = FALSE;

  if(( pqmsg->msg >= WM_BUTTONCLICKFIRST ) &&
     ( pqmsg->msg <= WM_BUTTONCLICKLAST ))
  {
    // send message on any click
    fMouseMoved = TRUE;
  } else if( pqmsg->msg == WM_MOUSEMOVE ) {
    // send message on move
    // check position, because for WARP 3 there is
    // a periodic mousemove message without moving
    x = SHORT1FROMMP( pqmsg->mp1 );
    y = SHORT2FROMMP( pqmsg->mp1 );
    if(( x != xold ) || ( y != yold )) {
      fMouseMoved = TRUE;
    }
    xold = x;
    yold = y;
  }

  // post the message
  if( fMouseMoved && hookdataGlobal.hwndNotify ) {
    WinPostMsg( hookdataGlobal.hwndNotify, WM_TIMER,
                MPFROMSHORT( MOUSEMOVED_TIMER_ID ), MPFROMLONG( FALSE ));
  }

  return fRemove;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : SetupHooks                                                 ³
 *³ Kommentar : initialisiert Hook-Daten                                   ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

APIRET EXPENTRY
SetHooks( PSZ pszVersion, PHOOKDATA phookdata, HAB hab )
{
  APIRET rc = NO_ERROR;

  // check parms
  if(( pszVersion == NULL ) ||
     ( strcmp( __VERSION__, pszVersion ) != 0 ))
  {
    return ERROR_INVALID_PARAMETER;
  }

  // Zeiger bernehmen
  hookdataGlobal = *phookdata;

  if( !hmodule ) {
    if(( rc = DosQueryModuleHandle( "WPAMPTRH.DLL", &hmodule )) != NO_ERROR ) {
      return rc;
    }

    if( !WinSetHook( hab, 0, HK_INPUT, (PFN)InputHook, hmodule )) {
      hmodule = NULLHANDLE;
      return ERROR_GEN_FAILURE;
    }
  }

  return NO_ERROR;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : ReleaseHooks                                               ³
 *³ Kommentar : deinitialisiert den Hook                                   ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

APIRET EXPENTRY ReleaseHooks( HAB hab )
{
  if( hmodule && hab )
  {
    WinReleaseHook( hab, 0, HK_INPUT, (PFN)InputHook, hmodule );

    // This function releases the input hook and broadcasts a WM_NULL message
    // to all top level windows so that they will release the DLL.  If we don't
    // do this, the DLL will remain locked and we'll have to reboot in order to
    // recompile.

    WinBroadcastMsg( HWND_DESKTOP, WM_NULL, 0, 0, BMSG_FRAMEONLY | BMSG_POST );
    hmodule = NULLHANDLE;
  }

  return NO_ERROR;
}

