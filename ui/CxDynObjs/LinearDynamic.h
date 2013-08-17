// LinearDynamic.h : Declaration of the CLinearDynamic

#ifndef __LINEARDYNAMIC_H_
#define __LINEARDYNAMIC_H_

#include "resource.h"       // main symbols
#include "CxDynObj.h"
#include "CxHmiMisc.h"
#include "CxHmiObj.h"
#include "PropInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CLinearDynamic
class ATL_NO_VTABLE CLinearDynamic : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLinearDynamic, &CLSID_LinearDynamic>,
	public CCxDynamicObjectBase,
	public IPersistStream,
	public ICxDynamicObjectImpl<CLinearDynamic>,
	public IDispatchImpl<ILinearDynamic, &IID_ILinearDynamic, &LIBID_CXHMIOBJLib>
{
public:
	CLinearDynamic()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LINEARDYNAMIC)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLinearDynamic)
	COM_INTERFACE_ENTRY(ILinearDynamic)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY(ICxDynamicObject)
END_COM_MAP()

////////////////////////////////////////////////////////////////////////
	// IPersist
public:
	STDMETHOD(GetClassID)(CLSID *pClassID)
	{
		*pClassID = GetObjectCLSID();
		return S_OK;
	}
	STDMETHOD(InitNew)(long lPropInfo)
	{
		CDynamicPropInfo* pPropInfo = (CDynamicPropInfo *)lPropInfo;
		m_bUseOffset = FALSE;
		m_bEnableMouseInput = FALSE;
		m_bFrechInput = FALSE;
		m_varLoInValue = 0;
		m_varHiInValue = 100;
		m_varLoOutValue = 0;
		m_varHiOutValue = pPropInfo->varCurrentValue; 

		return S_OK;
	}
	// IPersistStream
	STDMETHOD(IsDirty)()
	{
		return (m_bRequiresSave) ? S_OK : S_FALSE;
	}
	STDMETHOD(Load)(LPSTREAM pStm)
	{
		m_bstrSource.ReadFromStream(pStm);
		m_varHiOutValue.ReadFromStream(pStm);
		m_varHiInValue.ReadFromStream(pStm);
		m_varLoOutValue.ReadFromStream(pStm);
		m_varLoInValue.ReadFromStream(pStm);
		pStm->Read(&m_bUseOffset, sizeof(m_bUseOffset), NULL);
		pStm->Read(&m_bEnableMouseInput, sizeof(m_bEnableMouseInput), NULL);
		pStm->Read(&m_bFrechInput, sizeof(m_bEnableMouseInput), NULL);
		return S_OK;
	}
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty)
	{
		m_bstrSource.WriteToStream(pStm);
		m_varHiOutValue.WriteToStream(pStm);
		m_varHiInValue.WriteToStream(pStm);
		m_varLoOutValue.WriteToStream(pStm);
		m_varLoInValue.WriteToStream(pStm);
		pStm->Write(&m_bUseOffset, sizeof(m_bUseOffset), NULL);
		pStm->Write(&m_bEnableMouseInput, sizeof(m_bEnableMouseInput), NULL);
		pStm->Write(&m_bFrechInput, sizeof(m_bEnableMouseInput), NULL);
		return S_OK;
	}
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR* /* pcbSize */)
	{
		ATLTRACENOTIMPL(_T("IPersistStream::GetSizeMax"));
	}

// ICxDynamicObject
public:
	STDMETHOD(GetClassName)(BSTR *pbstrClassName);
	STDMETHOD(GetCustomPage)(CLSID *pclsidPropPage);
	STDMETHOD(Evaluation)(VARIANT* pvaInValue, VARIANT* pvaOutValue);
	STDMETHOD(InvertEvaluation)(VARIANT* pvaOutValue, VARIANT* pvaInValue);
	STDMETHOD(IsEnableMouseInput)() { return m_bEnableMouseInput ? S_OK : S_FALSE; }
	STDMETHOD(InitRun)(ICxScriptEngineSite* pScriptSite, IDispatch* pDispObject, DISPID dispid)
	{
		//保存原始值用于计算偏移
		if (m_bUseOffset && pDispObject != NULL)
		{
			DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
			pDispObject->Invoke(dispid, IID_NULL,
					LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
					&dispparamsNoArgs, &m_varOriginValue, NULL, NULL);
		}
		return ICxDynamicObjectImpl<CLinearDynamic>::InitRun(pScriptSite, pDispObject, dispid);
	}

	STDMETHOD(SetScriptEngineSite)(ICxScriptEngineSite* pScriptSite)
	{
		HRESULT hr = ICxDynamicObjectImpl<CLinearDynamic>::SetScriptEngineSite(pScriptSite);
		if (FAILED(hr) || m_pScriptEngineSite == NULL)
			return hr;

		if (!m_bFrechInput)
		{
			hr = m_pScriptEngineSite->PreTreatExpression(m_bstrSource, NULL);
		}
		else
		{
			m_varLoInValue.Clear();
			m_varHiInValue.Clear();
			hr = m_pScriptEngineSite->PreTreatExpressionEx(m_bstrSource, NULL, &m_varLoInValue, &m_varHiInValue);
		}
		if (FAILED(hr))
			return S_FALSE;

		return S_OK;
	}

	HRESULT ChangeToR8(VARIANT* pvaValue);

// ILinearDynamic
public:
	STDMETHOD(get_FetchInput)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_FetchInput)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_EnableMouseInput)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_EnableMouseInput)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_UseOffset)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_UseOffset)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LoOutValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_LoOutValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_HiOutValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_HiOutValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_LoInValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_LoInValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_HiInValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_HiInValue)(/*[in]*/ VARIANT newVal);

protected:
	CComVariant m_varOriginValue;

	CComVariant m_varHiOutValue;
	CComVariant m_varHiInValue;
	CComVariant m_varLoOutValue;
	CComVariant m_varLoInValue;
	BOOL m_bUseOffset;
	BOOL m_bEnableMouseInput;
	BOOL m_bFrechInput;
};

#endif //__LINEARDYNAMIC_H_
