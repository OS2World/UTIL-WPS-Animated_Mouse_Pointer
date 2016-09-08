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

#include "title.h"
#include "mptranim.h"
#include "mptrset.h"
#include "mptrppl.h"
#include "mptrptr.h"
#include "mptrcnr.h"
#include "mptrhook.h"
#include "wmuser.h"
#include "macros.h"
#include "debug.h"

// Timer IDs fr die 2 Timer Variante
#define ANIMATION_TIMER_ID   256
#define WATCHDOG_TIMER_ID    257
#define WATCHDOG_TIMER_TIMEOUT DEFAULT_ANIMATION_TIMEOUT
#define HIDEPOINTER_TIMER_ID 258

// MOUSEMOVED_TIMER_ID in mptrhook.h definiert

#define ANIMATION_OBJECT_CLASS   "wpamptr_animate"
#define ANIMATION_ACCESS_TIMEOUT 100L

// global data
static HAB  habAnimationThread = NULLHANDLE;
static HWND hwndAnimation = NULLHANDLE;

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : IsDragPending                                              ³
 *³ Comentary : prft, ob Drag&Drop aktiv ist. Lazy Drag wird ignoriert.   ³
 *³ Author    : C.Langanke                                                 ³
 *³ Date      : 10.02.1997                                                 ³
 *³ Žnderung  : 10.02.1997                                                 ³
 *³ Call      : diverse                                                    ³
 *³ Calls on  : -                                                          ³
 *³ Input     : -                                                          ³
 *³ Tasks     : - Variable zurckgeben                                     ³
 *³ Return    : BOOL - Drag in Process                                     ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

