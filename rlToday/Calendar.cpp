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

#include "Calendar.h"

const __int64 nano100SecInDay=(__int64)10000000*60*60*24;

CCalendar::CCalendar(void){
	m_xpos = 0;
	m_ypos = 0;
	m_cxImage = NULL;
	m_toColorSet = false;
}

CCalendar::~CCalendar(void){
	if(m_cxImage != NULL)
		delete m_cxImage;
}

void CCalendar::SetTodayImage(const TCHAR *nFaceBitmap){
	if(m_cxImage != NULL){
		delete m_cxImage;
		m_cxImage = NULL;
	}
	if(*nFaceBitmap == 0)
		return;

	m_cxImage = new CxImage();
	m_cxImage->Load(nFaceBitmap);
}

void CCalendar::SetOffset(const int offset){
	m_Offset = offset;
	if( m_Offset < 0 ){
		if( m_Offset < -11 )
			m_Offset = -11;
	}else{
		if( m_Offset > 11 )
			m_Offset = 11;
	}
}

void CCalendar::SetToColor(COLORREF* color){
	m_rgbFontColorTo = *color;
	m_toColorSet = true;
}

void CCalendar::Paint(HDC drawdc){
	SYSTEMTIME stCal;
	FILETIME ft;
	__int64 *pi;
	UINT m_Month, m_Day, m_thisMonth;
	UINT nI, nK;
	TCHAR szDateText[6];
	int nRet;
	int tx = -1;
	int ty = -1;
	int m_DisplayMonth = 0;
	nRet = PaintElement(drawdc, m_vWeekDayAbbs[0].c_str(), (m_WeekStarts==0)?0:6, m_hFont, m_rgbFontColorSu);//Sunday
	nRet = PaintElement(drawdc, m_vWeekDayAbbs[1].c_str(), 1 - m_WeekStarts, m_hFont, m_rgbFontColor);//Monday
	nRet = PaintElement(drawdc, m_vWeekDayAbbs[2].c_str(), 2 - m_WeekStarts, m_hFont, m_rgbFontColor);//Tuesday
	nRet = PaintElement(drawdc, m_vWeekDayAbbs[3].c_str(), 3 - m_WeekStarts, m_hFont, m_rgbFontColor);//Wednesday
	nRet = PaintElement(drawdc, m_vWeekDayAbbs[4].c_str(), 4 - m_WeekStarts, m_hFont, m_rgbFontColor);//Thursday
	nRet = PaintElement(drawdc, m_vWeekDayAbbs[5].c_str(), 5 - m_WeekStarts, m_hFont, m_rgbFontColor);//Friday
	nRet = PaintElement(drawdc, m_vWeekDayAbbs[6].c_str(), 6 - m_WeekStarts, m_hFont, m_rgbFontColorSa);//Saturday
	GetLocalTime(&stCal);
	m_Month = stCal.wMonth;
	m_thisMonth = stCal.wMonth;
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
	(*pi) -= (__int64)(stCal.wDay - 1) * nano100SecInDay; 
	FileTimeToSystemTime(&ft, &stCal);
	if(m_WeekStarts == 1){	//Week starts Sunday
		if(stCal.wDayOfWeek == 0) // Sunday
			nK = 6;
		else
			nK = stCal.wDayOfWeek - 1;
	}else{					//Week starts Monday
		nK = stCal.wDayOfWeek;
	}
	nI = 1;
	m_Month = stCal.wMonth;
	while(stCal.wMonth == m_Month){
		wsprintf(szDateText, TEXT("%i"), stCal.wDay);
		if((stCal.wDay == m_Day) && (stCal.wMonth == m_thisMonth)){
			if( m_toColorSet == true ){
				nRet = PaintElement(drawdc, szDateText, nI * 7 + nK, m_hFont, m_rgbFontColorTo);
			}else{
				if(stCal.wDayOfWeek == 6){
					nRet = PaintElement(drawdc, szDateText, nI * 7 + nK, m_hFont, m_rgbFontColorSa);
				}else if(stCal.wDayOfWeek == 0){
					nRet = PaintElement(drawdc, szDateText, nI * 7 + nK, m_hFont, m_rgbFontColorSu);
				}else{
					nRet = PaintElement(drawdc, szDateText, nI * 7 + nK, m_hFont, m_rgbFontColor);
				}
			}
			tx = nK;
			ty = nI;
		}else if(stCal.wDayOfWeek == 6){
			nRet = PaintElement(drawdc, szDateText, nI * 7 + nK, m_hFont, m_rgbFontColorSa);
		}else if(stCal.wDayOfWeek == 0){
			nRet = PaintElement(drawdc, szDateText, nI * 7 + nK, m_hFont, m_rgbFontColorSu);
		}else{
			nRet = PaintElement(drawdc, szDateText, nI * 7 + nK, m_hFont, m_rgbFontColor);
		}
		nK++;
		if(nK >= 7){
			nK = 0;
			nI ++;
		}
		(*pi) += (__int64)nano100SecInDay; 
		FileTimeToSystemTime(&ft, &stCal);
	}
	if((tx >= 0) && (ty >= 0)){
		//draw today image
		int mXpos;
		int mYpos;
		if(m_cxImage != NULL){
			mXpos = tx * m_xraster + ( m_xraster / 2 ) - (m_cxImage->GetWidth() / 2) + m_xpos;
			mYpos = ty * m_yraster + ( m_yraster / 2 ) - (m_cxImage->GetHeight() / 2) + m_ypos;
			m_cxImage->Draw(drawdc, mXpos, mYpos);
		}
	}
}

int CCalendar::PaintElement(HDC hDC, const TCHAR *szText, UINT nPlace, HFONT hFont, COLORREF color){
	UINT nLine;
	UINT nCol;
	RECT rect;
	HFONT hFontOld;
	SIZE size;
	nLine = nPlace / 7;
	nCol = nPlace - nLine * 7;
	SetTextColor(hDC, color);
	SetBkMode(hDC, TRANSPARENT);
	hFontOld = (HFONT)SelectObject(hDC, hFont);
	GetTextExtentPoint32(hDC, _T("Mo"), _tcslen(_T("Mo")), &size);
	rect.left = m_xpos + (nCol * m_xraster);
	rect.top = m_ypos + (nLine * m_yraster);
	rect.right = m_xpos + ((nCol + 1) * m_xraster);
	rect.bottom = m_ypos + ((nLine + 1) * m_yraster);
	//ExtTextOut(hDC, local_pos_x, local_pos_y, ETO_OPAQUE, NULL, szText, lstrlen(szText), NULL);
	DrawText(hDC, szText, -1, &rect, DT_RIGHT);
	SelectObject(hDC, hFontOld);
	return rect.bottom;
}

RECT CCalendar::GetRect(){
	m_rect.left = m_xpos;
	m_rect.top = m_ypos;
	m_rect.right = m_xpos + 7 * m_xraster;
	m_rect.bottom = m_ypos + 6 * m_yraster;
	return m_rect;
}
