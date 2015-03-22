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

#include "mptredit.h"
#include "mptrcnr.h"
#include "wmuser.h"
#include "wpamptr.rch"
#include "macros.h"
#include "debug.h"
#include "pointer.h"
#include "nls/amptreng.rch"

// Daten f�r Editierfunktion
static BOOL fEditPending = FALSE;
static CHAR szEditFilename [ _MAX_PATH];
static HAPP happEdit;

/*������������������������������������������������������������������������Ŀ
 *� Name      : QueryEditPending                                           �
 *��������������������������������������������������������������������������
 */

BOOL QueryEditPending() {
  return fEditPending;
}

/*������������������������������������������������������������������������Ŀ
 *� Name      : BeginEditPointer                                           �
 *� Kommentar : Pointer in tempor�re Datei schreiben und in den Icon Editor�
 *�             laden. Frame wird �ber WM_APPTERMINATENOTIFY notifiziert,  �
 *�             wenn der Editvorgang beendet ist.                          �
 *� Autor     : C.Langanke                                                 �
 *� Datum     : 24.07.1996                                                 �
 *� �nderung  : 24.07.1996                                                 �
 *� aufgerufen: diverse                                                    �
 *� ruft auf  : -                                                          �
 *� Eingabe   : ###                                                        �
 *� Aufgaben  : - Datei erzeugen und anlegen                               �
 *� R�ckgabe  : BOOL - Flag, ob erfolgreich                                �
 *��������������������������������������������������������������������������
 */

BOOL
BeginEditPointer( HWND hwnd, HMODULE hmodResource, PPOINTERLIST ppl )
{
  BOOL        fSuccess = FALSE;
  APIRET      rc       = NO_ERROR;
  HFILE       hfileTmp = NULLHANDLE;
  ULONG       ulBytesWritten;

  ULONG       ulPtrId;
  PROGDETAILS progdetails;
  CHAR        szPrgFullName [ _MAX_PATH];

  PICONINFO   piconinfo;
  ICONINFO    iconinfo;

  FILESTATUS3 fs3;

  do
  {
    // Pointerliste ok ?
    if( !ppl ) {
      break;
    }

    // merhmaliges Edit verhindern (besser: mit sempahore !)
    if( fEditPending ) {
      break;
    } else {
      fEditPending = TRUE;
    }

    // check for single pointer or sytem default
    if( ppl->ulPtrCount < 2 ) {
      if( ppl->ulPtrCount == 1 ) {
        piconinfo = &ppl->iconinfo[ 0];
      } else {
        // System ID des Pointers ermitteln
        ulPtrId = ppl->ulPtrId;

        // Pointerdaten-L�nge und dann Pointerdaten holen
        piconinfo = &iconinfo;
        memset( &iconinfo, 0, sizeof( ICONINFO ));
        iconinfo.cb           = sizeof( ICONINFO );
        iconinfo.fFormat      = ICON_DATA;
        if( !WinQuerySysPointerData( HWND_DESKTOP, ulPtrId, piconinfo )) {
          rc = ERRORIDERROR( WinGetLastError( WinQueryAnchorBlock( hwnd )));
          break;
        }
        if(( piconinfo->pIconData = malloc( piconinfo->cbIconData )) == NULL ) {
          break;
        } else
        if( !WinQuerySysPointerData( HWND_DESKTOP, ulPtrId, piconinfo )) {
          break;
        }
      }

      // tempor�re Datei erstellen
      rc = OpenTmpFile( "WP!!", "PTR", &hfileTmp, szEditFilename, sizeof( szEditFilename ));
      if( rc != NO_ERROR ) {
        break;
      }

      // Daten in Datei schreiben
      rc = DosWrite( hfileTmp,
                     piconinfo->pIconData,
                     piconinfo->cbIconData,
                     &ulBytesWritten );
      if( rc != NO_ERROR ) {
        break;
      }

      // Datei schlie�en
      rc = DosClose( hfileTmp );

      // Archiv Attribut dediziert l�schen, da DosOpen dies nicht tut
      rc = DosQueryPathInfo( szEditFilename, FIL_STANDARD, &fs3, sizeof( fs3 ));
      if( rc != NO_ERROR ) {
        break;
      }
      fs3.attrFile = 0;
      rc = DosSetPathInfo( szEditFilename, FIL_STANDARD, &fs3, sizeof( fs3 ), DSPI_WRTTHRU );
      if( rc != NO_ERROR ) {
        break;
      }

      // icon editor suchen
      rc = DosSearchPath( SEARCH_CUR_DIRECTORY |
                          SEARCH_ENVIRONMENT   |
                          SEARCH_IGNORENETERRS,
                          "PATH",
                          "ICONEDIT.EXE",
                          (PBYTE)szPrgFullName,
                          sizeof( szPrgFullName ));
      if( rc != NO_ERROR ) {
        break;
      }

      // jetzt icon editor starten
      memset( &progdetails, 0, sizeof( PROGDETAILS ));
      progdetails.Length         = sizeof( PROGDETAILS );
      progdetails.progt.progc    = PROG_PM;
      progdetails.progt.progc    = SHE_VISIBLE;
      progdetails.pszExecutable  = szPrgFullName;
      progdetails.pszParameters  = szEditFilename;

      happEdit = WinStartApp( hwnd, &progdetails, szEditFilename, NULL, 0 );
      if( happEdit == NULLHANDLE ) {
        break;
      }
      // alles ok
      fSuccess = TRUE;
    }
  } while( FALSE );

  // aufr�umen
  if( hfileTmp ) {
    DosClose( hfileTmp );
    if( !fSuccess ) {
      DosDelete( szEditFilename );
    }
  }

  if( !fSuccess ) {
    fEditPending = FALSE;
  }

  return fSuccess;
}

