// OPCServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "OPCServerDlg.h"
#include "OPCDataSource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COPCServerDlg dialog


COPCServerDlg::COPCServerDlg(BOOL bAdd, CWnd* pParent /*=NULL*/)
	: CDialog(COPCServerDlg::IDD, pParent)
{
	m_bAdd = bAdd;
	m_pOPCDataSource = NULL;

	//{{AFX_DATA_INIT(COPCServerDlg)
	m_strName = _T("");
	m_strOPCServer = _T("");
	m_strComputer = _T("");
	//}}AFX_DATA_INIT
}


void COPCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COPCServerDlg)
	
	//}}AFX_DATA_MAPDDX_Control(pDX, IDC_LIST1, m_lbName);
	DDX_Control(pDX, IDC_COMBO1, m_cbOPCServer);
	DDX_Control(pDX, IDC_EDIT2, m_editComputer);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Control(pDX, IDC_EDIT1, m_editName);
	DDX_CBString(pDX, IDC_COMBO1, m_strOPCServer);
	DDX_Text(pDX, IDC_EDIT2, m_strComputer);
}


BEGIN_MESSAGE_MAP(COPCServerDlg, CDialog)
	//{{AFX_MSG_MAP(COPCServerDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COPCServerDlg message handlers

const GUID CATID_OPCDAServer10 = 
{ 0x63d5f430, 0xcfe4, 0x11d1, { 0xb2, 0xc8, 0x0, 0x60, 0x8, 0x3b, 0xa1, 0xfb } };
// {63D5F430-CFE4-11d1-B2C8-0060083BA1FB}

const GUID CATID_OPCDAServer20 = 
{ 0x63d5f432, 0xcfe4, 0x11d1, { 0xb2, 0xc8, 0x0, 0x60, 0x8, 0x3b, 0xa1, 0xfb } };
// {63D5F432-CFE4-11d1-B2C8-0060083BA1FB}

BOOL COPCServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitOPCServers(CATID_OPCDAServer10);
	InitOPCServers(CATID_OPCDAServer20);
	InitOPCServers();

	// get the computer server name....
	if (m_bAdd)
	{
//		DWORD dwLen = MAX_COMPUTERNAME_LENGTH+1;
//		CString strDefSvr;
//		GetComputerName(strDefSvr.GetBuffer(dwLen), &dwLen);
//		strDefSvr.ReleaseBuffer();
//		m_editComputer.SetWindowText(strDefSvr);
	}

	m_editName.SetSel(0, -1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COPCServerDlg::InitOPCServers(CATID cid)
{
	// Create the standard COM Category Manager
	ICatInformation* pICatInformation = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_StdComponentCategoriesMgr,
									NULL, CLSCTX_ALL, IID_ICatInformation,
									(void**)&pICatInformation);
	if (FAILED(hr))
		return FALSE;

	// Array of Categories
	int cIDs = 1;
	CATID IDs[1];
	IDs[0] = cid;

		// Get the IEnumCLSID interface.
	IEnumCLSID* pIEnumCLSID = NULL ;
	hr = pICatInformation->EnumClassesOfCategories(cIDs, IDs, 0, NULL, &pIEnumCLSID);

	// Get the next CLSID in the list.
//	char szFriendlyName[128];
	CLSID clsid ;

	while ((hr = pIEnumCLSID->Next(1, &clsid, NULL)) == S_OK)
	{
		WCHAR* pwszProgID = NULL;
		if (!FAILED(ProgIDFromCLSID(clsid, &pwszProgID)))
		{
			if (FindOpcClsid(clsid) == -1)
			{
				CString str = pwszProgID;
				int iIndex = m_cbOPCServer.AddString(str);
				CLSID* pClsid = new CLSID;
				memcpy(pClsid, &clsid, sizeof(CLSID));
				m_cbOPCServer.SetItemData(iIndex, (DWORD)pClsid);
			}
		
			CoTaskMemFree(pwszProgID);
	    }
  	}

	// CleanUp
	pICatInformation->Release();

	return TRUE;
}

//枚举早期版本OPC服务器
BOOL COPCServerDlg::InitOPCServers()
{
	LONG lResult;
	DWORD iIndex;
	TCHAR szSubkey[MAX_PATH];
	DWORD nSubkeyLength;
	BOOL bStopEnumerating;
	HKEY hkSubkey;
	FILETIME time;

	iIndex = 0;
	bStopEnumerating = FALSE;
	while (!bStopEnumerating)
	{
		nSubkeyLength = sizeof(szSubkey )/sizeof(szSubkey[0]);
		lResult = RegEnumKeyEx(HKEY_CLASSES_ROOT, iIndex, szSubkey, &nSubkeyLength, NULL,
			NULL, NULL, &time);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, szSubkey, 0, KEY_READ, &hkSubkey);
			if (lResult == ERROR_SUCCESS)
			{
				HKEY hk;
				lResult = RegOpenKeyEx(hkSubkey, _T( "OPC" ), 0, KEY_READ, &hk);
				if (lResult == ERROR_SUCCESS)
				{
					USES_CONVERSION;
					CLSID* pClsid = new CLSID;
					CLSIDFromProgID(T2OLE(szSubkey), pClsid);
					if (FindOpcClsid(*pClsid) == -1)
					{
						CString str = szSubkey;
						int iIndex = m_cbOPCServer.AddString(str);
						m_cbOPCServer.SetItemData(iIndex, (DWORD)pClsid);
					}
					else
					{
						delete pClsid;
					}
					
					RegCloseKey(hk);
				}

				RegCloseKey(hkSubkey);
			}
		}
		else
		{
			bStopEnumerating = TRUE;
		}
		iIndex++;
	}
	
	return TRUE;
}

void COPCServerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	int nCount = m_cbOPCServer.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		delete (CLSID *)m_cbOPCServer.GetItemData(i);
	}
}

int COPCServerDlg::FindOpcClsid(CLSID clsid)
{
	int nCount = m_cbOPCServer.GetCount();
	for (int iIndex = 0; iIndex < nCount; iIndex++)
	{
		if (IsEqualCLSID(clsid, *(CLSID *)m_cbOPCServer.GetItemData(iIndex)))
			return iIndex;
	}

	return -1;
}

void COPCServerDlg::OnOK() 
{
	UpdateData();

	if (m_strName.IsEmpty() || !m_pOPCDataSource->IsUniqueSeverName(m_strName))
	{
		m_editName.SetFocus();
		return;
	}
	if (m_strOPCServer.IsEmpty() || m_cbOPCServer.FindStringExact(-1, m_strOPCServer) == CB_ERR)
	{
		m_cbOPCServer.SetFocus();
		return;
	}
	
	CDialog::OnOK();
}
