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
/* Retrieves a message from the messages catalog.
 */

#define  INCL_ERRORS
#define  INCL_WIN
#define  INCL_GPI
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "ecolange.h"
#include "debug.h"

APIRET xmessage( char* result, int size, const char* key )
{
  LNGHANDLE lng;
  APIRET rc;
  const char* source;
  char* target;
  char* mangled = (char*)malloc( strlen( key ) * 2 + 1 );

  if( !mangled ) {
    strncpy( result, key, size );
    return ERROR_NOT_ENOUGH_MEMORY;
  }

  source = key;
  target = mangled;

  while( *source ) {
     if( *source == '\n' ) {
       *target++ = '\\'; *target++ = 'n';
     } else if( *source == '\r' ) {
       *target++ = '\\'; *target++ = 'r';
     } else if( *source == '\t' ) {
       *target++ = '\\'; *target++ = 't';
     } else if( *source == '\\' ) {
       *target++ = '\\'; *target++ = '\\';
     } else {
       *target++ = *source;
     }
     ++source;
  }
  *target = 0;

  LngLoadDialect("\\ecs\\lang\\wpamptr.nls", &lng );
  rc = LngGetString(lng, mangled, result, size );
  LngCloseDialect( lng );

  source = result;
  target = result;

  while( *source ) {
    if( source[0] == '\\' ) {
      if( source[1] == 'n' ) {
        *target++ = '\n'; source += 2;
      } else if( source[1] == 'r' ) {
        *target++ = '\r'; source += 2;
      } else if( source[1] == 't' ) {
        *target++ = '\t'; source += 2;
      } else if( source[1] == '\\' ) {
        *target++ = '\\'; source += 2;
      } else {
        *target++ = *source++;
      }
    } else {
      *target++ = *source++;
    }
  }
  *target = 0;

  DEBUGMSG("%s\n", key);
  DEBUGMSG("%s\n", result);

  free( mangled );
  return rc;
}

void translatemenu(HWND handle)
{
  SHORT    item_id;
  SHORT    i;
  SHORT    count = (SHORT)WinSendMsg( handle, MM_QUERYITEMCOUNT, 0, 0 );
  MENUITEM item;

  char current[1024] = "";
  char message[1024];

  for( i = 0; i < count; i++ ) {
    item_id = (SHORT)WinSendMsg( handle, MM_ITEMIDFROMPOSITION, MPFROMSHORT(i), 0 );
    if( item_id != MIT_ERROR ) {
      if( WinSendMsg( handle, MM_QUERYITEMTEXT,
                      MPFROM2SHORT( item_id, sizeof( current )),
                      MPFROMP( current )) > 0 )
      {
        xmessage( message, sizeof( message ), current );
        WinSendMsg( handle, MM_SETITEMTEXT, MPFROMSHORT( item_id ), MPFROMP( message ));
      }

      if( WinSendMsg( handle, MM_QUERYITEM,
                      MPFROM2SHORT( item_id, FALSE ), MPFROMP( &item )))
      {
        if( item.afStyle | MIS_SUBMENU ) {
          translatemenu( item.hwndSubMenu );
        }
      }
    }
  }
}

