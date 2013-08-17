// ExpertBar.cpp: implementation of the CExpertBar class.
//功能:专家工具栏窗口对象
//制作:孙越洋
//时间:2001.12.26
//所属工程:CxDev
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "..\CxSCBar\SCBEXT.h"
#include "ActionObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TOOLHIGHT	21
#define BUTTON_WIDTH	16

BEGIN_MESSAGE_MAP(CExpertBar, CSizingControlBar)
    //{{AFX_MSG_MAP(CExplorerBar)
    ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCLBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDevApp::m_nSelectChangedMsg, OnSelectChangedMsg)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CExpertBar::CExpertBar()
{
	m_bShowPage = FALSE;
}

CExpertBar::~CExpertBar()
{
}

/////////////////////////////////////////////////////////
//功能:创建Window窗口句柄
/////////////////////////////////////////////////////////
BOOL CExpertBar::Create(const CLSID& clsidPage, LPCTSTR lpszTitle, UINT nID, CWnd* pParentWnd)
{
	if (!CSizingControlBar::Create(lpszTitle, pParentWnd, m_sizePage,
            TRUE, nID))
    {
		TRACE0("Failed to create expert\n");
        return FALSE; 
    }

	SetBarStyle(GetBarStyle() | SCBS_SHOWEDGES | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOATING);

	ASSERT(m_pPage == NULL);
	HRESULT hr = m_pPage.CreateInstance(clsidPage);
	if (FAILED(hr))
		return FALSE;
	
	m_pPage->SetPageSite(&m_xPageSite);

	PROPPAGEINFO ppi;
	m_pPage->GetPageInfo(&ppi);
	m_sizePage = ppi.size;
	m_sizePage.cx += 10;
	m_sizePage.cy += 10;

	m_szHorz = m_sizePage; // set the size members
    m_szVert = m_sizePage;
    m_szFloat = m_sizePage;

	return TRUE;
}

int CExpertBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
        return -1;

	theApp.m_listSelectNotify.AddTail(m_hWnd);

	return TRUE;
}

BOOL CExpertBar::PreTranslateMessage(MSG* pMsg)
{
	// 处理快捷键
	if (m_pPage != NULL)
	{
		if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
		{
			if (m_pPage->TranslateAccelerator(pMsg) == S_OK)
				return TRUE;
		}
	}
	
	return CSizingControlBar::PreTranslateMessage(pMsg);
}

BEGIN_INTERFACE_MAP(CExpertBar, CSizingControlBar)
	INTERFACE_PART(CExpertBar, IID_IPropertyPageSite, PageSite)
	INTERFACE_PART(CExpertBar, IID_ICxExpertPageSite, PageSite)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////
//pageSite接口函数
STDMETHODIMP_(ULONG) CExpertBar::XPageSite::AddRef()
{
	METHOD_PROLOGUE(CExpertBar, PageSite)

    return pThis->ExternalAddRef();
}

