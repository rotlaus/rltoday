#if !defined __REGISTRY_H__
#define __REGISTRY_H__

#include "skinitem.h"
class CRegistry : public SkinItem{
public:
	CRegistry();
	virtual ~CRegistry();
	void SetKey(TSTRING* key){reg_key = *key;};
	void SetUnknown(TSTRING* uk){unknown = *uk;};
	void SetFont(HFONT* font){m_hFont = *font;};
	void SetColor(COLORREF* color){m_rgbFontColor = *color;};
	void SetAlignment(UINT align){m_nAlignment = align;};
	//SkinItem Functions
	virtual void Paint(HDC hDC);
protected:
	TSTRING		reg_key;
	TSTRING		unknown;
	UINT		m_nAlignment;
	HFONT		m_hFont;
	COLORREF	m_rgbFontColor;
};
#endif // __REGISTRY_H__

