// LookupDynamic.h : Declaration of the CLookupDynamic

#ifndef __LOOKUPDYNAMIC_H_
#define __LOOKUPDYNAMIC_H_

#include "resource.h"       // main symbols
#include "CxDynObj.h"

/////////////////////////////////////////////////////////////////////////////
// CLookupDynamic
class ATL_NO_VTABLE CLookupDynamic : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLookupDynamic, &CLSID_LookupDynamic>,
	public IDispatchImpl<ILookupDynamic, &IID_ILookupDynamic, &LIBID_CXHMIOBJLib>,
	public IPersistStream,
	public CCxDynamicObjectBase,
	public ICxDynamicObjectImpl<CLookupDynamic>
{
public:
	CLookupDynamic()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LOOKUPDYNAMIC)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLookupDynamic)
	COM_INTERFACE_ENTRY(ILookupDynamic)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY(ICxDynamicObject)
END_COM_MAP()

// IDynamicObject
public:
	STDMETHOD(GetClassName)(BSTR *pbstrClassName);
	STDMETHOD(GetCustomPage)(CLSID *pclsidPropPage);
	STDMETHOD(Evaluation)(VARIANT* pvaInValue, VARIANT* pvaOutValue);

public:
	STDMETHOD(InitNew)(long lPropInfo)
	{
		m_bExactMatch = FALSE;

		return S_OK;
	}
	STDMETHOD(GetClassID)(CLSID *pClassID)
	{
		*pClassID = GetObjectCLSID();
		return S_OK;
	}
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR* /* pcbSize */)
	{
		ATLTRACENOTIMPL(_T("IPersistStream::GetSizeMax"));
	}
	STDMETHOD(IsDirty)()
	{
		return (m_bRequiresSave) ? S_OK : S_FALSE;
	}
	STDMETHOD(Load)(LPSTREAM pStm)
	{
		m_bstrSource.ReadFromStream(pStm);
		pStm->Read(&m_bExactMatch, sizeof(m_bExactMatch), NULL);
		int nCount;
		pStm->Read(&nCount, sizeof(int), NULL);
		for (int i = 0; i < nCount; i++)
		{
			CLevel level;
			level.Input1.ReadFromStream(pStm);
			level.Input2.ReadFromStream(pStm);
			level.Output.ReadFromStream(pStm);
			m_Levels.Add(level);
		}
		return S_OK;
	}
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty)
	{
		m_bstrSource.WriteToStream(pStm);
		pStm->Write(&m_bExactMatch, sizeof(m_bExactMatch), NULL);
		int nCount = m_Levels.GetSize();
		pStm->Write(&nCount, sizeof(int), NULL);
		for (int i = 0; i < nCount; i++)
		{
			m_Levels[i].Input1.WriteToStream(pStm);
			m_Levels[i].Input2.WriteToStream(pStm);
			m_Levels[i].Output.WriteToStream(pStm);
		}
		return S_OK;
	}

// ILookupDynamic
public:
	STDMETHOD(RemoveAllLevels)();
	STDMETHOD(RemoveLevel)(/*[in]*/ int iIndex);

private:

	class CLevel
	{
	public:
		CLevel()
		{
		}
		CLevel(const CLevel& level)
		{
			InternalCopy(level);
		}
		CLevel& operator=(const CLevel& level)
		{
			InternalCopy(level);
			return *this;
		}
		void InternalCopy(const CLevel& level)
		{
			Input1 = level.Input1;
			Input2 = level.Input2;
			Output = level.Output;
		}

		CComVariant Input1;
		CComVariant Input2;
		CComVariant Output;
	};

	typedef CSimpleValArray<CLevel> _LEVELS;

public:
	STDMETHOD(get_ExactMatch)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_ExactMatch)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_LevelsCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_LevelsCount)(/*[in]*/ long newVal);
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);
	STDMETHOD(AddLevel)(/*[in]*/ VARIANT pInput1, /*[in]*/ VARIANT pInput2, /*[in]*/ VARIANT pOutput);
	STDMETHOD(GetLevel)(/*[in]*/ int iIndex, /*[out]*/ VARIANT* pInput1, /*[out]*/ VARIANT* pInput2, /*[out]*/ VARIANT* pOutput);
	_LEVELS m_Levels;
	BOOL m_bExactMatch;
};


#endif //__LOOKUPDYNAMIC_H_
