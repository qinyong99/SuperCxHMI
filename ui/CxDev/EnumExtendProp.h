#ifndef __ENUMEXTENDPROP_H__
#define __ENUMEXTENDPROP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnumExtendProp.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CEnumExtendProp command target

class CProp
{
public:
	CString m_strName;
	CString m_strDescription;
	enumPropType m_proptype;
	DISPID m_dispid;
	_variant_t m_value;
public:
	CProp()
	{
		m_dispid = 0;
		m_proptype = PROPTYPE_UNKNOWN;
	}

	CProp& operator = (CProp& source)
	{
		m_strName = source.m_strName;
		m_strDescription = source.m_strDescription;
		m_proptype = source.m_proptype;
		m_dispid = source.m_dispid;

		return *this;
	}
};

class CEnumExtendProp : public CCmdTarget
{
public:
	CEnumExtendProp(CProp* pProps, UINT nSize);           // protected constructor used by dynamic creation
	virtual ~CEnumExtendProp();

// Attributes
public:
	int m_nCurPos; 
	CProp* m_pProps;
	int m_nSize;

// Operations
public:
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumExtendProp)
	//}}AFX_VIRTUAL

// Implementation
public:
	BEGIN_INTERFACE_PART(EnumProp, ICxEnumExtendProperty)
		STDMETHOD(GetName)(BSTR *pbstrName);
		STDMETHOD(GetDescription)(BSTR *pbstrDescription);
		STDMETHOD(GetType)(LONG *pIndexed);
		STDMETHOD(GetDispid)(DISPID *pDispid);
		STDMETHOD(Reset)();
		STDMETHOD(Next)(LONG *pIndex);
		STDMETHOD(Skip)(LONG celt);
		STDMETHOD(Clone)(ICxEnumExtendProperty **ppenum);
	END_INTERFACE_PART(EnumProp)

	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ENUMEXTENDPROP_H__)
