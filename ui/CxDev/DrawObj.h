// DrawObj.h: interface for the CDrawObj class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRAWOBJ_H__
#define __DRAWOBJ_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDrawObj : public CDevObj  
{
	DECLARE_SERIAL(CDrawObj)	

public:
	CDrawObj(CDevObjDoc* pContainerDoc = NULL);
	virtual ~CDrawObj();

// Attributes
public:
	ICxDrawObject* m_pDrawObject;
//	LPOLEOBJECT m_lpObject; 

// Implementation
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawObj)
	public:
	//}}AFX_VIRTUAL

public:
	BOOL m_bNeedLoadOrSaveStorage;

	void ReadObject(CArchive& ar);
	void WriteObject( CArchive& ar);
	
	virtual void Draw(CDC* pDC);
	virtual void Print(CDC* pDC);
	virtual void DrawFrontToBack(CDC* pDC);

	virtual BOOL HaveDynamic();

    BOOL FinishCreate( HRESULT hr );
	BOOL Create( REFCLSID clsid, LPCTSTR pszName = NULL );
	BOOL CreateOrLoad(REFCLSID clsid, LPCTSTR pszName, IStream* pStream);
	
	virtual LPUNKNOWN GetObjectUnknown();
	
	//”√”⁄∏¥÷∆
	virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);
	virtual enumObjectType GetObjectType(){ return drawobj; }

	virtual void Release();

	virtual CRectF GetPositionRect();
	virtual CRect GetBoundingRect();
	virtual BOOL HitTest(CPoint point, CLayoutView* pView);

	virtual int GetHandleCount(TrackerState state);
	virtual CPoint GetHandle(int nHandle, TrackerState state, CLayoutView* pView);
	virtual HCURSOR GetHandleCursor(int nHandle, TrackerState state);
	virtual void MoveHandleTo(int nHandle, TrackerState state, CPoint point, CLayoutView* pView = NULL);
	virtual void DrawTrackerHandleShape(CDC* pDC, int nHandle, TrackerState state, CPoint point, CLayoutView* pView);
	virtual TrackerState GetTrackerState(TrackerState state);

	virtual BOOL Intersects(const CRect& rect);
	virtual void MoveTo(const CRectF& position, BOOL bUniformScale, CLayoutView* pView = NULL);

	virtual void Rotate(int lAngle, CLayoutView* pView = NULL);
	virtual CPoint GetRotateBasePoint();
	virtual void SetRotateBasePoint(CPoint point);
	virtual void RotateAt(CPointF point, float fAngle, CLayoutView* pView = NULL);
	virtual void ScaleAt(CPointF point, float fHorScale, float fVerScale, CLayoutView* pView = NULL);
	virtual void DrawRotateAtShape(CDC* pDC, CPointF point, long lAngle);
	virtual void DrawTrackerShape(CDC* pDC);

	virtual CString GetClassName();
	
	virtual BOOL GetPropertyPageCLSIDs(CArray< CLSID, CLSID& >& aclsidPages);

	virtual void OnChange();

	virtual void OnUngrouped();
	
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
		STDMETHOD(OnMouseInput)(const WCHAR* szPropName, BOOL bProces);
		STDMETHOD(SetTimer)(UINT nElapse);
		STDMETHOD(TrackMouseLeave)();
		STDMETHOD(TrackMouse)(BOOL bTrack);
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

	friend class CFormDoc;
};

#endif // !defined(AFX_DRAWOBJ_H__14093845_64E3_11D5_A46A_FD45FCAC3B67__INCLUDED_)
