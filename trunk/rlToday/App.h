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

#if !defined __APP_H__

#include "skinitem.h"
#include "..\CxImage\ximage.h"
#include "helper.h"
class CApp : public SkinItem{
public:
	CApp();
	virtual ~CApp();
	//SkinItem Functions
	virtual void Paint(HDC hDC);
	virtual void SetSize(const int size){m_size = size;};
	virtual RECT GetRect();
protected:
	HICON		m_hIcon;
	CxImage*	m_cxImage;
	int m_size;
};
#endif // __APP_H__
