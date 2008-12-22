#ifndef __LINE_H__
#define __LINE_H__
#include <vector>
#include "Helper.h"
class CLine{
public:
	CLine(int x0, int y0, int x1, int y1, COLORREF color);
	virtual ~CLine();
	void SetColor(COLORREF color){m_rgbLineColor = color;};
	void GetTransformedLine(int* x0, int* y0, int* x1, int* y1, int index);
protected:
	COLORREF	m_rgbLineColor;
	std::vector<int> m_x0;
	std::vector<int> m_y0;
	std::vector<int> m_x1;
	std::vector<int> m_y1;
	void calcCoordinates(int x0, int y0, int x1, int y1);
};
#endif 
