#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	//-----------------------------------------------------------------------------
	// DllExport marker
	//-----------------------------------------------------------------------------
	#define SETUP_API __declspec(dllexport)

	//-----------------------------------------------------------------------------
	// Install
	//-----------------------------------------------------------------------------
	typedef enum
	{
		codeINSTALL_INIT_CONTINUE = 0,
		codeINSTALL_INIT_CANCEL
	} codeINSTALL_INIT;

	typedef enum
	{
		codeINSTALL_EXIT_DONE = 0,
		codeINSTALL_EXIT_UNINSTALL
	}
	codeINSTALL_EXIT;

	SETUP_API codeINSTALL_INIT Install_Init(
		HWND        hwndParent,
		BOOL        fFirstCall,
		BOOL        fPreviouslyInstalled,
		LPCTSTR     pszInstallDir
	);

	SETUP_API codeINSTALL_EXIT Install_Exit(
		HWND    hwndParent,
		LPCTSTR pszInstallDir,
		WORD    cFailedDirs,
		WORD    cFailedFiles,
		WORD    cFailedRegKeys,
		WORD    cFailedRegVals,
		WORD    cFailedShortcuts
	);

	//-----------------------------------------------------------------------------
	// Uninstall
	//-----------------------------------------------------------------------------
	typedef enum
	{
		codeUNINSTALL_INIT_CONTINUE = 0,
		codeUNINSTALL_INIT_CANCEL
	} codeUNINSTALL_INIT;

	typedef enum
	{
		codeUNINSTALL_EXIT_DONE = 0
	} codeUNINSTALL_EXIT;

	SETUP_API codeUNINSTALL_INIT Uninstall_Init(HWND hwndParent, LPCTSTR pszInstallDir);
	SETUP_API codeUNINSTALL_EXIT Uninstall_Exit(HWND hwndParent);

#ifdef __cplusplus
};
#endif
