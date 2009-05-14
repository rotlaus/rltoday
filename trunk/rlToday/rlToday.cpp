/*******************************************************************************
*   rlToday - multi purpose today plugin.                                      *
*                                                                              *
*   Copyright (C) 2005-2009 Andre Adrian aka Rotlaus                           *
*	(rotlaus@gmail.com)                                                        *
*                                                                              *
*   This program is free software; you can redistribute it and/or              *
*   modify it under the terms of the GNU General Public License version 2      *
*   as published by the Free Software Foundation.                              *
*                                                                              *
*   This program is distributed in the hope that it will be useful,            *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
*   General Public License version 2 for more details.                         *
*                                                                              *
*   You should have received a copy of the GNU General Public License          *
*   version 2 along with this library; if not, write to the Free Software      *
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *
*******************************************************************************/

// rlToday.cpp : Defines the entry point for the DLL application.
#define _INC_OLE
#include <windows.h>
#undef  _INC_OLE
#include "rlToday.h"
#include <todaycmn.h>
#include <commdlg.h>
#include <Commctrl.h>
#include <aygshell.h>
#include "resource.h"
#include "SelectFile.h"
#include "tinyxml.h"
//Icon anzeigen?
HMENU   g_hMainMenu;
HWND	hGeneralHandle;
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
	switch (ul_reason_for_call){
	case DLL_PROCESS_ATTACH:			
		m_hInstance = (HINSTANCE)hModule;
		g_hMainMenu = (HMENU)LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_CONTEXT_MENU));
		m_tInterval = 2000;
		//initilize the application class, and set the global window handle
		UnregisterClass(MAIN_WND_CLASS, m_hInstance);
		//InitializeClasses();
		m_cSkin = NULL;
		break;
	case DLL_PROCESS_DETACH:
		// The DLL is being unloaded by a given process.  Do any
		// per-process clean up here, such as undoing what was done in
		// DLL_PROCESS_ATTACH.	The return value is ignored.
		DestroyMenu(g_hMainMenu);
		UnregisterClass(MAIN_WND_CLASS, m_hInstance);
		m_hInstance = NULL;
		if(m_cSkin != NULL){
			delete m_cSkin;
			m_cSkin = NULL;
		}
		break;
	}
	return TRUE;
}

void PFCSipButtonShow( BOOL bShow ){
    HWND hWnd = ::FindWindow( _T( "MS_SIPBUTTON" ), NULL );
    if (hWnd == NULL)
        return;
    if (bShow)
        ::ShowWindow( hWnd, SW_SHOW );
    else
        ::ShowWindow( hWnd, SW_HIDE );
}

