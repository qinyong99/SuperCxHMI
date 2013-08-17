// RunCtrl.cpp : Implementation of the CRunCtrl ActiveX Control class.

#include "stdafx.h"
#include "RunInc.h"
#include "CxRunCtrl.h"
#include "RunCtrl.h"
#include "RunCtrlPpg.h"
#include "BitmapView.h"
#ifdef L_IMPL_OBJECTSAFETY
#include <objsafe.h>
#endif // L_IMPL_OBJECTSAFETY


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRunCtrl, CActiveXDocControl)

#ifdef L_IMPL_OBJECTSAFETY
BEGIN_INTERFACE_MAP(CRunCtrl, CActiveXDocControl)
INTERFACE_PART(CRunCtrl, IID_IObjectSafety, ObjectSafety)
END_INTERFACE_MAP()
#endif // L_IMPL_OBJECTSAFETY

short _AfxShiftState()
{
	BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
	BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);
	BOOL bAlt   = (GetKeyState(VK_MENU) < 0);
	
	return (short)(bShift + (bCtrl << 1) + (bAlt << 2));
}

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRunCtrl, CActiveXDocControl)
	//{{AFX_MSG_MAP(CRunCtrl)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_REPLACE_PICTURE, OnReplacePicture)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CRunCtrl, CActiveXDocControl)
	//{{AFX_DISPATCH_MAP(CRunCtrl)
	DISP_PROPERTY_EX(CRunCtrl, "Picture", GetPicture, SetPicture, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "RefreshEnabled", m_bRefreshEnabled, OnRefreshEnabledChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "ZoomFitWindow", m_bZoomFitWindow, OnZoomFitWindowChanged, VT_BOOL)
	DISP_PROPERTY_EX(CRunCtrl, "ZoomRatio", GetZoomRatio, SetZoomRatio, VT_R4)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "AllowMouseZoom", m_bAllowMouseZoom, OnAllowMouseZoomChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "AllowScrollBars", m_bAllowScrollBars, OnAllowScrollBarsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "DragMode", m_bDragMode, OnDragModeChanged, VT_BOOL)
	DISP_PROPERTY_EX(CRunCtrl, "WindowHandle", GetWindowHandle, SetNotSupported, VT_HANDLE)
	DISP_PROPERTY_EX(CRunCtrl, "CurrentPath", GetCurrentPath, SetCurrentPath, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "Server", m_server, OnServerChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "Port", m_lPort, OnPortChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "QueryString", m_strQueryString, OnQueryStringChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "DefaultUser", m_strDefaultUser, OnDefaultUserChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "DefaultPassword", m_strDefaultPassword, OnDefaultPasswordChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "CreateServerObjectAsLocal", m_bCreateServerObjectAsLocal, OnCreateServerObjectAsLocalChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CRunCtrl, "LocalServerUseSACProxy", m_bLocalServerUseSACProxy, OnLocalServerUseSACProxyChanged, VT_BOOL)
	DISP_PROPERTY_PARAM(CRunCtrl, "Application", GetApplication, SetApplication, VT_VARIANT, VTS_BSTR)
	DISP_PROPERTY_PARAM(CRunCtrl, "Frame", GetFrame, SetFrame, VT_VARIANT, VTS_BSTR)
	DISP_FUNCTION(CRunCtrl, "GetActiveWindow", GetActiveWindow, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "ClearPicturesCache", ClearPicturesCache, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "GoBack", GoBack, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "GoForward", GoForward, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "CanBack", CanBack, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "CanForward", CanForward, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "SetHighlightMouseObject", SetHighlightMouseObject, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CRunCtrl, "GetMainMenu", GetMainMenu, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "GetMainForm", GetMainForm, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "ZoomIn", ZoomIn, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "ZoomOut", ZoomOut, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "CanZoomIn", CanZoomIn, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "CanZoomOut", CanZoomOut, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "ReloadPicture", ReloadPicture, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CRunCtrl, "VerifySoftLock", VerifySoftLock, VT_BOOL, VTS_BOOL)
	DISP_FUNCTION(CRunCtrl, "GetInternalObject", GetInternalObject, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CRunCtrl, "CurrentVersion", 101, GetCurrentVersion, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX_ID(CRunCtrl, "ZoomX", 102, GetZoomX, SetZoomX, VT_R4)
	DISP_PROPERTY_EX_ID(CRunCtrl, "ZoomY", 103, GetZoomY, SetZoomY, VT_R4)
	DISP_STOCKPROP_BORDERSTYLE()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CRunCtrl, CActiveXDocControl)
	//{{AFX_EVENT_MAP(CRunCtrl)
	EVENT_CUSTOM("PictrueSize", FirePictrueSize, VTS_I4  VTS_I4)
	EVENT_CUSTOM("OnQuit", FireOnQuit, VTS_NONE)
	EVENT_CUSTOM("BeforeShowPicture", FireBeforeShowPicture, VTS_BSTR  VTS_PBOOL)
	EVENT_CUSTOM("ShowPictureComplete", FireShowPictureComplete, VTS_BSTR)
	EVENT_CUSTOM("ContainerContext", FireContainerContext, VTS_PVARIANT)
	EVENT_CUSTOM("FrameItemChanged", FireFrameItemChanged, VTS_BSTR)
	EVENT_STOCK_CLICK()
	EVENT_STOCK_DBLCLICK()
	EVENT_STOCK_MOUSEDOWN()
	EVENT_STOCK_MOUSEMOVE()
	EVENT_STOCK_MOUSEUP()
	EVENT_STOCK_KEYDOWN()
	EVENT_STOCK_KEYPRESS()
	EVENT_STOCK_KEYUP()
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CRunCtrl, 1)
	PROPPAGEID(CRunCtrlPropPage::guid)
