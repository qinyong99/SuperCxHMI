// TypeChanger.cpp: implementation of the CTypeChanger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "CxDevIF.h"
#include "TypeChanger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct EXTYPEENTRY
{
	LPTSTR typeName;
	enumPropType type;
};

static struct EXTYPEENTRY staticAryExType[] = 
{
	{_T("颜色"), PROPTYPE_COLOR},	{_T("布尔"), PROPTYPE_BOOL}, 
	{_T("字符串"), PROPTYPE_STRING},{_T("数字"), PROPTYPE_NUMBER},
//	{_T("枚举"), PROPTYPE_ENUM}
};


///////////////////////////////////////////////////////////////
//功能:返回enumType所指扩展属性类型的名称
///////////////////////////////////////////////////////////////
CString CTypeChanger::GetExTypeName(enumPropType enumType)
{
	CString strRet = _T("未知类型");

	int nCount = GetExTypeCount();
	for (int i = 0; i < nCount; i++)
	{
		if (staticAryExType[i].type == enumType)
		{
			strRet = staticAryExType[i].typeName;
			break;
		}
	}

	return strRet;
}

//////////////////////////////////////////////////////////////
//功能:返回支持的扩展属性的总数
//plan, realtime, ctrl, info,
//////////////////////////////////////////////////////////////
int CTypeChanger::GetExTypeCount()
{
	return sizeof(staticAryExType) / sizeof(EXTYPEENTRY);
}

//////////////////////////////////////////////////////////////
//功能:返回第nIndex个扩展属性类型的枚举值
//////////////////////////////////////////////////////////////
enumPropType CTypeChanger::GetExTypeID(int nIndex)
{
	if(nIndex >= GetExTypeCount())
		return PROPTYPE_UNKNOWN;

	return staticAryExType[nIndex].type;
}

//////////////////////////////////////////////////////////////
//功能:由扩展属性类型的名称转化为对象的枚举
//////////////////////////////////////////////////////////////
enumPropType CTypeChanger::GetExType(CString &strTypeName)
{
	int nCount = sizeof(staticAryExType) / sizeof(EXTYPEENTRY);
	for(int i = 0; i < nCount; i++)
	{
		CString str = staticAryExType[i].typeName;
		if(str == strTypeName)
			return staticAryExType[i].type;
	}

	return PROPTYPE_UNKNOWN;
}


HRESULT CTypeChanger::GetPropertyByName(IUnknown* pUnknown, LPCSTR strProp, VARIANT* pVar)
{
	USES_CONVERSION;
	
	IDispatchPtr pDisp = pUnknown;
	
	LPOLESTR lpszName = T2OLE(strProp);
	
	DISPID dwDispID;
	pDisp->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpszName, 1, LOCALE_USER_DEFAULT, &dwDispID);
	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	return pDisp->Invoke(dwDispID, IID_NULL,
		LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
		&dispparamsNoArgs, pVar, NULL, NULL);
}

HRESULT CTypeChanger::PutPropertyByName(IUnknown* pUnknown, LPCSTR strProp, VARIANT* pVar)
{
	USES_CONVERSION;
	
	IDispatchPtr pDisp = pUnknown;
	
	LPOLESTR lpszName = T2OLE(strProp);
	
	DISPID dwDispID;
	pDisp->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpszName, 1, LOCALE_USER_DEFAULT, &dwDispID);
	
	DISPPARAMS dispparams = {NULL, NULL, 1, 1};
	dispparams.rgvarg = pVar;
	DISPID dispidPut = DISPID_PROPERTYPUT;
	dispparams.rgdispidNamedArgs = &dispidPut;
	
	if (pVar->vt == VT_UNKNOWN || pVar->vt == VT_DISPATCH || 
		(pVar->vt & VT_ARRAY) || (pVar->vt & VT_BYREF))
	{
		HRESULT hr = pDisp->Invoke(dwDispID, IID_NULL,
			LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF,
			&dispparams, NULL, NULL, NULL);
		if (SUCCEEDED(hr))
			return hr;
	}
	
	return pDisp->Invoke(dwDispID, IID_NULL,
		LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
		&dispparams, NULL, NULL, NULL);
}