/******************************************************************************
	WndProc
******************************************************************************/
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	SYSTEMTIME systim;
	int	item_height_temp;
	switch(msg){
	case WM_CREATE:
		GetLocalTime(&systim);
		SetTimer(hWnd, ID_CLOCK_TIMER, (systim.wSecond % 10) * 2000, NULL);
		GetTodayDefaults(hWnd);
		if(m_cSkin != NULL){
			delete m_cSkin;
			m_cSkin = NULL;
		}
		break;
	case WM_DESTROY:
		if(m_cSkin != NULL){
			delete m_cSkin;
			m_cSkin = NULL;
		}
		return 0;
	case WM_TODAYCUSTOM_CLEARCACHE:
		break;
	case WM_TODAYCUSTOM_QUERYREFRESHCACHE:
		if(Refresh == TRUE){
			Refresh = FALSE;
			// Höhe hat sich geändert, neu setzen
			((TODAYLISTITEM *)(wParam))->cyp = m_nItem_height;
			return TRUE;
		}
		return FALSE;
	case WM_LBUTTONDOWN:
			SHRGINFO    shrg;
			HMENU       hmenu;
			RECT		rect;
			BOOL		nRet;
			shrg.cbSize = sizeof(shrg);
			shrg.hwndClient = hWnd;
			shrg.ptDown.x = LOWORD(lParam);
			shrg.ptDown.y = HIWORD(lParam);
			shrg.dwFlags = SHRG_RETURNCMD;
			if (SHRecognizeGesture(&shrg) == GN_CONTEXTMENU) {
				GetWindowRect(hWnd,(LPRECT)&rect);
				hmenu = GetSubMenu(g_hMainMenu, 0);
				nRet = TrackPopupMenuEx(hmenu,
								 TPM_LEFTALIGN,
								 shrg.ptDown.x + rect.left,                                              
								 shrg.ptDown.y + rect.top,
								 hWnd,
								 NULL);
				if(nRet)
					rect.left = 1;
				else
					rect.left = 0;
			}else{
				POINT point;
				point.x = LOWORD(lParam);
				point.y = HIWORD(lParam);
				OnLButtonDown(wParam, point);
				InvalidateRect(hWnd, NULL, FALSE);
				UpdateWindow(hWnd);
			}
			break;
		return 0;
	case WM_LBUTTONUP :
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			OnLButtonUp(wParam, point);
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			return 0;
	case WM_PAINT:
			PAINTSTRUCT ps;
			// Today Screen malen
			hdc = BeginPaint(hWnd, &ps);
			item_height_temp = OnPaint(hWnd, hdc, &ps, 0);
			EndPaint(hWnd, &ps);
			if(m_nItem_height != item_height_temp){
				m_nItem_height = item_height_temp;
				Refresh = TRUE;
			}
			break;
	case WM_ERASEBKGND:
		TODAYDRAWWATERMARKINFO dwi;
		dwi.hdc = (HDC)wParam;
		GetClientRect(hWnd, &dwi.rc);
		dwi.hwnd = hWnd;
		SendMessage(GetParent(hWnd), TODAYM_DRAWWATERMARK, 0,(LPARAM)&dwi);
		return TRUE;
	case WM_TIMER:
		switch(wParam){
		case ID_CLOCK_TIMER:
			// Uhr Timer abgelaufen, Fenster updaten
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			KillTimer(hWnd, ID_CLOCK_TIMER);
			GetLocalTime(&systim);
			SetTimer(hWnd, ID_CLOCK_TIMER, m_tInterval, NULL);
			break;
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case MNU_OPTIONS:
			//DialogBoxParam(m_hInstance, MAKEINTRESOURCE(IDD_TODAY_CUSTOM), hWnd, CustomItemOptionsDlgProc, 0);
			PFCSipButtonShow(TRUE);
			DialogBox(m_hInstance, MAKEINTRESOURCE(500), hWnd, CustomItemOptionsDlgProc);
			PFCSipButtonShow(FALSE);
			break;
		case MNU_RELOADSKIN:
			ReloadSkinFile();
		}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

/******************************************************************************
	InitInstance
******************************************************************************/
static HWND InitInstance(HWND pWnd, TODAYLISTITEM *ptli){
	WNDCLASS wc;
	m_hInstance = ptli->hinstDLL;
	// Registry lesen
	// GetOption();
	// Defaults fürs Today Fenster setzen
    wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = 0;
	wc.lpszMenuName = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ptli->hinstDLL;
	wc.hIcon = NULL;
	wc.hbrBackground = NULL;
	wc.lpszClassName = MAIN_WND_CLASS;
	UnregisterClass(MAIN_WND_CLASS, ptli->hinstDLL);
	RegisterClass(&wc);
	Refresh = TRUE;
	// Today Fenster erzeugen
	return CreateWindow(MAIN_WND_CLASS, WINDOW_TITLE, WS_CHILD | WS_VISIBLE, CW_DEFAULT, CW_DEFAULT, 0, 0,
		pWnd, NULL, ptli->hinstDLL, NULL);
}

/******************************************************************************
	InitializeCustomItem
******************************************************************************/
HWND APIENTRY InitializeCustomItem(TODAYLISTITEM *ptli, HWND pWnd){
	if(ptli->fEnabled == 0){
		return NULL;
	}
	return InitInstance(pWnd, ptli);
}

/******************************************************************************
	GetFileName
******************************************************************************/
static BOOL GetFileName(HWND hWnd, TCHAR *ret){
	OPENFILENAME of;
	TCHAR path[BUF_SIZE];
	TCHAR *ph;
	lstrcpy(path, ret);
	ZeroMemory(&of, sizeof(OPENFILENAME));
	of.lStructSize = sizeof(OPENFILENAME);
	of.hInstance = m_hInstance;
	of.hwndOwner = hWnd;
	of.lpstrFilter = TEXT("Graphics\0*.bmp;*.2bp;*.gif;*.png;*.jpg\0All files (*.*)\0*.*\0\0");
	of.nFilterIndex = 1;
	of.lpstrTitle = TEXT("");
	of.lpstrFile = path;
	of.nMaxFile = BUF_SIZE - 1;
	of.lpstrDefExt = TEXT("ns");
	of.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(GetOpenFileName((LPOPENFILENAME)&of) == FALSE){
		return FALSE;
	}
	ph = path;
	if(*ph == TEXT('\\') && *(ph + 1) == TEXT('\\')){
		ph++;
	}
	if(*(ph + lstrlen(ph)) == TEXT('.')){
		*(ph + lstrlen(ph)) = TEXT('\0');
	}
	lstrcpy(ret, ph);
	return TRUE;
}

bool IsSkinDir(TSTRING path){
	TiXmlDocument* m_doc;
	TSTRING fileToTest;
	char buffer[MAX_PATH];
	fileToTest = m_sPathName + L"\\";
	fileToTest += path;
	fileToTest += L"\\Skin.xml";
	m_doc = new TiXmlDocument( Uni2Ascii(fileToTest.c_str(), buffer) );
	bool loadOkay = m_doc->LoadFile();
	delete m_doc;
	if ( loadOkay ){
		return true;
	}else{
		return false;
	}
}

/******************************************************************************
	GeneralProc
******************************************************************************/
static BOOL CALLBACK GeneralProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	SHINITDLGINFO shidi;
	TSTRING testPath;
	switch(uMsg){
	case WM_INITDIALOG:{
		// Hier die Einstellungen aus der INI im Dialog anzeigen
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = hDlg;
		SHInitDialog(&shidi);
		SetWindowText(hDlg, TEXT("General"));
		HANDLE hFile;
		WIN32_FIND_DATA foundFile;
		TSTRING testPath;
		long lResult;
		testPath = m_sPathName + L"\\*";
		hFile = FindFirstFile(testPath.c_str(), &foundFile);
		int actSkin;
		int skinCount;
		skinCount = 0;
		if(foundFile.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			TSTRING checkPath;
			checkPath = foundFile.cFileName;
			if(IsSkinDir(checkPath)){
				//HWND hwndItem;
				
				TCHAR skinPath[MAX_PATH];
				lstrcpy(skinPath, checkPath.c_str());
				
				lResult = SendDlgItemMessage(hDlg, IDC_COMBO_SKIN, CB_ADDSTRING ,0 ,(LPARAM)(LPCSTR)skinPath);
				if(checkPath == m_sSkinfile)
					actSkin = skinCount;
				skinCount++;
			}
		}
		while(FindNextFile(hFile, &foundFile)){
			if(foundFile.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
				TSTRING checkPath;
				
				checkPath = foundFile.cFileName;
				if(IsSkinDir(checkPath)){
					//HWND hwndItem;
					
					TCHAR skinPath[MAX_PATH];
					lstrcpy(skinPath, checkPath.c_str());
					
					lResult = SendDlgItemMessage(hDlg, IDC_COMBO_SKIN, CB_ADDSTRING ,0 ,(LPARAM)(LPCSTR)skinPath);
					if(checkPath == m_sSkinfile)
						actSkin = skinCount;
					skinCount++;
				}
			}
		}
		FindClose(hFile);
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_SKIN, CB_SETCURSEL, (WPARAM)actSkin, 0);
		break;}
	case WM_DESTROY:
		// Hier die Einstellungen des Dialogs wegspeichern
		TCHAR skinPath[MAX_PATH];
		GetDlgItemText(hDlg, IDC_COMBO_SKIN, skinPath, MAX_PATH-1);
		m_sSkinfile = skinPath;
		SaveConfiguration();
		ReloadSkinFile();
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			EndDialog(hDlg, IDOK);
			Refresh = TRUE;
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************
	AboutProc
******************************************************************************/
static BOOL CALLBACK AboutProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	SHINITDLGINFO shidi;
	switch(uMsg){
	case WM_INITDIALOG:
		//PocketPC
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = hDlg;
		SHInitDialog(&shidi);
		SetWindowText(hDlg, TEXT("About"));
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			EndDialog(hDlg, IDOK);
			Refresh = TRUE;
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************
	ImageListIconAdd
******************************************************************************/
static void ImageListIconAdd(HWND hDlg, HIMAGELIST hIconList, HIMAGELIST SysIconList, TCHAR *path, TCHAR *File, TCHAR *iconpath, int iconresourcenum, int smallicon){
	WIN32_FIND_DATA FindData;
	HANDLE hFindFile;
	HICON hIcon = NULL;
	SHFILEINFO  shfi;
	TCHAR buf[BUF_SIZE];
	if(path != NULL){
		wsprintf(buf, TEXT("%s\\%s"), path, File);
	}else{
		lstrcpy(buf, File);
	}
	if(lstrcmp(buf, TEXT("\\")) == 0){
		lstrcpy(buf, TEXT("\\windows"));
	}
	if(*iconpath!=TEXT('\0')){
		if((hFindFile = FindFirstFile(iconpath, &FindData)) != INVALID_HANDLE_VALUE){
			FindClose(hFindFile);
			if(smallicon)
				ExtractIconEx(iconpath, -iconresourcenum, NULL, &hIcon, 1);
			else
				ExtractIconEx(iconpath, -iconresourcenum, &hIcon, NULL, 1);
			if(hIcon==NULL){
				HBITMAP hIconBmp=NULL,hBitMap;//,hBitMap2;
				HDC memdc,memdc2,memdc3;//,memdc4;
				ICONINFO iconinfo;
				BITMAP bmp;
				HBRUSH hBrush;
				RECT rect;
				int size;
				if(smallicon)
					size=16;
				else
					size=32;
				hIconBmp = SHLoadDIBitmap(iconpath);
				if(hIconBmp!=NULL)
				{
					memdc = GetDC(hDlg);
					memdc2 = CreateCompatibleDC(memdc);
					SelectObject(memdc2, hIconBmp);
					GetObject(hIconBmp, sizeof(BITMAP), &bmp);
					memdc3 = CreateCompatibleDC(memdc);
					hBitMap = CreateCompatibleBitmap(memdc3, size, size);
					SelectObject(memdc3, hBitMap);
					hBrush = CreateSolidBrush(0xff00ff);
					SetRect(&rect, 0, 0, size, size);
					FillRect(memdc3, &rect, hBrush);
					DeleteObject(hBrush);
					TransparentImage(memdc3, 0, 0, size, size, memdc2, 0, 0, size, size, 0xff00ff);
	//				BitBlt(memdc3, 0, 0, size, size, memdc2, 0, 0, SRCCOPY);
					iconinfo.hbmMask=hBitMap;
	//				iconinfo.hbmMask=hIconBmp;
					iconinfo.hbmColor=hIconBmp;
					iconinfo.fIcon=1;
					iconinfo.xHotspot=0;
					iconinfo.yHotspot=0;
					hIcon=CreateIconIndirect(&iconinfo);
				}
				DeleteObject(hIconBmp);
				DeleteObject(hBitMap);
				DeleteDC(memdc);
				DeleteDC(memdc2);
				DeleteDC(memdc3);
#if 0
				DeleteDC(memdc4);
				DeleteObject(hBitMap2);
#endif
			}
		}
	}else if(iconresourcenum!=0){
		if(smallicon)
			ExtractIconEx(buf, -iconresourcenum, NULL, &hIcon, 1);
		else
			ExtractIconEx(buf, -iconresourcenum, &hIcon, NULL, 1);
	}
	if(hIcon == NULL){
		if((hFindFile = FindFirstFile(buf, &FindData)) != INVALID_HANDLE_VALUE){
			FindClose(hFindFile);
			SHGetFileInfo(buf, 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX);
			hIcon = ImageList_GetIcon(SysIconList, shfi.iIcon, ILD_NORMAL);
		}
		if(hIcon == NULL){
			hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON_NON));
		}
	}
	ImageList_AddIcon(hIconList, hIcon);
	DestroyIcon(hIcon);
}

