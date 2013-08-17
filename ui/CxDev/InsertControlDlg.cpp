// InsertControlDlg.Cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertControlDlg dialog


CInsertControlDlg::CInsertControlDlg( CWnd* pParent ) :
   CDialog( CInsertControlDlg::IDD, pParent )
{
	//{{AFX_DATA_INIT(CInsertControlDlg)
	//}}AFX_DATA_INIT
}


void CInsertControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertControlDlg)
	DDX_Control(pDX, IDC_SERVERPATH, m_staticServerPath);
	DDX_Control(pDX, IDC_CONTROLS, m_lbControls);
	//}}AFX_DATA_MAP

   if( pDX->m_bSaveAndValidate )
   {
		GetSelectCLSIDs();
   }
}


BEGIN_MESSAGE_MAP(CInsertControlDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertControlDlg)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	ON_LBN_SELCHANGE(IDC_CONTROLS, OnSelchangeControls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertControlDlg message handlers

BOOL CInsertControlDlg::OnInitDialog()
{
/*	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(10,10);
	rect.right = rect.left + rect.Width() * 2 / 3;
	m_lbControls.Create(LBS_SORT | LBS_OWNERDRAWFIXED |WS_TABSTOP | WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_BORDER, rect, this, 500);
*/
   HRESULT hResult;
   CATID catid;

   hResult = m_pCatInfo.CreateInstance( CLSID_StdComponentCategoriesMgr, NULL,
	  CLSCTX_INPROC_SERVER );
   if( FAILED( hResult ) )
   {
	  TRACE( "Failed to create category manager\n" );
	  EndDialog( IDCANCEL );
	  return( TRUE );
   }

	CDialog::OnInitDialog();

   catid = CATID_Control;
   m_aImplementedCategories.Add( catid );

//   m_butIgnoreRequiredCategories.SetCheck( 0 );

   CFont* pFont = GetFont();
   if(pFont != NULL)
		m_lbControls.SetFont(pFont);
//   m_lbControls.ModifyStyleEx(0, WS_EX_RIGHTSCROLLBAR);
   m_lbControls.ModifyStyle(0, WS_VSCROLL);

   RefreshControlList();

   m_lbControls.SetFocus();

	return TRUE;
}

void GetClassServerPath( REFCLSID clsid, CString& strServerPath )
{
   HKEY hKey;
   HKEY hServerKey;
   OLECHAR szCLSID[64];
   LONG nResult;
   ULONG nBytes;
   DWORD dwType;
   LPTSTR pszServerPath;

   StringFromGUID2( clsid, szCLSID, 64 );

   hKey = NULL;
   hServerKey = NULL;
   try
   {
	  nResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, CString( "CLSID\\" )+CString(
		 szCLSID ), 0, KEY_READ, &hKey );
	  if( nResult != ERROR_SUCCESS )
	  {
		 throw( E_FAIL );
	  }

	  nResult = RegOpenKeyEx( hKey, _T( "InprocServer32" ), 0, KEY_READ,
		 &hServerKey );
	  if( nResult != ERROR_SUCCESS )
	  {
		 nResult = RegOpenKeyEx( hKey, _T( "InprocHandler32" ), 0, KEY_READ,
			&hServerKey );
		 if( nResult != ERROR_SUCCESS )
		 {
			nResult = RegOpenKeyEx( hKey, _T( "LocalServer32" ), 0, KEY_READ,
			   &hServerKey );
			if( nResult != ERROR_SUCCESS )
			{
			   throw( E_FAIL );
			}
		 }
	  }

	  nBytes = 0;
	  nResult = RegQueryValueEx( hServerKey, NULL, NULL, &dwType, NULL,
		 &nBytes );
	  if( (nResult != ERROR_SUCCESS) || (dwType != REG_SZ) )
	  {
		 throw( E_FAIL );
	  }
	  pszServerPath = LPTSTR( _alloca( nBytes ) );
	  nResult = RegQueryValueEx( hServerKey, NULL, NULL, &dwType,
		 LPBYTE( pszServerPath ), &nBytes );
	  if( (nResult != ERROR_SUCCESS) || (dwType != REG_SZ) )
	  {
		 throw( E_FAIL );
	  }

	  strServerPath = pszServerPath;

	  RegCloseKey( hKey );
	  hKey = NULL;
	  RegCloseKey( hServerKey );
	  hServerKey = NULL;
   }
   catch( HRESULT )
   {
	  if( hKey != NULL )
	  {
		 RegCloseKey( hKey );
	  }
	  if( hServerKey != NULL )
	  {
		 RegCloseKey( hServerKey );
	  }

	  strServerPath.LoadString(IDS_SERVERNOTFOUND);

	  return;
   }
}

