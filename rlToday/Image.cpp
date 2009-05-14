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

#include "Image.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CImage::CImage(){
	m_xpos = 0;
	m_ypos = 0;
	m_sSource = L"";
	m_cxImage = NULL;
}

CImage::~CImage(){
	if(m_cxImage != NULL)
		delete m_cxImage;
}

void CImage::Paint(HDC drawdc){
	if(m_cxImage != NULL)
		m_cxImage->Draw(drawdc, m_xpos, m_ypos);
}

RECT CImage::GetRect(){
	m_rect.left = m_xpos;
	m_rect.top = m_ypos;
	m_rect.right = m_xpos + m_cxImage->GetWidth();
	m_rect.bottom = m_ypos + m_cxImage->GetHeight();
	return m_rect;
}
