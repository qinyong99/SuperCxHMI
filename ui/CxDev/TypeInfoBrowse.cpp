// TypeInfoBrowse.cpp: implementation of the CTypeInfoBrowse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "TypeInfoBrowse.h"

//////////////////////////////////////////////////////////////////
//功能：得到指定类型信息接口指定属性名称的类型
//参数：0		类型接口类指针
//	1			属性名称
//返回：类型
///////////////////////////////////////////////////////////////////
enumPropType CTypeInfoBrowse::CrackPropertyType(IUnknown* pUnknown, ITypeInfo* pTypeInfo, 
	IPerPropertyBrowsing* ppb, BSTR bstrPropName, LPARAM lParam)
{
	enumPropType type = PROPTYPE_UNKNOWN;

	TYPEATTR* pta = NULL;
	HRESULT hr = pTypeInfo->GetTypeAttr(&pta);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < pta->cFuncs; i++)
		{
			FUNCDESC* pfd = NULL;
			pTypeInfo->GetFuncDesc(i, &pfd);
			if (pfd && (pfd->invkind == INVOKE_PROPERTYPUT || pfd->invkind == INVOKE_PROPERTYPUTREF)
				&& ((pfd->wFuncFlags & (FUNCFLAG_FRESTRICTED | FUNCFLAG_FHIDDEN | FUNCFLAG_FNONBROWSABLE)) == 0))
			{
				CComBSTR bstrName;
				pTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, NULL, NULL);
										
				if (bstrName == bstrPropName)
				{
					type = PROPTYPE_UNKNOWN;
					if (ppb != NULL)
		 				type = CrackPerPropertyBrowsingType(ppb, pfd->memid, lParam);
					if (type == PROPTYPE_UNKNOWN)
						type = CrackPropertyType(pUnknown, pTypeInfo, pfd->lprgelemdescParam[0].tdesc, lParam);
					pTypeInfo->ReleaseFuncDesc(pfd);
					return type;
				}
			}
			pTypeInfo->ReleaseFuncDesc(pfd);
		}
		
		for (i = 0; i < pta->cVars; i++)
		{
			VARDESC* pvd = NULL;
			pTypeInfo->GetVarDesc(i, &pvd);
			
			if(((pvd->wVarFlags & VARFLAG_FREADONLY) == 0) && 
				((pvd->wVarFlags & (VARFLAG_FHIDDEN | VARFLAG_FNONBROWSABLE)) == 0))
			{
				CComBSTR bstrName;
				pTypeInfo->GetDocumentation(pvd->memid, &bstrName, NULL, 0, 0);
				CString strPropName = bstrName;
							
				if (bstrName == bstrPropName)
				{
					type = PROPTYPE_UNKNOWN;
					if (ppb != NULL)
						type = CrackPerPropertyBrowsingType(ppb, pvd->memid, lParam);
					if (type == PROPTYPE_UNKNOWN)
						type = CrackPropertyType(pUnknown, pTypeInfo, pvd->elemdescVar.tdesc, lParam);
					pTypeInfo->ReleaseVarDesc(pvd);	
					return type;
				}
			}
			pTypeInfo->ReleaseVarDesc(pvd);	
		}
	}
	
	return type;	
}

enumPropType CTypeInfoBrowse::CrackPerPropertyBrowsingType(IPerPropertyBrowsing* ppb, MEMBERID memid, LPARAM lParam)
{
	CALPOLESTR castr;
	CADWORD cadw; 
	
	memset(&castr, 0, sizeof(castr));
	memset(&cadw, 0, sizeof(cadw));

	enumPropType type = PROPTYPE_UNKNOWN;
	
	HRESULT hr = ppb->GetPredefinedStrings(memid, &castr, &cadw);
	if (FAILED(hr)) 
		return type;

	if (lParam != NULL)
	{
		CEnumPropItemArray* pArray = new CEnumPropItemArray;
		
		for (ULONG i = 0; i < cadw.cElems; i++)
		{
			CComVariant var;			
			hr = ppb->GetPredefinedValue(memid, cadw.pElems[i], &var);			
			if (FAILED(hr)) 
				continue;
			
			CEnumPropItem item;
			item.m_bstrName = castr.pElems[i];
			item.m_bstrDesc = item.m_bstrName;
			item.m_varValue = var;
			
			pArray->Add(item);
		}
		
		if (castr.cElems > 0)
		{
			for (ULONG i = 0; i < castr.cElems; i++)
				CoTaskMemFree((void *)castr.pElems[i]);  
			CoTaskMemFree((void *)castr.pElems); 			
		}
		if (cadw.cElems > 0)
		{			
			CoTaskMemFree((void *)cadw.pElems);			
		}
		
		CEnumPropItemArray** pArrEnumItem = (CEnumPropItemArray**)lParam;
		*pArrEnumItem = pArray;
	}
	
	return PROPTYPE_ENUM;
}

