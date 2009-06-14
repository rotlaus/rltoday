#include "Skin.h"
#include "Helper.h"
#include <tchar.h>
#include <string>

CSkin::CSkin(TSTRING* filename){
	char filetoread[MAX_PATH];
	m_doc = new TiXmlDocument( Uni2Ascii(filename->c_str(), filetoread) );
	bool loadOkay = m_doc->LoadFile();
	m_rgbTransparent = RGB(255, 0, 255);
	m_sPathName = filename->substr(0, filename->rfind('\\'));
	m_vTimeZones.clear();
	m_nRefresh = 0;
	if ( !loadOkay ){
		TSTRING msg;
		msg = L"The Skin File " + *filename;
		msg += L"has erors or could not be found.";
		ErrorMessage(msg.c_str());
		return;
	}
}

void CSkin::ClearSkin(){
	for(int i=0;i < m_Elements.size(); i++){
		switch (m_Elements[i].m_type){
			case ANALOGCLOCK:
				m_eAnalogClock = NULL;
				m_eAnalogClock = (CAnalogClock*)m_Elements[i].m_Object;
				delete m_eAnalogClock;
				break;
			case DIGITALCLOCK:
				m_eDigitalClock = NULL;
				m_eDigitalClock = (CDigitalClock*)m_Elements[i].m_Object;
				delete m_eDigitalClock;
				break;
			case DATE:
				m_eDate = NULL;
				m_eDate = (CDate*)m_Elements[i].m_Object;
				delete m_eDate;
				break;
			case TEXT:
				m_eText = NULL;
				m_eText = (CText*)m_Elements[i].m_Object;
				delete m_eText;
				break;
			case IMAGE:
				m_eImage = NULL;
				m_eImage = (CImage*)m_Elements[i].m_Object;
				delete m_eImage;
				break;
			case CALENDAR:
				m_eCalendar = NULL;
				m_eCalendar = (CCalendar*)m_Elements[i].m_Object;
				delete m_eCalendar;
				break;
			case CALENDARHEADER:
				m_eCalHeader = NULL;
				m_eCalHeader = (CCalHeader*)m_Elements[i].m_Object;
				delete m_eCalHeader;
				break;
			case TZNAME:
				m_eTZName = NULL;
				m_eTZName = (CTZName*)m_Elements[i].m_Object;
				delete m_eTZName;
				break;
			case APPLICATION:
				m_eApp = NULL;
				m_eApp = (CApp*)m_Elements[i].m_Object;
				delete m_eApp;
				break;
			case APPNAME:
				m_eAppName = NULL;
				m_eAppName = (CAppName*)m_Elements[i].m_Object;
				delete m_eAppName;
				break;
			case REGISTRY:
				m_eRegistry = NULL;
				m_eRegistry = (CRegistry*)m_Elements[i].m_Object;
				delete m_eRegistry;
				break;
			default:
				break;
		}
	}
	m_Elements.clear();
}

CSkin::~CSkin(){
	delete m_doc;
	ClearSkin();
}

int CSkin::Paint(HDC drawdc){
	SkinItem* siTemp;
	for(int i=0;i < m_Elements.size(); i++)	{
		siTemp = (SkinItem*)m_Elements[i].m_Object;
		siTemp->Paint(drawdc);
	}
	return 0;
}

void CSkin::LButtonUp(UINT nFlags, POINT point){
	SkinItem* siTemp;
	RECT rcTemp;
	for(int i=0;i < m_Elements.size(); i++)	{	
		siTemp = (SkinItem*)m_Elements[i].m_Object;
		siTemp->SetSelected(false);
		rcTemp = siTemp->GetRect();
		if(PtInRect(&rcTemp, point)){
			siTemp->LButtonUp(nFlags, point);
		}
	}
}

void CSkin::LButtonDown(UINT nFlags, POINT point){
	SkinItem* siTemp;
	RECT rcTemp;
	for(int i=0;i < m_Elements.size(); i++)	{	
		siTemp = (SkinItem*)m_Elements[i].m_Object;
		rcTemp = siTemp->GetRect();
		if(PtInRect(&rcTemp, point)){
			siTemp->SetSelected(true);
			siTemp->LButtonDown(nFlags, point);
		}else{
			siTemp->SetSelected(false);
		}
	}
}

