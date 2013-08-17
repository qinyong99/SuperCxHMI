// DynamicExpert.cpp : Implementation of CDynamicExpert
#include "stdafx.h"
#include "CxDynExp.h"
#include "DynamicExpert.h"
#include "DialogSizeHelper.h"
#include "MovementDlg.h"
#include "ScaleDlg.h"
#include "FillDlg.h"
#include "ColorDlg.h"
#include "VisibleDlg.h"
#include "AdvancedDlg.h"
#include "ChineseVarUtils.h"

#define CMovementPage CTabPageImpl<CMovementDlg>
#define CScalePage CTabPageImpl<CScaleDlg> 
#define CFillPage CTabPageImpl<CFillDlg> 
#define CColorPage CTabPageImpl<CColorDlg> 
#define CVisiblePage CTabPageImpl<CVisibleDlg>  
#define CAdvancedPage CComObject<CTabPageImpl<CAdvancedDlg> > 


HRESULT HelperSetText(HWND hDlg, int nIDDlgItem, CComVariant &var)
{
	USES_CONVERSION;
	
	if (var.vt != VT_BSTR)
		var.ChangeType(VT_BSTR);
	
	if (var.vt != VT_BSTR)
		return S_FALSE;
	
	SetDlgItemText(hDlg, nIDDlgItem, W2T(var.bstrVal));
	
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CDynamicExpert

CDynamicExpert* CDynamicExpert::m_pExpCrtExp = NULL;

LRESULT CDynamicExpert::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_treeModes.SubclassWindow(GetDlgItem(IDC_MODES));
	m_treeModes.ModifyStyle(0, WS_HSCROLL, SWP_NOREDRAW | SWP_NOSENDCHANGING);
// 	m_treeModes.ModifyStyle(WS_HSCROLL, 0);

	for (int i = 0; i < NUM_PAGES; i++)
		m_apPages[i] = NULL;

	return 1;
}

void CDynamicExpert::OnFinalMessage(HWND hWnd)
{
	for (int i = 0; i < NUM_PAGES; i++)
	{
		if (m_apPages[i] != NULL)
		{
			delete m_apPages[i];
			m_apPages[i] = NULL;
		}
	}
}