/******************************************************************************
	SetLvIcon
******************************************************************************/
static void SetLvIcon(HWND hDlg, HWND hListView){
	HIMAGELIST SysIconList;
	HIMAGELIST IconList;
	SHFILEINFO  shfi;
	LV_ITEM lvi;
	TCHAR buf[BUF_SIZE];
	int i, cnt;
	TCHAR iconpath[BUF_SIZE];
	TCHAR iconresource[BUF_SIZE];
	int iconresourcenum;
	IconList = ListView_GetImageList(hListView, LVSIL_SMALL);
	if(IconList == NULL){
		IconList = ImageList_Create(16, 16, ILC_COLOR | ILC_MASK, 0, 0);
	}else{
		ImageList_Remove(IconList, -1);
	}
	ImageList_SetBkColor(IconList, GetSysColor(COLOR_WINDOW));
	SysIconList = (HIMAGELIST)SHGetFileInfo(TEXT(""), 0, &shfi, sizeof(SHFILEINFO),
		SHGFI_ICON | SHGFI_SMALLICON | SHGFI_SYSICONINDEX);
	if(SysIconList == NULL){
		return;
	}
	cnt = ListView_GetItemCount(hListView);
	for(i = 0; i < cnt; i++){
		*buf = '\0';
		ListView_GetItemText(hListView, i, 1, buf, BUF_SIZE - 1);
		ListView_GetItemText(hListView, i, 3, iconpath, BUF_SIZE - 1);
		ListView_GetItemText(hListView, i, 4, iconresource, BUF_SIZE - 1);
		iconresourcenum=_wtoi(iconresource);
		ImageListIconAdd(hDlg, IconList, SysIconList, NULL, buf, iconpath, iconresourcenum,1);
		memset(&lvi, 0, sizeof(LVITEM));
		lvi.mask = LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.iImage = i;
		ListView_SetItem(hListView, &lvi);
	}
	ListView_SetImageList(hListView, IconList, LVSIL_SMALL);
}
/******************************************************************************
	ListView_MoveItem
******************************************************************************/
static void ListView_MoveItem(HWND hListView, int SelectItem, int Move, int ColCnt){
	LV_ITEM lvi;
	TCHAR buf[5][BUF_SIZE];
	int i = 0;
	int image;
	for(i = 0;i < ColCnt;i++){
		*(*(buf + i)) = '\0';
	}
	for(i = 0;i < ColCnt;i++){
		ListView_GetItemText(hListView, SelectItem, i, *(buf + i), BUF_SIZE - 1);
	}

	memset(&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_IMAGE;
	lvi.iItem = SelectItem;
	lvi.iSubItem = 0;
	lvi.iImage = 0;
	ListView_GetItem(hListView, &lvi);
	image = lvi.iImage;
	ListView_DeleteItem(hListView, SelectItem);
	SelectItem = SelectItem + Move;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	lvi.iItem = SelectItem;
	lvi.iSubItem = 0;
	lvi.pszText = *buf;
	lvi.cchTextMax = lstrlen(*buf);
	lvi.iImage = image;
	ListView_InsertItem(hListView, &lvi);
	for(i = 1;i < ColCnt;i++){
		ListView_SetItemText(hListView, SelectItem, i, *(buf + i));
	}
	ListView_SetItemState(hListView, SelectItem,
		LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	ListView_EnsureVisible(hListView, SelectItem, TRUE);
}

TCHAR filepath[BUF_SIZE];
TCHAR iconpath[BUF_SIZE];
int	iResourceID=0;
TCHAR iconresource[BUF_SIZE];
int iconresourcenum;
/******************************************************************************
	EditDlgProc
******************************************************************************/
static BOOL CALLBACK EditDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	SHINITDLGINFO shidi;
	HWND hListView;
	LV_ITEM lvi;
	TCHAR name[BUF_SIZE];
	TCHAR path[BUF_SIZE];
	TCHAR cmd[BUF_SIZE];
	TCHAR *p, *r;
	static int index;
	switch(uMsg){
	case WM_INITDIALOG:
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
//		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_FULLSCREENNOMENUBAR;
		shidi.hDlg = hDlg;
		SHInitDialog(&shidi);
		// Dialog Language setting not in rlToday
		// SetWindowText(hDlg, text_string[language][10]/*CL_FILE*/);
		// SendDlgItemMessage(hDlg, IDC_STATIC_NAME, WM_SETTEXT, 0, (LPARAM)text_string[language][12]);
		// SendDlgItemMessage(hDlg, IDC_STATIC_FILE_PATH, WM_SETTEXT, 0, (LPARAM)text_string[language][13]);
		// SendDlgItemMessage(hDlg, IDC_STATIC_COMMAND_LINE, WM_SETTEXT, 0, (LPARAM)text_string[language][14]);
		// SendDlgItemMessage(hDlg, IDC_BUTTON_SCANICON, WM_SETTEXT, 0, (LPARAM)text_string[language][71]);
		// SendDlgItemMessage(hDlg, IDCANCEL, WM_SETTEXT, 0, (LPARAM)text_string[language][48]);
		index = lParam;
		if(lParam >= 0){
			hListView = hGeneralHandle;
			*name = TEXT('\0');
			ListView_GetItemText(hListView, index, 0, name, BUF_SIZE - 1);
			SendDlgItemMessage(hDlg, IDC_EDIT_NAME, WM_SETTEXT, 0, (LPARAM)name);
			*name = TEXT('\0');
			ListView_GetItemText(hListView, index, 1, name, BUF_SIZE - 1);
			SendDlgItemMessage(hDlg, IDC_EDIT_PATH, WM_SETTEXT, 0, (LPARAM)name);
			*name = TEXT('\0');
			ListView_GetItemText(hListView, index, 2, name, BUF_SIZE - 1);
			SendDlgItemMessage(hDlg, IDC_EDIT_CMD, WM_SETTEXT, 0, (LPARAM)name);
			*name = TEXT('\0');
			ListView_GetItemText(hListView, index, 3, name, BUF_SIZE - 1);
			SendDlgItemMessage(hDlg, IDC_EDIT_ICO, WM_SETTEXT, 0, (LPARAM)name);
			*iconresource = TEXT('\0');
			ListView_GetItemText(hListView, index, 4, iconresource, BUF_SIZE - 1);
			iconresourcenum=_wtoi(iconresource);
			iResourceID=-iconresourcenum;
		}else{
			*iconpath = TEXT('\0');
			*iconresource = TEXT('\0');
			iResourceID=0;
			iconresourcenum=0;
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_EDIT_NAME:
		case IDC_EDIT_PATH:
		case IDC_EDIT_CMD:
			switch(HIWORD(wParam)){
			case EN_SETFOCUS:
				SHSipPreference(hDlg, SIP_UP);
				break;
			case EN_KILLFOCUS:
				SHSipPreference(hDlg, SIP_DOWN);
				break;
			}
			break;
		}
		case IDC_BUTTON_FILESELECT:
			if(SelectFile(hDlg, m_hInstance, NULL, path) == TRUE){
				SendDlgItemMessage(hDlg, IDC_EDIT_PATH, WM_SETTEXT, 0, (LPARAM)path);
				if(SendDlgItemMessage(hDlg, IDC_EDIT_NAME, WM_GETTEXTLENGTH, 0, 0) > 0){
					break;
				}
				for(p = r = path; *p != TEXT('\0'); p++){
					if(*p == TEXT('\\')){
						r = p;
					}
				}
				lstrcpy(name, r + 1);
				for(p = name; *p != TEXT('\0') && *p != TEXT('.'); p++);
				*p = TEXT('\0');
				SendDlgItemMessage(hDlg, IDC_EDIT_NAME, WM_SETTEXT, 0, (LPARAM)name);
			}
			break;
		case IDC_BUTTON_CMDSELECT:
			if(SelectFile(hDlg, m_hInstance, NULL, cmd) == TRUE){
				SendDlgItemMessage(hDlg, IDC_EDIT_CMD, WM_SETTEXT, 0, (LPARAM)cmd);
			}
			break;
		case IDC_BUTTON_ICOSELECT:
			if(SelectFile(hDlg, m_hInstance, NULL, iconpath) == TRUE){
				SendDlgItemMessage(hDlg, IDC_EDIT_ICO, WM_SETTEXT, 0, (LPARAM)iconpath);
			}
			break;
		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			break;
		case IDOK:
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_NAME, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			*path = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_PATH, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)path);
			if(*name == TEXT('\0') || *path == TEXT('\0')){
				break;
			}
			if(lstrcmp(path, TEXT("\\")) != 0 && *(path + lstrlen(path) - 1) == TEXT('\\')){
				*(path + lstrlen(path) - 1) = TEXT('\0');
			}
			*cmd = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_CMD, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)cmd);
			hListView = hGeneralHandle;
			if(index == -1){
				memset(&lvi, 0, sizeof(LVITEM));
				lvi.mask = LVIF_TEXT | LVIF_IMAGE;
				lvi.iItem = ListView_GetItemCount(hListView);
				lvi.iSubItem = 0;
				lvi.pszText = name;
				lvi.cchTextMax = lstrlen(name);
				lvi.iImage = 0;
				index = ListView_InsertItem(hListView, &lvi);
			}else{
				ListView_SetItemText(hListView, index, 0, name);
			}
			ListView_SetItemText(hListView, index, 1, path);
			ListView_SetItemText(hListView, index, 2, cmd);
			ListView_SetItemText(hListView, index, 3, iconpath);
			ListView_SetItemText(hListView, index, 4, iconresource);
			EndDialog(hDlg, TRUE);
			break;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************
	AppProc
