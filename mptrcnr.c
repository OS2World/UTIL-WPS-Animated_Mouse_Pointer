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

#include "mptrcnr.h"
#include "mptrprop.h"
#include "mptrutil.h"
#include "mptrptr.h"
#include "mptrppl.h"
#include "mptrset.h"
#include "mptranim.h"
#include "wmuser.h"
#include "wpamptr.rch"
#include "macros.h"
#include "debug.h"
#include "nls/amptreng.rch"

// globale Variablen

#ifdef DEBUG
HAB  habMain = NULLHANDLE;
#endif

// resource file types ###
CHAR szFileTypeAll[ MAX_RES_STRLEN];
CHAR szFileTypeDefault[ MAX_RES_STRLEN];
CHAR szFileTypePointer[ MAX_RES_STRLEN];
CHAR szFileTypePointerSet[ MAX_RES_STRLEN];
CHAR szFileTypeCursor[ MAX_RES_STRLEN];
CHAR szFileTypeCursorSet[ MAX_RES_STRLEN];
CHAR szFileTypeAniMouse[ MAX_RES_STRLEN];
CHAR szFileTypeAniMouseSet[ MAX_RES_STRLEN];
CHAR szFileTypeWinAnimation[ MAX_RES_STRLEN];
CHAR szFileTypeWinAnimationSet[ MAX_RES_STRLEN];

static PSZ apszFileType[] = { szFileTypeDefault,
                              szFileTypePointer,
                              szFileTypePointerSet,
                              szFileTypeCursor,
                              szFileTypeCursorSet,
                              szFileTypeAniMouse,
                              szFileTypeAniMouseSet,
                              szFileTypeWinAnimation,
                              szFileTypeWinAnimationSet };

CHAR szPointerArrow[ MAX_RES_STRLEN];
CHAR szPointerText[ MAX_RES_STRLEN];
CHAR szPointerWait[ MAX_RES_STRLEN];
CHAR szPointerSizenwse[ MAX_RES_STRLEN];
CHAR szPointerSizewe[ MAX_RES_STRLEN];
CHAR szPointerMove[ MAX_RES_STRLEN];
CHAR szPointerSizenesw[ MAX_RES_STRLEN];
CHAR szPointerSizens[ MAX_RES_STRLEN];
CHAR szPointerIllegal[ MAX_RES_STRLEN];

PSZ  apszIconTitle[] = { szPointerArrow,
                         szPointerText,
                         szPointerWait,
                         szPointerSizenwse,
                         szPointerSizewe,
                         szPointerMove,
                         szPointerSizenesw,
                         szPointerSizens,
                         szPointerIllegal };

CHAR szTitleIcon[ MAX_RES_STRLEN];
CHAR szTitleName[ MAX_RES_STRLEN];
CHAR szTitleStatus[ MAX_RES_STRLEN];
CHAR szTitleAnimationType[ MAX_RES_STRLEN];
CHAR szTitlePointer[ MAX_RES_STRLEN];
CHAR szTitleAnimationName[ MAX_RES_STRLEN];
CHAR szTitleFrameRate[ MAX_RES_STRLEN];
CHAR szTitleInfoName[ MAX_RES_STRLEN];
CHAR szTitleInfoArtist[ MAX_RES_STRLEN];
CHAR szStatusOn[ MAX_RES_STRLEN];
CHAR szStatusOff[ MAX_RES_STRLEN];

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : InitCnrResources                                           ³
 *³ Kommentar : l„dt Strings aus Resource Lib - muss vor                   ³
 *³             InitPtrSetContainer aufgerufen werden !                    ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 12.06.1996                                                 ³
 *³ Žnderung  : 12.06.1996                                                 ³
 *³ aufgerufen: InitializePointerlist                                      ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : HMODULE - Handle der Resource lib                          ³
 *³ Aufgaben  : - Strings laden                                            ³
 *³ Rckgabe  : -                                                          ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