void CInsertControlDlg::RefreshControlList()
{
   BOOL tDone;
   HRESULT hResult;
   IEnumGUIDPtr pEnum;
   ULONG nImplementedCategories;
   CATID* pcatidImpl;
   ULONG nRequiredCategories;
   CATID* pcatidReq;
   CLSID clsid;
   LPOLESTR pszName;
   CString strName;
   ULONG iCategory;
   int iItem;
   POSITION posControl;
   CString strServerPath;
   CString strString;

   m_lbControls.ResetContent();
   m_lControls.RemoveAll();

   nImplementedCategories = m_aImplementedCategories.GetSize();
   if( nImplementedCategories == 0 )
   {
	  nImplementedCategories = ULONG( -1 );
	  pcatidImpl = NULL;
   }
   else
   {
	  pcatidImpl = (CATID*)_alloca( nImplementedCategories*sizeof( CATID ) );
	  for( iCategory = 0; iCategory < nImplementedCategories; iCategory++ )
	  {
		 pcatidImpl[iCategory] = m_aImplementedCategories[iCategory];
	  }
   }

  nRequiredCategories = m_aRequiredCategories.GetSize();
  if( nRequiredCategories == 0 )
  {
	 pcatidReq = NULL;
  }
  else
  {
	 pcatidReq = (CATID*)_alloca( nRequiredCategories*sizeof( CATID ) );
	 for( iCategory = 0; iCategory < nRequiredCategories; iCategory++ )
	 {
		pcatidReq[iCategory] = m_aRequiredCategories[iCategory];
	 }
  }

   hResult = m_pCatInfo->EnumClassesOfCategories( nImplementedCategories,
	  pcatidImpl, nRequiredCategories, pcatidReq, &pEnum );
   if( FAILED( hResult ) )
   {
	  return;
   }

   tDone = FALSE;
   while( !tDone )
   {
	  hResult = pEnum->Next( 1, &clsid, NULL );
	  if( hResult == S_OK )
	  {
		 pszName = NULL;
		 hResult = OleRegGetUserType( clsid, USERCLASSTYPE_FULL, &pszName );
		 if( SUCCEEDED( hResult ) )
		 {
			strName = pszName;
			CoTaskMemFree( pszName );
			pszName = NULL;
			iItem = m_lbControls.AddString( strName );
			if(IsCLSIDInUse(clsid))
				m_lbControls.SetCheck(iItem, 1);
			posControl = m_lControls.AddTail( clsid );
			m_lbControls.SetItemDataPtr( iItem, posControl );
		 }
	  }
	  else
	  {
		 tDone = TRUE;
	  }
   }
}

static DWORD rgmapCHID[] =
{
/*   IDC_CONTROLS, HIDC_CONTROLS,
   IDC_IMPLEMENTEDCATEGORIES, HIDC_IMPLEMENTEDCATEGORIES,
   IDC_REQUIREDCATEGORIES, HIDC_REQUIREDCATEGORIES,
   IDC_IGNOREREQUIREDCATEGORIES, HIDC_IGNOREREQUIREDCATEGORIES,
*/  0, 0
};

BOOL CInsertControlDlg::OnHelpInfo( HELPINFO* pHelpInfo )
{
	return( ::WinHelp( HWND( pHelpInfo->hItemHandle ),
	  AfxGetApp()->m_pszHelpFilePath, HELP_WM_HELP, DWORD( LPVOID(
	  rgmapCHID ) ) ) );
}

void CInsertControlDlg::OnContextMenu( CWnd* pWnd, CPoint /* point */ )
{
   ::WinHelp( HWND( *pWnd ), AfxGetApp()->m_pszHelpFilePath, HELP_CONTEXTMENU,
	  DWORD( LPVOID( rgmapCHID ) ) );
}

////////////////////////////////////////////////////////////////
//功能:从用户的选择中生成对应控件的CLSID列表于m_aryClsid中,
//	   并在m_idCounts中设置列表中项的个数
////////////////////////////////////////////////////////////////
void CInsertControlDlg::GetSelectCLSIDs()
{
	//去掉旧的列表
	m_aryClsid.RemoveAll();
	
	int count = m_lbControls.GetCount();
	while(count > 0)
	{
		count--;
		if(m_lbControls.GetCheck(count) == 1)
		{	//该项被勾中
			POSITION pos = (POSITION)(m_lbControls.GetItemDataPtr(count));
			if(pos != NULL)
			{
				CLSID tmpID = m_lControls.GetAt(pos);
				m_aryClsid.AddTail(tmpID);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////
//功能:对话框初始化时根据m_arClsid预设的参数,初始化应预选的项
//参数:clsid[in]		要判定的CLSID
//返回:是TRUE,否FALSE
/////////////////////////////////////////////////////////////////
BOOL CInsertControlDlg::IsCLSIDInUse(CLSID& clsid)
{
	POSITION pos = m_aryClsid.GetHeadPosition();
	while(pos != NULL)
	{
		CLSID usedID = m_aryClsid.GetNext(pos);
		if(usedID == clsid)
			return TRUE;
	}

	return FALSE;
}

void CInsertControlDlg::OnSelchangeControls() 
{
	int iItem;
	POSITION posControl;
	CString strServerPath;
	CLSID clsid;
	CDC dc;
	CFont* pFont;
	LPTSTR pszServerPath;
	CRect rect;
	CFont* pOldFont;
	
	iItem = m_lbControls.GetCurSel();
	if( iItem != LB_ERR )
	{
		posControl = POSITION( m_lbControls.GetItemDataPtr( iItem ) );
		clsid = m_lControls.GetAt( posControl );
		GetClassServerPath( clsid, strServerPath );
		
		dc.CreateCompatibleDC( NULL );
		
		pFont = m_staticServerPath.GetFont();
		pOldFont = dc.SelectObject( pFont );
		
		// Workaround for SHLWAPI bug (in weird cases, PathCompactPath actually
		// expands the pathname)
		pszServerPath = strServerPath.GetBuffer( MAX_PATH+2 );
		m_staticServerPath.GetWindowRect( &rect );
		PathCompactPath( dc, pszServerPath, rect.Width() );
		strServerPath.ReleaseBuffer();
		
		dc.SelectObject( pOldFont );
		
		m_staticServerPath.SetWindowText( strServerPath );
	}
	else
	{
		m_staticServerPath.SetWindowText( NULL );
	}
}
