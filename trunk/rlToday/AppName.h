#if !defined __APPNAME_H__
#define __APPNAME_H__

#include "skinitem.h"
#include "helper.h"
class CAppName : public SkinItem{
public:
	CAppName();
	virtual ~CAppName();
	//SkinItem Functions
	virtual void Paint(HDC hDC);
	void SetFont(HFONT* font){m_hFont = *font;};
	void SetColor(COLORREF* color){m_rgbFontColor = *color;};
	void SetAlignment(UINT align){m_nAlignment = align;};
protected:
	HFONT		m_hFont;
	COLORREF	m_rgbFontColor;
	UINT		m_nAlignment;
};
#endif // __APPNAME_H__
