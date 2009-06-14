//////////////////////////////////////////////////////////////////////
#include "Line.h"
#include "math.h"
#include "Helper.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLine::CLine(int x0, int y0, int x1, int y1, COLORREF color){
	m_rgbLineColor = color;
	calcCoordinates(x0, y0, x1, y1);
}

CLine::~CLine(){
}

void CLine::calcCoordinates(int x0, int y0, int x1, int y1){
	m_x0.push_back(x0);
	m_y0.push_back(y0);
	m_x1.push_back(x1);
	m_y1.push_back(y1);
	for( int i = 1; i < 60 ; i++){
		float radians	= (float) ((PI * (6. * i)) / 180.);
		float cosine	= (float)cos(radians);
		float sine		= (float)sin(radians);
		short x0_new	= (short)(x0 * cosine) - (y0 * sine);
		short y0_new	= (short)(x0 * sine) + (y0 * cosine);
		short x1_new	= (short)(x1 * cosine) - (y1 * sine);
		short y1_new	= (short)(x1 * sine) + (y1 * cosine);
		m_x0.push_back(x0_new);
		m_y0.push_back(y0_new);
		m_x1.push_back(x1_new);
		m_y1.push_back(y1_new);
	}
}

void CLine::GetTransformedLine(int* x0, int* y0, int* x1, int* y1, int index = 0){
	*x0 = m_x0[index];
	*y0 = m_y0[index];
	*x1 = m_x1[index];
	*y1 = m_y1[index];
}
