// LinearDynamic.cpp : Implementation of CLinearDynamic
#include "stdafx.h"
#include "CxDynObjs.h"
#include "LinearDynamic.h"
#include "CxComVariant.h"

/////////////////////////////////////////////////////////////////////////////
// CLinearDynamic


STDMETHODIMP CLinearDynamic::get_HiInValue(VARIANT *pVal)
{
	VariantCopy(pVal, &m_varHiInValue);

	return S_OK;
}

STDMETHODIMP CLinearDynamic::put_HiInValue(VARIANT newVal)
{
	m_varHiInValue.Copy(&newVal);
	m_varHiInValue.ChangeType(VT_R8);

	return S_OK;
}

STDMETHODIMP CLinearDynamic::get_LoInValue(VARIANT *pVal)
{
	VariantCopy(pVal, &m_varLoInValue);

	return S_OK;
}

STDMETHODIMP CLinearDynamic::put_LoInValue(VARIANT newVal)
{
	m_varLoInValue.Copy(&newVal);
	m_varLoInValue.ChangeType(VT_R8);

	return S_OK;
}

STDMETHODIMP CLinearDynamic::get_HiOutValue(VARIANT *pVal)
{
	VariantCopy(pVal, &m_varHiOutValue);

	return S_OK;
}

STDMETHODIMP CLinearDynamic::put_HiOutValue(VARIANT newVal)
{
	m_varHiOutValue.Copy(&newVal);
	m_varHiOutValue.ChangeType(VT_R8);

	return S_OK;
}

STDMETHODIMP CLinearDynamic::get_LoOutValue(VARIANT *pVal)
{
	VariantCopy(pVal, &m_varLoOutValue);

	return S_OK;
}

STDMETHODIMP CLinearDynamic::put_LoOutValue(VARIANT newVal)
{
	m_varLoOutValue.Copy(&newVal);
	m_varLoOutValue.ChangeType(VT_R8);

	return S_OK;
}

STDMETHODIMP CLinearDynamic::GetClassName(BSTR *pbstrClassName)
{
	*pbstrClassName = ::SysAllocString(L"Linear");

	return S_OK;
}

STDMETHODIMP CLinearDynamic::GetCustomPage(CLSID *pclsidPropPage)
{
	*pclsidPropPage = CLSID_LinearPage;

	return S_OK;
}


STDMETHODIMP CLinearDynamic::get_Source(BSTR *pVal)
{
	*pVal = m_bstrSource.Copy();

	return S_OK;
}

STDMETHODIMP CLinearDynamic::put_Source(BSTR newVal)
{
	m_bstrSource = newVal;

	return S_OK;
}

inline HRESULT CLinearDynamic::ChangeToR8(VARIANT* pvaValue)
{
	if (pvaValue->vt == VT_BSTR)
	{
		USES_CONVERSION;
		USES_CHINESEVAR_CONVERSION;

		BSTR bstr = pvaValue->bstrVal;
		m_pScriptEngineSite->EvaluateExpression(T2W(VARC2A(W2T(bstr))), pvaValue);
	//	if (pvaValue->vt == VT_EMPTY)
	//		m_pScriptEngineSite->EvaluateExpression(bstr, pvaValue);
	
		SysFreeString(bstr); // 必须释放
	}
	return VariantChangeType(pvaValue, pvaValue, 0, VT_R8);
}

STDMETHODIMP CLinearDynamic::Evaluation(VARIANT* pvaInValue, VARIANT* pvaOutValue)
{
	HRESULT hr = ::VariantChangeType(pvaInValue, pvaInValue, 0, VT_R8);
	if (FAILED(hr))
		return hr;

	CComVariant varHiInput(m_varHiInValue);
	CComVariant varLoInput(m_varLoInValue);
	CComVariant varHiOutput(m_varHiOutValue);
	CComVariant varLoOutput(m_varLoOutValue);

	hr = ChangeToR8(&varHiInput);
	if (FAILED(hr))
		return hr;
	hr = ChangeToR8(&varLoInput);
	if (FAILED(hr))
		return hr;
	hr = ChangeToR8(&varHiOutput);
	if (FAILED(hr))
		return hr;
	hr = ChangeToR8(&varLoOutput);
	if (FAILED(hr))
		return hr;

	pvaOutValue->vt = VT_R8;

	if (varHiInput.dblVal == varLoInput.dblVal)
		return S_FALSE;
	if (pvaInValue->dblVal >= varLoInput.dblVal && pvaInValue->dblVal <= varHiInput.dblVal ||
		pvaInValue->dblVal <= varLoInput.dblVal && pvaInValue->dblVal >= varHiInput.dblVal)
	{
		//插入法查值
		pvaOutValue->dblVal = (pvaInValue->dblVal - varLoInput.dblVal) / (varHiInput.dblVal - varLoInput.dblVal) * 
				(varHiOutput.dblVal - varLoOutput.dblVal) + varLoOutput.dblVal;
	}
	//上/下限值的限定
	else if (varLoInput.dblVal < varHiInput.dblVal)
	{
		if (pvaInValue->dblVal > varHiInput.dblVal)
			pvaOutValue->dblVal = varHiOutput.dblVal;
		else 
			pvaOutValue->dblVal = varLoOutput.dblVal;
	}
	else
	{
		if (pvaInValue->dblVal < varHiInput.dblVal)
			pvaOutValue->dblVal = varHiOutput.dblVal;
		else 
			pvaOutValue->dblVal = varLoOutput.dblVal;
	}
	
	//偏移计算
	if (m_bUseOffset)
	{
		CComVariant varOriginValue(m_varOriginValue);
		varOriginValue.ChangeType(VT_R8);
		pvaOutValue->dblVal += varOriginValue.dblVal;
	}

	return S_OK;
}

