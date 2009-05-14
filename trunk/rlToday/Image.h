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

#if !defined __Image_H__
#define __Image_H__
#include "Helper.h"
#include "..\CxImage\ximage.h"
#include "skinitem.h"
class CImage:public SkinItem{
public:
	CImage();
	~CImage();
	//SkinItem Functions
	virtual void Paint(HDC hDC);
	virtual void SetPosition(const int x, const int y){ m_xpos = x; m_ypos = y; };
	virtual RECT GetRect();
	void SetSource(TSTRING* source){
		m_sSource = *source;
		if(m_cxImage != NULL){
			delete m_cxImage;
			m_cxImage = NULL;
		}
		if(m_sSource != L""){
			m_cxImage = new CxImage();
			m_cxImage->Load(m_sSource.c_str());
		}
	}
protected:
	TSTRING		m_sSource;
	CxImage*	m_cxImage;
};
#endif // __Image_H__