void CSkin::Parse(){
	ClearSkin();
	ParseNode(m_doc);
}

void CSkin::ParseNode(TiXmlNode* pParent){
	if ( !pParent ) return;
	TiXmlNode* pChild;
	TSTRING msg;
	int t = pParent->Type();
	static int count;
	// Uns interessiert hier nur das Root Element namens Today. 
	// Alles andere kommt in den einzelnen Today Elementen
	switch ( t ){
		case TiXmlNode::ELEMENT:
			msg = ctow(pParent->Value());
			if(!cmp_nocase(msg, L"today")){
				TiXmlElement* pElement = (TiXmlElement*)pParent;
				TiXmlAttribute* pAttrib=pElement->FirstAttribute();
				while(pAttrib){
					msg=ctow(pAttrib->Name());
					if(!cmp_nocase(L"transparent", ctow(pAttrib->Name()))){
						m_rgbTransparent=RGB2COLOR(ctow(pAttrib->Value()));
					}else if(!cmp_nocase(L"height", ctow(pAttrib->Name()))){
						pAttrib->QueryIntValue(&m_nHeight);
					}else if(!cmp_nocase(L"refresh", ctow(pAttrib->Name()))){
						pAttrib->QueryIntValue(&m_nRefresh);
					}
					pAttrib=pAttrib->Next();
				}
				ParseTodayValues((TiXmlElement*)pParent);
			}
			break;
		default:
			break;
	}
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
		ParseNode( pChild );
	return;
}

void CSkin::ParseTodayValues(TiXmlElement* pParent){
	if ( !pParent ) return;
	TiXmlNode* pChild;
	TSTRING msg;
	int t = pParent->Type();
	switch ( t ){
		case TiXmlNode::ELEMENT:
			msg = ctow(pParent->Value());
			if(!cmp_nocase(msg, L"analogclock")){
				ParseAnalogClock(pParent);
			}else if(!cmp_nocase(msg, L"application")){
				ParseApplication(pParent);
			}else if(!cmp_nocase(msg, L"appname")){
				ParseAppName(pParent);
			}else if(!cmp_nocase(msg, L"registry")){
				ParseRegistry(pParent);
			}else if(!cmp_nocase(msg, L"digitalclock")){
				ParseDigitalClock(pParent);
			}else if(!cmp_nocase(msg, L"date")){
				ParseDate(pParent);
			}else if(!cmp_nocase(msg, L"image")){
				ParseImage(pParent);
			}else if(!cmp_nocase(msg, L"text")){
				ParseText(pParent);
			}else if(!cmp_nocase(msg, L"calendar")){
				ParseCalendar(pParent);
			}else if(!cmp_nocase(msg, L"calendarheader")){
				ParseCalHeader(pParent);
			}else if(!cmp_nocase(msg, L"timezonename")){
				ParseTZName(pParent);
			}
			break;
		default:
			break;
	}
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
		ParseTodayValues((TiXmlElement*)pChild);
}

