// CodeModuleDoc.h: interface for the CCodeModuleDoc class.
//功能: 代码模块文档
//制作: 孙越洋
//所属工程: CxDev
//时间: 2002-3=26
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEMODULEDOC_H__182753BE_3098_49BA_B778_A5439034EDDE__INCLUDED_)
#define AFX_CODEMODULEDOC_H__182753BE_3098_49BA_B778_A5439034EDDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevDoc.h"

#endif // !defined(AFX_CODEMODULEDOC_H__182753BE_3098_49BA_B778_A5439034EDDE__INCLUDED_)
/////////////////////////////////////////////////////////////////////////////
// CCodeModuleDoc document

class CCodeModuleDoc : public CFormDoc
{
public:
	class CCodeSubDoc : public CFormDoc::CFormSubDoc
	{
	public:
		CCodeSubDoc();

	public:
		virtual CDevDoc::SUBDOCTYPE GetType() { return CDevDoc::enumCodeDoc; }

		virtual CDocument* LoadChildDocumentFromFile(LPTSTR fileName);
		virtual CDocument* CreateChildDocument(CDevDoc* pDoc);
		virtual CDocument* LoadChildDocument(BOOL bInitFrame = TRUE);
	}* m_pMainDocRecord;

protected:
	CCodeModuleDoc(CDevDoc* pDoc = NULL);           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCodeModuleDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeModuleDoc)
	public:
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnActivate();
	virtual ~CCodeModuleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeModuleDoc)
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CDevDoc* m_pParentDoc;

	friend class CCodeModuleDoc::CCodeSubDoc;
};
