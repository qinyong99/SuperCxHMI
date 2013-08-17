// EventExpert.cpp : Implementation of CEventExpert
#include "stdafx.h"
#include "CxEvnExp.h"
#include "EventExpert.h"
#include "ActionParser.h"

/////////////////////////////////////////////////////////////////////////////
// CEventExpert

STDMETHODIMP CEventExpert::Apply(void)
{
	ATLTRACE(_T("CEventExpert::Apply\n"));
	for (UINT i = 0; i < m_nObjects; i++)
	{
		// Do something interesting here
		// ICircCtl* pCirc;
		// m_ppUnk[i]->QueryInterface(IID_ICircCtl, (void**)&pCirc);
		// pCirc->put_Caption(CComBSTR("something special"));
		// pCirc->Release();
	}
	m_bDirty = FALSE;
	return S_OK;
}

void CEventExpert::OnObjectChanged()
{
	if (m_nObjects == 0 || m_ppUnk == NULL)
	{
		m_listEvents.DeleteAllItems();
		m_cmbActions.SetCurSel(-1);
		SyncActionPage();
		::EnableWindow(GetDlgItem(IDC_APPLY), FALSE);
		return;
	}

	LoadEventsList();
	DisplayShowHandState();
}

static _TCHAR *_gszActions[NUM_PAGES] =
{
	_T("（无）"), 
	_T("显示画面"), 
	_T("下载配方"),
	_T("数据输入"), 
	_T("其它"),
};

LRESULT CEventExpert::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_listEvents.SubclassWindow(GetDlgItem(IDC_EVENTS));
	m_listEvents.SetExtendedListViewStyle(/*m_listEvents.GetExtendedLVStyle() | */LVS_EX_CHECKBOXES);
	m_listEvents.InsertColumn(0, _T("事件"), LVCFMT_LEFT, 108, -1);
	
	m_cmbActions.Attach(GetDlgItem(IDC_ACTIONS));
	::EnableWindow(GetDlgItem(IDC_APPLY), FALSE);

	m_btnShowHand.Attach(GetDlgItem(IDC_SHOW_HAND));

	for (int i = 0; i < NUM_PAGES; i++)
	{
		m_cmbActions.AddString(_gszActions[i]);
		m_apPages[i] = NULL;
	}

	LoadEventsList();
	DisplayShowHandState();

	return 0;
}

void CEventExpert::OnFinalMessage(HWND hWnd)
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

LRESULT CEventExpert::OnClickedNextObject(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);
	spExperSite->OnNextEventObject();
	
	return 0;
}

LRESULT CEventExpert::OnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);

	// 动作脚本
	int nSelAction = m_cmbActions.GetCurSel();
	if (nSelAction == CB_ERR)
		return 0;

	
	int nSelEvent = m_listEvents.GetSelectedIndex();
	if (nSelEvent == -1)
		return 0;
	
	CString strEvent = m_arrEvents[m_listEvents.GetItemData(nSelEvent)].m_strName;
	
	if (m_apPages[nSelAction] != NULL)
	{
		m_apPages[nSelAction]->Apply();
		
		CString strParams;
		CStringArray arrParams;
		GetObjectEventParams(m_ppUnk[0], arrParams, strEvent);
		for (int i = 0; i < arrParams.GetSize(); i++)
		{
			strParams += arrParams[i];
			if (i < arrParams.GetSize() - 1)
				strParams += _T(", ");
		}
		
		if (spExperSite->InsertEventHandler(T2OLE(strEvent), T2OLE(strParams), T2OLE(m_strActionScript)) == S_OK)
			m_listEvents.SetCheckState(nSelEvent, TRUE);
	}
	else
	{
		// 删除动作
		if (nSelAction == 0)
		{
			if (spExperSite->InsertEventHandler(T2OLE(strEvent), NULL, NULL) == S_OK)
				m_listEvents.SetCheckState(nSelEvent, FALSE);
		}
		// 其它动作
		else if (nSelAction == m_cmbActions.GetCount() - 1) 
		{
			if (spExperSite->OnEditEventHandler(T2OLE(strEvent)) == S_OK)
				m_listEvents.SetCheckState(nSelEvent, TRUE);
		}
	}
	
	BOOL bOtherAction = (m_cmbActions.GetCurSel() == m_cmbActions.GetCount() - 1);
	::EnableWindow(GetDlgItem(IDC_APPLY), bOtherAction);
	::SetWindowText(GetDlgItem(IDC_APPLY), bOtherAction ? _T("编辑") : _T("应用"));

	return 0;
}