******************************************************************************/
static BOOL CALLBACK AppProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	SHINITDLGINFO shidi;
	int i;
	LVCOLUMN lvc;
	LV_ITEM lvi;
	int ItemIndex;
	TCHAR buffer[BUF_SIZE];
	switch(uMsg){
	case WM_INITDIALOG:
		//PocketPC
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = hDlg;
		SHInitDialog(&shidi);
		SetWindowText(hDlg, TEXT("Apps"));
		lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvc.fmt = LVCFMT_LEFT;
		lvc.cx = 150;
		lvc.pszText = TEXT("Name");
		lvc.cchTextMax = lstrlen(TEXT("Name"));
		lvc.iSubItem = 0;
		ListView_InsertColumn(GetDlgItem(hDlg, IDC_LIST_FILE), 0, &lvc);
		lvc.cx = 150;
		lvc.pszText = TEXT("File path");
		lvc.cchTextMax = lstrlen(TEXT("File path"));
		lvc.iSubItem = 1;
		ListView_InsertColumn(GetDlgItem(hDlg, IDC_LIST_FILE), 1, &lvc);
		lvc.cx = 100;
		lvc.pszText = TEXT("Command line");
		lvc.cchTextMax = lstrlen(TEXT("Command line"));
		lvc.iSubItem = 2;
		ListView_InsertColumn(GetDlgItem(hDlg, IDC_LIST_FILE), 2, &lvc);
		lvc.cx = 100;
		lvc.pszText = TEXT("Icon path"); //Icon path
		lvc.cchTextMax = lstrlen(TEXT("Icon path")); //Iconpath
		lvc.iSubItem = 3;
		ListView_InsertColumn(GetDlgItem(hDlg, IDC_LIST_FILE), 3, &lvc);
		lvc.cx = 100;
		lvc.pszText = TEXT("Icon handle");//Icon handle
		lvc.cchTextMax = lstrlen(TEXT("Icon handle"));
		lvc.iSubItem = 4;
		ListView_InsertColumn(GetDlgItem(hDlg, IDC_LIST_FILE), 4, &lvc);
		for(i = 0; i < m_FileList.size(); i++){
			int index;
			hGeneralHandle = GetDlgItem(hDlg, IDC_LIST_FILE);
			memset(&lvi, 0, sizeof(LVITEM));
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			lvi.iItem = ListView_GetItemCount(hGeneralHandle);
			lvi.iSubItem = 0;
			swprintf(buffer, _T("%s"), m_FileList[i].Name.c_str());
			lvi.pszText = buffer;
			lvi.cchTextMax = lstrlen(buffer);
			lvi.iImage = 0;
			index = ListView_InsertItem(hGeneralHandle, &lvi);
			swprintf(buffer, _T("%s"), m_FileList[i].FileName.c_str());
			ListView_SetItemText(hGeneralHandle, index, 1, buffer);
			swprintf(buffer, _T("%s"), m_FileList[i].CommandLine.c_str());
			ListView_SetItemText(hGeneralHandle, index, 2, buffer);
			swprintf(buffer, _T("%s"), m_FileList[i].IconPath.c_str());
			ListView_SetItemText(hGeneralHandle, index, 3, buffer);
			//SetLvIcon(hDlg, GetDlgItem(hDlg, IDC_LIST_FILE));
		}
		break;
	case WM_DESTROY:
		i = ListView_GetItemCount(GetDlgItem(hDlg, IDC_LIST_FILE));
		ItemIndex = i;
		m_FileList.clear();
		for(i = 0;i < ItemIndex; i++){
			FILELIST tmpFL;
			TCHAR	  tmpName[BUF_SIZE];
			ListView_GetItemText(GetDlgItem(hDlg, IDC_LIST_FILE), i, 0, tmpName, BUF_SIZE - 1);
			tmpFL.Name = tmpName;
			ListView_GetItemText(GetDlgItem(hDlg, IDC_LIST_FILE), i, 1, tmpName, BUF_SIZE - 1);
			tmpFL.FileName = tmpName;
			ListView_GetItemText(GetDlgItem(hDlg, IDC_LIST_FILE), i, 2, tmpName, BUF_SIZE - 1);
			tmpFL.CommandLine = tmpName;
			ListView_GetItemText(GetDlgItem(hDlg, IDC_LIST_FILE), i, 3, tmpName, BUF_SIZE - 1);
			tmpFL.IconPath = tmpName;
			tmpFL.Index = i;
			m_FileList.push_back(tmpFL);
		}
		SaveConfiguration();
		ReloadSkinFile();
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			EndDialog(hDlg, IDOK);
			Refresh = TRUE;
			break;
		case IDC_BUTTON_ADD:
			hGeneralHandle = GetDlgItem(hDlg, IDC_LIST_FILE);
			DialogBoxParam(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG_EDIT), hDlg, EditDlgProc, -1);
			//SetLvIcon(hDlg, GetDlgItem(hDlg, IDC_LIST_FILE));
			//SHSipPreference(hDlg, SIP_DOWN);
			break;
		case IDC_BUTTON_EDIT:
			hGeneralHandle = GetDlgItem(hDlg, IDC_LIST_FILE);
			if((i = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LIST_FILE),
				-1, LVNI_FOCUSED | LVIS_SELECTED)) == -1){
				break;
			}
			DialogBoxParam(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG_EDIT), hDlg, EditDlgProc, i);
			//SetLvIcon(hDlg, GetDlgItem(hDlg, IDC_LIST_FILE));
			//SHSipPreference(hDlg, SIP_DOWN);
			break;
		case IDC_BUTTON_DELETE:
			if(ListView_GetSelectedCount(GetDlgItem(hDlg, IDC_LIST_FILE)) <= 0){
				break;
			}
			if(MessageBox(hDlg, TEXT("Delete selected item?"), TEXT("Delete"), MB_ICONQUESTION | MB_YESNO) == IDNO){
				break;
			}
			while((i = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LIST_FILE), -1, LVNI_SELECTED)) != -1){
				ListView_DeleteItem(GetDlgItem(hDlg, IDC_LIST_FILE), i);
			}
			break;
		case IDC_BUTTON_UP:
			if((i = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LIST_FILE), -1, LVNI_SELECTED)) <= 0){
				break;
			}
			ListView_MoveItem(GetDlgItem(hDlg, IDC_LIST_FILE), i, -1, 5);
			break;
		case IDC_BUTTON_DOWN:
			if((i = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LIST_FILE), -1, LVNI_SELECTED)) == -1){
				break;
			}
			if(i == ListView_GetItemCount(GetDlgItem(hDlg, IDC_LIST_FILE)) - 1){
				break;
			}
			ListView_MoveItem(GetDlgItem(hDlg, IDC_LIST_FILE), i, 1, 5);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************
	CalProc
