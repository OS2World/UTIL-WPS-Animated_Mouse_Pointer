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
#define  INCL_WINWORKPLACE
#define  INCL_DOS
#include <os2.h>

#define NEWLINE "\n"

#include "wpfsys.h"
#include "mptrlset.h"
#include "mptrcnr.h"
#include "wpamptr.rch"
#include "macros.h"
#include "debug.h"

BOOL DisplayHelp( PVOID somSelf, ULONG ulHelpPanelId );

// data structs
typedef struct _LOADSETDATA
{
  M_WPObject* m_somSelf;
  WPObject*   somSelf;
  HMODULE     hmodResource;
  PSZ         pszDirectory;
  CHAR        szSetname[  _MAX_PATH];
  BOOL        fFindSet;
} LOADSETDATA, *PLOADSETDATA;

typedef struct _FILTERDATA
{
  HWND  hwndCnr;
  ULONG ulResourceFileType;
} FILTERDATA, *PFILTERDATA;

// global data
CHAR szFileTypeAll[ MAX_RES_STRLEN];
CHAR szFileTypePointer[ MAX_RES_STRLEN];
CHAR szFileTypeCursor[ MAX_RES_STRLEN];
CHAR szFileTypeWinAnimation[ MAX_RES_STRLEN];
CHAR szFileTypeAnimouseSet[ MAX_RES_STRLEN];
CHAR szFileTypeAnimationSetDir[ MAX_RES_STRLEN];

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : InsertCnrObjects                                           ³
 *³ Kommentar : Sucht Dateien und fgt sie in einen Container ein          ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 14.11.1996                                                 ³
 *³ Žnderung  : 14.11.1996                                                 ³
 *³ aufgerufen: PM System Message Queue                                    ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : ###                                                        ³
 *³ Aufgaben  : - Messages bearbeiten                                      ³
 *³ Rckgabe  : ULONG - Anzahl gefundener Objekte                          ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