void CSkin::ParseAnalogClock(TiXmlElement* pParent){
	m_eAnalogClock = new CAnalogClock();
	int x=0;
	int y=0;
	int nTimeZone = -1;
	TSTRING image;
	TiXmlNode* pChild;
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"clockface", ctow(pAttrib->Name()))){
			image=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"timezone", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&nTimeZone);
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}
		pAttrib=pAttrib->Next();
	}
	m_eAnalogClock->SetPosition(x, y);
	if(nTimeZone >= 0){
		if(nTimeZone <= m_vTimeZones.size()){
			nTimeZone = m_vTimeZones[nTimeZone].GetTimeZone();
			m_eAnalogClock->SetTimeZone(nTimeZone);
		}
	}
	TSTRING path;
	path = m_sPathName + L"\\";
	image = path + TSTRING(image);
	m_eAnalogClock->SetClockface(image.c_str());

	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		COLORREF    handColor = RGB(0,0,0);
		bool		show = true;
		TSTRING     source = L"";
		if(!cmp_nocase(L"secondhand", ctow(pChild->Value()))){
			ParseClockHand((TiXmlElement*)pChild, &handColor, &show, &source);
			if(source != L"")
				image = path + source;
			else
				image = L"";
			m_eAnalogClock->SetSecondHandColor(handColor);
			m_eAnalogClock->ShowSecondHand(show);
			m_eAnalogClock->SetSecondHand(image.c_str());
		}else if(!cmp_nocase(L"minutehand", ctow(pChild->Value()))){
			ParseClockHand((TiXmlElement*)pChild, &handColor, &show, &source);
			if(source != L"")
				image = path + source;
			else
				image = L"";
			m_eAnalogClock->SetMinuteHandColor(handColor);
			m_eAnalogClock->ShowMinuteHand(show);
			m_eAnalogClock->SetMinuteHand(image.c_str());
		}else if(!cmp_nocase(L"hourhand", ctow(pChild->Value()))){
			ParseClockHand((TiXmlElement*)pChild, &handColor, &show, &source);
			if(source != L"")
				image = path + source;
			else
				image = L"";
			m_eAnalogClock->SetHourHandColor(handColor);
			m_eAnalogClock->ShowHourHand(show);
			m_eAnalogClock->SetHourHand(image.c_str());
		}else if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
			FILELIST myFileList;;
			myFileList = ParseOnClick((TiXmlElement*)pChild);
			m_eAnalogClock->SetApp(myFileList);
		}
	}
	MyElement newElement;
	newElement.m_Object = m_eAnalogClock;
	newElement.m_type = ANALOGCLOCK;
	m_Elements.push_back(newElement);
}

void CSkin::ParseDigitalClock(TiXmlElement* pParent){
	int x=0;
	int y=0;
	int nTimeZone = -1; 
	TSTRING format;
	TiXmlNode* pChild;
	COLORREF fontcolor = m_rgbDefaultColor;
	m_eDigitalClock = new CDigitalClock();
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"format", ctow(pAttrib->Name()))){
			format=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			fontcolor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"timezone", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&nTimeZone);
		}else if(!cmp_nocase(L"alignment", ctow(pAttrib->Name()))){
			if(!cmp_nocase(ctow(pAttrib->Value()), L"left")){
				m_eDigitalClock->SetAlignment(DT_LEFT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"right")){
				m_eDigitalClock->SetAlignment(DT_RIGHT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"center")){
				m_eDigitalClock->SetAlignment(DT_CENTER);
			}
		}
		pAttrib=pAttrib->Next();
	}
	m_eDigitalClock->SetPosition(x, y);
	m_eDigitalClock->SetFormat(&format);
	m_eDigitalClock->SetFont(&m_hNormalFont);
	m_eDigitalClock->SetColor(&fontcolor);
	if(nTimeZone >= 0){
		if(nTimeZone <= m_vTimeZones.size()){
			nTimeZone = m_vTimeZones[nTimeZone].GetTimeZone();
			m_eDigitalClock->SetTimeZone(nTimeZone);
		}
	}

	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		if(!cmp_nocase(L"font", ctow(pChild->Value()))){
			HFONT hTmpFont;
			hTmpFont = ParseFont((TiXmlElement*)pChild);
			m_eDigitalClock->SetFont(&hTmpFont);
		}else if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
			FILELIST myFileList;;
			myFileList = ParseOnClick((TiXmlElement*)pChild);
			m_eDigitalClock->SetApp(myFileList);
		}
	}
	MyElement newElement;
	newElement.m_Object = m_eDigitalClock;
	newElement.m_type = DIGITALCLOCK;
	m_Elements.push_back(newElement);
}