******************************************************************************/
static BOOL CALLBACK CalProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	SHINITDLGINFO shidi;
	TCHAR name[BUF_SIZE];
	TSTRING tmpName;
	long lResult=0;
	switch(uMsg){
	case WM_INITDIALOG:
		//PocketPC
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = hDlg;
		SHInitDialog(&shidi);
		SendDlgItemMessage(hDlg, IDC_EDIT_SU, WM_SETTEXT, 0, (LPARAM)m_WeekDayAbbs[0].c_str());
		SendDlgItemMessage(hDlg, IDC_EDIT_MO, WM_SETTEXT, 0, (LPARAM)m_WeekDayAbbs[1].c_str());
		SendDlgItemMessage(hDlg, IDC_EDIT_TU, WM_SETTEXT, 0, (LPARAM)m_WeekDayAbbs[2].c_str());
		SendDlgItemMessage(hDlg, IDC_EDIT_WE, WM_SETTEXT, 0, (LPARAM)m_WeekDayAbbs[3].c_str());
		SendDlgItemMessage(hDlg, IDC_EDIT_TH, WM_SETTEXT, 0, (LPARAM)m_WeekDayAbbs[4].c_str());
		SendDlgItemMessage(hDlg, IDC_EDIT_FR, WM_SETTEXT, 0, (LPARAM)m_WeekDayAbbs[5].c_str());
		SendDlgItemMessage(hDlg, IDC_EDIT_SA, WM_SETTEXT, 0, (LPARAM)m_WeekDayAbbs[6].c_str());
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_WEEKDAY, CB_ADDSTRING ,0 ,(LPARAM)_T("Sunday"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_WEEKDAY, CB_ADDSTRING ,0 ,(LPARAM)_T("Monday"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_WEEKDAY, CB_SETCURSEL, (WPARAM)m_WeekStarts, 0);
		SetWindowText(hDlg, TEXT("Apps"));
		break;
	case WM_DESTROY:
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_SU, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			tmpName = name;
			m_WeekDayAbbs[0] = tmpName;
						
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_MO, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			tmpName = name;
			m_WeekDayAbbs[1] = tmpName;
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_TU, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			tmpName = name;
			m_WeekDayAbbs[2] = tmpName;
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_WE, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			tmpName = name;
			m_WeekDayAbbs[3] = tmpName;
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_TH, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			tmpName = name;
			m_WeekDayAbbs[4] = tmpName;
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_FR, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			tmpName = name;
			m_WeekDayAbbs[5] = tmpName;
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_SA, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			tmpName = name;
			m_WeekDayAbbs[6] = tmpName;
			m_WeekStarts = SendDlgItemMessage(hDlg, IDC_COMBO_WEEKDAY, CB_GETCURSEL, 0, 0);  
			SaveConfiguration();
			ReloadSkinFile();
			break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			EndDialog(hDlg, IDOK);
			Refresh = TRUE;
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************
	Edit Timezone Dialog
******************************************************************************/
static BOOL CALLBACK EditTZProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	SHINITDLGINFO shidi;
	HWND hListView;
	LV_ITEM lvi;
	TCHAR name[BUF_SIZE];
	TCHAR tz_index[BUF_SIZE];
	int tz_int;
	static int index;
	long lResult;
	switch(uMsg){
	case WM_INITDIALOG:
		//PocketPC
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = hDlg;
		SHInitDialog(&shidi);
		SetWindowText(hDlg, TEXT("File"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-12:00) Dateline Standard Time: Eniwetok, Kwajalein"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-11:00) Samoa Standard Time: Midway Island, Samoa"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-10:00) Hawaiian Standard Time: Hawaii"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-09:00) Alaskan Standard Time: Alaska"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-08:00) Pacific Standard Time: Pacific Time (US & Canada); Tijuana"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-07:00) Mountain Standard Time: Mountain Time (US & Canada)"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-07:00) US Mountain Standard Time: Arizona"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-06:00) Central Standard Time: Central Time (US & Canada)"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-06:00) Canada Central Standard Time: Saskatchewan"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-06:00) Central America Standard Time: Central America"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-06:00) Mexico Standard Time: Mexico City"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-05:00) Eastern Standard Time: Eastern Time (US & Canada)"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-05:00) US Eastern Standard Time: Indiana (East)"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-05:00) SA Pacific Standard Time: Bogota, Lima,Quito"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-04:00) Atlantic Standard Time: Atlantic Time (Canada)"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-04:00) SA Western Standard Time: Caracas, La Paz"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-04:00) Pacific SA Standard Time: Santiago"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-03:30) Newfoundland Standard Time: Newfoundland"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-03:00) Greenland Standard Time: Greenland"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-03:00) E. South America Standard Time: Brasilia"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-03:00) SA Eastern Standard Time: Buenos Aires, Georgetown"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-02:00) Mid-Atlantic Standard Time: Mid-Atlantic"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-01:00) Cape Verde Standard Time: Cape Verde Is."));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT-01:00) Azores Standard Time: Cape Verde Is."));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT) GMT: Greenwich Mean Time; Dublin, Edinburgh, Lisbon, London"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT) GMT Standard Time: Monrovia, Casablanca"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+01:00) West Central Africa Standard Time: West Central Africa"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+01:00) Central Europe Standard Time: Belgrade, Bratislava, Budapest, Ljubljana, Prague"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+01:00) Central European Standard Time: Sarajevo, Skopje, Sofjia, Vilnius, Warsaw, Zagreb"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+01:00) Romance Standard Time: Brussels, Copenhagen, Madrid, Paris"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+01:00) W. Europe Standard Time: Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+02:00) E. Europe Standard Time: Bucharest"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+02:00) Egypt Standard Time: Cairo"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+02:00) FLE Standard Time: Helsinki, Riga, Tallinn"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+02:00) GTB Standard Time: Athens, Istanbul, Minsk"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+02:00) Israel Standard Time: Jerusalem"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+02:00) South Africa Standard Time: Harare, Pretoria"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+03:00) Russian Standard Time: Moscow, St. Petersburg, Volgograd"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+03:00) Arab Standard Time: Kuwait, Riyadh"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+03:00) Arabic Standard Time: Baghdad"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+03:00) East Africa Standard Time: Nairobi"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+03:30) Iran Standard Time: Tehran"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+04:00) Arabian Standard Time: Abu Dhabi, Muscat"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+04:00) Caucasus Standard Time: Baku, Tbilisi, Yerevan"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+04:30) Afghanistan Standard Time: Kabul"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+04:00) Yekaterinburg Standard Time: Yekaterinburg"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+05:00) West Asia Standard Time: Islamabad, Karachi, Tashkent"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+05:30) India Standard Time: Bombay, Calcutta, Madras, New Delhi"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+05:45) Nepal Standard Time: Kathmandu"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+06:00) Central Asia Standard Time: Astana, Dhaka"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+06:00) Central Asia North Standard Time: Almaty, Novosibirsk"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+06:00) Sri Lanka Standard Time: Sri Jayawardenepura"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+06:30) Myanmar Standard Time: Yangoon"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+07:00) SE Asia Standard Time: Bangkok, Hanoi, Jakarta"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+07:00) North Asia Standard Time: Krasnoyarsk"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+08:00) China Standard Time: Beijing, Chongqing, Hong Kong, Urumqi"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+08:00) Singapore Standard Time: Singapore"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+08:00) Taipei Standard Time: Taipei"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+08:00) West Australia Standard Time: Perth"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+08:00) North Asia East Standard Time: Irkutsk, Ulaanbatar"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+09:00) Korea Standard Time: Seoul"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+09:00) Tokyo Standard Time: Osaka, Sapporo, Tokyo"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+09:00) Yakutsk Standard Time: Yakutsk"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+09:30) AUS Central Standard Time: Darwin"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+09:30) Cen. Australia Standard Time: Adelaide"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+10:00) Sydney Standard Time: Canberra, Melbourne, Sydney"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+10:00) E Australia Standard Time: Brisbane"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+10:00) Tasmania Standard Time: Hobart"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+10:00) Vladivostok Standard Time: Vladivostok"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+10:00) West Pacific Standard Time: Guam, Port Moresby"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+11:00) Central Pacific Standard Time: Magadan, Solomon Is., New Caledonia"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+12:00) Fiji Standard Time: Fiji, Kamchatka, Marshall Is."));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+12:00) New Zealand Standard Time: Auckland, Wellington"));
		lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_ADDSTRING ,0 ,(LPARAM)_T("(GMT+13:00) Tonga Standard Time: Nuku'alofa"));
		index = lParam;
		if(lParam >= 0){
			hListView = hGeneralHandle;
			*name = TEXT('\0');
			ListView_GetItemText(hListView, index, 0, name, BUF_SIZE - 1);
			SendDlgItemMessage(hDlg, IDC_EDIT_TZ_NAME, WM_SETTEXT, 0, (LPARAM)name);
			ListView_GetItemText(hListView, index, 1, tz_index, BUF_SIZE - 1);
			tz_int = MyAtoI(tz_index);
			lResult = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_SETCURSEL, (WPARAM)tz_int, 0);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_EDIT_TZ_NAME:
		case IDC_COMBO_TZ:
			switch(HIWORD(wParam)){
			case EN_SETFOCUS:
				//SHSipPreference(hDlg, SIP_UP);
				break;
			case EN_KILLFOCUS:
				//SHSipPreference(hDlg, SIP_DOWN);
				break;
			}
			break;
		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			break;
		case IDOK:
			//
			*name = TEXT('\0');
			SendDlgItemMessage(hDlg, IDC_EDIT_TZ_NAME, WM_GETTEXT, BUF_SIZE - 1, (LPARAM)name);
			tz_int = SendDlgItemMessage(hDlg, IDC_COMBO_TZ, CB_GETCURSEL, 0, 0);  
			if(*name == TEXT('\0') || tz_int < 0){
				break;
			}
			hListView = hGeneralHandle;
			if(index == -1){
				// 
				memset(&lvi, 0, sizeof(LVITEM));
				lvi.mask = LVIF_TEXT | LVIF_IMAGE;
				lvi.iItem = ListView_GetItemCount(hListView);
				lvi.iSubItem = 0;
				lvi.pszText = name;
				lvi.cchTextMax = lstrlen(name);
				lvi.iImage = 0;
				index = ListView_InsertItem(hListView, &lvi);
			}else{
				ListView_SetItemText(hListView, index, 0, name);
			}
			swprintf(tz_index, _T("%d"), tz_int);
			ListView_SetItemText(hListView, index, 1, tz_index);
			EndDialog(hDlg, TRUE);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************
	TimezoneProc
