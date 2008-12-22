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
