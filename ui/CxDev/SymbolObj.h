// SymbolObj.h: interface for the CSymbolObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLOBJ_H__2DE353FE_62D2_4143_85B4_03128C98E88E__INCLUDED_)
#define AFX_SYMBOLOBJ_H__2DE353FE_62D2_4143_85B4_03128C98E88E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TypeChanger.h"
_COM_SMARTPTR_TYPEDEF(ICxDynamicObject, __uuidof(ICxDynamicObject));

#include "ScriptEngine.h"

class CSymbolProp : public CProp
{
public:
	class CRelatedProp
	{
	public:
		CRelatedProp()
		{
			m_nSubIndex = -1;
		}
		CRelatedProp& operator = (const CRelatedProp& source)
		{
			m_nSubIndex = source.m_nSubIndex;
			m_strPropName = source.m_strPropName;
			m_pDynamicObject = source.m_pDynamicObject;
			return *this;
		}

	public:
		int m_nSubIndex;
		CString m_strPropName;	//子对象中的对应属性名
		ICxDynamicObjectPtr m_pDynamicObject;
	};

public:
	CArray<CRelatedProp, CRelatedProp&> m_arrObjNum;

	CString m_strValidScript;
	
	void Serialize(CArchive& ar);
	void operator = (const CSymbolProp& source)
	{
		m_arrObjNum.RemoveAll();

		m_value = source.m_value;
		m_strName = source.m_strName;
		m_strDescription = source.m_strDescription;
		m_proptype = source.m_proptype;
		m_dispid = source.m_dispid;
		m_strValidScript = source.m_strValidScript;
		
		m_arrObjNum.RemoveAll();
		for(int i = 0; i < source.m_arrObjNum.GetSize(); i++)
		{
			m_arrObjNum.Add(source.m_arrObjNum[i]);
		}
	}
protected:
	void SerializeExtendInterface(CArchive& ar, ICxDynamicObjectPtr& pIn, ICxDynamicObjectPtr& pOutk);
	void SerializeExData(CArchive& ar, _variant_t& data);
};

class CSymbolObj;

class CSymbolScrEng : public CScriptEngine  
{
public:
	CSymbolScrEng(CSymbolObj* pObj)
	{
		ASSERT(pObj != NULL);
		m_pObject = pObj;
	}

// Attributes
public:
	CSymbolObj* m_pObject;
	
// Operations
public:
	virtual HRESULT AddItemsName();
	virtual HRESULT GetItemDispatch(LPCOLESTR pszName, IUnknown** ppDispatch);

//	CTypedPtrMap<CMapStringToPtr, CString, CDevObj*> m_mapNamedItems;
};

class CSymbolObj : public CGroupObj  
{
	DECLARE_SERIAL(CSymbolObj)

public:
	CSymbolObj(CDevObjDoc* pContainerDoc = NULL);
	virtual ~CSymbolObj();
		
protected:
	CArray<CSymbolProp, CSymbolProp&> m_arrExProp;	//扩展属性
	CSymbolScrEng* m_pScriptEngine;
	
public:
	BOOL InitScriptEngine();

	virtual BOOL IsBaseObject() { return FALSE; }

	//用于复制
	virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);
	virtual enumObjectType GetObjectType(){ return symbolobj; }

	virtual void Release();
	virtual CRect GetBoundingRect();

	virtual ICxEnumExtendProperty* EnumExtendProperty();
	
	BOOL IsValidExPropValue(CSymbolProp* pProp, VARIANT* pvaValue);
	HRESULT UpdateAllExPropValue();

	void RemoveAllExProp();
	int AddExtendProperty(CSymbolProp* pExProp);
	BOOL SetExPropInfo(int index, const CSymbolProp* pInfo);
	BOOL GetExPropInfo(int index, CSymbolProp* pOutInfo);
	int GetExPropCount();
	BOOL BeginDesign(CWnd* pParentWnd);

	BOOL HasMouseInputDynamic(DISPID dispid);

	virtual HRESULT MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat);
	virtual HRESULT GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName);

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CSymbolObj)
	afx_msg VARIANT_BOOL IsConnected(LPCTSTR pszProertyName);
	afx_msg void ConnectObject(LPCTSTR lpszProertyName, LPUNKNOWN punkObject);
	afx_msg void ConnectDirect(LPCTSTR lpszProertyName, LPCTSTR pszDataSource);
	afx_msg LPUNKNOWN GetConnectObject(LPCTSTR lpszProertyName);
	afx_msg void Disconnect(LPCTSTR lpszProertyName);
	afx_msg int GetConnectState(DISPID dispid);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	
	//扩展属性自动化接口
	BEGIN_INTERFACE_PART(DispatchExtend, IDispatch)
		STDMETHOD(GetIDsOfNames)(REFIID iid, LPOLESTR* ppszNames, UINT nNames,
			LCID lcid, DISPID* pDispIDs);
		STDMETHOD(GetTypeInfo)(UINT iTypeInfo, LCID lcid,
			ITypeInfo** ppTypeInfo);
		STDMETHOD(GetTypeInfoCount)(UINT* pnInfoCount);
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID iid, LCID lcid,
			WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
			EXCEPINFO* pExceptionInfo, UINT* piArgError);
	END_INTERFACE_PART(DispatchExtend)

	BEGIN_INTERFACE_PART(ScriptEngineSite, ICxScriptEngineSite)
		STDMETHOD(EvaluateExpression)(BSTR bstrSource, VARIANT* pvaValidObjects);
		STDMETHOD(Execute)(BSTR bstrStatement);
		STDMETHOD(PreTreatExpression)(BSTR bstrSource, BSTR* pbstrSource);
		STDMETHOD(PreTreatExpressionEx)(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax);
	END_INTERFACE_PART(ScriptEngineSite)

	DECLARE_INTERFACE_MAP()

protected:
	void RepareExPropertyFrom(int nFrom, int nSub, int nDel);
	DISPID GetNewDispid();
	virtual void Serialize(CArchive& ar);

	virtual BOOL GetDispatchIID(IID* piid);

	HRESULT GetExtendProperty(DISPID dwDispID, VARIANT* pVar);
	HRESULT PutExtendProperty(DISPID dwDispID, VARIANT* pVar);
};

#endif // !defined(AFX_SYMBOLOBJ_H__2DE353FE_62D2_4143_85B4_03128C98E88E__INCLUDED_)
