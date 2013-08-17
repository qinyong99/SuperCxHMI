// DrawItem.h: interface for the CDrawObj class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRAWITEM_H__
#define __DRAWITEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


interface ICxDrawObject;

class CDrawObj : public CDevObj  
{
	DECLARE_SERIAL(CDrawObj)	

public:
	CDrawObj(CDevObjDoc* pContainerDoc = NULL);
	virtual ~CDrawObj();

// Attributes
public:
	ICxDrawObject* m_pDrawObject;
	IDispatch* m_pDispatch;

// Implementation
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawObj)
	public:
	//}}AFX_VIRTUAL

public:
	BOOL m_bNeedLoadOrSaveStorage;

	void ReadObject( CArchive& ar );
	void WriteObject( CArchive& ar );

	virtual void Draw(CDC* pDC);
	virtual void Print(CDC* pDC);
	virtual void DrawFrontToBack(CDC* pDC);
    virtual BOOL FinishCreate(HRESULT hr);

	virtual CRect GetBoundingRect();
	virtual CRectF GetPositionRect();
	virtual void OnChange();
	virtual BOOL OnWindowMessage(const MSG* pMessage, BOOL bSubObj = FALSE);
	virtual BOOL HitTest(CPoint point, const CRunView* pView);
	virtual void MoveTo(const CRectF& rect, BOOL bUniformScale, CRunView* pView = NULL);
	virtual void RotateAt(CPointF point, float fAngle, CRunView* pView = NULL);
	virtual void ScaleAt(CPointF point, float fHorScale, float fVerScale, CRunView* pView = NULL);


	//用于复制
	void Release();

	virtual IDispatch* GetObjectDispatch();
	virtual LPUNKNOWN GetObjectUnknown();

	virtual CString GetClassName();

	virtual void InitRun();
	virtual void FakeParseDynamic();
	virtual void ParseDynamic();
	virtual void OnTimer();
	virtual void OnMouseLeave();

	virtual BOOL IsDefaultMousePointer();

protected:	
	BEGIN_INTERFACE_PART(DrawObjectSite, ICxDrawObjectSite)
		STDMETHOD(SetCapture)(BOOL bCapture);
		STDMETHOD(GetCapture)();
		STDMETHOD(SetFocus)(BOOL bFocus);
		STDMETHOD(GetFocus)();
		STDMETHOD(GetEventObject)(ICxEventObj** ppEvent);
		STDMETHOD(IsRunMode)();
		STDMETHOD(GetClientName)(BSTR *pbstrName);
        STDMETHOD(SetClientName)(BSTR bstrName);
        STDMETHOD(GetClientLayer)(long *plLayer);
        STDMETHOD(SetClientLayer)(long lLayer);
		STDMETHOD(GetClientLocked)(BOOL* pbLocked);
        STDMETHOD(SetClientLocked)(BOOL bLocked);
		STDMETHOD(GetClientVisibility)(BOOL* pbVisible);
        STDMETHOD(SetClientVisibility)(BOOL bVisible);
		STDMETHOD(GetClientToolTip)(BSTR *pbstrText);
        STDMETHOD(SetClientToolTip)(BSTR bstrText);
		STDMETHOD(GetClientPrivilege)(long *plArea);
        STDMETHOD(SetClientPrivilege)(long lArea);
		STDMETHOD(OnMouseInput)(const WCHAR* szPropName, BOOL bProcess);
		STDMETHOD(SetTimer)(UINT nElapse);
		STDMETHOD(TrackMouseLeave)();
		STDMETHOD(GetExpressionCreatorProc)(long lProc);
		STDMETHOD(GetWindow)(HWND* phWindow);
		STDMETHOD(InvalidateRect)(LPCRECT prect, BOOL tErase);
		STDMETHOD(InvalidateRgn)(HRGN hRgn, BOOL tErase);
		STDMETHOD(GetDC)(HDC* phDC);
		STDMETHOD(ReleaseDC)(HDC hDC);
		STDMETHOD(GetDefaultServer)(BSTR *pbstrComputer);
		STDMETHOD(SetNeedStorage)();
		STDMETHOD(DocToClient)(RECT* pRect);
		STDMETHOD(GetFillBoundRect)(RECT** ppRect);
	END_INTERFACE_PART(DrawObjectSite)

	BEGIN_INTERFACE_PART(DrawObjectAdviseSink, ICxDrawObjectAdviseSink)
		STDMETHOD(OnViewChange)();
		STDMETHOD(OnDynamicChange)();
	END_INTERFACE_PART(DrawObjectAdviseSink)

	BEGIN_INTERFACE_PART(AmbientProperties, IDispatch )
		STDMETHOD( GetIDsOfNames )( REFIID iid, LPOLESTR* ppszNames, UINT nNames,
			LCID lcid, DISPID* pDispIDs );
		STDMETHOD( GetTypeInfo )( UINT iTypeInfo, LCID lcid,
			ITypeInfo** ppTypeInfo );
		STDMETHOD( GetTypeInfoCount )( UINT* pnInfoCount );
		STDMETHOD( Invoke )( DISPID dispidMember, REFIID iid, LCID lcid,
			WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
			EXCEPINFO* pExceptionInfo, UINT* piArgError );
	END_INTERFACE_PART(AmbientProperties)

	DECLARE_INTERFACE_MAP()

protected:

	virtual void Serialize(CArchive& ar);
	virtual void SaveProperty(CArchive& ar);   // 保存属性
	virtual void RestoreProperty(CArchive& ar);   // 还原属性

	friend class CFormDoc;

};

#endif // __DRAWITEM_H__