END_PROPPAGEIDS(CRunCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRunCtrl, "CXRUNCTRL.RunCtrl.1",
	0xc162a5cc, 0x6bb1, 0x4c0a, 0x90, 0x81, 0xc5, 0x20, 0xa4, 0xea, 0xdd, 0xa7)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CRunCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DRunCtrl =
		{ 0x1e9b944, 0xb3e1, 0x4883, { 0xa8, 0xd7, 0x94, 0x5e, 0xb1, 0xf7, 0x6b, 0x6f } };
const IID BASED_CODE IID_DRunCtrlEvents =
		{ 0xf3231ef3, 0xbc1a, 0x4198, { 0x9a, 0xb0, 0xd2, 0xa, 0xf5, 0xd1, 0xf4, 0x7 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwRunCtrlOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE/*
	 |
		OLEMISC_NOUIACTIVATE*/
	;

IMPLEMENT_OLECTLTYPE(CRunCtrl, IDS_RUNCTRL, _dwRunCtrlOleMisc)


#ifdef L_IMPL_OBJECTSAFETY
// Implementation of IObjectSafety
STDMETHODIMP CRunCtrl::XObjectSafety::GetInterfaceSafetyOptions(
			REFIID riid, 
			DWORD __RPC_FAR *pdwSupportedOptions, 
			DWORD __RPC_FAR *pdwEnabledOptions)
{
	METHOD_PROLOGUE_EX(CRunCtrl, ObjectSafety)

	if (!pdwSupportedOptions || !pdwEnabledOptions)
	{
		return E_POINTER;
	}

	*pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA;
	*pdwEnabledOptions = 0;

	if (NULL == pThis->GetInterface(&riid))
	{
		TRACE("Requested interface is not supported.\n");
		return E_NOINTERFACE;
	}

	// What interface is being checked out anyhow?
	OLECHAR szGUID[39];
	int i = StringFromGUID2(riid, szGUID, 39);

	if (riid == IID_IDispatch)
	{
		// Client wants to know if object is safe for scripting
		*pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER;
		return S_OK;
	}
	else if (riid == IID_IPersistPropertyBag 
		  || riid == IID_IPersistStreamInit
		  || riid == IID_IPersistStorage
		  || riid == IID_IPersistMemory)
	{
		// Those are the persistence interfaces COleControl derived controls support
		// as indicated in AFXCTL.H
		// Client wants to know if object is safe for initializing from persistent data
		*pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA;
		return S_OK;
	}
	else
	{
		// Find out what interface this is, and decide what options to enable
		TRACE("We didn't account for the safety of this interface, and it's one we support...\n");
		return E_NOINTERFACE;
	}	
}

STDMETHODIMP CRunCtrl::XObjectSafety::SetInterfaceSafetyOptions(
		REFIID riid, 
		DWORD dwOptionSetMask, 
		DWORD dwEnabledOptions)
{
	METHOD_PROLOGUE_EX(CRunCtrl, ObjectSafety)

	OLECHAR szGUID[39];
	// What is this interface anyway?
	// We can do a quick lookup in the registry under HKEY_CLASSES_ROOT\Interface
	int i = StringFromGUID2(riid, szGUID, 39);

	if (0 == dwOptionSetMask && 0 == dwEnabledOptions)
	{
		// the control certainly supports NO requests through the specified interface
		// so it's safe to return S_OK even if the interface isn't supported.
		return S_OK;
	}

	// Do we support the specified interface?
	if (NULL == pThis->GetInterface(&riid))
	{
		TRACE1("%s is not support.\n", szGUID);
		return E_FAIL;
	}


	if (riid == IID_IDispatch)
	{
		TRACE("Client asking if it's safe to call through IDispatch.\n");
		TRACE("In other words, is the control safe for scripting?\n");
		if (INTERFACESAFE_FOR_UNTRUSTED_CALLER == dwOptionSetMask && INTERFACESAFE_FOR_UNTRUSTED_CALLER == dwEnabledOptions)
		{
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else if (riid == IID_IPersistPropertyBag 
		  || riid == IID_IPersistStreamInit
		  || riid == IID_IPersistStorage
		  || riid == IID_IPersistMemory)
	{
		TRACE("Client asking if it's safe to call through IPersist*.\n");
		TRACE("In other words, is the control safe for initializing from persistent data?\n");

		if (INTERFACESAFE_FOR_UNTRUSTED_DATA == dwOptionSetMask && INTERFACESAFE_FOR_UNTRUSTED_DATA == dwEnabledOptions)
		{
			return NOERROR;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		TRACE1("We didn't account for the safety of %s, and it's one we support...\n", szGUID);
		return E_FAIL;
	}
}

STDMETHODIMP_(ULONG) CRunCtrl::XObjectSafety::AddRef()
{
	METHOD_PROLOGUE_EX_(CRunCtrl, ObjectSafety)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CRunCtrl::XObjectSafety::Release()
{
	METHOD_PROLOGUE_EX_(CRunCtrl, ObjectSafety)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CRunCtrl::XObjectSafety::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CRunCtrl, ObjectSafety)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

#endif // L_IMPL_OBJECTSAFETY

/////////////////////////////////////////////////////////////////////////////
// CRunCtrl::CRunCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CRunCtrl

BOOL CRunCtrl::CRunCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_RUNCTRL,
			IDB_RUNCTRL,
			afxRegApartmentThreading,
			_dwRunCtrlOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CRunCtrl::CRunCtrl - Constructor

DWORD CRunCtrl::m_dwInstanceCount = 0;

CRunCtrl::CRunCtrl()
{
	InitializeIIDs(&IID_DRunCtrl, &IID_DRunCtrlEvents);

	AddDocTemplate(new CActiveXDocTemplate(
			RUNTIME_CLASS(CFormDoc),
			RUNTIME_CLASS(CRunFrame), 
			RUNTIME_CLASS(CRunView)));

	m_pPictureString = NULL;
	m_pPictureDoc = NULL;
	m_bBusying = FALSE;

	m_strCurrentPath = theApp.m_strLastPicturePath;

	m_bPrintPreview = FALSE;

	m_posGo = NULL;
	m_bGo = FALSE;

	m_lPort = -1;
	m_fZoomRatio = 1.0;
	m_fZoomX = -1.0;
	m_fZoomY = -1.0;
	m_bAllowMouseZoom = FALSE;
	m_bAllowScrollBars = FALSE;
	m_bDragMode = FALSE;
	m_bCreateServerObjectAsLocal = FALSE;
	m_bLocalServerUseSACProxy = FALSE;
	
	m_dwInstanceCount++;

	//加载GDIPLUS
	GdiplusStartup(&m_gdiplusToken, &m_gdiStartup, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CRunCtrl::~CRunCtrl - Destructor

CRunCtrl::~CRunCtrl()
{
	if (m_pPictureString != NULL)
		delete m_pPictureString;

	if (m_gdiplusToken != NULL)
		GdiplusShutdown(m_gdiplusToken);

	m_dwInstanceCount--;
	if (m_dwInstanceCount == 0)
		theApp.ClearApplicationVariables();
}

/////////////////////////////////////////////////////////////////////////////
// CRunCtrl message handlers

void CRunCtrl::OnServerChanged()
{
	USES_CONVERSION;

	// 重新打开画面
	if (m_pPictureDoc != NULL)
	{
		ClosePicture();
		OnShowPicture();
	}

	SetModifiedFlag();
}

void CRunCtrl::OnPortChanged()
{
	USES_CONVERSION;
	
	// 重新打开画面
	if (m_pPictureDoc != NULL)
	{
		ClosePicture();
		OnShowPicture();
	}
	
	SetModifiedFlag();
}

void CRunCtrl::OnCreateServerObjectAsLocalChanged()
{
}

void CRunCtrl::OnLocalServerUseSACProxyChanged()
{

}

void CRunCtrl::OnQueryStringChanged()
{
	SetModifiedFlag();
}

void CRunCtrl::OnDefaultUserChanged() 
{
	SetModifiedFlag();
}

void CRunCtrl::OnDefaultPasswordChanged() 
{
	SetModifiedFlag();
}

VARIANT CRunCtrl::GetApplication(LPCTSTR pszItem) 
{
	return theApp.GetApplication(pszItem);
}

void CRunCtrl::SetApplication(LPCTSTR pszItem, const VARIANT FAR& newValue) 
{
	theApp.SetApplication(pszItem, newValue);
}

VARIANT CRunCtrl::GetFrame(LPCTSTR pszItem) 
{
	if (m_pPictureDoc != NULL)
		return m_pPictureDoc->GetFrame(pszItem);

	VARIANT vaResult;
	VariantInit(&vaResult);
	return vaResult;
}

void CRunCtrl::SetFrame(LPCTSTR pszItem, const VARIANT FAR& newValue) 
{
	if (m_pPictureDoc != NULL)
		m_pPictureDoc->SetFrame(pszItem, newValue);
}

void CRunCtrl::SetHighlightMouseObject(boolean bHighlight)
{
	theApp.m_bHighlightMouseObject = bHighlight;
}

BSTR CRunCtrl::GetPicture() 
{
	return m_picture.AllocSysString();
}

BSTR CRunCtrl::GetCurrentVersion()
{
	CString strVer;
	strVer.Format(_T("%d.%d"), _wVerMajor, _wVerMinor);

	return strVer.AllocSysString();
}

void CRunCtrl::UpdateAllRunViewPara()
{
	if (m_pPictureDoc != NULL)
	{
		m_pPictureDoc->SetAllRunView(m_bRefreshEnabled, m_bZoomFitWindow, m_fZoomRatio, m_fZoomX, m_fZoomY,
			m_bAllowMouseZoom, m_bAllowScrollBars, m_bDragMode);
	}
}

void CRunCtrl::OnShowPicture()
{
	if (!m_bGo && !m_picture.IsEmpty())
	{
		m_listGoPictures.AddTail(m_picture);
		POSITION pos = m_listGoPictures.GetTailPosition();
		while (pos != NULL && pos != m_posGo)
		{
			m_listGoPictures.RemoveTail();
			pos = m_listGoPictures.GetTailPosition();
		}

		m_posGo = m_listGoPictures.AddTail(m_picture);
	}

	if (m_pPictureDoc == NULL)
	{
		if (!m_picture.IsEmpty())
		{
			if (m_strCurrentPath.IsEmpty())
				SetCurrentPath(GetCurrentPicturePath());
			
			CString strPath = m_picture;
			int nIndex = strPath.ReverseFind(_T('?'));
			if (nIndex != -1)
			{
				m_strOpenParameter = strPath.Mid(nIndex + 1);
				strPath = strPath.Left(nIndex);
			}
			else
			{
				m_strOpenParameter.Empty();
			}

			m_bBusying = TRUE;
			strPath = GetQualityPictureName(strPath);
		
			CDevDoc::ShowPicture(this, strPath);
			UpdateAllRunViewPara();
			
			m_bBusying = FALSE;
		}
	}
	else
	{
		if (m_picture.IsEmpty())
		{
			ClosePicture();
		}
		else
		{
			CString* pPictureString = new CString(m_picture);
			PostMessage(WM_REPLACE_PICTURE, 0, (LPARAM)pPictureString);
		}
	}
}

void CRunCtrl::SetPicture(LPCTSTR lpszNewValue) 
{
	CString strNewPath = GetQualityPictureName(lpszNewValue);
	CString strOldPath = GetQualityPictureName(m_picture);
	
	m_picture = lpszNewValue;
	
	if (strOldPath.CompareNoCase(strNewPath) == 0)
	{
		if (m_pPictureDoc != NULL && theApp.m_pCacheFileManager != NULL)
		{
			BOOL bExist = FALSE;
			theApp.m_pCacheFileManager->OpenCacheFile(strNewPath, m_pPictureDoc->m_pIDataSession, &bExist);
			if (bExist)
				return;
		}
	}
	
	OnShowPicture();

	SetModifiedFlag();
}

CString CRunCtrl::GetQualityPictureName(LPCTSTR lpszPath)
{
	CString strDocPath = lpszPath;

	//如果不是全路径
	if (strDocPath.FindOneOf(_T("\\/:")) == -1)
	{
		CString strPath = GetCurrentPicturePath();
		strDocPath = strPath + strDocPath;
	}
	
	if (!strDocPath.IsEmpty())
	{
		TCHAR chT = strDocPath[strDocPath.GetLength() - 1];
		if (chT != '\\' && chT != '/')
		{	
			//没有扩展名
			CString strExt = strDocPath.Right(4);
			if (strExt != _T(".grf"))
				strDocPath += _T(".grf");
		}
	}

	return strDocPath;
}

LRESULT CRunCtrl::OnReplacePicture(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_pPictureDoc != NULL);

	m_pPictureDoc->ResetAllRunView();

	// 等待脚本停止
	if (!m_pPictureDoc->CanReplacePicture())
	{
		if (m_pPictureString != NULL)
			delete m_pPictureString;
		m_pPictureString = (CString *)lParam;
		m_nTimer = SetTimer(5, 50, NULL);

		return 0;
	}

	CString* pPictrueString = (CString *)lParam;
	CString strPath = *pPictrueString;
	delete pPictrueString;

	int nIndex = strPath.ReverseFind(_T('?'));
	if (nIndex != -1)
	{
		m_strOpenParameter = strPath.Mid(nIndex + 1);
		strPath = strPath.Left(nIndex);
	}
	else
	{
		m_strOpenParameter.Empty();
	}

	strPath = GetQualityPictureName(strPath);

	m_bBusying = TRUE;
	BOOL bReplace = m_pPictureDoc->OnOpenDocument(strPath);
	m_bBusying = FALSE;

	if (!bReplace)
		ClosePicture();
	else
		m_pPictureDoc->UpdateAllViews(NULL);

	return 0;
}

void CRunCtrl::OnTimer(UINT nIDEvent) 
{
	ASSERT(m_pPictureString != NULL);

	PostMessage(WM_REPLACE_PICTURE, 0, (LPARAM)m_pPictureString);
	KillTimer(m_nTimer);
	m_pPictureString = NULL;
		
	CActiveXDocControl::OnTimer(nIDEvent);
}

BOOL CRunCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CRunCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect&)
{
	if (m_pPictureDoc == NULL)
	{
		if (!m_bBusying)
		{
			if (m_bitmap.m_hObject == NULL)
			{
				if (!m_bitmap.LoadBitmap(IDB_RUNCTRL))
					return;
			}
			
			CDC dcImage;
			if (!dcImage.CreateCompatibleDC(pdc))
				return;
			
			BITMAP bm;
			m_bitmap.GetBitmap(&bm);
			
			// Paint the image.
			CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
			pdc->StretchBlt(rcBounds.left, rcBounds.top, rcBounds.Width(), rcBounds.Height(), &dcImage, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			
			dcImage.SelectObject(pOldBitmap);
		}
			
//
//		pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
//
//		HICON hIcon = LoadIcon(theApp.m_hInstance, MAKEINTRESOURCE(IDR_CXRUNTYPE));
//		pdc->DrawIcon(rcBounds.TopLeft(), hIcon);

	}
}

void CRunCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor), TRUE);
	CActiveXDocControl::DoPropExchange(pPX);
	
	PX_String(pPX, _T("Server"), m_server, _T(""));

	if (!pPX->IsLoading() || m_dwVersionLoaded > 0x10000)
		PX_Long(pPX, _T("Port"), m_lPort, -1);
	if (!pPX->IsLoading() || m_dwVersionLoaded > 0x10002)
		PX_String(pPX, _T("QueryString"), m_strQueryString, _T(""));
	if (!pPX->IsLoading() || m_dwVersionLoaded > 0x10001)
	{
		PX_String(pPX, _T("DefaultUser"), m_strDefaultUser, _T(""));
		PX_String(pPX, _T("DefaultPassword"), m_strDefaultPassword, _T(""));
	}

	PX_String(pPX, _T("Picture"), m_picture, _T(""));
	PX_Bool(pPX, _T("RefreshEnabled"), m_bRefreshEnabled, TRUE);
	PX_Bool(pPX, _T("ZoomFitWindow"), m_bZoomFitWindow, FALSE);
	
	if (!pPX->IsLoading() || m_dwVersionLoaded > 0x10003)
		PX_Float(pPX, _T("ZoomRatio"), m_fZoomRatio, 1.0f);
	if (!pPX->IsLoading() || m_dwVersionLoaded > 0x10004)
		PX_Bool(pPX, _T("AllowScrollBars"), m_bAllowScrollBars, FALSE);
	if (!pPX->IsLoading() || m_dwVersionLoaded > 0x10005)
	{
		PX_Bool(pPX, _T("AllowMouseZoom"), m_bAllowMouseZoom, FALSE);
		PX_Bool(pPX, _T("DragMode"), m_bDragMode, FALSE);

#ifdef _DEBUG
//		m_bDragMode = TRUE;
#endif
	}
}

HRESULT CRunCtrl::OnActivateInPlace(BOOL bUIActivate, LPMSG pMsg)
{
	HRESULT hr = CActiveXDocControl::OnActivateInPlace(bUIActivate, pMsg);

	if (m_pPictureDoc == NULL && !m_picture.IsEmpty())
		OnShowPicture();

	return hr;
}

void CRunCtrl::OnDestroy() 
{
	ClosePicture();

	CActiveXDocControl::OnDestroy();
}

void CRunCtrl::OnRefreshEnabledChanged() 
{
	UpdateAllRunViewPara();
	SetModifiedFlag();
}

OLE_HANDLE CRunCtrl::GetWindowHandle() 
{
	return (OLE_HANDLE)m_hWnd;
}

void CRunCtrl::OnZoomFitWindowChanged() 
{
	UpdateAllRunViewPara();
	SetModifiedFlag();
}

float CRunCtrl::GetZoomRatio()
{
	return m_fZoomRatio;
}

void CRunCtrl::SetZoomRatio(float fNewValue)
{
	// 缩放比例范围
	if (fNewValue < 0.0833f || fNewValue > 64)
		return;

	m_fZoomRatio = fNewValue;
	m_bZoomFitWindow = FALSE;
	
	UpdateAllRunViewPara();
	SetModifiedFlag();
}

float CRunCtrl::GetZoomX()
{
	return m_fZoomX;
}

void CRunCtrl::SetZoomX(float fNewValue)
{
	m_fZoomX = fNewValue;

	UpdateAllRunViewPara();
	SetModifiedFlag();
}

float CRunCtrl::GetZoomY()
{
	return m_fZoomY;
}

void CRunCtrl::SetZoomY(float fNewValue)
{
	m_fZoomY = fNewValue;

	UpdateAllRunViewPara();
	SetModifiedFlag();
}

// 常用缩放率
static float g_fGeneralZoomRatios[] = 
{
	0.0833f,
	0.125,
	0.25,
	0.5,
	1,
	1.25,
	1.5,
	2,
	4,
	8,
	16,
	24,
	32,
	64,
};

int CRunCtrl::GetNeareastGeneralZoomRatio()
{
	int nCount = sizeof(g_fGeneralZoomRatios) / sizeof(g_fGeneralZoomRatios[0]);
	for (int i = 0; i < nCount - 1; i++)
	{
		if (g_fGeneralZoomRatios[i + 1] > m_fZoomRatio)
			break;
	}

	if (fabs(m_fZoomRatio - g_fGeneralZoomRatios[i]) < fabs(g_fGeneralZoomRatios[i + 1] - m_fZoomRatio))
		return i;

	return i + 1;
}

void CRunCtrl::ZoomIn()
{
	int nCount = sizeof(g_fGeneralZoomRatios) / sizeof(g_fGeneralZoomRatios[0]);

	int nIndex = GetNeareastGeneralZoomRatio();
	if (nIndex == nCount - 1 && m_fZoomRatio != g_fGeneralZoomRatios[nIndex])
		nIndex--;
	if (nIndex < nCount - 1)
		SetZoomRatio(g_fGeneralZoomRatios[nIndex + 1]);
}

void CRunCtrl::ZoomOut()
{
	int nCount = sizeof(g_fGeneralZoomRatios) / sizeof(g_fGeneralZoomRatios[0]);

	int nIndex = GetNeareastGeneralZoomRatio();
	if (nIndex == 0 && m_fZoomRatio != g_fGeneralZoomRatios[nIndex])
		nIndex++;
	if (nIndex > 0)
		SetZoomRatio(g_fGeneralZoomRatios[nIndex - 1]);
}

BOOL CRunCtrl::CanZoomIn()
{
	int nCount = sizeof(g_fGeneralZoomRatios) / sizeof(g_fGeneralZoomRatios[0]);
	return m_fZoomRatio < g_fGeneralZoomRatios[nCount - 1];
}

BOOL CRunCtrl::CanZoomOut()
{
	return m_fZoomRatio > g_fGeneralZoomRatios[0];
}

void CRunCtrl::ReloadPicture()
{
	OnShowPicture();
}

BOOL CRunCtrl::VerifySoftLock(boolean bAllowDemoStatus)
{
	return TRUE;
}

void CRunCtrl::OnAllowMouseZoomChanged()
{
	UpdateAllRunViewPara();
	SetModifiedFlag();
}

void CRunCtrl::OnAllowScrollBarsChanged()
{
	UpdateAllRunViewPara();
	SetModifiedFlag();
}

void CRunCtrl::OnDragModeChanged()
{
	UpdateAllRunViewPara();
	SetModifiedFlag();
}

void CRunCtrl::KeyDown(USHORT* pnChar)
{
	USHORT nShiftState = _AfxShiftState();
	FireKeyDown(pnChar, nShiftState);
	
	// If handler set *pnChar to zero, cancel further processing.
	if (*pnChar != 0)
		OnKeyDownEvent(*pnChar, nShiftState);
}

void CRunCtrl::KeyUp(USHORT* pnChar)
{
	USHORT nShiftState = _AfxShiftState();
	FireKeyUp(pnChar, nShiftState);
	
	// If handler set *pnChar to zero, cancel further processing.
	if (*pnChar != 0)
		OnKeyUpEvent(*pnChar, nShiftState);
}	

void CRunCtrl::ButtonDown(USHORT iButton, UINT, CPoint point)
{
	if (m_iButtonState == 0)
		m_iButtonState |= iButton;
		
	FireMouseDown(iButton, _AfxShiftState(), point.x, point.y);
	
	m_iDblClkState &= ~iButton;
}

void CRunCtrl::ButtonUp(USHORT iButton, UINT, CPoint point)
{
	if (m_iButtonState != 0)
	{
		m_iButtonState &= ~iButton;
		
		FireMouseUp(iButton, _AfxShiftState(), point.x, point.y);
		
		if (!(m_iDblClkState & iButton))
		{
			CRect rect;
			GetClientRect(&rect);
			if (rect.PtInRect(point))
				FireClick();
		}
		
		m_iDblClkState &= ~iButton;
	}
}

void CRunCtrl::ButtonDblClk(USHORT iButton, UINT, CPoint)
{
	m_iButtonState |= iButton;
	
	FireDblClick();
	m_iDblClkState |= iButton;
}

void CRunCtrl::MouseMove(UINT /*nFlags*/, CPoint point)
{
	FireMouseMove((short)m_iButtonState, _AfxShiftState(), point.x, point.y);
}

LPDISPATCH CRunCtrl::GetActiveWindow() 
{
	LPDISPATCH lpDispatch = NULL;

	if (m_pPictureDoc != NULL)
	{
		CRunView* pView = m_pPictureDoc->GetActiveRunView();
		if (pView != NULL)
			pView->GetDocument()->GetInterface(&IID_IUnknown)->QueryInterface(IID_IDispatch, (void **)&lpDispatch);
	}

	return lpDispatch;
}

void CRunCtrl::ClearPicturesCache()
{
 	if (theApp.m_pCacheFileManager != NULL)
 		theApp.m_pCacheFileManager->Clear();
}

LPDISPATCH CRunCtrl::GetMainMenu()
{
	LPDISPATCH lpDispatch = NULL;
	
	if (m_pPictureDoc != NULL && m_pPictureDoc->m_pMainForm != NULL)
	{
		CDevObj* pObj = m_pPictureDoc->m_pMainForm->GetMenuObject();
		if (pObj != NULL)
			pObj->GetObjectUnknown()->QueryInterface(IID_IDispatch, (void **)&lpDispatch);
	}
	
	return lpDispatch;
}

LPDISPATCH CRunCtrl::GetMainForm()
{
	LPDISPATCH lpDispatch = NULL;
	
	if (m_pPictureDoc != NULL && m_pPictureDoc->m_pMainForm != NULL)
		m_pPictureDoc->m_pMainForm->GetInterface(&IID_IUnknown)->QueryInterface(IID_IDispatch, (void **)&lpDispatch);
	
	return lpDispatch;
}

LPDISPATCH CRunCtrl::GetInternalObject()
{
	LPDISPATCH lpDispatch = NULL;
	
	if (m_pPictureDoc != NULL && m_pPictureDoc->m_pMainForm != NULL)
		lpDispatch = m_pPictureDoc->m_pMainForm->GetScriptEngine()->GetIDispatch(TRUE);
	
	return lpDispatch;
}

void CRunCtrl::GoBack()
{
	ASSERT(m_posGo != NULL);
	if (m_posGo == NULL)
		return;

	POSITION pos = m_posGo;
	m_listGoPictures.GetPrev(pos);
	ASSERT(pos != NULL);
	if (pos == NULL)
		return;

	m_posGo = pos;

	CString strPicture = m_listGoPictures.GetAt(m_posGo);

	m_bGo = TRUE;
	SetPicture(strPicture);
	m_bGo = FALSE;
}

void CRunCtrl::GoForward()
{
	ASSERT(m_posGo != NULL);
	if (m_posGo == NULL)
		return;

	POSITION pos = m_posGo;
	m_listGoPictures.GetNext(pos);
	ASSERT(pos != NULL);
	if (pos == NULL)
		return;
	
	m_posGo = pos;

	CString strPicture = m_listGoPictures.GetAt(m_posGo);	

	m_bGo = TRUE;
	SetPicture(strPicture);
	m_bGo = FALSE;
}

BOOL CRunCtrl::CanBack()
{
	return (m_posGo != NULL && m_listGoPictures.GetHeadPosition() != m_posGo); 
}

BOOL CRunCtrl::CanForward()
{
	return (m_posGo != NULL && m_listGoPictures.GetTailPosition() != m_posGo); 
}

CString CRunCtrl::GetCurrentPicturePath()
{
	CString strPath = m_strCurrentPath;
	
	if (strPath.IsEmpty())
	{
		if (m_pPictureDoc != NULL)
			strPath = ((CDevDoc *)m_pPictureDoc)->GetPathName();
		else if (!m_picture.IsEmpty())
			strPath = m_picture;
		else
			return strPath;
		
		LPTSTR lpszTemp = (LPTSTR)(LPCTSTR)strPath;
		for (LPCTSTR lpsz = lpszTemp; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// remember last directory/drive separator
			if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
				lpszTemp = (LPTSTR)_tcsinc(lpsz);
		}
		
		strPath = strPath.Left(lpszTemp - (LPTSTR)(LPCTSTR)strPath);
	}
	
	return strPath;
}

BSTR CRunCtrl::GetCurrentPath() 
{
	CString strPath = GetCurrentPicturePath();
	return strPath.AllocSysString();
}

void CRunCtrl::SetCurrentPath(LPCTSTR lpszNewValue) 
{
	m_strCurrentPath = lpszNewValue;
	theApp.m_strLastPicturePath = m_strCurrentPath;

	SetModifiedFlag();
}

void CRunCtrl::ClosePicture()
{
	if (m_pPictureDoc != NULL && !m_bBusying)
	{	
		CDevDoc* pDoc = m_pPictureDoc;
		m_pPictureDoc = NULL;
		pDoc->OnCloseDocument();
	}
}

void CRunCtrl::PrintDirect(CDocument* pDocument, CRuntimeClass* pViewClass)
{
	// 创建框架
	CCreateContext context;
	context.m_pCurrentDoc = pDocument;
	context.m_pNewViewClass = pViewClass;
	
	CFrameWnd* pFrameWnd = (CFrameWnd*)new CPrintFrame;
	ASSERT_KINDOF(CFrameWnd, pFrameWnd);

	if (!pFrameWnd->LoadFrame(IDR_PRINT,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
		this, &context))
	{
		return;
	}

    pFrameWnd->InitialUpdateFrame(NULL, FALSE);

	CFrameWnd* pOldFrame = (CFrameWnd*)AfxGetThread()->m_pMainWnd;
	AfxGetApp()->m_pMainWnd = pFrameWnd;

	CPrintView* pView = (CPrintView *)pFrameWnd->GetActiveView();
    pView->SendMessage(WM_COMMAND, ID_FILE_PRINT);

	AfxGetApp()->m_pMainWnd = pOldFrame;
	
	// cleanup
 	pFrameWnd->DestroyWindow();
}

void CRunCtrl::PrintPreview(CRunView* pPrintView, CDocument* pDocument, CRuntimeClass* pViewClass)
{
	if (m_bPrintPreview)
        return;

	m_pFrameWnd->BeginModalState();

	// 创建框架
	CCreateContext context;
	context.m_pCurrentDoc = pDocument;
	context.m_pNewViewClass = pViewClass;
	
	CFrameWnd* pFrameWnd = (CFrameWnd*)new CPrintFrame;
	ASSERT_KINDOF(CFrameWnd, pFrameWnd);

	if (!pFrameWnd->LoadFrame(IDR_PRINT,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
		this, &context))
	{
		ASSERT(FALSE);

		delete pFrameWnd;
		m_pFrameWnd->EndModalState();
		m_bPrintPreview = FALSE;

		return;
	}

	// 禁止父窗口
	HWND hWndTop;
	HWND hWndParent = CWnd::GetSafeOwner_(GetSafeHwnd(), &hWndTop);
	BOOL bEnableParent = FALSE;
	if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}
	// 停止鼠标捕获
	HWND hWndCapture = ::GetCapture();
	if (hWndCapture != NULL)
		::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);

	m_bPrintPreview = TRUE;
	
    pFrameWnd->InitialUpdateFrame(NULL, FALSE);

//	CWnd* pOldTopParent = GetTopLevelParent();
//	pOldTopParent->EnableWindow(FALSE);

	pFrameWnd->SetWindowText(_T("打印预览"));

	WINDOWPLACEMENT wp;
	::GetWindowPlacement(hWndParent, &wp);
	if (wp.showCmd == SW_SHOWMAXIMIZED)
	{
		pFrameWnd->SetWindowPlacement(&wp);	
	}
	else
	{
		//	CRect rcWindow;
		//	GetWindowRect(&rcWindow);
		
		CRect rcTopWindow;
		::GetWindowRect(hWndParent, &rcTopWindow);
		
		//	// 在顶层窗口比本窗口还小时，最大化预览窗口
		//	if (rcWindow.left <= rcTopWindow.left &&
		//		rcWindow.top <= rcTopWindow.top &&
		//		rcWindow.right >= rcTopWindow.right &&
		//		rcWindow.bottom >= rcTopWindow.bottom)
		//	{
		//		pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);	
		//	}
		//	else
		//	{
		
		CRect rect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		rect.IntersectRect(&rect, &rcTopWindow);
		
		pFrameWnd->MoveWindow(&rect);
		pFrameWnd->ShowWindow(SW_SHOW);
		//	}
	}

	CFrameWnd* pOldFrame = (CFrameWnd*)AfxGetThread()->m_pMainWnd;
	AfxGetApp()->m_pMainWnd = pFrameWnd;
 
	CPrintView* pView = (CPrintView *)pFrameWnd->GetActiveView();
    pView->OnFilePrintPreview(this, pPrintView);

	// setup for modal loop and creation
	pFrameWnd->m_nFlags |= WF_CONTINUEMODAL;

	if (pFrameWnd->ContinueModal() && m_bPrintPreview)
	{
		// enter modal loop
		DWORD dwFlags = MLF_SHOWONIDLE;
		if (pFrameWnd->GetStyle() & DS_NOIDLEMSG)
			dwFlags |= MLF_NOIDLEMSG;
		pFrameWnd->RunModalLoop(dwFlags);
	}

	// hide the window before enabling parent window, etc.
	pFrameWnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
		SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

	if (bEnableParent)
		::EnableWindow(hWndParent, TRUE);
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);

	AfxGetApp()->m_pMainWnd = pOldFrame;
	
	// cleanup
	pFrameWnd->DestroyWindow();
	
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);

	m_pFrameWnd->EndModalState();

	m_bPrintPreview = FALSE;
}

