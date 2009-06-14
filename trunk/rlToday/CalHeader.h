#ifndef __CALHEADER_H__
#define __CALHEADER_H__

#define BUF_SIZE				256
#include "skinitem.h"

class CCalHeader : public SkinItem{
public:
	CCalHeader();
	virtual ~CCalHeader();
	virtual void Paint(HDC hDC);
	void SetFormat(TSTRING* format){date_format = *format;};
	void SetFont(HFONT* font){m_hFont = *font;};
	void SetColor(COLORREF* color){m_rgbFontColor = *color;};
	void SetAlignment(UINT align){m_nAlignment = align;};
	void SetOffset(const int offset);
protected:
	TSTRING		date_format;
	UINT		m_nAlignment;
	HFONT		m_hFont;
	COLORREF	m_rgbFontColor;
	int			m_Offset;
};
#endif // __CALHEADER_H__

