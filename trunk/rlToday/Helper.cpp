#include "Helper.h"
#include <vector>

void DrawWuLine(HDC *pDC, int X0, int Y0, int X1, int Y1, COLORREF clrLine){
	/* Make sure the line runs top to bottom */
	if (Y0 > Y1){
		int Temp = Y0; Y0 = Y1; Y1 = Temp;
		Temp = X0; X0 = X1; X1 = Temp;
	}
	/* Draw the initial pixel, which is always exactly intersected by
	the line and so needs no weighting */
	SetPixel(*pDC, X0, Y0, clrLine);
	int XDir, DeltaX = X1 - X0;
	if(DeltaX >= 0){
		XDir = 1;
	}else{
		XDir   = -1;
		DeltaX = 0 - DeltaX; /* make DeltaX positive */
	}
	/* Special-case horizontal, vertical, and diagonal lines, which
	require no weighting because they go right through the center of
	every pixel */
	int DeltaY = Y1 - Y0;
	if (DeltaY == 0){
		/* Horizontal line */
		while (DeltaX-- != 0){
			X0 += XDir;
			SetPixel(*pDC, X0, Y0, clrLine);
		}
		return;
	}
	if (DeltaX == 0){
		/* Vertical line */
		do{
			Y0++;
			SetPixel(*pDC, X0, Y0, clrLine);
		} while (--DeltaY != 0);
		return;
	}
	if (DeltaX == DeltaY){
		/* Diagonal line */
		do{
			X0 += XDir;
			Y0++;
			SetPixel(*pDC, X0, Y0, clrLine);
		} while (--DeltaY != 0);
		return;
	}
	
	unsigned short ErrorAdj;
	unsigned short ErrorAccTemp, Weighting;
	/* Line is not horizontal, diagonal, or vertical */
	unsigned short ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
	BYTE rl = GetRValue(clrLine);
	BYTE gl = GetGValue(clrLine);
	BYTE bl = GetBValue(clrLine);
	double grayl = rl * 0.299 + gl * 0.587 + bl * 0.114;
	/* Is this an X-major or Y-major line? */
	if (DeltaY > DeltaX){
		/* Y-major line; calculate 16-bit fixed-point fractional part of a
		pixel that X advances each time Y advances 1 pixel, truncating the
		result so that we won't overrun the endpoint along the X axis */
		ErrorAdj = (unsigned short)
			(((unsigned long) DeltaX << 16) / (unsigned long) DeltaY);
		/* Draw all pixels other than the first and last */
		while (--DeltaY){
			ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
			ErrorAcc = (unsigned short)(ErrorAcc + ErrorAdj);      /* calculate error for next pixel */
			if (ErrorAcc <= ErrorAccTemp)
			{
				/* The error accumulator turned over, so advance the X coord */
				X0 += XDir;
			}
			Y0++; /* Y-major, so always advance Y */
			/* The IntensityBits most significant bits of ErrorAcc give us the
			intensity weighting for this pixel, and the complement of the
			weighting for the paired pixel */
			Weighting = (unsigned short) (ErrorAcc >> 8);
			ASSERT(Weighting < 256);
			ASSERT((Weighting ^ 255) < 256);
			COLORREF clrBackGround = GetPixel(*pDC, X0, Y0);
			BYTE rb = GetRValue(clrBackGround);
			BYTE gb = GetGValue(clrBackGround);
			BYTE bb = GetBValue(clrBackGround);
			double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;
			BYTE rr = (rb > rl ? ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (rb - rl) + rl)) : ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (rl - rb) + rb)));
			BYTE gr = (gb > gl ? ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (gb - gl) + gl)) : ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (gl - gb) + gb)));
			BYTE br = (bb > bl ? ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (bb - bl) + bl)) : ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (bl - bb) + bb)));
			SetPixel(*pDC, X0, Y0, RGB(rr, gr, br));
			clrBackGround = ::GetPixel(*pDC, X0 + XDir, Y0);
			rb = GetRValue(clrBackGround);
			gb = GetGValue(clrBackGround);
			bb = GetBValue(clrBackGround);
			grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;
			rr = (rb > rl ? ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (rb - rl) + rl)) : ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (rl - rb) + rb)));
			gr = (gb > gl ? ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (gb - gl) + gl)) : ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (gl - gb) + gb)));
			br = (bb > bl ? ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (bb - bl) + bl)) : ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (bl - bb) + bb)));
			SetPixel(*pDC, X0 + XDir, Y0, RGB(rr, gr, br));
		}
		/* Draw the final pixel, which is always exactly intersected by the line
		and so needs no weighting */
		SetPixel(*pDC, X1, Y1, clrLine);
		return;
	}
	/* It's an X-major line; calculate 16-bit fixed-point fractional part of a
	pixel that Y advances each time X advances 1 pixel, truncating the
	result to avoid overrunning the endpoint along the X axis */
	ErrorAdj = (unsigned short)(((unsigned long) DeltaY << 16) / (unsigned long) DeltaX);
	/* Draw all pixels other than the first and last */
	while (--DeltaX){
		ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
		ErrorAcc = (unsigned short)(ErrorAcc + ErrorAdj);      /* calculate error for next pixel */
		if (ErrorAcc <= ErrorAccTemp){
			/* The error accumulator turned over, so advance the Y coord */
			Y0++;
		}
		X0 += XDir; /* X-major, so always advance X */
		/* The IntensityBits most significant bits of ErrorAcc give us the
		intensity weighting for this pixel, and the complement of the
		weighting for the paired pixel */
		Weighting = (unsigned short) (ErrorAcc >> 8);
		ASSERT(Weighting < 256);
		ASSERT((Weighting ^ 255) < 256);
		COLORREF clrBackGround = GetPixel(*pDC, X0, Y0);
		BYTE rb = GetRValue(clrBackGround);
		BYTE gb = GetGValue(clrBackGround);
		BYTE bb = GetBValue(clrBackGround);
		double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;
		BYTE rr = (rb > rl ? ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (rb - rl) + rl)) : ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (rl - rb) + rb)));
		BYTE gr = (gb > gl ? ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (gb - gl) + gl)) : ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (gl - gb) + gb)));
		BYTE br = (bb > bl ? ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (bb - bl) + bl)) : ((BYTE)(((double)(grayl<grayb?Weighting:(Weighting ^ 255))) / 255.0 * (bl - bb) + bb)));
		SetPixel(*pDC, X0, Y0, RGB(rr, gr, br));
		clrBackGround = ::GetPixel(*pDC, X0, Y0 + 1);
		rb = GetRValue(clrBackGround);
		gb = GetGValue(clrBackGround);
		bb = GetBValue(clrBackGround);
		grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;
		rr = (rb > rl ? ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (rb - rl) + rl)) : ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (rl - rb) + rb)));
		gr = (gb > gl ? ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (gb - gl) + gl)) : ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (gl - gb) + gb)));
		br = (bb > bl ? ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (bb - bl) + bl)) : ((BYTE)(((double)(grayl<grayb?(Weighting ^ 255):Weighting)) / 255.0 * (bl - bb) + bb)));
		SetPixel(*pDC, X0, Y0 + 1, RGB(rr, gr, br));
	}
	/* Draw the final pixel, which is always exactly intersected by the line
	and so needs no weighting */
	SetPixel(*pDC, X1, Y1, clrLine);
}

