#ifndef __HELPER_H__
#define __HELPER_H__

#include <windows.h>
#include <string>

#ifdef _UNICODE
# define TSTRING std::wstring
#else
# define TSTRING std::string
#endif

#ifndef PI
#define PI					3.1415926535
#endif

void DrawLine(HDC pDC, int x1, int y1, int x2, int y2, COLORREF m_rgbMinuteHand);
wchar_t* Ascii2Uni(const char* SomeAsciiStr, wchar_t* buffer);
char* Uni2Ascii(const TCHAR* SomeUnicodeStr, char* buffer);
void Message(const TCHAR* textToSay);
void Message(const TSTRING* textToSay);
void ErrorMessage(const TCHAR* textToSay);
void ErrorMessage(const TSTRING* textToSay);
std::wstring ctow(const char* src);
int cmp_nocase(const TSTRING& s, const TSTRING& s2);
COLORREF RGB2COLOR(const TSTRING& color);
void Add2Log(const TCHAR* message);
int MyAtoI(const TCHAR* asciizahl);
void ShellOpen(const TCHAR *FileName, const TCHAR *CommnadLine);
std::wstring GetRegistry(const TSTRING& key);
typedef struct _FILELIST {
	TSTRING Name;
	TSTRING FileName;
	TSTRING CommandLine;
	TSTRING IconPath;
	int IconResource;
	int Index;
} FILELIST;

#endif // __HELPER_H__
