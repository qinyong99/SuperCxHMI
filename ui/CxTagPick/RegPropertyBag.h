// RegPropertyBag.h: interface for the CRegPropertyBag class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _REGPROPERTYBAG_H__
#define _REGPROPERTYBAG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RegProfile.h"

class ATL_NO_VTABLE CRegPropertyBag : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CRegProfile,
	public IPropertyBag 
{
public:
	CRegPropertyBag(LPCTSTR lpszSection = NULL)
	{
		if (lpszSection != NULL	)
			m_pszSection = _tcsdup(lpszSection);
		else
			m_pszSection = _tcsdup(_T("PropertyBag"));
	}

	virtual ~CRegPropertyBag()
	{
		free((void*)m_pszSection);
	}

	class CPropertyBagItem
	{
	public:
		CPropertyBagItem()
		{
		}
		CPropertyBagItem(const CPropertyBagItem& item)
		{
			InternalCopy(item);
		}
		CPropertyBagItem& operator=(const CPropertyBagItem& item)
		{
			InternalCopy(item);
			return *this;
		}
		void InternalCopy(const CPropertyBagItem& item)
		{
			m_bstrName = item.m_bstrName;
			m_varValue = item.m_varValue;
		}

	public:
		CComBSTR m_bstrName;
		CComVariant m_varValue;
	};

BEGIN_COM_MAP(CRegPropertyBag)
	COM_INTERFACE_ENTRY(IPropertyBag)
END_COM_MAP()

// IPropertyBag
public:
	STDMETHOD(Read)(LPCOLESTR pszPropName, VARIANT* pvarValue, IErrorLog* pErrorLog)
	{
		USES_CONVERSION;
		HRESULT hr;
		EXCEPINFO error;

		if (pszPropName == NULL)
			return E_POINTER;
		if (pvarValue == NULL)
			return E_POINTER;

		memset(&error, 0, sizeof(error));

		for (int i = 0; i < m_arrItems.GetSize(); i++)
		{
			CPropertyBagItem& item = m_arrItems[i];

			if (item.m_bstrName == pszPropName)
			{
				if (pvarValue->vt == VT_EMPTY)
					hr = VariantCopy(pvarValue, &item.m_varValue);
				else
					hr = VariantChangeType(pvarValue, &item.m_varValue, 0, pvarValue->vt);

				if (FAILED(hr))
				{
				   if (pErrorLog != NULL)
				   {
					  error.scode = hr;
					  pErrorLog->AddError(pszPropName, &error);
				   }
					
				   return E_FAIL;
				}

				return S_OK;
			}
		}

		return E_INVALIDARG;
	}

	STDMETHOD(Write)(LPCOLESTR pszPropName, VARIANT* pvarValue)
	{
		if (pszPropName == NULL || pvarValue == NULL)
			return E_POINTER;
 
		CPropertyBagItem item;
		item.m_bstrName = pszPropName;
		item.m_varValue = *pvarValue;
	 	
		m_arrItems.Add(item);

		return S_OK;	
	}

	void SaveToRegistry()
	{
		USES_CONVERSION;
		TCHAR szEntry[256];

		// 清除整个区块
		WriteProfileString(m_pszSection, NULL, NULL);

		int nCount = m_arrItems.GetSize();
		WriteProfileInt(m_pszSection, _T("ItemCount"), nCount);

		for (int i = 0; i < nCount; i++)
		{
			CPropertyBagItem& item = m_arrItems[i];
		
			wsprintf(szEntry, _T("ItemName%d"), i);
			WriteProfileString(m_pszSection, szEntry, OLE2T(item.m_bstrName));

			wsprintf(szEntry, _T("ItemValueType%d"), i);
			WriteProfileInt(m_pszSection, szEntry, item.m_varValue.vt);

			wsprintf(szEntry, _T("ItemValue%d"), i);
			CComVariant var = item.m_varValue;
			var.ChangeType(VT_BSTR);
			WriteProfileString(m_pszSection, szEntry, OLE2T(var.bstrVal));
		}
	}

	void LoadFromRegistry()
	{
		USES_CONVERSION;
		TCHAR szEntry[256];
	
		m_arrItems.RemoveAll();
		int nCount = GetProfileInt(m_pszSection, _T("ItemCount"), 0);

		for (int i = 0; i < nCount; i++)
		{
			CPropertyBagItem item;

			wsprintf(szEntry, _T("ItemName%d"), i);
			item.m_bstrName = GetProfileString(m_pszSection, szEntry);

			wsprintf(szEntry, _T("ItemValueType%d"), i);
			VARTYPE vt = GetProfileInt(m_pszSection, szEntry, VT_BSTR);

			wsprintf(szEntry, _T("ItemValue%d"), i);
			item.m_varValue = GetProfileString(m_pszSection, szEntry);
			item.m_varValue.ChangeType(vt);

			m_arrItems.Add(item);
		}
	}

	LPCTSTR m_pszSection;

	CSimpleArray<CPropertyBagItem> m_arrItems;
};

#endif // _REGPROPERTYBAG_H__
