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
#ifndef MPTRFILE_H
#define MPTRFILE_H

#include "mptrppl.h"

// Prototypes

BOOL EXPENTRY SelectFile( HWND hwndOwner, ULONG ulDialogStyle,
                          PSZ pszInitialFilename, PULONG pulTargetFileType,
                          PSZ pszFileName, ULONG  ulBuflen );

BOOL EXPENTRY WriteTargetFiles( PSZ pszFileName, ULONG ulTargetFileType,
                                PPOINTERLIST ppl, BOOL fSaveAllPointers );

#endif // MPTRFILE_H

