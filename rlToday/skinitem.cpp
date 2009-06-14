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
