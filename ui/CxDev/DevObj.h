// DevObj.h: interface for the CDevObj class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DEVOBJ_H__
#define __DEVOBJ_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDevObjDoc;
class CFormDoc;
class CLayoutView;
class CGroupObj;
class CDevObjDoc;
class CDynamicSet;

_COM_SMARTPTR_TYPEDEF(ICxExtendedObject, _uuidof(ICxExtendedObject));

#define OBJMISC_EVENTSINK  0x00000001L

#define ABSOLUTEVALUE(a) ((a) < 0 ? (-a) : (a))

//根据两个点求相对角度
int CalculateAngle(POINT ptOrigin, POINT ptCal);
float CalculateAngle(POINTF ptOrigin, POINTF ptCal);
POINTF CalculateRotatePoint(POINTF ptOrgin, POINTF ptCal, float fAngle);

void RemoveRightDigits(CString& str);

typedef struct tagLINKINFO
{
	DISPID dispid;
	ICxDynamicObject* pDynamicObject;
} LINKINFO;

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
	virtual BOOL HaveDynamic() { return m_arLinkInfo.GetSize() > 0; };

protected:
	CArray<LINKINFO, LINKINFO&> m_arLinkInfo;

// Constructors
protected:      // abstract class
	CDevObj(CDevObjDoc* pContainer = NULL);

// Attributes
public:
	DWORD m_dwVer;

	//三种对象类型，对应三个不同的CDevObj子类
	enum enumObjectType
	{
		devobj,
		drawobj,
		groupobj,
		symbolobj,
		ctrlobj,
		formobj,
		reportobj,
	};

	CDC* m_pDC;

	CRectF m_position;
	int m_nLayer;
	BOOL m_bLocked;
	BOOL m_bVisible;
	CString m_strToolTip; //提示文本
	int m_nPrivilege; //安全区
	CString m_strDescription;
	
// Overridables
public:
	virtual void Draw(CDC* pDC) {}
	virtual void Print(CDC* pDC) { Draw(pDC); }
	virtual void DrawBackToFront(CDC* pDC){}
	virtual void DrawFrontToBack(CDC* pDC){}

	virtual void OnUngrouped(){};
	
	virtual CString GetDisplayName(){ return m_strDisplayName; }
	virtual BOOL PutDisplayName(LPCSTR strDisplayName, BOOL bValidName = TRUE);
	virtual CString GetClassName(){ return m_strClassName; }

	virtual BOOL IsUniformScale() { return FALSE; }
	virtual BOOL IsRunMode() { return (m_pGroupObj != NULL); }
	virtual BOOL IsDrawVisible() { return (m_bVisible || m_pGroupObj == NULL); }

	virtual BOOL IsBaseObject() { return TRUE; }

	virtual enumObjectType GetObjectType(){ return devobj;}
	virtual ICxEnumExtendProperty* EnumExtendProperty(){ return NULL; }

	//动态属性属性
	HRESULT LoadDynamicProperty(IStream *pStream);
	HRESULT SaveDynamicProperty(IStream *pStream);
	void SerializeDynamicProperty(CArchive& ar);

	//用于复制
	virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);
	
	enum TrackerState { general, resize, reshape, rotate };
	virtual TrackerState GetTrackerState(TrackerState state);
	virtual void DrawTracker(CDC* pDC, TrackerState state, CLayoutView* pView = NULL);
	virtual CRectF GetPositionRect(){ return m_position; }
	virtual CRect GetBoundingRect(){ return CRect(m_position.GetRECT()); }
	virtual int GetHandleCount(TrackerState state);
	virtual CPoint GetHandle(int nHandle, TrackerState state, CLayoutView* pView);
	virtual HCURSOR GetHandleCursor(int nHandle, TrackerState state);
	virtual int HitTestHandle(CPoint point, CLayoutView* pView, TrackerState state);
	virtual void MoveHandleTo(int nHandle, TrackerState state, CPoint point, CLayoutView* pView = NULL);
	virtual void DrawTrackerHandleShape(CDC* pDC, int nHandle, TrackerState state, CPoint point, CLayoutView* pView);
	virtual BOOL HitTest(CPoint point, CLayoutView* pView);
	virtual BOOL Intersects(const CRect& rect);
	virtual void MoveTo(const CRectF& position, BOOL bUniformScale, CLayoutView* pView = NULL);

	virtual void Rotate(int lAngle, CLayoutView* pView = NULL);
	virtual void RotateAt(CPointF point, float fAngle, CLayoutView* pView = NULL);
	virtual void ScaleAt(CPointF point, float fHorScale, float fVerScale, CLayoutView* pView = NULL);
	virtual CPoint GetRotateBasePoint();
	virtual void SetRotateBasePoint(CPoint point);
	virtual void DrawRotateAtShape(CDC* pDC, CPointF point, long lAngle);
	virtual void DrawTrackerShape(CDC* pDC);

	virtual void Invalidate(CLayoutView* pView = NULL);
	virtual BOOL GetPropertyPageCLSIDs(CArray< CLSID, CLSID& >& aclsidPages);
	virtual void Delete(BOOL bAutoDelete = TRUE);
	virtual LPUNKNOWN GetObjectUnknown(){ return NULL; }

	virtual HRESULT PutProperty(DISPID dwDispID, VARIANT* pVar);

	virtual void Release();

	virtual void SetChildObjectDocument() {}