enumPropType CTypeInfoBrowse::CrackPropertyType(IUnknown* pUnknown, ITypeInfo* pti, TYPEDESC& tdesc, LPARAM lParam)
{
	switch (tdesc.vt)
	{	 
		case VT_EMPTY: 
		case VT_NULL:
		case VT_ERROR:			
			return PROPTYPE_UNKNOWN;
		case VT_PTR:	
			return CrackPropertyType(pUnknown, pti, *tdesc.lptdesc, lParam);
		case VT_USERDEFINED:	
			return CrackUserDefinedPropertyType(pUnknown, pti, tdesc.hreftype, lParam);
			break;
		
		case VT_BSTR:
			return PROPTYPE_STRING;
			break;
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
		case VT_CY:			
		case VT_VARIANT:
		case VT_DECIMAL:
		case VT_I1:
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_INT:
		case VT_UINT:  								
			return PROPTYPE_NUMBER;
			break;

		case VT_BOOL:  
			return PROPTYPE_BOOL;
			break;
				
		default: 
			return PROPTYPE_UNKNOWN; 
	}	
	
	return PROPTYPE_UNKNOWN; 
}

#include <initguid.h>
#include "../CxPropCtrl/PropGuids.h"

enumPropType CTypeInfoBrowse::CrackUserDefinedPropertyType(IUnknown* pUnknown, ITypeInfo* pti, HREFTYPE hrt, LPARAM lParam)
{
	CComPtr<ITypeInfo> pti2;
	HRESULT hr;
	enumPropType type = PROPTYPE_UNKNOWN;

	hr = pti->GetRefTypeInfo(hrt, &pti2);
	if (FAILED(hr))
		return type;

	TYPEATTR* pta = 0;
	hr = pti2->GetTypeAttr(&pta);
	if (FAILED(hr))	
		return type;

	switch(pta->typekind)
	{
		case TKIND_ALIAS:
			if (pta->tdescAlias.vt == VT_USERDEFINED)
			{	
				type = CrackUserDefinedPropertyType(pUnknown, pti2, pta->tdescAlias.hreftype, lParam);							
			}			
			else if(InlineIsEqualGUID(pta->guid, GUID_COLOR))
			{
				type = PROPTYPE_COLOR;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_FILE))
			{
				type = PROPTYPE_FAKEFILE;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_BRUSHFILE))
			{
				type = PROPTYPE_FAKEFILE;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_TAGGROUPTYPE))
			{
				type = PROPTYPE_TAGGROUP;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_SECURITYAREATYPE))
			{
				type = PROPTYPE_SECURITYAREA;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_FONTNAME))
			{
				type = PROPTYPE_FAKEFONTNAME;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_FOLDER))
			{
				type = PROPTYPE_FAKEFOLDER; 
			}			
			else if(InlineIsEqualGUID(pta->guid, GUID_CXVARIABLETYPE))
			{ 
				IDispatchPtr pDispatch = pUnknown;

				COleDispatchDriver dd;
				dd.AttachDispatch(pDispatch, FALSE);
				
				enumPropertyType nType;
				dd.GetProperty(CXPROPID_VariableType, VT_I4, (void*)&nType);

				switch (nType)
				{
				case PropertyTypeColor:
					type = PROPTYPE_COLOR;
					break;
				case PropertyTypeBool:
					type = PROPTYPE_BOOL;
					break;
				default:
				case PropertyTypeString:
					type = PROPTYPE_STRING;
					break;
				case PropertyTypeNumeric:
					type = PROPTYPE_NUMBER;
					break;
				}	
				
				hr = S_OK;
			}
			else
			{				
				type = CrackPropertyType(pUnknown, pti, pta->tdescAlias, lParam);
			}								
			break;

	 case TKIND_ENUM:
		if (lParam != NULL)
		{
			CEnumPropItemArray* pArray = new CEnumPropItemArray;
			VARDESC* pvd = 0;
			WCHAR szBuf[128];
			for (int i = 0; i < pta->cVars; i++)
			{
				pti2->GetVarDesc(i, &pvd);				
				CComBSTR bstrName;
				CComBSTR bstrDocString;
				pti2->GetDocumentation(pvd->memid, &bstrName, &bstrDocString, 0, 0);

				CEnumPropItem item;
				swprintf(szBuf, L"%d - %s", pvd->lpvarValue->lVal, bstrName);
				item.m_bstrName = szBuf;
				swprintf(szBuf, L"%d - %s", pvd->lpvarValue->lVal, bstrDocString);
				item.m_bstrDesc = szBuf;
				item.m_varValue = *pvd->lpvarValue;
				pArray->Add(item);		
				
				pti2->ReleaseVarDesc(pvd);
			}
		
			CEnumPropItemArray** pArrEnumItem = (CEnumPropItemArray**)lParam;
			*pArrEnumItem = pArray;
		 }
		type = PROPTYPE_ENUM; 
		break;
	case TKIND_INTERFACE:
	case TKIND_DISPATCH:
		if (InlineIsEqualGUID(pta->guid, IID_IFontDisp) ||			
			InlineIsEqualGUID(pta->guid, IID_IFont) )
		{
			type = PROPTYPE_FAKEFONT;	
		}
		else if(InlineIsEqualGUID(pta->guid, IID_IPictureDisp)
			|| InlineIsEqualGUID(pta->guid, IID_IPicture) )
		{
			type = PROPTYPE_FAKEPICTURE;
		}		
		break;
		
	default:	//TKIND_COCLASS, TKIND_RECORD, TKIND_UNION
		break;
	}

	pti2->ReleaseTypeAttr(pta);

	return type;
}

