/*******************************************************************************
*   rlToday - multi purpose today plugin.                                      *
*                                                                              *
*   Copyright (C) 2005-2009 Andre Adrian aka Rotlaus                           *
*	(rotlaus@gmail.com)                                                        *
*                                                                              *
*   This program is free software; you can redistribute it and/or              *
*   modify it under the terms of the GNU General Public License version 2      *
*   as published by the Free Software Foundation.                              *
*                                                                              *
*   This program is distributed in the hope that it will be useful,            *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
*   General Public License version 2 for more details.                         *
*                                                                              *
*   You should have received a copy of the GNU General Public License          *
*   version 2 along with this library; if not, write to the Free Software      *
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *
*******************************************************************************/

#ifndef __HELPER_H__
#define __HELPER_H__

#include <windows.h>
#include <string>

#ifdef _UNICODE
# define TSTRING std::wstring
#else
# define TSTRING std::string
#endif

#ifndef PI
#define PI					3.1415926535
#endif

void DrawLine(HDC pDC, int x1, int y1, int x2, int y2, COLORREF m_rgbMinuteHand);
wchar_t* Ascii2Uni(const char* SomeAsciiStr, wchar_t* buffer);
char* Uni2Ascii(const TCHAR* SomeUnicodeStr, char* buffer);
void Message(const TCHAR* textToSay);
void Message(const TSTRING* textToSay);
void ErrorMessage(const TCHAR* textToSay);
void ErrorMessage(const TSTRING* textToSay);
std::wstring ctow(const char* src);
int cmp_nocase(const TSTRING& s, const TSTRING& s2);
COLORREF RGB2COLOR(const TSTRING& color);
void Add2Log(const TCHAR* message);
int MyAtoI(const TCHAR* asciizahl);
void ShellOpen(const TCHAR *FileName, const TCHAR *CommnadLine);
std::wstring GetRegistry(const TSTRING& key);
typedef struct _FILELIST {
	TSTRING Name;
	TSTRING FileName;
	TSTRING CommandLine;
	TSTRING IconPath;
	int IconResource;
	int Index;
} FILELIST;

#endif // __HELPER_H__