// Implementation
public:
	CDevObjDoc* m_pDocument;
	CGroupObj* m_pGroupObj;	//所在组合对象

	DWORD m_dwSubObjNum; // 对象存在于一个组合对象中时的编号

	static CImageList c_imageGeneralHandle;
	static CImageList c_imageRotateHandle;

	DWORD m_dwObjectNumber;

	void SetEnventSinkStatus(BOOL b) 
	{ 
		if(b)
			m_dwMiscStatus |= OBJMISC_EVENTSINK;
		else
			m_dwMiscStatus &= ~OBJMISC_EVENTSINK;
	}
	BOOL GetEnventSinkStatus() { return (m_dwMiscStatus	& OBJMISC_EVENTSINK != 0); }

	DWORD GetNewObjectNumber();
	void GetObjectName(LPTSTR lpszItemName) const;

private:
	CString m_strDisplayName;
protected:
	CString m_strClassName;
	DWORD m_dwMiscStatus;

public:
	BEGIN_INTERFACE_PART(PropertyNotifySink, IPropertyNotifySink)
		STDMETHOD(OnChanged )(DISPID dispid);
	    STDMETHOD(OnRequestEdit)(DISPID dispid);
    END_INTERFACE_PART(PropertyNotifySink)

	DECLARE_INTERFACE_MAP()
	
public:
	virtual void Serialize(CArchive& ar);   // for Native data
	virtual ~CDevObj();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	friend class CFormDoc;              // for access to back pointer
	friend class CLayoutView;	
	friend class CExtendedControl;
};

class CMapNoCaseStringToPtr : public CMapStringToPtr
{
	DECLARE_DYNAMIC(CMapNoCaseStringToPtr)
public:
	// Construction
	CMapNoCaseStringToPtr(int nBlockSize = 10);

	// add a new (key, value) pair
	void SetAt(LPCTSTR key, void* newValue);
	
	// removing existing (key, ?) pair
	BOOL RemoveKey(LPCTSTR key);
	void RemoveAll();

	BOOL LookupNoCase(LPCTSTR key, void*& rValue) const;

	CMapStringToPtr m_mapNocase;

protected:
	// local typedefs for CTypedPtrMap class template
	typedef CString BASE_KEY;
	typedef LPCTSTR BASE_ARG_KEY;
	typedef void* BASE_VALUE;
	typedef void* BASE_ARG_VALUE;
};

template<class BASE_CLASS, class KEY, class VALUE>
class CTypedPtrMapEx : public CTypedPtrMap<BASE_CLASS, KEY, VALUE>
{
public:
	
	// Construction
	CTypedPtrMapEx(int nBlockSize = 10)
		: CTypedPtrMap<BASE_CLASS, KEY, VALUE>(nBlockSize) { }
	
	// Lookup
	BOOL LookupNoCase(BASE_CLASS::BASE_ARG_KEY key, VALUE& rValue) const
	{ return BASE_CLASS::LookupNoCase(key, (BASE_CLASS::BASE_VALUE&)rValue); }
};

typedef CTypedPtrList<CObList, CDevObj*> CDevObjList;
typedef CTypedPtrArray<CObArray, CDevObj*> CDevObjArray;
typedef CTypedPtrMapEx<CMapNoCaseStringToPtr, CString, CDevObj*> CNamedDevobjMap;

#endif // !defined(__DEVOBJ_H__)