STDMETHODIMP CLinearDynamic::InvertEvaluation(VARIANT* pvaOutValue, VARIANT* pvaInValue)
{
	HRESULT hr = ::VariantChangeType(pvaOutValue, pvaOutValue, 0, VT_R8);
	if (FAILED(hr))
		return hr;

	//偏移计算
	if (m_bUseOffset)
	{
		CComVariant varOriginValue(m_varOriginValue);
		varOriginValue.ChangeType(VT_R8);
		pvaOutValue->dblVal -= varOriginValue.dblVal;
	}
	
	CComVariant varHiInput(m_varHiInValue);
	CComVariant varLoInput(m_varLoInValue);
	CComVariant varHiOutput(m_varHiOutValue);
	CComVariant varLoOutput(m_varLoOutValue);

	hr = ChangeToR8(&varHiInput);
	if (FAILED(hr))
		return hr;
	hr = ChangeToR8(&varLoInput);
	if (FAILED(hr))
		return hr;
	hr = ChangeToR8(&varHiOutput);
	if (FAILED(hr))
		return hr;
	hr = ChangeToR8(&varLoOutput);
	if (FAILED(hr))
		return hr;
	
	
//	CComVariant var1(m_varHiOutValue);
//	ChangeToR8(&var1);
//	varHiOutput = var;
	

	pvaInValue->vt = VT_R8;

	if (varHiOutput.dblVal == varLoOutput.dblVal)
		return S_FALSE;
	if (pvaOutValue->dblVal >= varLoOutput.dblVal && pvaOutValue->dblVal <= varHiOutput.dblVal ||
		pvaOutValue->dblVal <= varLoOutput.dblVal && pvaOutValue->dblVal >= varHiOutput.dblVal)
	{
		//插入法查值
		pvaInValue->dblVal = (pvaOutValue->dblVal - varLoOutput.dblVal) / (varHiOutput.dblVal - varLoOutput.dblVal) * 
				(varHiInput.dblVal - varLoInput.dblVal) + varLoInput.dblVal;
	}
	//上/下限值的限定
	else if (varLoOutput.dblVal < varHiOutput.dblVal)
	{
		if (pvaOutValue->dblVal > varHiOutput.dblVal)
			pvaInValue->dblVal = varHiInput.dblVal;
		else 
			pvaInValue->dblVal = varLoInput.dblVal;
	}
	else
	{
		if (pvaOutValue->dblVal < varHiOutput.dblVal)
			pvaInValue->dblVal = varHiInput.dblVal;
		else 
			pvaInValue->dblVal = varLoInput.dblVal;
	}
	
	return S_OK;
}

STDMETHODIMP CLinearDynamic::get_UseOffset(BOOL *pVal)
{
	*pVal = m_bUseOffset;

	return S_OK;
}

STDMETHODIMP CLinearDynamic::put_UseOffset(BOOL newVal)
{
	m_bUseOffset = newVal;

	return S_OK;
}

STDMETHODIMP CLinearDynamic::get_EnableMouseInput(BOOL *pVal)
{
	*pVal = m_bEnableMouseInput;

	return S_OK;
}

STDMETHODIMP CLinearDynamic::put_EnableMouseInput(BOOL newVal)
{
	m_bEnableMouseInput = newVal;

	return S_OK;
}


STDMETHODIMP CLinearDynamic::get_FetchInput(BOOL *pVal)
{
	*pVal = m_bFrechInput;

	return S_OK;
}

STDMETHODIMP CLinearDynamic::put_FetchInput(BOOL newVal)
{
	m_bFrechInput = newVal;

	return S_OK;
}
