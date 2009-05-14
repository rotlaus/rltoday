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

#include "DigitalClock.h"
#include "TimeZone.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDigitalClock::CDigitalClock(){
	m_xpos = 0;
	m_ypos = 0;
	date_format = L"ddd, d. MMM yyyy";
	m_nAlignment = DT_LEFT;
	m_nTimeZone = -1;
}

CDigitalClock::~CDigitalClock(){
}

void CDigitalClock::Paint(HDC drawdc){
	HFONT hFontOld;
	RECT rect;
	SIZE size;
	SYSTEMTIME st;
	TCHAR szText[BUF_SIZE];
	if(m_nTimeZone >= 0){
		CTimeZone tzTime;
		tzTime.SetTimeZone(m_nTimeZone);
		st = tzTime.GetTime();
	}else{
		GetLocalTime(&st);
	}
	if(date_format == L""){
		GetTimeFormat(0, 0, &st, NULL, szText, BUF_SIZE - 1);
	}else{
		GetTimeFormat(0, 0, &st, date_format.c_str(), szText, BUF_SIZE - 1);
	}
	hFontOld = (HFONT)SelectObject(drawdc, m_hFont);
	SetTextColor(drawdc, m_rgbFontColor);
	GetTextExtentPoint32(drawdc, szText, _tcslen(szText), &size);
	rect.left = m_xpos;
	rect.top = m_ypos;
	rect.right = rect.left + size.cx;
	rect.bottom = rect.top + size.cy;
	if(m_nAlignment == DT_RIGHT){
		rect.left -= size.cx;
		rect.right -= size.cx;
	}else if(m_nAlignment == DT_CENTER){
		rect.left -= (size.cx / 2);
		rect.right -= (size.cx / 2);
	}
	if(m_selected){
		HBRUSH hBrush;	
		hBrush = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		FillRect(drawdc, &m_rect, hBrush);
		DeleteObject(hBrush);
	}
	DrawText(drawdc, szText, -1, &rect, m_nAlignment);
	m_rect = rect;
	SelectObject(drawdc, hFontOld);
}
void CDigitalClock::LButtonUp(UINT nFlags, POINT point){
	PROCESS_INFORMATION pi;
	::CreateProcess(_T("\\Windows\\ctlpnl.exe"), _T("cplmain.cpl,16"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi);
}
