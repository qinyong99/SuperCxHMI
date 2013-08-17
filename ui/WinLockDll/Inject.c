/*****************************************************************************
 * Inject - Inject code into a remote process.                               *
 *          InjectDll()/EjectDll() - Inject a library into a remote process  *
 *          using the CreateRemoteThread()/LoadLibrary() technique.          *
 *          InjectCode()/EjectCode() - Inject code into a remote process     *
 *          using the CreateRemoteThread()/WriteProcessMemory() technique.   *
 *                                                                           *
 * (c) A. Miguel Feijão, 1/6/2004                                            *
 *                                                                           *
 * Adapted from the original code by Robert Kuster.                          *
 *                                                                           *
 *****************************************************************************/

#include <windows.h>
#include <tlhelp32.h>
#include "Inject.h"

// Remove comment if you want to inject the library
//#define INJECT_DLL 1

/**************************************
 * Inject code into a remote process. *
 **************************************/
BOOL Inject()
{
#ifdef INJECT_DLL
	return (InjectDll() != 0);
#else
	return (InjectCode() != 0);
#endif
}


/*************************************
 * Eject code from a remote process. *
 *************************************/
BOOL Eject()
{
#ifdef INJECT_DLL
	return (EjectDll() != 0);
#else
	return (EjectCode() != 0);
#endif
}

////////////////////////////////// General purpose routines //////////////////////

/******************************
 * Enable/Disable privilege.  *
 * Called with SE_DEBUG_NAME. *
 ******************************/
BOOL EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable)
{
	HANDLE				hToken;
	TOKEN_PRIVILEGES	tp;
	LUID				luid;
	BOOL				ret;

	if (!OpenProcessToken(GetCurrentProcess(),
					      TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY | TOKEN_READ,
					      &hToken))
		return FALSE;

	if (!LookupPrivilegeValue(NULL, lpszPrivilegeName, &luid))
		return FALSE;

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

	ret = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);

	CloseHandle(hToken);

	return ret;
}


/********************************************
 * Return PID using the Toolhelp functions. *
 ********************************************/
DWORD GetPIDFromNameToolhelp(char *szProcessName)
{
    typedef HANDLE (WINAPI *CREATESNAPSHOT) (DWORD, DWORD);
    typedef BOOL   (WINAPI *PROCESSWALK)    (HANDLE, LPPROCESSENTRY32); 

    HINSTANCE       hKernel;
    CREATESNAPSHOT  CreateToolhelp32Snapshot;
    PROCESSWALK     Process32First;
    PROCESSWALK     Process32Next;

    HANDLE          hSnapshot;
    PROCESSENTRY32  pe32;
    BOOL            bRes;
    char            *p;
    DWORD           dwPID = -1;

	// Check szProcessName
	if (!szProcessName)
		return -1;

    // Get Kernel32 handle
    if (!(hKernel = GetModuleHandle("Kernel32.dll")))
        return -1;

    // We must link to these functions explicitly.
    // Otherwise it will fail on Windows NT which doesn't have Toolhelp
    // functions defined in Kernel32.
    CreateToolhelp32Snapshot = (CREATESNAPSHOT) GetProcAddress(hKernel, "CreateToolhelp32Snapshot");
    Process32First = (PROCESSWALK) GetProcAddress(hKernel, "Process32First");
    Process32Next = (PROCESSWALK) GetProcAddress(hKernel, "Process32Next");
    if (!CreateToolhelp32Snapshot || !Process32First || !Process32Next)
	{
		FreeLibrary(hKernel);
		SetLastError(ERROR_PROC_NOT_FOUND);
        return -1;
	}

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return -1;

    pe32.dwSize = sizeof(pe32);
    bRes = Process32First(hSnapshot, &pe32);
    
    while (bRes)
    {
        // Strip off full path
        p = strrchr(pe32.szExeFile, '\\');
        if (p)
            p++;
        else
            p = pe32.szExeFile;

        // Process found ?
        if (stricmp(p, szProcessName) == 0)
        {
            dwPID = pe32.th32ProcessID;
            break;
        }

        bRes = Process32Next(hSnapshot, &pe32);
    }

    CloseHandle(hSnapshot);
    return dwPID;
}


/*****************************************
 * Return PID using the PSAPI functions. *
 *****************************************/
