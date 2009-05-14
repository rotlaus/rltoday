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

#include "SkinItem.h"

SkinItem::SkinItem(void){
	m_selected = false;
}

SkinItem::~SkinItem(void){
}

void SkinItem::Paint(HDC hDC){
}

void SkinItem::LButtonDown(UINT nFlags, POINT point){
}

RECT SkinItem::GetRect(){
	return m_rect;
}

void SkinItem::LButtonUp(UINT nFlags, POINT point){
	for(int i = 0; i < m_FileList.size(); i++){
		if(m_FileList[i].FileName != L"")
			ShellOpen(m_FileList[i].FileName.c_str(), m_FileList[i].CommandLine.c_str());
	}
}

void SkinItem::SetApp(FILELIST app){
	m_FileList.push_back(app);
}
