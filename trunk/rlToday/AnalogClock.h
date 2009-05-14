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

#ifndef __ANALOGCLOCK_H__
#define __ANALOGCLOCK_H__
#include <vector>
#include "Line.h"
#include "Helper.h"
#include "skinitem.h"
#include "..\cximage\ximage.h"
/////////////////////////////////////////////////////////////////////////////
// CAnalogClock window
class CAnalogClock:public SkinItem{
// Construction
public:
	CAnalogClock();
	virtual ~CAnalogClock();
// Attributes
public:
	SYSTEMTIME GetTime() { return m_time; };
	SIZE GetWindowSize();
	TSTRING ClockFaceName;
	void SetClockface(const TCHAR *nFaceBitmap);
	void SetHourHand(const TCHAR *nFaceBitmap);
	void SetMinuteHand(const TCHAR *nFaceBitmap);
	void SetSecondHand(const TCHAR *nFaceBitmap);
	void CreateHandArray(CxImage* original, std::vector<CxImage>* store);
	void SetHourHandColor(COLORREF rgb);
	void SetMinuteHandColor(COLORREF rgb);
	void SetSecondHandColor(COLORREF rgb);
	void SetTime(SYSTEMTIME& t) { m_time = t; };
	void ShowSecondHand(BOOL bShowSecondHand) { m_bShowSecondHand = bShowSecondHand; };
	void ShowMinuteHand(BOOL bShowMinuteHand) { m_bShowMinuteHand = bShowMinuteHand; };
	void ShowHourHand(BOOL bShowHourHand) { m_bShowHourHand = bShowHourHand; };
	void SetTimeZone(int timezone){m_nTimeZone = timezone;};
	//SkinItem Functions
	virtual void Paint(HDC hDC);
	virtual RECT GetRect();
// Implementation
protected:
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();
	float ConvertToDegrees(float dTime);
	void PaintMinuteAndHourHands(HDC pDC);
	void PaintSecondHand(HDC pDC);
	CxImage*	m_cxClockface;
	CxImage*	m_cxMinutehand;
	std::vector<CxImage> m_Minutes;
	std::vector<CLine> m_MinuteLines;
	CxImage*	m_cxHourhand;
	std::vector<CxImage> m_Hours;
	std::vector<CLine> m_HourLines;
	CxImage*	m_cxSecondhand;
	std::vector<CxImage> m_Seconds;
	std::vector<CLine> m_SecondLines;
	CxImage*	m_cxDrawBuffer;
	int			m_nPrevMinute;
	BOOL		m_bShowSecondHand;
	BOOL		m_bShowMinuteHand;
	BOOL		m_bShowHourHand;
	int			m_nTimeZone;
	SYSTEMTIME	m_time;
	COLORREF	m_rgbSecondHand;
	COLORREF	m_rgbMinuteHand;
	COLORREF	m_rgbHourHand;
	// Size and hand length
	int width;
	int height;
	int hw;
	int co;
	float mhl;
	float hhl;
	float shl;
};
#endif //__ANALOGCLOCK_H__
