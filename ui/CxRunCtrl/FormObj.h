// FormObj.h: interface for the CFormObj class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __FORMOBJ_H__
#define __FORMOBJ_H__

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
	virtual IDispatch* GetObjectDispatch();
	virtual void Draw(CDC* pDC);
	virtual void Print(CDC* pDC);
//	virtual void DrawFrontToBack(CDC* pDC);
	virtual void MoveTo(const CRectF& position, BOOL bUniformScale, CRunView* pView = NULL);

	virtual BOOL HitTest(CPoint point, CRunView* pView);

	virtual CRect GetBoundingRect();

	void RecalcPosition();
	CPointF GetScaleBasePointF();

	virtual BOOL CreateFormDoc();

	virtual CPoint GetRotateBasePoint();
	virtual void SetRotateBasePoint(CPoint point);
	virtual void RotateAt(CPointF point, float fAngle, CRunView* pView = NULL);
	virtual void ScaleAt(CPointF point, float fHorScale, float fVerScale, CRunView* pView = NULL);
	
	virtual void Release();

	virtual void InitRun();
	virtual void FakeParseDynamic();
	virtual void ParseDynamic();

	virtual void Active(CRunView* pView);

	CDevObj* FindVariableObject(LPCTSTR lpszName);
	

	// Firing events
	void AFX_CDECL FireEvent(DISPID dispid, BYTE* pbParams, ...);
	void FireEventV(DISPID dispid, BYTE* pbParams, va_list argList);
	void FireEvent(DISPID dispid);

	virtual void OnMouseLeave();
	virtual BOOL OnWindowMessage(const MSG* pMessage, BOOL bSubObj = FALSE);

	virtual BOOL IsDefaultMousePointer();

	CDevDoc* GetDevDoc();
	CFormDoc* GetTopFormDoc();
	CView* GetRunView();

	// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoClickPoint)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

protected:
	DECLARE_OLETYPELIB(CFormObj)

	//事件
	BEGIN_CONNECTION_PART(CFormObj, FormObjectCP)
		CONNECTION_IID(__uuidof(IFormObjectEvents))
	END_CONNECTION_PART(FormObjectCP)

	DECLARE_CONNECTION_MAP()

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
	afx_msg BOOL GetLocked();
	afx_msg void SetLocked(BOOL bNewValue);
	afx_msg BOOL GetVisible();
	afx_msg void SetVisible(BOOL bNewValue);
	afx_msg BSTR GetName();
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
	afx_msg void OnDescriptionChanged();
	afx_msg BSTR GetTagGroup();
	afx_msg void SetTagGroup(LPCTSTR lpszNewValue);
	afx_msg BSTR GetAssocTagGroup();

	afx_msg OLE_COLOR GetForeColor();
	afx_msg void SetForeColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetBackColor();
	afx_msg void SetBackColor(OLE_COLOR nNewValue);
	afx_msg int GetVisibleLayer();
	afx_msg void SetVisibleLayer(int iNewValue);
	afx_msg void OnTransparencyChanged();

	afx_msg enumMousePointer GetMousePointer();
	afx_msg void SetMousePointer(enumMousePointer fNewValue);

	afx_msg LPUNKNOWN GetObjectByName(LPCTSTR lpszName);
	afx_msg LPDISPATCH GetObjectCollection();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	//扩展自动化接口
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

	BEGIN_INTERFACE_PART(ProvideMultipleClassInfo, IProvideMultipleClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
		STDMETHOD(GetMultiTypeInfoCount)(ULONG *pcti);
		STDMETHOD(GetInfoOfIndex)(ULONG iti, DWORD dwFlags, ITypeInfo** pptiCoClass, DWORD* pdwTIFlags, ULONG* pcdispidReserved, IID* piidPrimary, IID* piidSource);
	END_INTERFACE_PART_STATIC(ProvideMultipleClassInfo)

	DECLARE_INTERFACE_MAP()

protected:
	
	virtual void Serialize(CArchive& ar);
	BOOL GetDispatchIID(IID* piid);

public:
	CFormDoc* m_pFormDoc;

	DWORD m_dwSubformMoudleNumber;
	CString m_strSubformMoudleName;

protected:
	CPointF m_ptRotateBase;
	enumScaleBasePoint m_enumScaleBase;
	BOOL m_bUniformScale;
	BOOL m_bTransparency;

	static CDevObj* m_pObjectWithCapture;
// 	static CDevObj* m_pObjectWithFocus;

	float m_fHorScale;							//水平缩放因子
	float m_fVerScale;							//垂直缩放因子
	float m_fRotateAngle;                       //旋转角度

	BOOL m_bParseScript;
};

#endif // !defined(__FORMOBJ_H__)
