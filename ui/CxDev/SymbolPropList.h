#ifndef _SYMBOLPROPLIST_H__
#define _SYMBOLPROPLIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TagTableList.h : header file
//


#include "InplaceEdit.h"
#include "PopupList.h"

/////////////////////////////////////////////////////////////////////////////
// CSymbolPropList window

class CSymbolPropList : public CListCtrl
{
// Construction
public:
	CSymbolPropList();

// Attributes
public:
	CFont m_font;
	CFont m_fontBold;
	CPen m_penGrid;

 	CInplaceEdit m_editInplace;
	int m_nSelectColumn;
	int m_nEditItem;
	int m_nEditColumn;

// Operations
public:
	void Init();

	BOOL AddPropertyItem(CSymbolProp* pProp, BOOL bInsert = FALSE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolPropList)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	ICxEnumExtendProperty* EnumItem();

	BOOL OnEditCell(int nItem, int nColumn);

	void OnChangeTagType();
	CString MakeUniqueTagName(int* pnNum = NULL);
	BOOL IsNameDuplicate(CString& strNew);
	CSymbolProp* GetPropertyItem(int iIndex);
	int GetSelectPropertyItem();
	enumPropType GetSelectItemType();
	void EditSelectPropertyItem();
	void DeleteSelectPropertyItem();
	int GetPropertyItemCount();

	virtual ~CSymbolPropList();

//	afx_msg void OnDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
//	afx_msg void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	// Generated message map functions
protected:
	//{{AFX_MSG(CSymbolPropList)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocusEdit();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _SYMBOLPROPLIST_H__
