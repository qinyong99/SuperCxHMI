
#ifndef __SAVEPROMPTDLG_H__
#define __SAVEPROMPTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SavePromptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSavePromptDlg dialog

class CSavePromptDlg : public CDialog
{
// Construction
public:
	CSavePromptDlg(CWnd* pParent = NULL);   // standard constructor

	enum enumSaveDocType 
	{ 
		enumSaveDevDoc, 
		enumSaveSubDoc
	};
	class CItemInfo
	{
	public:
		CItemInfo(enumSaveDocType type, DWORD dwData)
		{
			m_type = type;
			m_dwData = dwData;
		}
		enumSaveDocType m_type;
		DWORD m_dwData;
	};
// Dialog Data
	//{{AFX_DATA(CSavePromptDlg)
	enum { IDD = IDD_SAVEPROMPT };
	CListBox	m_ctlListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSavePromptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSavePromptDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnYes();
	afx_msg void OnNo();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SAVEPROMPTDLG_H__)