void CSkin::ParseDate(TiXmlElement* pParent){
	int x=0;
	int y=0;
	TSTRING format;
	TiXmlNode* pChild;
	COLORREF fontcolor = m_rgbDefaultColor;
	int nTimeZone = -1; 
	m_eDate = new CDate();
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"format", ctow(pAttrib->Name()))){
			format=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			fontcolor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"timezone", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&nTimeZone);
		}else if(!cmp_nocase(L"alignment", ctow(pAttrib->Name()))){
			if(!cmp_nocase(ctow(pAttrib->Value()), L"left")){
				m_eDate->SetAlignment(DT_LEFT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"right")){
				m_eDate->SetAlignment(DT_RIGHT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"center")){
				m_eDate->SetAlignment(DT_CENTER);
			}
		}
		pAttrib=pAttrib->Next();
	}
	m_eDate->SetPosition(x, y);
	m_eDate->SetFormat(&format);
	m_eDate->SetFont(&m_hNormalFont);
	m_eDate->SetColor(&fontcolor);
	if(nTimeZone >= 0){
		if(nTimeZone <= m_vTimeZones.size()){
			nTimeZone = m_vTimeZones[nTimeZone].GetTimeZone();
			m_eDate->SetTimeZone(nTimeZone);
		}
	}
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		if(!cmp_nocase(L"font", ctow(pChild->Value()))){
			HFONT hTmpFont;
			hTmpFont = ParseFont((TiXmlElement*)pChild);
			m_eDate->SetFont(&hTmpFont);
		}else if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
			FILELIST myFileList;;
			myFileList = ParseOnClick((TiXmlElement*)pChild);
			m_eDate->SetApp(myFileList);
		}
	}
	MyElement newElement;
	newElement.m_Object = m_eDate;
	newElement.m_type = DATE;
	m_Elements.push_back(newElement);
}

void CSkin::ParseApplication(TiXmlElement* pParent){
	int x=0;
	int y=0;
	int size=32;
	int number=-1;
	TSTRING source;
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"source", ctow(pAttrib->Name()))){
			source=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"size", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&size);
		}else if(!cmp_nocase(L"appnumber", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&number);
		}
		pAttrib=pAttrib->Next();
	}

	if(( number >= 0) && ( number < m_FileList.size() )){
		m_eApp = new CApp();
		m_eApp->SetPosition(x, y);
		m_eApp->SetApp(m_FileList[number]);
		m_eApp->SetSize(size);
		MyElement newElement;
		newElement.m_Object = m_eApp;
		newElement.m_type = APPLICATION;
		m_Elements.push_back(newElement);
	}
}

void CSkin::ParseAppName(TiXmlElement* pParent){
	int x=0;
	int y=0;
	TSTRING format;
	TiXmlNode* pChild;
	COLORREF fontcolor = m_rgbDefaultColor;
	int number=-1;
	m_eAppName = new CAppName();
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			fontcolor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"alignment", ctow(pAttrib->Name()))){
			if(!cmp_nocase(ctow(pAttrib->Value()), L"left")){
				m_eAppName->SetAlignment(DT_LEFT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"right")){
				m_eAppName->SetAlignment(DT_RIGHT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"center")){
				m_eAppName->SetAlignment(DT_CENTER);
			}
		}else if(!cmp_nocase(L"appnumber", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&number);
		}
		pAttrib=pAttrib->Next();
	}
	m_eAppName->SetPosition(x, y);
	m_eAppName->SetFont(&m_hNormalFont);
	m_eAppName->SetColor(&fontcolor);
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		if(!cmp_nocase(L"font", ctow(pChild->Value()))){
			HFONT hTmpFont;
			hTmpFont = ParseFont((TiXmlElement*)pChild);
			m_eAppName->SetFont(&hTmpFont);
		}
	}
	if(( number >= 0) && ( number < m_FileList.size() )){
		m_eAppName->SetApp(m_FileList[number]);
		MyElement newElement;
		newElement.m_Object = m_eAppName;
		newElement.m_type = APPNAME;
		m_Elements.push_back(newElement);
	}
}

void CSkin::ParseText(TiXmlElement* pParent){
	int x=0;
	int y=0;
	TSTRING format;
	TiXmlNode* pChild;
	COLORREF fontcolor = m_rgbDefaultColor;
	m_eText = new CText();
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"text", ctow(pAttrib->Name()))){
			format=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			fontcolor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"alignment", ctow(pAttrib->Name()))){
			if(!cmp_nocase(ctow(pAttrib->Value()), L"left")){
				m_eText->SetAlignment(DT_LEFT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"right")){
				m_eText->SetAlignment(DT_RIGHT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"center")){
				m_eText->SetAlignment(DT_CENTER);
			}
		}
		pAttrib=pAttrib->Next();
	}
	m_eText->SetPosition(x, y);
	m_eText->SetFormat(&format);
	m_eText->SetFont(&m_hNormalFont);
	m_eText->SetColor(&fontcolor);
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		if(!cmp_nocase(L"font", ctow(pChild->Value()))){
			HFONT hTmpFont;
			hTmpFont = ParseFont((TiXmlElement*)pChild);
			m_eText->SetFont(&hTmpFont);
		}else if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
			FILELIST myFileList;;
			myFileList = ParseOnClick((TiXmlElement*)pChild);
			m_eText->SetApp(myFileList);
		}
	}
	MyElement newElement;
	newElement.m_Object = m_eText;
	newElement.m_type = TEXT;
	m_Elements.push_back(newElement);
}

