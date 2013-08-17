#if !defined(AFX_SERVERPROMPTMODIFIEDDLG_H__A030BA58_EB03_4EA8_AEFD_CCC8A1473865__INCLUDED_)
#define AFX_SERVERPROMPTMODIFIEDDLG_H__A030BA58_EB03_4EA8_AEFD_CCC8A1473865__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerPromptModifiedDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerPromptModifiedDlg dialog

class CServerPromptModifiedDlg : public CDialog
{
// Construction
public:
	CServerPromptModifiedDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerPromptModifiedDlg)
	enum { IDD = IDD_PROMPT_MODIFIED };
	BOOL	m_bNotPrompt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerPromptModifiedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerPromptModifiedDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERPROMPTMODIFIEDDLG_H__A030BA58_EB03_4EA8_AEFD_CCC8A1473865__INCLUDED_)
