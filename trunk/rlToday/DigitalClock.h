#if !defined __DIGITALCLOCK_H__
#define __DIGITALCLOCK_H__
#define BUF_SIZE				256
#include "Helper.h"
#include "skinitem.h"
class CDigitalClock:public SkinItem{
public:
	CDigitalClock();
	virtual ~CDigitalClock();
	//SkinItem Functions
	virtual void Paint(HDC hDC);
	virtual void LButtonUp(UINT nFlags, POINT point);
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
#endif // __DIGITALCLOCK_H__

