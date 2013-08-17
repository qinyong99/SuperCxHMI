// GroupItem.h: interface for the CGroupObj class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GROUPOBJ_H__
#define __GROUPOBJ_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGroupObj : public CDevObj  
{
	DECLARE_SERIAL(CGroupObj)
public:
	CGroupObj(CDevObjDoc* pContainerDoc = NULL);
	virtual ~CGroupObj();

public: 
	BOOL CreateOrLoad(LPCTSTR pszName,
		REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium);
	void AddSubObject(CDevObj* pObj);
	void RemoveObject(CDevObj* pObj);
	void RemoveAllObjects();
	CDevObjArray& GetSubObjects() { return m_arrSubObj; }
	CDevObjArray m_arrSubObj;

	virtual LPUNKNOWN GetObjectUnknown();
	virtual void Draw(CDC* pDC);
	virtual void Print(CDC* pDC);
	virtual void DrawFrontToBack(CDC* pDC);
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

	virtual void OnUngrouped();

	virtual void SetChildObjectDocument();

	//用于复制
	virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);
	virtual enumObjectType GetObjectType(){ return groupobj; }

	void RecalcPosition();
	CPointF GetScaleBasePointF();

	virtual HRESULT PutProperty(DISPID dwDispID, VARIANT* pVar);

	inline CRect* GetFillBoundRect() { return m_pFillBoundRect;	}
	void RecalcFillBoundRect();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoClickPoint)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL GetDispatchIID(IID* piid);

	virtual HRESULT MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat);
	virtual HRESULT GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName);

	DECLARE_OLETYPELIB(CGroupObj)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CGroupObj)
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
	afx_msg void OnMousePointerChanged();
	afx_msg int GetPrivilege();
	afx_msg void SetPrivilege(int iNewValue);
	afx_msg void OnToolTipTextChanged();
	afx_msg void OnDescriptionChanged();

	afx_msg void OnFillStyleChanged();
	afx_msg void OnHorizontalFillDirectionChanged();
	afx_msg void OnVerticalFillDirectionChanged();
	afx_msg int GetHorizontalFillPercent();
	afx_msg void SetHorizontalFillPercent(int iNewValue);
	afx_msg int GetVerticalFillPercent();
	afx_msg void SetVerticalFillPercent(int iNewValue);
	afx_msg void OnBrushImageFileNameChanged();

	afx_msg void OnEdgeStyleChanged();
	afx_msg float GetEdgeWidth();
	afx_msg void SetEdgeWidth(float fNewValue);
	afx_msg void OnLineJoinStyleChanged();

	afx_msg void OnEdgeColorChanged();
	afx_msg void OnHighLightEdgeColorChanged();
	afx_msg void OnFillColorChanged();
	afx_msg void OnHighLightFillColorChanged();

	afx_msg LPUNKNOWN GetObjectByName(LPCTSTR lpszName);
	afx_msg LPDISPATCH GetObjectCollection();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	BEGIN_INTERFACE_PART(CategorizeProperties, ICategorizeProperties)
		STDMETHOD(MapPropertyToCategory)(DISPID dispid, PROPCAT* ppropcat);
		STDMETHOD(GetCategoryName)(PROPCAT propcat, LCID lcid, BSTR* pbstrName);
	END_INTERFACE_PART(CategorizeProperties)

	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo2)
		INIT_INTERFACE_PART(CGroupObj, ProvideClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
	END_INTERFACE_PART_STATIC(ProvideClassInfo)

	DECLARE_INTERFACE_MAP()

protected:
	CPointF m_ptRotateBase;
	enumScaleBasePoint m_enumScaleBase;
	enumMousePointer m_enumMousePointer;
	BOOL m_bUniformScale;

	float m_fHorScale;							//水平缩放因子
	float m_fVerScale;							//垂直缩放因子
	float m_fRotateAngle;                       //旋转角度

	OLE_COLOR m_clrEdgeColor;				//边界颜色
	OLE_COLOR m_clrHighLightEdge;			//边界高亮色
	enumEdgeStyle m_enumEdgeStyle;				//边界型式
	enumLineJoinStyle m_enumLineJoinStyle;		//接头类型
	float m_fEdgeWidth;							//边界线宽
	
	OLE_COLOR m_clrFillColor;		//前景颜色
	OLE_COLOR m_clrHighLightFill;			//前景高亮色
	enumFillStyle m_enumFillStyle;				//前景类型
	CString m_strBrushImageFileName;				//位图画刷文件名
	
	enumVerticalDirection m_enumVerFillDirection;	//垂直填充方向
	enumHorizontalDirection m_enumHorFillDirection;	//水平填充方向
	long m_nVerFillPercent;					//垂直填充百分比
	long m_nHorFillPercent;					//水平填充百分比

	CRect* m_pBoundRect;
	CRect* m_pFillBoundRect;

	friend class CFormDoc;
};

#endif // !defined(AFX_GROUPITEM_H__4BD87EC1_6448_11D5_A46A_CA20CAFC6067__INCLUDED_)
