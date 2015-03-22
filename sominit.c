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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define  INCL_ERRORS
#define  INCL_PM
#define  INCL_DOS
#include <os2.h>

#include "wpamptr.h"
#include "debug.h"

#if defined( WPSCUR ) && WPSCUR == 1
#include "wpwcur.h"
#endif

#ifdef __IBMC__
#pragma linkage( SOMInitModule, system )
#endif

SOMEXTERN void SOMLINK
SOMInitModule( long majorVersion, long minorVersion, string className )
{
  FUNCENTER();

  SOM_IgnoreWarning( majorVersion );   /* This function makes */
  SOM_IgnoreWarning( minorVersion );   /* no use of the passed */
  SOM_IgnoreWarning( className );      /* arguments.   */

  WPAnimatedMousePointerNewClass( WPAnimatedMousePointer_MajorVersion, WPAnimatedMousePointer_MinorVersion );

  #if defined( WPSCUR ) && WPSCUR == 1
  WPWinCursorNewClass( WPWinCursor_MajorVersion, WPWinCursor_MinorVersion );
  #endif
}
