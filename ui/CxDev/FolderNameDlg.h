
#ifndef __FOLDERNAMEDLG_H__
#define __FOLDERNAMEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderNameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderNameDlg dialog

class CFolderNameDlg : public CDialog
{
// Construction
public:
	CString m_strTitle;
	CFolderNameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFolderNameDlg)
	enum { IDD = IDD_FOLDERNAME };
	CString	m_strFolderName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFolderNameDlg)
	afx_msg void OnChangeFoldername();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__FOLDERNAMEDLG_H__)