void CDynamicExpert::PopulateTree()
{
	USES_CONVERSION;
	HTREEITEM hRootItem;
	HTREEITEM hItem;
	int nConectState;
	BOOL bCheckRoot;
	int nPropIndex;

 	m_treeModes.SetRedraw(FALSE);
	m_treeModes.DeleteAllItems();

	hRootItem = NULL;
	bCheckRoot = FALSE;
	if ((nPropIndex = FindProperty(CXPROPID_HorPosition)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_HorPosition);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("运动"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("水平"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);

			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if ((nPropIndex = FindProperty(CXPROPID_VerPosition)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_VerPosition);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("运动"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("垂直"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if ((nPropIndex = FindProperty(CXPROPID_RotateAngle)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_RotateAngle);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("运动"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("旋转"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if (hRootItem != NULL)
	{
		m_treeModes.Expand(hRootItem);
		if (bCheckRoot)
			m_treeModes.SetCheckState(hRootItem, TRUE);
	}

	hRootItem = NULL;
	bCheckRoot = FALSE;
	if ((nPropIndex = FindProperty(CXPROPID_HorScale)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_HorScale);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("缩放"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("水平"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if ((nPropIndex = FindProperty(CXPROPID_VerScale)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_VerScale);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("缩放"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("垂直"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if (hRootItem != NULL)
	{
		m_treeModes.Expand(hRootItem);
		if (bCheckRoot)
			m_treeModes.SetCheckState(hRootItem, TRUE);
	}
	
	hRootItem = NULL;
	bCheckRoot = FALSE;
	if ((nPropIndex = FindProperty(CXPROPID_HorFillPercent)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_HorFillPercent);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("填充"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("水平"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if ((nPropIndex = FindProperty(CXPROPID_VerFillPercent)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_VerFillPercent);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("填充"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("垂直"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if (hRootItem != NULL)
	{
		m_treeModes.Expand(hRootItem);
		if (bCheckRoot)
			m_treeModes.SetCheckState(hRootItem, TRUE);
	}

	hRootItem = NULL;
	bCheckRoot = FALSE;
	for (int i = 0; i < m_arrProps.GetSize(); i++)
	{
		CPropInfo& prop = m_arrProps[i];
		if (prop.m_proptype == PROPTYPE_COLOR)
		{
			nConectState = GetConnectState(prop.m_dispid);
			if (nConectState != CSTATE_DISABLE)
			{
				if (hRootItem == NULL)
					hRootItem = m_treeModes.InsertItem(_T("颜色"), TVI_ROOT, TVI_LAST);
				
				hItem = m_treeModes.InsertItem(m_bShowHelpString ? OLE2T(prop.m_bstrDesc) : OLE2T(prop.m_bstrName), 
					hRootItem, TVI_LAST);
				m_treeModes.SetItemData(hItem, i);
				
				if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
				{
					m_treeModes.SetCheckState(hItem, TRUE);
					bCheckRoot = TRUE;
				}
			}
		}
	}
	if (hRootItem != NULL)
	{
		m_treeModes.Expand(hRootItem);
		if (bCheckRoot)
			m_treeModes.SetCheckState(hRootItem, TRUE);
	}
	
	hRootItem = NULL;
	bCheckRoot = FALSE;
	if ((nPropIndex = FindProperty(CXPROPID_Visible)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_Visible);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("可见"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("可见"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if (hRootItem != NULL)
	{
		m_treeModes.Expand(hRootItem);
		if (bCheckRoot)
			m_treeModes.SetCheckState(hRootItem, TRUE);
	}

	hRootItem = NULL;
	bCheckRoot = FALSE;
	if ((nPropIndex = FindProperty(CXPROPID_Value)) != -1)
	{
		nConectState = GetConnectState(CXPROPID_Value);
		if (nConectState != CSTATE_DISABLE)
		{
			if (hRootItem == NULL)
				hRootItem = m_treeModes.InsertItem(_T("值"), TVI_ROOT, TVI_LAST);
			hItem = m_treeModes.InsertItem(_T("值"), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, nPropIndex);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}
	if (hRootItem != NULL)
	{
		m_treeModes.Expand(hRootItem);
		if (bCheckRoot)
			m_treeModes.SetCheckState(hRootItem, TRUE);
	}

	hRootItem = NULL;
	bCheckRoot = FALSE;
	if (!m_strExtendCat.IsEmpty())
	{
		for (i = m_nExtendPropFirst; i < m_arrProps.GetSize(); i++)
		{
			CPropInfo& prop = m_arrProps[i];
			nConectState = GetConnectState(prop.m_dispid);
			if (nConectState != CSTATE_DISABLE)
			{
				if (hRootItem == NULL)
					hRootItem = m_treeModes.InsertItem(m_strExtendCat, TVI_ROOT, TVI_LAST);
				
				hItem = m_treeModes.InsertItem(m_bShowHelpString ? OLE2T(prop.m_bstrDesc) : OLE2T(prop.m_bstrName), 
					hRootItem, TVI_LAST);
				m_treeModes.SetItemData(hItem, i);
				
				if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
				{
					m_treeModes.SetCheckState(hItem, TRUE);
					bCheckRoot = TRUE;
				}
			}
		}
	}

	if (hRootItem != NULL)
	{
		m_treeModes.Expand(hRootItem);
		if (bCheckRoot)
			m_treeModes.SetCheckState(hRootItem, TRUE);
	}

	if (m_arrProps.GetSize() > 0)
	{
		hRootItem = m_treeModes.InsertItem(_T("高级"), TVI_ROOT, TVI_LAST);
		if (m_strLastSelectCat == _T("高级"))
		{
			InsertAllPropNodes(hRootItem);
		}
		else
		{
			hItem = m_treeModes.InsertItem(_T(" "), hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, -1);
			
			CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);
			if (spExperSite->HaveDynamic() == S_OK)
				m_treeModes.SetCheckState(hRootItem, TRUE);
		}
	}
	
  	m_treeModes.SetRedraw(TRUE);
}

LRESULT CDynamicExpert::OnItemexpandingModes(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pnmh;
	
	HTREEITEM hRootItem = pnmtv->itemNew.hItem;
	CString strItem;
	m_treeModes.GetItemText(hRootItem, strItem);
	if (strItem == _T("高级"))
		EnsureExpandAdvanceNode(hRootItem);
	
	return 0;
}

void CDynamicExpert::EnsureExpandAdvanceNode(HTREEITEM hRootItem)
{
	HTREEITEM hItem = m_treeModes.GetChildItem(hRootItem);
	if (hItem != NULL && m_treeModes.GetItemData(hItem) == -1)
	{
		m_treeModes.DeleteItem(hItem);
		InsertAllPropNodes(hRootItem);
	}
}

void CDynamicExpert::InsertAllPropNodes(HTREEITEM hRootItem)
{
	USES_CONVERSION;
	HTREEITEM hItem;
	int nConectState;
	BOOL bCheckRoot;

	bCheckRoot = FALSE;
	for (int i = 0; i < m_arrProps.GetSize(); i++)
	{
		CPropInfo& prop = m_arrProps[i];
		if (prop.m_dispid == CXPROPID_Name)
			continue;
		
		BOOL bAdded = FALSE;
		for (int j = 0; j < i; j++)
		{
			CPropInfo& propTemp = m_arrProps[j];
			if (propTemp.m_dispid == prop.m_dispid)
			{
				bAdded = TRUE;
				break;
			}
		}
		if (bAdded)
			continue;
		
		nConectState = GetConnectState(prop.m_dispid);
		if (nConectState != CSTATE_DISABLE)
		{
			hItem = m_treeModes.InsertItem(m_bShowHelpString ? OLE2T(prop.m_bstrDesc) : OLE2T(prop.m_bstrName), 
				hRootItem, TVI_LAST);
			m_treeModes.SetItemData(hItem, i);
			
			if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
			{
				m_treeModes.SetCheckState(hItem, TRUE);
				bCheckRoot = TRUE;
			}
		}
	}

	if (hRootItem != NULL)
	{
		if (bCheckRoot)
			m_treeModes.SetCheckState(hRootItem, TRUE);
	}
}

int CDynamicExpert::FindProperty(DISPID dispid)
{
	for (int i = 0; i < m_arrProps.GetSize(); i++)
	{
		if (m_arrProps[i].m_dispid == dispid)
			return i;
	}

	return -1;
}

int CDynamicExpert::GetConnectState(DISPID dispid)
{
	VARIANTARG* pvars = new VARIANTARG[1];
	
	VariantInit(&pvars[0]);
	pvars[0].vt = VT_I4;
	pvars[0].lVal = dispid;
	
	DISPPARAMS disp = {pvars, NULL, 1, 0};
	CComVariant varResult;
	
	CComQIPtr<IDispatch> spDisp = m_ppUnk[0];
	HRESULT hr = spDisp->Invoke(CXMETHID_GetConnectState, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
	
	delete[] pvars;
	
	if (FAILED(hr))
		return -1;
	
	return varResult.lVal;
}

void CDynamicExpert::OnObjectChanged()
{
// 	m_treeModes.SetRedraw(FALSE);

	m_treeModes.SelectItem(NULL);
	
	if (m_pPropInfo != NULL)
	{
		delete m_pPropInfo;
		m_pPropInfo = NULL;
	}

	if (m_nObjects == 0 || m_ppUnk == NULL)
	{
		SyncModePage();
		m_treeModes.DeleteAllItems();
// 		m_treeModes.SetRedraw(TRUE);
		return;
	}

	m_arrProps.RemoveAll();
	m_strExtendCat.Empty();
	
	CTypeInfoBrowse::CrackProperty(m_ppUnk[0], m_arrProps, PROPTYPE_UNKNOWN);
	m_nExtendPropFirst = m_arrProps.GetSize();
	if (m_ppEnumProp[0] != NULL)
		CTypeInfoBrowse::CrackExtendProperty(m_ppEnumProp[0], m_arrProps);

	if (m_arrProps.GetSize() > m_nExtendPropFirst)
	{
		CComQIPtr<ICategorizeProperties> spCategorizeProperties(m_ppUnk[0]);
		if (spCategorizeProperties != NULL)
		{
			CPropInfo& prop = m_arrProps[m_nExtendPropFirst];
			PROPCAT propcat = 0;
			spCategorizeProperties->MapPropertyToCategory(prop.m_dispid, &propcat);
			if (propcat != 0)
			{
				CComBSTR bstrName;
				spCategorizeProperties->GetCategoryName(propcat, LOCALE_SYSTEM_DEFAULT, &bstrName);
				m_strExtendCat = bstrName;
			}
		}
	}
	
	PopulateTree();

	if (m_pLastSelectProp != NULL)
		SelectLastSelectProp();
	else
		SyncModePage();

 	m_treeModes.SetRedraw(TRUE);
}

void CDynamicExpert::SyncModePage()
{
	CTabPage* pPage;
	int nPage;

	SyncSouce();

	// 动态属性页使用了对象信息，必须在此释放
	if (m_nCurPage == 1)
		((CAdvancedPage *)m_apPages[m_nCurPage])->HideDynamicPage();

	HTREEITEM hSelItem = m_treeModes.GetSelectedItem();
	HTREEITEM hRootItem = m_treeModes.GetParentItem(hSelItem);
	
	if (hRootItem == NULL)
	{
		nPage = 0;

		if (m_nObjects != 0 && m_pLastSelectProp != NULL)
		{
			delete m_pLastSelectProp;
			m_pLastSelectProp = NULL;
			m_strLastSelectCat.Empty();
		}
	}
	else
	{
		nPage = 1;

		int nPropIndex = m_treeModes.GetItemData(hSelItem);
		CPropInfo& prop = m_arrProps[nPropIndex];

		CString strCat;
		m_treeModes.GetItemText(hRootItem, strCat);

		if (strCat != _T("高级"))
		{
			if (strCat == _T("颜色"))
				nPage = 6;
					
			switch (prop.m_dispid) 
			{
			case CXPROPID_HorPosition:
			case CXPROPID_VerPosition:
			case CXPROPID_RotateAngle:
				nPage = 2;
				break;
			case CXPROPID_Visible:
				nPage = 3;
				break;
			case CXPROPID_HorScale:
			case CXPROPID_VerScale:
				nPage = 4;
				break;
			case CXPROPID_HorFillPercent:
			case CXPROPID_VerFillPercent:
				nPage = 5;
				break;
			default:
				break;
			}
		}
	
		if (m_pLastSelectProp == NULL)
			m_pLastSelectProp = new CPropInfo;
		*m_pLastSelectProp = prop;
		m_strLastSelectCat = strCat;
	}

	if (nPage != -1)
	{
		pPage = m_apPages[nPage];
		
		if (pPage == NULL)
		{
			switch (nPage)
			{
			case 1:   // 高级
				pPage = new CAdvancedPage;
				((CAdvancedPage *)pPage)->AddRef();
				((CAdvancedPage *)pPage)->m_pExpert = this;
				break;
			case 2:
				pPage = new CMovementPage;
				((CMovementPage *)pPage)->m_pExpert = this;
				break;
			case 3:
				pPage = new CVisiblePage;
				((CVisiblePage *)pPage)->m_pExpert = this;
				break;
			case 4:
				pPage = new CScalePage;
				((CScalePage *)pPage)->m_pExpert = this;
				break;
			case 5:
				pPage = new CFillPage;
				((CFillPage *)pPage)->m_pExpert = this;
				break;
			case 6:
				pPage = new CColorPage;
				((CColorPage *)pPage)->m_pExpert = this;
				break;
			default:
				break;
			}

			m_apPages[nPage] = pPage;
			
			if (pPage != NULL)
			{
				CWindow winRect;
				winRect.Attach(GetDlgItem(IDC_RECT));
				RECT rc;
				winRect.GetWindowRect(&rc);
				ScreenToClient(&rc);
				
				pPage->Create(m_hWnd);
				pPage->MoveTo(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
			}
		}

		if (nPage == 1)
			((CAdvancedPage *)m_apPages[nPage])->m_bCanChangeMode = m_strLastSelectCat == _T("高级");

	}
	
	for (int i = 0; i < NUM_PAGES; i++)
	{
		pPage = m_apPages[i];
		if (pPage != NULL)
			pPage->Show(nPage == i);
	}

	m_nCurPage = nPage;

}

LRESULT CDynamicExpert::OnLButtonDownModes(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	HTREEITEM hCurrItem = NULL;
		
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	TVHITTESTINFO tvh;
	tvh.pt = ptMsg;
	if (m_treeModes.HitTest(&tvh) != NULL && tvh.flags == TVHT_ONITEMSTATEICON/*  && ::GetKeyState(VK_CONTROL) >= 0*/)
	{
		hCurrItem = tvh.hItem;
		m_treeModes.SetFocus();
		m_treeModes.SelectItem(hCurrItem);
		if (m_treeModes.GetParentItem(hCurrItem) == NULL || !m_treeModes.GetCheckState(hCurrItem))
		{
 			SyncModePage();
			return 1;
		}
	}
	
	LRESULT lRet = m_treeModes.DefWindowProc(uMsg, wParam, lParam);
	
	if (hCurrItem != NULL)
	{
		if (!m_treeModes.GetCheckState(hCurrItem))
		{
			int nPropIndex = m_treeModes.GetItemData(hCurrItem);
			CPropInfo& prop = m_arrProps[nPropIndex];

			OnPropertyChanging();
			DeleteDynamic(prop.m_bstrName);
			OnPropertyChanged();
			m_treeModes.SetCheckState(hCurrItem, FALSE);
		}
	}
	
 	SyncModePage();
	
	return lRet;
}

LRESULT CDynamicExpert::OnKeyDownModes(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	HTREEITEM hCurrItem = NULL;
	
	if (wParam == VK_SPACE)
	{
		hCurrItem = m_treeModes.GetSelectedItem();
		if (m_treeModes.GetParentItem(hCurrItem) == NULL || !m_treeModes.GetCheckState(hCurrItem))
		{
 			SyncModePage();
			return 1;
		}
	}
	
	LRESULT lRet = m_treeModes.DefWindowProc(uMsg, wParam, lParam);
	
	if (hCurrItem != NULL)
	{
		if (!m_treeModes.GetCheckState(hCurrItem))
		{
			int nPropIndex = m_treeModes.GetItemData(hCurrItem);
			CPropInfo& prop = m_arrProps[nPropIndex];
			
			OnPropertyChanging();
			DeleteDynamic(prop.m_bstrName);
			OnPropertyChanged();
 			m_treeModes.SetCheckState(hCurrItem, FALSE);
		}
	}
	
 	SyncModePage();
	
	return lRet;
}

LRESULT CDynamicExpert::OnClickedCreateExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	CComBSTR bstr;
	GetDlgItemText(IDC_EXPRESSION, bstr.m_str); 
	
	CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);
	spExperSite->OnGetDataSource(&bstr);
	
	SetDlgItemText(IDC_EXPRESSION, W2T(bstr));
	
	return 0;
}

void CDynamicExpert::ExpressionCreatorProc(BSTR* pbstrExpression, int iFlag)
{
	ATLASSERT(m_pExpCrtExp != NULL);

	if (iFlag == 0)
	{
		CComQIPtr<ICxExpertPageSite> spExperSite(m_pExpCrtExp->m_pPageSite);
		spExperSite->OnGetDataSource(pbstrExpression);
	}
}

HRESULT CDynamicExpert::GetDynamic(BSTR bstrPropName, IUnknown** ppunkDyn)
{
	VARIANTARG* pvars = new VARIANTARG[1];
	for (int i = 0; i < 1; i++)
		VariantInit(&pvars[i]);

	pvars[0].vt = VT_BSTR;
	pvars[0].bstrVal = bstrPropName;

	DISPPARAMS disp = {pvars, NULL, 1, 0};
	CComVariant varResult;
	CComQIPtr<IDispatch> spDisp = m_ppUnk[0];
	HRESULT hr = spDisp->Invoke(CXMETHID_GetConnectObject, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
	delete[] pvars;
	*ppunkDyn = varResult.punkVal;
	
	return hr;
}

HRESULT CDynamicExpert::PutDynamic(BSTR bstrPropName, IUnknown* punkDyn)
{
	VARIANTARG* pvars = new VARIANTARG[2];
	for (int i = 0; i < 2; i++)
		VariantInit(&pvars[i]);

	pvars[1].vt = VT_BSTR;
	pvars[1].bstrVal = bstrPropName;
	pvars[0].vt = VT_UNKNOWN;
	pvars[0].punkVal = punkDyn;

	DISPPARAMS disp = {pvars, NULL, 2, 0};
	CComVariant varResult;
	CComQIPtr<IDispatch> spDisp = m_ppUnk[0];
	HRESULT hr = spDisp->Invoke(CXMETHID_ConnectObject, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
	delete[] pvars;

	return hr;
}

HRESULT CDynamicExpert::DeleteDynamic(BSTR bstrPropName)
{
	VARIANTARG* pvars = new VARIANTARG[1];
	VariantInit(&pvars[0]);
	
	pvars[0].vt = VT_BSTR;
	pvars[0].bstrVal = bstrPropName;
	
	DISPPARAMS disp = {pvars, NULL, 1, 0};
	CComVariant varResult;
	CComQIPtr<IDispatch> spDisp = m_ppUnk[0];
	HRESULT hr = spDisp->Invoke(CXMETHID_Disconnect, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
	delete[] pvars;

	return hr;
}

void CDynamicExpert::GetDynamicClassID(IUnknown* pDynUnk, CLSID* pClassID)
{
	*pClassID = CLSID_NULL;
	
	if (pDynUnk != NULL)
	{
		CComQIPtr<ICxDynamicObject> spDynamicObject = pDynUnk;
		
		CComBSTR bstr;
		spDynamicObject->GetClassName(&bstr);
		
		if (bstr == L"Lookup")
			*pClassID = CLSID_LookupDynamic;
		else if (bstr == L"Linear")
			*pClassID = CLSID_LinearDynamic;
		else if (bstr == L"Format")
			*pClassID = CLSID_FormatDynamic;
		else if (bstr == L"Direct")
			*pClassID = CLSID_DirectDynamic;
	}
}

void CDynamicExpert::OnPropertyChanging()
{
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_VALIDATE);
}

void CDynamicExpert::OnPropertyChanged()
{
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
}

BOOL CDynamicExpert::GetSource(BSTR& bstrText)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	CComBSTR& bstrExpression = (CComBSTR&)bstrText;
	GetDlgItemText(IDC_EXPRESSION, (BSTR&)bstrExpression); 
	bstrExpression = VARC2A(OLE2T(bstrExpression));
	
	CString str = bstrText;
	str.TrimLeft();
	str.TrimRight();
	return !str.IsEmpty();
}

void CDynamicExpert::SetSource(BSTR bstrText)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;
	
	CComBSTR bstrT = VARA2C(OLE2T(bstrText));
	
	SetDlgItemText(IDC_EXPRESSION, W2T(bstrT));
}

void CDynamicExpert::SyncSouce()
{
	HTREEITEM hSelItem = m_treeModes.GetSelectedItem();
	if (m_treeModes.GetParentItem(hSelItem) != NULL)
	{
		::EnableWindow(GetDlgItem(IDC_EXPRESSION), TRUE);

		int nPropIndex = m_treeModes.GetItemData(hSelItem);
		CPropInfo& prop = m_arrProps[nPropIndex];
		
		IUnknown* punkDyn = NULL;
		GetDynamic(prop.m_bstrName, &punkDyn);
		
		if (punkDyn != NULL)
		{
			CComDispatchDriver dd(punkDyn);
			CComVariant var;
			dd.GetPropertyByName(L"Source", &var);
			if (var.vt != VT_BSTR)
				var.ChangeType(VT_BSTR);
			
			if (var.vt == VT_BSTR)
				SetSource(var.bstrVal);

			return;
		}
	}
	else
	{
		::EnableWindow(GetDlgItem(IDC_EXPRESSION), FALSE);
	}

	SetDlgItemText(IDC_EXPRESSION, _T(""));
}

void CDynamicExpert::InitDynamicPropInfo()
{
	USES_CONVERSION;

	if (m_pPropInfo != NULL)
		delete m_pPropInfo;

	m_pPropInfo = new CDynamicPropInfo;

	HTREEITEM hSelItem = m_treeModes.GetSelectedItem();
	ATLASSERT(m_treeModes.GetParentItem(hSelItem) != NULL);
	
	int nPropIndex = m_treeModes.GetItemData(hSelItem);
	CPropInfo& prop = m_arrProps[nPropIndex];

	if (prop.m_proptype == PROPTYPE_UNKNOWN)
		return;
	
	m_pPropInfo->spUnkObject = m_ppUnk[0];
	m_pPropInfo->nType = prop.m_proptype;
	m_pPropInfo->bstrPropertyName = prop.m_bstrName;

	CComDispatchDriver dd(m_pPropInfo->spUnkObject);
	dd.GetProperty(prop.m_dispid, &m_pPropInfo->varCurrentValue);

	m_pPropInfo->bSupportMouseInput = (GetConnectState(prop.m_dispid) & 0x1) != 0;

	m_pPropInfo->pfnExpression = ExpressionCreatorProc;

	IUnknown* punkDyn = NULL;
	GetDynamic(m_pPropInfo->bstrPropertyName, &punkDyn);
	m_pPropInfo->spUnkDynamic = punkDyn;

	if (m_pPropInfo->nType == PROPTYPE_ENUM || m_pPropInfo->nType == PROPTYPE_ENUMSTRING)
	{
		CEnumPropItemArray* pArrEnumItem = NULL;
		CTypeInfoBrowse::CrackPropertyType(m_ppUnk[0], prop.m_bstrName, (LPARAM)&pArrEnumItem);
		
		ATLASSERT(pArrEnumItem != NULL);

		for (int i = 0; i < pArrEnumItem->GetSize(); i++)
		{
			CEnumPropItem item;
			item.m_bstrName = (*pArrEnumItem)[i].m_bstrName;
			item.m_bstrDesc = m_bShowHelpString ? 
				(*pArrEnumItem)[i].m_bstrDesc : (*pArrEnumItem)[i].m_bstrName;
			item.m_varValue =(*pArrEnumItem)[i].m_varValue;
			m_pPropInfo->m_arrEnumPropItems.Add(item);
		}

		if (pArrEnumItem)
			delete pArrEnumItem;
	}
}

LRESULT CDynamicExpert::OnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	if (m_nCurPage != -1 && m_apPages[m_nCurPage] != NULL)
	{
		OnPropertyChanging();
		m_apPages[m_nCurPage]->Apply();
 		OnPropertyChanged();
	}

// 	::EnableWindow(GetDlgItem(IDC_APPLY), FALSE);

	return 0;
}

LRESULT CDynamicExpert::OnClickedNextObject(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);
	spExperSite->OnNextDynamicObject();
	
	return 0;
}

void CDynamicExpert::SelectAdvacedProp(DISPID dispid)
{
	CString strItem;
	HTREEITEM hRootItem = m_treeModes.GetRootItem();
	while (hRootItem != NULL)
	{
		m_treeModes.GetItemText(hRootItem, strItem);
		if (strItem == _T("高级"))
			break;
		
		hRootItem = m_treeModes.GetNextSiblingItem(hRootItem);
	}

	if (hRootItem != NULL)
	{
		EnsureExpandAdvanceNode(hRootItem);

		HTREEITEM hItem = m_treeModes.GetChildItem(hRootItem);
		while (hItem != NULL)
		{
			int nPropIndex = m_treeModes.GetItemData(hItem);
			CPropInfo& prop = m_arrProps[nPropIndex];
			if (prop.m_dispid == dispid)
			{
				m_treeModes.SelectItem(hItem);
				SyncModePage();
				break;
			}
			
			hItem = m_treeModes.GetNextSiblingItem(hItem);
		}
	}
}

void CDynamicExpert::SelectLastSelectProp()
{
	if (m_pLastSelectProp == NULL)
		return;

	CString strItem;
	HTREEITEM hRootItem = m_treeModes.GetRootItem();
	while (hRootItem != NULL)
	{
		m_treeModes.GetItemText(hRootItem, strItem);
		if (strItem == m_strLastSelectCat)
			break;
		
		hRootItem = m_treeModes.GetNextSiblingItem(hRootItem);
	}

	HTREEITEM hItem = m_treeModes.GetChildItem(hRootItem);
	while (hItem != NULL)
	{
		int nPropIndex = m_treeModes.GetItemData(hItem);
		CPropInfo& prop = m_arrProps[nPropIndex];
		if (prop.m_bstrName == m_pLastSelectProp->m_bstrName ||
			prop.m_dispid == m_pLastSelectProp->m_dispid)
		{
		// 	m_treeModes.SetRedraw(FALSE);
		// 	m_treeModes.PostMessage(TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem);
		  	m_treeModes.SelectItem(hItem);
		// 	m_treeModes.SetRedraw(TRUE);
		 // 	m_treeModes.InvalidateRect(NULL);
		// 	m_treeModes.UpdateWindow();
			
		// 	
 			SyncModePage();
			
			break;
		}

		hItem = m_treeModes.GetNextSiblingItem(hItem);
	}
}

void CDynamicExpert::GetSelectProp(CPropInfo& prop)
{
	HTREEITEM hSelItem = m_treeModes.GetSelectedItem();
	ATLASSERT(m_treeModes.GetParentItem(hSelItem) != NULL);
	
	int nPropIndex = m_treeModes.GetItemData(hSelItem);
	prop = m_arrProps[nPropIndex];
}