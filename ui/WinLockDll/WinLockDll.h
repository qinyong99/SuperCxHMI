#ifndef WINLOCKDLL_H
#define WINLOCKDLL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _THREAD_DATA
{
	HDESK hDesk;
	char  szDesktopName[20];
} THREAD_DATA;

enum 
{
	DISABLE_KEY_ALT_TAB = 0x1,
	DISABLE_KEY_WIN = 0x2,
	DISABLE_KEY_ALT_ESC = 0x4,
	DISABLE_KEY_CTRL_ESC = 0x8,
	DISABLE_KEY_CTRL_SHIFT_ESC = 0x10,
	DISABLE_KEY_ALL = 0x1f,
};

#ifdef  WINLOCKDLL_EXPORTS
#define DLL_EXP_IMP __declspec(dllexport)
#else
#define DLL_EXP_IMP __declspec(dllimport)
#endif

DLL_EXP_IMP int WINAPI Desktop_Show_Hide(BOOL bShowHide);
DLL_EXP_IMP int WINAPI StartButton_Show_Hide(BOOL bShowHide);
DLL_EXP_IMP int WINAPI Taskbar_Show_Hide(BOOL bShowHide);
DLL_EXP_IMP int WINAPI Clock_Show_Hide(BOOL bShowHide);
DLL_EXP_IMP int WINAPI Keys_Enable_Disable(BOOL bEnableDisable);
DLL_EXP_IMP int WINAPI AltTab1_Enable_Disable(BOOL bEnableDisable);
DLL_EXP_IMP int WINAPI AltTab2_Enable_Disable(HWND hWnd, BOOL bEnableDisable);
DLL_EXP_IMP int WINAPI TaskSwitching_Enable_Disable(BOOL bEnableDisable, DWORD dwMask);
DLL_EXP_IMP int WINAPI TaskManager_Enable_Disable(BOOL bEnableDisable);
DLL_EXP_IMP int WINAPI CtrlAltDel_Enable_Disable(BOOL bEnableDisable);
DLL_EXP_IMP int WINAPI Thread_Desktop(LPTHREAD_START_ROUTINE ThreadFunc, THREAD_DATA *td);
DLL_EXP_IMP int WINAPI Process_Desktop(char *szDesktopName, char *szPath);

#ifdef __cplusplus
}
#endif

#endif