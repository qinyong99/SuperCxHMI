#if !defined(AFX_OBJECTCOLLECTION_H__B96A5D72_8A7D_4457_B2EE_9321B33339AD__INCLUDED_)
#define AFX_OBJECTCOLLECTION_H__B96A5D72_8A7D_4457_B2EE_9321B33339AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectCollection.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CObjectCollection command target

class CObjectCollection : public CCmdTarget
{
	DECLARE_DYNCREATE(CObjectCollection)

	CObjectCollection();           // protected constructor used by dynamic creation

// Attributes
public:
	CArray<CComVariant, CComVariant> m_arrObjects;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringCollect)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CObjectCollection();

	// Generated message map functions
	//{{AFX_MSG(CObjectCollection)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CObjectCollection)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CStringCollect)
	afx_msg long GetCount();
	afx_msg VARIANT GetItem(long nIndex);
	//}}AFX_DISPATCH
	afx_msg LPUNKNOWN GetNewEnum();

	DECLARE_DISPATCH_MAP()

	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTCOLLECTION_H__B96A5D72_8A7D_4457_B2EE_9321B33339AD__INCLUDED_)
