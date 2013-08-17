////////////////////////////////////////////////////////////////
// SymbolDesignDlg.h : header file

#ifndef _SYMBOLDESIGNDLG_H__
#define _SYMBOLDESIGNDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SymbolPropList.h"
#include "GroupObj.h"	// Added by ClassView
#include "SymbolPreviewWnd.h"


class CGroupObj;

#define WM_TREEITEMCLICK  WM_USER + 101 

/////////////////////////////////////////////////////////////////////////////
// CSymbolDesignDlg dialog

class CSymbolDesignDlg : public CDialog
{
// Construction
public:
	CSymbolDesignDlg(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CSymbolObj* m_pObject;

	static void ExpressionCreatorProc(BSTR* pbstrExpression, int iFlag = 0);
	static CSymbolObj* m_pExpCrtObject;
	static CSymbolPropList* m_pExpCrtList;
	
	void OnSelchangedListProp(BOOL bSavePrev);
	void OnChangeTagType();
	void SyncPropAndAdvance();

	void SaveSubObjectChoice();
	void SaveSubObjectCheckState(HTREEITEM hObjectItem, CSymbolProp *pProp);
	void UpdateSubObjectCheckState(HTREEITEM hObjectItem, CSymbolProp* pProp);
	int AddSubObjectProperties(CDevObj* pObj, enumPropType enumType, HTREEITEM hParent);
	void UpdateSubObjectItem();
	
	void UpdateSubObjectProperyTree();

	void InitSubObjectProperyTree();
	BOOL InitPreview();

// Dialog Data
	//{{AFX_DATA(CSymbolDesignDlg)
	enum { IDD = IDD_GROUP_DESIGN };
	CSymbolPropList	m_listProp;
	CButton	m_btnAdvance;
	CButton	m_btnCheck;
	CTreeCtrl	m_treeProps;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolDesignDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	enumPropType GetObjectPropType(CSymbolProp::CRelatedProp *pRelation, LPARAM lParam);
	
	void OnPropValueChanged();
	void OnPropValueBeginEdit();

	CDevObj* m_pPropValueEditObj;
	CString m_strPropValueEditName;
	
	int m_nLastSelItem;
	BOOL m_bDisplayAllProp;
	
	CPropInfoArray* GetSubObjectProperties(CDevObj* pSubObj, enumPropType enumType);
	CArray<CString, CString&> m_arrPropName;

	CSymbolPreviewWnd* m_pPreviewWnd;

	// Generated message map functions
	//{{AFX_MSG(CSymbolDesignDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDelline();
	afx_msg void OnEditline();
	afx_msg void OnInsertline();
	afx_msg void OnCheckAll();
	afx_msg void OnButtonAdvance();
	afx_msg void OnSelchangedTreeProp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTreeProp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnKeydownTreeProp(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnPropTreeItemClick();
	afx_msg LRESULT OnObjectSelected(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	DECLARE_EVENTSINK_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _SYMBOLDESIGNDLG_H__