void translate(HWND handle)
{
  HENUM  henum;
  HWND   hnext;
  char   current[1024] = "";
  char   message[1024];
  char   classname[1024];

  if (handle==NULLHANDLE)
  {
    DEBUGMSG("NULLHANDLE. exiting...", 0);
    return;
  }

  WinQueryClassName(handle, sizeof(classname), (classname));
  DEBUGMSG("Classname=%s\n", classname);
#if 0
if (!strcmp("#4",classname)) // Обработка класса меню
  {
  SHORT    item_id;
  SHORT    i;
  SHORT    count = (SHORT)WinSendMsg( handle, MM_QUERYITEMCOUNT, 0, 0 );
  MENUITEM item;

  char current[1024] = "";
  char message[1024];

  for( i = 0; i < count; i++ ) {
    item_id = (SHORT)WinSendMsg( handle, MM_ITEMIDFROMPOSITION, MPFROMSHORT(i), 0 );
    if( item_id != MIT_ERROR ) {
      if( WinSendMsg( handle, MM_QUERYITEMTEXT,
                      MPFROM2SHORT( item_id, sizeof( current )),
                      MPFROMP( current )) > 0 )
      {
        xmessage( message, sizeof( message ), current );
        WinSendMsg( handle, MM_SETITEMTEXT, MPFROMSHORT( item_id ), MPFROMP( message ));
      }

      if( WinSendMsg( handle, MM_QUERYITEM,
                      MPFROM2SHORT( item_id, FALSE ), MPFROMP( &item )))
      {
        if( item.afStyle | MIS_SUBMENU ) {
          translate( item.hwndSubMenu );
        }
      }
    }
  }
  } else
#endif
  if ((!strcmp("#2",classname))||(!strcmp("SchComboBox",classname))) // Обработка класса WC_COMBOBOX
  {
//    translate(WinWindowFromID(handle,CBID_LIST));
//    translate(WinWindowFromID(handle,CBID_EDIT));
//  } else
//  if (!strcmp("#7",classname)) // Обработка класса WC_LISTBOX
//  {
  SHORT    item_id;
  SHORT    i;
  SHORT    count = (SHORT)WinSendMsg( handle, LM_QUERYITEMCOUNT, 0, 0 );
  MENUITEM item;

  char current[1024] = "";
  char message[1024];

  for( i = 0; i < count; i++ ) {
      if( WinSendMsg( handle, LM_QUERYITEMTEXT,
                      MPFROM2SHORT( i, sizeof( current )),
                      MPFROMP( current )) > 0 )
      {
        xmessage( message, sizeof( message ), current );
        WinSendMsg( handle, LM_SETITEMTEXT, MPFROMSHORT( i ), MPFROMP( message ));
      }
    }
  } else
  if (!strcmp("#37",classname)) // Обработка класса контейнера
  {

  } else
  if (!strcmp("#40",classname)) // Обработка класса блокнота
  {
    ULONG id = (ULONG)WinSendMsg(handle, BKM_QUERYPAGEID, 0, MPFROM2SHORT( BKA_FIRST, 0 ));

  while( id != BOOKERR_INVALID_PARAMETERS && id != 0 )
  {
    char     old_text[1024];
    char     new_text[1024];
    USHORT   text_len;
    BOOKTEXT book_text;
    HWND     hwnd = (HWND)WinSendMsg(handle, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG( id ), 0 );

    if( hwnd != (HWND)BOOKERR_INVALID_PARAMETERS && hwnd != NULLHANDLE ) {
      translate(hwnd);
    }

    book_text.pString = old_text;
    book_text.textLen = sizeof( old_text );
    text_len = (USHORT)WinSendMsg(handle, BKM_QUERYTABTEXT, MPFROMLONG( id ), MPFROMP( &book_text ));

    if( text_len != (USHORT)BOOKERR_INVALID_PARAMETERS &&
        text_len != 0 )
    {
      xmessage( new_text, sizeof( new_text ), old_text );
      WinSendMsg(handle, BKM_SETTABTEXT, MPFROMLONG( id ), MPFROMP( new_text ));
    }

    book_text.pString = old_text;
    book_text.textLen = sizeof( old_text );
    text_len = (USHORT)WinSendMsg(handle, BKM_QUERYSTATUSLINETEXT, MPFROMLONG( id ), MPFROMP( &book_text ));

    if( text_len != (USHORT)BOOKERR_INVALID_PARAMETERS &&
        text_len != 0 )
    {
      xmessage( new_text, sizeof( new_text ), old_text );
      WinSendMsg(handle, BKM_SETSTATUSLINETEXT, MPFROMLONG( id ), MPFROMP( new_text ));
    }

    id = (ULONG)WinSendMsg(handle, BKM_QUERYPAGEID, MPFROMLONG( id ), MPFROM2SHORT( BKA_NEXT, 0 ));
  }
  } else {

  WinQueryWindowText(handle, sizeof( current ), current );
  xmessage( message, sizeof( message ), current );
  WinSetWindowText(handle, message);

  henum = WinBeginEnumWindows(handle);

  while(( hnext = WinGetNextWindow( henum )) != NULLHANDLE )
  {
    translate(hnext);
  }

  WinEndEnumWindows( henum );
  }
}
