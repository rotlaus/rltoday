#include "TZName.h"
#include "TimeZone.h"
CTZName::CTZName(){
	m_xpos = 0;
	m_ypos = 0;
	m_nAlignment = DT_LEFT;
}
CTZName::~CTZName(){
}
void CTZName::Paint(HDC drawdc){
	HFONT hFontOld;
	RECT rect;
	SIZE size;
	hFontOld = (HFONT)SelectObject(drawdc, m_hFont);
	GetTextExtentPoint32(drawdc, m_TZName.c_str(), _tcslen(m_TZName.c_str()), &size);
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
	SetTextColor(drawdc, m_rgbFontColor);
	if(m_selected){
		HBRUSH hBrush;	
		hBrush = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		FillRect(drawdc, &m_rect, hBrush);
		DeleteObject(hBrush);
	}
	DrawText(drawdc, m_TZName.c_str(), -1, &rect, m_nAlignment);
	m_rect = rect;
	SelectObject(drawdc, hFontOld);
}
void CTZName::LButtonUp(UINT nFlags, POINT point){
	PROCESS_INFORMATION pi;
	::CreateProcess(_T("\\Windows\\ctlpnl.exe"), _T("cplmain.cpl,16"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi);
}
