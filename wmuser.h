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
#ifndef WMUSER_H
#define WMUSER_H

// *** user messages for container handling
// enable animation:          mp1 = ppl;     mp2=fEnable;
#define WM_USER_ENABLEANIMATION     (WM_USER + 0x01000)
// enable container:          mp1 = fEnable; mp2 = 0;
#define WM_USER_ENABLECONTAINER     (WM_USER + 0x01001)
// enable container controls: mp1 = 0;       mp2 = 0;
#define WM_USER_ENABLECONTROLS      (WM_USER + 0x01002)
// enable container:          mp1 = fEnable; mp2 = 0;
#define WM_USER_ENABLEHIDEPOINTER   (WM_USER + 0x01003)

// asyncronous creation of dropped files after end of Drag&Drop Conversation
//                            mp1 = source   mp2 = target (strdup strings !)
#define WM_USER_CREATEDROPPEDFILES  (WM_USER + 0x01008)

// *** user messages for properties notebook handling
// verify page data:          mp1 = (PVERIFYRESULT); mp2= 0;
#define WM_USER_VERIFYDATA          (WM_USER + 0x01010)

// *** user messages for exe loader
// initialize asynchronously: mp1 = 0;       mp2 = 0;
#define WM_USER_DELAYEDINIT         (WM_USER + 0x01011)


// service callback message
#define WM_USER_SERVICE       (WM_USER + 0x02000)
#define SERVICE_SAVE           0x0001
#define SERVICE_RESTORE        0x0002
#define SERVICE_HELP           0x0003
#define SERVICE_FIND           0x0004
#define SERVICE_LOAD           0x0005
#define SERVICE_HELPMENU       0x0006

// nur fÅr Testprogramm: Pointerset laden bei Startup
#define WM_USER_LOADRESOURCE  (WM_USER + 0x02001)

#endif // WMUSER_H

