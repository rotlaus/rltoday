#include "stdafx.h"
#include "setup.h"

//-----------------------------------------------------------------------------
// Var
//-----------------------------------------------------------------------------
HINSTANCE g_hinstModule;

//-----------------------------------------------------------------------------
// DllMain
//-----------------------------------------------------------------------------
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            g_hinstModule = (HINSTANCE)hModule;
            break;
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
// Skip "before install" option
//-----------------------------------------------------------------------------
SETUP_API codeINSTALL_INIT Install_Init(
    HWND        hwndParent,
    BOOL        fFirstCall,
    BOOL        fPreviouslyInstalled,
    LPCTSTR     pszInstallDir)
{
    return codeINSTALL_INIT_CONTINUE;
}

//-----------------------------------------------------------------------------
// Broadcast update
//-----------------------------------------------------------------------------
SETUP_API codeINSTALL_EXIT Install_Exit(
    HWND    hwndParent,
    LPCTSTR pszInstallDir,
    WORD    cFailedDirs,
    WORD    cFailedFiles,
    WORD    cFailedRegKeys,
    WORD    cFailedRegVals,
    WORD    cFailedShortcuts)
{
	::PostMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);
    return codeINSTALL_EXIT_DONE;
}

//-----------------------------------------------------------------------------
// Skip "before uninstall"
//-----------------------------------------------------------------------------
SETUP_API codeUNINSTALL_INIT Uninstall_Init(HWND hwndParent, LPCTSTR pszInstallDir)
{
	return codeUNINSTALL_INIT_CONTINUE;
}

//-----------------------------------------------------------------------------
// Broadcast update
//-----------------------------------------------------------------------------
SETUP_API codeUNINSTALL_EXIT Uninstall_Exit(HWND hwndParent)
{
	::PostMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);
    return codeUNINSTALL_EXIT_DONE;
}
//-----------------------------------------------------------------------------
