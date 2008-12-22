#include "Registry.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRegistry::CRegistry(){
	m_xpos = 0;
	m_ypos = 0;
	reg_key = L"";
	unknown = L"xxx";
	m_nAlignment = DT_LEFT;
}

CRegistry::~CRegistry(){
}

void CRegistry::Paint(HDC drawdc){
	HFONT hFontOld;
	RECT rect;
	SIZE size;
	TSTRING text;
	text = GetRegistry(reg_key);
	if(text == L"xxx")
		text = unknown;
	hFontOld = (HFONT)SelectObject(drawdc, m_hFont);
	GetTextExtentPoint32(drawdc, text.c_str(), _tcslen(text.c_str()), &size);
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
	DrawText(drawdc, text.c_str(), -1, &rect, m_nAlignment);
	m_rect = rect;
	SelectObject(drawdc, hFontOld);
}
