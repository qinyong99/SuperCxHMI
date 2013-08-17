// DevItem.h : interface of the CDevObj class
//

#ifndef __DEVITEM_H__
#define __DEVITEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDevObjDoc;
class CRunView;
class CGroupObj;

_COM_SMARTPTR_TYPEDEF(ICxExtendedObject, _uuidof(ICxExtendedObject));

interface IDynamicSet;

#define OBJMISC_EVENTSINK  0x00000001L

#define ABSOLUTEVALUE(a) ((a) < 0 ? (-a) : (a))

//根据两个点求相对角度
int CalculateAngle(POINT ptOrigin, POINT ptCal);
float CalculateAngle(POINTF ptOrigin, POINTF ptCal);
POINTF CalculateRotatePoint(POINTF ptOrgin, POINTF ptCal, float fAngle);

typedef struct tagLINKINFO
{
	DISPID dispid;
	ICxDynamicObject* pDynamicObject;
} LINKINFO;

typedef struct tagEVENTINFO
{
	DISPID dispid;
	CString strMethod;
} EVENTINFO;

class CDevObj : public CCmdTarget  
{
	DECLARE_SERIAL(CDevObj)

//原DynamicSet组件函数
public:
	HRESULT HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected);
	HRESULT GetDynamic(DISPID dispid, IUnknown** ppUnknown);
	HRESULT RemoveDynamic(DISPID dispid);
	HRESULT AddDynamic(DISPID dispid, IUnknown* pUnknown);
	//新增
	BOOL HaveDynamic() { return m_arLinkInfo.GetSize() > 0; }
	virtual BOOL OnMouseInput(LPCSTR szPropName, BOOL bProcess);

protected:
	CArray<LINKINFO, LINKINFO&> m_arLinkInfo;

	BOOL m_bInitEventInfo;
	IID m_iidEventInterface;
	CArray<EVENTINFO, EVENTINFO&> m_arrEventInfo;

	DWORD m_dwEventCookie;
	IConnectionPointPtr m_pEventCP;

public:
	CDevObj(CDevObjDoc* pContainer = NULL);
	virtual ~CDevObj();

// Attributes
public:
	DWORD m_dwVer;

	CDC* m_pDC;

	CRectF m_position;
	int m_nLayer;
	BOOL m_bLocked;
	BOOL m_bVisible;
	CString m_strToolTip; //提示文本
	int m_nPrivilege; //安全区
	CString m_strDescription;
	
// Operations
public:
	BOOL GetEnventSinkStatus() { return (m_dwMiscStatus	& OBJMISC_EVENTSINK != 0); }

	virtual void OnDynamicChange();
	virtual BOOL OnWindowMessage(const MSG* pMessage, BOOL bSubObj = FALSE){ return FALSE; }
	virtual BOOL TranslateAccelerator(const MSG* pMessage) { return FALSE; }
	
	virtual void Active(CRunView* pView){}
	virtual void Deactive(){}

	virtual void Draw(CDC* pDC){}
	virtual void Print(CDC* pDC) { Draw(pDC); }
	virtual void DrawBackToFront(CDC* pDC){}
	virtual void DrawFrontToBack(CDC* pDC){}
	virtual BOOL HitTest(CPoint point, const CRunView* pView);
	virtual void MoveTo(const CRectF& position, BOOL bUniformScale, CRunView* pView = NULL);

	virtual void RotateAt(CPointF point, float fAngle, CRunView* pView = NULL);
	virtual void ScaleAt(CPointF point, float fHorScale, float fVerScale, CRunView* pView = NULL);

	virtual CString GetDisplayName(){ return m_strDisplayName; }
	virtual void PutDisplayName(LPCSTR strDisplayName){ m_strDisplayName = strDisplayName; };

	virtual CString GetClassName();
	virtual IDispatch* GetObjectDispatch(){ return NULL; }
	virtual LPUNKNOWN GetObjectUnknown(){ return NULL; }

	virtual HRESULT PutProperty(DISPID dwDispID, VARIANT* pVar);

	virtual CRect GetBoundingRect(){ return CRect(m_position.GetRECT()); }
	virtual CRectF GetPositionRect(){ return m_position; }
	virtual void Invalidate(CRunView* pView = NULL);
	virtual void Release();

	virtual void InitRun();

	virtual void FakeParseDynamic();
	virtual void ParseDynamic();
	virtual void OnTimer() {}
	virtual void OnMouseLeave() {}

	virtual BOOL IsDefaultMousePointer() { return FALSE; }
	
	virtual void Serialize(CArchive& ar);   // for Native data
	virtual void SaveProperty(CArchive& ar);   // 保存属性
	virtual void RestoreProperty(CArchive& ar);   // 还原属性

	void InitEventInfo();
	BOOL GetEventByDispid(DISPID dispid, CString& strEvent);

	void AdviseEnventSink();

	IUnknown* GetInterfaceHook(const void* pv);

	DWORD m_dwMiscStatus;

	WORD m_wAreaMask[8];

// Implementation
public:
	CDevObjDoc* m_pDocument;
	CGroupObj* m_pGroupObj;	//所在组合对象

	DWORD m_dwSubObjNum; // 对象存在于一个组合对象中时的编号

	DWORD m_dwObjectNumber;
	LPSTORAGE m_lpStorage;
	BYTE m_bNeedCommit;     // TRUE if need CommitItem

	LPSTORAGE m_lpNewStorage; 

	DWORD GetNewObjectNumber();
	void GetObjectName(LPTSTR lpszItemName) const;

	//动态属性属性
	HRESULT LoadDynamicProperty(IStream *pStream);
	HRESULT SaveDynamicProperty(IStream *pStream);
	void SerializeDynamicProperty(CArchive& ar);
	
protected:	
	BEGIN_INTERFACE_PART(ScriptEngineSite, ICxScriptEngineSite)
		STDMETHOD(EvaluateExpression)(BSTR bstrSource, VARIANT* pvaValidObjects);
		STDMETHOD(Execute)(BSTR bstrStatement);
		STDMETHOD(PreTreatExpression)(BSTR bstrSource, BSTR* pbstrSource);
		STDMETHOD(PreTreatExpressionEx)(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax);
	END_INTERFACE_PART(ScriptEngineSite)

	BEGIN_INTERFACE_PART(EventHandler, IDispatch)
		STDMETHOD(GetIDsOfNames)(REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs);
		STDMETHOD(GetTypeInfo)(UINT iTypeInfo, LCID lcid,	ITypeInfo** ppTypeInfo);
		STDMETHOD(GetTypeInfoCount)(UINT* pnInfoCount);
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID iid, LCID lcid,
			WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo, UINT* piArgError);
	END_INTERFACE_PART(EventHandler)

	DECLARE_INTERFACE_MAP()

protected:
	BOOL m_bExtendedObject;
	CString m_strDisplayName;
	CString m_strClassName;

	ICxExtendedObjectPtr m_pExtendedObject;


	friend class CExtendedControl;

};

typedef CTypedPtrList<CObList, CDevObj*> CDevObjList;
typedef CTypedPtrArray<CObArray, CDevObj*> CDevObjArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CDevObj*> CNamedDevobjMap;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __DEVITEM_H__