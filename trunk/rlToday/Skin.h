#ifndef __SKIN_H__
#define __SKIN_H__
#include <windows.h>
#include <string>
#include <vector>
#include "tinyxml.h"
#include "Helper.h"
#include "skinitem.h"
#include "AnalogClock.h"
#include "App.h"
#include "Calendar.h"
#include "DigitalClock.h"
#include "Date.h"
#include "Text.h"
#include "Image.h"
#include "TimeZone.h"
#include "CalHeader.h"
#include "AppName.h"
#include "Registry.h"
#include "TZName.h"
class CSkin{
// Construction
public:
	CSkin(TSTRING* filename);
	~CSkin();
	enum NodeType{
		TODAYVALUES,
		ANALOGCLOCK,
		DIGITALCLOCK,
		DATE,
		TEXT,
		IMAGE,
		CALENDAR,
		CALENDARHEADER,
		TZNAME,
		APPLICATION,
		APPNAME,
		REGISTRY
	};
public:
	int Paint(HDC drawdc);
	void LButtonUp(UINT nFlags, POINT point);
	void LButtonDown(UINT nFlags, POINT point);
	void Parse();
	const int GetHeight(){return m_nHeight;};
	const int GetRefresh(){return m_nRefresh;};
	void SetNormalFont(HFONT font){m_hNormalFont = font;};
	void SetNormalFontColor(COLORREF color){m_rgbDefaultColor = color;};
	void SetTimeZoneVector(std::vector<CTimeZone> timezones){m_vTimeZones = timezones;};
	void SetFileListVector(std::vector<FILELIST> filelist){m_FileList = filelist;};
	void SetWeekDayAbbVector(std::vector<TSTRING> & weekdayabbs){m_vWeekDayAbbs = weekdayabbs;};
	void SetWeekStarts(int weekStart){m_WeekStarts = weekStart;};
// Implementation
protected:
	struct MyElement{
		//Objektzeiger
		void* m_Object;
		NodeType m_type;
	};
	void ClearSkin();
	void ParseNode(TiXmlNode* pParent);
	void ParseTodayValues(TiXmlElement* pParent);
	void ParseAnalogClock(TiXmlElement* pParent);
	void ParseDigitalClock(TiXmlElement* pParent);
	void ParseDate(TiXmlElement* pParent);
	void ParseText(TiXmlElement* pParent);
	void ParseTZName(TiXmlElement* pParent);
	void ParseImage(TiXmlElement* pParent);
	void ParseCalendar(TiXmlElement* pParent);
	void ParseCalHeader(TiXmlElement* pParent);
	void ParseApplication(TiXmlElement* pParent);
	HFONT ParseFont(TiXmlElement* pParent);
	void ParseClockHand(TiXmlElement* pParent, COLORREF* handColor, bool* show, TSTRING* source);
	void ParseAppName(TiXmlElement* pParent);
	void ParseRegistry(TiXmlElement* pParent);
	FILELIST ParseOnClick(TiXmlElement* pParent);
	TiXmlDocument *m_doc;
	// Der Vektor für alle gesammelten Skin Elemente
	std::vector<MyElement> m_Elements;
	// Der Vektor für alle Subskins
	std::vector<CSkin*> m_Subskins;
	// Zeiger für die zu erstellenden Objekte
	CAnalogClock*	m_eAnalogClock;
	CDate*			m_eDate;
	CDigitalClock*	m_eDigitalClock;
	CText*			m_eText;
	CImage*			m_eImage;
	CCalendar*		m_eCalendar;
	CCalHeader*		m_eCalHeader;
	CTZName*		m_eTZName;
	CApp*			m_eApp;
	CAppName*		m_eAppName;
	CRegistry*		m_eRegistry;
	// Pfadname für Skin Images
	TSTRING		m_sPathName;
	// Eigenschaften des Skins Files
	COLORREF	m_rgbTransparent;
	int			m_nHeight;
	int			m_nRefresh;
	HFONT		m_hNormalFont;
	COLORREF	m_rgbDefaultColor;
	std::vector<CTimeZone> m_vTimeZones;
	std::vector<TSTRING> m_vWeekDayAbbs;
	std::vector<FILELIST> m_FileList;
	int  m_WeekStarts;
};
#endif // __SKIN_H__
