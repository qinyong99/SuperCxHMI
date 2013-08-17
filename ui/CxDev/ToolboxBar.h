// ToolboxBar.h: interface for the CExplorerBar class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __TOOLBOXBAR_H__
#define __TOOLBOXBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\CxSCBar\SCBEXT.h"

class CToolboxBar : public CSizingControlBar  
{
public:
	CToolboxBar();
	virtual ~CToolboxBar();

// Attributes
public:
	CBoxCtrl m_wndBoxCtrl;
	//CExplorerTree m_wndExplorerTree;

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
	virtual void NcPaintGripper(CDC* pDC, CRect rcClient);


protected:
    //{{AFX_MSG(CToolboxBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp); 
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // !__TOOLBOXBAR_H__
