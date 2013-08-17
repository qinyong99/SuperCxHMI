// CxDynObj.h: interface for the ICxOleObjecImpl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CXDYNOBJ_H_
#define __CXDYNOBJ_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class ATL_NO_VTABLE CCxDynamicObjectBase
{
public:
	CCxDynamicObjectBase()
	{
		m_pdispObject = NULL;
		m_dispidObject = DISPID_UNKNOWN;
	}
	~CCxDynamicObjectBase()
	{

	}

public:
	IDispatch* m_pdispObject;
	DISPID m_dispidObject;
	unsigned m_bRequiresSave:1;
};

template <class T>
class ATL_NO_VTABLE ICxDynamicObjectImpl : public ICxDynamicObject
{
public:
	CComBSTR m_bstrSource;
	ICxScriptEngineSite* m_pScriptEngineSite;
	BOOL m_bChanged;
	BOOL m_bValidOrigin;
	CComVariant m_varOrigin; // 最原始的值

	STDMETHOD(InitNew)(long lPropInfo)
	{
		return S_OK;
	}
	STDMETHOD(GetClassName)(BSTR *pbstrClassName)
	{
		ATLASSERT(FALSE);
		return E_FAIL;
	}
	STDMETHOD(GetCustomPage)(CLSID *pclsidPropPage)
	{
		*pclsidPropPage = CLSID_NULL;
		return S_OK;
	}
	STDMETHOD(SetConnectionExpression)(BSTR bstrExpression)
	{
		m_bstrSource = bstrExpression;
		return S_OK;
	}
	STDMETHOD(GetConnectionExpression)(BSTR *pbstrExpression)
	{
		*pbstrExpression = m_bstrSource.Copy();
		return S_OK;
	}
	STDMETHOD(SetScriptEngineSite)(ICxScriptEngineSite* pScriptSite)
	{
		m_pScriptEngineSite = pScriptSite;

		FakeParseConnection();

		return S_OK;
	}
	STDMETHOD(InitRun)(ICxScriptEngineSite* pScriptSite, IDispatch* pDispObject, DISPID dispid)
	{
		T* pT = static_cast<T*>(this);

		SetScriptEngineSite(pScriptSite);
		pT->m_pdispObject = pDispObject;
		pT->m_dispidObject = dispid;

		m_bChanged = FALSE;

		// CxDev 调用时可能 pDispObject = NULL
		if (pT->m_pdispObject != NULL)
		{
			DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
			HRESULT hr = pT->m_pdispObject->Invoke(pT->m_dispidObject, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				&dispparamsNoArgs, &m_varOrigin, NULL, NULL);
			m_bValidOrigin = SUCCEEDED(hr); 
		}
	
		return S_OK;
	}
	STDMETHOD(InvertParseConnection)()
	{
		T* pT = static_cast<T*>(this);

		if (m_pScriptEngineSite == NULL || IsEnableMouseInput() != S_OK)
			return S_FALSE;

		//取属性值
		CComVariant var;
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		pT->m_pdispObject->Invoke(pT->m_dispidObject, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				&dispparamsNoArgs, &var, NULL, NULL);

		
		InvertEvaluation(&var, &var);

		BOOL bString = FALSE;
		if (var.vt != VT_BSTR)
			var.ChangeType(VT_BSTR);
		else
			bString = TRUE;
		CComBSTR bstrStatement = m_bstrSource;
		bstrStatement.Append("=");
		if (bString)
			bstrStatement.Append("\"");
		bstrStatement.AppendBSTR(var.bstrVal);
		if (bString)
			bstrStatement.Append("\"");
		
		m_pScriptEngineSite->Execute(bstrStatement);

		ParseConnection(); // 立即重新刷新

		return S_OK;
	}
	STDMETHOD(FakeParseConnection)()
	{
		if (m_pScriptEngineSite != NULL)
		{
			//求表达式的值，但不刷新对象属性，目的是使数据会话获取标签信息
			CComVariant varExpressionValue;
			HRESULT hr = m_pScriptEngineSite->EvaluateExpression(m_bstrSource, &varExpressionValue);
			
			//处理不带属性的标签表达式时，以下语句会使自动化接口求缺省属性值
			if (varExpressionValue.vt == VT_DISPATCH)
			{
				CComVariant var;
				DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
				return varExpressionValue.pdispVal->Invoke(0, IID_NULL,
						LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
						&dispparamsNoArgs, &var, NULL, NULL);
				
				
//				VariantChangeType(&varExpressionValue, &varExpressionValue, 0, VT_BSTR);
			}
			
		}
		
		return S_OK;
	}
	STDMETHOD(ParseConnection)()
	{
		T* pT = static_cast<T*>(this);

		CComVariant varExpressionValue;
		if (m_pScriptEngineSite == NULL)
			return S_FALSE;

		//求表达式值
		HRESULT hr = m_pScriptEngineSite->EvaluateExpression(m_bstrSource, &varExpressionValue);
		
		CComVariant varValue;
		if (hr == S_OK)
			hr = Evaluation(&varExpressionValue, &varValue);
		
		if (hr != S_OK)
		{
			if (!m_bChanged || !m_bValidOrigin)
				return S_FALSE;
			
			varValue = m_varOrigin;
			m_bChanged = FALSE;	
		}
		else
		{
			m_bChanged = TRUE;
		}

		DISPPARAMS dispparams = {NULL, NULL, 1, 1};
		dispparams.rgvarg = &varValue;
		DISPID dispidPut = DISPID_PROPERTYPUT;
		dispparams.rgdispidNamedArgs = &dispidPut;

		ATLASSERT(pT->m_pdispObject != NULL);
		if (varValue.vt == VT_UNKNOWN || varValue.vt == VT_DISPATCH || 
			(varValue.vt & VT_ARRAY) || (varValue.vt & VT_BYREF))
		{
			HRESULT hr = pT->m_pdispObject->Invoke(pT->m_dispidObject, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF,
				&dispparams, NULL, NULL, NULL);
			if (SUCCEEDED(hr))
				return hr;
		}

		return pT->m_pdispObject->Invoke(pT->m_dispidObject, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
				&dispparams, NULL, NULL, NULL);
	}
	STDMETHOD(Evaluation)(VARIANT* pvaInValue, VARIANT* pvaOutValue)
	{
		VariantCopy(pvaOutValue, pvaInValue);

		return S_OK;
	}
	STDMETHOD(InvertEvaluation)(VARIANT* pvaOutValue, VARIANT* pvaInValue)
	{
		VariantCopy(pvaInValue, pvaOutValue);

		return S_OK;
	}
	STDMETHOD(IsEnableMouseInput)()
	{
		return S_FALSE;
	}
	STDMETHOD(IsInitRun)()
	{
		T* pT = static_cast<T*>(this);
		return pT->m_pdispObject == NULL ? S_FALSE : S_OK;
	}
	STDMETHOD(GetPropInfo)(long* plPropInfo)
	{
		return E_NOTIMPL;
	}
};


#endif // __CXDYNOBJ_H_