void CEventExpert::SyncEventChange()
{
	USES_CONVERSION;
	
	m_strActionScript.Empty();
	
	int nSelEvent = m_listEvents.GetSelectedIndex();
	if (nSelEvent == -1)
		return;

	m_strLastSelectEvent = m_arrEvents[m_listEvents.GetItemData(nSelEvent)].m_strName;
	
	CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);
	
	CComBSTR bstrEventSub;
	if (spExperSite->GetEventHandler(T2OLE(m_strLastSelectEvent), &bstrEventSub) == S_OK)
		m_strActionScript = bstrEventSub;
	
	// 根据已经存在的脚本激活对应的页面
	if (m_strActionScript.IsEmpty())
	{
		m_cmbActions.SelectString(0, _T("（无）"));
	}
	else
	{
		CActionParser parser(m_strActionScript);
		CString strAction;
		if (parser.IsExpertGenerate(strAction))
			m_cmbActions.SelectString(0, strAction);
		else
			m_cmbActions.SelectString(0, _T("其它"));
	}
	
	SyncActionPage();

	BOOL bOtherAction = (m_cmbActions.GetCurSel() == m_cmbActions.GetCount() - 1);
	::EnableWindow(GetDlgItem(IDC_APPLY), bOtherAction);
	::SetWindowText(GetDlgItem(IDC_APPLY), bOtherAction ? _T("编辑") : _T("应用"));
}

void CEventExpert::SyncActionPage()
{
	CTabPage* pPage;
	int nSel = m_cmbActions.GetCurSel();
	if (nSel != CB_ERR)
	{
		pPage = m_apPages[nSel];
		
		if (pPage == NULL)
		{
			switch (nSel)
			{
			case 1:   // 显示画面
				pPage = new CShowPicturePage;
				((CShowPicturePage *)pPage)->m_pExpert = this;
				break;
			case 2:   // 下载配方
				pPage = new CDownloadRecipePage;
				((CDownloadRecipePage *)pPage)->m_pExpert = this;
				break;
			case 3:   // 数据输入
				pPage = new CDataEntryPage;
				((CDataEntryPage *)pPage)->m_pExpert = this;
				break;
			default:
				break;
			}

			m_apPages[nSel] = pPage;

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
	}
	
	for (int i = 0; i < NUM_PAGES; i++)
	{
		pPage = m_apPages[i];
		if (pPage != NULL)
			pPage->Show(nSel == i);
	}
}

LRESULT CEventExpert::OnSelchangeActions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SyncActionPage();

	BOOL bOtherAction = (m_cmbActions.GetCurSel() == m_cmbActions.GetCount() - 1);
//	if (bOtherAction)
		::EnableWindow(GetDlgItem(IDC_APPLY), TRUE);
	::SetWindowText(GetDlgItem(IDC_APPLY), bOtherAction ? _T("编辑") : _T("应用"));
	
	return 0;
}

void CEventExpert::LoadEventsList()
{
	USES_CONVERSION;

	if (!::IsWindow(m_listEvents.m_hWnd))
		return;
	
	m_listEvents.DeleteAllItems();
	m_arrEvents.RemoveAll();

	if (m_nObjects == 0 || m_ppUnk == NULL)
		return;

	CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);

	CComBSTR bstrEventSubs;
	spExperSite->GetEventHandler(NULL, &bstrEventSubs);
	CString strEventsSubs = bstrEventSubs;
	strEventsSubs.MakeUpper();

	int nDefaultIndex = 0;
	GetObjectEvents(m_ppUnk[0], m_arrEvents, nDefaultIndex);
	
	for (int i = 0; i < m_arrEvents.GetSize(); i++)
	{
		CEventInfo& event = m_arrEvents[i];
		CString strEvent = event.m_strName;
		int nIndex = m_listEvents.InsertItem(i, 
			m_bShowHelpString ? event.m_strDescription : strEvent);
		m_listEvents.SetItemData(nIndex, i);

		if (m_strLastSelectEvent.CompareNoCase(strEvent) == 0)
			m_listEvents.SelectItem(nIndex);

		strEvent.MakeUpper();
		strEvent.Insert(0, _T("_"));
		int nFind = strEventsSubs.Find(strEvent);
		if (nFind != -1)
		{
			nFind += strEvent.GetLength();
			if (strEventsSubs.GetLength() <= nFind || strEventsSubs[nFind] == TCHAR(' '))
				m_listEvents.SetCheckState(nIndex, TRUE);
		}
	}

	if (m_listEvents.GetSelectedIndex() == -1 && m_arrEvents.GetSize() > 0)
		m_listEvents.SelectItem(nDefaultIndex);

	m_parrSortEvents = &m_arrEvents;
	m_listEvents.SortItems((PFNLVCOMPARE)CompareItems, 0);
	m_parrSortEvents = NULL;

	SyncEventChange();
}

