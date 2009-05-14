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

#include "App.h"

CApp::CApp(){
	m_xpos = 0;
	m_ypos = 0;
	m_cxImage = NULL;
	m_hIcon = NULL;
	m_size = 16;
}

CApp::~CApp(){
	if(m_cxImage != NULL)
		delete m_cxImage;
	if(m_hIcon != NULL)
		DestroyIcon(m_hIcon);
}

void CApp::Paint(HDC drawdc){
	RECT tmpRect = GetRect();
	if((m_cxImage == NULL) && (m_hIcon==NULL)){
		TCHAR buffer[256];
		swprintf(buffer, _T("%s"), m_FileList[0].FileName.c_str());
		ExtractIconEx(buffer, 0, &m_hIcon, NULL,  1);
		if(m_FileList[0].IconPath != L""){
			m_cxImage = new CxImage();
			m_cxImage->Load(m_FileList[0].IconPath.c_str());
		}else{
			ExtractIconEx(buffer, 0, &m_hIcon, NULL,  1);
		}
	}
	if(m_selected){
		HBRUSH hBrush;	
		hBrush = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		FillRect(drawdc, &tmpRect, hBrush);
		DeleteObject(hBrush);
	}
	if(m_cxImage == NULL)
		DrawIconEx(drawdc, m_xpos, m_ypos, m_hIcon, m_size, m_size, 0, NULL, DI_NORMAL);
	else
		m_cxImage->Draw(drawdc, m_xpos, m_ypos, m_size, m_size);
}

RECT CApp::GetRect(){
	m_rect.left = m_xpos;
	m_rect.top = m_ypos;
	m_rect.right = m_xpos + m_size;
	m_rect.bottom = m_ypos + m_size;
	return m_rect;
}