ULONG
InsertCnrObjects( M_WPObject* m_somSelf, HWND hwndCnr, PSZ pszDirectory,
                  PSZ pszFilesToSearch, ULONG ulAttr, ULONG ulAnimationFileType )
{
  APIRET          rc = NO_ERROR;
  HOBJECT         hobject;
  WPObject*       pwpobject;
  CHAR            szObjectName[ _MAX_PATH];
  ULONG           ulObjectCount = 0;

  PSZ pszFileName;
  HDIR            hdir = HDIR_CREATE;
  FILEFINDBUF3    ffb3;
  ULONG           ulFileCount;
  BOOL            fFileValid = FALSE;

  PMINIRECORDCORE prec;
  PULONG          pulApplication;

  do
  {
    // Parameter prfen
    if( hwndCnr == NULLHANDLE ) {
      break;
    }

    // search files and insert ito cnr
    strcpy( szObjectName, pszDirectory );
    pszFileName = &szObjectName[ strlen( szObjectName ) + 1];

    strcat( szObjectName, "\\" );
    strcat( szObjectName, pszFilesToSearch );

    // Datei suchen
    ulFileCount = 1;
    rc = DosFindFirst( szObjectName,
                       &hdir,
                       ulAttr,
                       &ffb3,
                       sizeof( ffb3 ),
                       &ulFileCount,
                       FIL_STANDARD );

    // process and search all files
    while( rc == NO_ERROR )
    {
      // append filename to directory path
      strcpy( pszFileName, ffb3.achName );

      // do not use "." and ".."
      fFileValid = TRUE;

      if( ulAttr & FILE_DIRECTORY ) {
        if(( strcmp( pszFileName, "." ) == 0 ) || ( strcmp( pszFileName, "." ) == 0 )) {
          fFileValid = FALSE;
        }
      }

      if( fFileValid ) {
        // Object einfgen
        hobject = WinQueryObject( szObjectName );
        if( hobject != NULL ) {
          pwpobject = _wpclsQueryObject( m_somSelf, hobject );
          if( pwpobject ) {
            POINTL pointlFirstIcon;

            pointlFirstIcon.x = 50;
            pointlFirstIcon.y = 50;

            prec =  _wpCnrInsertObject( pwpobject,
                                        hwndCnr,
                                        &pointlFirstIcon,
                                        NULL,
                                        NULL );
            if( prec != NULL ) {
              // store animation file type within MINIRECORDCORE
              pulApplication = ShlGetUserWordPtr( hwndCnr, prec );
              *pulApplication = ulAnimationFileType;

              // adjust counter
              ulObjectCount++;
            }
          }
        }
      }

      // search next file
      ulFileCount = 1;
      rc = DosFindNext( hdir,
                        &ffb3,
                        sizeof( ffb3 ),
                        &ulFileCount );
    }
  } while( FALSE );

  return ulObjectCount;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : CnrFilter                                                  ³
 *³ Kommentar : Filterfunjktion fr Container                              ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 14.11.1996                                                 ³
 *³ Žnderung  : 14.11.1996                                                 ³
 *³ aufgerufen: Container                                                  ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : PMINIRECORDCORE - Zeiger auf Item-Daten                    ³
 *³             PULONG          - Zeiger auf Typenvariable                 ³
 *³ Aufgaben  : - Filtterbedingung prfen                                  ³
 *³ Rckgabe  : BOOL - weiterhin Anzeige des Items oder nicht              ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL EXPENTRY
CnrFilter( PMINIRECORDCORE prec, PFILTERDATA pfilterdata )
{
  BOOL  fShowItem = FALSE;
  ULONG ulResourceFileType;

  do
  {
    if( pfilterdata == NULL ) {
      break;
    }

    // examine if item to be filtered;
    ulResourceFileType = USERWORD_FROM_PREC( pfilterdata->hwndCnr, prec );
    fShowItem = ( pfilterdata->ulResourceFileType == IDSTR_FILETYPE_ALL ) ?
                TRUE :
                ( pfilterdata->ulResourceFileType == ulResourceFileType );
  } while( FALSE );

  return fShowItem;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : LoadSetDialogProc                                          ³
 *³ Kommentar : Window-Procedure fr den Dialog LoadSet                    ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 14.11.1996                                                 ³
 *³ Žnderung  : 14.11.1996                                                 ³
 *³ aufgerufen: PM System Message Queue                                    ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : HWND   - window handle                                     ³
 *³             ULONG  - message id                                        ³
 *³             MPARAM - message parm 1                                    ³
 *³             MPARAM - message parm 2                                    ³
 *³ Aufgaben  : - Messages bearbeiten                                      ³
 *³ Rckgabe  : MRESULT - Message Result                                   ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

MRESULT EXPENTRY
LoadSetDialogProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg ) {
    case WM_INITDLG:
    {
      HWND         hwndCnr = WinWindowFromID( hwnd, IDDLG_CN_FOUNDSETS );
      ULONG        ulObjectsFound = 0;
      PLOADSETDATA ploadsetdata;

      HAB          hab = WinQueryAnchorBlock( hwnd );
      HMODULE      hmodResource;

      // save pointer to result
      ploadsetdata = mp2;
      WinSetWindowPtr( hwnd, QWL_USER, ploadsetdata );
      hmodResource = ploadsetdata->hmodResource;

      if( !ploadsetdata->fFindSet ) {
        // search OS/2 Pointer
        ulObjectsFound += InsertCnrObjects( ploadsetdata->m_somSelf, hwndCnr,
                                            ploadsetdata->pszDirectory,
                                            "*.ptr",
                                            FILE_ARCHIVED | FILE_READONLY,
                                            IDSTR_FILETYPE_POINTER );

        // search Win Cursor
        ulObjectsFound += InsertCnrObjects( ploadsetdata->m_somSelf, hwndCnr,
                                            ploadsetdata->pszDirectory,
                                            "*.cur",
                                            FILE_ARCHIVED | FILE_READONLY,
                                            IDSTR_FILETYPE_CURSOR );

        // search Win Animation
        ulObjectsFound += InsertCnrObjects( ploadsetdata->m_somSelf, hwndCnr,
                                            ploadsetdata->pszDirectory,
                                            "*.ani",
                                            FILE_ARCHIVED | FILE_READONLY,
                                            IDSTR_FILETYPE_WINANIMATION );
      }

      // search animouse animations
      ulObjectsFound += InsertCnrObjects( ploadsetdata->m_somSelf, hwndCnr,
                                          ploadsetdata->pszDirectory,
                                          "*.and",
                                          FILE_ARCHIVED | FILE_READONLY,
                                          IDSTR_FILETYPE_ANIMOUSESET );

      // search animation set directories
      ulObjectsFound += InsertCnrObjects( ploadsetdata->m_somSelf, hwndCnr,
                                          ploadsetdata->pszDirectory,
                                          "*",
                                          FILE_DIRECTORY | MUST_HAVE_DIRECTORY,
                                          IDSTR_FILETYPE_ANIMATIONSETDIR );

      // report error if nothing found
      if( ulObjectsFound == 0 ) {
        WinDismissDlg( hwnd, MBID_ERROR );
      }

      // initialize combobox and select first item
      LOADSTRING( IDSTR_FILETYPE_ALL, szFileTypeAll );
      if( !ploadsetdata->fFindSet ) {
        LOADSTRING( IDSTR_FILETYPE_POINTER,      szFileTypePointer );
        LOADSTRING( IDSTR_FILETYPE_CURSOR,       szFileTypeCursor );
        LOADSTRING( IDSTR_FILETYPE_WINANIMATION, szFileTypeWinAnimation );
      }

      LOADSTRING( IDSTR_FILETYPE_ANIMOUSESET,     szFileTypeAnimouseSet );
      LOADSTRING( IDSTR_FILETYPE_ANIMATIONSETDIR, szFileTypeAnimationSetDir );

      INSERTITEM(   hwnd, IDDLG_CO_FILTER, szFileTypeAll );
      if( !ploadsetdata->fFindSet ) {
        INSERTITEM(   hwnd, IDDLG_CO_FILTER, szFileTypePointer );
        INSERTITEM(   hwnd, IDDLG_CO_FILTER, szFileTypeCursor );
        INSERTITEM(   hwnd, IDDLG_CO_FILTER, szFileTypeWinAnimation );
      }
      INSERTITEM(   hwnd, IDDLG_CO_FILTER, szFileTypeAnimouseSet );
      INSERTITEM(   hwnd, IDDLG_CO_FILTER, szFileTypeAnimationSetDir );
      SETSELECTION( hwnd, IDDLG_CO_FILTER, 0 );

      translate(hwnd);

      // do not change focus
      return FALSE;
    }

    case WM_CONTROL:
      if( SHORT1FROMMP( mp1 ) == IDDLG_CO_FILTER ) {
        if( SHORT2FROMMP( mp1 ) == LN_SELECT )
        {
          ULONG        ulSelected = QUERYSELECTION( hwnd, IDDLG_CO_FILTER, LIT_FIRST );
          FILTERDATA   filterdata;
          HWND         hwndCnr = WinWindowFromID( hwnd, IDDLG_CN_FOUNDSETS );
          PLOADSETDATA ploadsetdata = WinQueryWindowPtr( hwnd, QWL_USER );

          // set data for filtering
          memset( &filterdata, 0, sizeof( filterdata ));
          filterdata.hwndCnr = hwndCnr;

          if( ploadsetdata->fFindSet ) {
            switch( ulSelected )
            {
              default:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_ALL;
                break;
              case 1:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_ANIMOUSESET;
                break;
              case 2:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_ANIMATIONSETDIR;
                break;
            }
          } else {
            switch( ulSelected )
            {
              default:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_ALL;
                break;
              case 1:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_POINTER;
                break;
              case 2:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_CURSOR;
                break;
              case 3:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_WINANIMATION;
                break;
              case 4:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_ANIMOUSESET;
                break;
              case 5:
                filterdata.ulResourceFileType = IDSTR_FILETYPE_ANIMATIONSETDIR;
                break;
            }
          }

          // now perform filtering
          WinSendMsg( hwndCnr, CM_FILTER, MPFROMP( CnrFilter ), &filterdata );
        }
      }
      break; // end case WM_INITDLG

    case WM_COMMAND:
    {
      switch( SHORT1FROMMP( mp1 ))
      {
        case IDDLG_PB_OK:
        {
          PMINIRECORDCORE precSelected;
          HWND hwndCnr = WinWindowFromID( hwnd, IDDLG_CN_FOUNDSETS );
          PVOID pvObject;
          PLOADSETDATA    ploadsetdata = WinQueryWindowPtr( hwnd, QWL_USER );

          // selektieres Item ermitteln
          precSelected = WinSendMsg( hwndCnr, CM_QUERYRECORDEMPHASIS,
                                     MPFROMP( CMA_FIRST ), MPFROMSHORT( CRA_SELECTED ));
          if( precSelected != NULL ) {
            pvObject = OBJECT_FROM_PREC( precSelected );
            _wpQueryFilename( pvObject, ploadsetdata->szSetname, TRUE );
          }
          break;   // case IDDLG_PB_OK
        }

        case IDDLG_PB_CANCEL:
          break;   // case IDDLG_PB_CANCEL

        case IDDLG_PB_HELP:
        {
          PLOADSETDATA ploadsetdata  = WinQueryWindowPtr( hwnd, QWL_USER );
          PVOID somSelf       = ploadsetdata->somSelf;

          // Hilfe anfordern
          DisplayHelp( somSelf, IDPNL_USAGE_NBPAGE_CNRLOADSET );
          return (MRESULT)TRUE;
        }
      }
      break; // end case WM_COMMAND
    }
  }

  return WinDefDlgProc( hwnd, msg, mp1, mp2 );
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : FindSet                                                    ³
 *³ Kommentar : zeigt einen Dialog mit gefundenen Sets zur Auswahl an      ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 14.11.1996                                                 ³
 *³ Žnderung  : 14.11.1996                                                 ³
 *³ aufgerufen: ###                                                        ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : ###                                                        ³
 *³ Aufgaben  : - Dialog anzeigen                                          ³
 *³ Rckgabe  : APIRET - OS/2 Fehlercode                                   ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

APIRET
FindFiles( PVOID m_wpobject, PVOID wpobject, HWND hwndParent, HWND hwndOwner,
           HMODULE hmodResource, PSZ pszDirectory, PSZ pszFullName,
           ULONG ulMaxLen, BOOL fFindSet )
{
  APIRET      rc = NO_ERROR;
  CHAR        szDirectory[ _MAX_PATH];
  LOADSETDATA loadsetdata;
  ULONG       ulDialogResult;
  HWND        hwndDlg;

  do
  {
    // check parms
    if(( pszDirectory == NULL )                     ||
       ( pszFullName  == NULL )                     ||
       ( strlen( pszFullName ) > ( _MAX_PATH - 1 )) ||
       ( ulMaxLen     <  1 ))
    {
      rc = ERROR_INVALID_PARAMETER;
      break;
    }

    // initialize
    *pszFullName = 0;

    // copy parameter into result buffer
    strcpy( szDirectory, pszDirectory );

    // change ? to boot drive
    if( strncmp( szDirectory, "?:", 2 ) == 0 )
    {
      ULONG ulBootDrive;
      DosQuerySysInfo( QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof( ULONG ));
      szDirectory[0] = (CHAR)ulBootDrive + 'A' - 1;
    }

    memset( &loadsetdata, 0, sizeof( loadsetdata ));
    loadsetdata.pszDirectory  = szDirectory;
    loadsetdata.szSetname[0]  = 0;
    loadsetdata.m_somSelf     = m_wpobject;
    loadsetdata.somSelf       = wpobject;
    loadsetdata.hmodResource  = hmodResource;
    loadsetdata.fFindSet      = fFindSet;

    // process dialog
//    ulDialogResult = WinDlgBox( hwndParent,
//                                hwndOwner,
//                                LoadSetDialogProc,
//                                hmodResource,
//                                IDDLG_DLG_LOADSET,
//                                &loadsetdata );
    hwndDlg = WinLoadDlg(hwndParent,
                         hwndOwner,
                         LoadSetDialogProc,
                         hmodResource,
                         IDDLG_DLG_LOADSET,
                         &loadsetdata);
    ulDialogResult = WinProcessDlg (hwndDlg);
    WinDestroyWindow (hwndDlg);


    switch( ulDialogResult )
    {
      case IDDLG_PB_OK:
        if(( strlen( loadsetdata.szSetname )  + 1 ) > ulMaxLen ) {
          rc = ERROR_MORE_DATA;
        } else {
          strcpy( pszFullName, loadsetdata.szSetname );
        }
        break;

      case MBID_ERROR:
      {
        HAB  hab = WinQueryAnchorBlock( hwndOwner );
        CHAR szTitle[ MAX_RES_STRLEN];
        CHAR szMessage[ MAX_RES_MSGLEN];

        LOADMESSAGE( IDMSG_TITLENOTFOUND, szTitle );
        LOADMESSAGE( IDMSG_MSGNOTFOUND, szMessage );
        WinMessageBox( HWND_DESKTOP, hwndOwner, szMessage, szTitle,
                       IDPNL_USAGE_NBPAGE_CNRSETTINGS, MB_OK /* | MB_HELP */ );
        break;
      }

      default:
        break;
    }
  } while( FALSE );

  return rc;
}