VOID
InitStringResources( HAB hab, HMODULE hmodResource )
{
  // Strings laden
  LOADSTRING( IDSTR_FILETYPE_ALL,             szFileTypeAll );
  LOADSTRING( IDSTR_FILETYPE_DEFAULT,         szFileTypeDefault );
  LOADSTRING( IDSTR_FILETYPE_POINTER,         szFileTypePointer );
  LOADSTRING( IDSTR_FILETYPE_POINTERSET,      szFileTypePointerSet );
  LOADSTRING( IDSTR_FILETYPE_CURSOR,          szFileTypeCursor );
  LOADSTRING( IDSTR_FILETYPE_CURSORSET,       szFileTypeCursorSet );
  LOADSTRING( IDSTR_FILETYPE_ANIMOUSE,        szFileTypeAniMouse );
  LOADSTRING( IDSTR_FILETYPE_ANIMOUSESET,     szFileTypeAniMouseSet );
  LOADSTRING( IDSTR_FILETYPE_WINANIMATION,    szFileTypeWinAnimation );
  LOADSTRING( IDSTR_FILETYPE_WINANIMATIONSET, szFileTypeWinAnimationSet );
  LOADSTRING( IDSTR_POINTER_ARROW,            szPointerArrow );
  LOADSTRING( IDSTR_POINTER_TEXT,             szPointerText );
  LOADSTRING( IDSTR_POINTER_WAIT,             szPointerWait );
  LOADSTRING( IDSTR_POINTER_SIZENWSE,         szPointerSizenwse );
  LOADSTRING( IDSTR_POINTER_SIZEWE,           szPointerSizewe );
  LOADSTRING( IDSTR_POINTER_MOVE,             szPointerMove );
  LOADSTRING( IDSTR_POINTER_SIZENESW,         szPointerSizenesw );
  LOADSTRING( IDSTR_POINTER_SIZENS,           szPointerSizens );
  LOADSTRING( IDSTR_POINTER_ILLEGAL,          szPointerIllegal );
  LOADSTRING( IDSTR_TITLE_ICON,               szTitleIcon );
  LOADSTRING( IDSTR_TITLE_NAME,               szTitleName );
  LOADSTRING( IDSTR_TITLE_STATUS,             szTitleStatus );
  LOADSTRING( IDSTR_TITLE_ANIMATIONTYPE,      szTitleAnimationType );
  LOADSTRING( IDSTR_TITLE_POINTER,            szTitlePointer );
  LOADSTRING( IDSTR_TITLE_ANIMATIONNAME,      szTitleAnimationName );
  LOADSTRING( IDSTR_TITLE_FRAMERATE,          szTitleFrameRate );
  LOADSTRING( IDSTR_TITLE_INFONAME,           szTitleInfoName );
  LOADSTRING( IDSTR_TITLE_INFOARTIST,         szTitleInfoArtist );
  LOADSTRING( IDSTR_STATUS_ON,                szStatusOn );
  LOADSTRING( IDSTR_STATUS_OFF,               szStatusOff );

  return;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : InitPtrSetContainer                                        ³
 *³ Kommentar : ermittelt Basisname                                        ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 12.06.1996                                                 ³
 *³ Žnderung  : 12.06.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : HWND   - frame window handle                               ³
 *³             PSZ    - Speicherbereich                                   ³
 *³             ULONG  - L„nge des Speicherbereichs                        ³
 *³ Aufgaben  : - Basisname ermitteln                                      ³
 *³ Rckgabe  : BOOL - Flag, ob erfolgreich                                ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

#define SPLITBAR_OFFSET   150
#define CNR_COLUMNS_COUNT 9

BOOL
InitPtrSetContainer( HWND hwnd, PRECORDCORE* ppvCnrData )
{
  BOOL            fSuccess  = FALSE;
  ULONG           i;
  HWND            hwndCnrPointerSet = WinWindowFromID( hwnd, IDDLG_CN_POINTERSET );
  PRECORDCORE     prec = NULL;
  PMYRECORDCORE   pmyrec;
  RECORDINSERT    recinsert;
  PFIELDINFO      pfi, pfiFirst, pfiLastLeftCol;
  FIELDINFOINSERT fii;
  CNRINFO         cnri;
  BOOL            fActivateDemo;

  do
  {
    // Parameter prfen
    if( ppvCnrData == NULL ) {
      break;
    }
    if( hwndCnrPointerSet == NULLHANDLE ) {
      break;
    }

    // Speicher holen
    prec = (PRECORDCORE)WinSendMsg( hwndCnrPointerSet,
                                    CM_ALLOCRECORD,
                                    MPFROMLONG( sizeof( MYRECORDCORE) - sizeof( RECORDCORE)),
                                    MPFROMLONG( NUM_OF_SYSCURSORS ));

    // aktuelle Pointer anzeigen
    for( i = 0, pmyrec = (PMYRECORDCORE)prec;
         i < NUM_OF_SYSCURSORS;
         i++, pmyrec = (PMYRECORDCORE)pmyrec->rec.preccNextRecord )
    {
      pmyrec->rec.flRecordAttr      = CRA_RECORDREADONLY;
      pmyrec->rec.pszIcon           = apszIconTitle[ i];
      pmyrec->rec.hptrIcon          = WinQuerySysPointer( HWND_DESKTOP, QueryPointerSysId( i ), FALSE );
      pmyrec->pszAnimationName      = "";
      pmyrec->pszAnimationType      = "";
      pmyrec->pszFrameRate          = "";
      pmyrec->pszInfoName           = "";
      pmyrec->pszInfoArtist         = "";
      pmyrec->ulPtrCount            = 0;
      pmyrec->pszAnimate            = szStatusOff;
      pmyrec->ulPointerIndex        = i;
      pmyrec->ppl                   = QueryPointerlist( i );
    }

    // Record in Container einstellen
    recinsert.cb                 = sizeof( RECORDINSERT );
    recinsert.pRecordOrder       = (PRECORDCORE)CMA_END;
    recinsert.pRecordParent      = NULL;
    recinsert.fInvalidateRecord  = TRUE;
    recinsert.zOrder             = CMA_TOP;
    recinsert.cRecordsInsert     = NUM_OF_SYSCURSORS;

    if( !WinSendMsg( hwndCnrPointerSet,
                     CM_INSERTRECORD,
                     MPFROMP( prec ),
                     MPFROMP( &recinsert )))
    {
      break;
    }

    // jetzt Detailsview konfigurieren
    // Speicher holen
    if(( pfi = WinSendMsg( hwndCnrPointerSet,
                           CM_ALLOCDETAILFIELDINFO,
                           MPFROMLONG( CNR_COLUMNS_COUNT ),
                           0L )) == NULL )
    {
      break;
    }

    pfiFirst = pfi; // Zeiger auf erste Struktur festhalten
    // Details fr Symbol
    pfi->flData     = CFA_BITMAPORICON | CFA_HORZSEPARATOR | CFA_SEPARATOR;
    pfi->flTitle    = CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitleIcon;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, rec.hptrIcon );

    // Titel
    pfi             = pfi->pNextFieldInfo;
    pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR;
    pfi->flTitle    = CFA_CENTER | CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitleName;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, rec.pszIcon );

    pfiLastLeftCol = pfi; // letzte linke Spalte festhalten

    // Status
    pfi             = pfi->pNextFieldInfo;
    pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
    pfi->flTitle    = CFA_CENTER | CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitleStatus;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, pszAnimate );

    // Animationstyp
    pfi             = pfi->pNextFieldInfo;
    pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
    pfi->flTitle    = CFA_CENTER | CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitleAnimationType;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, pszAnimationType );

    // Anzahl Pointer
    pfi             = pfi->pNextFieldInfo;
    pfi->flData     = CFA_ULONG  | CFA_RIGHT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
    pfi->flTitle    = CFA_CENTER | CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitlePointer;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, ulPtrCount );

    // Animationsname
    pfi             = pfi->pNextFieldInfo;
    pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
    pfi->flTitle    = CFA_CENTER | CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitleAnimationName;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, pszAnimationName );

    // Timeouts
    pfi             = pfi->pNextFieldInfo;
    pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
    pfi->flTitle    = CFA_CENTER | CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitleFrameRate;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, pszFrameRate );

    // Info animation name
    pfi             = pfi->pNextFieldInfo;
    pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
    pfi->flTitle    = CFA_CENTER | CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitleInfoName;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, pszInfoName );

    // Info animation artist
    pfi             = pfi->pNextFieldInfo;
    pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR;
    pfi->flTitle    = CFA_CENTER | CFA_FITITLEREADONLY;
    pfi->pTitleData = szTitleInfoArtist;
    pfi->offStruct  = FIELDOFFSET( MYRECORDCORE, pszInfoArtist );

    // Struktur an Container senden
    memset( &fii, 0, sizeof( FIELDINFOINSERT ));
    fii.cb                   = sizeof( FIELDINFOINSERT );
    fii.pFieldInfoOrder      = (PFIELDINFO)CMA_END;
    fii.cFieldInfoInsert     = (SHORT)CNR_COLUMNS_COUNT;
    fii.fInvalidateFieldInfo = TRUE;

    if( WinSendMsg( hwndCnrPointerSet,
                    CM_INSERTDETAILFIELDINFO,
                    MPFROMP( pfiFirst ),
                    MPFROMP( &fii )) != (MRESULT)CNR_COLUMNS_COUNT )
    {
      break;
    }

    // Splitbar setzen
    memset( &cnri, 0, sizeof( CNRINFO ));
    cnri.cb             = sizeof( CNRINFO );
    cnri.pFieldInfoLast = pfiLastLeftCol;
    cnri.xVertSplitbar  = SPLITBAR_OFFSET;

    if( !WinSendMsg( hwndCnrPointerSet,
                     CM_SETCNRINFO,
                     MPFROMP( &cnri ),
                     MPFROMLONG( CMA_PFIELDINFOLAST | CMA_XVERTSPLITBAR )))
    {
      break;
    }

    // fr Setzen der Controls sorgen
    WinPostMsg( hwnd, WM_USER_ENABLECONTROLS, 0, 0 );

    // ggfs. Demo anstellen
    fActivateDemo = QueryDemo();
    if( fActivateDemo ) {
      ToggleDemo( hwnd, prec, FALSE, &fActivateDemo );
    }

    // aktuellen Stand aus Pointerlisten einstellen
    RefreshCnrItem( hwnd, NULL, prec, TRUE );

    // fertig
    fSuccess = TRUE;
  } while( FALSE );

  // Struktur zurckgeben
  if( fSuccess ) {
    *ppvCnrData = prec;
  } else if( prec ) {
    WinSendMsg( hwndCnrPointerSet,
                CM_ALLOCRECORD,
                MPFROMP( prec ),
                MPFROMLONG( NUM_OF_SYSCURSORS ));
  }

  return fSuccess;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : RefreshCnrItem                                             ³
 *³ Kommentar : fhrt einen Refresh fr ein Item durch                     ³
 *³             Setzt ausserdem alle Mauszeiger auf den statischen         ³
 *³             oder auf den Default-Zeiger zurck.                        ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 24.07.1996                                                 ³
 *³ Žnderung  : 24.07.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : HWND        - frame window                                 ³
 *³             PRECORDCORE - Zeiger auf Containeritem oder NULL           ³
 *³             PRECORDCORE - Zeiger auf Gesamt-Record                     ³
 *³             BOOL        - Flag, ob Arrow-Pointer refreshed werden soll ³
 *³ Aufgaben  : - Refresh durchfhren                                      ³
 *³ Rckgabe  : BOOL - Flag, ob erfolgreich                                ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL
