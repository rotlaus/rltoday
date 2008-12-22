#if !defined __Date_H__
#define __Date_H__
#define BUF_SIZE				256
#include "Helper.h"
#include "skinitem.h"

class CDate:public SkinItem{
public:
	CDate();
	virtual ~CDate();
	//SkinItem Functions
	virtual void Paint(HDC hDC);
	void SetFormat(TSTRING* format){date_format = *format;};
	void SetFont(HFONT* font){m_hFont = *font;};
	void SetColor(COLORREF* color){m_rgbFontColor = *color;};
	void SetAlignment(UINT align){m_nAlignment = align;};
	void SetTimeZone(int timezone){m_nTimeZone = timezone;};
protected:
	TSTRING		date_format;
	UINT		m_nAlignment;
	HFONT		m_hFont;
	COLORREF	m_rgbFontColor;
	int			m_nTimeZone;
};
#endif // __Date_H__

