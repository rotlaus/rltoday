#if !defined __TZNAME_H__
#define __TZNAME_H__

#include "Helper.h"
#include "skinitem.h"

class CTZName : public SkinItem {
public:
	CTZName();
	virtual ~CTZName();
	//SkinItem Functions
	virtual void Paint(HDC hDC);
	virtual void LButtonUp(UINT nFlags, POINT point);
	void SetTimeZoneName(TSTRING* format){m_TZName = *format;};
	void SetFont(HFONT* font){m_hFont = *font;};
	void SetColor(COLORREF* color){m_rgbFontColor = *color;};
	void SetAlignment(UINT align){m_nAlignment = align;};
protected:
	TSTRING		m_TZName;
	UINT		m_nAlignment;
	HFONT		m_hFont;
	COLORREF	m_rgbFontColor;
};
#endif // __TZNAME_H__