DWORD GetPIDFromNamePSAPI(char *szProcessName)
{
    typedef BOOL  (WINAPI *ENUMPROCESSES) (DWORD *, DWORD, DWORD *);
    typedef BOOL  (WINAPI *ENUMPROCESSMODULES) (HANDLE, HMODULE *, DWORD, LPDWORD);
    typedef DWORD (WINAPI *GETMODULEBASENAME) (HANDLE, HMODULE, LPTSTR, DWORD);

	DWORD				dwPID = -1;				
    HINSTANCE           hPsApi;
    ENUMPROCESSES       EnumProcesses;
    ENUMPROCESSMODULES  EnumProcessModules;
	GETMODULEBASENAME	GetModuleBaseName;
    DWORD               * PIDArray = NULL;
	DWORD               cbAlloc = 128;
    DWORD               cbNeeded;
    DWORD               nProcesses;
    HANDLE              hProcess;
    DWORD               i;
    HMODULE             hExeModule;
    char                szModuleName[MAX_PATH];

	// Check szProcessName
	if (!szProcessName)
		return -1;

    // Get PSAPI handle
    if (!(hPsApi = LoadLibrary("PSAPI.DLL")))
        return -1;

    // Load functions in PSAPI dynamically.
    // We can't be sure PSAPI.DLL is installed.
    EnumProcesses = (ENUMPROCESSES) GetProcAddress(hPsApi, "EnumProcesses");
    EnumProcessModules = (ENUMPROCESSMODULES) GetProcAddress(hPsApi, "EnumProcessModules");
	GetModuleBaseName = (GETMODULEBASENAME) GetProcAddress(hPsApi, "GetModuleBaseNameA");
    if (!EnumProcesses || !EnumProcessModules || !GetModuleBaseName)
	{
		FreeLibrary(hPsApi);
		SetLastError(ERROR_PROC_NOT_FOUND);
        return -1;
	}

    // Find needed buffer length
	do
	{
	    cbAlloc *= 2;

		if (PIDArray)
			free(PIDArray);

		PIDArray = malloc(cbAlloc);
		if (!PIDArray)
		{
			FreeLibrary(hPsApi);
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
	        return -1;
		}
		
		if (!EnumProcesses(PIDArray, cbAlloc, &cbNeeded))
		{
			FreeLibrary(hPsApi);
			free(PIDArray);
	        return -1;
		}

	} while (cbNeeded == cbAlloc);

    nProcesses = cbNeeded / sizeof(DWORD);

    for (i=0; i < nProcesses; i++)
    {
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                               FALSE,
                               PIDArray[i]);
        if (!hProcess)
            continue;

        if (!EnumProcessModules(hProcess, &hExeModule, sizeof(hExeModule), &cbNeeded))
        {
            CloseHandle(hProcess);
            continue;
        }

        GetModuleBaseName(hProcess,
                          hExeModule,
                          szModuleName,
                          sizeof(szModuleName));

        // Process found ?
        if (stricmp(szModuleName, szProcessName) == 0)
        {
            dwPID = PIDArray[i];
			CloseHandle(hProcess);
            break;
        }

        CloseHandle(hProcess);
    }

	FreeLibrary(hPsApi);
	free(PIDArray);
    return dwPID;
}


/************************************************
 * Return PID using ZwQuerySystemInformation(). *
 ************************************************/
