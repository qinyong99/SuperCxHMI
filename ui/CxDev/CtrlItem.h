// CCtrlItem.h : interface of the CCtrlItem class
//

#ifndef __CTRLITEM_H__
#define __CTRLITEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int MARGIN_PIXELS = 5;

class CFormDoc;
class CLayoutView;
class CCtrlObj;

_COM_SMARTPTR_TYPEDEF(ICxExtendedControl, _uuidof(ICxExtendedControl));

class CCtrlItem : public COleClientItem
{
	DECLARE_SERIAL(CCtrlItem)

// Constructors
public:
	CCtrlItem(CDevObjDoc* pContainer = NULL, CCtrlObj* pCtrlObj = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CFormDoc* GetDocument()
		{ return (CFormDoc*)COleClientItem::GetDocument(); }
	CLayoutView* GetActiveView()
		{ return (CLayoutView*)COleClientItem::GetActiveView(); }

	CCtrlObj* m_pCtrlObj;    // back pointer to OLE draw object

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	void OnActivateUI();
	virtual void OnActivate();

	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnGetClipRect(CRect& rPosition);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateOrLoad(REFCLSID clsid, 
		REFIID iidPersistanceMedium, IUnknown* pPeristanceMedium);
	CString GetNameOfClass();
    BOOL FinishCreate( HRESULT hr );

	BOOL GetPropertyPageCLSIDs( CArray< CLSID, CLSID& >& aclsidPages );
	DWORD HitTest( CPoint point );
	BOOL Draw( CDC* pDC, LPCRECT prectBounds, DVASPECT nDrawAspect = DVASPECT(
		-1 ) );
	BOOL DrawBackToFront( CDC* pDC, LPCRECT prectBounds );
	BOOL DrawFrontToBack( CDC* pDC, LPCRECT prectBounds );

	//”√”⁄øΩ±¥
	void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);

	BOOL m_bWindowless;
	DWORD m_dwAspectPass2;
	ICxExtendedControlPtr m_pExtendedControl;
	BOOL m_bExtendedControl;
	IOleInPlaceObjectWindowlessPtr m_pOleInPlaceObjectWindowless;
	IViewObjectExPtr m_pViewObjectEx;
	DWORD m_dwMiscStatus;
	DWORD m_dwViewStatus;

	IConnectionPointPtr m_pPropertyCP;
	DWORD m_dwPropertyCookie;

	IUnknown* GetInterfaceHook( const void* pv );
	BOOL IsInvisibleAtRuntime() const;
	BOOL ActsLikeButton() const;
	BOOL IsLabelControl() const;
	BOOL IsUIActive() const;
	void OnDeactivate(BOOL tNoRedraw);
	void ReadItem(CArchive& ar);
	void WriteItem(CArchive& ar);
	void CommitItem(BOOL bSuccess);
	BOOL UpdateExtent();

	HWND GetInPlaceObjectWindow();
	void BringWindowTop();

	~CCtrlItem();

protected:
	CDC* m_pDC;
	void Release( OLECLOSE dwCloseOption = OLECLOSE_NOSAVE );

	BEGIN_INTERFACE_PART(PropertyNotifySink, IPropertyNotifySink)
		STDMETHOD(OnChanged )(DISPID dispid);
		STDMETHOD(OnRequestEdit)( DISPID dispid);
	END_INTERFACE_PART(PropertyNotifySink)

	 BEGIN_INTERFACE_PART( AmbientProperties, IDispatch )
	  STDMETHOD( GetIDsOfNames )( REFIID iid, LPOLESTR* ppszNames, UINT nNames,
		 LCID lcid, DISPID* pDispIDs );
	  STDMETHOD( GetTypeInfo )( UINT iTypeInfo, LCID lcid,
		 ITypeInfo** ppTypeInfo );
	  STDMETHOD( GetTypeInfoCount )( UINT* pnInfoCount );
	  STDMETHOD( Invoke )( DISPID dispidMember, REFIID iid, LCID lcid,
		 WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
		 EXCEPINFO* pExceptionInfo, UINT* piArgError );
   END_INTERFACE_PART( AmbientProperties )

	BEGIN_INTERFACE_PART( OleInPlaceSiteWindowless, IOleInPlaceSiteWindowless )
	  STDMETHOD( ContextSensitiveHelp )( BOOL tEnterMode );
	  STDMETHOD( GetWindow )( HWND* phWnd );

	  STDMETHOD( CanInPlaceActivate )();
	  STDMETHOD( DeactivateAndUndo )();
	  STDMETHOD( DiscardUndoState )();
	  STDMETHOD( GetWindowContext )( IOleInPlaceFrame** ppFrame,
		 IOleInPlaceUIWindow** ppDoc, LPRECT prectPos, LPRECT prectClip,
		 LPOLEINPLACEFRAMEINFO pFrameInfo );
	  STDMETHOD( OnInPlaceActivate )();
	  STDMETHOD( OnInPlaceDeactivate )();
	  STDMETHOD( OnPosRectChange )( LPCRECT prectPos );
	  STDMETHOD( OnUIActivate )();
	  STDMETHOD( OnUIDeactivate )( BOOL tUndoable );
	  STDMETHOD( Scroll )( SIZE sizeExtent );

	  STDMETHOD( OnInPlaceActivateEx )( BOOL* ptNoRedraw, DWORD dwFlags );
	  STDMETHOD( OnInPlaceDeactivateEx )( BOOL tNoRedraw );
	  STDMETHOD( RequestUIActivate )();

	  STDMETHOD( AdjustRect )( LPRECT prect );
	  STDMETHOD( CanWindowlessActivate )();
	  STDMETHOD( GetCapture )();
	  STDMETHOD( GetDC )( LPCRECT prect, DWORD dwFlags, HDC* phDC );
	  STDMETHOD( GetFocus )();
	  STDMETHOD( InvalidateRect )( LPCRECT prect, BOOL tErase );
	  STDMETHOD( InvalidateRgn )( HRGN hRgn, BOOL tErase );
	  STDMETHOD( OnDefWindowMessage )( UINT nMessage, WPARAM wParam,
		 LPARAM lParam, LRESULT* plResult );
	  STDMETHOD( ReleaseDC )( HDC hDC );
	  STDMETHOD( ScrollRect )( int dx, int dy, LPCRECT prectScroll,
		 LPCRECT prectClip );
	  STDMETHOD( SetCapture )( BOOL tCapture );
	  STDMETHOD( SetFocus )( BOOL tFocus );
   END_INTERFACE_PART( OleInPlaceSiteWindowless );

   DECLARE_INTERFACE_MAP()

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);

	friend class CCtrlObj;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXDEVITEM_H__972EC672_E6D3_11D4_8C60_5254AB20ED22__INCLUDED_)