void CSkin::ParseRegistry(TiXmlElement* pParent){
	int x=0;
	int y=0;
	TSTRING format;
	TSTRING unknown;
	TiXmlNode* pChild;
	COLORREF fontcolor = m_rgbDefaultColor;
	m_eRegistry = new CRegistry();
	unknown = L"xxx";
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"key", ctow(pAttrib->Name()))){
			format=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"unknown", ctow(pAttrib->Name()))){
			unknown=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			fontcolor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"alignment", ctow(pAttrib->Name()))){
			if(!cmp_nocase(ctow(pAttrib->Value()), L"left")){
				m_eRegistry->SetAlignment(DT_LEFT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"right")){
				m_eRegistry->SetAlignment(DT_RIGHT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"center")){
				m_eRegistry->SetAlignment(DT_CENTER);
			}
		}
		pAttrib=pAttrib->Next();
	}
	m_eRegistry->SetPosition(x, y);
	m_eRegistry->SetKey(&format);
	m_eRegistry->SetUnknown(&unknown);
	m_eRegistry->SetFont(&m_hNormalFont);
	m_eRegistry->SetColor(&fontcolor);
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		if(!cmp_nocase(L"font", ctow(pChild->Value()))){
			HFONT hTmpFont;
			hTmpFont = ParseFont((TiXmlElement*)pChild);
			m_eRegistry->SetFont(&hTmpFont);
		}else if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
			FILELIST myFileList;;
			myFileList = ParseOnClick((TiXmlElement*)pChild);
			m_eRegistry->SetApp(myFileList);
		}
	}
	MyElement newElement;
	newElement.m_Object = m_eRegistry;
	newElement.m_type = REGISTRY;
	m_Elements.push_back(newElement);
}

void CSkin::ParseImage(TiXmlElement* pParent){
	int x=0;
	int y=0;
	TSTRING source;
	TiXmlNode* pChild;
	m_eImage = new CImage();
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"source", ctow(pAttrib->Name()))){
			source=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}
		pAttrib=pAttrib->Next();
	}
	m_eImage->SetPosition(x, y);
	TSTRING image;
	image = m_sPathName + L"\\";
	source = image + TSTRING(source);
	m_eImage->SetSource(&source);
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
			FILELIST myFileList;;
			myFileList = ParseOnClick((TiXmlElement*)pChild);
			m_eImage->SetApp(myFileList);
		}
	}
	MyElement newElement;
	newElement.m_Object = m_eImage;
	newElement.m_type = IMAGE;
	m_Elements.push_back(newElement);
}

