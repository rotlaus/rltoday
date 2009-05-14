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

#include "AnalogClock.h"
#include "math.h"
#include "Helper.h"
#include "TimeZone.h"
#include "..\cximage\ximage.h"

#define SECOND_HAND_COLOR	RGB(220,20,60)
#define MINUTE_HAND_COLOR	RGB(0,0,0)
#define HOUR_HAND_COLOR		RGB(0,0,0)

CAnalogClock::CAnalogClock(){
	m_nPrevMinute     = -1;
	m_bShowSecondHand = TRUE;
	m_bShowMinuteHand = TRUE;
	m_bShowHourHand	  = TRUE;
	GetLocalTime(&m_time);
	m_rgbSecondHand   = SECOND_HAND_COLOR;
	m_rgbMinuteHand   = MINUTE_HAND_COLOR;
	m_rgbHourHand     = HOUR_HAND_COLOR;
	m_xpos = 0;
	m_ypos = 0;
	m_nTimeZone = -1;	//GMT Standard Time
	m_cxClockface = NULL;
	m_cxHourhand = NULL;
	m_cxMinutehand = NULL;
	m_cxSecondhand = NULL;
}

CAnalogClock::~CAnalogClock(){
	if(m_cxClockface != NULL){
		delete m_cxClockface;
	}
	if(m_cxHourhand != NULL){
		delete m_cxHourhand;
		m_Hours.clear();
	}
	if(m_cxMinutehand != NULL){
		delete m_cxMinutehand;
		m_Hours.clear();
	}
	if(m_cxSecondhand != NULL){
		delete m_cxSecondhand;
		m_Hours.clear();
	}
}

void CAnalogClock::SetClockface(const TCHAR *nFaceBitmap){
	if(m_cxClockface != NULL){
		delete m_cxClockface;
		m_cxClockface = NULL;
	}
	m_cxClockface = new CxImage();
	m_cxClockface->Load(nFaceBitmap);
	width = m_cxClockface->GetWidth();
	height = m_cxClockface->GetHeight();
	if(width > 70){
		co = 4;
		hw = 2;
	}else if(width > 50){
		co = 2;
		hw = 1;
	}else{
		co = 0;
		hw = 0;
	}
	if(width >= height){
		mhl = (float)(height * 0.39);
		hhl = (float)(height * 0.28);
		shl = (float)(height * 0.35);
	}else{
		mhl = (float)(width * 0.39);
		hhl = (float)(width * 0.28);
		shl = (float)(width * 0.35);
	}
	if( width > 0 ){
		CLine secLine1(0, 0, 0, -(width * 0.37), m_rgbSecondHand);
		CLine secLine2(1, 0, 1, -(width * 0.37), m_rgbSecondHand);
		m_SecondLines.push_back(secLine1);
		m_SecondLines.push_back(secLine2);
		CLine minLine1(-1, 0, -1, -(width * 0.41), m_rgbMinuteHand);
		CLine minLine2(0, 0, 0, -(width * 0.41), m_rgbMinuteHand);
		CLine minLine3(1, 0, 1, -(width * 0.41), m_rgbMinuteHand);
		m_MinuteLines.push_back(minLine1);
		m_MinuteLines.push_back(minLine2);
		m_MinuteLines.push_back(minLine3);
		CLine hourLine1(-1, 0, 0, -(width * 0.30), m_rgbHourHand);
		CLine hourLine2(0, 0, 0, -(width * 0.30), m_rgbHourHand);
		CLine hourLine3(1, 0, 1, -(width * 0.30), m_rgbHourHand);
		CLine hourLine4(2, 0, 1, -(width * 0.30), m_rgbHourHand);
		m_HourLines.push_back(hourLine1);
		m_HourLines.push_back(hourLine2);
		m_HourLines.push_back(hourLine3);
		m_HourLines.push_back(hourLine4);
	}
}

void CAnalogClock::SetHourHandColor(COLORREF rgb) { 
	m_rgbHourHand = rgb; 
	for(int i = 0; i < m_HourLines.size(); i ++){
		m_HourLines[i].SetColor(m_rgbHourHand);
	}
}

void CAnalogClock::SetMinuteHandColor(COLORREF rgb) { 
	m_rgbMinuteHand = rgb;
	for(int i = 0; i < m_MinuteLines.size(); i ++){
		m_MinuteLines[i].SetColor(m_rgbHourHand);
	}
}

void CAnalogClock::SetSecondHandColor(COLORREF rgb) { 
	m_rgbSecondHand = rgb; 	
	for(int i = 0; i < m_SecondLines.size(); i ++){
		m_SecondLines[i].SetColor(m_rgbHourHand);
	}
}

void CAnalogClock::SetHourHand(const TCHAR *nFaceBitmap){
	if(m_cxHourhand != NULL){
		delete m_cxHourhand;
		m_cxHourhand = NULL;
	}
	if(*nFaceBitmap == 0)
		return;
	m_cxHourhand = new CxImage();
	m_cxHourhand->Load(nFaceBitmap);
	CreateHandArray(m_cxHourhand, &m_Hours);
}