******************************************************************************/
static BOOL CALLBACK TimezoneProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	SHINITDLGINFO shidi;
	LVCOLUMN lvc;
	LV_ITEM lvi;
	int ItemIndex;
	unsigned int i;
	switch(uMsg){
	case WM_INITDIALOG:
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = hDlg;
		SHInitDialog(&shidi);
		SetWindowText(hDlg, TEXT("Times"));
		lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvc.fmt = LVCFMT_LEFT;
		lvc.cx = 150;
		lvc.pszText = TEXT("Name");
		lvc.cchTextMax = lstrlen(TEXT("Name"));
		lvc.iSubItem = 0;
		ListView_InsertColumn(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), 0, &lvc);
		lvc.cx = 100;
		lvc.pszText = TEXT("Timezone");
		lvc.cchTextMax = lstrlen(TEXT("Timezone"));
		lvc.iSubItem = 1;
		ListView_InsertColumn(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), 1, &lvc);
		for(i = 0; i < m_TimeZones.size(); i++){
			TCHAR buffer[64];
			memset(&lvi, 0, sizeof(LVITEM));
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			lvi.iItem = ListView_GetItemCount(GetDlgItem(hDlg, IDC_LIST_TIMEZONES));
			lvi.iSubItem = 0;
			swprintf(buffer, _T("%s"), m_TimeZones[i].GetName().c_str());
			lvi.pszText = buffer;
			lvi.cchTextMax = lstrlen(m_TimeZones[i].GetName().c_str());
			lvi.iImage = 0;
			ItemIndex = ListView_InsertItem(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), &lvi);
			swprintf(buffer, _T("%d"), m_TimeZones[i].GetTimeZone());
			ListView_SetItemText(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), ItemIndex, 1, buffer);
		}
		break;
	case WM_DESTROY:
		i = ListView_GetItemCount(GetDlgItem(hDlg, IDC_LIST_TIMEZONES));
		ItemIndex = i;
		m_TimeZones.clear();
		for(i = 0;i < ItemIndex; i++){
			CTimeZone tmpTZ;
			TCHAR	  tmpName[BUF_SIZE];
			ListView_GetItemText(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), i, 0, tmpName, BUF_SIZE - 1);
			tmpTZ.SetName(tmpName);
			ListView_GetItemText(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), i, 1, tmpName, BUF_SIZE - 1);
			tmpTZ.SetTimeZone(MyAtoI(tmpName));
			m_TimeZones.push_back(tmpTZ);
		}
		SaveConfiguration();
		ReloadSkinFile();
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			EndDialog(hDlg, IDOK);
			Refresh = TRUE;
			break;
		case IDC_BUTTON_ADD:
			hGeneralHandle = GetDlgItem(hDlg, IDC_LIST_TIMEZONES);
			DialogBoxParam(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG_TZ_EDIT), hDlg, EditTZProc, -1);
			//SetLvIcon(GetDlgItem(hDlg, IDC_LIST_TIMEZONES));
			//SHSipPreference(hDlg, SIP_DOWN);
			break;
		case IDC_BUTTON_EDIT:
			if((i = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LIST_TIMEZONES),
				-1, LVNI_FOCUSED | LVIS_SELECTED)) == -1){
				break;
			}
			hGeneralHandle = GetDlgItem(hDlg, IDC_LIST_TIMEZONES);
			DialogBoxParam(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG_TZ_EDIT), hDlg, EditTZProc, i);
			//SetLvIcon(GetDlgItem(hDlg, IDC_LIST_FILE));
			//SHSipPreference(hDlg, SIP_DOWN);
			break;
		case IDC_BUTTON_DELETE:
			if(ListView_GetSelectedCount(GetDlgItem(hDlg, IDC_LIST_TIMEZONES)) <= 0){
				break;
			}
			if(MessageBox(hDlg, TEXT("Delete selected item?"), TEXT("Delete"), MB_ICONQUESTION | MB_YESNO) == IDNO){
				break;
			}
			while((i = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), -1, LVNI_SELECTED)) != -1){
				ListView_DeleteItem(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), i);
			}
			break;
		case IDC_BUTTON_UP:
			if((i = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), -1, LVNI_SELECTED)) <= 0){
				break;
			}
			ListView_MoveItem(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), i, -1, 3);
			break;
		case IDC_BUTTON_DOWN:
			if((i = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), -1, LVNI_SELECTED)) == -1){
				break;
			}
			if(i == ListView_GetItemCount(GetDlgItem(hDlg, IDC_LIST_TIMEZONES)) - 1){
				break;
			}
			ListView_MoveItem(GetDlgItem(hDlg, IDC_LIST_TIMEZONES), i, 1, 3);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************
	 PropSheetPageProc
	 Cover callback for our property sheet to setup the tabs
	 correctly.
