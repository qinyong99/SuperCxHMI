// TagGroupPickDlg.h: interface for the CTagGroupPickDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAGGROUPPICKDLG_H__A4CF7A5B_9D20_456F_B3CB_252D2DE26EB4__INCLUDED_)
#define AFX_TAGGROUPPICKDLG_H__A4CF7A5B_9D20_456F_B3CB_252D2DE26EB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "Node.h"

class CTagGroupPickDlg : public CDialogImpl<CTagGroupPickDlg>
{
public:
	CTagGroupPickDlg()
	{
		m_pRootNode = NULL;
		m_bDisableAssoc = FALSE;
	}
	
	enum {IDD = IDD_TAGGROUP};
	
	BEGIN_MSG_MAP(CTagGroupPickDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_ASSOC, BN_CLICKED, OnClickedAssoc)
		NOTIFY_HANDLER(IDC_GROUP, TVN_ITEMEXPANDING, OnItemexpandingGroup)
		NOTIFY_HANDLER(IDC_GROUP, TVN_SELCHANGED, OnSelchangedGroup)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	LRESULT OnClickedAssoc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnItemexpandingGroup(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelchangedGroup(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	
	void InitGroupTree(BOOL bAssoc);
	void UpdateGroupTree();
	HTREEITEM InsertNode(HTREEITEM hParent, CNode* pNode);
	void ExpandNode(HTREEITEM hItem);
	
	void DeleteAllChildItem(HTREEITEM hItem);
	CString GetTagGroup(HTREEITEM hItem);
	
	void OnFinalMessage(HWND hWnd);
	
public:
	CRootNode* m_pRootNode;
	
	CComBSTR m_bstrAssocGroup;
	CString m_strPickGroup;

	BOOL m_bDisableAssoc;
	
	CTreeViewCtrl m_tree;   //±êÇ©×é
	CImageList m_ImageList;
	
	CButton m_btnAssoc;
	CEdit m_editSel;
};

#endif // !defined(AFX_TAGGROUPPICKDLG_H__A4CF7A5B_9D20_456F_B3CB_252D2DE26EB4__INCLUDED_)