BOOL IsDragPending( void )
{
  BOOL  fDragPending = FALSE;
  ULONG ulKeyState = WinGetKeyState( HWND_DESKTOP, VK_BUTTON2 );

  if(( ulKeyState & 0x8000 ) > 0 ) {
    fDragPending = ( DrgQueryDragStatus() == DGS_DRAGINPROGRESS );
  }

  return fDragPending;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : QueryAnimationHab, QueryAnimationHwnd                      ³
 *³ Cometary  : Queryfuntkionen fr statische Variablen                    ³
 *³ Author    : C.Langanke                                                 ³
 *³ Date      : 07.06.1996                                                 ³
 *³ Žnderung  : 07.06.1996                                                 ³
 *³ Call      : diverse                                                    ³
 *³ Calls on  : -                                                          ³
 *³ Imput     : -                                                          ³
 *³ Tasks     : - Variable zurckgeben                                     ³
 *³ Return    : HAB / HWND                                                 ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

HAB  QueryAnimationHab() {
  return habAnimationThread;
}

HWND QueryAnimationHwnd() {
  return hwndAnimation;
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : AnimationThread                                            ³
 *³ Kommentar : Thread-Funktion fr den Thread, der die Animation          ³
 *³             durchfhrt.                                                ³
 *³ Autor     : C.Langanke                                                 ³
 *³ Datum     : 07.06.1996                                                 ³
 *³ Žnderung  : 07.06.1996                                                 ³
 *³ aufgerufen: diverse                                                    ³
 *³ ruft auf  : -                                                          ³
 *³ Eingabe   : ULONG        - Pointer auf Parameterstruktur               ³
 *³ Aufgaben  : - Animation durchfhren                                    ³
 *³ Rckgabe  : -                                                          ³
 *ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

VOID _Optlink
AnimationThread( PVOID Parms )
{
  HAB     hab;
  HMQ     hmq;
  QMSG    qmsg;
  PSZ     pszWindowClass = ANIMATION_OBJECT_CLASS;
  PHEV    phevStartup = Parms;
  PSZ     pszAnimPriority;
  ULONG   ulAnimPriority = PRTYC_FOREGROUNDSERVER;

  do
  {
    // Priorit„t setzen
    pszAnimPriority = DEBUGSETTINGVAL( SET_ANIMTHREADPRIORITY );
    if( pszAnimPriority != NULL ) {
      ulAnimPriority  = atol( pszAnimPriority );
      if( ulAnimPriority > PRTYC_FOREGROUNDSERVER ) {
        ulAnimPriority = PRTYC_FOREGROUNDSERVER;
      }
    }
    SetPriority( ulAnimPriority );

    // PM-Ressourcen holen
    hab = WinInitialize( 0 );
    if( hab == NULL ) {
      break;
    }
    hmq = WinCreateMsgQueue( hab, 200 );
    if( hmq == NULL ) {
      break;
    }

    // // msg-Queue von Shutdown Processing abtrennen
    // *** nicht durchfhren, sonst wird thread einfach gekillt,
    // *** anstatt daá ein WM_CLOSE/WM_QUIT geschickt wird !!!
    // WinCancelShutdown( hmq, TRUE);

    // Secure hab
    habAnimationThread = hab;

    // Register class
    if( !WinRegisterClass( hab, pszWindowClass, ObjectWindowProc, 0, sizeof( ULONG ))) {
      break;
    }

    // create object window
    hwndAnimation = WinCreateWindow( HWND_OBJECT,
                                     pszWindowClass,
                                     "",
                                     0,
                                     0, 0, 0, 0,
                                     HWND_DESKTOP,
                                     HWND_TOP,
                                     0,
                                     phevStartup,
                                     NULL );

    if( hwndAnimation == NULLHANDLE ) {
      break;
    }

    // run the object window
    while( WinGetMsg( hab, &qmsg, (HWND)NULL, 0, 0 )) {
      WinDispatchMsg( hab, &qmsg );
    }

    // cleanup
    DeinitializePointerlist();
  } while( FALSE );

  // Add PM Resource
  if( hwndAnimation ) {
    WinDestroyWindow( hwndAnimation );
    hwndAnimation = NULLHANDLE;
  }

  if( hmq ) {
    WinDestroyMsgQueue( hmq );
  }

  if( hab ) {
    WinTerminate( hab );
    hab = NULLHANDLE;
    habAnimationThread = NULLHANDLE;
  }

  DEBUGMSG( "info: terminating animation thread" NEWLINE, 0 );
  _endthread();
}

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 *³ Name      : ObjectWindowProc                                           ³
 *³ Comentary : Window-Procedure des Object Window fr die Animation       ³
 *³ Author    : C.Langanke                                                 ³
 *³ Date      : 28.07.1996                                                 ³
 *³ Žnderung  : 28.07.1996                                                 ³
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
ObjectWindowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  APIRET      rc;
  PHEV        phevStartup;
  static BOOL fUse9Timer = FALSE;

  switch( msg )
  {
    case WM_CREATE:
      // startup sem  posten
      phevStartup = (PHEV)mp1;
      if( phevStartup != NULLHANDLE ) {
        DosPostEventSem( *phevStartup );
      }

      // Timer Variante ermitteln
      fUse9Timer = DEBUGSETTING( SET_ANIM9TIMER );
      break;

    case WM_USER_ENABLEHIDEPOINTER:
    {
      BOOL  fEnable = SHORT1FROMMP( mp1 );
      HAB   hab = WinQueryAnchorBlock( hwnd );
      ULONG ulHidePointerDelay = getHidePointerDelay();

      if( fEnable )
      {
        HOOKDATA hookdata = { hwnd };

        rc = SetHooks( __VERSION__, &hookdata, WinQueryAnchorBlock( hwnd ));
        if( rc != NO_ERROR ) {
          overrideSetHidePointer( FALSE );
          return (MRESULT)FALSE;
        }

        WinStartTimer( hab, hwnd, HIDEPOINTER_TIMER_ID, ulHidePointerDelay * 1000 );
      } else {
        WinStopTimer ( hab, hwnd, HIDEPOINTER_TIMER_ID );
        rc = ReleaseHooks( WinQueryAnchorBlock( hwnd ));
        // make pointer visible again
        WinSendMsg( hwnd, WM_TIMER, MPFROMLONG( MOUSEMOVED_TIMER_ID ), MPFROMLONG( TRUE ));
      }

      return (MRESULT)( rc == NO_ERROR );
    }

    case WM_USER_ENABLEANIMATION:
    {
      PPOINTERLIST ppl       = PVOIDFROMMP( mp1 );
      BOOL   fEnable   = SHORT1FROMMP( mp2 );
      ULONG  ulTimerId = 0;
      ULONG  ulTimeout = getDefaultTimeout();
      BOOL   fSuccess  = FALSE;

      // Parameter prfen
      if( ppl == NULL ) {
        break;
      }


      if( !fUse9Timer ) {
        // einfach als "animiert" kennzeichnen
        ppl->fAnimate = fEnable;
        fSuccess = TRUE;

        if( fEnable ) {
          // watchdog timer mit default timeframe value starten
          WinStartTimer( WinQueryAnchorBlock( hwnd ),
                         hwnd,
                         WATCHDOG_TIMER_ID,
                         WATCHDOG_TIMER_TIMEOUT );

          // eine Dummy Timer Message versenden
          // um die erste Animation anzuwerfen
          //          WinSendMsg( hwnd, WM_TIMER, MPFROMLONG( ANIMATION_TIMER_ID), 0);
          // ersten Timer einstellen
          ulTimeout = ppl->aulTimer[ ppl->ulPtrIndex];
          if(( ulTimeout == 0 ) || ( getOverrideTimeout())) {
            ulTimeout = getDefaultTimeout();
          }
          WinStartTimer( WinQueryAnchorBlock( hwnd ), hwnd, ANIMATION_TIMER_ID, ulTimeout );
        } else {
          // wird nichts mehr animiert ?
          // dann Watchdog Timer ausschalten
          if( !QueryAnimate( 0, TRUE )) {
            WinStopTimer( WinQueryAnchorBlock( hwnd ),
                          hwnd,
                          WATCHDOG_TIMER_ID );
          }

          // ausserdem Demo und Animation zurcksetzen
          ppl->ulPtrIndex    = 0;
          ppl->ulPtrIndexCnr = 0;
          ResetAnimation( ppl, ( ppl->ulPtrId == SPTR_ARROW ));
        }
      } else {
        // timer Id fr den betreffenden Mauszeiger ermitteln
        ulTimerId =  ( ppl - ( QueryPointerlist( 0 ))) + 1;
        if( ulTimerId > NUM_OF_SYSCURSORS ) {
          break;
        }
        switch( fEnable )
        {
          case TRUE:
            // timeout ermitteln
            ulTimeout = ppl->aulTimer[ ppl->ulPtrIndex];
            if( ulTimeout == 0 ) {
              ulTimeout = getDefaultTimeout();
            }

            // Timer immer erst stoppen, sonst gehen die Resourcen aus
            WinStopTimer( WinQueryAnchorBlock( hwnd ), hwnd, ulTimerId );

            // start timer / timer starten
            ulTimerId = WinStartTimer( WinQueryAnchorBlock( hwnd ),
                                       hwnd,
                                       ulTimerId,
                                       ulTimeout );
            fSuccess  = ( ulTimerId != 0 );
            break;

          case FALSE:
            // stop timer / timer stoppen
            fSuccess = WinStopTimer( WinQueryAnchorBlock( hwnd ), hwnd, ulTimerId );

            // ausserdem Demo und Animation zurcksetzen
            ppl->ulPtrIndex    = 0;
            ppl->ulPtrIndexCnr = 0;
            ResetAnimation( ppl, ( ppl->ulPtrId == SPTR_ARROW ));
            break;
        }

        // Flag „ndern
        if( fSuccess ) {
          ppl->fAnimate = fEnable;
        }
      }

      return (MRESULT)fSuccess;
    }

    case WM_TIMER:
    {
      ULONG ulTimerId   = LONGFROMMP( mp1 );
      ULONG ulPtrId = 0;
      PPOINTERLIST ppl;
      ULONG ulTimeout;
      BOOL  fIsActive;
      BOOL  fDragPending = FALSE;
      HAB   hab = WinQueryAnchorBlock( hwnd );

      switch( ulTimerId )
      {
        case HIDEPOINTER_TIMER_ID:
        {
          // Zeiger verstecken
          WinShowPointer( HWND_DESKTOP, FALSE );
          // disable timer
          WinStopTimer( hab, hwnd, HIDEPOINTER_TIMER_ID );
          break;
        }

        // MOUSEMOVED_TIMER_ID  ist kein echter Timer,
        // sondern wird vom hook gesendet, wenn
        // WM_MOUSEMOVE ber den Hook geht
        case MOUSEMOVED_TIMER_ID:
        {
          ULONG ulPointerLevel = WinQuerySysValue( HWND_DESKTOP, SV_POINTERLEVEL );
          ULONG i;
          ULONG ulHidePointerDelay = getHidePointerDelay();
          BOOL  fDontRestart = LONGFROMMP( mp2 );

          // show the pointer again
          for( i = 0; i < ulPointerLevel; i++ ) {
            WinShowPointer( HWND_DESKTOP, TRUE );
          }

          if( !fDontRestart ) {
            // reenable timer
            WinStartTimer( hab,
                           hwnd,
                           HIDEPOINTER_TIMER_ID,
                           ulHidePointerDelay * 1000 );
          }
          break;
        }

        // zwei cases fr die 2 timer Variante
        case ANIMATION_TIMER_ID:
          // Timer anhalten
          WinStopTimer( WinQueryAnchorBlock( hwnd ), hwnd, ANIMATION_TIMER_ID );

          // Zugriff auf Daten holen
          // bei Timeout Message verwerfen
          rc = REQUEST_DATA_ACCESS_TIMEOUT( ANIMATION_ACCESS_TIMEOUT );
          if( rc == ERROR_TIMEOUT ) {
            break;
          }
          if( rc != NO_ERROR ) {
            RELEASE_DATA_ACCESS();
            break;
          }

          // zuletzt animierten Zeiger ermitteln
          ulPtrId = WinQueryWindowULong( hwnd, QWL_USER );
          ppl = QueryPointerlist( ulPtrId );
          ulPtrId = ppl->ulPtrId;

          // prfen, ob es der Pfeil Zeiger ist
          // und Mausknopf 2 gedrckt ist
          if( ulPtrId == SPTR_ARROW ) {
            fDragPending = IsDragPending();
          }

          // n„chsten Pointer setzen
          if( ppl->fAnimate ) {
            if( !fDragPending ) {
              SetNextAnimatedPointer( habAnimationThread, ppl, FALSE );
            }

            // ggfs. n„chste Timeout-Zeit setzen
            {
              // jetzt neue Zeit einstellen
              ulTimeout = ppl->aulTimer[ ppl->ulPtrIndex];
              if(( ulTimeout == 0 ) || ( getOverrideTimeout())) {
                ulTimeout = getDefaultTimeout();
              }
              WinStartTimer( WinQueryAnchorBlock( hwnd ), hwnd, ANIMATION_TIMER_ID, ulTimeout );
            }
          }

          // Zugriff freigeben
          rc = RELEASE_DATA_ACCESS();
          break;

        case WATCHDOG_TIMER_ID:
          // Timer anhalten
          WinStopTimer( WinQueryAnchorBlock( hwnd ), hwnd, WATCHDOG_TIMER_ID );

          // Zugriff auf Daten holen
          // bei Timeout Message verwerfen
          rc = REQUEST_DATA_ACCESS_TIMEOUT( ANIMATION_ACCESS_TIMEOUT );
          if( rc == ERROR_TIMEOUT ) {
            break;
          }
          if( rc != NO_ERROR ) {
            RELEASE_DATA_ACCESS();
            break;
          }

          // zuletzt animierten Zeiger ermitteln
          ulPtrId = WinQueryWindowULong( hwnd, QWL_USER );
          if( ulPtrId > NUM_OF_SYSCURSORS ) {
            fIsActive = FALSE;
          } else {
            ppl = QueryPointerlist( ulPtrId );
            ulPtrId = ppl->ulPtrId;

            // ist der zuletzt animierte Zeiger nicht mehr aktiv ?
            fIsActive = IsPointerActive( ulPtrId, NULL );
          }

          if( !fIsActive ) {
            ULONG i;

            // aktuellen Zeiger ermitteln
            for( i = 0; i < NUM_OF_SYSCURSORS; i++ )
            {
              ppl = QueryPointerlist( i );
              ulPtrId = ppl->ulPtrId;

              fIsActive = IsPointerActive( ulPtrId, NULL );
              if( fIsActive ) {
                // und sichern
                WinSetWindowULong( hwnd, QWL_USER, i );

                // ggfs. n„chste Timeout-Zeit setzen
                if( ppl->fAnimate ) {
                  // Animation sofort weiterlaufen lassen
                  WinStartTimer( WinQueryAnchorBlock( hwnd ), hwnd, ANIMATION_TIMER_ID, 0 );
                } else {
                  WinStopTimer( WinQueryAnchorBlock( hwnd ), hwnd, ANIMATION_TIMER_ID );
                }

                // nicht weitersuchen
                break;
              }
            }
          }

          // Watchdog Timer mit niedrigster Zeitdauer wieder starten
          WinStartTimer( WinQueryAnchorBlock( hwnd ),
                         hwnd,
                         WATCHDOG_TIMER_ID,
                         WATCHDOG_TIMER_TIMEOUT );
          // Zugriff freigeben
          rc = RELEASE_DATA_ACCESS();
          break;

        // default case fr die 9 timer Variante
        default:
          if( !fUse9Timer ) {
            DosBeep( 1000, 10 );       // this may not occur ! (if (!fUse9Timer))
            break;
          }

          // ist es unser Timer ?
          if( ulTimerId <= NUM_OF_SYSCURSORS ) {
            // Timer anhalten
            WinStopTimer( WinQueryAnchorBlock( hwnd ), hwnd, ulTimerId );

            // Zugriff auf Daten holen
            // bei Timeout Message verwerfen
            rc = REQUEST_DATA_ACCESS_TIMEOUT( ANIMATION_ACCESS_TIMEOUT );
            if( rc == ERROR_TIMEOUT ) {
              break;
            }
            if( rc != NO_ERROR ) {
              RELEASE_DATA_ACCESS();
              break;
            }

            // Daten ermitteln
            ppl = QueryPointerlist( ulTimerId - 1 );

            // prfen, ob es der Pfeil Zeiger ist
            // und Mausknopf 2 gedrckt ist
            if( ppl->ulPtrId == SPTR_ARROW ) {
              fDragPending = IsDragPending();
            }

            // n„chsten Pointer setzen
            if( !fDragPending ) {
              SetNextAnimatedPointer( habAnimationThread, ppl, FALSE );
            }

            // ggfs. n„chste Timeout-Zeit setzen
            if( ppl->fAnimate ) {
              // jetzt neue Zeit einstellen
              ulTimeout = ppl->aulTimer[ ppl->ulPtrIndex];
              if(( ulTimeout == 0 ) || ( getOverrideTimeout())) {
                ulTimeout = getDefaultTimeout();
              }
              WinStartTimer( WinQueryAnchorBlock( hwnd ), hwnd, ulTimerId, ulTimeout );
            }

            // Zugriff freigeben
            rc = RELEASE_DATA_ACCESS();
          }
          break;
      }
      return (MRESULT)FALSE;
    }
  }

  return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}