void CSkin::ParseCalendar(TiXmlElement* pParent){
	int x = 0;
	int y = 0;
	int r_x = 12;
	int r_y = 12;
	int offset = 0;
	TSTRING image;
	TiXmlNode* pChild;
	COLORREF fontcolor = m_rgbDefaultColor;
	COLORREF fontcolor2 = m_rgbDefaultColor;
	COLORREF fontcolor3 = m_rgbDefaultColor;
	COLORREF fontcolor4 = m_rgbDefaultColor;
	bool toColorSet = false;
	m_eCalendar = new CCalendar();
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"todayimage", ctow(pAttrib->Name()))){
			image=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"offset", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&offset);
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"rasterx", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&r_x);
		}else if(!cmp_nocase(L"rastery", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&r_y);
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			fontcolor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"saturdaycolor", ctow(pAttrib->Name()))){
			fontcolor2 = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"sundaycolor", ctow(pAttrib->Name()))){
			fontcolor3 = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"todaycolor", ctow(pAttrib->Name()))){
			fontcolor4 = RGB2COLOR(ctow(pAttrib->Value()));
			toColorSet = true;
		}
		pAttrib=pAttrib->Next();
	}

	if(image!= L""){
		TSTRING path;
		path = m_sPathName + L"\\";
		image = path + TSTRING(image);
		m_eCalendar->SetTodayImage(image.c_str());
	}
	m_eCalendar->SetPosition(x, y);
	m_eCalendar->SetRaster(r_x, r_y);
	m_eCalendar->SetOffset(offset);
	m_eCalendar->SetFont(&m_hNormalFont);
	m_eCalendar->SetColor(&fontcolor);
	m_eCalendar->SetSaColor(&fontcolor2);
	m_eCalendar->SetSuColor(&fontcolor3);
	if(toColorSet == true)
		m_eCalendar->SetToColor(&fontcolor4);
	m_eCalendar->SetWeekDayAbbVector(m_vWeekDayAbbs);
	m_eCalendar->SetWeekStarts(m_WeekStarts);
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		if(!cmp_nocase(L"font", ctow(pChild->Value()))){
			HFONT hTmpFont;
			hTmpFont = ParseFont((TiXmlElement*)pChild);
			m_eCalendar->SetFont(&hTmpFont);
		}else if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
			FILELIST myFileList;;
			myFileList = ParseOnClick((TiXmlElement*)pChild);
			m_eCalendar->SetApp(myFileList);
		}
	}
	MyElement newElement;
	newElement.m_Object = m_eCalendar;
	newElement.m_type = CALENDAR;
	m_Elements.push_back(newElement);
}

HFONT CSkin::ParseFont(TiXmlElement* pParent){
	LOGFONT lf;
	HFONT hSysFont;
	HFONT hRetFont;
	// Default today fonts
	hSysFont = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hSysFont, sizeof(LOGFONT), &lf);
	_tcscpy(lf.lfFaceName, _T("Tahoma"));
	lf.lfHeight = (long)-11;
	// Normal text
	lf.lfWeight = FW_BOLD;
	int iVal;
	TSTRING sVal;
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"font", ctow(pAttrib->Name()))){
			sVal = L"";
			sVal=ctow(pAttrib->Value());
			if(sVal != L"")
				_tcscpy(lf.lfFaceName, sVal.c_str());
		}else if(!cmp_nocase(L"weight", ctow(pAttrib->Name()))){
			sVal = L"";
			sVal=ctow(pAttrib->Value());
			if(!cmp_nocase(sVal.c_str(), L"bold"))
				lf.lfWeight = FW_BOLD;
			else
				lf.lfWeight = FW_NORMAL;
		}else if(!cmp_nocase(L"size", ctow(pAttrib->Name()))){
			iVal = 0;
			pAttrib->QueryIntValue(&iVal);
			if(iVal != 0)
				lf.lfHeight = (long)-iVal;
		}
		pAttrib=pAttrib->Next();
	}
	hRetFont = CreateFontIndirect(&lf);
	return hRetFont;
}

FILELIST CSkin::ParseOnClick(TiXmlElement* pParent){
	FILELIST tmpFileList;
	TSTRING sVal;
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"file", ctow(pAttrib->Name()))){
			sVal = L"";
			sVal=ctow(pAttrib->Value());
			if(sVal != L"")
				tmpFileList.FileName = sVal;
		}else if(!cmp_nocase(L"command", ctow(pAttrib->Name()))){
			sVal = L"";
			sVal=ctow(pAttrib->Value());
			if(sVal != L"")
				tmpFileList.CommandLine = sVal;
		}
		pAttrib=pAttrib->Next();
	}
	return tmpFileList;
}

void CSkin::ParseClockHand(TiXmlElement* pParent, COLORREF* handColor, bool* show, TSTRING* source){
	TSTRING sVal;
	*show = TRUE;
	*handColor = RGB(0, 0, 0);
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"show", ctow(pAttrib->Name()))){
			*show = TRUE;
			sVal = L"";
			sVal=ctow(pAttrib->Value());
			if(!cmp_nocase(sVal.c_str(), L"false"))
				*show = false;
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			*handColor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"source", ctow(pAttrib->Name()))){
			*source = ctow(pAttrib->Value());
		}
		pAttrib=pAttrib->Next();
	}
}

