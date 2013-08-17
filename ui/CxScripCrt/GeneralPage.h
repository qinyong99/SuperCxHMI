// GeneralPage.h: interface for the CGeneralPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERALPAGE_H__6FF92FD5_BF83_4C26_89D5_D7CE8FF1B4D9__INCLUDED_)
#define AFX_GENERALPAGE_H__6FF92FD5_BF83_4C26_89D5_D7CE8FF1B4D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreePage.h"

struct _SymbolNode;

void GetExtendSymbols(DWORD dwID, CString& strText);

class CGeneralPage : public CTreePage  
{
public:
	CGeneralPage();

	virtual void OnInit();

	void InsertSymbols(DWORD dwSymbolMask);
	void InsertSymbolNode(LPCTSTR lpszNode, _SymbolNode* pSymbols, int nCount, DWORD dwMask = -1);

	virtual CString GetQualifyNameSpace(HTREEITEM hItem);

	BOOL OnBuildSymbol(DWORD dwID, CString& strText);

	BOOL OnBuildTag(CString& strText);
	BOOL OnBuildOLEDBSource(CString& strText);
	BOOL OnBuildSQL(CString& strText);

	DWORD m_dwSymbolMask;
};

#endif // !defined(AFX_GENERALPAGE_H__6FF92FD5_BF83_4C26_89D5_D7CE8FF1B4D9__INCLUDED_)