******************************************************************************/
UINT CALLBACK PropSheetPageProc(HWND hwndDlg, UINT uMsg, LPARAM  ppsp){
    if (uMsg == PSCB_INITIALIZED){
        // Get tab control
        HWND hwndTabs = GetDlgItem (hwndDlg, 0x3020);
        DWORD dwStyle = GetWindowLong (hwndTabs, GWL_STYLE);
        SetWindowLong (hwndTabs, GWL_STYLE, dwStyle | TCS_BOTTOM);
        SHINITDLGINFO shidi;
        shidi.dwMask = SHIDIM_FLAGS;
        shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_SIPDOWN;
        shidi.hDlg = hwndDlg;
        SHInitDialog(&shidi);
    }
	else if (uMsg ==  PSCB_GETVERSION)
        return COMCTL32_VERSION;
    return 1;
}

/******************************************************************************
	CustomItemOptionsDlgProc
******************************************************************************/
BOOL APIENTRY CustomItemOptionsDlgProc(HWND hDlg, UINT uMsg, UINT wParam, LONG lParam){
    switch (uMsg) 
    {
		case WM_INITDIALOG:
			{
				SetWindowText(hDlg, TEXT("rlToday"));
				if(m_cSkin == NULL){
					ReloadSkinFile();
				}
				// This dialog is a cover for the real
				// options dialog, which is done through
				// a property sheet.
				PROPSHEETPAGE rgpages[5];
				memset(rgpages, 0, sizeof(rgpages));
				rgpages[0].dwSize = sizeof(PROPSHEETPAGE);
				rgpages[0].dwFlags = PSP_USETITLE;
				rgpages[0].hInstance = m_hInstance;
				rgpages[0].pszTitle = _T("General");
				rgpages[0].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_GENERAL);
				rgpages[0].pfnDlgProc = GeneralProc;
				rgpages[1].dwSize = sizeof(PROPSHEETPAGE);
				rgpages[1].dwFlags = PSP_USETITLE;
				rgpages[1].hInstance = m_hInstance;
				rgpages[1].pszTitle = _T("Times");
				rgpages[1].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_TIMEZONES);
				rgpages[1].pfnDlgProc = TimezoneProc;
				rgpages[2].dwSize = sizeof(PROPSHEETPAGE);
				rgpages[2].dwFlags = PSP_USETITLE;
				rgpages[2].hInstance = m_hInstance;
				rgpages[2].pszTitle = _T("Apps");
				rgpages[2].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_APPS);
				rgpages[2].pfnDlgProc = AppProc;
				rgpages[3].dwSize = sizeof(PROPSHEETPAGE);
				rgpages[3].dwFlags = PSP_USETITLE;
				rgpages[3].hInstance = m_hInstance;
				rgpages[3].pszTitle = _T("Calendar");
				rgpages[3].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_CALENDAR);
				rgpages[3].pfnDlgProc = CalProc;
				rgpages[4].dwSize = sizeof(PROPSHEETPAGE);
				rgpages[4].dwFlags = PSP_USETITLE;
				rgpages[4].hInstance = m_hInstance;
				rgpages[4].pszTitle = _T("About");
				rgpages[4].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_ABOUT);
				rgpages[4].pfnDlgProc = AboutProc;
				PROPSHEETHEADER psh;
				memset(&psh, 0, sizeof(psh));
				psh.dwSize = sizeof(PROPSHEETHEADER);
				psh.dwFlags = PSH_PROPSHEETPAGE | PSH_MAXIMIZE | PSH_USECALLBACK;
				psh.hwndParent = hDlg;
				psh.hInstance = m_hInstance;
				psh.pszCaption = _T("Settings");
				psh.nPages = ARRAY_SIZE(rgpages);
				psh.nStartPage = 0;
				psh.ppsp = rgpages;
				psh.pfnCallback = (PFNPROPSHEETCALLBACK)PropSheetPageProc;
				// If no custom PIMs are available, remove the last launch page
				PropertySheet(&psh);
				EndDialog(hDlg, 0);
				//SaveOptions();
			}
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDOK:
				EndDialog(hDlg, IDOK);
				break;
			}
		default:
			return FALSE;
    }
    return TRUE;
}

/*************************************************************
 Default Schriftarten setzen
*************************************************************/
void GetTodayDefaults(HWND hWnd){
	LOGFONT lf;
	HFONT hSysFont;
	// Default today item font color
	m_rgbTodayText = SendMessage(GetParent(hWnd), TODAYM_GETCOLOR, (WPARAM)TODAYCOLOR_TEXT, NULL);
	// Default today fonts
	hSysFont = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hSysFont, sizeof(LOGFONT), &lf);
	_tcscpy(lf.lfFaceName, _T("Tahoma"));
	//lf.lfHeight = -(font_size);
	// Normal text
	lf.lfWeight = FW_NORMAL;
	m_hNormalTodayFont = CreateFontIndirect(&lf);
}

/******************************************************************************
	OnPaint
******************************************************************************/
int OnPaint(HWND hWnd, HDC hdc, PAINTSTRUCT *ps, UINT top){
	TODAYDRAWWATERMARKINFO dwi;
	HDC drawdc;
	HBITMAP hDrawBitMap;
	HBITMAP hRetDrawBmp;
	RECT rect;
	if(m_cSkin == NULL){
		ReloadSkinFile();
	}
	//Grösse des Today Plugins ermitteln
	GetClientRect(hWnd,(LPRECT)&rect);
	//Zeichencontainer erstellen
	drawdc = CreateCompatibleDC(hdc);
	hDrawBitMap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	hRetDrawBmp = (HBITMAP)SelectObject(drawdc, hDrawBitMap);
	//Hintergrund des Today Screens kopieren
	dwi.hdc = drawdc;
	GetClientRect(hWnd, &dwi.rc);
	dwi.hwnd = hWnd;
	SendMessage(GetParent(hWnd), TODAYM_DRAWWATERMARK, 0, (LPARAM)&dwi);
	GetClientRect(hWnd,(LPRECT)&rect);
	SetBkMode(drawdc, TRANSPARENT);
	//Draw Skin
	if(m_cSkin != NULL)
		m_cSkin->Paint(drawdc);
	TransparentBlt(hdc, ps->rcPaint.left, ps->rcPaint.top, ps->rcPaint.right, ps->rcPaint.bottom,
		drawdc, ps->rcPaint.left, ps->rcPaint.top + top, ps->rcPaint.right, ps->rcPaint.bottom + top, SRCCOPY);
	SelectObject(drawdc, hRetDrawBmp);
	DeleteObject(hDrawBitMap);
	DeleteDC(drawdc);
	return m_nItem_height;
}

void OnLButtonUp(UINT nFlags, POINT point){
	if(m_cSkin != NULL)
		m_cSkin->LButtonUp(nFlags, point);
}

void OnLButtonDown(UINT nFlags, POINT point){
	if(m_cSkin != NULL)
		m_cSkin->LButtonDown(nFlags, point);
}

/******************************************************************************
	GetProgramFilePath
******************************************************************************/
void GetProgramFilePath(){
	TCHAR appPath[MAX_PATH];
	GetModuleFileName(m_hInstance, appPath,MAX_PATH); 
	m_sPathName = appPath;
	m_sPathName = m_sPathName.substr(0, m_sPathName.rfind('\\'));
}

