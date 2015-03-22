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
#ifndef MPTRUTIL_H
#define MPTRUTIL_H


// Prototypes
APIRET GetModuleInfo( PFN pfn, PHMODULE phmod, PSZ pszBuffer, ULONG ulBufLen );
APIRET GetBaseName( PFN pfn, PSZ pszBuffer, ULONG ulBufLen );
APIRET ReloadClassLib( PHMODULE phmodResource );
APIRET GetHelpLibName( PSZ pszBuffer, ULONG ulMaxLen );
APIRET IsRemoveableMedia( PSZ pszDeviceName, PBOOL pfIsRemoveableMedia );
APIRET IsLocalDrive( PSZ pszDeviceName, PBOOL pfIsLocalDrive );
APIRET OpenTmpFile( PSZ pszBaseName, PSZ pszExtension, PHFILE phfile, PSZ pszFileName, ULONG ulMaxlen );
BOOL   IsWARP3( void );
BOOL   SetPriority( ULONG ulPriorityClass );
APIRET GetNextFile( PSZ pszFileMask, PHDIR phdir, PSZ pszNextFile, ULONG ulBufLen );
BOOL   FileExist( PSZ pszName, BOOL fCheckDirectory );
BOOL   FileMaskExists( PSZ pszFilename, PSZ pszFirstFile, ULONG ulBuflen );
BOOL   IsFileModified( PSZ pszFilename, PFILESTATUS3 pfs3 );
APIRET ConvertFile( PSZ pszFilename, ULONG ulTargetFiletype );

#define CHECK_DIRECTORY TRUE
#define CHECK_FILE      FALSE

BOOL IsFilenameValid( PSZ pszFilename, ULONG ulCheckType, PULONG pulValue );

// Styles with trailing _ are not yet implemented
#define CHANGE__MASK             0x000f
#define CHANGE_DRIVE_            0x0001
#define CHANGE_PATH_             0x0002
#define CHANGE_DRIVEPATH         0x0003
#define CHANGE_NAME_             0x0004
#define CHANGE_EXTENSION         0x0005
#define CHANGE_USEPOINTERNAME    0x0006
#define CHANGE_USEDLLNAME        0x0007

#define CHANGE_DELETE__MASK      0x00f0
#define CHANGE_DELETEDRIVE_      0x0010
#define CHANGE_DELETEPATH_       0x0020
#define CHANGE_DELETEDRIVEPATH_  0x0030
#define CHANGE_DELETEFILENAME    0x0040
#define CHANGE_DELETEEXTENSION_  0x0050

#define CHANGE_NUMERATION__MASK  0x0f00
#define CHANGE_ADDNUMERATION     0x0100
#define CHANGE_SETNUMERATION     0x0200
#define CHANGE_DELNUMERATION     0x0300

APIRET CreateTmpFile( PSZ pszMainFile, PHFILE phfile, PSZ pszBuffer, ULONG ulBufferlen );

PSZ Filespec( PSZ pszFilename, ULONG ulPart );

#define FILESPEC_PATHNAME  1
#define FILESPEC_NAME      2
#define FILESPEC_EXTENSION 3

APIRET CopyFromFileToFile( HFILE hfileTarget, HFILE hfileSource, ULONG ulDataLen );

MRESULT EXPENTRY SubclassStaticWindowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
APIRET  EXPENTRY LoadResourceLib( PHMODULE phmodResource );
PSZ     EXPENTRY LoadStringResource( ULONG ulResourceType, HAB hab, HLIB hlibResource, ULONG ulResId, PSZ pszBuffer, ULONG ulBufLen );

#define FILENAME_CONTAINSNUMERATION  1
#define FILENAME_CONTAINSPOINTERNAME 2
#define FILENAME_CONTAINSWILDCARDS   3
#define FILENAME_CONTAINSFULLNAME    4

APIRET  EXPENTRY ChangeFilename( PSZ pszFileName, ULONG ulChangeType, PSZ pszNewName, ULONG ulBufLen,
                                 PSZ pszNewNamePart, ULONG ulPointerIndex, ULONG ulNumeration );

#endif // MPTRUTIL_H

