/////////////////////////////////////////////////////////
//			动态属性----格式页
//
//	创建:wengjiang				日期:2001年9月5日
//
//	修改:
/////////////////////////////////////////////////////////
#ifndef __FORMATDYNAMIC_H_
#define __FORMATDYNAMIC_H_

#include "resource.h"       // main symbols
#include "CxDynObj.h"
#include "PropInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CFormatDynamic
class ATL_NO_VTABLE CFormatDynamic : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFormatDynamic, &CLSID_FormatDynamic>,
	public IDispatchImpl<IFormatDynamic, &IID_IFormatDynamic, &LIBID_CXHMIOBJLib>,
	public IPersistStream,
	public CCxDynamicObjectBase,
	public ICxDynamicObjectImpl<CFormatDynamic>
{
public:
	CFormatDynamic()
	{
		m_bEnableMouseInput = FALSE;
		m_pPropinfo = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FORMATDYNAMIC)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFormatDynamic)
	COM_INTERFACE_ENTRY(IFormatDynamic)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY(ICxDynamicObject)
END_COM_MAP()

public:
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
	STDMETHOD(GetCustomPage)(CLSID* pclsidPropPage);
	STDMETHOD(Evaluation)(VARIANT* pvaInValue, VARIANT* pvaOutValue);
	STDMETHOD(IsEnableMouseInput)() { return m_bEnableMouseInput ? S_OK : S_FALSE; }

	STDMETHOD(InitNew)(long lPropInfo)
	{
		m_iDataType = 1;
		m_varWholeDigits = 8;
		m_varDecimalDigits = 2;
		m_iJustify = 0;
		m_bstrFormat = L"%s";
		m_bEnableMouseInput = FALSE;

		m_pPropinfo = (CDynamicPropInfo *)lPropInfo;

		return S_OK;
	}
	STDMETHOD(GetPropInfo)(long* plPropInfo)
	{
		*plPropInfo = (long)m_pPropinfo;
		return S_OK;
	}

public:
	STDMETHOD(GetClassID)(CLSID* pClassID);
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR* /* pcbSize */)
	{
		ATLTRACENOTIMPL(_T("IPersistStream::GetSizeMax"));
	}
	STDMETHOD(IsDirty)();
	STDMETHOD(Load)(LPSTREAM pStm);
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty);
	
public:
	STDMETHOD(get_DisplaySample)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(Clone)(IFormatDynamic** ppDynamic);
	STDMETHOD(GetNumericFormat)(VARIANT* pvaWholeDigits, VARIANT* pvaDecimalDigits, int* pJustify);
	STDMETHOD(SetNumericFormat)(VARIANT vaWholeDigits, VARIANT vaDecimalDigits, int iJustify);
	STDMETHOD(get_SourceDataType)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_SourceDataType)(/*[in]*/ int newVal);
	STDMETHOD(get_Format)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Format)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_EnableMouseInput)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_EnableMouseInput)(/*[in]*/ BOOL newVal);

	HRESULT ChangeToI4(VARIANT* pvaValue);
	
	CComBSTR m_bstrFormat; //格式化字符串
	int m_iDataType;	//0：格式化1：数字2：字母-数字
	CComVariant m_varWholeDigits; //用于存放数字的位数或字符串的行数
	CComVariant m_varDecimalDigits; //用于存放数字的小数位数或字符串的每行的字符数
	int m_iJustify; //0：左对齐1：中间对齐2：右对齐
	BOOL m_bEnableMouseInput;

	CDynamicPropInfo* m_pPropinfo;
};

#endif //__FORMATDYNAMIC_H_