void AFXAPI GetClippingCoordinates(LPCRECT pPosRect, LPCRECT pClipRect,
	LPRECT pIntersectRect, LPPOINT pOffsetPoint)
{
	int clipLeft = 0;
	int clipTop = 0;

	if ((pClipRect == NULL) || IsRectEmpty(pClipRect))
	{
		CopyRect(pIntersectRect, pPosRect);
	}
	else
	{
		IntersectRect(pIntersectRect, pPosRect, pClipRect);
		clipLeft = pClipRect->left;
		clipTop = pClipRect->top;
	}

	pOffsetPoint->x = min(0, pPosRect->left - clipLeft);
	pOffsetPoint->y = min(0, pPosRect->top - clipTop);
}

BOOL CRunCtrl::OnSetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect) 
{ 
	if (!AmbientUserMode())
		return COleControl::OnSetObjectRects(lprcPosRect, lprcClipRect);

	ASSERT(lprcPosRect != NULL);

	// Remember the position rectangle for later
	m_rcPos = *lprcPosRect;

	// Calculate complete rectangle including the tracker (if present)
	CRect rectPos = m_rcPos;
	if (m_bUIActive && m_pRectTracker != NULL)
	{
		// Save new clipping rectangle (for DestroyTracker)
		if (lprcClipRect != NULL)
			m_pRectTracker->m_rectClip = *lprcClipRect;

		// Adjust tracker rectangle to new dimensions
		CRect rectTmp = rectPos;
		rectTmp.OffsetRect(-rectTmp.left, -rectTmp.top);
		m_pRectTracker->m_rect = rectTmp;

		// Adjust the "true" rectangle to include handles/hatching
		UINT nHandleSize = m_pRectTracker->m_nHandleSize - 1;
		rectPos.InflateRect(nHandleSize, nHandleSize);
	}

	// Now clip that rectangle as appropriate
	CRect rectClip;
	GetClippingCoordinates(rectPos, lprcClipRect, rectClip, &m_ptOffset);

	// Move outer window first. then inner window

	if (!m_bInPlaceSiteWndless)
	{
		CWnd* pWndOuter = GetOuterWindow();
		if (pWndOuter != NULL)
		{
			::SetWindowPos(pWndOuter->m_hWnd, NULL, rectClip.left, rectClip.top,
				rectClip.Width(), rectClip.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
		
			// 调用下面语句会导致在浏览器中拖动卷动条出现窗口边框
		//	::MoveWindow(pWndOuter->m_hWnd, rectClip.left, rectClip.top,
		//		rectClip.Width(), rectClip.Height(), TRUE);
		}
		if (pWndOuter != this)
		{
			SetWindowPos(NULL, m_ptOffset.x, m_ptOffset.y,
				rectPos.Width(), rectPos.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	
			// 调用下面语句会导致在浏览器中拖动卷动条出现窗口边框
//			MoveWindow(m_ptOffset.x, m_ptOffset.y, rectPos.Width(), rectPos.Height());
		}

		// 必须重新绘制窗口，否则将无法正确刷新
//		if (m_pFrameWnd != NULL)
//			m_pFrameWnd->GetActiveView()->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
//		else
			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	return TRUE;
}

void CRunCtrl::OnSetFocus(CWnd* pOldWnd)
{
	if (m_pFrameWnd != NULL)
	{
		CView* pView = m_pFrameWnd->GetActiveView();
		if (pView != NULL)
			pView->SetFocus();
	}
	else
	{
		COleControl::OnSetFocus(pOldWnd);
	}
}

BOOL CRunCtrl::IsInvokeAllowed(DISPID dispid)
{
	if (dispid == 101)
		return TRUE;

	return CActiveXDocControl::IsInvokeAllowed(dispid);
}
