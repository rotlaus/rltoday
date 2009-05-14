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

#ifndef __skinitem_h__
#define __skinitem_h__
#include <vector>
#include "Helper.h"
class SkinItem{
public:
	SkinItem(void);
	virtual ~SkinItem(void);
	virtual void Paint(HDC hDC);
	virtual void LButtonUp(UINT nFlags, POINT point);
	virtual void LButtonDown(UINT nFlags, POINT point);
	virtual void SetPosition(const int x, const int y){ m_xpos = x; m_ypos = y; };
	virtual void SetSelected(BOOL sel){m_selected = sel;};
	virtual RECT GetRect();
	void SetApp(FILELIST app);

protected:
	int m_xpos;
	int m_ypos;
	RECT m_rect;
	BOOL m_selected;
	std::vector<FILELIST> m_FileList;
};
#endif // __skinitem_h__
