// SQLBuilder.cpp : Implementation of CSQLBuilder
#include "stdafx.h"
#include "CxExtObj.h"
#include "SQLBuilder.h"
#include "DataSourceDlg.h"
#include "SQLBuildDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSQLBuilder


STDMETHODIMP CSQLBuilder::Build(/*[in]*/ long ParentWnd, /*[out, retval]*/ BSTR* pbstrSQL)
{
	CDataSourceDlg dlgSource;
	if (IDOK != dlgSource.DoModal((HWND)ParentWnd))
		return S_FALSE;

	CSQLBuildDlg dlgSQLBuild;
	dlgSQLBuild.m_bstrConnectionString = dlgSource.m_bstrConnectionString;

	if (IDOK != dlgSQLBuild.DoModal((HWND)ParentWnd))
		return S_FALSE;

	*pbstrSQL = dlgSQLBuild.m_bstrSQL.Copy();

	return S_OK;
}
