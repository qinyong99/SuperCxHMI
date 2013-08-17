/////////////////////////////////////////////////////////////////////
// FormObj.h: interface for the CFormObj class.

#if !defined(AFX_FORMOBJ_H__47F0F2A2_B6F2_409C_BF0E_0ACA046DB26E__INCLUDED_)
#define AFX_FORMOBJ_H__47F0F2A2_B6F2_409C_BF0E_0ACA046DB26E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDevDoc;

class CFormObj : public CDevObj  
{
	DECLARE_SERIAL(CFormObj)
public:
	CFormObj(CDevObjDoc* pContainerDoc = NULL);
	virtual ~CFormObj();

	virtual LPUNKNOWN GetObjectUnknown();
	virtual void Draw(CDC* pDC);
	virtual void Print(CDC* pDC);
//	virtual void DrawFrontToBack(CDC* pDC);
	virtual void MoveTo(const CRectF& position, BOOL bUniformScale, CLayoutView* pView = NULL);
	virtual void Delete(BOOL bAutoDelete = TRUE);
	virtual void Release();

	virtual BOOL HitTest(CPoint point, CLayoutView* pView);

	virtual CRect GetBoundingRect();
	virtual BOOL IsUniformScale() { return m_bUniformScale; }

	virtual void Rotate(int lAngle, CLayoutView* pView = NULL);
	virtual CPoint GetRotateBasePoint();
	virtual void SetRotateBasePoint(CPoint point);
	virtual void RotateAt(CPointF point, float fAngle, CLayoutView* pView = NULL);
	virtual void ScaleAt(CPointF point, float fHorScale, float fVerScale, CLayoutView* pView = NULL);
	virtual void DrawRotateAtShape(CDC* pDC, CPointF point, long lAngle);

	virtual void MoveHandleTo(int nHandle, TrackerState state, CPoint point, CLayoutView* pView = NULL);
	virtual void DrawTrackerHandleShape(CDC* pDC, int nHandle, TrackerState state, CPoint point, CLayoutView* pView);
	virtual TrackerState GetTrackerState(TrackerState state);

	virtual void RecalcPosition();
	CPointF GetScaleBasePointF();

	virtual CMultiDocTemplate* GetLayoutTemplate() { return theApp.m_pLayoutTemplate; }
	
	virtual BOOL CreateFormDoc();

	void OnEdit();

	CDevDoc* GetDevDoc();
	CFormDoc* GetFormDoc();

	virtual BOOL IsBaseObject() { return FALSE; }

	BOOL CreateOrLoad(LPCTSTR pszName,
		REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium, RECTF* pRect = NULL);

	void AddSubObject(CDevObj* pObj);
	void RemoveObject(CDevObj* pObj);

	//用于复制
	virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);
	BOOL CreateFromSubDocumentClipboard(CArchive& ar, IStorage* pStorage);
	virtual enumObjectType GetObjectType(){ return formobj; }

	virtual HRESULT MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat);
	virtual HRESULT GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName);

	virtual ICxEnumExtendProperty* EnumExtendProperty();

	CDevObj* FindVariableObject(LPCTSTR lpszName);
	

	// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoClickPoint)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

protected:
	DECLARE_OLETYPELIB(CFormObj)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFormObj)
	afx_msg float GetHorizontalPosition();
	afx_msg void SetHorizontalPosition(float fNewValue);
	afx_msg float GetVerticalPosition();
	afx_msg void SetVerticalPosition(float fNewValue);
	afx_msg float GetWidth();
	afx_msg void SetWidth(float fNewValue);
	afx_msg float GetHeight();
	afx_msg void SetHeight(float fNewValue);
	afx_msg float GetRoateBasePointX();
	afx_msg void SetRoateBasePointX(float fNewValue);
	afx_msg float GetRoateBasePointY();
	afx_msg void SetRoateBasePointY(float fNewValue);
	afx_msg float GetRotateAngle();
	afx_msg void SetRotateAngle(float lNewValue);
	afx_msg enumScaleBasePoint GetScaleBasePoint();
	afx_msg void SetScaleBasePoint(enumScaleBasePoint fNewValue);
	afx_msg float GetHorizontalScale();
	afx_msg void SetHorizontalScale(float fNewValue);
	afx_msg float GetVerticalScale();
	afx_msg void SetVerticalScale(float fNewValue);
	afx_msg int GetLayer();
	afx_msg void SetLayer(int iNewValue);
	afx_msg BSTR GetBrushImageFile();
	afx_msg void SetBrushImageFile(LPCTSTR lpszNewValue);
	afx_msg void OnLockedChanged();
	afx_msg void OnVisibleChanged();
	afx_msg BSTR GetName();
	afx_msg void SetName(LPCTSTR lpszNewValue);
	afx_msg VARIANT_BOOL IsConnected(LPCTSTR pszProertyName);
	afx_msg void ConnectObject(LPCTSTR lpszProertyName, LPUNKNOWN punkObject);
	afx_msg void ConnectDirect(LPCTSTR lpszProertyName, LPCTSTR pszDataSource);
	afx_msg LPUNKNOWN GetConnectObject(LPCTSTR lpszProertyName);
	afx_msg void Disconnect(LPCTSTR lpszProertyName);
	afx_msg int GetConnectState(DISPID dispid);
	afx_msg void OnUniformScaleChanged();
	afx_msg int GetPrivilege();
	afx_msg void SetPrivilege(int iNewValue);
	afx_msg void OnToolTipTextChanged();
	afx_msg BSTR GetTagGroup();
	afx_msg void SetTagGroup(LPCTSTR lpszNewValue);
	afx_msg BSTR GetAssocTagGroup();

	afx_msg OLE_COLOR GetForeColor();
	afx_msg void SetForeColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetBackColor();
	afx_msg void SetBackColor(OLE_COLOR nNewValue);
	afx_msg int GetVisibleLayer();
	afx_msg void SetVisibleLayer(int iNewValue);
	afx_msg void OnDescriptionChanged();
	afx_msg void OnTransparencyChanged();

	afx_msg enumMousePointer GetMousePointer();
	afx_msg void SetMousePointer(enumMousePointer fNewValue);

	afx_msg LPUNKNOWN GetObjectByName(LPCTSTR lpszName);
	afx_msg LPDISPATCH GetObjectCollection();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	BEGIN_INTERFACE_PART(CategorizeProperties, ICategorizeProperties)
		STDMETHOD(MapPropertyToCategory)(DISPID dispid, PROPCAT* ppropcat);
		STDMETHOD(GetCategoryName)(PROPCAT propcat, LCID lcid, BSTR* pbstrName);
	END_INTERFACE_PART(CategorizeProperties)

	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo2)
		INIT_INTERFACE_PART(CFormObj, ProvideClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
	END_INTERFACE_PART_STATIC(ProvideClassInfo)

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

	DECLARE_INTERFACE_MAP()

protected:
	
	virtual void Serialize(CArchive& ar);
	BOOL GetDispatchIID(IID* piid);

public:
	CFormDoc* m_pFormDoc;

protected:
	CPointF m_ptRotateBase;
	enumScaleBasePoint m_enumScaleBase;
	BOOL m_bUniformScale;
	BOOL m_bTransparency;

	float m_fHorScale;							//水平缩放因子
	float m_fVerScale;							//垂直缩放因子
	float m_fRotateAngle;                       //旋转角度
};

#endif // !defined(AFX_FORMOBJ_H__47F0F2A2_B6F2_409C_BF0E_0ACA046DB26E__INCLUDED_)
