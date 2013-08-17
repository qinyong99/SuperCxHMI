// TypeInfoBrowse.h: interface for the CTypeInfoBrowse class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TYPEINFOBROWSE_H__
#define _TYPEINFOBROWSE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../CxDynObjs/PropInfo.h"

class CTypeInfoBrowse  
{
public:
	static enumPropType CrackPropertyType(IUnknown* pUnknown, BSTR bstrPropName, LPARAM lParam); 
	static enumPropType CrackPropertyType(IUnknown* pUnknown, ITypeInfo* pti,
		IPerPropertyBrowsing* ppb, BSTR bstrPropName, LPARAM lParam); 
	
	static HRESULT CrackProperty(IUnknown* pUnknown, CPropInfoArray& arrProp, enumPropType enumType);
	static HRESULT CrackProperty(IUnknown* pUnknown, ITypeInfo* pTypeInfo, 
		IPerPropertyBrowsing* ppb, CPropInfoArray& arrProp, enumPropType enumType);
	static HRESULT CrackExtendProperty(ICxEnumExtendProperty* pEnum, CPropInfoArray& arrProp);
	
	static enumPropType CrackUserDefinedPropertyType(IUnknown* pUnknown, ITypeInfo* pti, HREFTYPE hrt, LPARAM lParam);
	static enumPropType CrackPropertyType(IUnknown* pUnknown, ITypeInfo* pITypeInfo, TYPEDESC& tdesc, LPARAM lParam);
	static enumPropType CrackPerPropertyBrowsingType(IPerPropertyBrowsing* ppb, MEMBERID memid, LPARAM lParam);
};

#endif // _TYPEINFOBROWSE_H__