void CEventExpert::DisplayShowHandState()
{
	if (!::IsWindow(m_btnShowHand.m_hWnd))
		return;
	
	BOOL bEnable = FALSE;
	
	if (m_nObjects != 0 && m_ppUnk != NULL)
	{
		CComDispatchDriver dd(m_ppUnk[0]);
		CComVariant var;
		if (dd.GetProperty(CXPROPID_MousePointer, &var) == S_OK)
		{
			bEnable = TRUE;
			m_btnShowHand.SetCheck(var.lVal == MousePointerHand ? 1 : 0);
		}
	}
	
	m_btnShowHand.EnableWindow(bEnable);
}

void CEventExpert::GetObjectEventParams(LPUNKNOWN pUnkObject, CStringArray& arrParams, LPCTSTR lpszEvent)
{
	USES_CONVERSION;

	CComPtr<IProvideClassInfo> spProvideClassInfo;
	CComPtr<ITypeInfo> spClassInfo;
	CComPtr<ITypeInfo> spTypeInfo;
	TYPEATTR* pClassTypeAttr = NULL;
	TYPEATTR* pTypeAttr = NULL;

	if (lpszEvent == NULL)
	{
		ATLASSERT(FALSE);
		return;
	}

	// 获取类信息
	HRESULT hr = pUnkObject->QueryInterface(&spProvideClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	hr = spProvideClassInfo->GetClassInfo(&spClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	spClassInfo->GetTypeAttr(&pClassTypeAttr);
	ATLASSERT(pClassTypeAttr->typekind == TKIND_COCLASS);

	//遍历类所实现的接口
	int i;
	for (i = 0; i < pClassTypeAttr->cImplTypes; i++)
	{
		int nImplTypeFlags; 
		spClassInfo->GetImplTypeFlags(i, &nImplTypeFlags);
		if ((nImplTypeFlags & IMPLTYPEFLAG_FSOURCE) == 0)	// 不是事件接口
			continue;
		
		// 获取应用类型信息句柄
		HREFTYPE hRef;
		hr = spClassInfo->GetRefTypeOfImplType(i, &hRef);
		if (FAILED(hr))
			goto ONERROR;

		// 获取类型信息
		hr = spClassInfo->GetRefTypeInfo(hRef, &spTypeInfo);
		if (FAILED(hr))
			goto ONERROR;	

		spTypeInfo->GetTypeAttr(&pTypeAttr);

		// 查找Dispatch接口
		if (pTypeAttr->typekind == TKIND_INTERFACE)
		{
			//Get Dispatch Interface's Info;
			hr = spTypeInfo->GetRefTypeOfImplType(-1, &hRef);
			if (FAILED(hr))
				goto ONERROR;	

			CComPtr<ITypeInfo> spTypeInfoTmp;
			hr = spTypeInfo->GetRefTypeInfo(hRef, &spTypeInfoTmp);
			if (FAILED(hr))
				goto ONERROR;

			spTypeInfo->ReleaseTypeAttr(pTypeAttr);
			spTypeInfo = spTypeInfoTmp;
			spTypeInfo->GetTypeAttr(&pTypeAttr);
		}

		ATLASSERT(pTypeAttr->typekind == TKIND_DISPATCH);

		BSTR bstrNames = T2OLE(lpszEvent); 
		MEMBERID memberid;
		spTypeInfo->GetIDsOfNames(&bstrNames, 1, &memberid);

		BSTR bstrParams[32];
		UINT numParams = 0;

		spTypeInfo->GetNames(memberid, bstrParams, 32, &numParams);
		for (UINT nParam = 1; nParam < numParams; nParam++)
		{
			arrParams.Add(CString(bstrParams[nParam]));
			SysFreeString(bstrParams[nParam]);
		}
			
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeAttr = NULL;
		spTypeInfo.Release();
	}

ONERROR:
	if (spClassInfo != NULL && pClassTypeAttr != NULL)
		spClassInfo->ReleaseTypeAttr(pClassTypeAttr);
	if (spTypeInfo != NULL && pClassTypeAttr != NULL)
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);
}

void CEventExpert::GetObjectEvents(LPUNKNOWN pUnkObject, CEventInfoArray& arrInfos, int& nDefaultIndex)
{
	USES_CONVERSION;

	CComPtr<IProvideClassInfo> spProvideClassInfo;
	CComPtr<ITypeInfo> spClassInfo;
	CComPtr<ITypeInfo> spTypeInfo;
	TYPEATTR* pClassTypeAttr = NULL;
	TYPEATTR* pTypeAttr = NULL;
	nDefaultIndex = -1;
	LONG lDefaultMemberID;

	// 获取类信息
	HRESULT hr = pUnkObject->QueryInterface(&spProvideClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	hr = spProvideClassInfo->GetClassInfo(&spClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	spClassInfo->GetTypeAttr(&pClassTypeAttr);
	ATLASSERT(pClassTypeAttr->typekind == TKIND_COCLASS);

	//遍历类所实现的接口
	int i;
	for (i = 0; i < pClassTypeAttr->cImplTypes; i++)
	{
		int nImplTypeFlags; 
		spClassInfo->GetImplTypeFlags(i, &nImplTypeFlags);
		if ((nImplTypeFlags & IMPLTYPEFLAG_FSOURCE) == 0)	// 不是事件接口
			continue;
		
		// 获取应用类型信息句柄
		HREFTYPE hRef;
		hr = spClassInfo->GetRefTypeOfImplType(i, &hRef);
		if (FAILED(hr))
			goto ONERROR;

		// 获取类型信息
		hr = spClassInfo->GetRefTypeInfo(hRef, &spTypeInfo);
		if (FAILED(hr))
			goto ONERROR;	

		spTypeInfo->GetTypeAttr(&pTypeAttr);

		// 查找Dispatch接口
		if (pTypeAttr->typekind == TKIND_INTERFACE)
		{
			//Get Dispatch Interface's Info;
			hr = spTypeInfo->GetRefTypeOfImplType(-1, &hRef);
			if (FAILED(hr))
				goto ONERROR;	

			CComPtr<ITypeInfo> spTypeInfoTmp;
			hr = spTypeInfo->GetRefTypeInfo(hRef, &spTypeInfoTmp);
			if (FAILED(hr))
				goto ONERROR;

			spTypeInfo->ReleaseTypeAttr(pTypeAttr);
			spTypeInfo = spTypeInfoTmp;
			spTypeInfo->GetTypeAttr(&pTypeAttr);
		}

		ATLASSERT(pTypeAttr->typekind == TKIND_DISPATCH);

		//遍历Dispatch接口中的所有函数
		for (int nFunction = 0; nFunction < pTypeAttr->cFuncs; nFunction++)
		{
			FUNCDESC* pfd;
			spTypeInfo->GetFuncDesc(nFunction, &pfd);
			
			CComBSTR bstrName;
			CComBSTR bstrDocString;
			spTypeInfo->GetDocumentation(pfd->memid, &bstrName, 
				m_bShowHelpString ? &bstrDocString : NULL, NULL, NULL);

			CEventInfo event;
			event.m_strName = bstrName;
			event.m_strDescription = bstrDocString;

			arrInfos.Add(event);

			// 确定缺省事件
			if (nDefaultIndex == -1 ||
				(pfd->memid < lDefaultMemberID && (pfd->memid >= 0 || lDefaultMemberID >= 3)))
			{
				nDefaultIndex = nFunction;
				lDefaultMemberID = pfd->memid;
			}
					
			spTypeInfo->ReleaseFuncDesc(pfd);
		}
		
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeAttr = NULL;
		spTypeInfo.Release();
	}

ONERROR:
	if (spClassInfo != NULL && pClassTypeAttr != NULL)
		spClassInfo->ReleaseTypeAttr(pClassTypeAttr);
	if (spTypeInfo != NULL && pClassTypeAttr != NULL)
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);	
}

void CEventExpert::UpdateActionScript(LPCTSTR lpszAction, LPCTSTR lpszScript)
{
	m_strActionScript = _T("'{动作专家(");
	m_strActionScript += lpszAction;
	m_strActionScript += _T(")\r\n");
	m_strActionScript += lpszScript;
	m_strActionScript += _T("\r\n");
	m_strActionScript += _T("' 动作专家}");
}

void CEventExpert::OnActionScriptChanged()
{
	::EnableWindow(GetDlgItem(IDC_APPLY), TRUE);
}

LRESULT CEventExpert::OnClickedShowHand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_nObjects != 0 && m_ppUnk != NULL)
	{
		CComDispatchDriver dd(m_ppUnk[0]);
		CComVariant var = m_btnShowHand.GetCheck() == 0  ? MousePointerArrow : MousePointerHand;
		if (SUCCEEDED(dd.PutProperty(CXPROPID_MousePointer, &var)))
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	}

	return 0;
}