DWORD GetPIDFromNameZwQuerySystemInformation(char *szProcessName)
{
	// Some definitions from NTDDK and other sources

	typedef LONG	NTSTATUS;
	typedef LONG	KPRIORITY;

	#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
	#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)
	#define SystemProcessesAndThreadsInformation	5

	typedef struct _CLIENT_ID {
		DWORD	    UniqueProcess;
		DWORD	    UniqueThread;
	} CLIENT_ID;

	typedef struct _UNICODE_STRING {
		USHORT	    Length;
		USHORT	    MaximumLength;
		PWSTR	    Buffer;
	} UNICODE_STRING;

	typedef struct _VM_COUNTERS {
		SIZE_T	    PeakVirtualSize;
		SIZE_T	    VirtualSize;
		ULONG	    PageFaultCount;
		SIZE_T	    PeakWorkingSetSize;
		SIZE_T	    WorkingSetSize;
		SIZE_T	    QuotaPeakPagedPoolUsage;
		SIZE_T	    QuotaPagedPoolUsage;
		SIZE_T	    QuotaPeakNonPagedPoolUsage;
		SIZE_T	    QuotaNonPagedPoolUsage;
		SIZE_T	    PagefileUsage;
		SIZE_T	    PeakPagefileUsage;
	} VM_COUNTERS;

	typedef struct _SYSTEM_THREADS {
		LARGE_INTEGER   KernelTime;
		LARGE_INTEGER   UserTime;
		LARGE_INTEGER   CreateTime;
		ULONG			WaitTime;
		PVOID			StartAddress;
		CLIENT_ID	    ClientId;
		KPRIORITY	    Priority;
		KPRIORITY	    BasePriority;
		ULONG			ContextSwitchCount;
		LONG			State;
		LONG			WaitReason;
	} SYSTEM_THREADS, * PSYSTEM_THREADS;

	// NOTE: SYSTEM_PROCESSES structure is different on NT 4 and Win2K
	typedef struct _SYSTEM_PROCESSES {
		ULONG			NextEntryDelta;
		ULONG			ThreadCount;
		ULONG			Reserved1[6];
		LARGE_INTEGER   CreateTime;
		LARGE_INTEGER   UserTime;
		LARGE_INTEGER   KernelTime;
		UNICODE_STRING  ProcessName;
		KPRIORITY	    BasePriority;
		ULONG			ProcessId;
		ULONG			InheritedFromProcessId;
		ULONG			HandleCount;
		ULONG			Reserved2[2];
		VM_COUNTERS	    VmCounters;
	#if _WIN32_WINNT >= 0x500
		IO_COUNTERS	    IoCounters;
	#endif
		SYSTEM_THREADS  Threads[1];
	} SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;

	typedef LONG (WINAPI *ZWQUERYSYSTEMINFORMATION)(UINT SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

	ZWQUERYSYSTEMINFORMATION    ZwQuerySystemInformation;
	PSYSTEM_PROCESSES           pInfo;
    HINSTANCE					hNTDll;
	WCHAR						wcProcessName[MAX_PATH+2];
	PCWSTR						pszProcessName;
	DWORD						dwPID;
	ULONG						BufferLen = 0x8000;
	LPVOID						pBuffer = NULL;
	LONG						Status;

	// Check szProcessName
	if (!szProcessName)
		return -1;

	// Convert szProcessName to Unicode
	if (!MultiByteToWideChar(CP_ACP, 0, szProcessName, -1, wcProcessName, sizeof(wcProcessName) / sizeof(WCHAR)))
		return -1;

    // Get NTDLL handle
    if (!(hNTDll = LoadLibrary("NTDLL.DLL")))
        return -1;

    // Load ZwQuerySystemInformation() dynamically
    if (!(ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(hNTDll, "ZwQuerySystemInformation")))
	{
		FreeLibrary(hNTDll);
		SetLastError(ERROR_PROC_NOT_FOUND);
        return -1;
	}

    // Find needed buffer length
    do
    {
		if (!(pBuffer = malloc(BufferLen)))
		{
			FreeLibrary(hNTDll);
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
	        return -1;
		}

		Status = ZwQuerySystemInformation(SystemProcessesAndThreadsInformation,
					                      pBuffer, BufferLen, NULL);

		if (Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			free(pBuffer);
			BufferLen *= 2;
		}
		else if (!NT_SUCCESS(Status))
		{
			free(pBuffer);
			FreeLibrary(hNTDll);
			return -1;
		}
    }
    while (Status == STATUS_INFO_LENGTH_MISMATCH);

	pInfo = (PSYSTEM_PROCESSES)pBuffer;
    for (;;)
    {
		pszProcessName = pInfo->ProcessName.Buffer;
		if (pszProcessName == NULL)
			pszProcessName = L"Idle";

        // Process found ?
        if (wcsicmp(pszProcessName, wcProcessName) == 0)
        {
			dwPID = pInfo->ProcessId;
			free(pBuffer);
			FreeLibrary(hNTDll);
			return dwPID;
        }

		if (pInfo->NextEntryDelta == 0)
			break;

		// Find the address of the next process structure
		pInfo = (PSYSTEM_PROCESSES)(((PUCHAR)pInfo) + pInfo->NextEntryDelta);
    }

    free(pBuffer);
	FreeLibrary(hNTDll);
    return -1;
}


/********************************
 * Return PID for Process Name. *
 ********************************/
DWORD GetPIDFromName(char *szProcessName)
{
    DWORD   dwPID;
    OSVERSIONINFO   info;

    info.dwOSVersionInfoSize = sizeof(info);
    GetVersionEx(&info);

	// Get PID using Toolhelp or  ZwQuerySystemInformation() depending on the OS version
    if (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
        dwPID = GetPIDFromNameToolhelp(szProcessName);
    else if (info.dwPlatformId == VER_PLATFORM_WIN32_NT)
        dwPID = GetPIDFromNameZwQuerySystemInformation(szProcessName);
    else
        dwPID = -1;

    return dwPID;
}


/////////////////////////////////////////////// Inject DLL ////////////////////////////////////////////

extern		HINSTANCE	hInst;					// Instance handle
DWORD		hLibModule;							// Handle of injected library
char		*szDllName     = "injdll.dll";			// Library to inject
char		*szProcessName = "winlogon.exe";	// Process to inject

/*****************************************
 * Load a library into a remote process. *
 *****************************************/
int InjectDll()
{
	HANDLE	hThread;				// Thread handle
	HANDLE	hProcess;				// Process handle
    DWORD	dwPID;					// Process ID
	char	szLibPath[MAX_PATH];	// Full DLL path
	void	*pLibRemote;			// Address (in the remote process) where szLibPath will be copied to

	// Enable DEBUG privilege
    if (!EnablePrivilege(SE_DEBUG_NAME, TRUE))
		return 0;

	// Get remote process id
    dwPID = GetPIDFromName(szProcessName);
    if (dwPID == -1)
		return 0;

	// Open remote process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hProcess == NULL)
		return 0;

	// Get full path of the DLL
	if (!GetModuleFileName(hInst, szLibPath, MAX_PATH))
		return 0;
	strcpy(strrchr(szLibPath, '\\') + 1 , szDllName);

	// Allocate memory in the remote process to store the szLibPath string
	pLibRemote = VirtualAllocEx(hProcess, NULL, sizeof(szLibPath), MEM_COMMIT, PAGE_READWRITE);
	if (pLibRemote == NULL)
		return 0;
	// Copy the szLibPath string to the remote process.
	if (!WriteProcessMemory(hProcess, pLibRemote, (void*)szLibPath, sizeof(szLibPath), NULL))
		return 0;

	// Load the DLL into the remote process 
	// (via CreateRemoteThread() & LoadLibrary())
	hThread = CreateRemoteThread(hProcess, 
								 NULL, 
								 0,	
								 (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA"), 
								 pLibRemote, 
								 0, 
								 NULL);
	// Failed
	if(hThread == NULL)
	{
		VirtualFreeEx(hProcess, pLibRemote, sizeof(szLibPath), MEM_RELEASE);
		return 0;
	}

	// Wait for LoadLibrary() to finish and get return code (handle of loaded library)
	WaitForSingleObject(hThread, INFINITE);
	GetExitCodeThread(hThread, &hLibModule);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	// Free remote memory for szLibPath
	VirtualFreeEx(hProcess, pLibRemote, sizeof(szLibPath), MEM_RELEASE);

	// Remote LoadLibrary() failed
	if (!hLibModule)
		return 0;

	// Disable DEBUG privilege
	EnablePrivilege(SE_DEBUG_NAME, FALSE);

	return 1;
}


/****************************************************
 * Unload injected library from the remote process. *
 ****************************************************/
int EjectDll()
{
	HANDLE	hThread;
	HANDLE	hProcess;
    DWORD	dwPID;
	DWORD	dwRetFreeLibrary;

	// Enable DEBUG privilege
    if (!EnablePrivilege(SE_DEBUG_NAME, TRUE))
		return 0;

	// Get remote process id
    dwPID = GetPIDFromName(szProcessName);
    if (dwPID == -1)
		return 0;

	// Open remote process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hProcess == NULL)
		return 0;

	// Unload the DLL from the remote process 
	// (via CreateRemoteThread & FreeLibrary)
	hThread = CreateRemoteThread(hProcess,
								 NULL, 
								 0,
								 (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "FreeLibrary"),
								 (void*)hLibModule,
								 0, 
								 NULL );
	// Failed to unload
	if (hThread == NULL)
		return 0;

	// Wait for FreeLibrary() to finish and get return code
	WaitForSingleObject(hThread, INFINITE);
	GetExitCodeThread(hThread, &dwRetFreeLibrary);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	// Disable DEBUG privilege
	EnablePrivilege(SE_DEBUG_NAME, FALSE);

	return (dwRetFreeLibrary);
}

////////////////////////////////////////////// Inject Code ////////////////////////////////////////

/**************************************************************************************************************
 * Some tips when using CreateRemoteThread()/WriteProcessMemory().                                            *
 *                                                                                                            *
 * 1. Analise generated code.                                                                                 *
 *    Check injected functions for absolute addressing (calls, jumps, data references, ...)                   * 
 *    and other generated code that shouldn't be there.                                                       *
 *    Use:                                                                                                    *
 *    A) Project\Settings\C/C++\Listing Files\Listing file type=Assembly, Machine Code, and Source.           *
 *    B) A disassembler (wdasm32).                                                                            *
 *    C) A debugger (softice).                                                                                *
 *                                                                                                            *
 * 2. Turn off stack probes.                                                                                  *
 *    Check for __chkstk() references in the listing files.                                                   *
 *    A) Use #pragma check_stack(off). [DOESN'T SEEMS TO WORK ?!?]                                            *
 *    B) Use less than 4K of local variables.                                                                 *
 *    C) Augment the stack size: /Gs size (Project\Settings\C/C++\ProjectOptions)                             *
 *                                                                                                            *
 * 3. Remove the /GZ switch in the debug build.                                                               *
 *    Check for __chkesp() references in the listing files.                                                   *
 *    A) Project\Settings\C/C++\Project Options                                                               *
 *                                                                                                            *
 * 4. Disable incremental compilation (/Gi).                                                                  *
 *    A) Use #pragma comment(linker, "/INCREMENTAL:NO")                                                       *
 *    B) Remove the /Gi switch (Project\Settings\C/C++\Customize\Enable incremental compilation=Off           *
 *    C) Declare the functions as static.                                                                     *
 *                                                                                                            *
 * 5. Don't let optimization screw your code.                                                                 *
 *    A) Turn off optimization (Project\Settings\C/C++\General\Optimizations=Disable(Debug)                   *
 *    B) Use #pragma optimize("", off)                                                                        *
 *    C) Don't write functions with the same prototype (e.g. AfterFuncX()). Let them return different values. *
 *                                                                                                            *
 * 6. Split switch() statements in 3 cases maximum, or use if/then/else.                                      *
 *                                                                                                            *
 * 7. Don't call any functions besides those in KERNEL32.LL and USER32.DLL (USER32.DLL isn't garanted to be   *
 *    mapped into every process).                                                                             *
 *    Use LoadLibrary()/GetProcAddress if you need functions from other libraries.                            *
 *                                                                                                            *
 * 8. Don't use any static strings.                                                                           *
 *    Pass them in INJDATA.                                                                                   *
 *                                                                                                            *
 * 9. Don't call any function directly.                                                                       *
 *    Copy each routine to the remote process individually and supply their addresses in INJDATA.             *
 *                                                                                                            *
 * 10. Good luck.                                                                                             *
 *     If you analise the generated code (using a disassembler) you should catch any errors before executing  *
 *     the code (and crashing the process !).                                                                 *
 **************************************************************************************************************/  

#pragma comment(linker, "/INCREMENTAL:NO")	// Turns off incremental linking

// Global variables
DWORD	PID;						// PID of injected process
BYTE	*pDataRemote;				// Address of INJDATA in the remote process
BYTE	*pSASWinProcRemote;			// The address of SASWindowProc() in the remote process

#define	DUMMY_ADDR	0x12345678		// Dummy addr of INJDATA

// INJDATA: Memory block passed to each remote injected function.
// We pass every function address or string data in this block.
typedef LONG		(WINAPI *SETWINDOWLONG)	  (HWND, int, LONG); 
typedef LRESULT		(WINAPI *CALLWINDOWPROC)  (WNDPROC, HWND, UINT, WPARAM, LPARAM);
typedef HWND		(WINAPI *FINDWINDOW)	  (LPCTSTR, LPCTSTR);

typedef struct {
	SETWINDOWLONG	fnSetWindowLong;	// Addr. of SetWindowLong()
	CALLWINDOWPROC	fnCallWindowProc;	// Addr. of CallWindowProc()
	FINDWINDOW		fnFindWindow;		// Addr. of FindWindow()
	char			szClassName[50];	// Class name = "SAS Window class"
	char			szWindowName[50];	// Window name = "SAS window"
	HWND			hwnd;				// Window handle of injected process
	WNDPROC			fnSASWndProc;		// Addr. of remote SASWindowProc
	WNDPROC 		fnOldSASWndProc;	// Addr. of old SASWindowProc
} INJDATA, *PINJDATA;


/*****************************************************************
 * Subclassed window procedure handler for the injected process. *
 *****************************************************************/

#pragma optimize("", off)
#pragma check_stack(off)

static LRESULT CALLBACK SASWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// INJDATA pointer. 
	// Must be patched at runtime !
	INJDATA* pData = (INJDATA *)DUMMY_ADDR;

	if (uMsg == WM_HOTKEY)
	{
		// Ctrl+Alt+Del
		if (lParam == MAKELONG(MOD_CONTROL | MOD_ALT, VK_DELETE))
			return 1;

		// Ctrl+Shift+Esc
		if (lParam == MAKELONG(MOD_CONTROL | MOD_SHIFT, VK_ESCAPE))
			return 1;
	}

	// Call the original window procedure
	return pData->fnCallWindowProc(pData->fnOldSASWndProc, hwnd, uMsg, wParam, lParam);		
}


static int AfterSASWindowProc(void) {return 1;}



/*************************************************
 * Subclass the remote process window procedure. *
 * Return: 0=failure, 1=success                  *
 *************************************************/

#pragma optimize("", off)
#pragma check_stack(off)

static DWORD WINAPI InjectFunc (INJDATA *pData) 
{
	// Subclass window procedure
	pData->fnOldSASWndProc = (WNDPROC) pData->fnSetWindowLong (pData->hwnd, GWL_WNDPROC, (long)pData->fnSASWndProc);	

	return (pData->fnOldSASWndProc != NULL);
}


static int AfterInjectFunc(void) {return 2;}


/***********************************************************
 * Restore the subclassed remote process window procedure. *
 * Return: 0=failure, 1=success                            *
 ***********************************************************/

#pragma optimize("", off)
#pragma check_stack(off)

static DWORD WINAPI EjectFunc (INJDATA *pData) 
{
	return (pData->fnSetWindowLong(pData->hwnd, GWL_WNDPROC, (long)pData->fnOldSASWndProc) != 0);
}


static int AfterEjectFunc(void) {return 3;}



/**************************************************************
 * Return the window handle of the remote process (Winlogon). *
 **************************************************************/

#pragma optimize("", off)
#pragma check_stack(off)

static HWND WINAPI GetSASWnd (INJDATA *pData) 
{
	return (pData->fnFindWindow(pData->szClassName, pData->szWindowName));
}


static int AfterGetSASWnd(void) {return 4;}



/***************************************************************************
 * Copies InjectFunc(), GetSASWnd() , SASWindowProc() and INJDATA to the   *
 * remote process.                                                         *
 * Starts the execution of the remote InjectFunc(), which subclasses the   *
 * remote process default window procedure handler.                        *
 *                                                                         *
 * Return value: 0=failure, 1=success                                      *
 ***************************************************************************/

int InjectCode ()
{
	HANDLE		hProcess = 0;			// Process handle
	HMODULE		hUser32  = 0;			// Handle of user32.dll
	BYTE		*pCodeRemote;			// Address of InjectFunc() in the remote process.
	BYTE		*pGetSASWndRemote;		// Address of GetSASWnd() in the remote process.
	HANDLE		hThread	= 0;			// The handle and ID of the thread executing
	DWORD		dwThreadId = 0;			//   the remote InjectFunc().
	INJDATA		DataLocal;				// INJDATA structure
	BOOL		fUnicode;				// TRUE if remote process is Unicode
	int			nSuccess = 0;			// Subclassing succeded?
	DWORD		dwNumBytesCopied = 0;	// Number of bytes written to the remote process.
	DWORD		size;					// Calculated function size (= AfterFunc() - Func())
	int			SearchSize;				// SASWindowProc() dummy addr. search size
	int			nDummyOffset;			// Offset in SASWindowProc() of dummy addr.
	BOOL		FoundDummyAddr;			// Dummy INJDATA reference found in SASWindowProc() ?
	HWND		hSASWnd;				// Window handle of Winlogon process
	BYTE		*p;

	// Enable Debug privilege (needed for some processes)
    if (!EnablePrivilege(SE_DEBUG_NAME, TRUE))
		return 0;

	// Get handle of "USER32.DLL"
	hUser32 = GetModuleHandle("user32");
	if (!hUser32)
		return 0;

	// Get remote process ID
    PID = GetPIDFromName(szProcessName);
    if (PID == -1)
		return 0;

	// Open remote process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProcess)
		return 0;

	__try 
	{
		// Initialize INJDATA for GetSASWnd() call
		strcpy(DataLocal.szClassName, "SAS Window class");
		strcpy(DataLocal.szWindowName, "SAS window");
		DataLocal.fnFindWindow = (FINDWINDOW) GetProcAddress(hUser32, "FindWindowA");
		if (DataLocal.fnFindWindow == NULL)
			__leave;

		// Allocate memory in the remote process and write a copy of initialized INJDATA into it
		size = sizeof(INJDATA);
		pDataRemote = (PBYTE) VirtualAllocEx(hProcess, 0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);		
		if (!pDataRemote)
			__leave;
		if (!WriteProcessMemory(hProcess, pDataRemote, &DataLocal, size, &dwNumBytesCopied) || dwNumBytesCopied != size)
			__leave;

		// Allocate memory in remote process and write a copy of GetSASWnd() into it
		size = (PBYTE)AfterGetSASWnd - (PBYTE)GetSASWnd;
		pGetSASWndRemote = (PBYTE) VirtualAllocEx(hProcess, 0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);		
		if (!pGetSASWndRemote)
			__leave;
		if (!WriteProcessMemory(hProcess, pGetSASWndRemote, &GetSASWnd, size, &dwNumBytesCopied) || dwNumBytesCopied != size)
			__leave;
		
		// Start execution of remote GetSASWnd()
		hThread = CreateRemoteThread(hProcess, 
									 NULL, 
									 0, 
									 (LPTHREAD_START_ROUTINE) pGetSASWndRemote,
									 pDataRemote, 
									 0 , 
									 &dwThreadId);
		// Failed
		if (!hThread)
			__leave;

		// Wait for GetSASWnd() to terminate and get return code (SAS Wnd handle)
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, (PDWORD) &hSASWnd);

		// Didn't found "SAS window"
		if (!hSASWnd)
			__leave;

		// Cleanup
		VirtualFreeEx(hProcess, pGetSASWndRemote, 0, MEM_RELEASE);
		VirtualFreeEx(hProcess, pDataRemote, 0, MEM_RELEASE);
		pGetSASWndRemote = NULL;
		pDataRemote = NULL;
		
		// Allocate memory in remote process and write a copy of SASWindowProc() into it
		size = (PBYTE)AfterSASWindowProc - (PBYTE)SASWindowProc;
		pSASWinProcRemote = (PBYTE) VirtualAllocEx(hProcess, 0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);		
		if (!pSASWinProcRemote)
			__leave;
		if (!WriteProcessMemory(hProcess, pSASWinProcRemote, &SASWindowProc, size, &dwNumBytesCopied) || dwNumBytesCopied != size)
			__leave;

		// Is remote process unicode ?
		fUnicode = IsWindowUnicode(hSASWnd);

		// Initialize the INJDATA structure
		DataLocal.fnSetWindowLong = (SETWINDOWLONG)  GetProcAddress(hUser32, /*fUnicode ? "SetWindowLongW" : */"SetWindowLongA");
		DataLocal.fnCallWindowProc = (CALLWINDOWPROC) GetProcAddress(hUser32, /*fUnicode ? "CallWindowProcW": */"CallWindowProcA");
		DataLocal.fnSASWndProc = (WNDPROC) pSASWinProcRemote;
		DataLocal.hwnd = hSASWnd;

		if (DataLocal.fnSetWindowLong  == NULL || 			
			DataLocal.fnCallWindowProc == NULL)
		{
			__leave;		
		}

		// Allocate memory in the remote process and write a copy of initialized INJDATA into it
		size = sizeof(INJDATA);
		pDataRemote = (PBYTE) VirtualAllocEx(hProcess, 0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);		
		if (!pDataRemote)
			__leave;
		if (!WriteProcessMemory(hProcess, pDataRemote, &DataLocal, size, &dwNumBytesCopied) || dwNumBytesCopied != size)
			__leave;

		// Change dummy INJDATA address in SASWindowProc() by the real INJDATA pointer
		p = (PBYTE)&SASWindowProc;
		size = (PBYTE)AfterSASWindowProc - (PBYTE)SASWindowProc;
		SearchSize = size - sizeof(DWORD) + 1;
		FoundDummyAddr = FALSE;

		for (; SearchSize > 0; p++, SearchSize--)
		{
			if (*(DWORD *)p == DUMMY_ADDR)	// Found 
			{
				nDummyOffset = p - (PBYTE)&SASWindowProc; 
				if (!WriteProcessMemory(hProcess, pSASWinProcRemote + nDummyOffset, &pDataRemote, sizeof(pDataRemote), &dwNumBytesCopied) ||
					dwNumBytesCopied != sizeof(pDataRemote))
				{
					__leave;
				}
				FoundDummyAddr = TRUE;
				break;
			}
		}

		// Couldn't change the dummy INJDATA addr. by the real addr. in SASWindowProc() !?!
		// Don't execute the remote copy of SASWindowProc() because the pData pointer is invalid !
		if (!FoundDummyAddr)
		{
			__leave;
		}

		// Allocate memory in the remote process and write a copy of InjectFunc() to the allocated memory
		size = (PBYTE)AfterInjectFunc - (PBYTE)InjectFunc;
		pCodeRemote = (PBYTE) VirtualAllocEx(hProcess, 0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (!pCodeRemote)
			__leave;
		if (!WriteProcessMemory(hProcess, pCodeRemote, &InjectFunc, size, &dwNumBytesCopied) || dwNumBytesCopied != size)
			__leave;

		// Start execution of remote InjectFunc()
		hThread = CreateRemoteThread(hProcess, 
									 NULL, 
									 0, 
									 (LPTHREAD_START_ROUTINE) pCodeRemote,
									 pDataRemote, 
									 0 , 
									 &dwThreadId);
		if (!hThread)
			__leave;

		// Wait for InjectFunc() to terminate and get return code
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, (PDWORD) &nSuccess);

		// InjectFunc() successfull
//		if (nSuccess)
//			MessageBeep(0);
	}

	__finally 
	{
		// Failed ?
		if (!nSuccess)
		{
			// Release memory for INJDATA and SASWindowProc()
			if (pDataRemote)
				VirtualFreeEx(hProcess, pDataRemote, 0, MEM_RELEASE);
			if (pSASWinProcRemote)
				VirtualFreeEx(hProcess, pSASWinProcRemote, 0, MEM_RELEASE);
			pDataRemote = NULL;
			pSASWinProcRemote = NULL;
		}

		// Release remote GetSASWnd()
		if (pGetSASWndRemote)	
			VirtualFreeEx(hProcess, pGetSASWndRemote, 0, MEM_RELEASE);

		// Release remote InjectFunc() (no longer needed)
		if (pCodeRemote)	
			VirtualFreeEx(hProcess, pCodeRemote, 0, MEM_RELEASE);

		if (hThread)			
			CloseHandle(hThread);

	}

	CloseHandle(hProcess);

	// Disable the DEBUG privilege
	EnablePrivilege(SE_DEBUG_NAME, FALSE);

	return nSuccess;	// 0=failure; 1=success
}


/**********************************************************************
 * Copies EjectFunc() to the remote process and starts its execution. *
 * The remote EjectFunc() restores the old window procedure.          *
 *                                                                    *
 *	Return value: 0=failure, 1=success                                *
 **********************************************************************/

int EjectCode ()
{
	HANDLE		hProcess;				// Remote process handle
	DWORD		*pCodeRemote;			// Address of EjectFunc() in the remote process
	HANDLE		hThread = NULL;			// The handle and ID of the thread executing
	DWORD		dwThreadId = 0;			//   the remote EjectFunc().
	int			nSuccess	= 0;		// EjectFunc() success ?
	DWORD		dwNumBytesCopied = 0;	// Number of bytes written to the remote process. 
	DWORD		size;					// Calculated function size (= AfterFunc() - Func())

	// Enable Debug privilege (needed for some processes)
	EnablePrivilege(SE_DEBUG_NAME, TRUE);

	// Remote INDATA and SASWindowProc() must exist 
	if (!pDataRemote || !pSASWinProcRemote)
		return 0;

	// Open the process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL)
		return 0;

	// Allocate memory in the remote process and write a copy of EjectFunc() to the allocated memory
	size = (PBYTE)AfterEjectFunc - (PBYTE)EjectFunc;
	pCodeRemote = (PDWORD) VirtualAllocEx(hProcess, 0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pCodeRemote)
	{
		CloseHandle(hProcess);
		return 0;
	}
	if (!WriteProcessMemory(hProcess, pCodeRemote, &EjectFunc, size, &dwNumBytesCopied) || dwNumBytesCopied != size)
	{
		VirtualFreeEx(hProcess, pCodeRemote, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 0;
	}

	// Start execution of the remote EjectFunc()
	hThread = CreateRemoteThread(hProcess, 
								 NULL, 
								 0, 
								 (LPTHREAD_START_ROUTINE) pCodeRemote,
								 pDataRemote, 
								 0 , 
								 &dwThreadId);
	// Failed
	if (!hThread)
	{
		goto END;
	}

	// Wait for EjectFunc() to terminate and get return code
	WaitForSingleObject(hThread, INFINITE);	
	GetExitCodeThread(hThread, (PDWORD) &nSuccess);	

	// Failed to restore old window procedure ?
	// Then leave INJDATA and the SASWindowProc()
	if (nSuccess == 0)		
		goto END;			
							
	// Release memory for remote INJDATA and SASWindowProc()
	if (pDataRemote)
		VirtualFreeEx(hProcess, pDataRemote, 0, MEM_RELEASE);
	if (pSASWinProcRemote)
		VirtualFreeEx(hProcess, pSASWinProcRemote, 0, MEM_RELEASE);
	pDataRemote = NULL;
	pSASWinProcRemote = NULL;

//	MessageBeep(0);		// success

END:		
	if (hThread)
		CloseHandle(hThread);

	// Release EjectFunc() memory
	if (pCodeRemote)
		VirtualFreeEx(hProcess, pCodeRemote, 0, MEM_RELEASE);

	CloseHandle(hProcess);

	// Disable the DEBUG privilege
	EnablePrivilege(SE_DEBUG_NAME, FALSE);

	return nSuccess;	// 0=failure; 1=success
}