STDMETHODIMP CExpertBar::XPageSite::QueryInterface(REFIID iid, void** ppvObj)
{
	METHOD_PROLOGUE(CExpertBar, PageSite)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(ULONG) CExpertBar::XPageSite::Release()
{
	METHOD_PROLOGUE(CExpertBar, PageSite)

	return pThis->ExternalRelease();
}

STDMETHODIMP CExpertBar::XPageSite::OnStatusChange(DWORD dwFlags)
{
	METHOD_PROLOGUE(CExpertBar, PageSite)

	switch(dwFlags)
	{
	case PROPPAGESTATUS_DIRTY:
		pThis->OnPropertyChanged();
		break;
	case PROPPAGESTATUS_VALIDATE:
		pThis->OnPropertyChanging();
		break;
	default:
		break;
	}

//	CDocument* pActiveDoc = pThis->GetDocument();
//	if (pActiveDoc)
//	{
//		pActiveDoc->SetModifiedFlag();
//		if (pActiveDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
//			((CFormDoc *)pActiveDoc)->m_stackUndo.Reset();
//	}

	return S_OK;
}

STDMETHODIMP CExpertBar::XPageSite::GetLocaleID(LCID* pLocaleID)
{
	if(pLocaleID == NULL)
		return E_POINTER;
	*pLocaleID = LOCALE_SYSTEM_DEFAULT;
	return S_OK;
}

STDMETHODIMP CExpertBar::XPageSite::GetPageContainer(IUnknown** ppUnk)
{
	return E_NOTIMPL;
}

STDMETHODIMP CExpertBar::XPageSite::TranslateAccelerator(LPMSG pMsg)
{
	return S_OK;
}

STDMETHODIMP CExpertBar::XPageSite::OnGetDataSource(BSTR* pbstrSource)
{
	USES_CONVERSION;

	METHOD_PROLOGUE(CExpertBar, PageSite)

	CFormDoc::m_pExpCrtDoc = (CFormDoc *)pThis->GetDocument();
	CFormDoc::ExpressionCreatorProc(pbstrSource);
	
	return S_OK;
}

STDMETHODIMP CExpertBar::XPageSite::OnNextEventObject()
{
	METHOD_PROLOGUE(CExpertBar, PageSite)

	pThis->OnNextEventObject();

	return S_OK;
}

STDMETHODIMP CExpertBar::XPageSite::OnEditEventHandler(BSTR bstrEvent)
{
	USES_CONVERSION;
	
	METHOD_PROLOGUE(CExpertBar, PageSite)

	pThis->OnEditEventHandler(OLE2T(bstrEvent));

	return S_OK;
}

STDMETHODIMP CExpertBar::XPageSite::GetEventHandler(BSTR bstrEvent, BSTR *pbstrHandler)
{
	USES_CONVERSION;

	METHOD_PROLOGUE(CExpertBar, PageSite)

	CString strSubBody;
	BOOL b = pThis->GetEventHandler(OLE2T(bstrEvent), strSubBody);
	if (b && pbstrHandler != NULL)
	{
		if (*pbstrHandler != NULL)
			SysFreeString(*pbstrHandler);
		*pbstrHandler = strSubBody.AllocSysString();
	}
		
	return (b ? S_OK : E_FAIL);
}

STDMETHODIMP CExpertBar::XPageSite::InsertEventHandler(BSTR bstrEvent, BSTR bstrParams, BSTR bstrHandler)
{
	USES_CONVERSION;
	
	METHOD_PROLOGUE(CExpertBar, PageSite)

	BOOL b = pThis->InsertEventHandler(OLE2T(bstrEvent), OLE2T(bstrParams), OLE2T(bstrHandler));
		
	return (b ? S_OK : E_FAIL);
}

STDMETHODIMP CExpertBar::XPageSite::OnNextDynamicObject()
{
	METHOD_PROLOGUE(CExpertBar, PageSite)
		
 	pThis->OnNextDynamicObject();
	
	return S_OK;
}

STDMETHODIMP CExpertBar::XPageSite::HaveDynamic()
{
	METHOD_PROLOGUE(CExpertBar, PageSite)
		
	return pThis->HaveDynamic() ? S_OK : S_FALSE;
}

////////////////////////////////////////////////////////////////////////
//功 能：对象选择发生变化
//描 述：具备多选功能
////////////////////////////////////////////////////////////////////////
LONG CExpertBar::OnSelectChangedMsg(DWORD dwNotify, LONG)
{
	if (dwNotify == DWORD(this))
		return 0;

	if (!m_bShowPage)
	{
		CRect rect;
		rect.left = rect.top = 0;
		rect.right = m_sizePage.cx;
		rect.bottom = m_sizePage.cy;
		m_pPage->Activate(m_hWnd, &rect, TRUE);
		// 		m_pPage->Move(rect);
		m_pPage->Show(SW_SHOW);
		m_bShowPage = TRUE;
	}

	USES_CONVERSION;
	CLayoutView* pView = GetActiveLayoutView();
	IUnknown** ppObjects = NULL;
	ICxExpertPagePtr pExpertPage = m_pPage;
	ICxEnumExtendProperty** ppEnums = NULL;
	BOOL bShowHelpString;
		
	if (!IsWindowVisible() || pView == NULL || pView->m_bDestoy == TRUE)
	{
		if (pExpertPage != NULL)
			pExpertPage->SetExtendObjects(0, ppObjects, ppEnums);
		else
			m_pPage->SetObjects(0, ppObjects); 
		OnUpdateCaption(NULL);
	}
	else
	{
		int nObjectCount;
		bShowHelpString = TRUE;
		if (!pView->m_selection.IsEmpty())
		{
			if (pExpertPage != NULL && pExpertPage->IsSupportMultiObject() != S_OK)
				nObjectCount = 1;
			else
				nObjectCount = pView->m_selection.GetCount();
			ppObjects = (IUnknown**)_alloca(sizeof(IUnknown*) * nObjectCount);
			if (pExpertPage != NULL)
				ppEnums = (ICxEnumExtendProperty**)_alloca(sizeof(ICxEnumExtendProperty*) * nObjectCount);
					
			
			CDevObj* pObj;
			if (nObjectCount == 1)
			{
				pObj = pView->m_selection.GetHead();
				ppObjects[0] = pObj->GetObjectUnknown();
				if (pExpertPage != NULL)
					ppEnums[0] = pObj->EnumExtendProperty();
							
				if (pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
					bShowHelpString = FALSE;
			}
			else
			{
				// 从后向前遍历
				POSITION pos = pView->m_selection.GetTailPosition();
				int iIndex = 0;
				while (pos != NULL)
				{
					pObj = pView->m_selection.GetPrev(pos);
					ppObjects[iIndex] = pObj->GetObjectUnknown();
					if (pExpertPage != NULL)
						ppEnums[iIndex] = pObj->EnumExtendProperty();
					iIndex++;
					
					if (pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
						bShowHelpString = FALSE;
				}
			}
	
			OnUpdateCaption(pView->m_selection.GetTail()->GetDisplayName());
		}
		//没有选择对象，即作为选择文档
		else
		{
			CFormDoc* pDoc = (CFormDoc *)GetDocument();
			ASSERT(pDoc != NULL);

			bShowHelpString = TRUE;
			ppObjects = (IUnknown**)_alloca(sizeof(IUnknown*));
// 			ppObjects[0] = pDoc->GetInterface(&IID_IUnknown);

			if (pView->IsKindOf(RUNTIME_CLASS(CReportView)))
			{
				if (((CReportView *)pView)->m_pSelectSection != NULL)
				{
					CReportSection* pSection = ((CReportView *)pView)->m_pSelectSection;
					ppObjects[0] = pSection->GetObjectUnknown();
				}
				else
				{
					ppObjects[0] = pDoc->GetObjectUnknown();
				}
			}
			else
			{
				ppObjects[0] = pDoc->GetObjectUnknown();
			}

			if (pExpertPage != NULL)
			{
				ppEnums = (ICxEnumExtendProperty**)_alloca(sizeof(ICxEnumExtendProperty*));
				ppEnums[0] = NULL;
			}
			
			nObjectCount = 1;

			OnUpdateCaption(pDoc->GetDisplayName());
		}

		if (pExpertPage != NULL)
		{
			pExpertPage->ShowHelpString(bShowHelpString);
			pExpertPage->SetExtendObjects(nObjectCount, ppObjects, ppEnums);
		}
		else
		{
			m_pPage->SetObjects(nObjectCount, ppObjects);
		}
	}

	return 0;
}

CLayoutView* CExpertBar::GetActiveLayoutView()
{
	CDocument* pDoc = GetDocument();
	if (pDoc == NULL)
		return NULL;

	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CLayoutView)))
			return (CLayoutView *)pView;
	}
	return NULL;
}