void DrawLine(HDC pDC, int x1, int y1, int x2, int y2, COLORREF m_rgbMinuteHand){
	DrawWuLine(&pDC, x1, y1, x2, y2, m_rgbMinuteHand);
	//MoveToEx(*pDC, x1, y1, NULL);
	//LineTo(*pDC, x2, y2);
}

// Create array with enough room for maximum path, plus one for trailing NULL
//    (MAX_PATH is defined in windef.h)
wchar_t* Ascii2Uni(const char* SomeAsciiStr, wchar_t* buffer){
	// Convert an ASCII string to a Unicode String
	MultiByteToWideChar(CP_ACP, 0, SomeAsciiStr, -1, buffer, 1024);
	return buffer;
}

char* Uni2Ascii(const TCHAR* SomeUnicodeStr, char* buffer){
	// Convert a Unicode string to an ASCII string
	WideCharToMultiByte(CP_ACP, 0, SomeUnicodeStr, -1, buffer, 1024, NULL, NULL);
	return buffer;
}

void Message(const TCHAR* textToSay){
	MessageBox(NULL, textToSay, L"rlClock", MB_OK | MB_ICONINFORMATION);
}

void Message(const TSTRING* textToSay){
	MessageBox(NULL, textToSay->c_str(), L"rlClock", MB_OK | MB_ICONINFORMATION);
}

