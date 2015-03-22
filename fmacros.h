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

#ifndef FMACROS_H
#define FMACROS_H

// ######################################
// read file contents to buffer
// ######################################

#define READMEMORY(h, t, l)             \
{                                       \
rc = DosRead(  h,                       \
               &t,                      \
               l,                       \
               &ulBytesRead);           \
if (rc != NO_ERROR)                     \
   break;                               \
}rc                                     \

#define PREADMEMORY(h,p,l)              \
{                                       \
rc = DosRead(  h,                       \
               p,                       \
               l,                       \
               &ulBytesRead);           \
if (rc != NO_ERROR)                     \
   break;                               \
}rc                                     \

// ###
#define READULONG(h,t)                  \
READMEMORY(h, t, sizeof( ULONG))

// ######################################
// query/modify file ptr
// ######################################

#define MOVEFILEPTR(h,o)                \
{                                       \
rc = DosSetFilePtr( h,                  \
                    o,                  \
                    FILE_CURRENT,       \
                    &ulFilePtr);        \
if (rc != NO_ERROR)                     \
   break;                               \
}rc                                     \

#define QUERYFILEPTR(h)                 \
MOVEFILEPTR(h,0)


#define SETFILEPTR(h,o)                 \
{                                       \
rc = DosSetFilePtr( h,                  \
                    o,                  \
                    FILE_BEGIN,         \
                    &ulFilePtr);        \
if (rc != NO_ERROR)                     \
   break;                               \
}rc                                     \


// ######################################
// skip file contents
// ######################################

#define SKIPBYTES(h,n)                  \
if (n > 0)                              \
   MOVEFILEPTR(h,n)

#define SKIPULONG(h,n)                  \
SKIPBYTES(h, n * sizeof( ULONG))

// ######################################
// write file contents
// ######################################

// write memory
#define WRITEMEMORY(h,p,len)            \
{                                       \
rc = DosWrite( h,                       \
               p,                       \
               len,                     \
               &ulBytesWritten);        \
if (rc != NO_ERROR)                     \
   break;                               \
}rc                                     \


// write SZ
#define WRITESZ(h,s)                    \
WRITEMEMORY(h, s, strlen( s) + 1)

// write struct
#define WRITESTRUCT(h,s)                \
WRITEMEMORY(h, &s, sizeof( s))

#endif // FMACROS_H