/*������������������������������������������������������������������������Ŀ
 *� Name      : EndEditPointer                                             �
 *� Kommentar : Zuvor editierten pointer als                               �
 *�             laden. Frame wird �ber WM_APPTERMINATENOTIFY notifiziert,  �
 *�             wenn der Editvorgang beendet ist.                          �
 *� Autor     : C.Langanke                                                 �
 *� Datum     : 24.07.1996                                                 �
 *� �nderung  : 24.07.1996                                                 �
 *� aufgerufen: diverse                                                    �
 *� ruft auf  : -                                                          �
 *� Eingabe   : ###                                                        �
 *� Aufgaben  : - Datei erzeugen und anlegen                               �
 *� R�ckgabe  : BOOL - Flag, ob erfolgreich                                �
 *��������������������������������������������������������������������������
 */

BOOL
EndEditPointer( HWND hwnd, HAPP happ, ULONG ulReturncode, PPOINTERLIST ppl )
{
  BOOL        fSuccess = FALSE;
  APIRET      rc;
  ICONINFO    iconinfo;
  ULONG       ulPtrId;
  FILESTATUS3 fs3;
  BOOL        fFileChanged = FALSE;
  HPOINTER    hptrTmp = NULLHANDLE;

  do
  {
    // wird gerade editiert ?
    if( !fEditPending ) {
      break;
    }

    // ist es unser handle
    if( happ != happEdit ) {
      break;
    }

    // Pointerliste ok ?
    if( !ppl ) {
      break;
    }

    // ab jetzt keinen Fehler mehr geben !
    fSuccess = TRUE;

    // bei Fehler abbrechen
    if( ulReturncode != NO_ERROR ) {
      break;
    }

    // -------------------------------------------------------------------------------------

    // check for single pointer or sytem default
    if( ppl->ulPtrCount < 2 ) {
      // Dateiinfo holen
      rc = DosQueryPathInfo( szEditFilename, FIL_STANDARD, &fs3, sizeof( fs3 ));
      if( rc != NO_ERROR ) {
        break;
      }

      // pr�fen, ob die Datei sich ver�ndert hat
      fFileChanged = (( fs3.attrFile & FILE_ARCHIVED ) == FILE_ARCHIVED );
      if( !fFileChanged ) {
        break;
      }

      // System ID des Pointers ermitteln
      ulPtrId = ppl->ulPtrId;

      // Pointer laden
      if( !LoadPointerFromPointerFile( szEditFilename, &hptrTmp, &iconinfo, NULL )) {
        break;
      }

      // Pointer setzen
      CopyPointerlist( NULL, ppl, FALSE );
      ppl->hptrStatic = hptrTmp;
      ppl->ulPtrCount = 0;
      memcpy( &ppl->iconinfoStatic, &iconinfo, sizeof( ICONINFO ));

      WinSetSysPointerData( HWND_DESKTOP, ulPtrId, &iconinfo );
      if( ulPtrId  == SPTR_ARROW ) {
        WinSetPointer( HWND_DESKTOP, hptrTmp );
      }
    }
  } while( FALSE );

  // cleanup
  fEditPending = FALSE;
  DosDelete( szEditFilename );

  return fSuccess;
}