void CExpertBar::OnDestroy() 
{
	CSizingControlBar::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CExpertBar::OnNextEventObject()
{
	CLayoutView* pView = GetActiveLayoutView();
	if (pView == NULL)
		return;

	CFormDoc* pDoc = pView->GetDocument();

	// 空代表目前没有选中对象
	CDevObj* pCurSelObj = NULL;
	if (!pView->m_selection.IsEmpty())
		pCurSelObj = pView->m_selection.GetTail();
	
	CDevObjList* pObjList = pDoc->GetObjects(); 
	POSITION pos;
	if (pCurSelObj != NULL)
	{
		pos = pObjList->Find(pCurSelObj);
		pObjList->GetNext(pos);
	}
	else
	{
		pos = pObjList->GetHeadPosition();
	}

	while (pos != NULL)
	{
		CDevObj* pObj = pObjList->GetNext(pos);
		if (pDoc->HasObjectEventHandler(pObj))
		{
			pView->Select(pObj);
			pView->EnsureObjectVisible(pObj);
			AdjustPosition();
			return;
		}
	}

	if (pCurSelObj == NULL)
		return;

	if (pDoc->HasObjectEventHandler(NULL))
	{
		pView->Select(NULL);
		return;
	}

	pos = pObjList->GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = pObjList->GetNext(pos);
		if (pObj == pCurSelObj)
			return;
		if (pDoc->HasObjectEventHandler(pObj))
		{
			pView->Select(pObj);
			pView->EnsureObjectVisible(pObj);
			AdjustPosition();
			return;
		}
	}
}

