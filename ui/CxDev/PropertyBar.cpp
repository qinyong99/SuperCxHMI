// PropertyBar.cpp: implementation of the CPropertyBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "CxPropCtrl.h"
#include "CxPropCtrl_i.c"
#include "ActionObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropertyBar::CPropertyBar()
{
	m_pActiveObject = NULL;
	m_pPropertyCtrl = NULL;

	m_pLastAction = NULL;
}

CPropertyBar::~CPropertyBar()
{
	if (m_pPropertyCtrl)
		m_pPropertyCtrl->Release();
	if (m_pActiveObject)
		m_pActiveObject->Release();

	if (m_pLastAction != NULL)
	{
		delete m_pLastAction;
		m_pLastAction = NULL;
	}
}

BEGIN_MESSAGE_MAP(CPropertyBar, CSizingControlBar)
    //{{AFX_MSG_MAP(CPropertyBar)
    ON_WM_CREATE()
    ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDevApp::m_nSelectChangedMsg, OnSelectChangedMsg)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CPropertyBar, CSizingControlBar)
	ON_EVENT(CPropertyBar, AFX_IDW_PANE_FIRST, 1, OnComboDropDown, VTS_NONE)
	ON_EVENT(CPropertyBar, AFX_IDW_PANE_FIRST, 2, OnComboSelchange, VTS_NONE)
//	ON_EVENT(CPropertyBar, AFX_IDW_PANE_FIRST, 3, OnSelNameChange, VTS_BSTR)
	ON_EVENT(CPropertyBar, AFX_IDW_PANE_FIRST, 3, OnShowPropPages, VTS_NONE)
	ON_EVENT(CPropertyBar, AFX_IDW_PANE_FIRST, 4, OnPropertyChanging, VTS_I4 VTS_PBOOL)
	ON_EVENT(CPropertyBar, AFX_IDW_PANE_FIRST, 5, OnPropertyChanged, VTS_I4)
	ON_EVENT(CPropertyBar, AFX_IDW_PANE_FIRST, 6, OnGetExpressionCreatorProc, VTS_I4)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar message handlers

void CPropertyBar::OnShowPropPages()
{
	CLayoutView* pView = GetActiveLayoutView();
	if (pView != NULL)
	{
		pView->OnEditProperties();
	}
}
////////////////////////////////////////////////////////
//功 能：通知文挡属性变化
//描 述：回退功能可以根据此保存对象
////////////////////////////////////////////////////////
void CPropertyBar::OnPropertyChanging(DISPID dispid, BOOL* pbCancel)
{
	CFormDoc* pDoc = (CFormDoc *)GetDocument();
	ASSERT(pDoc != NULL);
	CLayoutView* pView = pDoc->GetLayoutView();
	ASSERT(pView != NULL);

	*pbCancel = VARIANT_FALSE;

	if (m_pLastAction != NULL)
	{
		delete m_pLastAction;
		m_pLastAction = NULL;
	}

	//没有处理窗口回退
	if (pView->m_selection.IsEmpty())
	{
		if (pView->IsKindOf(RUNTIME_CLASS(CReportView)))
		{
			CReportView* pReportView = (CReportView *)pView;
			CReportDoc* pReportDoc = (CReportDoc *)pDoc;
			
			if (pReportView->m_pSelectSection != NULL)
			{
				if (dispid == CXPROPID_Name)
				{
					m_strObjOldName = pReportView->m_pSelectSection->GetName();
					return;
				}

				CActionSection* pAct = new CActionSection(_T("改变属性"), pDoc, pReportView->m_pSelectSection);
				m_pLastAction = pAct;
			//	pDoc->m_stackUndo.Push(pAct);
				return;
			}
		}
	}

	CDevObj* pObj;

	if (dispid == CXPROPID_Name)
	{
		ASSERT(m_strObjOldName.IsEmpty());
		ASSERT(pView->m_selection.GetCount() <= 1);

		if (pView->m_selection.GetCount() == 1)
		{
			pObj = pView->m_selection.GetHead();
			m_strObjOldName = pObj->GetDisplayName();
		}
		else
		{
			// 禁止改变，避免出现回退错误
			if (pDoc->m_pFormObj != NULL)
			{
				*pbCancel = VARIANT_TRUE;
				return;
			}

			m_strObjOldName = pDoc->GetName();
		}
		
		return;
	}

	if (pView->m_selection.IsEmpty())
	{
		CActionDoc* pAct = new CActionDoc(_T("改变属性"), pDoc);
		m_pLastAction = pAct;
//		pDoc->m_stackUndo.Push(pAct);

		return;
	}

	CActionObjSet* pAct = new CActionObjSet(_T("改变属性"), pDoc);

	POSITION pos = pView->m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = pView->m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}
	pAct->ObjAddComplete();

	// 等待真正变化后再压入堆栈
	m_pLastAction = pAct;
