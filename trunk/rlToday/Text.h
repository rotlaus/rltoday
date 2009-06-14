#if !defined __Text_H__
#define __Text_H__
#define BUF_SIZE				256
#include "Helper.h"
#include "skinitem.h"
class CText:public SkinItem{
public:
	CText();
	virtual ~CText();
	void SetFormat(TSTRING* format){date_format = *format;};
	void SetFont(HFONT* font){m_hFont = *font;};
	void SetColor(COLORREF* color){m_rgbFontColor = *color;};
	void SetAlignment(UINT align){m_nAlignment = align;};
	//SkinItem Functions
	virtual void Paint(HDC hDC);
protected:
	TSTRING		date_format;
	UINT		m_nAlignment;
	HFONT		m_hFont;
	COLORREF	m_rgbFontColor;
};
#endif // __Text_H__

