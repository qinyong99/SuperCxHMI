// TagPick.cpp : Implementation of CTagPick
#include "stdafx.h"
#include "CxTagPick.h"
#include "TagPick.h"
#include "TagPickDlg.h"
#include "TagGroupPickDlg.h"
#include "SecurityAreaPickDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CTagPick

STDMETHODIMP CTagPick::PickTag(long hParent, long nX, long nY, BOOL bUseLocalSetup, BOOL bDisableAssoc, BSTR bstrAssocGroup, BSTR bstrTag, BSTR* pbstrPickTag)
{
	CTagPickDlg dlg;

	dlg.m_bUseLocalSetup = bUseLocalSetup;
	dlg.m_strAssocTagGroup = bstrAssocGroup;
	dlg.m_bDisableAssoc = (bDisableAssoc != VARIANT_FALSE);
	dlg.m_strPickTag = bstrTag;

	CString strAssocGroup = bstrAssocGroup;
	if (strAssocGroup.Left(7) == _T("NetNode"))
	{
		CComBSTR bstrHostAddr;
		long lHostPort;
		CComBSTR bstrUserName;
		CComBSTR bstrPassword;

		int nIndex1 = strAssocGroup.Find(_T("HostAddr:"), 0);
		int nIndex2 = strAssocGroup.Find(_T(";"), nIndex1);
		bstrHostAddr = strAssocGroup.Mid(nIndex1 + 9, nIndex2 - nIndex1 - 9);

		nIndex1 = strAssocGroup.Find(_T("HostPort:"), 0);
		nIndex2 = strAssocGroup.Find(_T(";"), nIndex1);
		lHostPort = atoi(strAssocGroup.Mid(nIndex1 + 9, nIndex2 - nIndex1 - 9));

		nIndex1 = strAssocGroup.Find(_T("UserName:"), 0);
		nIndex2 = strAssocGroup.Find(_T(";"), nIndex1);
		bstrUserName = strAssocGroup.Mid(nIndex1 + 9, nIndex2 - nIndex1 - 9);

		nIndex1 = strAssocGroup.Find(_T("Password:"), 0);
		nIndex2 = strAssocGroup.Find(_T(";"), nIndex1);
		bstrPassword = strAssocGroup.Mid(nIndex1 + 9, nIndex2 - nIndex1 - 9);

//		dlg.m_pRootNode->m_pOnlyNetNode = new CNetNode(L"ThisNode", bstrHostAddr, TRUE, lHostPort);
//		dlg.m_pRootNode->m_pOnlyNetNode->m_bstrUserName = bstrUserName;
//		dlg.m_pRootNode->m_pOnlyNetNode->m_bstrPassword = bstrPassword;
//		dlg.m_strAssocTagGroup = _T("Tags.ThisNode");
	}

	//注意，没有选择返回E_FAIL
	if (dlg.DoModal(hParent != NULL ? (HWND)hParent : ::GetActiveWindow()) != IDOK)
		return E_FAIL;
	
	*pbstrPickTag = dlg.m_strPickTag.AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPick::PickTagGroup(long hParent, long nX, long nY, BOOL bUseLocalSetup, BOOL bDisableAssoc, BSTR bstrAssocGroup, BSTR bstrGroup, BSTR* pbstrPickGroup)
{
	CTagGroupPickDlg dlg;
	dlg.m_pRootNode = new CRootNode(bUseLocalSetup);
	dlg.m_bstrAssocGroup = bstrAssocGroup;
	dlg.m_bDisableAssoc = (bDisableAssoc != VARIANT_FALSE);
	dlg.m_strPickGroup = bstrGroup;
	//注意，没有选择返回E_FAIL
	if (dlg.DoModal((HWND)hParent) != IDOK)
		return E_FAIL;
	
	*pbstrPickGroup = dlg.m_strPickGroup.AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPick::PickSecurityArea(long hParent, long nX, long nY, BOOL bUseLocalSetup, BSTR bstrArea, BSTR* pbstrPickArea)
{
	CSecurityAreaPickDlg dlg;
	dlg.m_strPickArea = bstrArea;
	//注意，没有选择返回E_FAIL
	if (dlg.DoModal((HWND)hParent) != IDOK)
		return E_FAIL;

	*pbstrPickArea = dlg.m_strPickArea.AllocSysString();

	return S_OK;
}