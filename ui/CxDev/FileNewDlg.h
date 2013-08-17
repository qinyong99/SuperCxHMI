#if !defined(AFX_FILENEWDLG_H__95E4297E_0255_47C2_A4F8_DE7E76DE2AE0__INCLUDED_)
#define AFX_FILENEWDLG_H__95E4297E_0255_47C2_A4F8_DE7E76DE2AE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileNewDlg dialog

class CFileNewDlg : public CDialog
{
// Construction
public:
	CFileNewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileNewDlg)
	enum { IDD = IDD_FILE_NEW };
	CString	m_strName;
	CString	m_strDesc;
	int		m_nWidth;
	int		m_nHeight;
	CString	m_strBrushFile;
	BOOL	m_bNextShow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileNewDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENEWDLG_H__95E4297E_0255_47C2_A4F8_DE7E76DE2AE0__INCLUDED_)
