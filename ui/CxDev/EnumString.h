#if !defined(AFX_ENUMSTRING_H__038DB224_FAC2_4831_A794_9167EBC1A0C2__INCLUDED_)
#define AFX_ENUMSTRING_H__038DB224_FAC2_4831_A794_9167EBC1A0C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnumString.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CEnumString command target

class CEnumString : public CCmdTarget
{
	CEnumString(LPOLESTR* pStrings, int nSize);  
	CEnumString(CStringArray& arrStrings);

// Attributes
public:
	int m_nCurPos; 
	LPOLESTR* m_pStrings;
	int m_nSize;

// Operations
public:
	static HRESULT CreateEnumString(CStringArray& arrStrings, IEnumString** ppEnumString);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumString)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEnumString();

	BEGIN_INTERFACE_PART(EnumString, IEnumString)
		STDMETHOD(Next)(ULONG celt, LPOLESTR* rgelt, ULONG* pceltFetched);
		STDMETHOD(Skip)(ULONG celt);
		STDMETHOD(Reset)();
		STDMETHOD(Clone)(IEnumString **ppenum);
	END_INTERFACE_PART(EnumString)
		
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMSTRING_H__038DB224_FAC2_4831_A794_9167EBC1A0C2__INCLUDED_)
