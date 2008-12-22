#include "Date.h"
#include "TimeZone.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDate::CDate(){
	m_xpos = 0;
	m_ypos = 0;
	date_format = L"ddd, d. MMM yyyy";
	m_nAlignment = DT_LEFT;
	m_nTimeZone = -1;
}

CDate::~CDate(){
}

void CDate::Paint(HDC drawdc){
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
		GetDateFormat(0, 0, &st, NULL, szText, BUF_SIZE - 1);
	}else{
		GetDateFormat(0, 0, &st, date_format.c_str(), szText, BUF_SIZE - 1);
	}

	SetTextColor(drawdc, m_rgbFontColor);
	hFontOld = (HFONT)SelectObject(drawdc, m_hFont);
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
