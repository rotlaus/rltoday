#include "CalHeader.h"

const __int64 nano100SecInDay=(__int64)10000000*60*60*24;

CCalHeader::CCalHeader(){
	m_xpos = 0;
	m_ypos = 0;
	date_format = L"ddd, d. MMM yyyy";
	m_nAlignment = DT_LEFT;
}

CCalHeader::~CCalHeader(){
}

void CCalHeader::SetOffset(const int offset){
	m_Offset = offset;
	if( m_Offset < 0 ){
		if( m_Offset < -11 )
			m_Offset = -11;
	}else{
		if( m_Offset > 11 )
			m_Offset = 11;
	}
}

void CCalHeader::Paint(HDC drawdc){
	HFONT hFontOld;
	RECT rect;
	SIZE size;
	SYSTEMTIME stCal;
	FILETIME ft;
	__int64 *pi;
	UINT m_Month, m_Day;
	TCHAR szText[BUF_SIZE];
	int m_DisplayMonth = 0;
	GetLocalTime(&stCal);
	m_Month = stCal.wMonth;
	m_Day = stCal.wDay;
	SystemTimeToFileTime(&stCal, &ft);
	pi = (__int64*) &ft;

	if( m_Offset < 0){
		m_DisplayMonth = m_Month + m_Offset;
		if( m_DisplayMonth <= 0)
			m_DisplayMonth = m_DisplayMonth + 12;
		
		while(stCal.wMonth != (m_DisplayMonth)){
			(*pi) -= (__int64)(1) * nano100SecInDay;
			FileTimeToSystemTime(&ft, &stCal);
		}
	}else if( m_Offset > 0){
		m_DisplayMonth = m_Month + m_Offset;
		if( m_DisplayMonth > 12)
			m_DisplayMonth = m_DisplayMonth - 12;
		while(stCal.wMonth != m_DisplayMonth ){
			(*pi) += (__int64)(1) * nano100SecInDay;
			FileTimeToSystemTime(&ft, &stCal);
		}
	}
	if(date_format == L""){
		GetDateFormat(0, 0, &stCal, NULL, szText, BUF_SIZE - 1);
	}else{
		GetDateFormat(0, 0, &stCal, date_format.c_str(), szText, BUF_SIZE - 1);
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
