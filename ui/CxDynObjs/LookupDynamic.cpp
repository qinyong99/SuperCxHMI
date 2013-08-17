// LookupDynamic.cpp : Implementation of CLookupDynamic
#include "stdafx.h"
#include "CxDynObjs.h"
#include "LookupDynamic.h"
#include "CxComVariant.h"


/////////////////////////////////////////////////////////////////////////////
// CLookupDynamic

//获取类名
STDMETHODIMP CLookupDynamic::GetClassName(BSTR *pbstrClassName)
{
	*pbstrClassName = ::SysAllocString(L"Lookup");

	return S_OK;
}

//获取当前页
STDMETHODIMP CLookupDynamic::GetCustomPage(CLSID *pclsidPropPage)
{
	*pclsidPropPage = CLSID_LookupPage;

	return S_OK;
}

STDMETHODIMP CLookupDynamic::Evaluation(VARIANT* pvaInValue, VARIANT* pvaOutValue)
{
	CxComVariant varTemp;
	if (pvaInValue->vt == VT_DISPATCH)
	{
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		HRESULT hr = pvaInValue->pdispVal->Invoke(0, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				&dispparamsNoArgs, &varTemp, NULL, NULL);
		if (FAILED(hr))
			return hr;
	}
	else
	{
		VariantCopy(&varTemp, pvaInValue);
	}
		
	if (m_bExactMatch)
	{
		for (int i = 0; i < m_Levels.m_nSize; i++)
		{
			if (varTemp == m_Levels[i].Input1)
			{
				VariantCopy(pvaOutValue, &m_Levels[i].Output);
				return S_OK;
			}
		}

		// 布尔类型的特殊处理
		if (varTemp.vt == VT_BOOL)
		{
			for (int i = 0; i < m_Levels.m_nSize; i++)
			{
				CComVariant var = m_Levels[i].Input1;
				var.ChangeType(VT_BOOL);

				if (varTemp == var)
				{
					VariantCopy(pvaOutValue, &m_Levels[i].Output);
					return S_OK;
				}
			}
		}
	}
	else
	{
		//插入法查值
		for (int i = 0; i < m_Levels.m_nSize; i++)
		{
			if (varTemp >= m_Levels[i].Input1 && 
				varTemp <= m_Levels[i].Input2)
			{
				VariantCopy(pvaOutValue, &m_Levels[i].Output);
				return S_OK;
			}
		}	
	}
		
	return S_FALSE;
}

STDMETHODIMP CLookupDynamic::RemoveLevel(int iIndex)
{
	m_Levels.RemoveAt(iIndex);
	return S_OK;
}

STDMETHODIMP CLookupDynamic::RemoveAllLevels()
{
	m_Levels.RemoveAll();
	return S_OK;
}

STDMETHODIMP CLookupDynamic::GetLevel(int iIndex, VARIANT *pInput1, VARIANT *pInput2, VARIANT *pOutput)
{
	VariantCopy(pInput1, &m_Levels[iIndex].Input1);
	VariantCopy(pInput2, &m_Levels[iIndex].Input2);
	VariantCopy(pOutput, &m_Levels[iIndex].Output);
	return S_OK;
}

STDMETHODIMP CLookupDynamic::AddLevel(VARIANT pInput1, VARIANT pInput2, VARIANT pOutput)
{
	CLevel LevelTemp;
 	LevelTemp.Input1.Copy(&pInput1);
 	LevelTemp.Input2.Copy(&pInput2);
 	LevelTemp.Output.Copy(&pOutput);
 	m_Levels.Add(LevelTemp);
	return S_OK;
}

STDMETHODIMP CLookupDynamic::get_Source(BSTR *pVal)
{
	*pVal = m_bstrSource.Copy();
	return S_OK;
}

STDMETHODIMP CLookupDynamic::put_Source(BSTR newVal)
{
	m_bstrSource = newVal;
	return S_OK;
}

STDMETHODIMP CLookupDynamic::get_LevelsCount(long *pVal)
{
	*pVal = m_Levels.GetSize();
	return S_OK;
}

STDMETHODIMP CLookupDynamic::put_LevelsCount(long newVal)
{
	return S_OK;
}

STDMETHODIMP CLookupDynamic::get_ExactMatch(BOOL *pVal)
{
	*pVal = m_bExactMatch;

	return S_OK;
}

STDMETHODIMP CLookupDynamic::put_ExactMatch(BOOL newVal)
{
	m_bExactMatch = newVal;

	return S_OK;
}