/******************************************************************************
	ReadConfiguration
******************************************************************************/
void ReadConfiguration(){
	CSimpleIni ini;
    const _TCHAR *pszSection = 0;
    const _TCHAR *pszKey = 0;
    const _TCHAR *pszVal = 0;
	TSTRING sIniFile;
	CTimeZone tzReadTimeZone;
	sIniFile = m_sPathName + L"\\rlToday.ini";
	if (0 != ini.LoadFile(sIniFile.c_str())){
		ErrorMessage(L"Could not find the INI File.");
		return;
	}
    pszVal = ini.GetValue(_T("General"), _T("Skinfile"), 0);
	m_sSkinfile = pszVal;
	m_TimeZones.clear();
	for(int i=0; i < 64; i++){
		TSTRING sName;
		TCHAR buffer[64];
		swprintf(buffer, _T("%d"), i);
		sName = _T("Name_");
		sName += buffer;
		pszVal = ini.GetValue(_T("Timezones"), sName.c_str(), 0);
		if(pszVal != NULL){
			int iVal = 85;
			CTimeZone tempTZ;
			tempTZ.SetName(pszVal);
			swprintf(buffer, _T("%d"), i);
			sName = _T("Zone_");
			sName += buffer;
			pszVal = ini.GetValue(_T("Timezones"), sName.c_str(), 0);
			sName = pszVal;
			iVal = MyAtoI(sName.c_str());
			tempTZ.SetTimeZone(iVal);
			m_TimeZones.push_back(tempTZ);
		}else{
			i = 64;
		}
	}
	m_FileList.clear();
	for(int i=0; i < 64; i++){
		TSTRING sName;
		TCHAR buffer[64];
		swprintf(buffer, _T("%d"), i);
		sName = _T("Name_");
		sName += buffer;
		pszVal = ini.GetValue(_T("Files"), sName.c_str(), 0);
		if(pszVal != NULL){
			int iVal = 85;
			FILELIST tempFL;
			tempFL.Name = pszVal;
			swprintf(buffer, _T("%d"), i);
			sName = _T("FileName_");
			sName += buffer;
			pszVal = ini.GetValue(_T("Files"), sName.c_str(), 0);
			tempFL.FileName = pszVal;
			swprintf(buffer, _T("%d"), i);
			sName = _T("CommmandLine_");
			sName += buffer;
			pszVal = ini.GetValue(_T("Files"), sName.c_str(), 0);
			tempFL.CommandLine = pszVal;
			swprintf(buffer, _T("%d"), i);
			sName = _T("IconPath_");
			sName += buffer;
			pszVal = ini.GetValue(_T("Files"), sName.c_str(), 0);
			tempFL.IconPath = pszVal;
			m_FileList.push_back(tempFL);
		}else{
			i = 64;
		}
	}
	m_WeekDayAbbs.clear();
	for(int i=0; i < 7; i++){
		TSTRING sName;
		TCHAR buffer[64];
		swprintf(buffer, _T("%d"), i);
		sName = _T("WeekDayAbb_");
		sName += buffer;
		pszVal = ini.GetValue(_T("Calendar"), sName.c_str() , 0);
		if(pszVal != NULL){
			TSTRING sAbb;
			sAbb = pszVal;
			m_WeekDayAbbs.push_back(sAbb);
		}else{
			TSTRING sAbb;
			sAbb = _T("");
			m_WeekDayAbbs.push_back(sAbb);
		}
	}
	pszVal = ini.GetValue(_T("Calendar"), _T("WeekStarts"), 0);
	int iVal;
	if(pszVal != NULL){
		TSTRING sName = pszVal;
		iVal = MyAtoI(sName.c_str());
	}else{
		iVal = 0;
	}
	m_WeekStarts = iVal;
}

void SaveConfiguration(){
	CSimpleIni ini;
	TSTRING sIniFile;
	long lReturn;
	char buffer[MAX_PATH];
	TCHAR shortbuffer[64];
	sIniFile = m_sPathName + L"\\rlToday.ini";
	/*if (0 != ini.LoadFile(sIniFile.c_str())){
		ErrorMessage(L"Could not find the INI File.");
		return;
	*/
    lReturn = ini.SetValue(_T("General"), _T("Skinfile"), m_sSkinfile.c_str());
	for(unsigned int i=0; i < 64; i++){
		if(i < m_TimeZones.size()){
			TSTRING sName;
			TCHAR buffer[64];
			swprintf(buffer, _T("%d"), i);
			sName = _T("Name_");
			sName += buffer;
			lReturn = ini.SetValue(_T("Timezones"), sName.c_str(), m_TimeZones[i].GetName().c_str());
			swprintf(buffer, _T("%d"), i);
			sName = _T("Zone_");
			sName += buffer;
			swprintf(buffer, _T("%d"), m_TimeZones[i].GetTimeZone());
			lReturn = ini.SetValue(_T("Timezones"), sName.c_str(), buffer);
		}
	}

	for(int i=0; i < 64; i++){
		if(i < m_FileList.size()){
			TSTRING sName;
			TCHAR buffer[64];
			swprintf(buffer, _T("%d"), i);
			sName = _T("Name_");
			sName += buffer;
			lReturn = ini.SetValue(_T("Files"), sName.c_str(), m_FileList[i].Name.c_str());
			swprintf(buffer, _T("%d"), i);
			sName = _T("FileName_");
			sName += buffer;
			lReturn = ini.SetValue(_T("Files"), sName.c_str(), m_FileList[i].FileName.c_str());
			swprintf(buffer, _T("%d"), i);
			sName = _T("CommmandLine_");
			sName += buffer;
			lReturn = ini.SetValue(_T("Files"), sName.c_str(), m_FileList[i].CommandLine.c_str());
			swprintf(buffer, _T("%d"), i);
			sName = _T("IconPath_");
			sName += buffer;
			lReturn = ini.SetValue(_T("Files"), sName.c_str(), m_FileList[i].IconPath.c_str());
		}
	}
	for(int i=0; i < 7; i++){
		TSTRING sName;
		TCHAR buffer[64];
		swprintf(buffer, _T("%d"), i);
		sName = _T("WeekDayAbb_");
		sName += buffer;
		if( i < m_WeekDayAbbs.size())
			lReturn = ini.SetValue(_T("Calendar"), sName.c_str(), m_WeekDayAbbs[i].c_str());
		else{
			TSTRING sEmpty;
			sEmpty = _T("");
			lReturn = ini.SetValue(_T("Calendar"), sName.c_str(), sEmpty.c_str());
		}
	}

	swprintf(shortbuffer, _T("%d"), m_WeekStarts);
	lReturn = ini.SetValue(_T("Calendar"), _T("WeekStarts"), shortbuffer);
    
	FILE * fp = fopen(Uni2Ascii(sIniFile.c_str(), buffer), "wb");
    if (fp) {
        // add a string to the file in the correct text format
        ini.SaveFile(fp);
        fclose(fp);
    }
}

void ReloadSkinFile(){
	GetProgramFilePath();
	ReadConfiguration();
	ReadSkinFile();
}

void ReadSkinFile(){
	TSTRING skinToLoad;
	skinToLoad = m_sPathName + L"\\" +  m_sSkinfile + L"\\skin.xml";
	if(m_cSkin != NULL){
		delete m_cSkin;
		m_cSkin = NULL;
	}
	m_cSkin = new CSkin( &skinToLoad );
	m_cSkin->SetNormalFont(m_hNormalTodayFont);
	m_cSkin->SetNormalFontColor(m_rgbTodayText);
	m_cSkin->SetTimeZoneVector(m_TimeZones);
	m_cSkin->SetWeekDayAbbVector(m_WeekDayAbbs);
	m_cSkin->SetFileListVector(m_FileList);
	m_cSkin->SetWeekStarts(m_WeekStarts);
	m_cSkin->Parse();
	m_nItem_height = m_cSkin->GetHeight();
	if(m_cSkin->GetRefresh() != 0){
		m_tInterval = m_cSkin->GetRefresh();
	}
	Refresh = true;
}