//////////////////////////////////////////////////////////////////
//功能：得到指定类型信息接口指定属性名称的类型
//参数：0		类型接口类指针
//	1			属性名称
//  2  要枚举的属性类型，PROPTYPE_UNKNOWN为所有类型
//返回：类型
///////////////////////////////////////////////////////////////////
HRESULT CTypeInfoBrowse::CrackProperty(IUnknown* pUnknown, ITypeInfo* pTypeInfo,
	IPerPropertyBrowsing* ppb, CPropInfoArray& arrProp, enumPropType enumType)
{
	enumPropType type;

	TYPEATTR* pta = NULL;
	HRESULT hr = pTypeInfo->GetTypeAttr(&pta);
	if (FAILED(hr))
		return hr;
		
	CPropInfo prop;
	for (int i = 0; i < pta->cFuncs; i++)
	{
		FUNCDESC* pfd = NULL;
		pTypeInfo->GetFuncDesc(i, &pfd);
		if (pfd && (pfd->invkind == INVOKE_PROPERTYPUT || pfd->invkind == INVOKE_PROPERTYPUTREF)
			&& ((pfd->wFuncFlags & (FUNCFLAG_FRESTRICTED | FUNCFLAG_FHIDDEN | FUNCFLAG_FNONBROWSABLE)) == 0))
		{
			type = PROPTYPE_UNKNOWN;
			if (ppb != NULL)
				type = CrackPerPropertyBrowsingType(ppb, pfd->memid, NULL);
			if (type == PROPTYPE_UNKNOWN)
				type = CrackPropertyType(pUnknown, pTypeInfo, pfd->lprgelemdescParam[0].tdesc, NULL);
			if (type != PROPTYPE_UNKNOWN && (type == enumType || enumType == PROPTYPE_UNKNOWN))
			{
				CComBSTR bstrName;
				CComBSTR bstrDocString;
				pTypeInfo->GetDocumentation(pfd->memid, &bstrName, &bstrDocString, NULL, NULL);
				
				prop.m_bstrName = bstrName;				
				prop.m_bstrDesc = bstrDocString;
				prop.m_proptype = type;
				prop.m_dispid = pfd->memid;
				arrProp.Add(prop);
			}
		}
		pTypeInfo->ReleaseFuncDesc(pfd);
	}

	for (i = 0; i < pta->cVars; i++)
	{
		VARDESC* pvd = NULL;
		pTypeInfo->GetVarDesc(i, &pvd);

		if(((pvd->wVarFlags & VARFLAG_FREADONLY) == 0) && 
			((pvd->wVarFlags & (VARFLAG_FHIDDEN | VARFLAG_FNONBROWSABLE)) == 0))
		{
			type = PROPTYPE_UNKNOWN;
			if (ppb != NULL)
				type = CrackPerPropertyBrowsingType(ppb, pvd->memid, NULL);
			if (type == PROPTYPE_UNKNOWN)
				type = CrackPropertyType(pUnknown, pTypeInfo, pvd->elemdescVar.tdesc, NULL);
			if (type != PROPTYPE_UNKNOWN && (type == enumType || enumType == PROPTYPE_UNKNOWN))
			{
				CComBSTR bstrName;
				CComBSTR bstrDocString;
				pTypeInfo->GetDocumentation(pvd->memid, &bstrName, &bstrDocString, NULL, NULL);
				
				prop.m_bstrName = bstrName;				
				prop.m_bstrDesc = bstrDocString;
				prop.m_proptype = type;
				prop.m_dispid = pvd->memid;
				arrProp.Add(prop);
			}
		}
		pTypeInfo->ReleaseVarDesc(pvd);	
	}

	pTypeInfo->ReleaseTypeAttr(pta);

	return S_OK;	
}