//	pDoc->m_stackUndo.Push(pAct);
}

void CPropertyBar::OnPropertyChanged(DISPID dispid)
{
	CFormDoc* pDoc = (CFormDoc *)GetDocument();
	ASSERT(pDoc != NULL);
	CLayoutView* pView = pDoc->GetLayoutView();
	ASSERT(pView != NULL);

	if (dispid == CXPROPID_Name)
	{
		if (pView->IsKindOf(RUNTIME_CLASS(CReportView)))
		{
			CReportView* pReportView = (CReportView *)pView;
			if (pReportView->m_pSelectSection != NULL)
			{
				if (m_strObjOldName != pReportView->m_pSelectSection->GetName())
				{
					pDoc->SetModifiedFlag();
					
					CActionName* pAct = new CActionName(pDoc, NULL, pReportView->m_pSelectSection, m_strObjOldName);
					pDoc->m_stackUndo.Push(pAct);

					theApp.NotifySelectChanged(TRUE);
				}

				m_strObjOldName.Empty();
				return;
			}
		}

		CDevObj* pObj = NULL;
		if (pView->m_selection.GetCount() == 1)
			pObj = pView->m_selection.GetHead();

		if (pObj != NULL)
		{
			if (pObj->GetDisplayName() == m_strObjOldName)
			{
				m_strObjOldName.Empty();
				return;
			}
		}
		else
		{
			if (m_strObjOldName == pDoc->GetName())
			{
				m_strObjOldName.Empty();
				return;
			}
		}

		pDoc->SetModifiedFlag();

		CActionName* pAct = new CActionName(pDoc, pObj, NULL, m_strObjOldName);
		pDoc->m_stackUndo.Push(pAct);

		theApp.NotifySelectChanged(TRUE);

		m_strObjOldName.Empty();
		return;
	}

	pDoc->SetModifiedFlag();
	
	// 把最后的动作压入回退堆栈
	if (m_pLastAction != NULL)
	{
		pDoc->m_stackUndo.Push(m_pLastAction);
		m_pLastAction = NULL;
	}
	
	if (dispid != CXPROPID_VariableType)
		theApp.NotifySelectChanged(TRUE, DWORD(this));
	else
		theApp.NotifySelectChanged(TRUE, NULL);
}

CLayoutView* CPropertyBar::GetActiveLayoutView()
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

int CPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
        return -1;

	CRect rect(0, 0, 0, 0);
	if (!m_wndPropertyCtrl.CreateControl(CLSID_PropertyCtrl, "",
				WS_VISIBLE | WS_CHILD, rect, this, AFX_IDW_PANE_FIRST))
		 return -1;

	LPUNKNOWN lpUnk = m_wndPropertyCtrl.GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_IPropertyCtrl, (void**) &m_pPropertyCtrl);
	if (!SUCCEEDED(hr))
	{
		m_pPropertyCtrl = NULL;
		m_wndPropertyCtrl.DestroyWindow();
		return -1;
	}

	lpUnk->QueryInterface(IID_IOleInPlaceActiveObject, (void**) &m_pActiveObject);
	
	theApp.m_listSelectNotify.AddTail(m_hWnd);
	
    return 0;
}

//void CPropertyBar::OnDestroy()
//{
//	CPropertyBar::OnDestroy();
//	POSITION pos = theApp.m_listSelectNotify.Find(m_hWnd);
//	ASSERT(pos != NULL);
//	theApp.m_listSelectNotify.RemoveAt(pos);
//}

