#ifndef _RETURNVALUE_H__
#define _RETURNVALUE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReturnValueA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReturnValue command target

class CReturnValue : public CCmdTarget
{
	DECLARE_DYNCREATE(CReturnValue)

	CReturnValue();           // protected constructor used by dynamic creation

// Attributes
public:
	CComVariant m_vaValue;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReturnValue)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReturnValue();

protected:
	// Generated message map functions
	//{{AFX_MSG(CReturnValue)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CReturnValue)
	afx_msg VARIANT GetValue();
	afx_msg void SetValue(const VARIANT FAR& newValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	//扩展自动化接口
	BEGIN_INTERFACE_PART(DispatchExtend, IDispatch)
		STDMETHOD(GetIDsOfNames)(REFIID iid, LPOLESTR* ppszNames, UINT nNames,
			LCID lcid, DISPID* pDispIDs);
		STDMETHOD(GetTypeInfo)(UINT iTypeInfo, LCID lcid,
			ITypeInfo** ppTypeInfo);
		STDMETHOD(GetTypeInfoCount)(UINT* pnInfoCount);
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID iid, LCID lcid,
			WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
			EXCEPINFO* pExceptionInfo, UINT* piArgError);
	END_INTERFACE_PART(DispatchExtend)

	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _RETURNVALUE_H__
