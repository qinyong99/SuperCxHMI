// BitmapDoc.h : interface of the CBitmapDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPDOC_H__485C8F84_BB54_46E9_8E9C_77494D80F689__INCLUDED_)
#define AFX_BITMAPDOC_H__485C8F84_BB54_46E9_8E9C_77494D80F689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBitmapDoc : public CDocument
{
public: // create from serialization only
	CBitmapDoc();
	DECLARE_DYNCREATE(CBitmapDoc)

// Attributes
public:
	CBitmap m_bitmap;
	CRect m_rcMargin;
	float m_fHorizontalScale;
	float m_fVerticalScale;
	BOOL m_bMultipleScale;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitmapDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBitmapDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPDOC_H__485C8F84_BB54_46E9_8E9C_77494D80F689__INCLUDED_)
