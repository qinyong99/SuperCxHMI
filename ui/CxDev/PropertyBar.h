// PropertyBar.h: interface for the CPropertyBar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PROPERTYBAR_H__
#define __PROPERTYBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\CxSCBar\SCBEXT.h"
interface IPropertyCtrl;

class CLayoutView;

class CPropertyBar : public CSizingControlBar  
{
public:
	CPropertyBar();
	virtual ~CPropertyBar();

// Attributes
public:
	CWnd m_wndPropertyCtrl;
	IPropertyCtrl* m_pPropertyCtrl;
	IOleInPlaceActiveObject* m_pActiveObject;

	CString m_strObjOldName;

	CAction* m_pLastAction;

public:
	void OnComboSelchange();
	void OnComboDropDown();
//	void OnSelNameChange(LPCTSTR lNewValue);
	void OnShowPropPages();
	void OnPropertyChanging(DISPID dispid, BOOL* pbCancel);
	void OnPropertyChanged(DISPID dispid);
	void OnGetExpressionCreatorProc(long lProc);

	CLayoutView* GetActiveLayoutView();
	
	CDocument* GetDocument() 
	{ 
		CMDIFrameWnd* pParentFrame = (CMDIFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd* pMDIActive = pParentFrame->MDIGetActive();
		if (pMDIActive == NULL)
			return NULL;
		return pMDIActive->GetActiveDocument();
	}

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CPropertyBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnSelectChangedMsg(DWORD dwNotify, BOOL bUpdateOnly);
    DECLARE_MESSAGE_MAP()

	DECLARE_EVENTSINK_MAP()

private:
	
};

#endif // !__PROPERTYBAR_H__