void CSkin::ParseCalHeader(TiXmlElement* pParent){
	int x=0;
	int y=0;
	TSTRING format;
	TiXmlNode* pChild;
	COLORREF fontcolor = m_rgbDefaultColor;
	int offset = 0;
	m_eCalHeader = new CCalHeader();
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"offset", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&offset);
		}else if(!cmp_nocase(L"format", ctow(pAttrib->Name()))){
			format=ctow(pAttrib->Value());
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			fontcolor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"alignment", ctow(pAttrib->Name()))){
			if(!cmp_nocase(ctow(pAttrib->Value()), L"left")){
				m_eCalHeader->SetAlignment(DT_LEFT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"right")){
				m_eCalHeader->SetAlignment(DT_RIGHT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"center")){
				m_eCalHeader->SetAlignment(DT_CENTER);
			}
		}
		pAttrib=pAttrib->Next();
	}
	m_eCalHeader->SetPosition(x, y);
	m_eCalHeader->SetFormat(&format);
	m_eCalHeader->SetFont(&m_hNormalFont);
	m_eCalHeader->SetColor(&fontcolor);
	m_eCalHeader->SetOffset(offset);
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
		if(!cmp_nocase(L"font", ctow(pChild->Value()))){
			HFONT hTmpFont;
			hTmpFont = ParseFont((TiXmlElement*)pChild);
			m_eCalHeader->SetFont(&hTmpFont);
		}else if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
			FILELIST myFileList;;
			myFileList = ParseOnClick((TiXmlElement*)pChild);
			m_eCalHeader->SetApp(myFileList);
		}
	}
	MyElement newElement;
	newElement.m_Object = m_eCalHeader;
	newElement.m_type = CALENDARHEADER;
	m_Elements.push_back(newElement);
}

void CSkin::ParseTZName(TiXmlElement* pParent){
	int x=0;
	int y=0;
	TiXmlNode* pChild;
	COLORREF fontcolor = m_rgbDefaultColor;
	int nTimeZone = -1;
	m_eTZName = new CTZName();
	TiXmlAttribute* pAttrib=pParent->FirstAttribute();
	while(pAttrib){
		if(!cmp_nocase(L"timezone", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&nTimeZone);
		}else if(!cmp_nocase(L"x", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&x);
		}else if(!cmp_nocase(L"y", ctow(pAttrib->Name()))){
			pAttrib->QueryIntValue(&y);
		}else if(!cmp_nocase(L"color", ctow(pAttrib->Name()))){
			fontcolor = RGB2COLOR(ctow(pAttrib->Value()));
		}else if(!cmp_nocase(L"alignment", ctow(pAttrib->Name()))){
			if(!cmp_nocase(ctow(pAttrib->Value()), L"left")){
				m_eTZName->SetAlignment(DT_LEFT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"right")){
				m_eTZName->SetAlignment(DT_RIGHT);
			}else if(!cmp_nocase(ctow(pAttrib->Value()), L"center")){
				m_eTZName->SetAlignment(DT_CENTER);
			}
		}
		pAttrib=pAttrib->Next();
	}
	m_eTZName->SetPosition(x, y);
	if( (nTimeZone >= 0) && ( nTimeZone < m_vTimeZones.size() )){
		TSTRING tz;
		tz = m_vTimeZones[nTimeZone].GetName();
		m_eTZName->SetTimeZoneName( &tz );
		m_eTZName->SetFont(&m_hNormalFont);
		m_eTZName->SetColor(&fontcolor);
		
		for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()){
			if(!cmp_nocase(L"font", ctow(pChild->Value()))){
				HFONT hTmpFont;
				hTmpFont = ParseFont((TiXmlElement*)pChild);
				m_eTZName->SetFont(&hTmpFont);
			}else if(!cmp_nocase(L"onclick", ctow(pChild->Value()))){
				FILELIST myFileList;;
				myFileList = ParseOnClick((TiXmlElement*)pChild);
				m_eTZName->SetApp(myFileList);
			}
		}
		MyElement newElement;
		newElement.m_Object = m_eTZName;
		newElement.m_type = TZNAME;
		m_Elements.push_back(newElement);
	}
}