void CPropertyBar::OnSize(UINT nType, int cx, int cy) 
{
    CSizingControlBar::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);

	if (::IsWindow(m_wndPropertyCtrl.m_hWnd))
	{
	//	rect.DeflateRect(4, 4, 4, 4);
		// need to push non-client borders out of the client area
		::AdjustWindowRectEx(rect,
			m_wndPropertyCtrl.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_wndPropertyCtrl.SetWindowPos(NULL, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

BOOL CPropertyBar::PreTranslateMessage(MSG* pMsg) 
{
	// allow server to translate accelerators
	if (m_pActiveObject != NULL &&
			pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		HRESULT hr = m_pActiveObject->TranslateAccelerator(pMsg);
		if (hr == S_OK)
			return TRUE;
	}

	return CSizingControlBar::PreTranslateMessage(pMsg);	
}

////////////////////////////////////////////////////////////////////////
//功 能：对象选择发生变化
//描 述：具备多选功能
////////////////////////////////////////////////////////////////////////
LONG CPropertyBar::OnSelectChangedMsg(DWORD dwNotify, BOOL bUpdateOnly)
{
	if (dwNotify == (DWORD)this)
		return 0;

	if (bUpdateOnly)
	{
		m_pPropertyCtrl->Refresh();
		return 0;
	}

	USES_CONVERSION;

	CLayoutView* pView = GetActiveLayoutView();
		
	if (pView == NULL || pView->m_bDestoy == TRUE)
		m_pPropertyCtrl->SetSelectedObject(0, NULL, NULL, NULL); 
	else
	{
		IUnknown** ppObjects;
		BSTR bstrName = NULL;
		ICxEnumExtendProperty** ppEnums = NULL;
		CDevObj* pObj = NULL; 
		int nObjectCount;
		BOOL bShowHelpString;

		if (!pView->m_selection.IsEmpty())
		{
			nObjectCount = pView->m_selection.GetCount();
			ppObjects = (IUnknown**)_alloca(sizeof(IUnknown*) * nObjectCount);
			ppEnums = (ICxEnumExtendProperty**)_alloca(sizeof(ICxEnumExtendProperty*) * nObjectCount);
					
			//从后向前遍历
			POSITION pos = pView->m_selection.GetTailPosition();
			int iIndex = 0;
			bShowHelpString = FALSE;
			while (pos != NULL)
			{
				pObj = pView->m_selection.GetPrev(pos);
				ppObjects[iIndex] = pObj->GetObjectUnknown();
				ppEnums[iIndex] = pObj->EnumExtendProperty();
				iIndex++;

				if (!pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
					bShowHelpString = TRUE;
			}

			if (nObjectCount == 1)
				bstrName = A2BSTR(pObj->GetDisplayName() + " " + pObj->GetClassName());
			else
				bstrName = ::SysAllocString(L"");
		}
		//没有选择对象，即作为选择文档
		else
		{
			CString strItemText;

			bShowHelpString = TRUE;
			ppObjects = (IUnknown**)_alloca(sizeof(IUnknown*));
			nObjectCount = 1;

			if (pView->IsKindOf(RUNTIME_CLASS(CReportView)))
			{
				if (((CReportView *)pView)->m_pSelectSection != NULL)
				{
					CReportSection* pSection = ((CReportView *)pView)->m_pSelectSection;
					ppObjects[0] = pSection->GetObjectUnknown();
					strItemText = pSection->GetName();
					strItemText += " Section";
				}
				else
				{
					CFormDoc* pDoc = (CFormDoc *)pView->GetDocument();
					ASSERT(pDoc != NULL);
					
					ppObjects[0] = pDoc->GetObjectUnknown();
					CComBSTR bstr;
					bstr.m_str = pDoc->GetName();
					strItemText = bstr;
					strItemText += " Report";
				}
			}
			else
			{
				CFormDoc* pDoc = (CFormDoc *)pView->GetDocument();
				ASSERT(pDoc != NULL);

				ppObjects[0] = pDoc->GetObjectUnknown();
				CComBSTR bstr;
				bstr.m_str = pDoc->GetName();
				strItemText = bstr;
				strItemText += _T(" ");
				strItemText += pDoc->GetClassName();
			}

			bstrName = strItemText.AllocSysString();
			ppEnums = NULL;
		}

		m_pPropertyCtrl->put_NameShowHelpString(bShowHelpString);

		m_pPropertyCtrl->SetSelectedObject(nObjectCount, ppObjects, bstrName, (IUnknown** )ppEnums); 
		if (bstrName != NULL)
			::SysFreeString(bstrName);
	}

	return 0;
}

void CPropertyBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CSizingControlBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);
}

void CPropertyBar::OnComboDropDown()
{
	CFormDoc* pDoc = (CFormDoc *)GetDocument();
	if (pDoc == NULL)
		return;

	ASSERT_VALID(pDoc);

	CString strObjectText;

	strObjectText = pDoc->GetName();
	strObjectText += _T(" ");
	strObjectText += pDoc->GetClassName();
	m_pPropertyCtrl->AddComboString(A2BSTR(strObjectText));

	if (pDoc->IsKindOf(RUNTIME_CLASS(CReportDoc)))
	{
		CReportDoc* pReportDoc = (CReportDoc *)pDoc;

		strObjectText = pReportDoc->m_secReportHeader.GetName();
		strObjectText += _T(" Section");
		m_pPropertyCtrl->AddComboString(A2BSTR(strObjectText));

		if (pReportDoc->m_pFormObj == NULL)
		{
			strObjectText = pReportDoc->m_secPageHeader.GetName();
			strObjectText += _T(" Section");
			m_pPropertyCtrl->AddComboString(A2BSTR(strObjectText));
		}
		
		strObjectText = pReportDoc->m_secDetail.GetName();
		strObjectText += _T(" Section");
		m_pPropertyCtrl->AddComboString(A2BSTR(strObjectText));

		if (pReportDoc->m_pFormObj == NULL)
		{
			strObjectText = pReportDoc->m_secPageFooter.GetName();
			strObjectText += _T(" Section");
			m_pPropertyCtrl->AddComboString(A2BSTR(strObjectText));
		}
	
		strObjectText = pReportDoc->m_secReportFooter.GetName();
		strObjectText += _T(" Section");
		m_pPropertyCtrl->AddComboString(A2BSTR(strObjectText));
	}

	CDevObjList* pObjList = pDoc->GetObjects();
	POSITION posObj = pObjList->GetHeadPosition();
	while (posObj != NULL)
	{
		CDevObj* pObj = pObjList->GetNext(posObj);

		unsigned long ulLayer = 1;
		ulLayer = ulLayer << pObj->m_nLayer;
		if ((pDoc->m_ulLockLayer & ulLayer) != 0 || (pDoc->m_ulVisibleLayer & ulLayer) == 0)	//被锁定
			continue;

		strObjectText = pObj->GetDisplayName() + " "+ pObj->GetClassName();
		m_pPropertyCtrl->AddComboString(A2BSTR(strObjectText));	
	}
}

void CPropertyBar::OnComboSelchange()
{
	USES_CONVERSION;

	CFormDoc* pDoc = (CFormDoc *)GetDocument();
	if (pDoc == NULL)
		return;
	CLayoutView* pView = GetActiveLayoutView();
	if (pView == NULL)
		return;

	CString strSelectedName;

	BSTR bstrTemp = NULL;
	m_pPropertyCtrl->GetSelectedItemName(&bstrTemp);
	strSelectedName = OLE2T(bstrTemp);

	CDevObj* pObj = NULL;
	pDoc->GetNamedObjectsMap()->Lookup(strSelectedName, pObj);
	if (pObj != NULL)
	{
		pView->Select(pObj);
	}
	else 
	{
		if (pView->IsKindOf(RUNTIME_CLASS(CReportView)))
		{
			CReportView* pReportView = (CReportView *)pView;
			CReportDoc* pReportDoc = (CReportDoc *)pDoc;

			CReportSection* pSelectSection = NULL;
			
			if (strSelectedName == pReportDoc->m_secReportHeader.GetName())
				pSelectSection = &pReportDoc->m_secReportHeader;
					
			if (strSelectedName == pReportDoc->m_secPageHeader.GetName())
				pSelectSection = &pReportDoc->m_secPageHeader;
			
			if (strSelectedName == pReportDoc->m_secDetail.GetName())
				pSelectSection = &pReportDoc->m_secDetail;
			
			if (strSelectedName == pReportDoc->m_secPageFooter.GetName())
				pSelectSection = &pReportDoc->m_secPageFooter;
			
			if (strSelectedName == pReportDoc->m_secReportFooter.GetName())
				pSelectSection = &pReportDoc->m_secReportFooter;

			pReportView->SelectSection(pSelectSection);
		}
		else
		{
			pView->Select(NULL);
		}
	}

	pView->SetFocus();
}

void CPropertyBar::OnGetExpressionCreatorProc(long lProc)
{
	CFormDoc::m_pExpCrtDoc = (CFormDoc *)GetDocument();
	*(ExpressionCreatorProc *)lProc = CFormDoc::ExpressionCreatorProc;
}