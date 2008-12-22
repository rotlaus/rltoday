#ifndef _rlToday_h_
#define _rlToday_h_

#include "Skin.h"
#include "SimpleIni.h"
#include "Helper.h"
#include "TimeZone.h"
#include <vector>

/**************************************************************************
	Define
**************************************************************************/
#define WINDOW_TITLE			TEXT("rlToday")
#define MAIN_WND_CLASS			TEXT("rlTodayWndClass")
#define REG_PATH				TEXT("Software\\rlToday")
#define ID_CLOCK_TIMER			1
#define BUF_SIZE				256
#define ARRAY_SIZE(a) (sizeof((a)) / sizeof(*(a)))

/**************************************************************************
	Global Variables
**************************************************************************/
static HINSTANCE m_hInstance;
// Höhe des Plugins geändert
static BOOL Refresh;
// Höhe des Today Plugins
static int m_nItem_height = 40;
// Das zentrale Skin File
static CSkin* m_cSkin;
static HFONT		m_hNormalTodayFont;
static COLORREF		m_rgbTodayText;
static int			m_tInterval;
static TSTRING		m_sPathName;
static TSTRING		m_sSkinfile;

/**************************************************************************
	Function Prototypes
**************************************************************************/
int OnPaint(HWND, HDC, PAINTSTRUCT*, UINT);
BOOL APIENTRY CustomItemOptionsDlgProc(HWND , UINT , UINT , LONG );
void GetTodayDefaults(HWND hWnd);
void GetProgramFilePath();
void ReadConfiguration();
void SaveConfiguration();
void ReloadSkinFile();
void ReadSkinFile();
void OnLButtonUp(UINT nFlags, POINT point);
void OnLButtonDown(UINT nFlags, POINT point);
std::vector<CTimeZone> m_TimeZones;
int  m_WeekStarts;
std::vector<TSTRING> m_WeekDayAbbs;
std::vector<FILELIST> m_FileList;

/**************************************************************************
	Constants
**************************************************************************/
const __int64 nano100SecInDay=(__int64)10000000*60*60*24;

#endif // #ifndef _rlToday_h_
