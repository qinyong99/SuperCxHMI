// ActivDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"

CActiveXDocTemplate::CActiveXDocTemplate(CRuntimeClass* pDocClass,
    CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
        : CSingleDocTemplate(IDR_NOTUSED, pDocClass, pFrameClass,
            pViewClass)
{
    ASSERT(pFrameClass);
}

CFrameWnd* CActiveXDocTemplate::CreateDocViewFrame(CWnd* pParentWnd)
{
    ASSERT(pParentWnd && IsWindow(*pParentWnd));
    ASSERT_KINDOF(CActiveXDocControl, pParentWnd);
    m_pParentWnd = pParentWnd;
    m_pFrameWnd = NULL;

    // OpenDocumentFile is a virtual method (implemented in
    // the CSingleDocTemplate base class) that creates
    // the document (either empty or loaded from the specified
    // file) and calls our CreateNewFrame function. Passing
    // NULL to the function creates a new document. Incidentally,
    // the view is created by the CFrameWnd's OnCreateClient()
    // method.

    if (!OpenDocumentFile(NULL))
        return NULL;

    // Since OpenDocumentFile sets m_pFrame, we can now use it.

    ASSERT(m_pFrameWnd);
    ASSERT_KINDOF(CFrameWnd, m_pFrameWnd);
    m_pFrameWnd->ShowWindow(SW_SHOWNORMAL);
    return m_pFrameWnd;
}

CFrameWnd* CActiveXDocTemplate::CreateNewFrame(CDocument* pDoc,
        CFrameWnd* pOther)
{
    ASSERT(pOther == NULL);
    ASSERT(m_pFrameClass != NULL);
    if (pDoc != NULL)
        ASSERT_VALID(pDoc);

    // Create a frame wired to the specified document

    CCreateContext context;
    context.m_pCurrentFrame = pOther;
    context.m_pCurrentDoc = pDoc;
    context.m_pNewViewClass = m_pViewClass;
    context.m_pNewDocTemplate = this;

    m_pFrameWnd = (CFrameWnd*)m_pFrameClass->CreateObject();
    if (m_pFrameWnd == NULL)
    {
        TRACE1("Warning: Dynamic create of frame %hs failed.\n",
            m_pFrameClass->m_lpszClassName);
        return NULL;
    }
    ASSERT_KINDOF(CFrameWnd, m_pFrameWnd);

    if (context.m_pNewViewClass == NULL)
        TRACE0("Warning: creating frame with no default view.\n");

    // The frame is created as a menu-less child of the
    // CActiveXDocControl in which it will reside.

    ASSERT_KINDOF(CActiveXDocControl, m_pParentWnd);
    if (!m_pFrameWnd->Create(NULL, "", WS_CHILD|WS_VISIBLE,
        CFrameWnd::rectDefault, m_pParentWnd, NULL, 0, &context))
    {
        TRACE0("Warning: CDocTemplate couldn't create a frame.\n");
        return NULL;
    }

    return m_pFrameWnd;
}

CDocument* CActiveXDocTemplate::OpenDocumentFile(
    LPCTSTR lpszPathName, BOOL bVerifyExists)
{
    SaveDocumentFile();
    m_docFile = lpszPathName;

    if (bVerifyExists)
    {
        DWORD dwAttrib = GetFileAttributes(m_docFile);
        if (dwAttrib == 0xFFFFFFFF ||
            dwAttrib == FILE_ATTRIBUTE_DIRECTORY)
        {
            lpszPathName = NULL;
        }
    }

    return CSingleDocTemplate::OpenDocumentFile(
        lpszPathName, TRUE);
}

void CActiveXDocTemplate::SaveDocumentFile()
{
    if (m_pOnlyDoc != NULL)
    {
        if (!m_docFile.IsEmpty())
            m_pOnlyDoc->OnSaveDocument(m_docFile);
        else
            m_pOnlyDoc->SetModifiedFlag(FALSE);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CActiveXDocControl

IMPLEMENT_DYNAMIC(CActiveXDocControl, COleControl)
BEGIN_MESSAGE_MAP(CActiveXDocControl, COleControl)
    //{{AFX_MSG_MAP(CActiveXDocControl)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
    ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()
BEGIN_DISPATCH_MAP(CActiveXDocControl, COleControl)
    //{{AFX_DISPATCH_MAP(CActiveXDocControl)
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()
BEGIN_EVENT_MAP(CActiveXDocControl, COleControl)
    //{{AFX_EVENT_MAP(COleFrameCtrl)
    //}}AFX_EVENT_MAP
END_EVENT_MAP()

int CActiveXDocControl::m_bDocInitialized = FALSE;

CActiveXDocControl::CActiveXDocControl()
{
    m_pDocTemplate = NULL;
    m_pFrameWnd = NULL;

    // Since we're in an OCX, CWinApp::InitApplication() is
    // not called by the framework. Unfortunately, that method
    // performs CDocManager initialization that is necessary
    // in order for our DocTemplate to clean up after itself.
    // We simulate the same initialization by calling the
    // following code the first time we create a control.

    if (!m_bDocInitialized)
    {
        CDocManager docManager;
        docManager.AddDocTemplate(NULL);
        m_bDocInitialized = TRUE;
    }
}

CActiveXDocControl::~CActiveXDocControl()
{
    // Note that the frame, the document, and the view are
    // all deleted automatically by the framework!
    
    delete m_pDocTemplate;
}

void CActiveXDocControl::AddDocTemplate(CActiveXDocTemplate* pDocTemplate)
{
    // I've decided to call this function AddDocTemplate to
    // be consistent with naming of CWinApp::AddDocTemplate.
    // However, only one DocTemplate is allowed per control.
    
    ASSERT(pDocTemplate);
    ASSERT(m_pDocTemplate == NULL);
    m_pDocTemplate = pDocTemplate;
}

int CActiveXDocControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (COleControl::OnCreate(lpCreateStruct) == -1)
        return -1;

    //// The CActiveXDocTemplate object will create the
    //    // document, the view, and the frame inside the
    //    // control. The reason we need a handle to the frame
    //    // is so that we can resize it when the control is
    //    // resized.
    //    
    //    ASSERT(m_pDocTemplate);    // Set in call to AddDocTemplate
    //    m_pFrameWnd = m_pDocTemplate->CreateDocViewFrame(this);
    //    ASSERT_KINDOF(CFrameWnd, m_pFrameWnd);
    //
    //    // By default, we'll create the control with a border,
    //    // since it looks better for frames containing a toolbar.
    //
     //   SetBorderStyle(TRUE);
    //    SetTimer(WM_IDLEUPDATECMDUI, 300, NULL);
    
    return 0;
}

CFrameWnd* CActiveXDocControl::CreateFrame(CDocument* pDoc)
{
// 	ASSERT(m_pFrameWnd == NULL);
//	SetBorderStyle(TRUE);

	m_pDocTemplate->m_pParentWnd = this;
	m_pFrameWnd = m_pDocTemplate->CreateNewFrame(pDoc, NULL);
	ASSERT_KINDOF(CFrameWnd, m_pFrameWnd);

	CRect rect;
	GetClientRect(rect);
	m_pFrameWnd->MoveWindow(rect.left, rect.top, rect.right,
		rect.bottom);

//	SetBorderStyle(TRUE);

	return m_pFrameWnd;
}

void CActiveXDocControl::OnSize(UINT nType, int cx, int cy) 
{
    COleControl::OnSize(nType, cx, cy);

    // The CFrameWnd should always fill up the entire client
    // area of the control.

    if (m_pFrameWnd != NULL)
    {
        ASSERT(IsWindow(*m_pFrameWnd));
        CRect rect;
		//AdjustWindowRectEx(&rect, WS_BORDER, FALSE, 0);
        GetClientRect(rect);
	//	rect.InflateRect(-2, -2);
	//	m_pFrameWnd->SetWindowPos(NULL, rect.left, rect.top, rect.Width(),
	//			rect.Height(), SWP_NOZORDER);
        m_pFrameWnd->MoveWindow(rect.left, rect.top, rect.right,
			rect.bottom);
    }
}

//void CActiveXDocControl::OnTimer(UINT nIDEvent) 
//{
//    // Since we're in an OCX, we don't control the message loop,
//    // so CWinThread::OnIdle is never called. That means we have
//    // to periodically pump the ON_UPDATE_COMMAND_UI messages
//    // by hand.
//    
//    SendMessageToDescendants(WM_IDLEUPDATECMDUI, TRUE);
//    COleControl::OnTimer(nIDEvent);



void CActiveXDocControl::OnDestroy() 
{
	AfxGetApp()->m_pMainWnd = NULL;
//	m_pDocTemplate->SaveDocumentFile();
	COleControl::OnDestroy();
}