RefreshCnrItem( HWND hwnd, PRECORDCORE prec, PRECORDCORE pcnrrec, BOOL fResetArrowPtr )
{
  BOOL          fSuccess = FALSE;
  HWND          hwndCnrPointerSet = WinWindowFromID( hwnd, IDDLG_CN_POINTERSET );
  PMYRECORDCORE pmyrec = (PMYRECORDCORE)prec;
  PICONINFO     piconinfo;
  ULONG         i;
  PPOINTERLIST  ppl;
  HPOINTER      hptrItem;

  do {
    // ersten Pointer der jeweiligen Pointerliste setzen
    for( i = 0; i < NUM_OF_SYSCURSORS; i++ )
    {
      ppl = QueryPointerlist( i );

      if( ppl->ulPtrCount > 0 ) {
        // Pointer auf n„chsten Wert setzen
        // SYS-Pointer setzen
        if( ppl->hptrStatic == 0 ) {
          // Default setzen
          fSuccess = WinSetSysPointerData( HWND_DESKTOP, QueryPointerSysId( i ), NULL );

          // Arrow Ptr aktualisieren
          if(( i == 0 ) && ( fResetArrowPtr )) {
            HPOINTER hptrOrg = WinQuerySysPointer( HWND_DESKTOP, QueryPointerSysId( i ), TRUE );
            WinSetPointer( HWND_DESKTOP, hptrOrg );
            WinDestroyPointer( hptrOrg );
          }
        } else {
          // neuen Pointer setzen
          if( ppl->fAnimate ) {
            piconinfo = &ppl->iconinfo[0];
          } else {
            piconinfo = &ppl->iconinfoStatic;
          }
          fSuccess = WinSetSysPointerData( HWND_DESKTOP,
                                           QueryPointerSysId( i ),
                                           piconinfo );
          // Arrow Ptr aktualisieren
          if(( i == 0 ) && ( fResetArrowPtr )) {
            WinSetPointer( HWND_DESKTOP, ppl->hptrStatic );
          }
        }
      }
    }

    // Der Rest wird nur gebraucht, wenn der Frame und der Container da ist
    if(( hwnd == NULLHANDLE ) || ( pcnrrec == NULL )) {
      fSuccess = TRUE;
      break;
    }

    // ersten Pointer der Liste setzen
    // ### noch zu optimieren: ggfs. nur betroffenes Objekt updaten
    for( i = 0, pmyrec = (PMYRECORDCORE)pcnrrec;
         i < NUM_OF_SYSCURSORS;
         i++, pmyrec = (PMYRECORDCORE)pmyrec->rec.preccNextRecord )
    {
      ULONG ulMaxLen;
      PSZ   pszList;
      ULONG s;

      ppl = QueryPointerlist( i );

      // alte Resourcen freigeben
      if( strlen( pmyrec->pszFrameRate ) > 0 ) {
        free( pmyrec->pszFrameRate );
      }

      // Container Item neu einstellen
      if( ppl->ulPtrCount > 0 ) {
        if( ppl->ulPtrCount == 1 ) {
          pszList = "";
        } else {
          // framerates ermitteln
          ulMaxLen = ( ppl->ulPtrCount * 7 ) + 1;  // "(nnnn) "
          pszList = malloc( ulMaxLen );

          if( pszList == NULL ) {
            pszList = "???";
          } else {
            BOOL fOverrideTimeout = getOverrideTimeout();
            memset( pszList, 0, ulMaxLen );

            for( s = 0; s < ppl->ulPtrCount; s++ )
            {
              if(( fOverrideTimeout ) || ( ppl->aulTimer[ s] == 0 ) || ( ppl->aulTimer[ s] == 0 )) {
                sprintf( ENDSTRING( pszList ), "(%u) ", getDefaultTimeout());
              } else {
                sprintf( ENDSTRING( pszList ), "%u ", ppl->aulTimer[ s] );
              }
            }
          }
        }

        // Werte fr Container-Item neu einstellen
        pmyrec->pszAnimationName = ppl->szAnimationName;
        pmyrec->pszAnimationType = apszFileType[ppl->ulResourceType];
        pmyrec->pszFrameRate     = pszList;
        pmyrec->pszInfoName      = ppl->pszInfoName;
        pmyrec->pszInfoArtist    = ppl->pszInfoArtist;
        pmyrec->ulPtrCount       = ppl->ulPtrCount;
        pmyrec->pszAnimate       = ( ppl->fAnimate ) ? szStatusOn : szStatusOff;
      } else {
        // Werte fr Container-Item neu einstellen
        pmyrec->pszAnimationName = "";
        pmyrec->pszAnimationType = "";
        pmyrec->pszFrameRate     = "";
        pmyrec->ulPtrCount       = 0;
        pmyrec->pszInfoName      = "";
        pmyrec->pszInfoArtist    = "";
        pmyrec->pszAnimate       = szStatusOff;
      }
    }

    // ersten Pointer der Liste setzen
    // optimieren: ggfs. nur betroffenes Objekt updaten
    for( i = 0, pmyrec = (PMYRECORDCORE)pcnrrec;
         i < NUM_OF_SYSCURSORS;
         i++, pmyrec = (PMYRECORDCORE)pmyrec->rec.preccNextRecord )
    {
      // Pointer fr Item ermitteln
      ppl = QueryPointerlist( i );
      if( ppl->hptr[ppl->ulPtrIndexCnr] == 0 ) {
        hptrItem = WinQuerySysPointer( HWND_DESKTOP, QueryPointerSysId( i ), FALSE );
      } else {
        if( ppl->fAnimate ) {
          hptrItem = ppl->hptr[ppl->ulPtrIndexCnr];
        } else {
          hptrItem = ppl->hptrStatic;
        }
      }

      pmyrec->rec.hptrIcon = hptrItem;
    }

    if( prec ) {
      WinSendMsg( hwndCnrPointerSet, CM_INVALIDATERECORD,
                  MPFROMP( &prec ),
                  MPFROM2SHORT( 1, CMA_NOREPOSITION | CMA_ERASE ));
    } else {
      WinSendMsg( hwndCnrPointerSet, CM_INVALIDATERECORD,
                  MPFROMP( NULL ),
                  MPFROM2SHORT( 0, CMA_NOREPOSITION ));
    }

    // alles ok
    fSuccess =  TRUE;

    // fr Setzen der Controls sorgen
    WinPostMsg( hwnd, WM_USER_ENABLECONTROLS, 0L, 0L );
  } while( FALSE );

  return fSuccess;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : SetContainerView                                           ³
 *³ Kommentar :                                                            ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 24.07.1996                                                 ³
 *³ Žnderung  : 24.07.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : HWND   - frame window handle                               ³
 *³             ULONG  - neuer View des Containers                         ³
 *³ Aufgaben  : - View „ndern                                              ³
 *³ Rckgabe  : BOOL - Flag, ob erfolgreich                                ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL
SetContainerView( HWND hwnd, ULONG ulViewStyle )
{
  BOOL    fSuccess  = FALSE;
  CNRINFO cnri;
  HWND    hwndCnrPointerSet = WinWindowFromID( hwnd, IDDLG_CN_POINTERSET );

  do
  {
    // Parameter prfen
    if(( ulViewStyle != CV_ICON ) &&
       ( ulViewStyle != CV_DETAIL ))
    {
      break;
    }

    // Struktur holen
    if((ULONG)WinSendMsg( hwndCnrPointerSet,
                          CM_QUERYCNRINFO,
                          MPFROMP( &cnri ),
                          MPFROMLONG( sizeof( CNRINFO ))) != sizeof( CNRINFO )) {
      break;
    }

    // zus„tzliche Einstellungen
    switch( ulViewStyle )
    {
      case CV_ICON:
        cnri.flWindowAttr = CA_DRAWICON | CA_DETAILSVIEWTITLES | CV_ICON;
        break;

      case CV_DETAIL:
        cnri.flWindowAttr = CA_DRAWICON | CA_DETAILSVIEWTITLES | CV_DETAIL | CV_MINI;
        break;
    }

    if( !WinSendMsg( hwndCnrPointerSet,
                     CM_SETCNRINFO,
                     MPFROMP( &cnri ),
                     MPFROMLONG( CMA_FLWINDOWATTR )))
    {
      break;
    }

    // alles ok
    fSuccess = TRUE;
  } while( FALSE );

  return fSuccess;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : QueryContainerView                                         ³
 *³ Kommentar :                                                            ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 24.07.1996                                                 ³
 *³ Žnderung  : 24.07.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : HWND   - frame window handle                               ³
 *³             PULONG - Variable fr aktuellen View                       ³
 *³ Aufgaben  : - View holen                                               ³
 *³ Rckgabe  : BOOL - Flag, ob erfolgreich                                ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL
QueryContainerView( HWND hwnd, PULONG pulViewStyle )
{
  BOOL    fSuccess  = FALSE;
  CNRINFO cnri;
  HWND    hwndCnrPointerSet = WinWindowFromID( hwnd, IDDLG_CN_POINTERSET );

  do
  {
    // Parameter prfen
    if( pulViewStyle == NULL ) {
      break;
    }

    // Struktur holen
    if((ULONG)WinSendMsg( hwndCnrPointerSet,
                          CM_QUERYCNRINFO,
                          MPFROMP( &cnri ),
                          MPFROMLONG( sizeof( CNRINFO ))) != sizeof( CNRINFO ))
    {
      break;
    }

    *pulViewStyle = cnri.flWindowAttr;

    // alles ok
    fSuccess = TRUE;
  } while( FALSE );

  return fSuccess;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : QueryItemSet                                               ³
 *³ Kommentar : fragt ab, ob eine Animation geladen wurde                  ³
 *³             Wenn eine Animation geladen ist, wird TRUE zurckgegeben   ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 24.07.1996                                                 ³
 *³ Žnderung  : 24.07.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : PRECORDCORE - Zeiger auf Containeritem oder NULL           ³
 *³ Aufgaben  : - Set Abfrage                                              ³
 *³ Rckgabe  : BOOL - Flag, ob ein Set geladen wurde                      ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL
QueryItemSet( PRECORDCORE prec )
{
  PMYRECORDCORE pmyrec = (PMYRECORDCORE)prec;
  PPOINTERLIST  ppl;

  ULONG  i;
  BOOL   fIsSet   = FALSE;
  ULONG  ulPointerIndex = ( pmyrec ) ? pmyrec->ulPointerIndex : 0;
  BOOL   fLoadSet = ( prec == NULL );
  ULONG  ulFirstPtr, ulLastPtr;

  do
  {
    // range fr Verarbeitung festlegen
    ulFirstPtr = ulPointerIndex;

    if( fLoadSet ) {
      ulLastPtr  = NUM_OF_SYSCURSORS;
    } else {
      ulLastPtr  = ulPointerIndex + 1;
    }

    // entsprechende Pointerlisten berprfen
    for( i = ulFirstPtr, ppl = QueryPointerlist( ulPointerIndex );
         i < ulLastPtr;
         i++, ppl++ )
    {
      // ist es ein Pointerset ?
      if( ppl->ulPtrCount > 1 ) {
        fIsSet = TRUE;
      }
    }
  } while( FALSE );

  return fIsSet;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : QueryItemLoaded                                            ³
 *³ Kommentar : fragt ab, ob mindestens ein Zeigerbild geladen wurde       ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 24.07.1996                                                 ³
 *³ Žnderung  : 24.07.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : PRECORDCORE - Zeiger auf Containeritem                     ³
 *³ Aufgaben  : - Pointerabfrage                                           ³
 *³ Rckgabe  : BOOL - Flag, ob mindestens ein Zeigerbild geladen wurde    ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL
QueryItemLoaded( PRECORDCORE prec )
{
  PMYRECORDCORE pmyrec = (PMYRECORDCORE)prec;
  PPOINTERLIST  ppl;

  ULONG  ulPointerIndex = ( pmyrec ) ? pmyrec->ulPointerIndex : 0;
  BOOL   fItemLoaded = FALSE;
  BOOL   fLoadSet = ( prec == NULL );
  ULONG  ulFirstPtr, ulLastPtr;
  ULONG  i;

  do
  {
    // range fr Verarbeitung festlegen
    ulFirstPtr = ulPointerIndex;

    if( fLoadSet ) {
      ulLastPtr  = NUM_OF_SYSCURSORS;
    } else {
      ulLastPtr  = ulPointerIndex + 1;
    }

    // entsprechende Pointerlisten berprfen
    for( i = ulFirstPtr, ppl = QueryPointerlist( ulPointerIndex );
         i < ulLastPtr;
         i++, ppl++ )
    {
      // ist es ein Pointerset ?
      if( ppl->hptrStatic != NULL ) {
        fItemLoaded = TRUE;
      }
    }
  } while( FALSE );

  return fItemLoaded;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : QueryItemAnimate                                           ³
 *³ Kommentar : fragt das Animation Flag fr einen oder alle Ptr ab        ³
 *³             Wenn eine Animation aktiv ist, wird TRUE zurckgegeben     ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 24.07.1996                                                 ³
 *³ Žnderung  : 24.07.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : PRECORDCORE - Zeiger auf Containeritem oder NULL           ³
 *³ Aufgaben  : - Animate abfragen                                         ³
 *³ Rckgabe  : BOOL - Flag, ob eine Animation aktiv ist                   ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL
QueryItemAnimate( PRECORDCORE prec )
{
  PMYRECORDCORE pmyrec = (PMYRECORDCORE)prec;
  ULONG ulPointerIndex = ( pmyrec ) ? pmyrec->ulPointerIndex : 0;
  BOOL  fQueryAll = ( prec == NULL );

  return QueryAnimate( ulPointerIndex, fQueryAll );
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : ToggleAnimate                                              ³
 *³ Kommentar : setzt das Animation Flag fr einen oder alle Ptr um        ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 24.07.1996                                                 ³
 *³ Žnderung  : 24.07.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : HWND        - frame window                                 ³
 *³             ULONG       - PointerIndex, falls Item = NULL              ³
 *³             PRECORDCORE - Zeiger auf Containeritem oder NULL           ³
 *³             PRECORDCORE - Zeiger auf Gesamt-Record                     ³
 *³             BOOL        - Refreshflag                                  ³
 *³             PBOOL       - Zeiger auf ForceFlag                         ³
 *³ Aufgaben  : - Animate umstellen                                        ³
 *³ Rckgabe  : BOOL - Flag, ob erfolgreich                                ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL
ToggleAnimate( HWND hwnd, ULONG ulPtrIndex,
               PRECORDCORE prec, PRECORDCORE pcnrrec, BOOL fChangeAll, BOOL fRefresh, PBOOL pfEnable )
{
  BOOL   fSuccess = FALSE;
  ULONG  i;
  BOOL   fAnimate;

  PMYRECORDCORE pmyrec = (PMYRECORDCORE)prec;
  PPOINTERLIST ppl, pplndx;
  ULONG  ulPointerIndex = ( pmyrec ) ? pmyrec->ulPointerIndex : ulPtrIndex;

  ULONG  ulFirstPtr, ulLastPtr;

  do {
    // ggfs. Flag overrulen
    if( pfEnable ) {
      fAnimate = !( *pfEnable );
    }                          // wird sp„ter nochmal negiert !
    else {
      fAnimate = QueryItemAnimate( prec );
    }

    // range fr Verarbeitung festlegen
    ulFirstPtr = ulPointerIndex;

    if( fChangeAll ) {
      ulFirstPtr = 0;
      ulLastPtr  = NUM_OF_SYSCURSORS;
    } else {
      ulLastPtr  = ulPointerIndex + 1;
    }

    ppl = QueryPointerlist( ulPointerIndex );

    for( i = ulFirstPtr, pplndx = ppl; i < ulLastPtr; i++, pplndx++ )
    {
      // alle m”glichen Animationen ein- bzw. Ausschalten
      if( pplndx->ulPtrCount > 1 ) {
        EnableAnimation( pplndx, !fAnimate );
      }
    }

    // Refresh durchfhren
    if(( pcnrrec ) && ( fRefresh )) {
      RefreshCnrItem( hwnd, prec, pcnrrec, FALSE );
    }

    // alles ok
    fSuccess = TRUE;
  } while( FALSE );

  return fSuccess;
}


/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : LoadAnimationResource                                      ³
 *³ Kommentar : l„dt Resourcen fr einen Pointer oder fr alle Pointer     ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 20.07.1996                                                 ³
 *³ Žnderung  : 20.07.1996                                                 ³
 *³ aufgerufen: Window Procedure                                           ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : HWND        - frame window                                 ³
 *³             PSZ         - Name  Pointerdatei / Verzeichnis             ³
 *³             PRECORDCORE - Zeiger auf Containeritem oder NULL           ³
 *³             PRECORDCORE - Zeiger auf Gesamt-Record                     ³
 *³ Aufgaben  : - Basisname ermitteln                                      ³
 *³ Rckgabe  : BOOL - Flag, ob erfolgreich                                ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL
LoadAnimationResource( HWND hwnd, PSZ pszName, PRECORDCORE prec, PRECORDCORE pcnrrec )
{
  PMYRECORDCORE pmyrec = (PMYRECORDCORE)prec;
  ULONG  ulPointerIndex = ( pmyrec ) ? pmyrec->ulPointerIndex : 0;
  BOOL   fSuccess = FALSE;

  do {
#ifdef DEBUG
    habMain = WinQueryAnchorBlock( hwnd );
#endif

    // Parameter prfen
    if( pcnrrec     == NULL ) {
      break;
    }

    // Animation laden - Fehler ignorieren
    LoadPointerAnimation( ulPointerIndex,
                          QueryPointerlist( ulPointerIndex ),
                          pszName,
                          ( prec == NULL ),
                          TRUE,
                          TRUE );

    // Refresh durchfhren
    RefreshCnrItem( hwnd, prec, pcnrrec, TRUE );

    // alles ok
    fSuccess =  TRUE;
  } while( FALSE );

  return fSuccess;
}

