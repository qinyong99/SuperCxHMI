// LocalServer.h: interface for the CLocalServer class.
// 用于本地服务器的控制，实现类似windows服务控制的功能。
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALSERVER_H__F1DC68D4_6A63_42E8_810C_582E9F6EA090__INCLUDED_)
#define AFX_LOCALSERVER_H__F1DC68D4_6A63_42E8_810C_582E9F6EA090__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// Server State -- for CurrentState
//
#define CX_SERVER_STOPPED                0x00000001
#define CX_SERVER_START_PENDING          0x00000002
#define CX_SERVER_STOP_PENDING           0x00000003
#define CX_SERVER_RUNNING                0x00000004
#define CX_SERVER_CONTINUE_PENDING       0x00000005
#define CX_SERVER_PAUSE_PENDING          0x00000006
#define CX_SERVER_PAUSED                 0x00000007

//
// Controls
//
#define CX_SERVER_CONTROL_STOP           0x00000001
#define CX_SERVER_CONTROL_PAUSE          0x00000002
#define CX_SERVER_CONTROL_CONTINUE       0x00000003
#define CX_SERVER_CONTROL_INTERROGATE    0x00000004
#define CX_SERVER_CONTROL_SHUTDOWN       0x00000005
#define CX_SERVER_CONTROL_PARAMCHANGE    0x00000006
#define CX_SERVER_CONTROL_NETBINDADD     0x00000007
#define CX_SERVER_CONTROL_NETBINDREMOVE  0x00000008
#define CX_SERVER_CONTROL_NETBINDENABLE  0x00000009
#define CX_SERVER_CONTROL_NETBINDDISABLE 0x0000000A
#define CX_SERVER_CONTROL_UPDATE_ALL	 0x00000080
#define CX_SERVER_CONTROL_SETUP_MODIFIED 0x00000081

typedef void (WINAPI *ServerMainProc)(DWORD dwNumServicesArgs, LPTSTR* lpServiceArgVectors);
typedef void (WINAPI *ServerHandlerProc)(DWORD dwControl);

class CLocalServer  
{
public:
	CLocalServer();
	virtual ~CLocalServer();

	// 参照 StartServiceCtrlDispatcher
	BOOL StartCtrlDispatcher(LPCTSTR lpszName, ServerMainProc pfnServerProc, BOOL bStartServerPorc = FALSE);
	DWORD ControlDispatcher();

	void RegisterCtrlHandler(ServerHandlerProc pfnHandlerProc);

	BOOL SetStatus(DWORD dwStatus);

	static DWORD _stdcall _ServerThreadProc(LPVOID pParam);
	DWORD ServerThreadProc();

	BOOL ServerStart(HANDLE hPipe, DWORD dwCount);
	BOOL ServerGetStatus(HANDLE hPipe);
	BOOL ServerControl(HANDLE hPipe, DWORD dwControl);

	BOOL RegisterServiceCtrlHandler(ServerHandlerProc lpHandlerProc);

	BOOL Start(DWORD dwNumArgs, LPCWSTR* lpArgVectors);
	HANDLE OpenPipe(); 
	DWORD StartProcess();
	BOOL SendStartMessage(HANDLE hPipe, LPCWSTR *argv, DWORD argc);
	BOOL WaitForStartup();

	BOOL GetStatus(HANDLE hPipe, DWORD* pdwStatus);
	BOOL QueryStatus(DWORD* pdwStatus);

	BOOL ControlServer(DWORD dwControl, DWORD* pdwStatus);
	BOOL SendControl(HANDLE hPipe, DWORD dwControl, DWORD* pdwResult);

	void GetPipeName(CComBSTR& bstrName);
	HANDLE GetEventHandle();

	BOOL GetClassServerPath(REFCLSID clsid, CComBSTR& bstrServerPath);

public:
	CComBSTR m_bstrName; // 服务名
	CLSID m_clsidLocalServer;
		
	ServerHandlerProc m_pfnHandlerProc; // 服务器控制处理过程

	ServerMainProc m_pfnServerProc; // 服务器主调用过程
	CComBSTR m_bstrServerThreadArgs;

	DWORD m_dwStatus;
	
	HANDLE m_hServerThread;
	HANDLE m_hStopEvent;
};

#endif // !defined(AFX_LOCALSERVER_H__F1DC68D4_6A63_42E8_810C_582E9F6EA090__INCLUDED_)