void ErrorMessage(const TCHAR* textToSay){
	MessageBox(NULL, textToSay, L"rlClock", MB_OK | MB_ICONERROR);
}

int MyAtoI(const TCHAR* asciizahl){
	int iVal;
	TSTRING sName;
	sName = asciizahl;
	swscanf( sName.c_str(), _T("%d"), &iVal);
	return iVal;
}

void ErrorMessage(const TSTRING* textToSay){
	MessageBox(NULL, textToSay->c_str(), L"rlClock", MB_OK | MB_ICONERROR);
}

std::wstring ctow(const char* src){
	std::vector<wchar_t> dest(strlen(src) * 2);
	int i = mbstowcs(&dest[0], src, strlen(src));
	return std::wstring(&dest[0]);
}

int cmp_nocase(const TSTRING& s, const TSTRING& s2){
	int result = _wcsicmp(s.c_str(), s2.c_str());
    /*TSTRING::const_iterator p = s.begin();
    TSTRING::const_iterator p2 = s2.begin();
	while (p != s.compare end() && p != s2.end())
    {
        if (towupper(*p) != towupper(*p2))
            return (towupper(*p) < towupper(*p2)) ? -1 : 1;
        ++p, ++p2;
    }*/
    
	return result; //s2.size() - s.size();
}

std::wstring ltow (DWORD n){
  int i=0,j;
  TSTRING s;
  TSTRING u;
  do{
    s += (char)( n%10+48 );
	i++;
    n-=n%10;
  }while((n/=10)>0);
  u = s;
  for (j=0;j<i;j++){
	  u[i-1-j]=s[j];
  }
  return u;
}

std::wstring GetRegistry(const TSTRING& key){
	TSTRING parseString = key;
	TSTRING path;
	TSTRING mkey;
	TSTRING name;
	TSTRING retVal;
	long ret;
	path = parseString.substr(0, parseString.find('\\'));
	parseString = parseString.substr(path.length() + 1, parseString.length() - path.length() - 1);
	mkey = parseString.substr(0, parseString.rfind('\\'));
	parseString = parseString.substr(mkey.length() + 1, parseString.length() - mkey.length() - 1);
	name = parseString;
	HKEY dwPath;
	if(!cmp_nocase(path, L"HKEY_CURRENT_USER"))
		dwPath = HKEY_CURRENT_USER;
	else if(!cmp_nocase(path, L"HKEY_CLASSES_ROOT"))
		dwPath = HKEY_CLASSES_ROOT;
	else if(!cmp_nocase(path, L"HKEY_LOCAL_MACHINE"))
		dwPath = HKEY_LOCAL_MACHINE;
	else if(!cmp_nocase(path, L"HKEY_USERS"))
		dwPath = HKEY_USERS;
	HKEY hKey = NULL; 
	DWORD dwDisposition = 0; 
	ret = RegCreateKeyEx( dwPath, mkey.c_str(), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );
	if (ret != ERROR_SUCCESS) { 
		return _T("xxx"); 
	}
	DWORD dwType = REG_SZ; 
	DWORD dwDataSize = 0; 
	TCHAR strName[256];
	DWORD dwName;
	ret = RegQueryValueEx( hKey, name.c_str(), 0, &dwType, (PBYTE)NULL, &dwDataSize );
	if ( ret == ERROR_SUCCESS) { 
		if(dwType == REG_SZ){
			RegQueryValueEx( hKey, name.c_str(), 0, &dwType, (PBYTE)strName, &dwDataSize ); 
			retVal = strName;
		}else if( dwType == REG_DWORD ){
			RegQueryValueEx( hKey, name.c_str(), 0, &dwType, (PBYTE)&dwName, &dwDataSize ); 
			retVal = ltow(dwName);
		}else{
			retVal = _T("xxx");
		}
	} else if( ret == 2 ){
		retVal = _T("xxx");
	}
	RegCloseKey(hKey); 
	return retVal;
}