LRESULT CEventExpert::OnLButtonDownEvents(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	int nCurrItem = -1;

	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	LVHITTESTINFO lvh;
	lvh.pt = ptMsg;
	if (m_listEvents.HitTest(&lvh) != -1 && lvh.flags == LVHT_ONITEMSTATEICON && ::GetKeyState(VK_CONTROL) >= 0)
	{
		nCurrItem = lvh.iItem;
		m_listEvents.SetFocus();
		m_listEvents.SelectItem(nCurrItem);
		if (!m_listEvents.GetCheckState(nCurrItem))
		{
			SyncEventChange();
			return 1;
		}
	}

	LRESULT lRet = m_listEvents.DefWindowProc(uMsg, wParam, lParam);

	if (nCurrItem != -1)
	{
		if (!m_listEvents.GetCheckState(nCurrItem))
		{
			CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);
			
			CString strEvent = m_arrEvents[m_listEvents.GetItemData(nCurrItem)].m_strName;
				
			BOOL b = spExperSite->InsertEventHandler(T2OLE(strEvent), NULL, NULL) == S_OK;
			m_listEvents.SetCheckState(nCurrItem, FALSE);
		}
	}

	SyncEventChange();

	return lRet;
}

LRESULT CEventExpert::OnKeyDownEvents(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	int nCurrItem = -1;
	
	if (wParam == VK_SPACE)
	{
		nCurrItem = m_listEvents.GetSelectedIndex();
		if (!m_listEvents.GetCheckState(nCurrItem))
		{
			SyncEventChange();
			return 1;
		}
	}
	
	LRESULT lRet = m_listEvents.DefWindowProc(uMsg, wParam, lParam);
	
	if (nCurrItem != -1)
	{
		if (!m_listEvents.GetCheckState(nCurrItem))
		{
			CComQIPtr<ICxExpertPageSite> spExperSite(m_pPageSite);
			
			CString strEvent = m_arrEvents[m_listEvents.GetItemData(nCurrItem)].m_strName;
			
			BOOL b = spExperSite->InsertEventHandler(T2OLE(strEvent), NULL, NULL) == S_OK;
			m_listEvents.SetCheckState(nCurrItem, FALSE);
		}
	}
	
	SyncEventChange();
	
	return lRet;
}

STDMETHODIMP CEventExpert::IsSupportMultiObject()
{
	return S_FALSE;
}

CEventInfoArray* CEventExpert::m_parrSortEvents = NULL;

int CALLBACK CEventExpert::CompareItems(int nItem1, int nItem2, LPARAM nCol)
{
	int nCmp = 0;
	CEventInfo& event1 = (*m_parrSortEvents)[nItem1];
	CEventInfo& event2 = (*m_parrSortEvents)[nItem2];
	
	switch (nCol)
	{
	case 0:
		nCmp = event1.m_strName.Compare(event2.m_strName);
		break;
	}

	return nCmp;
}