// TypeChanger.h: interface for the CTypeChanger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYPECHANGER_H__BD0324F1_1BD3_4509_9B9B_218DDF9120F8__INCLUDED_)
#define AFX_TYPECHANGER_H__BD0324F1_1BD3_4509_9B9B_218DDF9120F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTypeChanger  
{
public:
	static HRESULT GetPropertyByName(IUnknown* pUnknown, LPCSTR strProp, VARIANT* pVar);
	static HRESULT PutPropertyByName(IUnknown* pUnknown, LPCSTR strProp, VARIANT* pVar);

	static CString GetExTypeName(enumPropType enumType);
	static int GetExTypeCount();
	static enumPropType GetExTypeID(int nIndex);
	static enumPropType GetExType(CString &strTypeName);
};

#endif // !defined(AFX_TYPECHANGER_H__BD0324F1_1BD3_4509_9B9B_218DDF9120F8__INCLUDED_)
