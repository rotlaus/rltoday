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

#if !defined __REGISTRY_H__
#define __REGISTRY_H__

#include "skinitem.h"
class CRegistry : public SkinItem{
public:
	CRegistry();
	virtual ~CRegistry();
	void SetKey(TSTRING* key){reg_key = *key;};
	void SetUnknown(TSTRING* uk){unknown = *uk;};
	void SetFont(HFONT* font){m_hFont = *font;};
	void SetColor(COLORREF* color){m_rgbFontColor = *color;};
	void SetAlignment(UINT align){m_nAlignment = align;};
	//SkinItem Functions
	virtual void Paint(HDC hDC);
protected:
	TSTRING		reg_key;
	TSTRING		unknown;
	UINT		m_nAlignment;
	HFONT		m_hFont;
	COLORREF	m_rgbFontColor;
};
#endif // __REGISTRY_H__

