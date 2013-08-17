// RunDoc.h : interface of the CRunDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDOC_H__75BA333A_8194_43D3_968D_F29AF5CB12FD__INCLUDED_)
#define AFX_RUNDOC_H__75BA333A_8194_43D3_968D_F29AF5CB12FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRunDoc : public CDocument
{
protected: // create from serialization only
	CRunDoc();
	DECLARE_DYNCREATE(CRunDoc)

// Attributes
public:
	BOOL m_bLockedExternal;
	DWORD m_dwCookie;

	BOOL m_bOpened;
	
// Operations
public:
	void RegisterAsActive();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);

// Implementation
public:
	virtual ~CRunDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRunDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CRunDoc)
	afx_msg void OpenStartPicture();
	afx_msg void OpenPicture(LPCTSTR lpszPathName);
	afx_msg BOOL GetVisible();
	afx_msg void SetVisible(BOOL bNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNDOC_H__75BA333A_8194_43D3_968D_F29AF5CB12FD__INCLUDED_)
