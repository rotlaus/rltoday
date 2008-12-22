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
