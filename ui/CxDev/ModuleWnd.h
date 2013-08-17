#if !defined(AFX_MODULEWND_H__435A9AD8_5B7F_49BA_A84F_8E5F23A182A6__INCLUDED_)
#define AFX_MODULEWND_H__435A9AD8_5B7F_49BA_A84F_8E5F23A182A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
////////////////////////////////////////////////////////////////////
// ModuleWnd.h : header file
// 功能:模块工具窗口类
// 所属:AutoCX中的CxDev工程
// 作者:孙越洋
// 时间:2001.11.30
// 说明:在工具栏中运用, 用于用户使用模块库
////////////////////////////////////////////////////////////////////

class CBarListCtrl;
class CModuleDoc;
class CFormDoc;

/////////////////////////////////////////////////////////////////////////////
// CModuleWnd window
class CModuleWnd : public CBarListCtrl
{
// Construction
public:
	CModuleWnd();
	virtual ~CModuleWnd();

// Attributes
public:
	BOOL m_isDragBeginer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CanAddModule();
	void AddFormModule(CFormDoc* pFormDoc);

	void OnBackToRoot();

	COleDataSource* PrepareDataSource();
	BOOL GetSelectModuleData(LPSTGMEDIUM lpStgMedium);
	void GetDragInfoData(LPSTGMEDIUM lpStgMedium);

	void LibDelete(BOOL bPrompt = TRUE);

	// Generated message map functions
protected:
	void OnUpdate(void);
	virtual DROPEFFECT FindDragEffect(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL DoDrop(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);
	virtual void DoCommand(int nButton);
	//{{AFX_MSG(CModuleWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLibNew();
	afx_msg void OnLibRename();
	afx_msg void OnLibDelete();
	afx_msg void OnLibOpen();
	afx_msg void OnLibCut();
	afx_msg void OnLibCopy();
	afx_msg void OnLibPaste();
	afx_msg void OnListKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemdblclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnAddForm();
	afx_msg void OnAddSubmodule();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	CModuleDoc* m_pCurrentDocument;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULEWND_H__435A9AD8_5B7F_49BA_A84F_8E5F23A182A6__INCLUDED_)
