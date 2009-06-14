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