void CAnalogClock::SetMinuteHand(const TCHAR *nFaceBitmap){
	if(m_cxMinutehand != NULL){
		delete m_cxMinutehand;
		m_cxMinutehand = NULL;
	}
	if(*nFaceBitmap == 0)
		return;
	m_cxMinutehand = new CxImage();
	m_cxMinutehand->Load(nFaceBitmap);
	CreateHandArray(m_cxMinutehand, &m_Minutes);
}
void CAnalogClock::SetSecondHand(const TCHAR *nFaceBitmap){
	if(m_cxSecondhand != NULL){
		delete m_cxSecondhand;
		m_cxSecondhand = NULL;
	}
	if(*nFaceBitmap == 0)
		return;
	m_cxSecondhand = new CxImage();
	m_cxSecondhand->Load(nFaceBitmap);
	CreateHandArray(m_cxSecondhand, &m_Seconds);
}

SIZE CAnalogClock::GetWindowSize(){
	SIZE size;
	size.cx = m_cxClockface->GetWidth();
	size.cy =  m_cxClockface->GetHeight();
	return size;
}

void CAnalogClock::Paint(HDC drawdc){
	RECT tmpRect = GetRect();
	if(m_nTimeZone >= 0){
		CTimeZone tzTime;
		tzTime.SetTimeZone(m_nTimeZone);
		m_time = tzTime.GetTime();
	}else{
		GetLocalTime(&m_time);
	}
	if(m_selected){
		HBRUSH hBrush;	
		hBrush = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		FillRect(drawdc, &tmpRect, hBrush);
		DeleteObject(hBrush);
	}
	m_cxClockface->Draw(drawdc, m_xpos, m_ypos);
	PaintMinuteAndHourHands(drawdc);
	if (m_bShowSecondHand)
		PaintSecondHand(drawdc);
	return;
}

float CAnalogClock::ConvertToDegrees(float dTime){
	float degrees = 0.;
	degrees = (float)(90. - dTime * 6.0);
	if (degrees < 0.0)
		degrees = (float)360. + degrees;
	return degrees;
}

void CAnalogClock::PaintMinuteAndHourHands(HDC pDC){
	// draw minute hand
	if(m_bShowMinuteHand){
		int minute = GetMinute();
		if(m_cxMinutehand != NULL){
			m_Minutes[minute].Draw(pDC, m_xpos + width / 2 - m_Minutes[minute].GetWidth() / 2, 
										m_ypos + height / 2 - m_Minutes[minute].GetHeight() /2);
		}else{
			int x1, y1, x2, y2;
			int horOff = m_xpos + width / 2;
			int verOff = m_ypos + height / 2;
			for( int i = 0; i < m_MinuteLines.size(); i++ ){
				m_MinuteLines[i].GetTransformedLine(&x1, &y1, &x2, &y2, minute );
				DrawLine(pDC, x1 + horOff, y1 + verOff, x2 + horOff, y2 + verOff, m_rgbMinuteHand );
			}
		}
	}
	
	// draw hour hand
	if(m_bShowHourHand){
		int hour = GetHour();
		if (hour >= 12)
			hour -= 12;
		hour = hour*5 + GetMinute()/12;
		if(m_cxHourhand != NULL){
			m_Hours[hour].Draw(pDC, m_xpos + width / 2 - m_Hours[hour].GetWidth() / 2, 
										m_ypos + height / 2 - m_Hours[hour].GetHeight() /2);
		}else{
			int x1, y1, x2, y2;
			int horOff = m_xpos + width / 2;
			int verOff = m_ypos + height / 2;
			for( int i = 0; i < m_HourLines.size(); i++ ){
				m_HourLines[i].GetTransformedLine(&x1, &y1, &x2, &y2, hour );
				DrawLine(pDC, x1 + horOff, y1 + verOff, x2 + horOff, y2 + verOff, m_rgbHourHand );
			}
		}
	}
}

void CAnalogClock::PaintSecondHand(HDC pDC){
	if (m_bShowSecondHand){
		if(m_cxSecondhand != NULL){
			m_Seconds[GetSecond()].Draw(pDC, m_xpos + width / 2 - m_Seconds[GetSecond()].GetWidth() / 2, 
										m_ypos + height / 2 - m_Seconds[GetSecond()].GetHeight() /2);
		}else{
			// draw second hand
			int x1, y1, x2, y2;
			int horOff = m_xpos + width / 2;
			int verOff = m_ypos + height / 2;
			for( int i = 0; i < m_SecondLines.size(); i++ ){
				m_SecondLines[i].GetTransformedLine(&x1, &y1, &x2, &y2, GetSecond() );
				DrawLine(pDC, x1 + horOff, y1 + verOff, x2 + horOff, y2 + verOff, m_rgbSecondHand );
			}
		}
	}
}

int CAnalogClock::GetSecond(){
	return m_time.wSecond;
}

int CAnalogClock::GetMinute(){
	return m_time.wMinute;
}

int CAnalogClock::GetHour(){
	int hour = m_time.wHour;
	if (hour > 12)
		hour -= 12;
	return hour;
}

int CAnalogClock::GetDay(){
	int day = m_time.wDay;
	return day;
}

void CAnalogClock::CreateHandArray(CxImage* original, std::vector<CxImage>* store){
	CxImage tmpImage;
	if(original != NULL){
		store->clear();
		store->push_back(*original);
		for(int i = 1; i < 60; i++ ){
			original->Rotate( i * 6 , &tmpImage);
			store->push_back(tmpImage);
		}
	}
}

RECT CAnalogClock::GetRect(){
	m_rect.left = m_xpos;
	m_rect.top = m_ypos;
	m_rect.right = m_xpos + width;
	m_rect.bottom = m_ypos + height;
	return m_rect;
}