void CExpertBar::OnNextDynamicObject()
{
	CLayoutView* pView = GetActiveLayoutView();
	if (pView == NULL)
		return;
	
	CFormDoc* pDoc = pView->GetDocument();
	
	// 空代表目前没有选中对象
	CDevObj* pCurSelObj = NULL;
	if (!pView->m_selection.IsEmpty())
		pCurSelObj = pView->m_selection.GetTail();
	
	CDevObjList* pObjList = pDoc->GetObjects(); 
	POSITION pos;
	if (pCurSelObj != NULL)
	{
		pos = pObjList->Find(pCurSelObj);
		pObjList->GetNext(pos);
	}
	else
	{
		pos = pObjList->GetHeadPosition();
	}
	
	while (pos != NULL)
	{
		CDevObj* pObj = pObjList->GetNext(pos);
		if (pObj->HaveDynamic())
		{
			pView->Select(pObj);
			pView->EnsureObjectVisible(pObj);
			AdjustPosition();
			return;
		}
	}
	
	if (pCurSelObj == NULL)
		return;
	
	if (pDoc->HaveDynamic())
	{
		pView->Select(NULL);
		return;
	}
	
	pos = pObjList->GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = pObjList->GetNext(pos);
		if (pObj == pCurSelObj)
			return;
		if (pObj->HaveDynamic())
		{
			pView->Select(pObj);
			pView->EnsureObjectVisible(pObj);
			AdjustPosition();
			return;
		}
	}
}

void CExpertBar::OnEditEventHandler(LPCTSTR lpszEvent)
{
	CLayoutView* pView = GetActiveLayoutView();
	if (pView == NULL)
		return;
	
	CFormDoc* pDoc = pView->GetDocument();
	
	// 空代表目前没有选中对象
	CDevObj* pCurSelObj = NULL;
	if (!pView->m_selection.IsEmpty())
		pCurSelObj = pView->m_selection.GetTail();

	CMainFrame* pParentFrame = (CMainFrame *)AfxGetMainWnd();
	pParentFrame->ViewCode(pCurSelObj, pView, lpszEvent);
}

void CExpertBar::AdjustPosition()
{
	CLayoutView* pView = GetActiveLayoutView();

	if (pView != NULL && !pView->m_selection.IsEmpty())
	{
		CDevObj* pObj = pView->m_selection.GetTail();

		CPoint point;
		point.x = (int)pObj->GetPositionRect().left;
		point.y = (int)pObj->GetPositionRect().top;
		pView->DocToClient(point);
		pView->ClientToScreen(&point);

		CRect rect;
		GetWindowRect(&rect);

		if (rect.PtInRect(point))
		{
			if (point.y > rect.Height())
				rect.OffsetRect(0, point.y - rect.bottom - 20);
			else
			{
				int nVertExt = GetSystemMetrics(SM_CYSCREEN);
				if (point.y + rect.Height() < nVertExt)
					rect.OffsetRect(0, 40 + point.y - rect.top);
			}
			((CMainFrame *)AfxGetMainWnd())->FloatControlBar(this, rect.TopLeft());
		}
	}	
}