HRESULT CTypeInfoBrowse::CrackExtendProperty(ICxEnumExtendProperty* pEnum, CPropInfoArray& arrProp)
{
	LONG nIndex;
	HRESULT hr;
	CPropInfo prop;

	pEnum->Reset();
	while (S_FALSE != pEnum->Next(&nIndex))
	{
		hr = pEnum->GetType((LONG *)&prop.m_proptype);
		if (FAILED(hr))
			return hr;
			
		CComBSTR bstr;
		hr = pEnum->GetName(&bstr);
		if (FAILED(hr))
			return hr;
		prop.m_bstrName = bstr;

		bstr.Empty();
		hr = pEnum->GetDescription(&bstr);
		if (FAILED(hr))
			return hr;
		prop.m_bstrDesc = bstr;
	
		hr = pEnum->GetDispid(&prop.m_dispid);
		if (FAILED(hr))
			return hr;
			
		arrProp.Add(prop);
	}
	
	return S_OK;
}

HRESULT CTypeInfoBrowse::CrackProperty(IUnknown* pUnknown, CPropInfoArray& arrProp, enumPropType enumType)
{
	if (pUnknown == NULL)
		return E_INVALIDARG; 

	CComQIPtr<IPerPropertyBrowsing> ppb(pUnknown);

	CComQIPtr<IDispatch> spDisp = pUnknown;
	UINT nInfoCount = 0;
	HRESULT hr = spDisp->GetTypeInfoCount(&nInfoCount);
	if (FAILED(hr))
		return hr;

	CComPtr<ITypeInfo> pti;	
	for (UINT i = 0; i < nInfoCount; i++)
	{
		hr = spDisp->GetTypeInfo(i, LOCALE_SYSTEM_DEFAULT, &pti);	
		if (FAILED(hr)) 
			return hr;
		if (pti == NULL) 
			return E_UNEXPECTED;

		CrackProperty(pUnknown, pti, ppb, arrProp, enumType);

		pti.Release();
	}

	return S_OK;
}

enumPropType CTypeInfoBrowse::CrackPropertyType(IUnknown* pUnknown, BSTR bstrPropName, LPARAM lParam)
{
	enumPropType type = PROPTYPE_UNKNOWN;

	if (pUnknown == NULL)
		return type; 

	CComQIPtr<IPerPropertyBrowsing> ppb(pUnknown);

	CComQIPtr<IDispatch> spDisp = pUnknown;
	UINT nInfoCount = 0;
	HRESULT hr = spDisp->GetTypeInfoCount(&nInfoCount);
	if (FAILED(hr))
		return type;
	
	CComPtr<ITypeInfo> pti;	
	for (UINT i = 0; i < nInfoCount; i++)
	{
		hr = spDisp->GetTypeInfo(i, LOCALE_SYSTEM_DEFAULT, &pti);	
		if (FAILED(hr)) 
			return type;
		if (pti == NULL) 
			return type;

		type = CrackPropertyType(pUnknown, pti, ppb, bstrPropName, lParam);
		if (type != PROPTYPE_UNKNOWN)
			break;

		pti.Release();
	}

	return type;
}
