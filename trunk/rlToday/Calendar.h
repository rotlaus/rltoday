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

#ifndef __calendarheader_h__
#define __calendarheader_h__
#include "skinitem.h"
#include <vector>
#include "..\cximage\ximage.h"
class CCalendar : public SkinItem{
public:
	CCalendar(void);
public:
	~CCalendar(void);
	virtual void Paint(HDC hDC);
	void SetFont(HFONT* font){m_hFont = *font;};
	void SetColor(COLORREF* color){ m_rgbFontColor = *color;
									m_rgbFontColorSa = *color;
									m_rgbFontColorSu = *color;};
	void SetSaColor(COLORREF* color){ m_rgbFontColorSa = *color;};
	void SetSuColor(COLORREF* color){ m_rgbFontColorSu = *color;};
	void SetToColor(COLORREF* color);
	void SetTodayImage(const TCHAR *nFaceBitmap);
	virtual void SetRaster(const int rx, const int ry){ m_xraster = rx; m_yraster = ry; };
	void SetWeekDayAbbVector(const std::vector<TSTRING> & weekdayabbs){m_vWeekDayAbbs = weekdayabbs;};
	void SetWeekStarts(const int weekStart){m_WeekStarts = weekStart;};
	void SetOffset(const int offset);
	virtual RECT GetRect();
protected:
	HFONT		m_hFont;
	COLORREF	m_rgbFontColor;
	COLORREF	m_rgbFontColorSa;
	COLORREF	m_rgbFontColorSu;
	COLORREF	m_rgbFontColorTo;
	int m_xraster;
	int m_yraster;
	int m_Offset;
	std::vector<TSTRING> m_vWeekDayAbbs;
	int  m_WeekStarts;
	bool m_toColorSet;
	CxImage*	m_cxImage;
	int PaintElement(HDC hDC, const TCHAR *szText, UINT nPlace, HFONT hFont, COLORREF color);
};
#endif //__calendarheader_h__

