//////////////////////////////////////////////////////////////////////
// SymbolLibDoc.h : header file

#if !defined(AFX_SymbolLibDOC_H__4FCC3736_0B44_407D_8B66_0D141A44A901__INCLUDED_)
#define AFX_SymbolLibDOC_H__4FCC3736_0B44_407D_8B66_0D141A44A901__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDevObj;

/////////////////////////////////////////////////////////////////////////////
// CSymbolLibDoc document

class CSymbolLibDoc : public CDevObjDoc
{
public:
	CSymbolLibDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSymbolLibDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolLibDoc)
	public:
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CDevObj* ConvertObjectToSymbol(CDevObj* pObj);
	
	BOOL DeleteObject(CDevObj* pObj);
	static BOOL DeleteLib(CString& strLibName);
	int GetObjectCount();
	BOOL CloseLib();
	static LPTSTR m_szGLibFileExt;
	static CSymbolLibDoc* NewLib(CString& strName);
	static void RenameLib(LPCTSTR lpszOldName, LPCTSTR lpszNewName);

	BOOL SaveLib();
	BOOL OpenLib(CString strLibName);
	static CString GetDefaultPath();
	virtual ~CSymbolLibDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	//{{AFX_MSG(CSymbolLibDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SymbolLibDOC_H__4FCC3736_0B44_407D_8B66_0D141A44A901__INCLUDED_)
