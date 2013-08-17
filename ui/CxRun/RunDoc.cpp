// RunDoc.cpp : implementation of the CRunDoc class
//

#include "stdafx.h"
#include "CxRun.h"
#include "RunDoc.h"
#include "RunView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunDoc

IMPLEMENT_DYNCREATE(CRunDoc, CDocument)

BEGIN_MESSAGE_MAP(CRunDoc, CDocument)
	//{{AFX_MSG_MAP(CRunDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CRunDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CRunDoc)
	DISP_PROPERTY_EX(CRunDoc, "Visible", GetVisible, SetVisible, VT_BOOL)
	DISP_FUNCTION(CRunDoc, "OpenPicture", OpenPicture, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CRunDoc, "OpenStartPicture", OpenStartPicture, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICxRun to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8EA3D836-7E0E-4279-8B68-E02EA26B84A4}
static const IID IID_ICxRun =
{ 0x8ea3d836, 0x7e0e, 0x4279, { 0x8b, 0x68, 0xe0, 0x2e, 0xa2, 0x6b, 0x84, 0xa4 } };

BEGIN_INTERFACE_MAP(CRunDoc, CDocument)
	INTERFACE_PART(CRunDoc, IID_ICxRun, Dispatch)
	INTERFACE_PART(CRunDoc, IID_IDispatch, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunDoc construction/destruction

CRunDoc::CRunDoc()
{
	m_bOpened = FALSE;
	m_dwCookie = 0;
	m_bLockedExternal = FALSE;

	EnableAutomation();

	AfxOleLockApp();
}

CRunDoc::~CRunDoc()
{
	AfxOleUnlockApp();
}

BOOL CRunDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	RegisterAsActive();

	return TRUE;
}

BOOL CRunDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
 	m_bOpened = TRUE;

	RegisterAsActive();
	return TRUE;
}

void CRunDoc::OnCloseDocument() 
{
	//If we didn't throw an external lock on, we can just
	//close the document normally, otherwise, we should
	//unlock our document using CoLockObjectExternal.  We
	//are registered as ACTIVEOBJECT_WEAK so in this instance
	//unlocking the object with CoLockObjectExternal will be
	//the last reference count on the object which will remove
	//it from the ROT.
	
	if (!m_bLockedExternal)
	{
		CDocument::OnCloseDocument();
	}
	else
	{
		if (m_bOpened)
	 		InternalRelease();

		if (m_dwCookie != 0)
			RevokeActiveObject(m_dwCookie, NULL);

		m_bLockedExternal = FALSE;
		CoLockObjectExternal(GetControllingUnknown(), FALSE, TRUE);
	}
}

BOOL CRunDoc::SaveModified() 
{
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// CRunDoc serialization

void CRunDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CDocument enables serialization
	//  of the container document's COleClientItem objects.
	CDocument::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CRunDoc diagnostics

#ifdef _DEBUG
void CRunDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRunDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRunDoc commands


void CRunDoc::RegisterAsActive()
{
	//This helper function actually registers our
	//object as running in the running object table (ROT).
	//Register the object by passing the IUnknown pointer
	//to the object and the CLSID from our CWinApp object
	//which was given to us by AppWizard
	CRunApp *pApp = (CRunApp *)AfxGetApp();
	HRESULT hr = RegisterActiveObject(GetControllingUnknown(),
		pApp->GetAppCLSID(),
		ACTIVEOBJECT_WEAK,
		&m_dwCookie);
	if (FAILED(hr))
	{
		AfxMessageBox("Didn't register Active Object");
	}
}

BOOL CRunDoc::GetVisible() 
{
	return (AfxGetMainWnd()->GetStyle() & WS_VISIBLE) != 0;
}

// 暂时没有处理 bNewValue = FALSE 的情形
void CRunDoc::SetVisible(BOOL bNewValue) 
{
	CMainFrame* pFrameWnd = (CMainFrame *)theApp.m_pMainWnd;
	if (theApp.m_bFullScreen)
		pFrameWnd->FullScreenModeOn();
	else
		pFrameWnd->ActivateFrame(SW_SHOW);
	
	//Make sure the framework knows the user took control.
	//When an automation method makes an applications main window
	//visible, then the user is in control and the app should not
	//be shut down when the client disconnects from it
	AfxOleSetUserCtrl(TRUE);
	
	//We've put our user in control of our application, now we
	//should make sure our automation object, which is our document
	//in this case, doesn't get destroyed when the last user
	//disconnects from us.
	if (!m_bLockedExternal) //only lock external once
	{
		HRESULT hr = CoLockObjectExternal(GetControllingUnknown(), TRUE, TRUE);
		if (FAILED(hr))
		{
			AfxMessageBox("Didn't CoLockObjectExternal");
			return;
		}
		m_bLockedExternal = TRUE;
	}
}

void CRunDoc::OpenPicture(LPCTSTR lpszPathName) 
{
	POSITION pos = GetFirstViewPosition();
	CRunView* pView = (CRunView *)GetNextView(pos);
	if (pView != NULL)
	{
		pView->ClearPicturesCache();
		pView->SetPicture(lpszPathName);
	}

	SetVisible(TRUE);
}

void CRunDoc::OpenStartPicture() 
{
	OpenPicture(theApp.m_strStartPicture);
}

void CRunDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	// store the path fully qualified
// 	TCHAR szFullPath[_MAX_PATH];
// 	AfxFullPath(szFullPath, lpszPathName);
	m_strPathName = lpszPathName;
	ASSERT(!m_strPathName.IsEmpty());       // must be set to something
	m_bEmbedded = FALSE;
	ASSERT_VALID(this);
	
//	// set the document title based on path name
//	TCHAR szTitle[_MAX_FNAME];
//	if (AfxGetFileTitle(szFullPath, szTitle, _MAX_FNAME) == 0)
//		SetTitle(szTitle);
	
	// add it to the file MRU list
	if (bAddToMRU)
		AfxGetApp()->AddToRecentFileList(m_strPathName);
	
	ASSERT_VALID(this);
}
