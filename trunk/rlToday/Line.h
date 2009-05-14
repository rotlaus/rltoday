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

#ifndef __LINE_H__
#define __LINE_H__
#include <vector>
#include "Helper.h"
class CLine{
public:
	CLine(int x0, int y0, int x1, int y1, COLORREF color);
	virtual ~CLine();
	void SetColor(COLORREF color){m_rgbLineColor = color;};
	void GetTransformedLine(int* x0, int* y0, int* x1, int* y1, int index);
protected:
	COLORREF	m_rgbLineColor;
	std::vector<int> m_x0;
	std::vector<int> m_y0;
	std::vector<int> m_x1;
	std::vector<int> m_y1;
	void calcCoordinates(int x0, int y0, int x1, int y1);
};
#endif 
