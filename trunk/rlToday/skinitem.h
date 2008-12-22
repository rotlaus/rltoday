#ifndef __skinitem_h__
#define __skinitem_h__
#include <vector>
#include "Helper.h"
class SkinItem{
public:
	SkinItem(void);
	virtual ~SkinItem(void);
	virtual void Paint(HDC hDC);
	virtual void LButtonUp(UINT nFlags, POINT point);
	virtual void LButtonDown(UINT nFlags, POINT point);
	virtual void SetPosition(const int x, const int y){ m_xpos = x; m_ypos = y; };
	virtual void SetSelected(BOOL sel){m_selected = sel;};
	virtual RECT GetRect();
	void SetApp(FILELIST app);

protected:
	int m_xpos;
	int m_ypos;
	RECT m_rect;
	BOOL m_selected;
	std::vector<FILELIST> m_FileList;
};
#endif // __skinitem_h__