BOOL CExpertBar::GetEventHandler(LPCTSTR lpszEvent, CString& strSubBody)
{
	CLayoutView* pView = GetActiveLayoutView();
	if (pView == NULL)
		return FALSE;
	
	CFormDoc* pDoc = pView->GetDocument();
	CDevObj* pCurSelObj = NULL;
	if (!pView->m_selection.IsEmpty())
		pCurSelObj = pView->m_selection.GetTail();

	return pDoc->GetObjectEventHandler(pCurSelObj, lpszEvent, strSubBody);
}

BOOL CExpertBar::HaveDynamic()
{
	CLayoutView* pView = GetActiveLayoutView();
	if (pView == NULL)
		return FALSE;
	
	if (!pView->m_selection.IsEmpty())
	{
		CDevObj* pCurSelObj = pView->m_selection.GetTail();
		return pCurSelObj->HaveDynamic();
	}

	CFormDoc* pDoc = pView->GetDocument();
	return pDoc->HaveDynamic();
}

BOOL CExpertBar::InsertEventHandler(LPCTSTR lpszEvent, LPCTSTR lpszParams, LPCTSTR lpszSubBody)
{
	CLayoutView* pView = GetActiveLayoutView();
	if (pView == NULL)
		return FALSE;
	
	CFormDoc* pDoc = pView->GetDocument();
	
	// 空代表目前没有选中对象
	CDevObj* pCurSelObj = NULL;
	if (!pView->m_selection.IsEmpty())
		pCurSelObj = pView->m_selection.GetTail();

	if (lpszSubBody == NULL && lpszParams == NULL)
		return pDoc->DeleteObjectEventHandler(pCurSelObj, lpszEvent);
	else
		return pDoc->InsertOrReplaceObjectEventHandler(pCurSelObj, lpszEvent, lpszParams, lpszSubBody);
}

BOOL CExpertBar::OnUpdateCaption(LPCTSTR lpszObject)
{
	if (m_pDockSite != NULL && m_pDockBar != NULL)
	{
		CMiniDockFrameWnd* pDockFrame =
			(CMiniDockFrameWnd*)m_pDockBar->GetParent();

		TCHAR szTitle[_MAX_PATH];
		GetWindowText(szTitle, _MAX_PATH);

		CString strCaption = szTitle;
		if (lpszObject != NULL)
		{
			strCaption += _T("（");
			strCaption += lpszObject;
			strCaption += _T("）");	
		}
				
	//	SetWindowText(strCaption);
		pDockFrame->m_wndDockBar.SetWindowText(strCaption);
		pDockFrame->SetWindowText(strCaption);
	}

	return TRUE;
}

void CExpertBar::OnPropertyChanged()
{
	CDocument* pActiveDoc = GetDocument();
	if (pActiveDoc)
		pActiveDoc->SetModifiedFlag();
	theApp.NotifySelectChanged(FALSE, 0, TRUE);
}

void CExpertBar::OnPropertyChanging()
{
	CFormDoc* pDoc = (CFormDoc *)GetDocument();
	ASSERT(pDoc != NULL);
	CLayoutView* pView = pDoc->GetLayoutView();
	ASSERT(pView != NULL);
	
	if (pView->m_selection.IsEmpty())
	{
		if (pView->IsKindOf(RUNTIME_CLASS(CReportView)))
		{
			CReportView* pReportView = (CReportView *)pView;
			CReportDoc* pReportDoc = (CReportDoc *)pDoc;
			
			if (pReportView->m_pSelectSection != NULL)
			{
				CActionSection* pAct = new CActionSection(_T("改变属性"), pDoc, pReportView->m_pSelectSection);
				pDoc->m_stackUndo.Push(pAct);
			}
		}
		else
		{
			CActionDoc* pAct = new CActionDoc(_T("改变属性"), pDoc);
			pDoc->m_stackUndo.Push(pAct);
		}

		return;
	}
	
	CActionObjSet* pAct = new CActionObjSet(_T("改变属性"), pView->GetDocument());
	
	POSITION pos = pView->m_selection.GetHeadPosition();
	CDevObj* pObj;
	while (pos != NULL)
	{
		pObj = pView->m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}
	pAct->ObjAddComplete();
	
	pDoc->m_stackUndo.Push(pAct);
}