COLORREF RGB2COLOR(const TSTRING& color){
	TSTRING parseString = color;
	TSTRING red;	
	TSTRING green;
	TSTRING blue;
	parseString = parseString.substr(4, parseString.length() - 4);
	parseString = parseString.substr(0, parseString.length() - 1);
	red = parseString.substr(0, parseString.find(','));
	parseString = parseString.substr(red.length() + 1, parseString.length() - red.length() - 1);
	green = parseString.substr(0, parseString.find(','));
	parseString = parseString.substr(green.length() + 1, parseString.length() - green.length() - 1);
	blue = parseString.substr(0, parseString.find(','));
	int r = 0;
	int g = 0;
	int b = 0;
	CHAR buffer[5];
	r = atoi(Uni2Ascii(red.c_str(), buffer));
	g = atoi(Uni2Ascii(green.c_str(), buffer));
	b = atoi(Uni2Ascii(blue.c_str(), buffer));
	return RGB(r, g, b);
}

void Add2Log(const TCHAR* message){
	DWORD dwWritten;
	wchar_t NextLine[] = L"\n";
	HANDLE logfile = CreateFile (L"\\rlLog.txt",
                                GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
	SetFilePointer (logfile, 0, NULL, FILE_END);
	char outputBuffer[1024];
	//outputBuffer = Uni2Ascii(message, outputBuffer);
	SYSTEMTIME st;
	TCHAR szText[512];
	GetLocalTime(&st);
	GetTimeFormat(0, 0, NULL, NULL, szText, 512 - 1);
	WriteFile (logfile, Uni2Ascii(szText, outputBuffer),  strlen(Uni2Ascii(szText, outputBuffer)), &dwWritten, NULL);
	WriteFile (logfile, Uni2Ascii(L": ", outputBuffer),  strlen(Uni2Ascii(L": ", outputBuffer)), &dwWritten, NULL);
	WriteFile (logfile, Uni2Ascii(message, outputBuffer),  strlen(Uni2Ascii(message, outputBuffer)), &dwWritten, NULL);
	WriteFile (logfile, &NextLine,  _tcslen(NextLine), &dwWritten, NULL);
	CloseHandle (logfile); 
}

/******************************************************************************
	ShellOpen
******************************************************************************/
void ShellOpen(const TCHAR *FileName, const TCHAR *CommnadLine){
	SHELLEXECUTEINFO sei;
/*	if((hFindFile = FindFirstFile(FileName, &FindData)) != INVALID_HANDLE_VALUE){
		FindClose(hFindFile);
		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			//Folder open
			CommnadLine = FileName;
			FileName = FILE_EXPLORER;
	if(lstrcmp(FileName, TEXT("\\")) == 0){
		CommnadLine = FileName;
		FileName = FILE_EXPLORER;
	if(lstrcmp(FileName, TEXT("#Settings")) == 0){
		myhwnd=CreateDialog( hInst, MAKEINTRESOURCE(IDD_TODAY_CUSTOM), hWnd, (DLGPROC)CustomItemOptionsDlgProc );
		ShowWindow(myhwnd, SW_SHOW);
		UpdateWindow(myhwnd);
		return TRUE;
	if(lstrcmp(FileName, TEXT("#Arrow up")) == 0){
		icons_pageup();
		return TRUE;
		}
	if(lstrcmp(FileName, TEXT("#Arrow down")) == 0){
		icons_pagedown();
		return TRUE;
		}
	if(lstrcmp(FileName, TEXT("#Arrow left")) == 0){
		group_prev();
		return TRUE;
		}
	if(lstrcmp(FileName, TEXT("#Arrow right")) == 0){
		group_next();
		return TRUE;#
		}
*/
//	MostRecentProgUpdate(FileName);
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(sei);
	sei.fMask = 0;
	sei.hwnd = NULL;
	sei.lpVerb = NULL;
	sei.lpFile = FileName;
	if(*CommnadLine != TEXT('\0')){
		sei.lpParameters = CommnadLine;
	}
	sei.lpDirectory = NULL;
	sei.nShow = SW_SHOWNORMAL;
	//sei.hInstApp = hInst;
	ShellExecuteEx(&sei);
}
